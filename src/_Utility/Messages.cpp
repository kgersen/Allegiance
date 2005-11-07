/* Messages.cpp
   Interface between WinTrek and FedSrv
   Owner: 
*/

#include "pch.h"
#include "MessageCore.h"

const MsgClsPrio FedMessaging::c_mcpDefault = 1000;
static GUID g_guidApplication;

static DPID            g_dpidFrom = 0;
static bool            g_fHandlingMsg = false;
static CFMConnection * g_pcnxnFrom = NULL;
static LPBYTE          g_pbReceiveBuff = NULL;
static DWORD           g_dwMsgPacketSize = 0;


// Ok, this is kinda hacky, but it's too late to make wide-effect changes. The problem is that while in the OnAppMessage
// callback, they may delete the connection from which the message came. Then there may be another FEDMEESSAGE packed in 
// the dplay message. In order to process any FEDMESSAGE, we need the pfsPlayer, which is no longer valid if we've deleted
// the connection, so we need to set a flag when a connection is deleted, so we know not to process any more FEDMESSAGEs
// that might be part of the same packet. Note that this does not apply to other messages in different packets (dplay 
// messages), because in that case the private data for the player will be NULL, and we won't get the pfsPlayer
static bool g_fConnectionDeleted = false;

void DumpMemory()
{
  _CrtDumpMemoryLeaks();
}

void whodidit()
{
  if (g_fHandlingMsg)
  {
    CB cb = ((FEDMESSAGE*)(g_pbReceiveBuff))->cbmsg;
    FEDMSGID id = ((FEDMESSAGE*)(g_pbReceiveBuff))->fmid;
    debugf("Currently handling message from %s(%u).\n"
           "cbmsg=%d, fmid=%d, total packet size=%d\n", 
           g_pcnxnFrom ? g_pcnxnFrom->GetName() : "<unknown>", g_dpidFrom,
           cb, id, g_dwMsgPacketSize);
  }
  else
    debugf("Not currently handling a message.\n");
}


/*-------------------------------------------------------------------------
 * EnumAddressCallback
 *-------------------------------------------------------------------------
 * Purpose:
 *     Silly dplay mechanism to get all the parts of an address
 * 
 * Parameters:
 *     DPlay defined
 * 
 * Returns:
 *     Whether to continue enumerating address parts
 */
BOOL FAR PASCAL EnumAddressCallback(REFGUID guidDataType,
                                    DWORD dwDataSize,
                                    LPCVOID lpData,
                                    LPVOID lpContext)
{
  // What the hell? Why do I get so many parts, and not even of the known guid types??? Oh well, jump through the hoops
  if (DPAID_INet == guidDataType)
  {
    assert(dwDataSize > (DWORD) lstrlen((char *) lpData));
    lstrcpy((char *) lpContext, (char *) lpData);
    return FALSE;
  }
  return TRUE;
}


/*-------------------------------------------------------------------------
 * VerifyMessages
 *-------------------------------------------------------------------------
  Purpose:
    Make sure that our output buffer adheres to the expected format of a 
    string of valid FEDMESSAGEs.
  
  Side Effects:
    Important: The default recipient no longer remains the default if 
               SendMessages is called with a non-NULL precip
 */
void VerifyMessage(LPBYTE pb, CB cbTotal)
{
#ifndef NO_MSG_CRC
  // <sigh> we can't do this as long as we have the hack to support non-crc clients, since we munge the message length on ack from logon to lobby
#else  
  int cMsgs = 0;
  CB cbRunning = 0;
  while (cbRunning < cbTotal)
  {
    FEDMESSAGE * pfm = (FEDMESSAGE *) pb;
    assert(pfm->cbmsg > 0);
    assert(pfm->fmid > 0);
    cbRunning += pfm->cbmsg;
    pb += pfm->cbmsg;
    cMsgs++;
  }
  assert(cbRunning == cbTotal);
#endif
}


/*-------------------------------------------------------------------------
 * EnumSessionsCallback
 *-------------------------------------------------------------------------
 * Purpose:
 *    Called by IDirectPlay#::EnumSessions when doing a broadcast EnumSessions
 * 
 * Parameters:
 *    specified by dplay
 */
BOOL FAR PASCAL EnumSessionsCallback (LPCDPSESSIONDESC2 lpThisSD,
                                      LPDWORD lpdwTimeOut,
                                      DWORD dwFlags,
                                      LPVOID lpContext)
{
  if( dwFlags & DPESC_TIMEDOUT )
      return FALSE; // The enumeration has timed out, so stop the enumeration.

  assert(lpThisSD);
  // lpContext is a FedMessaging*
#ifdef DEBUG  
  char szBuff[128];
  wsprintf(szBuff, "Found a session: {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x} for application {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}\n", 
          lpThisSD->guidInstance.Data1,    lpThisSD->guidInstance.Data2, 
          lpThisSD->guidInstance.Data3,    lpThisSD->guidInstance.Data4[0], 
          lpThisSD->guidInstance.Data4[1], lpThisSD->guidInstance.Data4[2], 
          lpThisSD->guidInstance.Data4[3], lpThisSD->guidInstance.Data4[4], 
          lpThisSD->guidInstance.Data4[5], lpThisSD->guidInstance.Data4[6], 
          lpThisSD->guidInstance.Data4[7],
          lpThisSD->guidApplication.Data1,    lpThisSD->guidApplication.Data2, 
          lpThisSD->guidApplication.Data3,    lpThisSD->guidApplication.Data4[0], 
          lpThisSD->guidApplication.Data4[1], lpThisSD->guidApplication.Data4[2], 
          lpThisSD->guidApplication.Data4[3], lpThisSD->guidApplication.Data4[4], 
          lpThisSD->guidApplication.Data4[5], lpThisSD->guidApplication.Data4[6], 
          lpThisSD->guidApplication.Data4[7]);
  debugf(szBuff);
#endif  
  if (lpThisSD->guidApplication == g_guidApplication)
  {
    FedMessaging * pfm = (FedMessaging *)lpContext;
    FMSessionDesc fmSession(const_cast<LPDPSESSIONDESC2>(lpThisSD));
    pfm->m_guidInstance = lpThisSD->guidInstance;
    if (pfm->m_fSessionCallback)
      pfm->m_pfmSite->OnSessionFound(pfm, &fmSession);
  }
  return TRUE;
}



FedMessaging::FedMessaging(IFedMessagingSite * pfmSite) :
  m_pDirectPlay(NULL),
  m_pDirectPlayLobby(NULL),
  m_cMsgsOdometer(0),
  m_cBytesOdometer(0),
  m_pbFMNext(m_rgbbuffOutPacket),
  m_pfmSite(pfmSite),
  m_fConnected(false),
  m_precipDefault(NULL),
  m_fmGuaranteedDefault(FM_GUARANTEED),
  m_pcnxnServer(NULL),
  m_pgrpEveryone(NULL),
  m_pcnxnMe(NULL),
  m_fSecondaryOut(false),
  m_pbFMNextT(m_rgbbuffSecondaryOutPacket),
  m_guidInstance(GUID_NULL),
  m_guidApplication(GUID_NULL),
  m_timeMsgLast(Time::Now()),
  m_priority(c_mcpDefault)
{
  assert (pfmSite);
}


FedMessaging::~FedMessaging()
{
  Shutdown();
}


/*-------------------------------------------------------------------------
 * SetDefaultRecipient
 *-------------------------------------------------------------------------
  Purpose:
      Set default recipient, to be used if the out buffer overflows, or 
      NULL recipient is specified in SendMessages

      Important: The default recipient no longer remains the default if 
                 SendMessages is called with a NULL precip
 */
