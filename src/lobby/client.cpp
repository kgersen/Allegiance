/*-------------------------------------------------------------------------
  Client.cpp
  
  Per client stuff, including site for Clients session
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
//
// wlp 2006 - the next line goes above the pch.h to avoid compile errors
// 

#include "pch.h"
 
const DWORD CFLClient::c_dwID = 19680815;
#ifndef NO_MSG_CRC
bool g_fLogonCRC = true; 
#endif

//appweb
static MprMutex* mutex = new MprMutex();

// mdvalley: 2005 needs to specify dword
static DWORD GetRegDWORD(const char* szKey, DWORD dwDefault)
{
  DWORD dwResult = dwDefault;

  HKEY  hk;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, HKLM_AllLobby, 0, "", 
      REG_OPTION_NON_VOLATILE, KEY_READ, NULL, &hk, NULL) == ERROR_SUCCESS)
  {
    _Module.ReadFromRegistry(hk, false, szKey, &dwResult, dwDefault);
  }

  return dwResult;
}

static void doASGS(void* data, MprThread *threadp) {
	CSQLQuery * pQuery = (CSQLQuery *)data;  //use the AZ legacy data & callback
	CQLobbyLogon * pls = (CQLobbyLogon *)data;
	CQLobbyLogonData * pqd = pls->GetData();
	
	FedMessaging & fm = g_pLobbyApp->GetFMClients();
	CFMConnection * pcnxn = fm.GetConnectionFromId(pqd->dwConnectionID);
	
	int contentLen = 0; char *content; char szResponse[MAX_PATH];
	char szURL[MPR_HTTP_MAX_URL]; char szName[c_cbName]; 
	Strcpy(szURL,"http://asgs.alleg.net/asgs/services.asmx/AuthenticateTicket?Callsign="); 
	
	// one thread per connecting player
	Strcpy(szName,pqd->szCharacterName);
	
	// ignore "tokens" in player name
	ZString strName = szName;
	if ((isalnum(szName[0]) == 0) && (strName.Left(1) != "_"))
		Strcpy(szName,strName.RightOf(1));

	// add the name to the url
	Strcat(szURL,szName);
	
	// add the IP to the url
    char szAddress[16];
	fm.GetIPAddress(*pcnxn,szAddress);
	Strcat(szURL,"&IP=");
	Strcat(szURL,szAddress);

	// add the ticket to the url
	Strcat(szURL,"&Ticket=");
	char escaped[2048];
	maUrlEncode(escaped, sizeof(escaped), pqd->szCDKey, 1);
	Strcat(szURL,escaped);

	// the beast
	MaClient* client = new MaClient();
	
	// max wait 5 seconds
	client->setTimeout(2500);
	client->setRetries(2);
	client->setKeepAlive(0);

	// finally
	client->getRequest(szURL);

    // check registry to see is ASGS is on or off

    DWORD dw; // Gets result of whether it opened or created... - we only read
    HKEY  hk;
    if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, HKLM_FedSrv, 0, "", REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hk, &dw) == ERROR_SUCCESS)
    {
        // read ASGS is on or off in registry
        DWORD dwASGS_ON=0;
        bool bSuccess = _Module.ReadFromRegistry(hk, false, "ASGS_ON", &dwASGS_ON, 0);
        RegCloseKey(hk);
		if (bSuccess && dwASGS_ON) { // ASGS is on 
        	// Imago check for HTTP OK 8/3/08  (this wont hang either)
    	    if (client->getResponseCode() == 200) {
    		    content = client->getResponseContent(&contentLen);

    		    if (contentLen > 0) { // there's POSITIVE content, we excpect it a certain way...
    			    ZString strContent = content;
    			    strContent = strContent.RightOf(85);
    			    strContent = strContent.LeftOf("<");
    			    Strcpy(szResponse,(PCC)strContent);

    			    if (strcmp(szResponse,"-1") == 0) {
    				    mutex->lock();
    				    pqd->fValid = false;
    				    pqd->fRetry = false;
    				    char * szReason = "ASGS Authentication Failure.\n\nPlease restart the game using ASGS.";
	    			    pqd->szReason = new char[lstrlen(szReason) + 1];
    				    Strcpy(pqd->szReason,szReason);
    				    mutex->unlock();
			        }
		        } 
	        }
		}
	}

	// tell the main thread we've finished, use the existing thread msg for AZ SQL 
	PostThreadMessage(_Module.dwThreadID, wm_sql_querydone, (WPARAM) NULL, (LPARAM) pQuery);
	delete client;
}


void QueueMissions(FedMessaging * pfm)
{
    // Send client all the mission infos, in as few packets as possible
    bool fIsFreeLobby = g_pLobbyApp->EnforceCDKey();
    ListConnections::Iterator iterCnxn(*g_pLobbyApp->GetFMServers().GetConnections());
    while (!iterCnxn.End())
    {
      CFLServer * pServerT = CFLServer::FromConnection(*iterCnxn.Value());
      if (!pServerT->GetPaused())
      {
        MissionList::Iterator iterMissions(*pServerT->GetMissions());
        while (!iterMissions.End())
        {
          FMD_LS_LOBBYMISSIONINFO * plmi = iterMissions.Value()->GetMissionInfo();
		  //imago 7/1/09 removed old checks
          if (plmi)// && (fIsFreeLobby || plmi->nNumPlayers > 0 || plmi->fMSArena
			  //|| (!fIsFreeLobby && strcmp(FM_VAR_REF(plmi,szIGCStaticFile),"zone_core")))) //-KGJV: advertize custom core game 
            pfm->QueueExistingMsg(plmi);
          iterMissions.Next();
        }
      }
      iterCnxn.Next();
    }
}


/*-------------------------------------------------------------------------
 * GotLogonInfo
 *-------------------------------------------------------------------------
  Purpose:
      Callback from the database when a logon info query comes back

  Parameters:
      the completed query
 */
