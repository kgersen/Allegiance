#pragma once

class CSteamValidation
{

private:
	DWORD			m_dwThreadID;
	CSQLQuery *		m_sqlQuery;
	CQLobbyLogonData *	m_lobbyLogonData;
	int				m_characterID;
	bool			m_bAuthenticationComplete = false;

public:
	CSteamValidation(DWORD dwThreadID, int characterID, CSQLQuery * pQuery, CQLobbyLogonData * pqd);

	void BeginSteamAuthentication();

private:
	// Tells us a client has been authenticated and approved to play by Steam (passes auth, license check, VAC status, etc...)
	STEAM_GAMESERVER_CALLBACK(CSteamValidation, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t);

	void OnValidateAuthTicketResponse(CSteamID m_SteamID, EAuthSessionResponse eAuthSessionResponse, CSteamID ownerSteamID);


};