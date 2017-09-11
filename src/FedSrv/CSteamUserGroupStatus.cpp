#include "pch.h"


CSteamUserGroupStatus::CSteamUserGroupStatus(CSteamID userID, CSteamID groupID)
{
	m_groupID = groupID;
	m_userID = userID;
}

// Returns true if the user is a member of the group.
void CSteamUserGroupStatus::GetUserGroupStatus(bool &isMember, bool &isOfficer)
{
	if (SteamGameServer()->RequestUserGroupStatus(m_userID, m_groupID) == true)
	{
		// A spin lock here isn't the greatest way to pull this off. If it causes issues, this could move to a thread.
		for (int i = 0; i < 200 && m_requestComplete == false; i++)
		{
			SteamGameServer_RunCallbacks();
			Sleep(50);
		}
	}

	isMember = m_isMember;
	isOfficer = m_isOfficer;
}

void CSteamUserGroupStatus::OnUserGroupStatus(GSClientGroupStatus_t *clientGroupStatus)
{
	// Must make sure the message is for us.
	if (m_groupID == clientGroupStatus->m_SteamIDGroup && m_userID == clientGroupStatus->m_SteamIDUser)
	{
		m_isMember = clientGroupStatus->m_bMember;
		m_isOfficer = clientGroupStatus->m_bOfficer;
		m_requestComplete = true;
	}
}


bool CSteamUserGroupStatus::IsMemberOfModeratorsGroup(const char *strSteamID)
{
	uint64 steamUID = strtoull(strSteamID, NULL, 0);
	CSteamID steamID(steamUID);

	// The MSAlleg Steam Group ID. // TODO - Move this into the config file.
	CSteamID groupID((uint64)103582791460031578);

	CSteamUserGroupStatus userGroupStatus(steamID, groupID);
	bool isMember;
	bool isOfficer;
	userGroupStatus.GetUserGroupStatus(isMember, isOfficer);

	return isMember;
}
