#pragma once

// Note, even if you remove a value from this list, you must NEVER remove it from m_Achievements
// or the Enum to string assignments will not line up, and you will be handing out wrong achievements!
enum EAchievements
{
	BETA_ACHIEVEMENT_1_0 = 0,
	FIRST_MINER_KILL_1_1 = 1,
	FIRST_CON_KILL_1_2 = 2,
	FIRST_FORCE_EJECT_1_3 = 3,
	FIRST_BASE_KILL_1_4 = 4,
	FIRST_BASE_CAP_1_5 = 5

	// Don't forget to update g_nMaximumSteamAchievementCount!
};

enum EStats
{
	MINER_KILLS = 0,
	CON_KILLS = 1,
	FORCE_EJECT = 2,
	BASE_KILLS = 3,
	BASE_CAPS = 4
};
const int g_nMaximumSteamAchievementCount = 6; // Always keep this in sync with the number of achievments in EAchievements!
const int g_nMaximumSteamStatCount = 5; // Always keep this in sync with the number of stats in EStats!

// BT - STEAM

class CSteamAchievements
{
private:
	CSteamID m_steamID;
	bool m_gotRequestStatsResponse;
	bool m_gotSuccessfulRequestStatsResponse;
	bool m_gotStatsStoredResponse;
	bool m_gotSuccessfulStatsStoredResponse;

	CCallResult< CSteamAchievements, GSStatsStored_t > m_UserStatsStoredCallResult;


	const char * m_Achievements[g_nMaximumSteamAchievementCount] =
	{
		// Never remove an item from this list! (see note above).
		"BETA_ACHIEVEMENT_1_0",
		"FIRST_MINER_KILL_1_1",
		"FIRST_CON_KILL_1_2",
		"FIRST_FORCE_EJECT_1_3",
		"FIRST_BASE_KILL_1_4",
		"FIRST_BASE_CAP_1_5"
	};

	const char * m_Stats[g_nMaximumSteamStatCount] =
	{
		// Never remove an item from this list! (see note above).
		"MINER_KILLS",
		"CON_KILLS",
		"FORCE_EJECT",
		"BASE_KILLS",
		"BASE_CAPS"
	};

	

	bool GetAchievement(EAchievements achievement);
	bool SetAchievement(EAchievements achievement);
	//bool GetStat(CSteamID &steamID, EStats theStat);
	bool GetStat(EStats theStat, int * pVal);
	bool SetStat(EStats theStat, int val);
	bool InitiateStatsRequestAndWaitForStatsFromSteamServer();
	

	// Steam Callbacks
	STEAM_GAMESERVER_CALLBACK(CSteamAchievements, OnUserStatsReceived, GSStatsReceived_t);

	


public:
	CSteamAchievements(CSteamID &steamID);

	bool RemoveAchievement(EAchievements achievement);

	void AwardBetaParticipation();
	void AwardKillAchievement(PilotType pt);
	
	// Steam Call Results
	void OnUserStatsStored(GSStatsStored_t *pCallback, bool bIOFailure);

	void AddUserStats(int minerKills, int conKills, int forceEjects, int baseKills, int baseCaps);

	bool SaveStats();
	
};


