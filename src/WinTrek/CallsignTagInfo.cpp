#include "pch.h"

// BT - STEAM

#include "CallsignTagInfo.h"



CallsignTagInfo::CallsignTagInfo(ZString callsignTag, uint64 steamGroupID, int index, bool isOfficer) :
	m_callsignTag(callsignTag),
	m_steamGroupID(steamGroupID),
	m_index(index),
	m_isOfficer(isOfficer)
{
	m_callsignTag = FixupCallsignTag(m_callsignTag);
}

CallsignTagInfo::CallsignTagInfo(CSteamID steamGroupID)
{
	
}

CallsignTagInfo::CallsignTagInfo()
{
	m_callsignTag = "";
	m_steamGroupID = 0;
	m_index = -1;
}

ZString CallsignTagInfo::FixupCallsignTag(ZString callsignTag)
{
	ZString returnValue = callsignTag;

	// Allows us to have some group tags that may have already been claimed.
	if (returnValue.Find("@") >= 0)
		returnValue = returnValue.RightOf("@");

	if (returnValue.Find("#") >= 0)
		returnValue = returnValue.RightOf("#");

	if (returnValue.Find("!") >= 0)
		returnValue = returnValue.RightOf("!");

	if (returnValue.Find("$") >= 0)
		returnValue = returnValue.RightOf("$");

	// It's tradition!
	if (returnValue == "MsAlleg")
		returnValue = "Alleg";

	// Give our older members a nice badge of honor.
	if (returnValue == "AllegFAO")
		returnValue = "Vet";

	return returnValue;
}

ZString CallsignTagInfo::Render(ZString callsign)
{
	ZString returnValue = callsign;

	if (m_isOfficer == true)
		returnValue = m_callsignToken + returnValue;

	if (m_callsignTag.GetLength() > 0)
		returnValue = returnValue + ZString("@") + m_callsignTag;

	if (returnValue.GetLength() > 25)
		returnValue = returnValue.Left(25);

	return returnValue;
}


void CallsignTagInfo::LoadFromRegistry()
{
	char szSteamClanID[64];
	szSteamClanID[0] = '\0';
	DWORD cbClanID = sizeof(szSteamClanID);

	char szSteamOfficerToken[5];
	szSteamOfficerToken[0] = '\0';
	DWORD cbSteamOfficerToken = sizeof(szSteamOfficerToken);

	HKEY hKey;
	DWORD dwType;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
	{
		RegQueryValueEx(hKey, "SteamClanID", NULL, &dwType, (unsigned char*)&szSteamClanID, &cbClanID);
		RegQueryValueEx(hKey, "SteamOfficerToken", NULL, &dwType, (unsigned char*)&szSteamOfficerToken, &cbSteamOfficerToken);
		RegCloseKey(hKey);
	}

	m_isOfficer = false;
	m_callsignTag = "";
	m_index = 0;
	m_steamGroupID = 0;
	
	CSteamID targetGroupID(strtoull(szSteamClanID, NULL, NULL));

	CSteamID currentUser = SteamUser()->GetSteamID();

	int clanCount = SteamFriends()->GetClanCount();
	for (int i = 0; i < clanCount; i++)
	{
		if (targetGroupID == SteamFriends()->GetClanByIndex(i))
		{
			m_callsignTag = FixupCallsignTag(SteamFriends()->GetClanTag(targetGroupID));

			int officerCount = SteamFriends()->GetClanOfficerCount(targetGroupID);
			for (int j = 0; j < officerCount; j++)
			{
				if (SteamFriends()->GetClanOfficerByIndex(targetGroupID, j) == currentUser)
				{
					m_callsignToken = szSteamOfficerToken;
					m_isOfficer = true;
					break;
				}
			}
		}
	}
}