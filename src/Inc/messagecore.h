#ifndef MessagesCore_h
#define MessagesCore_h
#include <dplobby.h>
#include <d3dtypes.h>

// MONOLITHIC_DPLAY means that we want to use a private dplay (MSRGIP.dll, MSRGTRAN.dll) instead
// of the stock dplay, which has private fixes. Only those HOSTING a session will use this, even if
// it's defined, which means that clients will never use it.
// #define MONOLITHIC_DPLAY

#define OBLIVION_CLIENT_REG_KEY "Allegiance"

// Specify which flavor of dplay we're using. Never specify interface explicity anywhere else
#define IDirectPlayX IDirectPlay4A
#define IID_IDirectPlayX IID_IDirectPlay4A

#define IDirectPlayLobbyX IDirectPlayLobby3A
#define IID_IDirectPlayLobbyX IID_IDirectPlayLobby3A

#define FEDMSGID unsigned short
#define CFEDMSGID const FEDMSGID

#define IB unsigned short // index of bytes to var-length data from start of struct
#define CB unsigned short // count of bytes 
// ***NOTE: there is code that depends on these being the same size. 

typedef unsigned short MsgClsPrio;
/* A FedMessage by itself is never used. It is merely the start of every message.
   This way, any message can be coerced to a FedMessage.
*/
struct FEDMESSAGE
{
  CB cbmsg; 
  /* Size of message. Messages can be variable length. This means that you 
     don't create them off the stack. You must calculate cbmsg (sum of all
     fixed length fields (just the size of the struct) + all variable length 
     fields), and allocate a pointer to the message data off the heap. See
     below for function to easily create these things
  */
  FEDMSGID fmid;
  // actual message data immediately follows. 
};

/* A packet is just an arbitrary number of messages strung together. Since we
   know how big each message is, we can easily visit each message. It also 
   makes it easy for the client to string messages together.
*/

/* Message naming:
      FM_C_*:  Messages that the client sends
      FM_S_*:  Messages that the server sends
      FM_CS_*: Messages that both client and server send.

  Coresponding structures are named FMD_*, where * is the same. 
  
  Yes, FM_C_* messages and FM_S_* messages could share message numbers, but
      let's not, to keep it simple.
*/ 

/* 
  ************** MESSAGING CLASS ************** 
  Just make one of these FedMessaging (per session), and use it for all your messaging needs
*/

class FedMessaging;
class CFMConnection;
class CFMGroup;

/*-------------------------------------------------------------------------
 * CFMRecipient
 *-------------------------------------------------------------------------
  Purpose:
      This represents an addressable entity in FedMessaging
 */
class CFMRecipient
{
friend CFMConnection;
friend CFMGroup;
public:
  const char * GetName() {return m_szName;}
  DWORD     GetID() {return GetDPID();}
  virtual int GetCountConnections() = 0;

protected:
  CFMRecipient(const char * szName, DPID dpid) :
    m_dpid(dpid)
  {
    m_szName = new char[lstrlen(szName) + 1];
    lstrcpy(m_szName, szName);
  }
  ~CFMRecipient()
  {
    delete [] m_szName;
  }
  DPID     GetDPID() {return m_dpid;}

private:
  char * m_szName;

protected: // groups set their own dpid since they're not pre-created.
  void      SetDPID(DPID dpid) {m_dpid = dpid;}
  DPID    m_dpid;
};

/*-------------------------------------------------------------------------
 * CFMConnection
 *-------------------------------------------------------------------------
  Purpose:
      This represents one remote entity (connection). It's mainly useful only on the server, since there's lots of them
 */
class CFMConnection : public CFMRecipient// Hungarian prefix: cnxn
{
friend FedMessaging;
public:
  void    SetPrivateData(DWORD dw) {m_dwPrivate = dw;}
  DWORD   GetPrivateData() {return m_dwPrivate;}

