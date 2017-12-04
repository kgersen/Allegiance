#include "pch.h"

#include <inttypes.h>

CSteamValidation::CSteamValidation(DWORD dwThreadID, int characterID, CSQLQuery * pQuery, CQLogonStatsData * pqd)
{
	m_dwThreadID = dwThreadID;
	m_sqlQuery = pQuery;
	m_logonStatsData = pqd;
	m_characterID = characterID;
}

void CSteamValidation::BeginSteamAuthentication()
{
	EBeginAuthSessionResult result = SteamGameServer()->BeginAuthSession(m_logonStatsData->steamAuthTicket, m_logonStatsData->steamAuthTicketLength, m_logonStatsData->steamID);

	ZString responseMessage;

	switch (result)
	{
	case k_EBeginAuthSessionResultOK:
		responseMessage = "Authentication In Progress.";
		break;

	case k_EBeginAuthSessionResultInvalidTicket:
		responseMessage = "Couldn't begin authorization: Invalid Ticket.";
		break;

	case k_EBeginAuthSessionResultDuplicateRequest:
		responseMessage = "Couldn't begin authorization: Duplicate Request.";
		break;

	case k_EBeginAuthSessionResultInvalidVersion:
		responseMessage = "Couldn't begin authorization: Invalid Version.";
		break;

	case k_EBeginAuthSessionResultGameMismatch:
		responseMessage = "Couldn't begin authorization: Game Mismatch.";
		break;

	case k_EBeginAuthSessionResultExpiredTicket:
		responseMessage = "Couldn't begin authorization: Expired Ticket.";
		break;

	default:
		responseMessage = "Couldn't begin authorization: unknown error code returned: " + result;
		break;
	}

	if (result != k_EBeginAuthSessionResultOK)
	{
		// If we failed to start the Steam Auth, then give a query response now. Otherwise, the querey response will be triggered when call back succeeds.
		m_logonStatsData->fValid = result == k_EBeginAuthSessionResultOK;
		m_logonStatsData->fRetry = false;
		//m_logonStatsData->szReason = new char[strlen(responseMessage) + 1];
		m_logonStatsData->characterID = m_characterID;
		
		Strcpy(m_logonStatsData->szReason, responseMessage);
		//LeaveCriticalSection(g_pLobbyApp->GetLogonCS());
		PostThreadMessage(m_dwThreadID, wm_sql_querydone, (WPARAM)NULL, (LPARAM)m_sqlQuery);

		m_bAuthenticationComplete = true;
		return;
	}

	// Hang around until steam responds.
	for (int i = 0; i < 300 && m_bAuthenticationComplete == false; i++)
	{
		// This wait time can block up the main receive thread pump which stops the SteamGameServer_RunCallbacks from happening. 
		// This should help decrease the login wait time.
		SteamGameServer_RunCallbacks();
		Sleep(100);
	}

	if (m_bAuthenticationComplete == false)
	{
		responseMessage = "Timeout waiting for Authorization callback from Steam.";

		m_logonStatsData->fValid = false;
		m_logonStatsData->fRetry = false;
		//m_logonStatsData->szReason = new char[strlen(responseMessage) + 1];
		m_logonStatsData->characterID = m_characterID;
		Strcpy(m_logonStatsData->szReason, responseMessage);
		//LeaveCriticalSection(g_pLobbyApp->GetLogonCS());
		PostThreadMessage(m_dwThreadID, wm_sql_querydone, (WPARAM)NULL, (LPARAM)m_sqlQuery);
	}
	else
	{
		if (strlen(m_logonStatsData->szPassword) == 0)
			strcpy(m_logonStatsData->szPassword, "PORKPORKPORK1234");

		// Kick off the rank retrieval from the lobby. The response from that request will complete the client logon, and then the user will be allowed into the server.
		/*BEGIN_PFM_CREATE(g.fmLobby, pfmPlayerRank, LS, PLAYER_RANK)
		FM_VAR_PARM(PCC(m_logonStatsData->szCharacterName), CB_ZTS)
		FM_VAR_PARM(PCC(responseText), CB_ZTS)
		FM_VAR_PARM(PCC(m_logonStatsData->szPassword), CB_ZTS)
		FM_VAR_PARM(PCC(m_logonStatsData->szCDKey), CB_ZTS)
		END_PFM_CREATE

		pfmPlayerRank->characterID = m_logonStatsData->characterID;
		pfmPlayerRank->fCanCheat = m_logonStatsData->fCanCheat;
		pfmPlayerRank->fRetry = m_logonStatsData->fRetry;
		pfmPlayerRank->dwCookie = m_logonStatsData->dwCookie;
		pfmPlayerRank->fValid = pResponse->m_eAuthSessionResponse == k_EAuthSessionResponseOK;
		pfmPlayerRank->dwConnectionID = m_logonStatsData->dwConnectionID;
		g.fmLobby.SendMessages(g.fmLobby.GetServerConnection(), FM_GUARANTEED, FM_FLUSH);*/

		strcpy(m_logonStatsData->szReason, m_responseText);

		m_logonStatsData->characterID = m_logonStatsData->characterID;
		m_logonStatsData->fCanCheat = m_logonStatsData->fCanCheat;
		m_logonStatsData->fRetry = m_logonStatsData->fRetry;
		m_logonStatsData->dwCookie = m_logonStatsData->dwCookie;
		m_logonStatsData->fValid = m_isValid;
		m_logonStatsData->dwConnectionID = m_logonStatsData->dwConnectionID;

		if (m_isValid == true)
			m_logonStatsData->rank = GetRankForSteamID(m_logonStatsData->steamID);
		else
			m_logonStatsData->rank = 0;

		m_logonStatsData->sigma = 0;
		m_logonStatsData->mu = 0;
		m_logonStatsData->commandRank = 0;
		m_logonStatsData->commandSigma = 0;
		m_logonStatsData->commandMu = 0;

		PostThreadMessage(g.idReceiveThread, wm_sql_querydone, (WPARAM)NULL, (LPARAM)m_sqlQuery);
	}

}