void FedMessaging::SetDefaultRecipient(CFMRecipient * precip, FMGuaranteed fmg)
{
  assert(0 == CbUsedSpaceInOutbox()); // should call this BEFORE you queue any messages
  m_precipDefault = precip;
  m_fmGuaranteedDefault = fmg;
}


/*-------------------------------------------------------------------------
 * GetDefaultRecipient
 *-------------------------------------------------------------------------
  Purpose:
      Get default recipient, and optionally other details about default recipient. 
      See SetDefaultRecipient

  Parameters:
      pfGuaranteedDefault:  output parameter to get guaranteed flag, or NULL if don't care
 */
CFMRecipient * FedMessaging::GetDefaultRecipient(FMGuaranteed * pfmg)
{
  if (pfmg)
    *pfmg = m_fmGuaranteedDefault;
  return m_precipDefault;
}


/*-------------------------------------------------------------------------
 * PFedMsgCreate
 *-------------------------------------------------------------------------
 * Purpose:
 *     Generate a FedMsg of specified type, and construct all variable-length parms
 * 
 * Parameters:
 *     pfm - Where to create the message (non-NULL). If NULL, memory is allocated for message
 *           Ignored if fQueueMsg==true
 * Returns:
 *     pointer to newly created message
 * 
 * Side Effects:
 *     Queues up the message if fQueueMsg is set, otherwise allocates new buffer for msg
 */
void * FedMessaging::PFedMsgCreate(bool fQueueMsg, BYTE * pbFMBuff, FEDMSGID fmid, CB cbfm, ...)
{
  va_list vl;
  BYTE * pBlobSrc, * pBlobDst; 
  BYTE * pbFM;

  assert (IsConnected());

StartOver: // if we overrun our buffer, need to start over after flushing it
  va_start(vl, cbfm); // make sure we have the last required parm

  if (fQueueMsg && (cbfm > CbFreeSpaceInOutbox()))
    SendToDefault(FM_FLUSH);
    
  //figure out where to compose this message
  pbFM =  fQueueMsg ? m_pbFMNext :
         (pbFMBuff  ? pbFMBuff : m_rgbbuffAlloc);

  ((FEDMESSAGE*)pbFM)->fmid = fmid;

  pBlobDst = pbFM + cbfm; // where we start putting the blobs
  IB * pib = (IB*)(pbFM + sizeof(FEDMESSAGE)); // relies on ib/cb pairs starting immediately after fm
  CB * pcb;
  CB cbBlob;

  while (FM_END_VAR_PARMS != (pBlobSrc = va_arg(vl, BYTE *)))
  {
    pcb = pib + 1;
    *pib = pBlobDst - pbFM;
    *pcb = 0;
    BYTE * pbLastNonSpace; // we trim the trailing whitespace too as a free added bonus
    cbBlob = va_arg(vl, CB);
    if (CB_ZTS == cbBlob)
    {
      if (pBlobSrc) // we have something for this parm (as opposed to an intentional NULL)
      {
        if (*pBlobSrc) // pointer to non-NULL string
        {
          int cbLen = lstrlen((char *)(pBlobSrc));
          pbLastNonSpace = pBlobSrc + cbLen - 1;
          while (cbLen > 0 && ' ' == *pbLastNonSpace)
          {
            pbLastNonSpace--;
            cbLen--;
          }

          if (fQueueMsg && (pBlobDst + cbLen > BuffOut() + GetBuffOutSize()))
          {
            // Need to send what was cued up, then move the current message (portion so far) to the head of the cue
            SendToDefault(FM_FLUSH);
            goto StartOver;
          }

          CopyMemory(pBlobDst, pBlobSrc, cbLen);
          *(pBlobDst + cbLen++) = 0;
          pBlobDst += cbLen;
          *pcb = cbLen;
        }
      }
    }
    else // length supplied
    {
      if (fQueueMsg && (pBlobDst + cbBlob > BuffOut() + GetBuffOutSize()))
      {
        // Need to send what was cued up, then move the current message (portion so far) to the head of the cue
        SendToDefault(FM_FLUSH);
        goto StartOver;
      }
      // if non-NULL pointer, then copy, otherwise, just reserve the space
      if (pBlobSrc)
        CopyMemory(pBlobDst, pBlobSrc, cbBlob);
      pBlobDst += cbBlob;
      *pcb = cbBlob;
    }
    
    pib += 2; // advance to next ib/cb pair
  } // while more blobs left
  va_end(vl);
  
  ((FEDMESSAGE *)pbFM)->cbmsg = pBlobDst - pbFM;
  if (fQueueMsg || !pbFMBuff) // if we're copying to pre-alloced pfm, then we're done
  {
    if (fQueueMsg)
    {
      m_pbFMNext = pBlobDst;
      // check for overrun buffer--relies on unsigned CB, otherwise check for free space >= 0
      assert(CbFreeSpaceInOutbox() <= GetBuffOutSize()); 
    }
    else // need to allocate space
    {
      // allocate a new blob to return the message in
      assert (((FEDMESSAGE *)pbFM)->cbmsg <= sizeof(m_rgbbuffAlloc));
      BYTE * pbMsg = (BYTE *) GlobalAllocPtr(GMEM_MOVEABLE, ((FEDMESSAGE *)pbFM)->cbmsg);
      CopyMemory(pbMsg, pbFM, ((FEDMESSAGE *)pbFM)->cbmsg);
      pbFM = pbMsg;
    }
  }
  assert(((FEDMESSAGE*)pbFM)->cbmsg > 0 && ((FEDMESSAGE*)pbFM)->fmid > 0);
  return pbFM;
}


/*-------------------------------------------------------------------------
 * PfmGetNext
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get message after some provided message
 * 
 * Parameters:
 *    a message
 */
FEDMESSAGE * FedMessaging::PfmGetNext(FEDMESSAGE * pfm)
{
  BYTE * pfmNext = (BYTE*)pfm + pfm->cbmsg;
  assert(pfmNext >= BuffIn() && (BYTE*) pfm < (BuffIn()) + sizeof(m_rgbbuffInPacket));
  if (pfmNext - BuffIn() >= (int) PacketSize()) // no more messages in packet
  {
    assert((BYTE*)pfmNext == BuffIn() + PacketSize()); // should be exactly equal
    return NULL;
  }
  // Assert(!IsBadReadPointer(pfmNext, pfmNext->fm.cbfm))
  return (FEDMESSAGE *)pfmNext;
}


/*-------------------------------------------------------------------------
 * QueueExistingMsg
 *-------------------------------------------------------------------------
 * Purpose:
 *    Take a message that already exists, and queue it up for the outgoing packet
 * 
 * Parameters:
 *    pfm: the pre-existing, properly setup message
 */
void FedMessaging::QueueExistingMsg(const FEDMESSAGE * pfm)
{
  assert(m_fConnected);
  assert(pfm->cbmsg > 0);
  assert(pfm->fmid > 0);
  if (CbFreeSpaceInOutbox() < pfm->cbmsg) // overrun buffer--send it if we have a default recipient
  {
    assert(m_precipDefault);
    SendToDefault(FM_FLUSH);
  }
  CopyMemory(m_pbFMNext, pfm, pfm->cbmsg);
  m_pbFMNext += pfm->cbmsg;
  assert(CbFreeSpaceInOutbox() <= GetBuffOutSize()); 
}


/*-------------------------------------------------------------------------
 * GenericSend
 *-------------------------------------------------------------------------
 * Purpose:
 *    All messages go through this. Not used by FedMessaging users directly. Use SendMessages or ForwardMessage.
 */