void GotLogonInfo(CQLobbyLogon * pquery)
{
  FedMessaging & fm = g_pLobbyApp->GetFMClients();
  CQLobbyLogonData * pqd = pquery->GetData();
  char * szReason = pqd->szReason;

  CFMConnection * pcnxn = fm.GetConnectionFromId(pqd->dwConnectionID);
  if (!pcnxn)
    return; // nothing we can do for someone who's gone

  if (g_pLobbyApp->EnforceCDKey() && pqd->fValid && !pqd->fValidCode)
  {
    pqd->fValid = false;
    pqd->fRetry = true;
    szReason = "The CD Key you entered is not valid. Please "
               "press <CD Key> and re-enter the CD Key from "
               "the back of your CD case.";
  }

  if (pqd->fValid)
  {
    fm.SetDefaultRecipient(pcnxn, FM_GUARANTEED);
    BEGIN_PFM_CREATE(fm, pfmLogonAck, L, LOGON_ACK)
    END_PFM_CREATE
    pfmLogonAck->dwTimeOffset = pqd->dTime;

    // tell them which squads they are on
	/*
    int cRows;
    CQLobbyLogonData * pRows = pquery->GetOutputRows(&cRows); // all the rows are here

    SquadMembership* pargSquads = new SquadMembership[cRows];

    for (int iSquad = 0; iSquad < cRows; iSquad++)
    {
      Strcpy(pargSquads[iSquad].m_szSquadName, pRows->szSquadName);
      pargSquads[iSquad].m_squadID            = pRows->squadID;
      pargSquads[iSquad].m_fIsLeader          = 0 == pRows->status;
      pargSquads[iSquad].m_fIsAssistantLeader = 1 == pRows->status && 1 == pRows->detailedStatus;
    }

    BEGIN_PFM_CREATE(fm, pfmSquadMemberships, LS, SQUAD_MEMBERSHIPS)
        FM_VAR_PARM(cRows ? pargSquads : NULL, sizeof(SquadMembership) * cRows)
    END_PFM_CREATE
    pfmSquadMemberships->cSquadMemberships = cRows;          

    delete [] pargSquads;
	*/

	// KGJV - undeclared identifier error 
    // QueueMissions(FedMessaging * pfm)

	QueueMissions(&fm);
  }

  if (!pqd->fValid)
  {
    BEGIN_PFM_CREATE(fm, pfmLogonNack, L, LOGON_NACK)
      FM_VAR_PARM(szReason, CB_ZTS)
    END_PFM_CREATE
    pfmLogonNack->fRetry = pqd->fRetry;
  }
  
  g_pLobbyApp->GetFMClients().SendMessages(pcnxn, FM_GUARANTEED, FM_FLUSH);
}

