#include "pch.h"

#include <inttypes.h> // BT - STEAM

// BT - STEAM

CSteamAchievements::CSteamAchievements(CSteamID &steamID) :
	m_steamID(steamID),
	m_gotRequestStatsResponse(false),
	m_gotSuccessfulRequestStatsResponse(false),
	m_gotStatsStoredResponse(false),
	m_gotSuccessfulStatsStoredResponse(false)
{
	
	// Do not call InitiateStatsRequest() from here, the constructor must complete first.
	
}


void CSteamAchievements::InitiateStatsRequest()
{
	// Do not block here. We are going to assume that because the login process takes some time, this call result
	// should be triggered long before anyone actually tries to hit it. Because this object will be hooked onto
	// the player object, it will be available anywhere we want to hit stats. 
	SteamAPICall_t hSteamApiCall = SteamGameServerStats()->RequestUserStats(m_steamID);
	m_UserStatsRequestedCallResult.Set(hSteamApiCall, this, &CSteamAchievements::OnUserStatsReceived);
}


// Always use this to ensure that stats are available before you try to set anything. Because this should have been loaded
// when the user logged into the server, this call should always return immediately.
bool CSteamAchievements::InitiateStatsRequestAndWaitForStatsFromSteamServer()
{
	// We only need to initialize this once per CSteamAchievements object. SteamAPI will track the stats after that.
	if (m_gotRequestStatsResponse == true)
		return true;

	SteamAPICall_t hSteamApiCall = SteamGameServerStats()->RequestUserStats(m_steamID);
	m_UserStatsRequestedCallResult.Set(hSteamApiCall, this, &CSteamAchievements::OnUserStatsReceived);

	// Wait 10 seconds max for stats to come back. This operation will block the thread, so don't want to wait too long.
	// Setting a very fast spin here so the user isn't waiting too long. 
	for (int i = 0; i < 10 * 500 && m_gotRequestStatsResponse == false; i++)
	{
		SteamGameServer_RunCallbacks();
		Sleep(5);
	}

	return m_gotRequestStatsResponse && m_gotSuccessfulRequestStatsResponse;
}

bool CSteamAchievements::GetStat(EStats theStat, int * pVal)
{
	// Must block until steam triggers the callback before you can actually use the stats. 
	InitiateStatsRequestAndWaitForStatsFromSteamServer();

	if (SteamGameServerStats()->GetUserStat(m_steamID, m_Stats[theStat], pVal) == false)
	{
		ZDebugOutput("SteamGameServerStats()->GetUserStat - response not recieved from Steam Server\n");
		return false;
	}
	return true;
}

bool CSteamAchievements::SetStat(EStats theStat, int val)
{
	// Must block until steam triggers the callback before you can actually use the stats. 
	if (InitiateStatsRequestAndWaitForStatsFromSteamServer() == false)
	{
		ZDebugOutput("InitiateStatsRequestAndWaitForStatsFromSteamServer - response not recieved from Steam Server\n");
		return false;
	}

	if (SteamGameServerStats()->SetUserStat(m_steamID, m_Stats[theStat], val) == false)
	{
		ZDebugOutput("SteamGameServerStats()->SetUserStat - Failed to set stat.\n");
		return false;
	}
	return true;
}

bool CSteamAchievements::GetAchievement(EAchievements achievement)
{
	bool toReturn;

	// Must block until steam triggers the callback before you can actually use the stats. 
	InitiateStatsRequestAndWaitForStatsFromSteamServer();

	if (SteamGameServerStats()->GetUserAchievement(m_steamID, m_Achievements[achievement], &toReturn) == false)
	{
		ZDebugOutput("SteamGameServerStats()->GetUserAchievement - response not recieved from Steam Server\n");
		return false;
	}
	return true;
}
bool CSteamAchievements::SetAchievement(EAchievements achievement)
{
	// Must block until steam triggers the callback before you can actually use the stats. 
	InitiateStatsRequestAndWaitForStatsFromSteamServer();

	if (m_gotRequestStatsResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - response not received from Steam server.\n");
		return false;
	}

	if (m_gotSuccessfulRequestStatsResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - unsuccessful response getting steam stats for user from Steam server.\n");
		return false;
	}

	// Now that we have the stats back from the server, we can update them.
	if (SteamGameServerStats()->SetUserAchievement(m_steamID, m_Achievements[achievement]) == false)
	{
		ZDebugOutput("SteamGameServerStats()->SetUserAchievement - Could not set achievement for user.\n");
		return false;
	}

	return true;
}