HRESULT FedMessaging::GenericSend(CFMRecipient * precip, const void * pv, CB cb, FMGuaranteed fmg)
{
  DWORD dwFlags, dwPriority, dwTimeout;
  HRESULT hr;

  //
  // Give guaranteed messages a higher priority, and longer ttl
  //
  bool fGuaranteed = FM_GUARANTEED == fmg;
  dwFlags = DPSEND_ASYNC | (fGuaranteed ?
              DPSEND_GUARANTEED : DPSEND_NOSENDCOMPLETEMSG);
  dwPriority = m_priority;
  if (fGuaranteed)
    dwPriority += GetGuaranteedBias();
  
  dwTimeout = (fGuaranteed ? 0 : 500); // guaranteed messages will NEVER time out

#ifdef DUMPMSGS
  debugf("*** Sending message to %u, flags = 0x%x, Priority = %d, "
          "Timeout = %d, length = %d, thread = 0x%x\n", precip->GetDPID(), dwFlags,
          dwPriority, dwTimeout, cbBuffer, GetCurrentThreadId());
#endif

#ifndef NO_MSG_CRC
    // add the crc to the end to minimize possible conflict of assumptions
    // We have to use our own buffer, so as to not overrun any input buffers ** :-( ** 
    char crcbuff[sizeof(m_rgbbuffOutPacket)];
    CopyMemory(crcbuff, pv, cb);
    int crc = MemoryCRC(crcbuff, cb);
    *(int*)(crcbuff + cb) = crc;
    pv = crcbuff;
    cb += sizeof(crc);
#endif

static CTempTimer tt("in dplay SendEx", 0.05f);
  tt.Start();
  hr = m_pDirectPlay->SendEx(m_pcnxnMe->GetID(), precip->GetID(), dwFlags, (void*) pv, cb, 
                          dwPriority, dwTimeout, precip, NULL);
  tt.Stop("dpidTo=%u, guaranteed=%d, hr=%x, 1st message (fmid)=%u", precip->GetID(), fGuaranteed, hr, 
          ((FEDMESSAGE*)pv)->fmid);

  m_pfmSite->OnMessageSent(this, precip, pv, cb, fmg);
  assert(hr != E_INVALIDARG);
#ifdef DEBUG
  if (!(hr == E_PENDING || SUCCEEDED(hr)))
  {
    debugf("Return code of SendEx was 0x%x to player with dpid %u.\n", 
           hr, precip->GetID());
  }
#endif // DEBUG

  return(hr);
}


/*-------------------------------------------------------------------------
 * SendToDefault
 *-------------------------------------------------------------------------
  Purpose:
      Send messages to default recipient

  Returns:
      Same as SendMessages

 */
int FedMessaging::SendToDefault(FMFlush fmf)
{
  return SendMessages(NULL, m_fmGuaranteedDefault, fmf);
}


/*-------------------------------------------------------------------------
 * SendMessages
 *-------------------------------------------------------------------------
  Purpose:
    Send queued-up messages
  
  Parameters:
    pvContext:  This is passed back to us when dp tell us it couldn't 
        deliver a message that we were tracking. FedSrv uses this as a pFSCon,
        and the client doesn't use it (yet).
    fflush: Whether you want to flush the queued message buffer
  
  Returns:
    bytes sent
  
  Side Effects:
    Important: The default recipient no longer remains the default if 
               SendMessages is called with a non-NULL precip
 */
int FedMessaging::SendMessages(CFMRecipient * precip, FMGuaranteed fmg, FMFlush fmf)
{
  assert(m_fConnected);
  // if they don't supply a recip, then they better have previously supplied one
  assert (precip || m_precipDefault);
  if (precip)
  {
    m_precipDefault = NULL;
  }
  else
  {
    precip = m_precipDefault;
    fmg    = m_fmGuaranteedDefault;
  }
  HRESULT hr;
  hr = S_OK;
  int cbToSend = CbUsedSpaceInOutbox();
  assert(cbToSend <= GetBuffOutSize());

  if (cbToSend > 0)
  {
    VerifyMessage(BuffOut(), cbToSend);
    hr = GenericSend(precip, BuffOut(), cbToSend, fmg);

    m_cMsgsOdometer++;
    m_cBytesOdometer += cbToSend;

    if (FM_FLUSH == fmf)
      m_pbFMNext = BuffOut();
  }
  return cbToSend;
}


/*-------------------------------------------------------------------------
 * CheckOdometer
 *-------------------------------------------------------------------------
 * Purpose:
 *    Tells you:
 *        How long it's been, 
 *        How many messages have been sent, and
 *        How many bytes have been sent
 *    since you've last called CheckOdometer. So obviously the first time you 
 *    call it you can ignore the return values
 * 
 * Parameters:    The return values
 * 
 * Side Effects:  Resets the odometer
 *
 * Returns:       Current time
 */
Time FedMessaging::CheckOdometer(float& flDTime, int& cMsgsOdometer, int& cBytesOdometer)
{
  Time timeNow    = Time::Now ();
  flDTime         = timeNow - m_timeOdometerStart;
  cMsgsOdometer   = m_cMsgsOdometer;
  cBytesOdometer  = m_cBytesOdometer;

  m_timeOdometerStart = timeNow;
  m_cMsgsOdometer     = 0;
  m_cBytesOdometer    = 0;
  return timeNow;
}


/*-------------------------------------------------------------------------
 * HandleSysMsg
 *-------------------------------------------------------------------------
 * Purpose:
 *    Handle system message
 * 
 * Parameters:
 *    System messages are those that dplay send us. They're handled here
 * 
 * Side Effects:
 *    Various
 */
