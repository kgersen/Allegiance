#include "pch.h"

// BT - STEAM

CSteamAchievements::CSteamAchievements() :
	m_gotResponse(false),
	m_gotSuccessfulResponse(false),
	m_gotStatsStoredResponse(false),
	m_gotSuccessfulStatsStoredResponse(false)

{
}


//bool CSteamAchievements::RequestStats()
//{
//	// Is Steam loaded? If not we can't get stats.
//	if (NULL == SteamUserStats() || NULL == SteamUser())
//	{
//		return false;
//	}
//	// Is the user logged on?  If not we can't get stats.
//	if (!SteamUser()->BLoggedOn())
//	{
//		return false;
//	}
//	// Request user stats.
//	return SteamUserStats()->RequestCurrentStats();
//}

bool CSteamAchievements::GetAchievement(CSteamID &steamID, EAchievements achievement)
{
	m_steamID = steamID;
	m_gotResponse = false;
	bool toReturn;
	SteamGameServerStats()->RequestUserStats(steamID);
	m_gotResponse = SteamGameServerStats()->GetUserAchievement(m_steamID, m_Achievements[achievement], &toReturn);
	return toReturn;
}
bool CSteamAchievements::SetAchievement(CSteamID &steamID, EAchievements achievement)
{
	m_steamID = steamID;
	m_gotResponse = false;

	SteamGameServerStats()->RequestUserStats(steamID);

	// Timeout after 10 seconds.
	for (int i = 0; i < 100 && m_gotResponse == false; i++)
	{
		SteamGameServer_RunCallbacks();
		Sleep(100);
	}

	if (m_gotResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - response not received from Steam server.");
		return false;
	}

	if (m_gotSuccessfulResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - unsuccessful response getting steam stats for user from steam server.");
		return false;
	}

	// Now that we have the stats back from the server, we can update them.
	if (SteamGameServerStats()->SetUserAchievement(steamID, m_Achievements[achievement]) == false)
	{
		ZDebugOutput("SteamGameServerStats()->SetUserAchievement - Could not set achievement for user..");
		return false;
	}

	SteamAPICall_t hSteamApiCall = SteamGameServerStats()->StoreUserStats(steamID);
	m_UserStatsStoredCallResult.Set(hSteamApiCall, this, &CSteamAchievements::OnUserStatsStored);
	
	// Timeout after 10 seconds.
	for (int i = 0; i < 600 && m_gotStatsStoredResponse == false; i++)
	{
		SteamGameServer_RunCallbacks();
		Sleep(100);
	}

	if (m_gotStatsStoredResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->StoreUserStats - response not received from Steam server.");
		return false;
	}

	if (m_gotSuccessfulStatsStoredResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->StoreUserStats - unsuccessful response storing steam stats for user to steam.");
		return false;
	}

	// If not then we can't set achievements yet
	return false;
}

void CSteamAchievements::OnUserStatsReceived(GSStatsReceived_t *pCallback)
{
	// we may get callbacks for other user's stats arriving, ignore them
	if (m_steamID == pCallback->m_steamIDUser)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugStringA("Received stats and achievements from Steam\n");
			m_gotSuccessfulResponse = true;
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			OutputDebugStringA(buffer);
		}

		m_gotResponse = true;
	}
}

void CSteamAchievements::OnUserStatsStored(GSStatsStored_t *pCallback, bool bIOFailure)
{
	// we may get callbacks for other games' stats arriving, ignore them
	if (m_steamID == pCallback->m_steamIDUser)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			m_gotSuccessfulStatsStoredResponse = true;
			OutputDebugStringA("Stored stats for Steam\n");
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult);
			OutputDebugStringA(buffer);
		}

		m_gotStatsStoredResponse = true;
	}
}

bool CSteamAchievements::RemoveAchievement(char *szSteamID, EAchievements achievement)
{
	CSteamID steamID(strtoull(szSteamID, NULL, 0));
	m_steamID = steamID;
	m_gotResponse = false;

	SteamGameServerStats()->RequestUserStats(steamID);

	// Timeout after 10 seconds.
	for (int i = 0; i < 100 && m_gotResponse == false; i++)
	{
		SteamGameServer_RunCallbacks();
		Sleep(100);
	}

	if (m_gotResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - response not received from Steam server.");
		return false;
	}

	if (m_gotSuccessfulResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - unsuccessful response getting steam stats for user from steam server.");
		return false;
	}

	// Now that we have the stats back from the server, we can update them.
	if (SteamGameServerStats()->ClearUserAchievement(steamID, m_Achievements[achievement]) == false)
	{
		ZDebugOutput("SteamGameServerStats()->SetUserAchievement - Could not set achievement for user..");
		return false;
	}

	SteamAPICall_t hSteamApiCall = SteamGameServerStats()->StoreUserStats(steamID);
	m_UserStatsStoredCallResult.Set(hSteamApiCall, this, &CSteamAchievements::OnUserStatsStored);

	// Timeout after 10 seconds.
	for (int i = 0; i < 600 && m_gotStatsStoredResponse == false; i++)
	{
		SteamGameServer_RunCallbacks();
		Sleep(100);
	}

	if (m_gotStatsStoredResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->StoreUserStats - response not received from Steam server.");
		return false;
	}

	if (m_gotSuccessfulStatsStoredResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->StoreUserStats - unsuccessful response storing steam stats for user to steam.");
		return false;
	}

	// If not then we can't set achievements yet
	return false;
}

void CSteamAchievements::AwardBetaParticipation(CSteamID &steamID)
{
	time_t  timev;
	time(&timev);

	time_t startTime = 1504567654; // 9/4/2017
	time_t endTime = 1505537940; // 9/15/2017 23:59:59

	if (timev > startTime && timev < endTime)
		SetAchievement(steamID, EAchievements::BETA_ACHIEVEMENT_1_0);
}
void CSteamAchievements::AwardKillAchievement(CSteamID &steamID, PilotType pt)
{
	switch (pt)
	{
	case c_ptBuilder:
	{
		//if (!GetAchievement(steamID, EAchievements::FIRST_CON_KILL_1_2))
			SetAchievement(steamID, EAchievements::FIRST_CON_KILL_1_2);
		break;
	}
	case c_ptMiner:
	{
		//if (!GetAchievement(steamID, EAchievements::FIRST_MINER_KILL_1_1))
			SetAchievement(steamID, EAchievements::FIRST_MINER_KILL_1_1);
		break;
	}
	case c_ptPlayer:
	{
		//if (!GetAchievement(steamID, EAchievements::FIRST_FORCE_EJECT_1_3))
			SetAchievement(steamID, EAchievements::FIRST_FORCE_EJECT_1_3);
		break;
	}
	};
}
