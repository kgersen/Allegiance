/*-------------------------------------------------------------------------
  Client.cpp
  
  Per client stuff, including site for Clients session
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
//
// wlp 2006 - the next line goes above the pch.h to avoid compile errors
// 
#include "appWeb.h" //Imago: HTTP Library for ASGS services
#include "pch.h"

//
// wlp 2006 - added inet client support to access ASGS token XML web service
//  the idea is to verify with ASGS that a player can play
// this is the request string that we send
// http://asgs.alleg.net/asgs/services.asmx/AuthenticateTicket?Callsign=Radar&Ticket=BunchOfAsciiStuff
// and the response expected for an invalid token is :  
// <?xml version="1.0" encoding="utf-8" ?> 
//  <int xmlns="http://ASGS.Alleg.net/ASGS/ASGS">-1</int> 
// Valid tokens will return 0 or greater than zero 
// this code is a rework of the rank code by Imago to adapt it here for tokens access
// 
//

// thia encodes the data areas of the URL for service calls
//
void encodeURL( char * url,char * token) // url = output, token gets append to url
{
    // wlp - we will do brute force URL encoding - it's normal alphaNumeric or it's URL encoded
    // we will URL encode everything that is in data areas
   
	url = url + strlen(url) ;       // point past any existing data
	
	char* tokenEnd = ( token + strlen(token)); // this is where we end                    
	for( ; token < tokenEnd ;)
	{
	//  filter out special characters 
	// IF it is not (a to z) or (A to Z) or (0 to 9) then filter it
    	if ( ((*token >= 'a') && (*token <= 'z'))     
           | ((*token >= 'A') && (*token <= 'Z')) 
           | ((*token >= '0') && (*token <= '9'))
		   )
		{
        *(url++) = *(token++) ;   // unfiltered straight transfer
		}
		else // ok this needs URL encoding with percent sign followed by ascii hex value %22
		{
        *(url++) = '%' ;  // paste in percent sign then the 2 hex bytes
		*(url++) = (( *token / 16 ) < 10) ? ( *token / 16) + '0' : ( *token /16 ) + ('a'-10) ;
        *(url++) = (( *token & 0x0F ) < 10) ? ( *(token++) & 0x0F ) + '0' : ( *(token++) & 0x0F ) + ('a'-10) ;
		}
   }
	*url = 0 ;  // terminate the string - should always be at the end
}

// wlp - make ASGS service call to validate a ticket
// also check registry to see is ASGS is off
//
BOOLEAN  getASGS(char * strName,char* playerIP,char* ASGS_Token)
{
 // check registry to see is ASGS is on or off

  DWORD dw; // Gets result of whether it opened or created... - we only read
  HKEY  hk;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, HKLM_FedSrv, 0, "", REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE, NULL, &hk, &dw) == ERROR_SUCCESS)
  {
   // read ASGS is on or off in registry
   DWORD dwASGS_ON=0;
   bool bSuccess = _Module.ReadFromRegistry(hk, false, "ASGS_ON", &dwASGS_ON, 0);
   RegCloseKey(hk);
   if (!(bSuccess && dwASGS_ON)) return true ; // ASGS is off - so let them in 
  }

    // we will use ASGS from here on

 	// Wlp - setup variables up for the socket service
	// use setTimeout to adjust the transmit timer ( probably milliseconds )
	// use setRetries to adjust the retries
    
	// appWeb Socket
	MaClient *http_client;  

	// socket variables
	int contentLen;         // length of Web Service Response
	char *szContent;        // pointer to Service response
	char szResponse[255];   // actual XML Web Service Response
    ZString strContent ;    // work buffer for response
	 
	// URL buffers
	char szURL[2000];        // Web Service ASCII Request String ( URL Encoded by us )
	
	// wlp - the URL requires URL Encoding - %22 = quote marks
	// basically any special character is encoded with percent(%) and 2 character Hex Value 
	// like %22 for double quote marks
	// this can cause the ASGS Ticket to grow from around 300 characters to 900.
	//
	// Load initial value in Request String
    Strcpy(szURL,"http://asgs.alleg.net/asgs/services.asmx/AuthenticateTicket?Callsign=");
	
	// URL encode callsign and ticket data field as we build URL string
	//
	 encodeURL(szURL,strName); // unencoded Callsign
     strcat(szURL,"&IP=");  
     encodeURL(szURL,playerIP) ;  // include Ip of requester for security
	 strcat(szURL,"&Ticket=");  
     encodeURL(szURL,ASGS_Token) ; // and the unencoded ticket
  
	// Setup the socket Client to Process requests on AppWeb threads
	Mpr   mpr("AllSrv");
	mpr.start(MPR_SERVICE_THREAD);
	http_client = new MaClient();

	// Send the Service Request
	http_client->getRequest(szURL);

	// Check the Response From Service
	if (szContent = http_client->getResponseContent(&contentLen)) 
	   {
		// if we get answered then we 
		// Check it out to see if the player was validated
		strContent = szContent;

        // shut down the Service Socket
        mpr.stop(0);
		delete http_client;

        // look for failure string at the end 
		// anything else is a pass 
		if ( strContent.ReverseFind("<int xmlns=\"http://ASGS.Alleg.net/ASGS/ASGS\">-1</int>"   ) > -1 )
		{
			debugf("ASGS player was not validated!\n");
        // log it 
        debugf("sent asgs %s \n",szURL);   // string gets too long
		debugf(" And asgs sent:  %s\n", (PCC)strContent);
    	return false ;
		} 
		else
		{
        // debugf("ASGS player was validated!\n");
		return true ;
	    }
	   } else 
	    {
		debugf("ASGS Server did not respond!\n");
		return true ;  // Let them on
	    }
}
 
// wlp 2006 end of added routine ASGS
 
const DWORD CFLClient::c_dwID = 19680815;
#ifndef NO_MSG_CRC
bool g_fLogonCRC = true; 
#endif

static GetRegDWORD(const char* szKey, DWORD dwDefault)
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
          if (plmi && (fIsFreeLobby || plmi->nNumPlayers > 0 || plmi->fMSArena
			  || (!fIsFreeLobby && strcmp(FM_VAR_REF(plmi,szIGCStaticFile),"zone_core")))) //-KGJV: advertize custom core game 
            pfm->QueueExistingMsg(plmi);
          iterMissions.Next();
        }
      }
      iterCnxn.Next();
    }
}


#ifdef USECLUB
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

	// KGJV - undeclared identifier error 
    // QueueMissions(FedMessaging * pfm);
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
#endif

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

    case FM_C_LOGON_LOBBY:
    {
      CASTPFM(pfmLogon, C, LOGON_LOBBY, pfm);
      bool fValid = false; // whether we have a valid logon
      bool fRetry = false;
      char * szReason = NULL; // when fValid==false
#ifdef USEAUTH
      LPBYTE pZoneTicket = (LPBYTE) FM_VAR_REF(pfmLogon, ZoneTicket);
      TRef<IZoneAuthServer> pzas = g_pLobbyApp->GetZoneAuthServer();
#ifdef USECLUB
      CQLobbyLogon * pquery = new CQLobbyLogon(GotLogonInfo);
      CQLobbyLogonData * pqd = pquery->GetData();
      pqd->dTime = pfmLogon->dwTime - Time::Now().clock();
      Strcpy(pqd->szCharacterName, cnxnFrom.GetName()); // unless and until we get one form the zticket
      pqd->characterID = 0;

	  // KGJV - bug fix: the SQL query crashes if pqd->szCDKey isnt initialized
	  strcpy(pqd->szCDKey,""); // so set an empty value since we dont use it
#endif
      if (pzas) // it's all in the Zone Ticket
      {
        if (pZoneTicket) 
        {
          HRESULT hr = pzas->DecryptTicket(pZoneTicket, pfmLogon->cbZoneTicket);

          switch (hr)
          {
            case ZT_NO_ERROR:
            {
              bool fValidNow = false;
#ifdef USECLUB
              Strcpy(pqd->szCharacterName, pzas->GetName());
              pqd->characterID = pzas->GetAccountID();
#endif //USECLUB
              fValid = pzas->HasToken(g_pLobbyApp->GetToken(), &fValidNow);
              // todo: keep track of players for find/zone friends
              if (!(fValid && fValidNow))
              {
                fRetry = true;
/*
                const char szExpired[] = "The Allegiance Zone subscription for %s has expired.  Hit <Sign Up> to go to the Allegiance Zone signup and subscription page, where you can renew your account subscription.";
                const char szNoToken[] = "The %s Zone account does not have a valid Allegiance Zone subscription.  Hit <Sign Up> to go to the Allegiance Zone signup and subscription page, where you can look up your account status.";
                const DWORD cbReason = 25 + max(sizeof(szNoToken), sizeof(szExpired)); // 25 = sizeof max zoneid
                szReason = (char*)_alloca(cbReason); 
                _snprintf(szReason, cbReason, (fValid ? szExpired : szNoToken), pqd->szCharacterName); 
*/
				const char szNoAuth[] = "Your authentication has failed.  Please try again.";
                const DWORD cbReason = sizeof(szNoAuth); // 25 = sizeof max zoneid
                szReason = (char*)_alloca(cbReason); 
                _snprintf(szReason, cbReason, szNoAuth); 
                fValid = false;
              }