HRESULT FedMessaging::OnSysMessage(FedMessaging * pthis, LPDPMSG_GENERIC pMsg)
{
  // The body of each case is there so you can set a breakpoint and examine
  // the contents of the message received.

  static CTempTimer timerSysMsg("spent in OnSysMessage()", .01f);
  timerSysMsg.Start();
  Time timeNow = Time::Now();

  m_pfmSite->OnSysMessage(this); // just for tracking
    
  switch (pMsg->dwType)
  {
    case DPSYS_CREATEPLAYERORGROUP:
    {
      static CTempTimer tt("handling DPSYS_CREATEPLAYERORGROUP", .01f);
      tt.Start();
      LPDPMSG_CREATEPLAYERORGROUP lp = (LPDPMSG_CREATEPLAYERORGROUP) pMsg;
      bool fPlayer = lp->dwPlayerType == DPPLAYERTYPE_PLAYER;
      debugf("DPSYS_CREATEPLAYERORGROUP for %s %s(%u)\n", 
              fPlayer ? "player" : "group", lp->dpnName.lpszShortNameA, lp->dpId);

      // **CFMConnections** are created IN RESPONSE to getting DPSYS_CREATEPLAYERORGROUP.
      // But **CFMGroups** are created prior to, and result in, getting DPSYS_CREATEPLAYERORGROUP.
      if (fPlayer)
      {
        CFMConnection * pcnxn = CreateConnection(lp->dpnName.lpszShortNameA, lp->dpId);
        char szRemoteAddress[16];
        GetIPAddress(*pcnxn, szRemoteAddress);
        debugf(" ip=%s, which gives us %u players\n", szRemoteAddress, lp->dwCurrentPlayers);
      }
      else
      {
        assert(0 == lp->dpIdParent); // nesting groups is bad, so we don't support it
      }
      tt.Stop();
    }
    break;

    case DPSYS_DESTROYPLAYERORGROUP:
    {
      static CTempTimer tt("handling DPSYS_DESTROYPLAYERORGROUP", .01f);
      tt.Start();
      LPDPMSG_DESTROYPLAYERORGROUP lp = (LPDPMSG_DESTROYPLAYERORGROUP) pMsg;
      bool fPlayer = lp->dwPlayerType == DPPLAYERTYPE_PLAYER;
      debugf("DPSYS_DESTROYPLAYERORGROUP for %s %s(%u)\n", 
              fPlayer ? "player" : "group", lp->dpnName.lpszShortNameA, lp->dpId);

      if (fPlayer)
      {
        CFMConnection * pcnxn = GetConnectionFromDpid(lp->dpId);
        // if your player dies, or the server dies, you're SOL--not much point in 
        // waiting until dplay gets around to telling the session is dead,
        // since sometimes they don't anyway.
        if (pcnxn && (m_pcnxnMe == pcnxn || m_pcnxnServer == pcnxn))
        {
          m_pfmSite->OnSessionLost(this);
          Shutdown();
        }
        else if (pcnxn)
          DeleteConnection(*pcnxn);
        // else we've lost the above player, but they don't have an associated CFMConnection, probably because we manually nuked it
      }
      else
      {
        // we should've already nuked our CFMGroup
        // But there's a very small nonzero chance that dplay got around to nuking their group before we did
        //assert(!GetGroupFromDpid(lp->dpId));
      }
      tt.Stop();
    }
    break;

    case DPSYS_SENDCOMPLETE:
    {
      static CTempTimer tt("handling DPSYS_SENDCOMPLETE", .01f);
      tt.Start();
      LPDPMSG_SENDCOMPLETE lp = (LPDPMSG_SENDCOMPLETE) pMsg;

      bool fGroup = true;
      CFMRecipient * prcp = NULL;
      prcp = GetGroupFromDpid(lp->idTo);
      if (!prcp)
      {
        fGroup = false;
        CFMConnection * pcnxn = GetConnectionFromDpid(lp->idTo);
        if (pcnxn)
        {
          prcp = pcnxn;
          if (DP_OK == lp->hr)
            pcnxn->SetLastComplete(timeNow.clock());
        }
      }

      if (DP_OK != lp->hr)
      {

        debugf("DPSYS_SENDCOMPLETE: Message was not delivered to %s(%u), priority %d, "
                "hr=0x%x, SendTime=%d.\n",
                prcp ? prcp->GetName() : "<not found>", lp->idTo, lp->dwPriority, lp->hr, lp->dwSendTime);
        if (prcp) 
          m_pfmSite->OnMessageNAK(this, lp->dwSendTime, prcp);
      }
      tt.Stop();
    }
    break;

    case DPSYS_ADDPLAYERTOGROUP:
    {
      LPDPMSG_ADDPLAYERTOGROUP lp = (LPDPMSG_ADDPLAYERTOGROUP) pMsg;
      CFMGroup * pgrp = GetGroupFromDpid(lp->dpIdGroup );
      CFMConnection * pcnxn = GetConnectionFromDpid(lp->dpIdPlayer);
      if (pgrp)
        pgrp->PlayerAdded(pcnxn);
      debugf("DPSYS_ADDPLAYERTOGROUP: player %s(%u) to group %s(%u)\n", 
          pcnxn ? pcnxn->GetName() : "<gone>", lp->dpIdPlayer,
          pgrp ? pgrp->GetName() : "<gone>", lp->dpIdGroup);
    }
    break;

    case DPSYS_DELETEPLAYERFROMGROUP:
    {
      LPDPMSG_DELETEPLAYERFROMGROUP lp = (LPDPMSG_DELETEPLAYERFROMGROUP) pMsg;
      CFMGroup * pgrp = GetGroupFromDpid(lp->dpIdGroup );
      CFMConnection * pcnxn = GetConnectionFromDpid(lp->dpIdPlayer);
      if (pgrp)
        pgrp->PlayerDeleted(pcnxn);
      debugf("DPSYS_DELETEPLAYERFROMGROUP: player %s(%u) from group %s(%u)\n", 
          pcnxn ? pcnxn->GetName() : "<gone>", lp->dpIdPlayer,
          pgrp ? pgrp->GetName() : "<gone>", lp->dpIdGroup);
    }
    break;

    case DPSYS_SESSIONLOST:
    {
      m_pfmSite->OnSessionLost(this);
    }
    break;

    default:
    {
        debugf("***** Unknown system message, type %lu *****\n", pMsg->dwType);
    }
  }

  timerSysMsg.Stop();

  return(S_OK);
}

/*-------------------------------------------------------------------------
 * ReceiveMessages
 *-------------------------------------------------------------------------
 * Purpose:
 *    Handles all inbound messages and dispatches them appropriately
 */
HRESULT FedMessaging::ReceiveMessages()
{
  HRESULT hr = S_OK;
  DWORD   dwMsgBufferSize = 0;
  DPID dpidFrom, dpidTo;
  assert(!g_fHandlingMsg);
  g_fHandlingMsg = true;

  // loop to read all messages in queue
  while (SUCCEEDED(hr) && IsConnected())
  {
    // read messages from any player, including system player
    dpidFrom = 0;
    dpidTo = 0;

    m_dwcbPacket = sizeof(m_rgbbuffInPacket); // NOT PacketSize()!
    g_pbReceiveBuff = m_rgbbuffInPacket;
    static CTempTimer ttReceive("spent in m_pDirectPlay->Receive()", .01f);
    ttReceive.Start();
    hr = m_pDirectPlay->Receive(&dpidFrom, &dpidTo, DPRECEIVE_ALL, 
                                m_rgbbuffInPacket, &m_dwcbPacket);
    ttReceive.Stop();
    g_dpidFrom = dpidFrom;
    g_dwMsgPacketSize = m_dwcbPacket;
    assert(IMPLIES(FAILED(hr), DPERR_NOMESSAGES == hr)); // this is the only error return code we expect

    if ((SUCCEEDED(hr)) && // successfully read a message
        m_dwcbPacket >= sizeof(DPMSG_GENERIC))  // it is big enough
    {
        /*
        {
            static Time     timeStart;
            static DWORD    cbTotal;
            static DWORD    cMessagesTotal;
            static bool     bRunning = false;
            Time    now = Time::Now();
            if (bRunning)
            {
                float   dt = now - timeStart;
                if (dt >= 5.0f)
                {
                    debugf("Messages @ %d = %f bytes/second; %f total bytes/second; %f packets/second\n",
                           now.clock(), float(cbTotal) / dt, float(cbTotal + cMessagesTotal * 55) / dt, float(cMessagesTotal) / dt);
                    timeStart = now;
                    cbTotal = PacketSize();
                    cMessagesTotal = 1;
                }
                else
                {
                    cbTotal += PacketSize();
                    cMessagesTotal++;
                }
            }
            else
            {
                timeStart = now;
                cbTotal = 0;
                cMessagesTotal = 0;
                bRunning = true;
            }
        }
        */

      // check for system message
      if (dpidFrom == DPID_SYSMSG)
        OnSysMessage(this, (LPDPMSG_GENERIC) m_rgbbuffInPacket);
      else
      {
        CFMConnection * pcnxnFrom = NULL;
        DWORD dwSize = sizeof(CFMConnection *);
        hr = m_pDirectPlay->GetPlayerData(dpidFrom, &pcnxnFrom, &dwSize, DPGET_LOCAL);
        // debugf("Called GetPlayerData: hr=0x%08x, pcnxnFrom=0x%08x, dwSize=%u\n", hr, pcnxnFrom, dwSize);
        g_pcnxnFrom = pcnxnFrom;
        if (pcnxnFrom && SUCCEEDED(hr)) // if we can't resolve the player, then it's into the bit bucket with this message
        {
          FEDMESSAGE * pfm = (FEDMESSAGE *) m_rgbbuffInPacket;
          int cMsgs = 0;
#ifndef NO_MSG_CRC
          int crc = MemoryCRC(m_rgbbuffInPacket, PacketSize());
          if (crc == *(int*)(m_rgbbuffInPacket + PacketSize()))
          {
#endif            
            static CTempTimer tt("spent looping through message queue", .1f);
            tt.Start();
            g_fConnectionDeleted = false;
            while (pfm && !g_fConnectionDeleted)
            {
              if(pfm->fmid > 0 && pfm->cbmsg >= sizeof(FEDMESSAGE) && pfm->cbmsg <= PacketSize())
              {
                m_pfmSite->OnAppMessage(this, *pcnxnFrom, pfm);
                pfm = PfmGetNext(pfm);
                cMsgs++;
              }
              else
              {
                // ACK! Hacker?
                whodidit();
#ifndef NO_MSG_CRC
                // we'll just call it a bad crc, even though it's not quite the same thing
                m_pfmSite->OnBadCRC(this, *pcnxnFrom, m_rgbbuffInPacket, m_dwcbPacket);
#endif
                break;
              }
            }
            tt.Stop();
#ifndef NO_MSG_CRC          
          }
          else
          {
            // ACK! Failed CRC. WTF??? ...<sigh> but it happens
            m_pfmSite->OnBadCRC(this, *pcnxnFrom, m_rgbbuffInPacket, m_dwcbPacket);
          }
#endif          
        }
        else
        {
          debugf("Warning: Ignoring message from dpid %u who doesn't have an associated CFMConnection\n", dpidFrom);
          assert(DPERR_INVALIDPLAYER == hr);
        }
        m_timeMsgLast = Time::Now();
      }
    }
    else
      assert (DPERR_NOMESSAGES == hr || !IsConnected());
  }
  
  g_dpidFrom = 0;
  g_pcnxnFrom = NULL;
  g_fHandlingMsg = false;

  return (DP_OK);
}