  int IncAbsentCount()
  {
    return ++m_cAbsentCount;
  }

  void ResetAbsentCount()
  {
    m_cAbsentCount = 0;
  }

  void SetLastComplete(DWORD dwTime)
  {
    m_dwTimeLastComplete = dwTime;
  }

  DWORD GetLastComplete()
  {
    return m_dwTimeLastComplete;
  }

  virtual int GetCountConnections() {return 1;}


private:
  CFMConnection(FedMessaging * fm, const char * szName, DPID dpid); // only FedMessaging::CreateConnection can create these things
  ~CFMConnection() {} // you can't delete these directly. You must use Delete()

  void Delete(FedMessaging * pfm); // basically the destructor, but with a parameter
  
  int     m_cAbsentCount; // for roll call
  DWORD   m_dwTimeLastComplete;
  DWORD   m_dwPrivate;
};

typedef TList<CFMConnection*> ListConnections;

/*-------------------------------------------------------------------------
 * CMFGroup
 *-------------------------------------------------------------------------
  Purpose:
      Provide group management
 */
class CFMGroup : public CFMRecipient
{
friend FedMessaging;
public:
  void AddConnection(FedMessaging * pfm, CFMConnection * pcnxn);
  void DeleteConnection(FedMessaging * pfm, CFMConnection * pcnxn);
  virtual int GetCountConnections() {return m_cPlayers;}

private:
  CFMGroup(FedMessaging * pfm, const char * szName);
  ~CFMGroup() {}
  void PlayerAdded(CFMConnection * pcnxn);
  void PlayerDeleted(CFMConnection * pcnxn);
  void Delete (FedMessaging * pfm);

  int m_cPlayers;
};

typedef TList<CFMGroup*> ListGroups;

class FMSessionDesc
{
public:
  FMSessionDesc(LPDPSESSIONDESC2 pdpSessionDesc);
  PCC     GetGameName()   {return m_strGameName;}
  REFGUID GetInstance()   {return m_guidInstance;}
  int     GetNumPlayers() {return m_nNumPlayers;}
  int     GetMaxPlayers() {return m_nMaxPlayers;}
  
private:
  GUID    m_guidInstance;       // ID for the session instance
  ZString m_strGameName; 
  short   m_nNumPlayers;
  short   m_nMaxPlayers;
};


const CB c_cbBuff = 16<<10; // 16K

enum FMGuaranteed
{
  FM_GUARANTEED,
  FM_NOT_GUARANTEED,
};

enum FMFlush
{
  FM_FLUSH,
  FM_DONT_FLUSH,
};


// Outgoing event interface for FedMessaging
class IFedMessagingSite : public IObject
{
public:
  virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm) = 0;
  virtual HRESULT OnSysMessage(FedMessaging * pthis) {return S_OK;}
  virtual void    OnMessageNAK(FedMessaging * pthis, DWORD dwTime, CFMRecipient * prcp) {}
  virtual int     OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType) {return 0;}
  virtual HRESULT OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) {return S_OK;}
  virtual HRESULT OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) {return S_OK;}
  virtual HRESULT OnSessionLost(FedMessaging * pthis) {return S_OK;}
  virtual void    OnPreCreate (FedMessaging * pthis) {}
  virtual void    OnPostCreate(FedMessaging * pthis, IDirectPlayX* pdpIn, IDirectPlayX** pdpOut) {*pdpOut = pdpIn;}
  virtual void    OnSessionFound(FedMessaging * pthis, FMSessionDesc * pSessionDesc) {} // you cannot save any pointer to session desc
  virtual void    OnMessageSent(FedMessaging * pthis, CFMRecipient * precip, const void * pv, DWORD cb, FMGuaranteed fmg) {}
#ifndef NO_MSG_CRC
  virtual void    OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg) {}
#endif
};


