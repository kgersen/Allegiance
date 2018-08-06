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
}