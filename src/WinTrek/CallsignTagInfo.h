#pragma once

// BT - STEAM
class CallsignTagInfo
{
private:
	
	ZString FixupCallsignTag(ZString callsignTag);

public:
	ZString m_callsignTag;
	ZString m_callsignToken;
	uint64	m_steamGroupID;
	int		m_index;
	bool	m_isOfficer;


	CallsignTagInfo(ZString callsignTag, uint64 steamGroupID, int index, bool isOfficer);

	CallsignTagInfo(CSteamID steamGroupID);

	CallsignTagInfo();

	void LoadFromRegistry();

	ZString Render(ZString callsign);


	inline bool operator == (const CallsignTagInfo& compare) const { return m_steamGroupID == compare.m_steamGroupID; }
	inline  bool operator > (const CallsignTagInfo& compare) const { return m_steamGroupID > compare.m_steamGroupID; }


};