class FedMessaging
{
public:
  static const  MsgClsPrio c_mcpDefault;
  FedMessaging(IFedMessagingSite * pfmSite);
  ~FedMessaging();
  HRESULT             Connect(const char * szAddress);
  HRESULT             HostSession(GUID guidApplication, bool fKeepAlive, HANDLE hEventServer, bool fProtocol
#ifdef MONOLITHIC_DPLAY
    , bool fMonolithic = true
#endif
    );
  HRESULT             JoinSession(GUID guidApplication, const char * szServer, const char * szName);
  HRESULT             JoinSessionInstance(GUID guidInstance, const char * szName);

  GUID                GetHostApplicationGuid()
  {
    return m_guidApplication;
  }

  GUID                GetSessionGuid()    
  {
    assert(m_pDirectPlay); 
    return m_guidInstance;
  }
  int                 SendMessages(CFMRecipient * precip, FMGuaranteed fmg, FMFlush fmf);
  void                ForwardMessage(CFMRecipient * precip, const FEDMESSAGE* pfm, FMGuaranteed fmg)
  {
    GenericSend(precip, pfm, pfm->cbmsg, fmg);
  }
  HRESULT             GenericSend(CFMRecipient * precip, const void * pv, CB cb, FMGuaranteed fmg);
  void                Shutdown();
  inline bool         IsConnected()       
  {
    return m_fConnected;
  }
  bool                CheckVersion();

  void *              PFedMsgCreate(bool fQueueMsg, BYTE * pbFMBuff, FEDMSGID fmid, CB cbfm, ...);
  /* The variable parameters are pairings of pointers to the variable length
         members, and their lengths (type MUST be CB), followed by FM_END to 
         terminate the variable length parameter list. Pointers/lengths must 
         be provided for every variable length member of the message. Pointers 
         can be null if that member is not used (in which case the length is 
         ignored, but must still be supplied. It is an error to provide a valid 
         pointer with a length of zero. The value CB_ZTS may be used for null-
         terminated strings, instead of predetermining their length. After 
         creating a message, you then fill in the fixed length stuff yourself.
         The variable length stuff is copied, so you don't have to keep it around.
         
         ***NEVER*** call this function directly. Use the BEGINPFMCREATE, 
         FM_VAR_PARM, and ENDPFMCREATE macros.
     
     You can always cast PFedMsgCreate() to a FEDMESSAGE*, but I don't return
         FEDMESSAGE*, so that you can assign the return value to any message
         without requiring additional casting
     
     Since the only reason to create a message is to send it, messages 
         automagically get recycled after sending them. Hence it is a bug to try 
         to reference a message once it's been sent. Likewise, there is no need 
         (or way) to free a message created with this function.
  */
  
  HRESULT         ReceiveMessages();
  BYTE *          BuffOut()             
  {
    return m_fSecondaryOut ? m_rgbbuffSecondaryOutPacket : m_rgbbuffOutPacket;
  }
  CB              GetBuffOutSize()
  {
    return m_fSecondaryOut ? sizeof(m_rgbbuffSecondaryOutPacket) : sizeof(m_rgbbuffOutPacket)
#ifndef NO_MSG_CRC
      - sizeof(int) // crc
#endif      
    ;
  }
  BYTE *          BuffIn()              
  {
    return m_rgbbuffInPacket;
  }
  DWORD           PacketSize()          
  {
    return m_dwcbPacket
#ifndef NO_MSG_CRC
      - sizeof(int) // crc
#endif      
      ;
  }
  void            PurgeOutBox()         
  {
    m_pbFMNext = BuffOut();
    m_precipDefault = NULL;
  }
  CB              CbFreeSpaceInOutbox()
  {
    return GetBuffOutSize() - CbUsedSpaceInOutbox();
  }
  CB              CbUsedSpaceInOutbox() 
  {
    return m_pbFMNext - ((BYTE*) BuffOut());
  }
  void            QueueExistingMsg(const FEDMESSAGE * pfm);
  void            SetPriority(USHORT priority) {m_priority = priority;}
  USHORT          GetPriority()         
  {
    return m_priority;
  }
  static USHORT   GetGuaranteedBias()   
  {
    return 1000;
  }
  Time            CheckOdometer(float& flDTime, int& cMsgsOdometer, int& cBytesOdometer);
  void            SetDefaultRecipient(CFMRecipient * precip, FMGuaranteed fmg);
  CFMRecipient *  GetDefaultRecipient(FMGuaranteed * pfmg);