#ifdef USECLUB // Requires bits from club SQL callback
              else if (g_pLobbyApp->EnforceCDKey())
              {
                const char * szEncryptedCDKey = (const char*) FM_VAR_REF(pfmLogon, CDKey);
                const char * szName = pzas->GetName();
                if (!szEncryptedCDKey || szEncryptedCDKey[pfmLogon->cbCDKey - 1] != '\0')
                  szEncryptedCDKey = "";

                char * szDecryptionKey = (char *)_alloca(strlen(CL_LOGON_KEY) + 12 + c_cbName);
                wsprintf(szDecryptionKey, CL_LOGON_KEY, pfmLogon->dwTime, szName);

                // note: they can get away with a replay attack here, but only 
                // if they keep the name the same.  
                ZUnscramble(pqd->szCDKey, szEncryptedCDKey, szDecryptionKey);

                ZString strOldPlayer;
                if (g_pLobbyApp->BootPlayersByCDKey(pqd->szCDKey, szName, strOldPlayer))
                {
                    fValid = false;
                    fRetry = true;
                    
                    const char szDuplicateCDKey[] = "%s was logged on with your CD Key!";
                    const DWORD cbReason = 25 + sizeof(szDuplicateCDKey); // 25 = sizeof max zoneid
                    szReason = (char*)_alloca(cbReason); 
                    _snprintf(szReason, cbReason, szDuplicateCDKey, (PCC)strOldPlayer); 
                }
              }