/*-------------------------------------------------------------------------
 * ConnectToDPAddress
 *-------------------------------------------------------------------------
 * Purpose:
 *    Connect to a machine, given a dplay address
 */
HRESULT FedMessaging::ConnectToDPAddress(LPVOID pvAddress)
{
  HRESULT hr = m_pDirectPlay->InitializeConnection(pvAddress, 0);
  if (FAILED(hr))
  {
    if (DPERR_ALREADYINITIALIZED == hr)
      hr = S_OK;
    else
      m_pfmSite->OnMessageBox(this, "Failed to connect to server. Please check network connection.",
                            "Allegiance", MB_OK);
  }
  return hr;
}


/*-------------------------------------------------------------------------
 * Connect
 *-------------------------------------------------------------------------
 * Purpose:
 *    Connect to a host machine. Could be self.
 * 
 * Parameters:
 *    string address of host
 */
HRESULT FedMessaging::Connect(const char * szAddress)
{
  assert(m_pDirectPlay);
  assert(!m_fConnected);
  void * pvAddress = NULL;
  DWORD dwAddressSize;
  HRESULT hr = E_FAIL;
  // Find out how big the address buffer needs to be--intentional fail 1st time
  hr = m_pDirectPlayLobby->CreateAddress(DPSPGUID_TCPIP, DPAID_INet, szAddress, 
          lstrlen(szAddress) + 1, pvAddress, &dwAddressSize);
  assert(DPERR_BUFFERTOOSMALL == hr);
    
  pvAddress = new char[dwAddressSize];
  ZSucceeded(m_pDirectPlayLobby->CreateAddress(DPSPGUID_TCPIP, DPAID_INet, szAddress, 
          lstrlen(szAddress) + 1, pvAddress, &dwAddressSize));
  hr = ConnectToDPAddress(pvAddress);
  delete [] pvAddress;
  
  return hr;
}


/*-------------------------------------------------------------------------
 * KillSvr
 *-------------------------------------------------------------------------
   Purpose:
      Kill dplaysvr

   Returns:
      Whether it was sucessfully killed (or if it wasn't running in the first place)
   
   Side Effects:
      Any running dplay server apps will be killed. This should never be 
      true if any other dplay server app is running
 */
bool FedMessaging::KillSvr()
{
  PROCESS_INFORMATION pi;
  STARTUPINFO si;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si); 

  Shutdown(); // can't be doing this with an open session
  
  BOOL fCreated = CreateProcess(NULL, "KillSvr.exe", NULL, NULL, FALSE, 
            CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

  if (fCreated)
  {
    // Now we need to make sure it finishes before we go start using dplaysvr again
    // WARNING: We could hang if killsvr never comes back. Maybe just abort if it doesn't come back in a few seconds?
    DWORD exitcode = STILL_ACTIVE;
    
    int i = 0;
    const int cRetries = 50; // let's give it 50 retries
    while (STILL_ACTIVE == exitcode && i > cRetries) 
    {
      GetExitCodeProcess(pi.hProcess, &exitcode);
      Sleep(100);
      i++;
    }
    if (i > cRetries)
      fCreated = false;
    else
      debugf("Slept (100ms) %d times  waiting for killsvr\n", i);
    Sleep(1000); // wait another second to make sure it's really gone
  }
  else
  {
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  (LPTSTR) &lpMsgBuf,
                  0,
                  NULL 
                  );
    m_pfmSite->OnMessageBox(this, (LPCTSTR)lpMsgBuf, "Couldn't run killsvr.exe.", MB_OK | MB_ICONINFORMATION);
    LocalFree( lpMsgBuf );
  }
 
  return !!fCreated;
}


/*-------------------------------------------------------------------------
 * InitDPlay
 *-------------------------------------------------------------------------
 * Purpose:
 *    Must be called before you can do anything with dplay
 * 
 * Side Effects:
 *    Creates a dplay object m_pDirectPlay, that must later be closed, 
 *        released, and set to NULL.
 */