const int c_cMaxPlayers = GetRegDWORD("MaxPlayersPerServer", 350);

HRESULT LobbyClientSite::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
{
  CFLClient * pClient = CFLClient::FromConnection(cnxnFrom);
  assert(pClient);

  debugf("Client: %s from <%s> at time %u\n", g_rgszMsgNames[pfm->fmid], cnxnFrom.GetName(), Time::Now());
  
  switch (pfm->fmid)
  {
    // TODO: remove this post-beta.
    case FM_C_LOGON_LOBBY_OLD:
    {
#ifndef NO_MSG_CRC
      bool fCRC = g_fLogonCRC;
      g_fLogonCRC = true; // assume always yes until we find one via OnBadCRC that is not.
#endif      
      CASTPFM(pfmLogon, C, LOGON_LOBBY_OLD, pfm);

      // no need to authenticate - they're out of sync and need to auto-update


      if (g_pAutoUpdate && pfmLogon->crcFileList != g_pAutoUpdate->GetFileListCRC())
      {
        // they need auto update
        BEGIN_PFM_CREATE(*pthis, pfmAutoUpdate, L, AUTO_UPDATE_INFO)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPServer(), CB_ZTS)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPInitialDir(), CB_ZTS)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPAccount(), CB_ZTS)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPPassword(), CB_ZTS)
        END_PFM_CREATE
        pfmAutoUpdate->crcFileList = g_pAutoUpdate->GetFileListCRC();
        pfmAutoUpdate->nFileListSize = g_pAutoUpdate->GetFileListSize();
      }
      else 
      {
        // tell client that his version is wrong
        char * szReason = "Your game's version did not get auto-updated properly.  Please try again later.";
        BEGIN_PFM_CREATE(*pthis, pfmLogonNack, L, LOGON_NACK)
          FM_VAR_PARM((char *)szReason, CB_ZTS)
        END_PFM_CREATE
        pfmLogonNack->fRetry = false;
      }

#ifndef NO_MSG_CRC
      // Big hack to communicate w/ old non-crc clients. This is the ONLY time we send them any non-crc'd messages 
      // by increasing the announced size of the message, the client will skip past the crc.
      if (!fCRC)
        *(CB*)(pthis->BuffOut()) += sizeof(int);