#endif // USECLUB
              break;
            }

            case ZT_E_BUFFER_TOO_SMALL:
              g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_MoreTokens);
              break;

            case ZT_E_AUTH_INVALID_TICKET:
            {
              char szRemoteAddress[16];
              HRESULT hr = pthis->GetIPAddress(cnxnFrom, szRemoteAddress);
              g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_BadZTicket, 
                  SUCCEEDED(hr) ? szRemoteAddress : "unknown");
              szReason = "Could not validate Zone ID.";
              break;
            }

            default:
              g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_DecryptTicketFailure, hr);
          }
        }
        else
        {
          char szRemoteAddress[16];
          HRESULT hr = pthis->GetIPAddress(cnxnFrom, szRemoteAddress);
          g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_NoCredentials,
                SUCCEEDED(hr) ? szRemoteAddress : "unknown");
          szReason = "No login credentials provided. This is the zone lobby which requires secure logins. Perhaps you are looking for the Internet Lobby.";
        }
      }
      else
#endif // USEAUTH

        fValid = true; // if this isn't secure, then you're automatically in

       if (fValid) // wlp 2006 - added token verification code
	   {
	    // wlp grab  the requester IP 
        char szPlayerIP[16];
        HRESULT hr = pthis->GetIPAddress(cnxnFrom,szPlayerIP);
        // grab the ASGS Ticket from the variable field
        char * szASGS = (char*) FM_VAR_REF(pfmLogon, ASGS_Ticket);
      
       if (!szASGS || szASGS[pfmLogon->cbASGS_Ticket - 1] != '\0') szASGS = "";
	       
        if ( !getASGS(pfmLogon->szName,szPlayerIP,szASGS) ) // Let ASGS sort it out.
	    {
	    // rejected Ticket - Bad Ticket or ASGS didn't answer
	    fValid = false;
	    szReason = "ASGS Authentication Failure.\n\n Please restart the game using ASGS\n" ;
		}
       }

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
#ifdef USECLUB
        delete pquery;