  int             GetConnectionCount() 
  {
    return m_listCnxns.GetCount();
  }
  ListConnections * GetConnections()
  {
    return &m_listCnxns;
  }
  CFMConnection * GetConnectionFromId(DWORD id)
  {
    return GetConnectionFromDpid((DPID) id);
  }
  CFMConnection * GetServerConnection()
  {
    return m_pcnxnServer;
  }
  CFMGroup *      CreateGroup(const char * szName)
  {
    static CTempTimer tt("in CreateGroup", .01f);
    tt.Start();
    CFMGroup * pgrp = new CFMGroup(this, szName);
    tt.Stop();
    m_listGroups.PushFront(pgrp);
    return pgrp;
  }

  void            DeleteConnection(CFMConnection & cnxn) 
  {
    static CTempTimer tt("in DeleteConnection", .01f);
    tt.Start();
    m_pfmSite->OnDestroyConnection(this, cnxn);
    tt.Stop();
    m_listCnxns.Remove(&cnxn);
    cnxn.Delete(this);
  }

  void            DeleteGroup(CFMGroup * pgrp)
  {
    if (pgrp)
    {
      m_listGroups.Remove(pgrp);
      static CTempTimer tt("in DeleteGroup", .01f);
      tt.Start();
      pgrp->Delete(this);
      tt.Stop();
    }
  }

  void            AddConnectionToGroup(CFMGroup * pgrp, CFMConnection * pcnxn)
  { 
    assert(pgrp && pcnxn);
    static CTempTimer tt("in AddConnectionToGroup", .01f);
    tt.Start();
    pgrp->AddConnection(this, pcnxn); 
    tt.Stop();
  }

  void            DeleteConnectionFromGroup(CFMGroup * pgrp, CFMConnection * pcnxn)
  { 
    assert(pgrp && pcnxn);
    static CTempTimer tt("in DeleteConnectionFromGroup", .01f);
    tt.Start();
    pgrp->DeleteConnection(this, pcnxn); 
    tt.Stop();
  }

  HRESULT         GetIPAddress(CFMConnection & cnxn, char szRemoteAddress[16]);
  HRESULT         GetSendQueue(DWORD * pcMsgs, DWORD * pcBytes)
  {
    return GetDPlay()->GetMessageQueue(0, 0, DPMESSAGEQUEUE_SEND, pcMsgs, pcBytes);
  }
  HRESULT         GetReceiveQueue(DWORD * pcMsgs, DWORD * pcBytes)
  {
    return GetDPlay()->GetMessageQueue(0, 0, DPMESSAGEQUEUE_RECEIVE, pcMsgs, pcBytes);
  }

  HRESULT         GetConnectionSendQueue(CFMConnection * pcnxn, DWORD * pcMsgs, DWORD * pcBytes)
  {
    return GetDPlay()->GetMessageQueue(0, pcnxn->GetDPID(), DPMESSAGEQUEUE_SEND, pcMsgs, pcBytes);
  }

  CFMGroup *  Everyone() 
  {
    return m_pgrpEveryone;
  }

  IDirectPlayX *  GetDPlay() // only messaging stuff should need this
  {
    return m_pDirectPlay;
  } 