#endif      
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

	// Imago was here 6/26/08 7/5/08
    case FM_C_LOGON_LOBBY:
    {
	  CQLobbyLogon * pquery = new CQLobbyLogon(GotLogonInfo);
      CQLobbyLogonData * pqd = pquery->GetData();
    
      Strcpy(pqd->szCharacterName, cnxnFrom.GetName());
      pqd->characterID = 0; // Imago get Pook to have ASGS give us these? 
	  pqd->dwConnectionID = cnxnFrom.GetID();

      CASTPFM(pfmLogon, C, LOGON_LOBBY, pfm);
      bool fValid = true; // whether we have a valid logon, changed default (FREE Allegiance) -Imago
      bool fRetry = false;
      char * szReason = NULL; // when fValid==false
	  
	  // Imago make the SQL stuff we're using for asgs happy
	  pqd->dTime = pfmLogon->dwTime - Time::Now().clock();
	  const char * strCDKey = (const char*) FM_VAR_REF(pfmLogon, ASGS_Ticket); //ASGS_Ticket, CDKey, whatever
	  Strcpy(pqd->szCDKey,strCDKey);
	  pqd->fValid = fValid;
	  pqd->fRetry = fRetry;

      if (g_pAutoUpdate && pfmLogon->crcFileList != g_pAutoUpdate->GetFileListCRC())
      {
        // they need auto update
        BEGIN_PFM_CREATE(*pthis, pfmAutoUpdate, L, AUTO_UPDATE_INFO)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPServer(), CB_ZTS)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPInitialDir(), CB_ZTS)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPAccount(), CB_ZTS)
          FM_VAR_PARM(g_pAutoUpdate->GetFTPPassword(), CB_ZTS)
        END_PFM_CREATE
        pfmAutoUpdate->crcFileList = g_pAutoUpdate->GetFileListCRC();
        pfmAutoUpdate->nFileListSize = g_pAutoUpdate->GetFileListSize();

        // This is sort of a wacky case. It's the only case that's neither valid nor invalid.
        // That is, we neither send a logon ack, nor a logon nack, so we kind of have to munge things
        pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
        break; //client is out to autoupdate
      }
	  
	  if(pfmLogon->verLobby != LOBBYVER) {
		  fValid = false;
		  szReason = "Your game's version did not get auto-updated properly.  Please try again later.";
		  pqd->szReason = new char[lstrlen(szReason) + 1];
		  Strcpy(pqd->szReason, szReason);
	  }

      char mprthname[9]; 
	  mprSprintf(mprthname, sizeof(mprthname), "%d",pqd->dwConnectionID);
	  MprThread* threadp = new MprThread(doASGS, MPR_NORMAL_PRIORITY, (void*) pquery, mprthname); 
	  threadp->start(); //this could fail if a player is trying to login /w the same cnxn at the same time? (NYI TrapHack) - Imago 7/22/08  
    }
    break;

    case FM_C_LOGOFF_LOBBY:
    {
      // TODO: mark 'em so we know they weren't dropped
    }
    break;

	// KGJV #114
	case FM_C_GET_SERVERS_REQ:
	{
		// construct the servers list
		ListConnections::Iterator iterCnxn(*g_pLobbyApp->GetFMServers().GetConnections());
		int  cServers = 20; // fixed max servers (for now)
		ServerCoreInfo* pSCI = new ServerCoreInfo[cServers];
		int i=0;
		while (!iterCnxn.End())
		{
			CFLServer * pServerT = CFLServer::FromConnection(*iterCnxn.Value());
			if (pServerT)
			{
				if (!pServerT->GetPaused() && (pServerT->GetStaticCoreMask() != 0))// server isnt paused and has at least one core
				{
					strcpy_s(pSCI[i].szName,sizeof(pSCI[i].szName),iterCnxn.Value()->GetName());
					g_pLobbyApp->GetFMServers().GetIPAddress(*iterCnxn.Value(), pSCI[i].szRemoteAddress);
					strcpy_s(pSCI[i].szLocation,sizeof(pSCI[i].szLocation),pServerT->GetLocation());
					pSCI[i].iCurGames = pServerT->GetCurrentGamesCount();
					pSCI[i].iMaxGames = pServerT->GetMaxGamesAllowed();
					pSCI[i].dwCoreMask = pServerT->GetStaticCoreMask();
					i++;
				}
			}
			iterCnxn.Next();
			if (i>cServers) break; // dont go above max allowed servers
		}
		cServers = i;

		// construct the message
		BEGIN_PFM_CREATE(*pthis, pfmServerList, L, SERVERS_LIST)
			// fill in the master core list
			FM_VAR_PARM(g_pLobbyApp->GetcStaticCoreInfo() ? g_pLobbyApp->GetvStaticCoreInfo() : NULL, g_pLobbyApp->GetcStaticCoreInfo() * sizeof(StaticCoreInfo))
			// fill in the server list
			FM_VAR_PARM(cServers ? pSCI : NULL,cServers*sizeof(ServerCoreInfo))
        END_PFM_CREATE
		pfmServerList->cCores = g_pLobbyApp->GetcStaticCoreInfo();
		pfmServerList->cServers = cServers;

		// send the servers list
		pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
		delete [] pSCI;
		pSCI = NULL;
	}
    break;

    case FM_C_CREATE_MISSION_REQ:
    {
	  // KGJV #114
	  CASTPFM(pfmCreateMissionReq, C, CREATE_MISSION_REQ, pfm);
	  const char* szServer        = FM_VAR_REF(pfmCreateMissionReq, Server);
	  const char* szAddr          = FM_VAR_REF(pfmCreateMissionReq, Address);
	  const char* szIGCStaticFile = FM_VAR_REF(pfmCreateMissionReq, IGCStaticFile);
	  const char* szGameName      = FM_VAR_REF(pfmCreateMissionReq, GameName);

      // Find the server to host the game on
      CFLServer * pServerMin = NULL;
	  debugf("Received mission creation request from %s - create %s with %s on %s\n",
		  cnxnFrom.GetName(),
		  szGameName,
		  szIGCStaticFile,
		  szServer);
	  // loop thru servers
	  ListConnections::Iterator iterCnxn(*g_pLobbyApp->GetFMServers().GetConnections());
      while (!iterCnxn.End())
      {
        CFLServer * pServerT = CFLServer::FromConnection(*iterCnxn.Value());
		if (pServerT) // valid
		{
			if (!pServerT->GetPaused()) // not paused
			{
				char szRemoteAddress[16];
				g_pLobbyApp->GetFMServers().GetIPAddress(*iterCnxn.Value(), szRemoteAddress);
				//IMAGO REVIEW: Sanity check on szAddr first
				if (strcmp(szRemoteAddress,szAddr)==0) // IPs match
				{
					if (strcmp(iterCnxn.Value()->GetName(),szServer)==0) // names match
					{
						pServerMin = pServerT; // found it
						break;
					}
				}
			}
		}
		iterCnxn.Next();
	  }

	  // old AZ balancing code
      ////int cPlayersMin = 10000; // some big number we'll never hit      
      //int cPlayersMin = c_cMaxPlayers; 
      //debugf("Received mission creation request from %s\n", cnxnFrom.GetName());
      //ListConnections::Iterator iterCnxn(*g_pLobbyApp->GetFMServers().GetConnections());
      //while (!iterCnxn.End())
      //{
      //  CFLServer * pServerT = CFLServer::FromConnection(*iterCnxn.Value());
      //  if (!pServerT->GetPaused() && pServerT->GetPlayerCount() < cPlayersMin)
      //  {
      //    cPlayersMin = pServerT->GetPlayerCount();
      //    pServerMin = pServerT;
      //  }
      //  iterCnxn.Next();
      //}

      if (pServerMin)
      {
        CFLMission * pMission = pServerMin->CreateMission(pClient);
        BEGIN_PFM_CREATE(g_pLobbyApp->GetFMServers(), pfmNewMissionReq, L, CREATE_MISSION_REQ)
          FM_VAR_PARM(szGameName,	   CB_ZTS) // game name
          FM_VAR_PARM(szIGCStaticFile, CB_ZTS) // core name
        END_PFM_CREATE
        pfmNewMissionReq->dwCookie = pMission->GetCookie();

        // Tell the player that their mission is on the way
        BEGIN_PFM_CREATE(*pthis, pfmCreateMissionAck, L, CREATE_MISSION_ACK)
        END_PFM_CREATE
        pfmCreateMissionAck->dwCookie = pfmNewMissionReq->dwCookie;

        g_pLobbyApp->GetFMServers().SendMessages(pServerMin->GetConnection(), FM_GUARANTEED, FM_FLUSH);
        
        // TODO: check up on this mission later to make sure that we got a lobbymissioninfo from the server
      }
      else
      {
        debugf("Server not found to create the game on\n");
        BEGIN_PFM_CREATE(*pthis, pfmCreateMissionNack, L, CREATE_MISSION_NACK)
        END_PFM_CREATE
      }
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

    case FM_C_JOIN_GAME_REQ:
    {
      CASTPFM(pfmJoinGameReq, C, JOIN_GAME_REQ, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmJoinGameReq->dwCookie);
      if (pMission && pMission->GetServer()->GetPlayerCount() < c_cMaxPlayers)
      {
        BEGIN_PFM_CREATE(*pthis, pfmJoinMission, L, JOIN_MISSION)
        END_PFM_CREATE
        char szServer[16];
        g_pLobbyApp->GetFMServers().GetIPAddress(*pMission->GetServer()->GetConnection(), szServer);
        assert(lstrlen(szServer) < sizeof(pfmJoinMission->szServer)); // as long as szServer is fixed length
        Strcpy(pfmJoinMission->szServer, szServer);
		pfmJoinMission->dwPort = pMission->GetServer()->GetServerPort();	// mdvalley: pass the port to the client
        pfmJoinMission->dwCookie = pfmJoinGameReq->dwCookie;
        pfmJoinMission->guidInstance = GUID_NULL; // until we have separate sessions for each game -Imago: REVISIT
      }
      else
      {
        BEGIN_PFM_CREATE(*pthis, pfmJoingameNack, L, JOIN_GAME_NACK) 
        END_PFM_CREATE
        // client will know which one, because they're waiting for it
      }
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

    case FM_C_FIND_PLAYER:
    {
      CASTPFM(pfmFindPlayer, C, FIND_PLAYER, pfm);
      const char* szCharacterName = FM_VAR_REF(pfmFindPlayer, szCharacterName);
      CFLMission * pMissionFound = NULL;
      
      if (szCharacterName == NULL || szCharacterName[pfmFindPlayer->cbszCharacterName-1] != '\0')
      {
        // corrupt data
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_CorruptFindPlayerMsg,
          cnxnFrom.GetName(), cnxnFrom.GetID());
      }
      else
        pMissionFound = g_pLobbyApp->FindPlayersMission(szCharacterName);

      BEGIN_PFM_CREATE(*pthis, pfmFoundPlayer, L, FOUND_PLAYER)
      END_PFM_CREATE
      pfmFoundPlayer->dwCookie = pMissionFound ? pMissionFound->GetCookie() : -1; 
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
    }
    break;

    default:
      g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_UnknownMsgFromPlayer, 
        pfm->fmid, cnxnFrom.GetName(), cnxnFrom.GetID());  
  }

  return S_OK;
}