HRESULT FedMessaging::InitDPlay(
#ifdef MONOLITHIC_DPLAY
    bool fMonolithic
#endif
)
{
  HRESULT hr = E_FAIL;
  assert(!m_fConnected);
  assert(!m_pDirectPlay);

  // Create an IDirectPlay interface
  m_pfmSite->OnPreCreate(this);
  IDirectPlayX* pdp = NULL;

  hr = CoCreateInstance(
#ifdef MONOLITHIC_DPLAY
      fMonolithic ? MSRG_CLSID_DPLAY :
#endif
      CLSID_DirectPlay, NULL, CLSCTX_INPROC_SERVER, IID_IDirectPlayX, (void**)&pdp);

  m_pfmSite->OnPostCreate(this, pdp, &m_pDirectPlay);
  if (FAILED(hr))
  {
    assert(!pdp);
    m_pfmSite->OnMessageBox(this, "Failed to initialze DirectPlay in DirectX 6. Check installation of DirectX, and reinstall if necessary.",
                            "Allegiance", MB_OK);
    return hr;
  }

  if (!CheckVersion())
  {
    Shutdown();
    return E_FAIL;
  }

  IDirectPlayLobby * pDirectPlayLobby;
  ZSucceeded(DirectPlayLobbyCreate(NULL, &pDirectPlayLobby, NULL, NULL, 0));
  ZSucceeded(pDirectPlayLobby->QueryInterface(IID_IDirectPlayLobbyX, (LPVOID*) &m_pDirectPlayLobby)); 
  pDirectPlayLobby->Release();
  pDirectPlayLobby = NULL;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * HostSession
 *-------------------------------------------------------------------------
 * Purpose:
 *    Start a session and be the host (server)
 * 
 * Parameters:
 *    guidApplication: Clients will only find this session if they ask for the same application
 *    fKeepAlive:      Should dplay detect timeouts and kill players on dead connections?
 *    hEventServer:    Event to be signaled when a message comes to the server
 *    fKillSvr:        Whether to kill the dplaysvr process. 
 * 
 * Returns:
 *    
 * 
 * Side Effects:
 *    
 */
HRESULT FedMessaging::HostSession(GUID guidApplication, bool fKeepAlive, HANDLE hEventServer, bool fProtocol
#ifdef MONOLITHIC_DPLAY
                                  , bool fMonolithic
#endif
                                  )
{
  HRESULT hr = E_FAIL;
  void *           pvAddress = NULL; // dplay address of self
  // DPSECURITYDESC   securityDesc;
  DPSESSIONDESC2   sessionDesc;
  bool             fSecure = false;
  DPSECURITYDESC * pSecurityDesc = NULL; // assume no security descriptor will be needed;
  char             szHostName[20]; // length of machine name--also long enough for ip address if that's what we get
  WSADATA          WSAData;

  assert(!m_fConnected);
  if (false) // TODO: How do we know when to make sure dplaysvr isn't hosed?
  {
    if (!KillSvr())
    {
      m_pfmSite->OnMessageBox(this, "Failed to successfully run killsvr.exe (or it failed to complete), which kills dplaysvr.exe. If we can't veryify that dplaysvr starts out clean, we're in for trouble, so I'm aborting.",
                            "Allegiance", MB_OK);
      return E_FAIL;
    }
  }
  
  if(!m_pDirectPlay)
  {
    hr = InitDPlay(
#ifdef MONOLITHIC_DPLAY
      fMonolithic
#endif      
    );
    if FAILED(hr)
      goto CLEANUP;
  }
  
  hr = E_FAIL;
  WSAStartup(MAKEWORD(1,1), &WSAData);
  gethostname(szHostName, sizeof(szHostName));
  WSACleanup();

  hr = Connect(szHostName);
  if (FAILED(hr))
    goto CLEANUP;

  // Start session
  ZeroMemory(&sessionDesc, sizeof(DPSESSIONDESC2));
  sessionDesc.dwSize = sizeof(DPSESSIONDESC2);
  sessionDesc.guidApplication = guidApplication;
  sessionDesc.dwMaxPlayers = 0; // no limit
  sessionDesc.lpszSessionNameA = "FedSrv";
#if !defined(DPSESSION_NOSESSSIONDESCMESSAGES)
  #define DPSESSION_NOSESSSIONDESCMESSAGES  0x00020000  // only available in dx8 and private Allegiance dplay bits
#endif
  sessionDesc.dwFlags = DPSESSION_CLIENTSERVER | 
                        DPSESSION_OPTIMIZELATENCY |
                        (fProtocol ? DPSESSION_DIRECTPLAYPROTOCOL : 0) | 
                        DPSESSION_NODATAMESSAGES |
#ifdef MONOLITHIC_DPLAY
                        (fMonolithic ? DPSESSION_NOSESSSIONDESCMESSAGES : 0) | 
#else
                        DPSESSION_NOSESSSIONDESCMESSAGES | 
#endif
                        (fKeepAlive ? DPSESSION_KEEPALIVE    : 0) |
                        (fSecure    ? DPSESSION_SECURESERVER : 0);

  m_guidApplication = guidApplication;

/*
  if (fSecure)
  {
    ZeroMemory(&securityDesc, sizeof(DPSECURITYDESC));
    securityDesc.dwSize = sizeof(DPSECURITYDESC);
    securityDesc.dwFlags = 0;
    securityDesc.lpszSSPIProviderA = "something";
    pSecurityDesc = &securityDesc;
  }
*/

  // host the session
  hr = m_pDirectPlay->Open(&sessionDesc, DPOPEN_CREATE);
  // Now, since we need to use a flag that was added as a bug fix, the version of dplay we run on may or may not have this flag.
  // The only way to know is to try using it, and if it fails with DPERR_INVALIDFLAGS, then we need to try again without it.
  if (DPERR_INVALIDFLAGS == hr)
  {
    sessionDesc.dwFlags &= ~DPSESSION_NOSESSSIONDESCMESSAGES;
    hr = m_pDirectPlay->Open(&sessionDesc, DPOPEN_CREATE);
  }

  ZSucceeded(hr);
  m_fConnected = true;

  // Now let's get the sessiondesc back to look at it
  {
    DPSESSIONDESC2 * psd = NULL;
    DWORD dwSize = 0;
    hr = m_pDirectPlay->GetSessionDesc(psd, &dwSize);
    assert (DPERR_BUFFERTOOSMALL  == hr);
    psd = (DPSESSIONDESC2 *) new char[dwSize];
    hr = m_pDirectPlay->GetSessionDesc(psd, &dwSize);
    m_guidInstance = psd->guidInstance;
    delete [] psd;
  }

  // create a server "player"
  DPID dpid;
  ZSucceeded(m_pDirectPlay->CreatePlayer(&dpid, NULL, hEventServer, 
                                     NULL, 0, DPPLAYER_SERVERPLAYER));
  assert(dpid == DPID_SERVERPLAYER);
  m_pcnxnMe = new CFMConnection(this, "<Server connection>", dpid);

  // then create the Everyone group
  ZVerify(m_pgrpEveryone = CreateGroup("Everyone"));

  ResetOutBuffer();

CLEANUP:
  if (FAILED(hr))
    Shutdown();
  
  return hr;
}


/*-------------------------------------------------------------------------
 * Shutdown
 *-------------------------------------------------------------------------
 * Purpose:
 *    Shutdown an open session. It is assumed that logoff, etc. has already been done
 */
void FedMessaging::Shutdown()
{
  static CTempTimer tt("In FedMessaging::Shutdown", 0.1f);
  tt.Start();
  if (m_pDirectPlay)
  {
    // empty the msg queue, and wait to make sure sys msgs get flushed
    m_pDirectPlay->CancelMessage(0, 0);
    DWORD dwQueueLen = 0;
    do
    {
      Sleep(100); // don't ask
      GetSendQueue(&dwQueueLen, NULL);
    } while (dwQueueLen > 0);
    
    if (m_pcnxnMe)
    {
      Sleep(100); // don't ask
      m_pDirectPlay->DestroyPlayer(m_pcnxnMe->GetDPID());
      delete m_pcnxnMe;
      m_pcnxnMe = NULL;
    }
    Sleep(1); // don't ask
    
    m_pDirectPlay->Close();
    m_pDirectPlay->Release();
    m_pDirectPlay = NULL;

  }

  if (m_pcnxnServer)
  {
    delete m_pcnxnServer;
    m_pcnxnServer = NULL;
  }
  ResetOutBuffer();
  m_guidInstance = GUID_NULL;
  m_fConnected  = false;
  tt.Stop();
}


/*-------------------------------------------------------------------------
 * JoinSession
 *-------------------------------------------------------------------------
 * Purpose:
 *    Become a client in a session
 * 
 * Parameters:
 *    what server, what application, and what instance to join (or NULL for default)
 *    OUT: connection to the sevrer that all messages should be sent to
 */
HRESULT FedMessaging::JoinSession(GUID guidApplication, const char * szServer, const char * szCharName)
{
  HRESULT hr = E_FAIL;
  Time timeStart = Time::Now();
  m_fSessionCallback = false; // we're going to join the session right here, so we don't want any site methods being called

  hr = EnumSessionsInternal(guidApplication, szServer);
  do
  {
    Sleep(500);
    hr = EnumSessionsInternal(guidApplication, NULL);
    if (DPERR_CONNECTING != hr && FAILED(hr))
      goto Cleanup;
    // Important implementation detail!!! dplay times out enum requests at 15 seconds, and bad things have been known to happen
    // if you try and kill a request, so to avoid that possibility, let's make SURE we wait longer than that.
  } while ((SUCCEEDED(hr) || DPERR_CONNECTING == hr) && 
           IsEqualGUID(GUID_NULL, m_guidInstance) && 
           (Time::Now().clock() - timeStart.clock() < 17000)); // don't try for more than 17 seconds

  if (IsEqualGUID(GUID_NULL, m_guidInstance))
    hr = E_FAIL; // something else?
  else
    hr = JoinSessionInstance(m_guidInstance, szCharName);

  ResetOutBuffer();

Cleanup:
  if (FAILED(hr))
    Shutdown();
  
  return hr;
}


/*-------------------------------------------------------------------------
 * FedMessaging.JoinSessionInstance
 *-------------------------------------------------------------------------
  Purpose:
      Join a session that has already been enum'd

  Parameters:
      session instance and name of player to create 

  Side Effects:
      joins the session, creates the named player
 */
HRESULT FedMessaging::JoinSessionInstance(GUID guidInstance, const char * szName)
{
  DPSESSIONDESC2 sessionDesc;
  static CTempTimer tt("in FedMessaging::JoinSessionInstance", 0.5f);
  tt.Start();
  ZeroMemory(&sessionDesc, sizeof(sessionDesc));
  sessionDesc.dwSize = sizeof(sessionDesc);
  sessionDesc.guidInstance = guidInstance;

  assert(m_pDirectPlay);
  // But just to be paranoid, since people will be using retail clients before this can get lots of testing...
  if (!m_pDirectPlay)
    return E_FAIL;
  
  HRESULT hr = m_pDirectPlay->Open(&sessionDesc, DPOPEN_JOIN);
  if (FAILED(hr))
    goto CLEANUP;

  m_guidInstance = sessionDesc.guidInstance;
  m_fConnected = true;
  DPNAME dpName;
  dpName.dwSize = sizeof(DPNAME);
  dpName.dwFlags = 0;
  dpName.lpszLongNameA = 
  dpName.lpszShortNameA = const_cast<char*>(szName);
  DPID dpid;
  ZSucceeded(hr = m_pDirectPlay->CreatePlayer(&dpid, &dpName, NULL, NULL, 0, 0));
  m_pcnxnMe = new CFMConnection(this, szName, dpid);

  // Set the output connection that the client will send msgs to
  m_pcnxnServer = new CFMConnection(this, "Server", DPID_SERVERPLAYER);

CLEANUP:
  tt.Stop();
  return hr;
}


/*-------------------------------------------------------------------------
 * CheckVersion
 *-------------------------------------------------------------------------
   Purpose:
      Verify that the version of dplay on the system is the one expected
          by other machines in the session

      There are a variety of acceptable versions of dplay. You pass the test if:
          you're using our custom branded dplay (1.2.3.12), or
          you're using the released dx6.1a on win9x, or
          you're using some minimum build of dx7 on win9x, or
          you're using some minimum build of w2k (save caveat as above)
   
   Returns:
      Whether version is correct
   
   Side Effects:
      Fire message event if version is wrong
 */
bool FedMessaging::CheckVersion()
{
    HANDLE hMem = NULL;         
    LPVOID lpvMem = NULL;         
    char  szFullPath[256];     
    DWORD dwVerHnd; 
    DWORD dwVerInfoSize;
    bool fRet = false;
    UINT dwBytes = 0; 
    VS_FIXEDFILEINFO *lpvsFixedFileInfo = NULL;

    // Get version information from the application 
    HMODULE hmodDPlay = GetModuleHandle("dplayx");
    if (NULL == hmodDPlay) // why isn't the dll loaded???
      goto Cleanup;

    GetModuleFileName(hmodDPlay, szFullPath, sizeof(szFullPath)); 
    if (0 == (dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd)))
      goto Cleanup;

    if (NULL == (hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize)) ||
        NULL == (lpvMem = GlobalLock(hMem)))
      goto Cleanup;

    GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);
    if (!VerQueryValue(lpvMem, "\\", (LPVOID*) &lpvsFixedFileInfo, &dwBytes))
      goto Cleanup;
    {
      WORD ver1 = HIWORD(lpvsFixedFileInfo->dwFileVersionMS);
      WORD ver2 = LOWORD(lpvsFixedFileInfo->dwFileVersionMS);
      WORD ver3 = HIWORD(lpvsFixedFileInfo->dwFileVersionLS);
      WORD ver4 = LOWORD(lpvsFixedFileInfo->dwFileVersionLS);

      char msg[256];
      
      sprintf(msg, "DPlay Version found: %d.%d.%d.%d. Version Required: 4.7.0.177 (DX7) or 5.0.2046.1 (Win2k) or 4.6.3.516 (DX6.1a)\r\n", 
        ver1, ver2, ver3, ver4);
      debugf(msg);

      if (ver1 > 5) // assume all major new versions work
        fRet = true;

      if (ver1 == 5) // NT -- WARNING -- if future versions of DX go outside 4.??? and spill into NT versions, we're in trouble
      {
        fRet = ver2 > 0 || ver3 >= 2113; // ver3 == NT build #
      }
      
      if (ver1 == 4) // Stand-alone DX runtimes
      {
        if (ver2 > 7) // new major dx versions (DX8+)
          fRet = true;

        if (ver2 == 7) // DX7.0a
          fRet = ver3 > 0 || ver4 >= 700;

        if (ver2 == 6) // DX6.1a
          fRet = (ver3 == 3 && ver4 >= 516) || ver3 > 3;

        // Anything before 6.1a is NOT sufficient.
      }

      if (!fRet)
      {
        m_pfmSite->OnMessageBox(this, msg, "Allegiance",  MB_OK);
        goto Cleanup;
      }
    }
        
