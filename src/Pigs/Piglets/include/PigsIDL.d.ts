
// ***
// *** 
// *** This is a generated file. Please do not modify this file by hand or your changes will be lost.
// ***
// ***

interface IDispatch { }
interface IDictionary { }
interface IUnknown { }

type Guid = string;



declare namespace Enums
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// PigState Enum

	/**
	 * Constants for pig player states.
	 */
	enum PigState
		{
		PigState_NonExistant,
		PigState_LoggingOn,
		PigState_LoggingOff,
		PigState_MissionList,
		PigState_CreatingMission,
		PigState_JoiningMission,
		PigState_QuittingMission,
		PigState_TeamList,
		PigState_JoiningTeam,
		PigState_WaitingForMission,
		PigState_Docked,
		PigState_Launching,
		PigState_Flying,
		PigState_Terminated,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigMapType Enum

	/**
	 * Constants for mission map types.
	 */
	enum PigMapType
		{
		PigMapType_SingleRing,
		PigMapType_DoubleRing,
		PigMapType_PinWheel,
		PigMapType_DiamondRing,
		PigMapType_Snowflake,
		PigMapType_SplitBases,
		PigMapType_Brawl,
		PigMapType_Count,
		PigMapType_Default,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigLobbyMode Enum

	/**
	 * Constants for pig lobby server mode.
	 */
	enum PigLobbyMode
		{
		PigLobbyMode_Club,
		PigLobbyMode_Free,
		PigLobbyMode_LAN,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PigThrust Enum

	/**
	 * Constants for pig thrust actions.
	 */
	enum PigThrust
		{
		ThrustNone,
		ThrustLeft,
		ThrustRight,
		ThrustUp,
		ThrustDown,
		ThrustForward,
		ThrustBackward,
		ThrustBooster,
	}

}




////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehaviorType Interface

/**
 * Interface to a pig behavior type object.
 */
interface IPigBehaviorType extends IDispatch
{
	/**
	 * READONLY: Gets the collection of command strings that will invoke this behavior.
	 */
	readonly InvokeCommands: ITCStrings;
	/**
	 * READONLY: Gets the name of this behavior type.
	 */
	readonly Name: string;
	/**
	 * READONLY: Gets the description of this behavior type.
	 */
	readonly Description: string;
	/**
	 * READONLY: Gets the collection of pig behavior objects currently referencing this behavior type.
	 */
	readonly Behaviors: IPigBehaviors;
	/**
	 * READONLY: Gets the indicator of whether or not this object is based on a script or not.
	 */
	readonly IsScript: boolean;
	/**
	 * READONLY: Gets the behavior type that serves as this one's base class.
	 */
	readonly BaseBehaviorType: IPigBehaviorType;
	/**
	 * Gets/sets the flag indicating whether or not the script has encountered an error yet.
	 */
	AppearsValid(bAppearsValid: boolean): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehaviorTypes Interface

/**
 * Interface to a collection of pig behavior script objects.
 */
interface IPigBehaviorTypes extends ITCCollection
{
	/**
	 * Gets an item from the collection. Takes an argument, index, that is either the 0-relative index into the collection, the name of the behavior, or one of the invoke commands of the behavior.
	 */
	Item(pvIndex: object): IPigBehaviorType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehaviorScriptType Interface

/**
 * Interface to a pig behavior script object.
 */
interface IPigBehaviorScriptType extends IPigBehaviorType
{
	/**
	 * READONLY: Gets the path and name of the script file that defines this behavior.
	 */
	readonly FilePathName: string;
	/**
	 * READONLY: Gets the name of the script file that defines this behavior.
	 */
	readonly FileName: string;
	/**
	 * READONLY: Gets the time (UTC) that the script file was last modified.
	 */
	readonly FileModifiedTime: Date;
	/**
	 * READONLY: Gets the attributes of the script file.
	 */
	readonly FileAttributes: Enums.FileAttributes;
	/**
	 * READONLY: Gets the text of the entire script file.
	 */
	readonly FileText: string;
	/**
	 * READONLY: Gets the text of the script sections within the file.
	 */
	readonly ScriptText: ITCStrings;
	/**
	 * READONLY: Gets the Prog ID of the Active Scripting engine used to parse/execute this script.
	 */
	readonly ScriptEngineProgID: string;
	/**
	 * READONLY: Gets the CLSID of the Active Scripting engine used to parse/execute this script.
	 */
	readonly ScriptEngineCLSID: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehaviorHost Interface

/**
 * Interface to a pig behavior host object.
 */
interface IPigBehaviorHost extends IDispatch
{
	/**
	 * Useful test method that beeps as specified.
	 */
	Beep(nFrequency: number, nDuration: number): number;
	/**
	 * Creates a new pig object with the specified behavior type.
	 */
	CreatePig(bstrType: string, bstrCommandLine: string): IPig;
	/**
	 * Useful test method that echoes the specified text to the debugger output.
	 */
	Trace(bstrText: string): string;
	/**
	 * Gets the textual name of the specified pig state.
	 */
	StateName(ePigState: Enums.PigState): string;
	/**
	 * Generates a random integer between 0 and 32,767.
	 */
	Random(): number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehavior Interface

/**
 * Interface to a pig behavior object.
 */
interface IPigBehavior extends IDispatch
{
	/**
	 * READONLY: Gets the object that describes this class of behavior.
	 */
	readonly BehaviorType: IPigBehaviorType;
	/**
	 * READONLY: Get the behavior, if any, that serve's as the this one's base class.
	 */
	readonly BaseBehavior: IDispatch;
	/**
	 * READONLY: Gets the pig player object using this behavior instance.
	 */
	readonly Pig: IPig;
	/**
	 * READONLY: Gets the object used to access the rest of the Pigs object model.
	 */
	readonly Host: IPigBehaviorHost;
	/**
	 * READONLY: Gets the indicator of whether or not this behaior is the one currently active for the pig player object.
	 */
	readonly IsActive: boolean;
	/**
	 * READONLY: Gets the dictionary containing the behavior-defined properties.
	 */
	readonly Properties: IDictionary;
	/**
	 * READONLY: Gets the timer for which the event expression is currently executing, if any
	 */
	readonly Timer: IPigTimer;
	/**
	 * READONLY: Gets the command line text used to activate the behvaior, if any.
	 */
	readonly CommandLine: string;
	/**
	 * Creates a new timer object.
	 */
	CreateTimer(fInterval: number, bstrEventExpression: string, nRepetitions: number, bstrName: string): IPigTimer;
	/**
	 * Called when this behavior becomes a pig's active behavior.
	 */
	OnActivate(pPigDeactived: IPigBehavior): IPigBehavior;
	/**
	 * Called when this behavior is deactivated as a pig's active behavior.
	 */
	OnDeactivate(pPigActivated: IPigBehavior): IPigBehavior;
	/**
	 * Called when the state of the pig has changed to PigState_NonExistant.
	 */
	OnStateNonExistant(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_LoggingOn.
	 */
	OnStateLoggingOn(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_LoggingOff.
	 */
	OnStateLoggingOff(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_MissionList.
	 */
	OnStateMissionList(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_CreatingMission.
	 */
	OnStateCreatingMission(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_JoiningMission.
	 */
	OnStateJoiningMission(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_QuittingMission.
	 */
	OnStateQuittingMission(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_TeamList.
	 */
	OnStateTeamList(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_JoiningTeam.
	 */
	OnStateJoiningTeam(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_WaitingForMission.
	 */
	OnStateWaitingForMission(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_Docked.
	 */
	OnStateDocked(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_Launching.
	 */
	OnStateLaunching(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_Flying.
	 */
	OnStateFlying(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the state of the pig has changed to PigState_Terminated.
	 */
	OnStateTerminated(eStatePrevious: Enums.PigState): Enums.PigState;
	/**
	 * Called when the Pig's current mission begins.
	 */
	OnMissionStarted();
	/**
	 * Called when the Pig receives chat text.
	 */
	OnReceiveChat(bstrText: string, pShipSender: IAGCShip): boolean;
	/**
	 * Called when any ship takes damage.
	 */
	OnShipDamaged(pShip: IAGCShip, pModelLauncher: IAGCModel, fAmount: number, fLeakage: number, pVector1: IAGCVector, pVector2: IAGCVector): IAGCVector;
	/**
	 * Called when any ship is killed.
	 */
	OnShipKilled(pShip: IAGCShip, pModelLauncher: IAGCModel, fAmount: number, pVector1: IAGCVector, pVector2: IAGCVector): IAGCVector;
	/**
	 * Called when the Pig ship is transported to a new Sector.
	 */
	OnSectorChange(pSectorOld: IAGCSector, pSectorNew: IAGCSector): IAGCSector;
	/**
	 * Called when the Pig ship hits an Aleph.
	 */
	OnAlephHit(pAleph: IAGCAleph): IAGCAleph;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehaviors Interface

/**
 * Interface to a collection of pig behavior objects.
 */
interface IPigBehaviors extends ITCCollection
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigBehaviorStack Interface

/**
 * Interface to a stack of pig behavior objects.
 */
interface IPigBehaviorStack extends ITCCollection
{
	/**
	 * Creates a new behavior of the specified type and pushes it onto the top of the behavior stack.
	 */
	Push(bstrType: string, bstrCommandLine: string): IPigBehavior;
	/**
	 * Pops the top-most behavior off the stack, unless there is only one behavior remaining on the stack.
	 */
	Pop();
	/**
	 * Gets a reference to a behavior on the stack without removing it from the stack.
	 */
	Top(nFromTop: number): IPigBehavior;
	/**
	 * READONLY: Gets the pig object which owns this behavior stack.
	 */
	readonly Pig: IPig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPig Interface

/**
 * Interface to a pig player object.
 */
interface IPig extends IDispatch
{
	/**
	 * READONLY: Gets the current state of the pig.
	 */
	readonly PigState: Enums.PigState;
	/**
	 * READONLY: Gets the textual name of the state of the pig.
	 */
	readonly PigStateName: string;
	/**
	 * READONLY: Gets the stack of behaviors associated with this pig.
	 */
	readonly Stack: IPigBehaviorStack;
	/**
	 * READONLY: Gets the character name of the pig.
	 */
	readonly Name: string;
	/**
	 * READONLY: Gets the ship object of the pig.
	 */
	readonly Ship: IPigShip;
	/**
	 * Gets/sets the number of times per second that the IGC layer is updated.
	 */
	UpdatesPerSecond(nPerSecond: number): number;
	/**
	 * READONLY: Gets the collection of hull types available to this pig.
	 */
	readonly HullTypes: IPigHullTypes;
	/**
	 * READONLY: Gets this pig's liquid worth.
	 */
	readonly Money: AGCMoney;
	/**
	 * Logs on to the mission server.
	 */
	Logon();
	/**
	 * Logs off from the lobby server.
	 */
	Logoff();
	/**
	 * Creates (and joins) a new mission with the specified parameters.
	 */
	CreateMission(bstrServerName: string, bstrServerAddr: string, pMissionParams: IPigMissionParams): IPigMissionParams;
	/**
	 * Joins a mission, which may be specified by name, by a buddy player, or not at all.
	 */
	JoinMission(bstrMissionOrPlayer: string): string;
	/**
	 * Requests a position on a team, which may be specified by name, by a buddy player, or not at all.
	 */
	JoinTeam(bstrCivName: string, bstrTeamOrPlayer: string): string;
	/**
	 * Quits the current game.
	 */
	QuitGame();
	/**
	 * Undocks from the station and enters space.
	 */
	Launch();
	/**
	 * Forces the pig to exit the game.
	 */
	Shutdown();
	/**
	 * Returns true if player has game control
	 */
	IsMissionOwner(): BOOL;
	/**
	 * Returns true if player is the team leader
	 */
	IsTeamLeader(): BOOL;
	/**
	 * Sets the shoot, turn and goto skills
	 */
	SetSkills(fShoot: number, fTurn: number, fGoto: number): number;
	/**
	 * READONLY: Gets the AGCGame object that the Pig is a member of, if any.
	 */
	readonly Game: IAGCGame;
	/**
	 * READONLY: Gets the AGCShip object that sent the current chat. NULL if chat was from HQ or Pig is not currently processing chat text.
	 */
	readonly Me: IAGCShip;
	/**
	 * READONLY: Gets the target of the current chat. AGCChat_NoSelection if Pig is not currently processing chat text.
	 */
	readonly ChatTarget: Enums.AGCChatTarget;
	/**
	 * READONLY: Gets the object used to access the rest of the Pigs object model.
	 */
	readonly Host: IPigBehaviorHost;
	/**
	 * Sets mission params then starts the game.
	 */
	StartCustomGame(pMissionParams: IPigMissionParams): IPigMissionParams;
	/**
	 * Starts the game.
	 */
	StartGame();
	/**
	 * Contains a threshold value, representing an angle in radians, to which each ship's movement is compared.
	 */
	ShipAngleThreshold1(rRadians: number): number;
	/**
	 * Contains a threshold value, representing an angle in radians, to which each ship's movement is compared.
	 */
	ShipAngleThreshold2(rRadians: number): number;
	/**
	 * Contains a threshold value, representing an angle in radians, to which each ship's movement is compared.
	 */
	ShipAngleThreshold3(rRadians: number): number;
	/**
	 * Contains a threshold value, representing a distance, to which each ship's movement is compared.
	 */
	ShipDistanceThreshold1(rDistance: number): number;
	/**
	 * Contains a threshold value, representing a distance, to which each ship's movement is compared.
	 */
	ShipDistanceThreshold2(rDistance: number): number;
	/**
	 * Contains a threshold value, representing a distance, to which each ship's movement is compared.
	 */
	ShipDistanceThreshold3(rDistance: number): number;
	/**
	 * Contains a threshold value, representing ZTime, to which the (heavy_update_time - client_time) is compared.
	 */
	ShipsUpdateLatencyThreshold1(nMilliseconds: number): number;
	/**
	 * Contains a threshold value, representing ZTime, to which the (heavy_update_time - client_time) is compared.
	 */
	ShipsUpdateLatencyThreshold2(nMilliseconds: number): number;
	/**
	 * Contains a threshold value, representing ZTime, to which the (heavy_update_time - client_time) is compared.
	 */
	ShipsUpdateLatencyThreshold3(nMilliseconds: number): number;
	/**
	 * Sends the bytes specified to the connected lobby server, if any. pvBytes is either the name of a file, or a SAFEARRAY of bytes.
	 */
	SendBytesToLobby(pvBytes: object, bGuaranteed: boolean): boolean;
	/**
	 * Sends the bytes specified to the connected game server, if any. pvBytes is either the name of a file, or a SAFEARRAY of bytes.
	 */
	SendBytesToGame(pvBytes: object, bGuaranteed: boolean): boolean;
}

/**
 * This is the global pig object. It is created by the COM layer when the pig script is loaded by PigSrv. This is your main hook into all things pig.
 */
declare const Pig: IPig;


////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigs Interface

/**
 * Interface to a collection of pig player objects.
 */
interface IPigs extends ITCCollection
{
	/**
	 * Returns an IPig* from the collection or NULL if the specified named pig does not exist in the collection. Takes an argument, index, which must be the name of a pig in the collection.
	 */
	Item(pvIndex: object): IPig;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigTeam Interface

/**
 * Interface to a Team object.
 */
interface IPigTeam extends IDispatch
{
	/**
	 * READONLY: Gets the name of the team.
	 */
	readonly Name: string;
	/**
	 * READONLY: Gets/sets the civilization of the team.
	 */
	readonly PlayersMaximum: number;
	/**
	 * READONLY: Gets the current number of players on this team.
	 */
	readonly PlayerCount: number;
	/**
	 * Gets the collection of players on this team.
	 */
	IsAutoAccept(bAutoAccept: boolean): boolean;
	/**
	 * Gets/sets the 'ready' flag of this team.
	 */
	IsReady(bReady: boolean): boolean;
	/**
	 * Gets/sets the 'force-ready' flag of this team.
	 */
	IsForceReady(bForceReady: boolean): boolean;
	/**
	 * Gets/sets the 'active' flag of this team.
	 */
	IsActive(bActive: boolean): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigTeams Interface

/**
 * Interface to a collection of Team objects.
 */
interface IPigTeams extends ITCCollection
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigMissionParams Interface

/**
 * Interface to a MissionParams object.
 */
interface IPigMissionParams extends IDispatch
{
	/**
	 * Validates the properties of the object.
	 */
	Validate();
	/**
	 * Gets/sets the number of teams for the mission.
	 */
	TeamCount(nTeamCount: number): number;
	/**
	 * Gets/sets the maximum number of players allowed on a team.
	 */
	MaxTeamPlayers(nMaxTeamPlayers: number): number;
	/**
	 * Gets/sets the minimum number of players allowed on a team.
	 */
	MinTeamPlayers(nMinTeamPlayers: number): number;
	/**
	 * Gets/sets the map type for the mission.
	 */
	MapType(eMapType: Enums.PigMapType): Enums.PigMapType;
	/**
	 * Gets/sets the death match goal.
	 */
	TeamKills(nGoalTeamKills: number): number;
	/**
	 * Gets/sets the game name for the mission.
	 */
	GameName(bstrGameName: string): string;
	/**
	 * Gets/sets the core for the mission.
	 */
	CoreName(bstrCoreName: string): string;
	/**
	 * Gets/sets the KB Level.
	 */
	KillBonus(KBLevel: number): number;
	/**
	 * Gets/sets Defections allowed.
	 */
	Defections(Defections: boolean): boolean;
	/**
	 * Gets/sets the number of starting miners.
	 */
	Miners(Miners: number): number;
	/**
	 * Gets/sets Developments allowed.
	 */
	Developments(Developments: boolean): boolean;
	/**
	 * Gets/sets the Conquest goal.
	 */
	Conquest(Conquest: number): number;
	/**
	 * Gets/sets the Flags goal.
	 */
	Flags(Flags: number): number;
	/**
	 * Gets/sets the Artifacts goal.
	 */
	Artifacts(Artifacts: number): number;
	/**
	 * Gets/sets Pods allowed.
	 */
	Pods(Pods: boolean): boolean;
	/**
	 * Gets/sets Experimental allowed.
	 */
	Experimental(Experimental: boolean): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigMissionParamsPrivate Interface

/**
 * Private interface to a MissionParams object.
 */
interface IPigMissionParamsPrivate extends IUnknown
{
	/**
	 * Gets a stream containing the length-prefixed data structure.
	 */
	GetData(): IStream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigMission Interface

/**
 * Interface to a Mission object.
 */
interface IPigMission extends IDispatch
{
	/**
	 * READONLY: Gets the collection of (pig) players.
	 */
	readonly Pigs: IPigs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigMissions Interface

/**
 * Interface to a collection of Mission objects.
 */
interface IPigMissions extends ITCCollection
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigLobby Interface

/**
 * Interface to a Lobby object.
 */
interface IPigLobby extends IDispatch
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigLobbies Interface

/**
 * Interface to a collection of Lobby objects.
 */
interface IPigLobbies extends ITCCollection
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigSession Interface

/**
 * IPigSession Interface
 */
interface IPigSession extends IDispatch
{
	/**
	 * Gets/sets the session information object.
	 */
	SessionInfo(pSessionInfo: ITCSessionInfo): ITCSessionInfo;
	/**
	 * READONLY: Gets the collection of connected sessions.
	 */
	readonly SessionInfos: ITCSessionInfos;
	/**
	 * Gets/sets the directory for script files.
	 */
	ScriptDir(bstrScriptDir: string): string;
	/**
	 * Gets/sets the directory for artwork files.
	 */
	ArtPath(bstrArtPath: string): string;
	/**
	 * READONLY: Gets the collection of behavior scripts.
	 */
	readonly BehaviorTypes: IPigBehaviorTypes;
	/**
	 * READONLY: Gets the collection of game lobby servers.
	 */
	readonly Lobbies: IPigLobbies;
	/**
	 * READONLY: Gets the collection of all (pig) players.
	 */
	readonly Pigs: IPigs;
	/**
	 * Creates a new pig object with the specified behavior type.
	 */
	CreatePig(bstrType: string, bstrCommandLine: string): IPig;
	/**
	 * Gets/sets the server on which all pigs will be created.
	 */
	MissionServer(bstrServer: string): string;
	/**
	 * Gets/sets maximum number of pigs that can be created on this server.
	 */
	MaxPigs(nMaxPigs: number): number;
	/**
	 * READONLY: Gets the object that dispenses pig accounts.
	 */
	readonly AccountDispenser: IPigAccountDispenser;
	/**
	 * READONLY: Gets the version information of the Pig server.
	 */
	readonly Version: IAGCVersionInfo;
	/**
	 * READONLY: Gets the process ID of the server, meaningful only on the local machine.
	 */
	readonly ProcessID: number;
	/**
	 * Enables the firing of all available events from this session.
	 */
	ActivateAllEvents();
	/**
	 * Disables the firing of all available events from this session.
	 */
	DeactivateAllEvents();
	/**
	 * READONLY: Gets the event log object.
	 */
	readonly EventLog: IAGCEventLogger;
	/**
	 * Contains the server from which pig accounts will be dispensed. When empty, the MissionServer is used. When this property is a period character, the local machine is used.
	 */
	AccountServer(bstrServer: string): string;
	/**
	 * Contains the server on which the pig accounts will be authenticated. When empty, no authentication is performed.
	 */
	ZoneAuthServer(bstrServer: string): string;
	/**
	 * Contains the maximum amount of time allowed for authentication of pig accounts. Ignored when ZoneAuthServer is an empty string.
	 */
	ZoneAuthTimeout(nMilliseconds: number): number;
	/**
	 * Specifies the mode of lobby connection that is used for server connections. Affects the usage of the MissionServer property.
	 */
	LobbyMode(eMode: Enums.PigLobbyMode): Enums.PigLobbyMode;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigSessionEvents Interface

/**
 * IPigSessionEvents Interface
 */
interface IPigSessionEvents extends IUnknown
{
	/**
	 * Called when an event is fired for this session.
	 */
	OnEvent(pEvent: IAGCEvent): IAGCEvent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// _IPigSessionEvents Interface

/**
 * _IPigSessionEvents Interface
 */
interface _IPigSessionEvents
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigTimer Interface

/**
 * Interface to a Timer object.
 */
interface IPigTimer extends IPigEvent
{
	/**
	 * Gets/sets the number of interval occurences remaining for this timer.
	 */
	RepetitionCount(nRepetitionCount: number): number;
	/**
	 * Gets/sets the time, in seconds, of each timer interval.
	 */
	Interval(fInterval: number): number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigShip Interface

/**
 * Interface to a Pig Ship object.
 */
interface IPigShip extends IAGCShip
{
	/**
	 * Sells all parts loaded onto the ship.
	 */
	SellAllParts();
	/**
	 * Buys the specified hull type.
	 */
	BuyHull(pHullType: IAGCHullType): IAGCHullType;
	/**
	 * Buys the default loadout for the current hull type.
	 */
	BuyDefaultLoadout();
	/**
	 * Kills the ship.
	 */
	CommitSuicide(): string;
	/**
	 * Stops all thrusting, throttling, axis turning, and action events.
	 */
	AllStop(): string;
	/**
	 * Thrusts the ship in the specified direction(s).
	 */
	Thrust(e1: Enums.PigThrust, e2: Enums.PigThrust, e3: Enums.PigThrust, e4: Enums.PigThrust, e5: Enums.PigThrust, e6: Enums.PigThrust, e7: Enums.PigThrust): string;
	/**
	 * Turns afterburner on/off.
	 */
	Boost(bOn: boolean): string;
	/**
	 * Begins/ends the firing of the active weapon.
	 */
	FireWeapon(bBegin: boolean): string;
	/**
	 * Begins/ends the firing of the missile launcher.
	 */
	FireMissile(bBegin: boolean): string;
	/**
	 * Begins/ends the dropping of mines (or probes).
	 */
	DropMine(bBegin: boolean): string;
	/**
	 * Activates/deactivates the vector lock.
	 */
	LockVector(bLockVector: boolean): string;
	/**
	 * Activates/deactivates the rip cording action.
	 */
	RipCord(pvParam: object): string;
	/**
	 * Cloaks/decloaks the ship.
	 */
	Cloak(bCloak: boolean): string;
	/**
	 * Accepts the queued command.
	 */
	AcceptCommand(): string;
	/**
	 * Clears the accepted command.
	 */
	ClearCommand(): string;
	/**
	 * Turns the ship the specified number of positive degrees upward.
	 */
	PitchUp(fDegrees: number): string;
	/**
	 * Turns the ship the specified number of positive degrees downward.
	 */
	PitchDown(fDegrees: number): string;
	/**
	 * Turns the ship the specified number of positive degrees to the left.
	 */
	YawLeft(fDegrees: number): string;
	/**
	 * Turns the ship the specified number of positive degrees to the right.
	 */
	YawRight(fDegrees: number): string;
	/**
	 * Rolls the ship the specified number of positive degrees to the left
	 */
	RollLeft(fDegrees: number): string;
	/**
	 * Rolls the ship the specified number of positive degrees to the right.
	 */
	RollRight(fDegrees: number): string;
	/**
	 * Turns the ship to face the vector, specified by either an object name (VT_BSTR) or an IAGCVector* or IAGCModel* (VT_DISPATCH).
	 */
	Face(pvarObject: object, bstrCompletionExpr: string, bstrInterruptionExpr: string, bMatchUpVector: boolean): string;
	/**
	 * Defends the specified target.
	 */
	Defend(bstrObject: string): string;
	/**
	 * Attacks the specified target ship.
	 */
	AttackShip(pTarget: IAGCShip): string;
	/**
	 * Attacks the specified target station.
	 */
	AttackStation(pTarget: IAGCStation): string;
	/**
	 * Goto the specified target.
	 */
	Goto(bstrObject: string, bFriendly: boolean): string;
	/**
	 * Goto the specified station ID.
	 */
	GotoStationID(oid: number): string;
	/**
	 * Gets/sets the AutoPilot state.
	 */
	AutoPilot(bEngage: boolean): boolean;
	/**
	 * READONLY: Gets the Pig Ship action event object for this ship.
	 */
	readonly AutoAction: IPigShipEvent;
	/**
	 * Indicates whether the ship is thrusting in the specified direction.
	 */
	IsThrusting(eThrust: Enums.PigThrust): boolean;
	/**
	 * Gets/sets the weapon that is active. This must be 0 thru 3, or -1 for the default of 'all weapons'.
	 */
	ActiveWeapon(iWeapon: number): number;
	/**
	 * READONLY: Indicates whether the ship is firing the active weapon.
	 */
	readonly IsFiringWeapon: boolean;
	/**
	 * READONLY: Indicates whether the ship is firing the missile launcher.
	 */
	readonly IsFiringMissile: boolean;
	/**
	 * READONLY: Indicates whether the ship is dropping a mine (or probe).
	 */
	readonly IsDroppingMine: boolean;
	/**
	 * READONLY: Indicates whether the ship's vector lock is active.
	 */
	readonly IsVectorLocked: boolean;
	/**
	 * READONLY: Indicates whether the ship is rip-cording.
	 */
	readonly IsRipCording: boolean;
	/**
	 * READONLY: Indicates whether the ship is cloaked.
	 */
	readonly IsCloaked: boolean;
	/**
	 * READONLY: Gets the max range of the current missile
	 */
	readonly MissileRange: number;
	/**
	 * READONLY: Gets the currnt missile lock 0.0 to 1.0
	 */
	readonly MissileLock: number;
	/**
	 * Gets/sets the ship's throttle. The valid range is 0 to 100.
	 */
	Throttle(fThrottle: number): number;
	/**
	 * Gets/sets the ship's pitch speed. The valid range is -100 to 100.
	 */
	Pitch(fPitch: number): number;
	/**
	 * Gets/sets the ship's yaw speed. The valid range is -100 to 100.
	 */
	Yaw(fYaw: number): number;
	/**
	 * Gets/sets the ship's roll speed. The valid range is -100 to 100.
	 */
	Roll(fRoll: number): number;
	/**
	 * READONLY: Gets the queued command.
	 */
	readonly QueuedCommand: IAGCCommand;
	/**
	 * READONLY: Gets the accepted command.
	 */
	readonly AcceptedCommand: IAGCCommand;
	/**
	 * Gets/sets whether the ship automatically accepts queued commands.
	 */
	AutoAcceptCommands(bAutoAccept: boolean): boolean;
	/**
	 * READONLY: Gets the amount of time left until the ripcordind action completes.
	 */
	readonly RipCordTimeLeft: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigShipEvent Interface

/**
 * Interface to a Pig Ship Event object.
 */
interface IPigShipEvent extends IPigEvent
{
	/**
	 * READONLY: Gets name of the action in progress.
	 */
	readonly Action: string;
	/**
	 * READONLY: Gets the name of the target object for the action in progress.
	 */
	readonly Target: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigVector Interface

/**
 * Interface to a Pig Vector object.
 */
interface IPigVector extends IDispatch
{
	/**
	 * Gets/sets the x coordinate of the vector.
	 */
	X(xArg: number): number;
	/**
	 * Gets/sets the y coordinate of the vector.
	 */
	Y(yArg: number): number;
	/**
	 * Gets/sets the z coordinate of the vector.
	 */
	Z(zArg: number): number;
	/**
	 * Sets the x, y, and z coordinates of the vector.
	 */
	SetXYZ(xArg: number, yArg: number, zArg: number): number;
	/**
	 * READONLY: Gets the displayable string representation of the vector.
	 */
	readonly DisplayString: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigAccount Interface

/**
 * Interface to a Pig Account object.
 */
interface IPigAccount extends IDispatch
{
	/**
	 * READONLY: Gets the account name.
	 */
	readonly Name: string;
	/**
	 * READONLY: Gets the account password.
	 */
	readonly Password: string;
	/**
	 * READONLY: Gets the account CDKEY.
	 */
	readonly CdKey: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigAccountDispenser Interface

/**
 * Interface to a Pig Account Dispenser object.
 */
interface IPigAccountDispenser extends IDispatch
{
	/**
	 * READONLY: Gets an available account and marks it 'in-use'.
	 */
	readonly NextAvailable: IPigAccount;
	/**
	 * READONLY: Gets a collection of all the managed user names.
	 */
	readonly Names: ITCStrings;
	/**
	 * READONLY: Gets a collection of the available user names.
	 */
	readonly NamesAvailable: ITCStrings;
	/**
	 * READONLY: Gets a collection of the user names in use.
	 */
	readonly NamesInUse: ITCStrings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IPigHullTypes Interface

/**
 * Interface to a collection of hull type objects.
 */
interface IPigHullTypes extends ITCCollection
{
	/**
	 * Returns an IAGCHullType* from the collection or NULL if the specified named hull type does not exist in the collection. Takes an argument, index, which must be either the name of a hull type in the collection or a 0-based index into the collection.
	 */
	Item(pvIndex: object): IAGCHullType;
}