  void UseMainOutBox(bool fMain) // set false for one or more messages that don't go to the same recipient as main stream
  {
    if (fMain == m_fSecondaryOut)
    {
      BYTE * pbT = m_pbFMNextT;
      m_pbFMNextT = m_pbFMNext; // save this for when we switch back
      m_pbFMNext    = pbT;
      m_fSecondaryOut = !fMain;
    }
  }

  DWORD GetCountConnections()
  {
    DPSESSIONDESC2 * psd = NULL;
    DWORD dwSize = 0;
    HRESULT hr = m_pDirectPlay->GetSessionDesc(psd, &dwSize);
    assert (DPERR_BUFFERTOOSMALL  == hr);
    psd = (DPSESSIONDESC2 *) new char[dwSize];
    ZSucceeded(m_pDirectPlay->GetSessionDesc(psd, &dwSize));
    DWORD dwConnections = psd->dwCurrentPlayers - 1; // let's not count the server
    delete [] psd;
    return dwConnections;
  }

  Time GetLastMsgTime()
  {
    return m_timeMsgLast;
  }

  HRESULT GetLinkDetails(CFMConnection * pcnxn, OUT DWORD * pdwHundredbpsG, OUT DWORD * pdwmsLatencyG, 
                                                  OUT DWORD * pdwHundredbpsU, OUT DWORD * pdwmsLatencyU)
  {
    DPCAPS caps;
    HRESULT hr = E_FAIL;
    if (!pcnxn || !GetDPlay()) // throw error?
      return E_FAIL;
    
    caps.dwSize = sizeof(caps);
    hr = GetDPlay()->GetPlayerCaps(pcnxn->GetDPID(), &caps, DPGETCAPS_GUARANTEED);
    if FAILED(hr)
      goto Exit;
    if (pdwHundredbpsG)
      *pdwHundredbpsG = caps.dwHundredBaud;
    if (pdwmsLatencyG)
      *pdwmsLatencyG = caps.dwLatency;

    hr = GetDPlay()->GetPlayerCaps(pcnxn->GetDPID(), &caps, 0);
    if FAILED(hr)
      goto Exit;
    if (pdwHundredbpsU)
      *pdwHundredbpsU = caps.dwHundredBaud;
    if (pdwmsLatencyU)
      *pdwmsLatencyU = caps.dwLatency;
  Exit:    
    return hr;
  }

  HRESULT EnumSessions(GUID guidApplication, const char * szServer); // blank for broadcast

  void SetSessionDetails(char * szDetails) // tokenized name/value pairs: Name\tValue\nName\tValue...
  {
    char rgchBuff[10<<10];
    LPDPSESSIONDESC2 pdpSession = (LPDPSESSIONDESC2) rgchBuff;
    DWORD dw = sizeof(rgchBuff);
    assert(m_pDirectPlay);
    ZSucceeded(m_pDirectPlay->GetSessionDesc(pdpSession, &dw));
    pdpSession->lpszSessionNameA = szDetails;
    m_pDirectPlay->SetSessionDesc(pdpSession, 0);
  }

  void ResetOutBuffer()
  {
    m_pbFMNextT     = m_rgbbuffSecondaryOutPacket; // save this for when we switch back
    m_pbFMNext      = m_rgbbuffOutPacket;
    m_fSecondaryOut = false;
  }
  
private:
  Time            m_timeMsgLast;
  CFMConnection*  GetConnectionFromDpid(DPID dpid);
  CFMGroup *      GetGroupFromDpid(DPID dpid);
  CFMConnection * CreateConnection(const char * szName, DPID dpid); // after the physical connection has already been established