Cleanup:

    if (lpvMem)
      GlobalUnlock(hMem);
    if (hMem)
      GlobalFree(hMem);
        
  return fRet;
}

CFMConnection * FedMessaging::GetConnectionFromDpid(DPID dpid)
{
  DWORD dwSize = sizeof(CFMConnection);
  CFMConnection * pcnxn = NULL;
  static CTempTimer tt("in GetConnectionFromDpid", .01f);
  tt.Start();
  HRESULT hr = GetDPlay()->GetPlayerData(dpid, &pcnxn, &dwSize, DPGET_LOCAL);
  if (hr == DPERR_INVALIDPLAYER) // dplay killed our player already! :-( See if we can find the dpid in our list
  {
    assert(!pcnxn);
    ListConnections::Iterator iter(m_listCnxns);
    for (; !pcnxn && !iter.End(); iter.Next()) 
    {
      if (iter.Value()->GetDPID() == dpid)
      {
        pcnxn = iter.Value();
        break;
      }
    }
    if (m_pcnxnMe && (m_pcnxnMe->GetDPID() == dpid))
      pcnxn = m_pcnxnMe;
    else if (m_pcnxnServer && (m_pcnxnServer->GetDPID() == dpid))
      pcnxn = m_pcnxnServer;
  }
  tt.Stop();
  return pcnxn;
}


CFMGroup * FedMessaging::GetGroupFromDpid(DPID dpid)
{
  DWORD dwSize = sizeof(CFMGroup);
  CFMGroup * pgrp = NULL;
  static CTempTimer tt("in GetGroupFromDpid", .01f);
  tt.Start();
  GetDPlay()->GetGroupData(dpid, &pgrp, &dwSize, DPGET_LOCAL);
  tt.Stop();
  return pgrp;
}


HRESULT FedMessaging::GetIPAddress(CFMConnection & cnxn, char szRemoteAddress[16])
{
  char rgcDPAddress[300];
  DWORD cbDPAddress = sizeof(rgcDPAddress);
  static CTempTimer tt("in GetIPAddress", .01f);
  tt.Start();
  HRESULT hr = GetDPlay()->GetPlayerAddress(cnxn.GetDPID(), rgcDPAddress, &cbDPAddress);
  if (SUCCEEDED(hr))
    hr = GetDPlayLobby()->EnumAddress(EnumAddressCallback, rgcDPAddress, cbDPAddress, szRemoteAddress);
  tt.Stop();
  return hr;
}