bool CSteamAchievements::SaveStats()
{
	SteamAPICall_t hSteamApiCall = SteamGameServerStats()->StoreUserStats(m_steamID);
	m_UserStatsStoredCallResult.Set(hSteamApiCall, this, &CSteamAchievements::OnUserStatsStored);

	return true;

	// No Need to block, Steam will guarantee completion after this point.

	//// Timeout after 10 seconds.
	//for (int i = 0; i < 100 && m_gotStatsStoredResponse == false; i++)
	//{
	//	SteamGameServer_RunCallbacks();
	//	Sleep(100);
	//}

	//if (m_gotStatsStoredResponse == false)
	//{
	//	ZDebugOutput("SteamGameServerStats()->StoreUserStats - response not received from Steam server.\n");
	//	return false;
	//}

	//if (m_gotSuccessfulStatsStoredResponse == false)
	//{
	//	ZDebugOutput("SteamGameServerStats()->StoreUserStats - unsuccessful response storing steam stats for user to steam.");
	//	return false;
	//}

	//return true;
}

void CSteamAchievements::OnUserStatsReceived(GSStatsReceived_t *pCallback, bool bIOFailure)
{
	// we may get callbacks for other user's stats arriving, ignore them
	if (m_steamID == pCallback->m_steamIDUser)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			OutputDebugStringA("Received stats and achievements from Steam\n");
			m_gotSuccessfulRequestStatsResponse = true;
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			OutputDebugStringA(buffer);
		}

		m_gotRequestStatsResponse = true;
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

// This is only for testing, and shouldn't be used normally.
bool CSteamAchievements::RemoveAchievement(EAchievements achievement)
{
	// Must block until steam triggers the callback before you can actually use the stats. 
	InitiateStatsRequestAndWaitForStatsFromSteamServer();

	if (m_gotRequestStatsResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - response not received from Steam server.\n");
		return false;
	}

	if (m_gotSuccessfulRequestStatsResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->RequestUserStats - unsuccessful response getting steam stats for user from Steam server.\n");
		return false;
	}

	// Now that we have the stats back from the server, we can update them.
	if (SteamGameServerStats()->ClearUserAchievement(m_steamID, m_Achievements[achievement]) == false)
	{
		ZDebugOutput("SteamGameServerStats()->SetUserAchievement - Could not set achievement for user.\n");
		return false;
	}

	SteamAPICall_t hSteamApiCall = SteamGameServerStats()->StoreUserStats(m_steamID);
	m_UserStatsStoredCallResult.Set(hSteamApiCall, this, &CSteamAchievements::OnUserStatsStored);

	// Timeout after 10 seconds.
	for (int i = 0; i < 100 && m_gotStatsStoredResponse == false; i++)
	{
		SteamGameServer_RunCallbacks();
		Sleep(100);
	}

	if (m_gotStatsStoredResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->StoreUserStats - response not received from Steam server.\n");
		return false;
	}

	if (m_gotSuccessfulStatsStoredResponse == false)
	{
		ZDebugOutput("SteamGameServerStats()->StoreUserStats - unsuccessful response storing steam stats for user to steam.\n");
		return false;
	}

	// If not then we can't set achievements yet
	return false;
}

void CSteamAchievements::AwardBetaParticipation()
{
	time_t  timev;
	time(&timev);

	time_t startTime = 1504567654; // 9/4/2017
	time_t endTime = 1506729599; // 9/29/2017 23:59:59 UTC

	if (timev > startTime && timev < endTime)
		SetAchievement(EAchievements::BETA_ACHIEVEMENT_1_0);
}

void CSteamAchievements::AwardKillAchievement(PilotType pt)
{
	switch (pt)
	{
	case c_ptBuilder:
	{
		SetAchievement(EAchievements::FIRST_CON_KILL_1_2);
		break;
	}
	case c_ptMiner:
	{
		SetAchievement(EAchievements::FIRST_MINER_KILL_1_1);
		break;
	}
	case c_ptPlayer:
	{
		SetAchievement(EAchievements::FIRST_FORCE_EJECT_1_3);
		break;
	}
	};
}
void CSteamAchievements::AwardBaseKillOrCapture(bool kill)
{
	if (kill)
		SetAchievement(EAchievements::FIRST_BASE_KILL_1_4);
	else //Capture
		SetAchievement(EAchievements::FIRST_BASE_CAP_1_5);

}

