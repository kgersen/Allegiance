#pragma once

class CSteamUserGroupStatus
{
private:
	CSteamID m_userID;
	CSteamID m_groupID;

	bool m_isMember = false;
	bool m_isOfficer = false;
	bool m_requestComplete = false;

	void GetUserGroupStatus(bool &isMember, bool &isOfficer);

	STEAM_GAMESERVER_CALLBACK(CSteamUserGroupStatus, OnUserGroupStatus, GSClientGroupStatus_t);

public:
	CSteamUserGroupStatus(CSteamID userID, CSteamID groupID);

	static bool IsMemberOfModeratorsGroup(const char *steamID);
	static bool IsMemberOfDevelopersGroup(CSteamID steamID);

};