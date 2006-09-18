/*-------------------------------------------------------------------------
  Server.cpp
  
  Per server stuff, including site for servers session
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

const DWORD        CFLServer::c_dwID        = 19680815;
const CFLMission * CFLServer::c_AllMissions = (CFLMission*) -1;

HRESULT LobbyServerSite::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
{
  CFLServer * pServer = CFLServer::FromConnection(cnxnFrom);
  assert(pServer);

  cnxnFrom.ResetAbsentCount();

  switch (pfm->fmid)
  {
    case FM_S_LOGON_LOBBY:
    {
      CASTPFM(pfmLogon, S, LOGON_LOBBY, pfm);

      if (pfmLogon->verLobby == LOBBYVER_LS)
      {
        if(pfmLogon->dwPort != 0)						// A port of 0 means the server couldn't find out its listening port
          pServer->SetServerPort(pfmLogon->dwPort);
        else
          pServer->SetServerPort(6073);				// Tell the client to enum the old fashioned way
      break;
	  }
      char * szReason;

      if (pfmLogon->verLobby > LOBBYVER_LS)
        szReason = "The Public Lobby server that you connected to is older than your version.  The Zone needs to update their lobby server.  Please try again later.";
      else /*if (pfmLogon->verLobby < LOBBYVER_LS)*/
        szReason = "Your server's version did not get auto-updated properly.  Please try again later.";

      BEGIN_PFM_CREATE(*pthis, pfmNewMissionAck, L, LOGON_SERVER_NACK)
          FM_VAR_PARM((char *)szReason, CB_ZTS)
      END_PFM_CREATE
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
      pthis->DeleteConnection(cnxnFrom);
      break;
    }

    case FM_S_NEW_MISSION:
    {
      // a server has created a mission of their own volition. We need to map it into our "cookie space"
      CASTPFM(pfmNewMission, S, NEW_MISSION, pfm);
      CFLMission * pMission = pServer->CreateMission(NULL); // NULL = no client created this
      BEGIN_PFM_CREATE(*pthis, pfmNewMissionAck, L, NEW_MISSION_ACK)
      END_PFM_CREATE
      pfmNewMissionAck->dwIGCMissionID = pfmNewMission->dwIGCMissionID;
      pfmNewMissionAck->dwCookie = (DWORD) pMission;
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
      // we won't broadcast it until the server sends us a lobby mission info, when it's good and ready
    }
    break;

    case FM_LS_LOBBYMISSIONINFO:
    {
      CASTPFM(pfmLobbyMissionInfo, LS, LOBBYMISSIONINFO, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmLobbyMissionInfo->dwCookie);
      if (pMission) // if it's already gone away--just ignore it. 
      {
        pMission->SetLobbyInfo(pfmLobbyMissionInfo);
        pMission->NotifyCreator();
      }
      // else TODO: do something about waiting client, if there is one
    }
    break;

    case FM_LS_MISSION_GONE:
    {
      CASTPFM(pfmMissionGone, LS, MISSION_GONE, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmMissionGone->dwCookie);
      pServer->DeleteMission(pMission);
    }
    break;

    case FM_S_HEARTBEAT:
      // don't boot for missing roll call until we get one from them
      pServer->SetHere();
    break;
    
    case FM_S_PLAYER_JOINED:
    {
      CASTPFM(pfmPlayerJoined, S, PLAYER_JOINED, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmPlayerJoined->dwMissionCookie);
      const char* szCharacterName = FM_VAR_REF(pfmPlayerJoined, szCharacterName);
      const char* szCDKey = FM_VAR_REF(pfmPlayerJoined, szCDKey);

      if (NULL == szCharacterName || '\0' != szCharacterName[pfmPlayerJoined->cbszCharacterName-1]
          || NULL == szCDKey || '\0' != szCDKey[pfmPlayerJoined->cbszCDKey-1])
      {
        // corrupt data
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_CorruptPlayerJoinMsg,
          cnxnFrom.GetName());
      }
      else if (NULL == pMission)
      {
        // the requested game does not exist
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_PlayerJoinInvalidMission,
          szCharacterName, cnxnFrom.GetName(), pfmPlayerJoined->dwMissionCookie);
      }
      else
      {
        if (g_pLobbyApp->EnforceCDKey())
        {
          char * szUnencryptedCDKey = (char*)_alloca(strlen(szCDKey) + 1);
          ZUnscramble(szUnencryptedCDKey, szCDKey, szCharacterName);

          szCDKey = szUnencryptedCDKey;
        }

        g_pLobbyApp->SetPlayerMission(szCharacterName, szCDKey, pMission);
      }
    }
    break;

    case FM_S_PLAYER_QUIT:
    {
      CASTPFM(pfmPlayerQuit, S, PLAYER_QUIT, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmPlayerQuit->dwMissionCookie);
      const char* szCharacterName = FM_VAR_REF(pfmPlayerQuit, szCharacterName);

      if (NULL == szCharacterName || '\0' != szCharacterName[pfmPlayerQuit->cbszCharacterName-1])
      {
        // corrupt data
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_CorruptPlayerQuitMsg,
          cnxnFrom.GetName());
      }
      else
        g_pLobbyApp->RemovePlayerFromMission(szCharacterName, pMission);
    }
    break;

    case FM_S_PAUSE:
    {
      CASTPFM(pfmPause, S, PAUSE, pfm);
      pServer->Pause(pfmPause->fPause);
      break;
    }      

    default:
      ZError("unknown message\n");
  }
  return S_OK;
}