  FEDMESSAGE *    PfmGetNext(FEDMESSAGE * pfm); // Use to run through all messages in a received packet
      // returns NULL if no more messages in packet
  IDirectPlayLobbyX * GetDPlayLobby()     
  {
    return m_pDirectPlayLobby;
  }
  int             SendToDefault(FMFlush fmf);
  HRESULT         InitDPlay(
#ifdef MONOLITHIC_DPLAY
    bool fMonolithic
#endif
  );
  bool            KillSvr();
  HRESULT         ConnectToDPAddress(LPVOID pAddress);
  HRESULT         OnSysMessage(FedMessaging * pthis, LPDPMSG_GENERIC pMsg);
  HRESULT         EnumSessionsInternal(GUID guidApplication, const char * szServer);
  friend BOOL FAR PASCAL EnumSessionsCallback(LPCDPSESSIONDESC2 lpThisSD,
                                      LPDWORD lpdwTimeOut,
                                      DWORD dwFlags,
                                      LPVOID lpContext);
  
  BYTE                m_rgbbuffInPacket[c_cbBuff];
  BYTE                m_rgbbuffOutPacket[c_cbBuff];
  BYTE                m_rgbbuffSecondaryOutPacket[1<<10];
  BYTE                m_rgbbuffAlloc[2<<10];      // 2K this is a temp buffer for alloc'd messages
  BYTE *              m_pbFMNext;                 // where next message gets queued in the CURRENT outbox
  BYTE *              m_pbFMNextT;                 // where next message IN THE OTHER OUTBOX gets queued
  DWORD               m_dwcbPacket;               // size of packet read, not CB because used with DPlay
  USHORT              m_priority;
  Time                m_timeOdometerStart;
  int                 m_cMsgsOdometer;
  int                 m_cBytesOdometer;
  CFMRecipient *      m_precipDefault;              // default recipient, if buffer overflows, or DPID_UNKNOWN is specified in SendMessages
  FMGuaranteed        m_fmGuaranteedDefault;
  bool                m_fConnected    : 1;
  bool                m_fSecondaryOut : 1;
  bool                m_fSessionCallback : 1;
  CFMConnection *     m_pcnxnMe;                   // this connection is NOT in the list m_listCnxns, and no site events fire on it.
  CFMConnection *     m_pcnxnServer;               // same as above, used only by GetServerConnection
  CFMGroup *          m_pgrpEveryone;
  GUID                m_guidInstance;
  GUID                m_guidApplication;            // guid given to HostSession()
  IDirectPlayX *      m_pDirectPlay;
  IDirectPlayLobbyX * m_pDirectPlayLobby;
  IFedMessagingSite * m_pfmSite;

  // these lists are somewhat redundant, since dplay manages them also, but it's nice to have our own list (maybe)
  ListConnections     m_listCnxns; 
  ListGroups          m_listGroups; 

  int                 m_cbConnection; // sizeof each connection
};


#define CB_ZTS 0xffff // indicates zero terminated string ONLY when creating 
    // messages via PFedMsgCreate. CBs always hold true value for all var fields
#define FM_END_VAR_PARMS (BYTE*) 0xffffffff // Always last argument passed to PFedMsgCreate.


