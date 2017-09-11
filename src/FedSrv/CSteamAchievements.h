#pragma once

// Note, even if you remove a value from this list, you must NEVER remove it from m_Achievements
// or the Enum to string assignments will not line up, and you will be handing out wrong achievements!
enum EAchievements
{
	BETA_ACHIEVEMENT_1_0 = 0

	// Don't forget to update g_nMaximumSteamAchievementCount!
};

const int g_nMaximumSteamAchievementCount = 1; // Always keep this in sync with the number of achievments in EAchievements!

// BT - STEAM

class CSteamAchievements
{
private:
	CSteamID m_steamID;
	bool m_gotResponse;
	bool m_gotSuccessfulResponse;
	bool m_gotStatsStoredResponse;
	bool m_gotSuccessfulStatsStoredResponse;

	CCallResult< CSteamAchievements, GSStatsStored_t > m_UserStatsStoredCallResult;


	const char * m_Achievements[g_nMaximumSteamAchievementCount] =
	{
		// Never remove an item from this list! (see note above).
		"BETA_ACHIEVEMENT_1_0"
	};

	bool SetAchievement(CSteamID &steamID, EAchievements achievement);

	// Steam Callbacks
	STEAM_GAMESERVER_CALLBACK(CSteamAchievements, OnUserStatsReceived, GSStatsReceived_t);

	


public:
	CSteamAchievements();

	bool RemoveAchievement(char *szSteamID, EAchievements achievement);

	void AwardBetaParticipation(CSteamID &steamID);
	
	// Steam Call Results
	void OnUserStatsStored(GSStatsStored_t *pCallback, bool bIOFailure);
	
};


