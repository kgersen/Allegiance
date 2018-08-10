#pragma once

namespace AllegianceInterop
{
	public enum ConnectionType
	{
		Lobby,
		Server
	};

	//// From igc.h
	public enum class MissionStage
	{
		STAGE_NOTSTARTED,
		STAGE_STARTING,
		STAGE_STARTED,
		STAGE_OVER,
		STAGE_TERMINATE
	};

	public enum class LockdownCriteria
	{
		lockdownDonating = 1,
		lockdownLoadout = 2,
		lockdownTeleporting = 4
	};

	// From igc.h
	public enum class ChatTarget // if you change this please update AGCChatTarget in AGCIDL.idl
	{
		CHAT_EVERYONE = 0, CHAT_LEADERS, CHAT_ADMIN, CHAT_SHIP,
		CHAT_ALLIES, //imago added allies 7/3/09 ALLY
		CHAT_TEAM, CHAT_INDIVIDUAL, CHAT_INDIVIDUAL_NOFILTER, CHAT_WING, CHAT_INDIVIDUAL_ECHO,     //require objectID to be set
		CHAT_ALL_SECTOR, CHAT_FRIENDLY_SECTOR,                      //ditto
		CHAT_GROUP, CHAT_GROUP_NOECHO,                               //client only ... get translated into multiple sends
		CHAT_NOSELECTION,
		CHAT_MAX
	};
}