void CSteamValidation::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t *pResponse)
{
	char steamID[64];
	sprintf(steamID, "%" PRIu64, pResponse->m_SteamID.ConvertToUint64());

	// If the response was not for us, then just keep waiting. 
	if (pResponse->m_SteamID.ConvertToUint64() != m_logonStatsData->steamID)
		return;

	ZString strSteamID = steamID;

	ZString responseText;

	switch (pResponse->m_eAuthSessionResponse)
	{
	case k_EAuthSessionResponseOK:
		responseText = "Steam has verified the user is online, the ticket is valid and ticket has not been reused.";
		break;

	case k_EAuthSessionResponseUserNotConnectedToSteam:
		responseText = "The user in question is not connected to steam.";
		break;

	case k_EAuthSessionResponseNoLicenseOrExpired:
		responseText = "The license has expired.";
		break;

	case k_EAuthSessionResponseVACBanned:
		responseText = "The user is VAC banned for this game.";
		break;

	case k_EAuthSessionResponseLoggedInElseWhere:
		responseText = "The user account has logged in elsewhere and the session containing the game instance has been disconnected.";
		break;

	case k_EAuthSessionResponseVACCheckTimedOut:
		responseText = "VAC has been unable to perform anti-cheat checks on this user";
		break;

	case k_EAuthSessionResponseAuthTicketCanceled:
		responseText = "The ticket has been canceled by the issuer";
		break;

	case k_EAuthSessionResponseAuthTicketInvalidAlreadyUsed:
		responseText = "This ticket has already been used, it is not valid.";
		break;

	case k_EAuthSessionResponseAuthTicketInvalid:
		responseText = "This ticket is not from a user instance currently connected to steam.";
		break;

	case k_EAuthSessionResponsePublisherIssuedBan:
		responseText = "The user is banned for this game. The ban came via the web api and not VAC.";
		break;

	default:
		responseText = "Unknown response code: " + ZString((int)pResponse->m_eAuthSessionResponse);
		break;
	}

	ZDebugOutput("Steam Validation Response Received for steamID: " + strSteamID + ", response code: " + ZString(pResponse->m_eAuthSessionResponse)  + " - " + responseText + "\n");

	m_responseText = responseText;
	m_isValid = pResponse->m_eAuthSessionResponse == k_EAuthSessionResponseOK;

	m_bAuthenticationComplete = true;
}



RankID CSteamValidation::GetRankForSteamID(CSteamID steamID)
{
	CSteamAchievements achievementsForPlayer(steamID);
	RankID rank;
	rank = achievementsForPlayer.GetRank();
	return rank;
		
	//return 0;
}