HRESULT FedMessaging::EnumSessions(GUID guidApplication, const char * szServer)
{
  m_fSessionCallback = true;
  
  return EnumSessionsInternal(guidApplication, szServer);
}


HRESULT FedMessaging::EnumSessionsInternal(GUID guidApplication, const char * szServer)
{
  HRESULT hr = E_FAIL;
  DWORD dwSize = 0;
  Time timeStart = Time::Now();
  static CTempTimer tt("in FedMessaging::EnumSessionsInternal", 1.0f);
  tt.Start();
  // restore the hourglass cursor
  SetCursor(LoadCursor(NULL, IDC_WAIT));

  if (szServer)
    Shutdown();

  if(!m_pDirectPlay)
  {
    hr = InitDPlay(
#ifdef MONOLITHIC_DPLAY
        false
#endif
    );
    if FAILED(hr)
      goto CLEANUP;
  }

  if (szServer)
  {
    hr = Connect(szServer);
    if FAILED(hr)
      goto CLEANUP;
  }
    
  // Find the session.
  DPSESSIONDESC2 sessionDesc;

  ZeroMemory(&sessionDesc, sizeof(sessionDesc));
  sessionDesc.dwSize = sizeof(sessionDesc);
  g_guidApplication = guidApplication;
  sessionDesc.guidApplication = guidApplication; //GUID_NULL;
  debugf("Looking for session on \"%s\"\n", szServer);

  if (szServer)
    hr = m_pDirectPlay->EnumSessions(&sessionDesc, 20000, EnumSessionsCallback, // resend every 3 seconds if we don't hear back
                                    this, DPENUMSESSIONS_STOPASYNC);

  hr = m_pDirectPlay->EnumSessions(&sessionDesc, 20000, EnumSessionsCallback, // resend every 3 seconds if we don't hear back
                                    this, DPENUMSESSIONS_ASYNC);
      
CLEANUP:
  tt.Stop(); // the shutdown below has its own timer
  if (DPERR_CONNECTING != hr && FAILED(hr))
    Shutdown();
  // else we need to keep the connection/sessions around for more enum'ing and joining later
  
  return hr;
}  

CFMConnection * FedMessaging::CreateConnection(const char * szName, DPID dpid) // after the physical connection has already been established
{
  static CTempTimer tt("in CreateConnection", .01f);
  tt.Start();
  CFMConnection * pcnxn = new CFMConnection(this, szName, dpid);
  m_listCnxns.PushFront(pcnxn);
  m_pgrpEveryone->AddConnection(this, pcnxn); // we'll let dplay own removing them when they disconnect, since they retain lifetime membership
  m_pfmSite->OnNewConnection(this, *pcnxn);
  tt.Stop();
  return pcnxn;
}


CFMConnection::CFMConnection(FedMessaging * pfm, const char * szName, DPID dpid) : // only FedMessaging::CreateConnection can create these things
    CFMRecipient(szName, dpid),
    m_cAbsentCount(0),
    m_dwPrivate(0)
{
  assert(GetDPID() == dpid);
  debugf("New CFMConnection: dpid=%u, name=%s, playerdata(this)=%p, private=%u\n", 
          dpid, szName, this, m_dwPrivate);
  LPVOID pv = this;
  if (FAILED((pfm->GetDPlay()->SetPlayerData(GetDPID(), &pv, sizeof(pv), DPSET_LOCAL))))
    debugf("Player %s(%u) gone immediately after connection. He should drop out rsn.\n", szName, dpid);
}


void CFMConnection::Delete(FedMessaging * pfm) // basically the destructor, but with a parameter
{
  debugf("Deleting connection: dpid=%u, name=%s, playerdata=%p, private=%u\n", 
          GetDPID(), GetName(), this, m_dwPrivate);
  // Don't want to hear anything more from this player
  static CTempTimer tt("in DestroyPlayer/SetPlayerData", 0.02f);
  tt.Start();
  HRESULT hr = pfm->GetDPlay()->SetPlayerData(GetDPID(), NULL, 0, DPSET_LOCAL); // just in case dplay hasn't really killed the player yet. ignore return code; they may already be dead
  debugf("SetPlayerData=0x%08x\n", hr);
  hr = pfm->GetDPlay()->DestroyPlayer(GetDPID());
  debugf("DestroyPlayer=0x%08x\n", hr);
  tt.Stop();
  delete this;
  g_fConnectionDeleted = true;
}


// **************  CFMGroup  **************

void CFMGroup::AddConnection(FedMessaging * pfm, CFMConnection * pcnxn)
{
  pfm->GetDPlay()->AddPlayerToGroup(GetDPID(), pcnxn->GetDPID());
}


void CFMGroup::DeleteConnection(FedMessaging * pfm, CFMConnection * pcnxn)
{
  // ignore return code, since dplay connection could already be dead
  pfm->GetDPlay()->DeletePlayerFromGroup(GetDPID(), pcnxn->GetDPID());
}


CFMGroup::CFMGroup(FedMessaging * pfm, const char * szName) :
    m_cPlayers(0),
    CFMRecipient(szName, 0) // dplay group hasn't been created yet
{
  DPNAME dpn;
  dpn.dwSize = sizeof(dpn);
  dpn.dwFlags = 0;
  dpn.lpszShortNameA = const_cast<char*>(szName);
  dpn.lpszLongNameA  = "";
  ZSucceeded(pfm->GetDPlay()->CreateGroup(&m_dpid, &dpn, NULL, 0, 0));
  LPVOID pv = this;
  ZSucceeded(pfm->GetDPlay()->SetGroupData(GetDPID(), &pv, sizeof(pv), DPSET_LOCAL));
}


void CFMGroup::Delete (FedMessaging * pfm) 
{
  pfm->GetDPlay()->DestroyGroup(GetDPID());
  delete this;
}


void CFMGroup::PlayerAdded(CFMConnection * pcnxn)
{
  m_cPlayers++;
}


void CFMGroup::PlayerDeleted(CFMConnection * pcnxn)
{
  m_cPlayers--;
}


/*-------------------------------------------------------------------------
 * FMSessionDesc.FMSessionDesc
 *-------------------------------------------------------------------------
  Purpose:
      Crack the name/value pairs in the dplay session name and populate the members

  Side Effects:
      This is kinda of bad coupling, since the server packs this, and the messaging layer unpacks. 
 */
FMSessionDesc::FMSessionDesc(LPDPSESSIONDESC2 pdpSessionDesc)
{
  char * szName = NULL;
  char * szValue = NULL;
  m_nNumPlayers = 0;
  m_nMaxPlayers = 1;

  szName = strtok((char *) pdpSessionDesc->lpszSessionNameA, "\t");
  while(szName)
  {
    if (!lstrcmp(szName, "GAM"))
    {
      szValue = strtok(NULL, "\n");
      m_strGameName = szValue;
    }
    else if (!lstrcmp(szName, "PLR"))
    {
      szValue = strtok(NULL, "\n");
      m_nNumPlayers = (short)atoi(szValue);
    }
    else if (!lstrcmp(szName, "LIM"))
    {
      szValue = strtok(NULL, "\n");
      m_nMaxPlayers = (short)atoi(szValue);
    }
    // there could be many more
    else // throw away any remainder
    {
      szValue = strtok(NULL, "\n");
    }

    szName = strtok(NULL, "\t");
  }
  
  m_guidInstance = pdpSessionDesc->guidInstance;
}