#endif // USECLUB
        break; // out of big switch
      }

      if (fValid)
      {
        // they don't need auto update
        // verify messaging version
        if(pfmLogon->verLobby != LOBBYVER)
        {
            fValid = false;
            // tell client that his version is wrong
            szReason = "Your game's version did not get auto-updated properly.  Please try again later.";
        }
      }

#ifdef USECLUB
      pqd->fValid = fValid;
      pqd->fRetry = fRetry;
      // I'd rather not alloc, but since the strings are not all static, and failure should be fairly uncommon, I'll accept it
      pqd->szReason = new char[lstrlen(szReason) + 1]; 
      Strcpy(pqd->szReason, szReason);
      pqd->dwConnectionID = cnxnFrom.GetID();

      if (fValid)
        g_pLobbyApp->GetSQL().PostQuery(pquery);
      else
        pquery->DataReady();
#else
      pthis->SetDefaultRecipient(&cnxnFrom, FM_GUARANTEED);

	  // wlp 2006 - if there are no problems then log them in
	  if (fValid)
	  {
      BEGIN_PFM_CREATE(*pthis, pfmLogonAck, L, LOGON_ACK)
      END_PFM_CREATE
      pfmLogonAck->dwTimeOffset = pfmLogon->dwTime - Time::Now().clock();
      QueueMissions(pthis);
	  }
	  else // wlp 2006 - added rejection code - reject with message
	  {
      BEGIN_PFM_CREATE(*pthis, pfmLogonNack, L, LOGON_NACK)
      FM_VAR_PARM(szReason, CB_ZTS)
      END_PFM_CREATE
      pfmLogonNack->fRetry = false;
	  } // wlp 2006 end of rejection code

      g_pLobbyApp->GetFMClients().SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);

#endif
    }
    break;

    case FM_C_LOGOFF_LOBBY:
    {
      // TODO: mark 'em so we know they weren't dropped
    }
    break;
    
    case FM_C_CREATE_MISSION_REQ:
    {
      // Choose a server to host the game on
      CFLServer * pServerMin = NULL;
      //int cPlayersMin = 10000; // some big number we'll never hit      
      int cPlayersMin = c_cMaxPlayers; 
      debugf("Received mission creation request from %s\n", cnxnFrom.GetName());
      ListConnections::Iterator iterCnxn(*g_pLobbyApp->GetFMServers().GetConnections());
      while (!iterCnxn.End())
      {
        CFLServer * pServerT = CFLServer::FromConnection(*iterCnxn.Value());
        if (!pServerT->GetPaused() && pServerT->GetPlayerCount() < cPlayersMin)
        {
          cPlayersMin = pServerT->GetPlayerCount();
          pServerMin = pServerT;
        }
        iterCnxn.Next();
      }

      if (pServerMin)
      {
        CFLMission * pMission = pServerMin->CreateMission(pClient);
        BEGIN_PFM_CREATE(g_pLobbyApp->GetFMServers(), pfmNewMissionReq, L, CREATE_MISSION_REQ)
          FM_VAR_PARM(cnxnFrom.GetName(), CB_ZTS)
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
        debugf("No servers exist to create the game on\n");
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
        lstrcpy(pfmJoinMission->szServer, szServer);
		pfmJoinMission->dwPort = pMission->GetServer()->GetServerPort();		// mdvalley: Slap the server port info in there
        pfmJoinMission->dwCookie = pfmJoinGameReq->dwCookie;
        pfmJoinMission->guidInstance = GUID_NULL; // until we have separate sessions for each game
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
  CFLClient * pClient = new CFLClient(&cnxn);
  debugf("Player %s has connected.\n", cnxn.GetName());
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