/* *************** MESSAGE CREATION MACROS *************** 
 Use these to actually create messages
*/
// Use this for messages with a fixed size that will be sent
#define PFM_CREATE_STATIC(FM_VAR, TYPE, SHORTNAME) \
    static FMD_##TYPE##_##SHORTNAME FM_VAR; \
    FM_VAR.fmid = FM_##TYPE##_##SHORTNAME; \
    FM_VAR.cbmsg = sizeof(FMD_##TYPE##_##SHORTNAME);

// main macro use to create a message and queue it. 
#define BEGIN_PFM_CREATE(OBJ, FM_VAR, TYPE, SHORTNAME) \
  FMD_##TYPE##_##SHORTNAME * FM_VAR = (FMD_##TYPE##_##SHORTNAME *) \
      (OBJ).PFedMsgCreate(true, NULL, FM_##TYPE##_##SHORTNAME, sizeof(FMD_##TYPE##_##SHORTNAME),

// Use these to create message without queing them to be sent, and to delete the message
// The OBJ for doesn't reference any class data, so you can use any handy obj
#define BEGIN_PFM_CREATE_ALLOC(OBJ, FM_VAR, TYPE, SHORTNAME) \
  FMD_##TYPE##_##SHORTNAME * FM_VAR = (FMD_##TYPE##_##SHORTNAME *) \
      (OBJ).PFedMsgCreate(false, NULL, FM_##TYPE##_##SHORTNAME, sizeof(FMD_##TYPE##_##SHORTNAME),
#define PFM_DEALLOC(PFM) (GlobalFreePtr(PFM))


// Use this to create message into prealloc'd memory without queing it to be sent
#define BEGIN_PFM_CREATE_PREALLOC(OBJ, PBFM, TYPE, SHORTNAME) \
      (OBJ).PFedMsgCreate(false, (BYTE*)(PBFM), FM_##TYPE##_##SHORTNAME, sizeof(FMD_##TYPE##_##SHORTNAME),

// Use this to make sure they're always given in pairs and typed correctly
#define FM_VAR_PARM(PBLOB, CBBLOB) (BYTE *)(PBLOB), (CB *)(CBBLOB), 
#define END_PFM_CREATE FM_END_VAR_PARMS );

#define CASTPFM(FM_NEWVAR, TYPE, SHORTNAME, FM_OLDVAR) \
  FMD_##TYPE##_##SHORTNAME * FM_NEWVAR = (FMD_##TYPE##_##SHORTNAME *) (FM_OLDVAR)

// Only use these macros to define new messages
// TYPE is one of C (only client sends this message), S (only server creates),
//   or CS (both client and server send it--usually originated by client, rebroadcast by server)
// NUMBER is unique message number. Just use the next available number
#define DEFINE_FEDMSG(TYPE, SHORTNAME, NUMBER) \
  CFEDMSGID FM_##TYPE##_##SHORTNAME = NUMBER; \
  static AddMsg AM_##TYPE##_##SHORTNAME(NUMBER, "FM_" #TYPE "_" #SHORTNAME); \
  struct FMD_##TYPE##_##SHORTNAME : public FEDMESSAGE \
  { 

#define END_FEDMSG };

// Only use this to create variable length fields. NO fixed-size members can precede this
#define FM_VAR_ITEM(NAME) IB ib##NAME; CB cb##NAME
// Use this to actually reference existing var-length props
#define FM_VAR_REF(PFM, NAME) ((PFM)->cb##NAME ? (char*)(PFM) + (PFM)->ib##NAME : NULL)

/* For each message structure, the variable length items MUST go first, then the 
   fixed length items

   We also need to keep track of the most number of variable length parameters
   that any message has (MAXVARPARMS), because it makes creating messages much easier
*/
#define MAXVARPARMS 10

/* Pointers to messages (pfm's) can be saved only for subsequent messages within the same packet
   The following macro can be used for asserts where you want to ensure that a previous message
   pointer is still valid
*/
#define IS_VALID_PFM(PFM, TYPE, SHORTNAME) \
    (!IsBadReadPtr(PFM, sizeof(FEDMESSAGE)) && \
     FM_##TYPE##_##SHORTNAME == ((FEDMESSAGE *)(PFM))->fmid && \
     !IsBadReadPtr(PFM, ((FEDMESSAGE *)(PFM))->cbmsg))

extern char * g_rgszMsgNames[];

#define MAXMESSAGES 400
#define ALLOC_MSG_LIST char * g_rgszMsgNames[MAXMESSAGES + 1]

/*-------------------------------------------------------------------------
 * AddMsg
 *-------------------------------------------------------------------------
 * Purpose:
 *    Automatically build array of message names
 * 
 * Notes:
 *    Called during global variable initialization, so no call stack
 */
class AddMsg
{
public:
  AddMsg(FEDMSGID fmid, char * szMsgName)
  {
    assert (fmid <= MAXMESSAGES);
    g_rgszMsgNames[fmid] = szMsgName;
  }
};


#endif