HRESULT LobbyClientSite::OnSysMessage(FedMessaging * pthis) 
{
  return S_OK;
}


void LobbyClientSite::OnMessageNAK(FedMessaging * pthis, DWORD dwTime, CFMRecipient * prcp) 
{
  debugf("ACK!! A guaranteed message didn't make it through to recipient %s.\n", prcp->GetName());
}


HRESULT LobbyClientSite::OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
	 //Here is where we want to send an async request for ASGS
  CFLClient * pClient = new CFLClient(&cnxn); 
  debugf("Player %s has connected\n", cnxn.GetName());
  g_pLobbyApp->GetCounters()->cLogins++;

  
  return S_OK;
}


HRESULT LobbyClientSite::OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  debugf("Player %s has left.\n", cnxn.GetName());
  g_pLobbyApp->GetCounters()->cLogoffs++;
  delete CFLClient::FromConnection(cnxn);
  return S_OK;
}


HRESULT LobbyClientSite::OnSessionLost(FedMessaging * pthis) 
{
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_ClientsSessionLost);
  return S_OK;
}


int LobbyClientSite::OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType)
{
  debugf("LobbyClientSite::OnMessageBox: "); 
  return g_pLobbyApp->OnMessageBox(strText, strCaption, nType);
}


#ifndef NO_MSG_CRC
void LobbyClientSite::OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg)
{
  char buf[256];
  // We don't KNOW it's a logon, but let's assume it is (it's ok if it's not)
  FMD_C_LOGON_LOBBY * pfmLogon = (FMD_C_LOGON_LOBBY *) pMsg;
  if (pfmLogon->fmid == FM_C_LOGON_LOBBY && 
    cbMsg == sizeof(FMD_C_LOGON_LOBBY) + pfmLogon->cbZoneTicket) // we'll accept it, just so that we can auto-update
  {
    // there can never be a piggy-backed message w/ FM_C_LOGON_LOBBY
    g_fLogonCRC = false;
    OnAppMessage(pthis, cnxn, pfmLogon);
  }
  else
  {
    wsprintf(buf, "HEY! We got a corrupt message!\nPlayer=%s(%d), "
           "cbmsg=%d, fmid=%d, total packet size=%d.\n"
           "Copy the above line to crashplayers.txt on \\\\zoneagga01. Going to drop player now.\n", 
           cnxn.GetName(), cnxn.GetID(),
           cbMsg >= 2 ? pfmLogon->cbmsg : 0, cbMsg >= 4 ? pfmLogon->fmid : 0, cbMsg);

    OnMessageBox(pthis, buf, "AllLobby", 0);
    pthis->DeleteConnection(cnxn); // bye bye now
  }
}
#endif