void CSteamAchievements::AddUserStats(PlayerScoreObject*  ppso)
{
	int tempStat;
	bool getSucceed;

	
	int minerKills = ppso->GetMinerKills();
	if (minerKills > 0)
	{
		getSucceed = GetStat(EStats::MINER_KILLS, &tempStat);
		if (getSucceed) //only set stat if get passes otherwise we risk resetting the stat
			SetStat(EStats::MINER_KILLS, tempStat + minerKills);
	}

	int conKills = ppso->GetBuilderKills();
	if (conKills > 0)
	{
		getSucceed = GetStat(EStats::CON_KILLS, &tempStat);
		if (getSucceed)
			SetStat(EStats::CON_KILLS, tempStat + conKills);
	}

	int forceEjects = ppso->GetPlayerKills();
	if (forceEjects > 0)
	{
		getSucceed = GetStat(EStats::FORCE_EJECT, &tempStat);
		if (getSucceed)
			SetStat(EStats::FORCE_EJECT, tempStat + forceEjects);
	}

	int baseKills = ppso->GetBaseKills();
	if (baseKills > 0)
	{
		getSucceed = GetStat(EStats::BASE_KILLS, &tempStat);
		if (getSucceed)
			SetStat(EStats::BASE_KILLS, tempStat + baseKills);
	}

	//int(ppso->GetBaseCaptures()), int(ppso->GetScore()), ppso->GetAssists(), ppso->);

	int baseCaps = ppso->GetBaseCaptures();
	if (baseCaps > 0)
	{
		getSucceed = GetStat(EStats::BASE_CAPS, &tempStat);
		if (getSucceed)
			SetStat(EStats::BASE_CAPS, tempStat + baseCaps);
	}

	int score = ppso->GetScore();
	if (score > 0)
	{
		getSucceed = GetStat(EStats::SUM_SCORE, &tempStat);
		if (getSucceed)
		{
			SetStat(EStats::SUM_SCORE, tempStat + score);
			CheckRank(tempStat + score);
		}
	}

	if (ppso->GetWinner())
	{
		getSucceed = GetStat(EStats::PLAYER_WINS, &tempStat);
		if (getSucceed)
			SetStat(EStats::PLAYER_WINS, tempStat + 1);
	}
	if (ppso->GetLoser())
	{
		getSucceed = GetStat(EStats::PLAYER_LOSS, &tempStat);
		if (getSucceed)
			SetStat(EStats::PLAYER_LOSS, tempStat + 1);
	}

}

void CSteamAchievements::UpdateLeaderboard(PlayerScoreObject*  ppso)
{
	MaClient client;

	char steamID[100];
	sprintf(steamID, "%" PRIu64, this->m_steamID.ConvertToUint64());
	
	char buffer[2064];
	ZString url = ZString(g.szLeaderboardUpdateUrl) + ZString("?apiKey=") + ZString(g.szApiKey);

	url += "&steamID=" + ZString(steamID);
	url += "&assists=" + ZString(ppso->GetAssists());
	url += "&baseCaptures=" + ZString(ppso->GetBaseCaptures());
	url += "&baseKills=" + ZString(ppso->GetBaseKills());
	url += "&ejects=" + ZString(ppso->GetEjections());
	url += "&kills=" + ZString(ppso->GetKills());
	url += "&score=" + ZString(ppso->GetScore());
	url += "&playtimeMinutes=" + ZString(int(ppso->GetTimePlayed() / 60));
	url += "&constructorKills=" + ZString(ppso->GetBuilderKills());
	url += "&minerKills=" + ZString(ppso->GetMinerKills());
	url += "&commandTimeMinutes=" + ZString(int(ppso->GetTimeCommanded() / 60));

	int result = client.getRequest((char *) (PCC) url);
	int response = client.getResponseCode();

	debugf("Leaderboard Update(%ld): %s\n", response, (PCC) url);
}

bool CSteamAchievements::CheckRank(int currentScore)
{
	int currentRank, earnedRank;
	bool getSucceed;
	getSucceed = GetStat(EStats::PLAYER_RANK, &currentRank);
	//getSucceed = getSucceed && GetStat(EStats::SUM_SCORE, &currentScore);
	earnedRank = currentRank;
	if (getSucceed)
	{
		if (currentScore > RANK_REQUIREMENTS[earnedRank + 1])
		{
			earnedRank++;
			SetStat(EStats::PLAYER_RANK, earnedRank);
			return true; //add return for a future level up splash
		} 
	}
	return false;
}

RankID CSteamAchievements::GetRank()
{
	int rank;
	bool getSucceed;
	getSucceed = GetStat(EStats::PLAYER_RANK, &rank);
	if (getSucceed)
		return RankID(rank);
	else
		return 0;
}