HRESULT LobbyServerSite::OnSysMessage(FedMessaging * pthis) 
{
  return S_OK;
}


void    LobbyServerSite::OnMessageNAK(FedMessaging * pthis, DWORD dwTime, CFMRecipient * prcp) 
{}


HRESULT LobbyServerSite::OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  char szRemote[16];
  pthis->GetIPAddress(cnxn, szRemote);
  CFLServer * pServer = new CFLServer(&cnxn);
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerConnected, cnxn.GetName(), szRemote);

  // tell them what token to use
  BEGIN_PFM_CREATE(*pthis, pfmToken, L, TOKEN)
    FM_VAR_PARM(g_pLobbyApp->GetToken(), CB_ZTS)
  END_PFM_CREATE
  pthis->SendMessages(&cnxn, FM_GUARANTEED, FM_FLUSH);
  return S_OK;
}


HRESULT LobbyServerSite::OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  // connection is already gone, so we can't get address
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerDisconnected, cnxn.GetName());
  delete CFLServer::FromConnection(cnxn);
  return S_OK;
}


HRESULT LobbyServerSite::OnSessionLost(FedMessaging * pthis) 
{
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_ServersSessionLost);
  return S_OK;
}


int LobbyServerSite::OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType)
{
  debugf("LobbyServerSite::OnMessageBox: "); 
  return g_pLobbyApp->OnMessageBox(strText, strCaption, nType);
}


#ifndef NO_MSG_CRC
void LobbyServerSite::OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg)
{
  assert(0); // we should never get a bad crc from one of our own servers.
}
#endif


// ********** CFLServer *************

CFLServer::CFLServer(CFMConnection * pcnxn) :
  m_dwID(c_dwID),
  m_pcnxn(pcnxn),
  m_sPort(6703),		// Mdvalley: initialize to default enum, just in case
  m_cPlayers(0),
  m_maxLoad(300), // get from reg or something
  m_bHere(false),
  m_fPaused(false)
{
  assert(m_pcnxn);
  m_pcnxn->SetPrivateData((DWORD) this); // set up two-way link between connection and this

  m_pCounters = g_pLobbyApp->AllocatePerServerCounters(pcnxn->GetName());  
}

CFLServer::~CFLServer()
{
  DeleteMission(c_AllMissions);
  m_pcnxn->SetPrivateData(0); // disconnect two-way link between connection and this

  g_pLobbyApp->FreePerServerCounters(m_pCounters);
    
  // make sure all missions on this server are dead
  MissionList::Iterator iter(m_missions);
  while (!iter.End())
  { 
    delete iter.Value();
    iter.Remove(); 
  } 
}

void CFLServer::DeleteMission(const CFLMission * pMission)
{
  if (!pMission)
    return;

  if (c_AllMissions == pMission)
    g_pLobbyApp->RemoveAllPlayersFromServer(this);

  MissionList::Iterator iter(m_missions);
  while (!iter.End())
  { 
    if (c_AllMissions == pMission || iter.Value() == pMission)
    {
      if (c_AllMissions != pMission)
        g_pLobbyApp->RemoveAllPlayersFromMission(iter.Value());

      delete iter.Value();
      iter.Remove();
    }
    else
      iter.Next();
  } 
}

void CFLServer::Pause(bool fPause)
{
  if (m_fPaused == fPause)
    return;

  m_fPaused = fPause;
  MissionList::Iterator iter(m_missions);
  
  if (m_fPaused) // hide the games
  {
    g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerPause,
        GetConnection()->GetName());

    while (!iter.End())
    { 
      BEGIN_PFM_CREATE(g_pLobbyApp->GetFMClients(), pfmMissionGone, LS, MISSION_GONE)
      END_PFM_CREATE
      pfmMissionGone->dwCookie = iter.Value()->GetCookie();
      iter.Next();
    } 
    g_pLobbyApp->GetFMClients().SendMessages(g_pLobbyApp->GetFMClients().Everyone(), FM_GUARANTEED, FM_FLUSH);
  }
  else // show the games
  {
    g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerContinue, 
        GetConnection()->GetName());

    while (!iter.End())
    {
      FMD_LS_LOBBYMISSIONINFO * plmi = iter.Value()->GetMissionInfo();
      if (plmi)
        g_pLobbyApp->GetFMClients().QueueExistingMsg(plmi);
      iter.Next();
    }
    g_pLobbyApp->GetFMClients().SendMessages(g_pLobbyApp->GetFMClients().Everyone(), FM_GUARANTEED, FM_FLUSH);
  }
}

