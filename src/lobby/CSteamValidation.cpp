#include "pch.h"

#include <inttypes.h>

CSteamValidation::CSteamValidation(DWORD dwThreadID, int characterID, CSQLQuery * pQuery, CQLobbyLogonData * pqd)
{
	m_dwThreadID = dwThreadID;
	m_sqlQuery = pQuery;
	m_lobbyLogonData = pqd;
	m_characterID = characterID;
}

void CSteamValidation::BeginSteamAuthentication()
{
	EBeginAuthSessionResult result = SteamGameServer()->BeginAuthSession(m_lobbyLogonData->steamAuthTicket, m_lobbyLogonData->steamAuthTicketLength, m_lobbyLogonData->steamID);

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
		responseMessage = "Couldn't begin authorization: Game Mismatch - You need to update to the latest version.";
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
		m_lobbyLogonData->fValid = result == k_EBeginAuthSessionResultOK;
		m_lobbyLogonData->fRetry = false;
		m_lobbyLogonData->szReason = new char[strlen(responseMessage) + 1];
		m_lobbyLogonData->characterID = m_characterID;
		Strcpy(m_lobbyLogonData->szReason, responseMessage);
		LeaveCriticalSection(g_pLobbyApp->GetLogonCS());
		PostThreadMessage(_Module.dwThreadID, wm_sql_querydone, (WPARAM)NULL, (LPARAM)m_sqlQuery);

		m_bAuthenticationComplete = true;
	}

	// Hang around until steam responds.
	for (int i = 0; i < 300 && m_bAuthenticationComplete == false; i++)
	{
		Sleep(100);
	}

	if (m_bAuthenticationComplete == false)
	{
		responseMessage = "Timeout waiting for Authorization callback from Steam.";

		m_lobbyLogonData->fValid = false;
		m_lobbyLogonData->fRetry = false;
		m_lobbyLogonData->szReason = new char[strlen(responseMessage) + 1];
		m_lobbyLogonData->characterID = m_characterID;
		Strcpy(m_lobbyLogonData->szReason, responseMessage);
		LeaveCriticalSection(g_pLobbyApp->GetLogonCS());
		PostThreadMessage(_Module.dwThreadID, wm_sql_querydone, (WPARAM)NULL, (LPARAM)m_sqlQuery);
	}
}

void CSteamValidation::OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t *pResponse)
{
	char steamID[64];
	sprintf(steamID, "%" PRIu64, pResponse->m_SteamID.ConvertToUint64());

	// If the response was not for us, then just keep waiting. 
	if (pResponse->m_SteamID.ConvertToUint64() != m_lobbyLogonData->steamID)
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

	m_lobbyLogonData->fValid = pResponse->m_eAuthSessionResponse == k_EAuthSessionResponseOK;
	m_lobbyLogonData->fRetry = false;
	m_lobbyLogonData->szReason = new char[strlen(responseText) + 1];
	m_lobbyLogonData->characterID = m_characterID;
	Strcpy(m_lobbyLogonData->szReason, responseText);
	LeaveCriticalSection(g_pLobbyApp->GetLogonCS());
	PostThreadMessage(_Module.dwThreadID, wm_sql_querydone, (WPARAM)NULL, (LPARAM)m_sqlQuery);

	m_bAuthenticationComplete = true;
}