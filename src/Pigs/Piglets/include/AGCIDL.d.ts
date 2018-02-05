
// ***
// *** 
// *** This is a generated file. Please do not modify this file by hand or your changes will be lost.
// ***
// ***

interface IDispatch { }
interface IDictionary { }
interface IUnknown { }

type Guid = string;


/// <reference path='AGCEventsIDL.d.ts' />

declare namespace Enums
{
////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCObjectType      // IGC's ObjectType Enum

	/**
	 * AGCUniqueID type.
	 */
	enum AGCObjectType      // IGC's ObjectType
		{
		AGCObjectType_Invalid,
		AGC_ModelBegin,
		AGC_Ship,
		AGC_Station,
		AGC_Missile,
		AGC_Mine,
		AGC_Probe,
		AGC_Asteroid,
		AGC_Projectile,
		AGC_Warp,
		AGC_Treasure,
		AGC_Buoy,
		AGC_Chaff,
		AGC_BuildingEffect,
		AGC_ModelEnd,
		AGC_Side,
		AGC_Cluster,
		AGC_Bucket,
		AGC_PartBegin,
		AGC_Weapon,
		AGC_Shield,
		AGC_Cloak,
		AGC_Pack,
		AGC_Afterburner,
		AGC_LauncherBegin,
		AGC_Magazine,
		AGC_Dispenser,
		AGC_LauncherEnd,
		AGC_PartEnd,
		AGC_StaticBegin,
		AGC_ProjectileType,
		AGC_MissileType,
		AGC_MineType,
		AGC_ProbeType,
		AGC_ChaffType,
		AGC_Civilization,
		AGC_TreasureSet,
		AGC_BucketStart,
		AGC_HullType,
		AGC_PartType,
		AGC_StationType,
		AGC_Development,
		AGC_DroneType,
		AGC_BucketEnd,
		AGC_StaticEnd,
		AGC_Constants,
		AGC_AdminUser,
		AGCObjectType_Max,
		AGC_Any_Objects,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCHullAbility     // IGC's HullAbilityBitMask Enum

	/**
	 * Constants for the AGCHullAbilityBitMask type.
	 */
	enum AGCHullAbility     // IGC's HullAbilityBitMask
		{
		AGCHullAbility_Board,
		AGCHullAbility_Rescue,
		AGCHullAbility_Lifepod,
		AGCHullAbility_NoPickup,
		AGCHullAbility_NoEjection,
		AGCHullAbility_NoRipcord,
		AGCHullAbility_CheatToDock,
		AGCHullAbility_Fighter,
		AGCHullAbility_Captital,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCAsteroidAbility // IGC's AsteroidAbilityBitMask Enum

	/**
	 * Constants for the AGCAsteroidAbilityBitMask type.
	 */
	enum AGCAsteroidAbility // IGC's AsteroidAbilityBitMask
		{
		AGCAsteroidAbility_MineHe3,
		AGCAsteroidAbility_MineIce,
		AGCAsteroidAbility_MineGold,
		AGCAsteroidAbility_Buildable,
		AGCAsteroidAbility_Special,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCAxis            // IGC's Axis Enum

	/**
	 * Constants for AGCAxis.
	 */
	enum AGCAxis            // IGC's Axis
		{
		AGCAxis_Yaw,
		AGCAxis_Pitch,
		AGCAxis_Roll,
		AGCAxis_Throttle,
		AGCAxis_Max,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCEquipmentType   // IGC's EquipmentType Enum

	/**
	 * Constants for AGCEquipmentType.
	 */
	enum AGCEquipmentType   // IGC's EquipmentType
		{
		AGCEquipmentType_ChaffLauncher,
		AGCEquipmentType_Weapon,
		AGCEquipmentType_Magazine,
		AGCEquipmentType_Dispenser,
		AGCEquipmentType_Shield,
		AGCEquipmentType_Cloak,
		AGCEquipmentType_Pack,
		AGCEquipmentType_Afterburner,
		AGCEquipmentType_MAX,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCTreasureType    // IGC's TreasureCode Enum

	/**
	 * Constants for AGCTreasureType.
	 */
	enum AGCTreasureType    // IGC's TreasureCode
		{
		AGCTreasureType_Part,
		AGCTreasureType_Development,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCChatTarget      // IGC's ChatTarget Enum

	/**
	 * Constants for AGCChatTarget.
	 */
	enum AGCChatTarget      // IGC's ChatTarget
		{
		AGCChat_Everyone,
		AGCChat_Leaders,
		AGCChat_Admin,
		AGCChat_Ship,
		AGCChat_Allies,
		AGCChat_Team,
		AGCChat_Individual,
		AGCChat_Individual_NoFilter,
		AGCChat_Wing,
		AGCChat_Echo,
		AGCChat_AllSector,
		AGCChat_FriendlySector,
		AGCChat_Group,
		AGCChat_GroupNoEcho,
		AGCChat_NoSelection,
		AGCChat_Max,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCGameStage    // IGC's STAGE Enum

	/**
	 * Constants for AGCGameStage.
	 */
	enum AGCGameStage    // IGC's STAGE
		{
		AGCGameStage_NotStarted,
		AGCGameStage_Starting,
		AGCGameStage_Started,
		AGCGameStage_Over,
		AGCGameStage_Terminate,
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
// FileAttributes Enum

	/**
	 * Constants for file attributes.
	 */
	enum FileAttributes
		{
		FileAttribute_None,
		FileAttribute_ReadOnly,
		FileAttribute_Hidden,
		FileAttribute_System,
		FileAttribute_Directory,
		FileAttribute_Archive,
		FileAttribute_Encrypted,
		FileAttribute_Normal,
		FileAttribute_Temporary,
		FileAttribute_SparseFile,
		FileAttribute_ReparsePoint,
		FileAttribute_Compressed,
		FileAttribute_Offline,
		FileAttribute_NotContentIndexed,
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCUniqueID Alias

	/**
	 * AGCUniqueID type.
	 */
	type AGCUniqueID = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCObjectID Alias

	/**
	 * AGCObjectID type.
	 */
	type AGCObjectID = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCGameID Alias

	/**
	 * AGCGameID type.
	 */
	type AGCGameID = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCMoney Alias

	/**
	 * AGCMoney type.
	 */
	type AGCMoney = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCHitPoints Alias

	/**
	 * AGCHitPoints type.
	 */
	type AGCHitPoints = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCPartMask Alias

	/**
	 * AGCPartMask type.
	 */
	type AGCPartMask = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCMount Alias

	/**
	 * AGCMount type.
	 */
	type AGCMount = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCHullAbilityBitMask Alias

	/**
	 * AGCHullAbilityBitMask type.
	 */
	type AGCHullAbilityBitMask = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCAsteroidAbilityBitMask Alias

	/**
	 * AGCAsteroidAbilityBitMask type.
	 */
	type AGCAsteroidAbilityBitMask = number;

////////////////////////////////////////////////////////////////////////////////////////////////////
// AGCSoundID Alias

	/**
	 * AGCSoundID type.
	 */
	type AGCSoundID = number;



////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventSink Interface

/**
 * Interface implemented by the hosting application to receive triggered events. Intended for use ONLY by host applications.
 */
interface IAGCEventSink extends IUnknown
{
	/**
	 * Called when an AGC event has been triggered.
	 */
	OnEventTriggered(pEvent: IAGCEvent): IAGCEvent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventSinkSynchronous Interface

/**
 * Interface implemented by the hosting application to receive triggered events. Intended for use ONLY by host applications.
 */
interface IAGCEventSinkSynchronous extends IUnknown
{
	/**
	 * Called when an AGC event has been triggered.
	 */
	OnEventTriggeredSynchronous(pEvent: IAGCEvent): IAGCEvent;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCDebugHook Interface

/**
 * Implemented by an AGC host to hook debug events.
 */
interface IAGCDebugHook extends IUnknown
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCGlobal Interface

/**
 * Interface to the AGC Global object. Intended for use ONLY by host applications that deal directly with Igc.
 */
interface IAGCGlobal extends IUnknown
{
	/**
	 * Registers the function that is called to create the object that suports the specified interface
	 */
	GetAGCObject(pvIgc: object, riid: Guid, ppUnk: object): object;
	/**
	 * Adds a mapping of an Igc object pointer to an AGC object.
	 */
	MakeAGCEvent(idEvent: Enums.AGCEventID, pszContext: LPCSTR, pszSubject: LPCOLESTR, idSubject: AGCUniqueID, cArgTriplets: number, pvArgs: object): IAGCEvent;
	/**
	 * Sets the ranges of event ID's available from the host application.
	 */
	MakeAGCVector(pVectorRaw: object): IAGCVector;
	/**
	 * Creates an AGCOrientation object, initialized with the specified raw ZLib orientation.
	 */
	MakeAGCOrientation(pOrientationRaw: object): IAGCOrientation;
	/**
	 * Creates an AGCEventIDRange object, initialized with the specified values.
	 */
	MakeAGCEventIDRange(lower: Enums.AGCEventID, upper: Enums.AGCEventID): IAGCEventIDRange;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCVector Interface

/**
 * Interface to a AGC Vector object.
 */
interface IAGCVector extends IDispatch
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
	 * Gets/sets the displayable string representation of the vector.
	 */
	DisplayString(bstrDisplayString: string): string;
	/**
	 * Initializes the object with the specified x, y, and z values.
	 */
	InitXYZ(xArg: number, yArg: number, zArg: number): number;
	/**
	 * Initializes the object by copying the specified vector's x, y, and z values.
	 */
	InitCopy(pVector: IAGCVector): IAGCVector;
	/**
	 * Initializes the object with a random direction.
	 */
	InitRandomDirection();
	/**
	 * Initializes the object with a random position.
	 */
	InitRandomPosition(fRadius: number): number;
	/**
	 * READONLY: Compares the object to a zero vector.
	 */
	readonly IsZero: boolean;
	/**
	 * Compares the object to the specified vector.
	 */
	IsEqual(pVector: IAGCVector): boolean;
	/**
	 * READONLY: Computes the length of the object from a zero vector.
	 */
	readonly Length: number;
	/**
	 * READONLY: Computes the squared length of the object from a zero vector.
	 */
	readonly LengthSquared: number;
	/**
	 * READONLY: Computes the orthogonal vector of the object.
	 */
	readonly OrthogonalVector: IAGCVector;
	/**
	 * Computes the sum of the object and the specified vector. Does not modify the object.
	 */
	Add(pVector: IAGCVector): IAGCVector;
	/**
	 * Computes the difference of the object and the specified vector. Does not modify the object.
	 */
	Subtract(pVector: IAGCVector): IAGCVector;
	/**
	 * Computes the product of the object and the specified multiplier value. Does not modify the object.
	 */
	Multiply(f: number): IAGCVector;
	/**
	 * Computes the quotient of the object and the specified divisor value. Does not modify the object.
	 */
	Divide(f: number): IAGCVector;
	/**
	 * Computes the normalized vector of the object. Does not modify the object.
	 */
	Normalize(): IAGCVector;
	/**
	 * Computes the cross product of the object and the specified vector. Does not modify the object.
	 */
	CrossProduct(pVector: IAGCVector): IAGCVector;
	/**
	 * Computes an interpolation of the object with the specified vector. Does not modify the object.
	 */
	Interpolate(pVector: IAGCVector, fValue: number): IAGCVector;
	/**
	 * Computes the sum of the object and the specified vector, with the result being stored in the object.
	 */
	AddInPlace(pVector: IAGCVector): IAGCVector;
	/**
	 * Computes the difference of the object and the specified vector, with the result being stored in the object.
	 */
	SubtractInPlace(pVector: IAGCVector): IAGCVector;
	/**
	 * Computes the product of the object and the specified multiplier value, with the result being stored in the object.
	 */
	MultiplyInPlace(f: number): number;
	/**
	 * Computes the quotient of the object and the specified divisor value, with the result being stored in the object.
	 */
	DivideInPlace(f: number): number;
	/**
	 * Computes the normalized vector of the object, with the result being stored in the object.
	 */
	NormalizeInPlace();
	/**
	 * Computes the cross product of the object and the specified vector, with the result being stored in the object.
	 */
	CrossProductInPlace(pVector: IAGCVector): IAGCVector;
	/**
	 * Computes an interpolation of the object with the specified vector, with the result being stored in the object.
	 */
	InterpolateInPlace(pVector: IAGCVector, fValue: number): number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCVectorPrivate Interface

/**
 * Private interface to an AGC Vector object.
 */
interface IAGCVectorPrivate extends IUnknown
{
	/**
	 * Initializes the object from a raw (ZLib/IGC) Vector pointer.
	 */
	InitFromVector(pvVector: object): object;
	/**
	 * Copies the object's raw (ZLib/IGC) Vector to the specified Vector pointer.
	 */
	CopyVectorTo(pvVector: object): object;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCOrientation Interface

/**
 * Interface to an AGC Orientation object.
 */
interface IAGCOrientation extends IDispatch
{
	/**
	 * Initializes the object by copying the specified object.
	 */
	InitCopy(pOrientation: IAGCOrientation): IAGCOrientation;
	/**
	 * READONLY: Gets the Forward vector of the orientation matrix.
	 */
	readonly Forward: IAGCVector;
	/**
	 * READONLY: Gets the Backward vector of the orientation matrix.
	 */
	readonly Backward: IAGCVector;
	/**
	 * READONLY: Gets the Up vector of the orientation matrix.
	 */
	readonly Up: IAGCVector;
	/**
	 * READONLY: Gets the Right vector of the orientation matrix.
	 */
	readonly Right: IAGCVector;
	/**
	 * Compares the object to the specified object for absolute equality.
	 */
	IsEqual(pOrientation: IAGCOrientation): boolean;
	/**
	 * Compares the object to the specified object for 'fuzzy' equality.
	 */
	IsRoughlyEqual(pOrientation: IAGCOrientation): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCOrientationPrivate Interface

/**
 * Private interface to an AGC Orientation object.
 */
interface IAGCOrientationPrivate extends IUnknown
{
	/**
	 * Initializes the object from a raw (ZLib/IGC) Orientation pointer.
	 */
	InitFromOrientation(pvOrientation: object): object;
	/**
	 * Copies the object's raw (ZLib/IGC) Orientation to the specified Orientation pointer.
	 */
	CopyOrientationTo(pvOrientation: object): object;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventType Interface

/**
 * IAGCEventType Interface
 */
interface IAGCEventType extends IDispatch
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEvent Interface

/**
 * IAGCEvent Interface
 */
interface IAGCEvent extends IDispatch
{
	/**
	 * READONLY: Gets the event identifier.
	 */
	readonly ID: Enums.AGCEventID;
	/**
	 * READONLY: Gets the date and time that the event occurred.
	 */
	readonly Time: Date;
	/**
	 * READONLY: Gets the textual description of the event that occurred.
	 */
	readonly Description: string;
	/**
	 * READONLY: Gets the number of event parameters in the property map.
	 */
	readonly PropertyCount: number;
	/**
	 * Gets whether or not the specified event parameter is in the property map.
	 */
	PropertyExists(bstrKey: string): boolean;
	/**
	 * Gets the specified event parameter from the property map.
	 */
	Property(pvKey: object): object;
	/**
	 * READONLY: Gets the name of the computer on which the event occurred.
	 */
	readonly ComputerName: string;
	/**
	 * READONLY: Gets the ID of the subject of the event, if any. Otherwise, -1.
	 */
	readonly SubjectID: number;
	/**
	 * READONLY: Gets the name of the subject of the event, if any.
	 */
	readonly SubjectName: string;
	/**
	 * Persists the object to a string representation, to be resolved with LoadFromString.
	 */
	SaveToString(): string;
	/**
	 * Initializes the object from a persistence string returned by SaveToString.
	 */
	LoadFromString(bstr: string): string;
	/**
	 * READONLY: Gets a string representing the context in which the event occurred.
	 */
	readonly Context: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventCreate Interface

/**
 * IAGCEventCreate Interface
 */
interface IAGCEventCreate extends IDispatch
{
	/**
	 * Initializes object.
	 */
	Init();
	/**
	 * Sets the event identifier.
	 */
	ID(Val: Enums.AGCEventID): Enums.AGCEventID;
	/**
	 * Sets the date and time that the event occurred.
	 */
	Time(Val: Date): Date;
	/**
	 * Sets the time property to the current date/time.
	 */
	SetTimeNow();
	/**
	 * Adds the specified event parameter to the property map.
	 */
	AddProperty(pbstrKey: string, pvValue: object): object;
	/**
	 * Removes the specified event parameter from the property map.
	 */
	RemoveProperty(pbstrKey: string, pvValue: object): object;
	/**
	 * Sets the ID of the subject of the event, if any. Otherwise, -1.
	 */
	SubjectID(idSubject: number): number;
	/**
	 * Sets the name of the subject of the event, if any.
	 */
	SubjectName(bstrSubject: string): string;
	/**
	 * Sets the string representing the context in which the event occurred.
	 */
	Context(bstrContext: string): string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCGameParameters Interface

/**
 * IAGCGameParameters Interface
 */
interface IAGCGameParameters extends IDispatch
{
	/**
	 * Validates the properties of the object.
	 */
	Validate();
	/**
	 * Gets/sets the minimum players per team.
	 */
	MinPlayers(Val: number): number;
	/**
	 * Gets/sets the maximum players per team.
	 */
	MaxPlayers(Val: number): number;
	/**
	 * property GameName
	 */
	GameName: string;
	/**
	 * property EjectPods
	 */
	EjectPods: boolean;
	/**
	 * property AllowPrivateTeams
	 */
	AllowPrivateTeams: boolean;
	/**
	 * property PowerUps
	 */
	PowerUps: boolean;
	/**
	 * property AllowJoiners
	 */
	AllowJoiners: boolean;
	/**
	 * property Stations
	 */
	Stations: boolean;
	/**
	 * property ScoresCount
	 */
	ScoresCount: boolean;
	/**
	 * property Drones
	 */
	Drones: boolean;
	/**
	 * property IsGoalConquest
	 */
	IsGoalConquest: boolean;
	/**
	 * property IsGoalArtifacts
	 */
	IsGoalArtifacts: boolean;
	/**
	 * property IsGoalTeamMoney
	 */
	IsGoalTeamMoney: boolean;
	/**
	 * property IsGoalTeamKills
	 */
	IsGoalTeamKills: boolean;
	/**
	 * property Resources
	 */
	Resources: boolean;
	/**
	 * property ResourceAmountsVisible
	 */
	ResourceAmountsVisible: boolean;
	/**
	 * property RandomWormholes
	 */
	RandomWormholes: boolean;
	/**
	 * property NoTeams
	 */
	NoTeams: boolean;
	/**
	 * property ShowHomeSector
	 */
	ShowHomeSector: boolean;
	/**
	 * property CivIDs
	 */
	CivIDs(element: number, element: number, newVal: number): number;
	/**
	 * property GoalTeamMoney
	 */
	GoalTeamMoney: AGCMoney;
	/**
	 * property TsiPlayerStart
	 */
	TsiPlayerStart: number;
	/**
	 * property TsiNeutralStart
	 */
	TsiNeutralStart: number;
	/**
	 * property TsiPlayerRegenerate
	 */
	TsiPlayerRegenerate: number;
	/**
	 * property TsiNeutralRegenerate
	 */
	TsiNeutralRegenerate: number;
	/**
	 * property StartingMoney
	 */
	StartingMoney: number;
	/**
	 * property Lives
	 */
	Lives: number;
	/**
	 * property GoalTeamKills
	 */
	GoalTeamKills: number;
	/**
	 * property MapType
	 */
	MapType: number;
	/**
	 * property MapSize
	 */
	MapSize: number;
	/**
	 * property RandomEncounters
	 */
	RandomEncounters: number;
	/**
	 * property NeutralSectors
	 */
	NeutralSectors: boolean;
	/**
	 * property AlephPositioning
	 */
	AlephPositioning: number;
	/**
	 * property AlephsPerSector
	 */
	AlephsPerSector: number;
	/**
	 * property Teams
	 */
	Teams: number;
	/**
	 * property MinRank
	 */
	MinRank: number;
	/**
	 * property MaxRank
	 */
	MaxRank: number;
	/**
	 * property PlayerSectorAsteroids
	 */
	PlayerSectorAsteroids: number;
	/**
	 * property NeutralSectorAsteroids
	 */
	NeutralSectorAsteroids: number;
	/**
	 * property PlayerSectorTreasures
	 */
	PlayerSectorTreasures: number;
	/**
	 * property NeutralSectorTreasures
	 */
	NeutralSectorTreasures: number;
	/**
	 * property PlayerSectorTreasureRate
	 */
	PlayerSectorTreasureRate: number;
	/**
	 * property NeutralSectorTreasureRate
	 */
	NeutralSectorTreasureRate: number;
	/**
	 * property PlayerSectorMineableAsteroids
	 */
	PlayerSectorMineableAsteroids: number;
	/**
	 * property NeutralSectorMineableAsteroids
	 */
	NeutralSectorMineableAsteroids: number;
	/**
	 * property PlayerSectorSpecialAsteroids
	 */
	PlayerSectorSpecialAsteroids: number;
	/**
	 * property NeutralSectorSpecialAsteroids
	 */
	NeutralSectorSpecialAsteroids: number;
	/**
	 * property IGCStaticFile
	 */
	IGCStaticFile: string;
	/**
	 * property GamePassword
	 */
	GamePassword: string;
	/**
	 * property InvulnerableStations
	 */
	InvulnerableStations: boolean;
	/**
	 * property ShowMap
	 */
	ShowMap: boolean;
	/**
	 * property AllowDevelopments
	 */
	AllowDevelopments: boolean;
	/**
	 * property AllowDefections
	 */
	AllowDefections: boolean;
	/**
	 * property SquadGame
	 */
	SquadGame: boolean;
	/**
	 * property AllowFriendlyFire
	 */
	AllowFriendlyFire: boolean;
	/**
	 * READONLY: property IGCcoreVersion
	 */
	readonly IGCcoreVersion: number;
	/**
	 * property GameLength
	 */
	GameLength: number;
	/**
	 * property He3Density
	 */
	He3Density: number;
	/**
	 * property KillPenalty
	 */
	KillPenalty: AGCMoney;
	/**
	 * property KillReward
	 */
	KillReward: AGCMoney;
	/**
	 * property EjectPenalty
	 */
	EjectPenalty: AGCMoney;
	/**
	 * property EjectReward
	 */
	EjectReward: AGCMoney;
	/**
	 * READONLY: property TimeStart
	 */
	readonly TimeStart: number;
	/**
	 * READONLY: property TimeStartClock
	 */
	readonly TimeStartClock: number;
	/**
	 * property GoalArtifactsCount
	 */
	GoalArtifactsCount: number;
	/**
	 * property AutoRestart
	 */
	AutoRestart: boolean;
	/**
	 * property DefaultCountdown (in seconds)
	 */
	DefaultCountdown: number;
	/**
	 * property InitialMinersPerTeam
	 */
	InitialMinersPerTeam: number;
	/**
	 * property MaxDronesPerTeam
	 */
	MaxDronesPerTeam: number;
	/**
	 * property CustomMap
	 */
	CustomMap: string;
	/**
	 * property RestartCountdown (in seconds)
	 */
	RestartCountdown: number;
	/**
	 * property TotalMaxPlayers (for the entire game)
	 */
	TotalMaxPlayers: number;
	/**
	 * property LockTeamSettings (locks settings for players not admins)
	 */
	LockTeamSettings: boolean;
	/**
	 * property InvitationListID
	 */
	InvitationListID: number;
	/**
	 * property IsSquadGame
	 */
	IsSquadGame: boolean;
	/**
	 * property LockGameOpen
	 */
	LockGameOpen: boolean;
	/**
	 * property TeamName
	 */
	TeamName(iTeam: number, iTeam: number, newVal: string): string;
	/**
	 * property IsTechBitOverridden; returns true iff OverriddenTechBit was set for this Team's BitID
	 */
	IsTechBitOverridden(iTeam: number, iBitID: number): boolean;
	/**
	 * property OverriddenTechBit
	 */
	OverriddenTechBit(iTeam: number, iBitID: number, iTeam: number, iBitID: number, newVal: boolean): boolean;
	/**
	 * property SetOverriddenTechBitRange
	 */
	SetOverriddenTechBitRange(iTeam: number, iBitID_First: number, iBitID_Last: number, newVal: boolean): boolean;
	/**
	 * property IsGoalFlags
	 */
	IsGoalFlags: boolean;
	/**
	 * property GoalFlagsCount
	 */
	GoalFlagsCount: number;
	/**
	 * The text that describes the story of the game.
	 */
	StoryText: string;
	/**
	 * property AllowEmptyTeams
	 */
	AllowEmptyTeams: boolean;
	/**
	 * property AutoStart
	 */
	AutoStart: boolean;
	/**
	 * property AllowRestart
	 */
	AllowRestart: boolean;
	/**
	 * property Experimental
	 */
	Experimental: boolean;
	/**
	 * property AllowShipyardPath
	 */
	AllowShipyardPath: boolean;
	/**
	 * property AllowSupremacyPath
	 */
	AllowSupremacyPath: boolean;
	/**
	 * property AllowTacticalPath
	 */
	AllowTacticalPath: boolean;
	/**
	 * property AllowExpansionPath
	 */
	AllowExpansionPath: boolean;
	/**
	 * property MaxImbalance: Maximum allowed difference between smallest and largest team.
	 */
	MaxImbalance: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCCommand Interface

/**
 * Interface to an AGC Command object.
 */
interface IAGCCommand extends IDispatch
{
	/**
	 * READONLY: Gets the command target.
	 */
	readonly Target: string;
	/**
	 * READONLY: Gets the command verb.
	 */
	readonly Verb: string;
	/**
	 * READONLY: Gets the textual represenation of the command.
	 */
	readonly Text: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCCommandPrivate Interface

/**
 * Private interface to an AGC Command object.
 */
interface IAGCCommandPrivate extends IUnknown
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCVersionInfo Interface

/**
 * IAGCVersionInfo Interface
 */
interface IAGCVersionInfo extends IDispatch
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCDBParams Interface

/**
 * IAGCDBParams Interface
 */
interface IAGCDBParams extends IDispatch
{
	/**
	 * Gets/sets the database connection string.
	 */
	ConnectionString(bstr: string): string;
	/**
	 * Gets/sets the table name.
	 */
	TableName(bstr: string): string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventLogger Interface

/**
 * IAGCEventLogger Interface
 */
interface IAGCEventLogger extends IDispatch
{
	/**
	 * READONLY: Gets the entire list of AGC Events as an XML string.
	 */
	readonly EventList: string;
	/**
	 * Gets/sets the name of the (WinNT or Win2000) computer to which AGC Events will be logged as NT Events.
	 */
	NTEventLog(bstrComputer: string): string;
	/**
	 * Gets/sets the parameters of the database to which AGC Events will be logged.
	 */
	DBEventLog(pDBParams: IAGCDBParams): IAGCDBParams;
	/**
	 * Gets/sets the range(s) of AGC Events to be logged to the NT Event log.
	 */
	EnabledNTEvents(pEvents: IAGCEventIDRanges): IAGCEventIDRanges;
	/**
	 * Gets/sets the range(s) of AGC Events to be logged to the database event log.
	 */
	EnabledDBEvents(pEvents: IAGCEventIDRanges): IAGCEventIDRanges;
	/**
	 * READONLY: Gets the default range(s) of AGC Events to be logged to the NT Event log.
	 */
	readonly DefaultEnabledNTEvents: IAGCEventIDRanges;
	/**
	 * READONLY: Gets the default range(s) of AGC Events to be logged to the DB Event log.
	 */
	readonly DefaultEnabledDBEvents: IAGCEventIDRanges;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventLoggerHook Interface

/**
 * Implemented by an AGC host to hook event logging.
 */
interface IAGCEventLoggerHook extends IUnknown
{
	/**
	 * Called to allow the host application to log an event in its own way. Host should return S_FALSE to indicate to the event logger that it should perform 'normal' event logging.
	 */
	LogEvent(pEvent: IAGCEvent, bSynchronous: boolean): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventLoggerPrivate Interface

/**
 * IAGCEventLoggerPrivate Interface
 */
interface IAGCEventLoggerPrivate extends IUnknown
{
	/**
	 * Should be called immediately upon creating the object (and from the same thread or apartment).
	 */
	Initialize(bstrSourceApp: string, bstrRegKey: string): string;
	/**
	 * Should be called immediately prior to releasing the object.
	 */
	Terminate();
	/**
	 * Gets/sets whether or not logging to the NT Event log is enabled or not.
	 */
	LoggingToNTEnabled(bEnabled: boolean): boolean;
	/**
	 * Gets/sets whether or not logging to the DB Event log is enabled or not.
	 */
	LoggingToDBEnabled(bEnabled: boolean): boolean;
	/**
	 * Gets/sets a callback interface to hook events logged to the NT Event log.
	 */
	HookForNTLogging(pHook: IAGCEventLoggerHook): IAGCEventLoggerHook;
	/**
	 * Gets/sets a callback interface to hook events logged to the DB Event log.
	 */
	HookForDBLogging(pHook: IAGCEventLoggerHook): IAGCEventLoggerHook;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCRangePrivate Interface

/**
 * Private interface to an AGC Range object.
 */
interface IAGCRangePrivate extends IUnknown
{
	/**
	 * Initializes the object from a raw (TCLib) range<T> pointer.
	 */
	InitFromRange(pvRange: object): object;
	/**
	 * Copies the object's raw (TCLib) range to the specified range<T> pointer.
	 */
	CopyRangeTo(pvRange: object): object;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCRangesPrivate Interface

/**
 * Private interface to an AGC Ranges object.
 */
interface IAGCRangesPrivate extends IUnknown
{
	/**
	 * Initializes the object from a raw (TCLib) rangeset< range<T> > pointer.
	 */
	InitFromRanges(pvRanges: object): object;
	/**
	 * Copies the object's raw (TCLib) range to the specified rangeset< range<T> > pointer.
	 */
	CopyRangesTo(pvRanges: object): object;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventIDRange Interface

/**
 * IAGCEventIDRange Interface
 */
interface IAGCEventIDRange extends IDispatch
{
	/**
	 * Gets/sets the range as a string, formatted for display purposes.
	 */
	DisplayString(bstr: string): string;
	/**
	 * Sets the lower and upper (open) ends of the range.
	 */
	Init(lower: Enums.AGCEventID, upper: Enums.AGCEventID): Enums.AGCEventID;
	/**
	 * READONLY: Gets the lower end of the range.
	 */
	readonly Lower: Enums.AGCEventID;
	/**
	 * READONLY: Gets the upper (open) end of the range.
	 */
	readonly Upper: Enums.AGCEventID;
	/**
	 * READONLY: Determines whether this range is empty (Lower equals Upper).
	 */
	readonly IsEmpty: boolean;
	/**
	 * Determines whether the specified value intersects with this range.
	 */
	IntersectsWithValue(value: Enums.AGCEventID): boolean;
	/**
	 * Determines whether the specified range intersects with this range.
	 */
	IntersectsWithRangeValues(value1: Enums.AGCEventID, value2: Enums.AGCEventID): boolean;
	/**
	 * Determines whether the specified range intersects with this range.
	 */
	IntersectsWithRange(pRange: IAGCEventIDRange): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCEventIDRanges Interface

/**
 * IAGCEventIDRanges Interface
 */
interface IAGCEventIDRanges extends IDispatch
{
	/**
	 * READONLY: Returns the number of items in the collection.
	 */
	readonly Count: number;
	/**
	 * READONLY: Returns an enumerator object that implements IEnumVARIANT.
	 */
	readonly _NewEnum: IUnknown;
	/**
	 * Returns a AGCShip from the collection, or NULL if the item does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCEventIDRange;
	/**
	 * Gets/sets the range set as a string, formatted for display purposes.
	 */
	DisplayString(bstr: string): string;
	/**
	 * Adds a range into the set, specified by the range's lower and upper bounds.
	 */
	AddByValues(value1: Enums.AGCEventID, value2: Enums.AGCEventID): Enums.AGCEventID;
	/**
	 * Adds a range into the set, specified by a range object.
	 */
	Add(pRange: IAGCEventIDRange): IAGCEventIDRange;
	/**
	 * Removes a range from the set, specified by the range's lower and upper bounds.
	 */
	RemoveByValues(value1: Enums.AGCEventID, value2: Enums.AGCEventID): Enums.AGCEventID;
	/**
	 * Removes a range from the set, specified by a range object.
	 */
	Remove(pRange: IAGCEventIDRange): IAGCEventIDRange;
	/**
	 * Removes all ranges from the set, specified by a range object.
	 */
	RemoveAll();
	/**
	 * Determines whether the specified value intersects with any range in this set.
	 */
	IntersectsWithValue(value: Enums.AGCEventID): boolean;
	/**
	 * Determines whether the specified range intersects with any range in this set.
	 */
	IntersectsWithRangeValues(value1: Enums.AGCEventID, value2: Enums.AGCEventID): boolean;
	/**
	 * Determines whether the specified range intersects with any range in this set.
	 */
	IntersectsWithRange(pRange: IAGCEventIDRange): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCCollection Interface

/**
 * Base interface for most collection interfaces.
 */
interface ITCCollection extends IDispatch
{
	/**
	 * READONLY: Returns the number of items in the collection.
	 */
	readonly Count: number;
	/**
	 * READONLY: Returns an enumerator object that implements IEnumVARIANT.
	 */
	readonly _NewEnum: IUnknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCStrings Interface

/**
 * Interface to a collection of strings.
 */
interface ITCStrings extends ITCCollection
{
	/**
	 * Returns a BSTR from the collection or NULL if the index is out of range. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): string;
	/**
	 * Adds the specified string to the collection.
	 */
	Add(bstr: string): string;
	/**
	 * Removes the specified string from the collection.
	 */
	Remove(pvIndex: object): object;
	/**
	 * Removes all strings from the collection.
	 */
	RemoveAll();
	/**
	 * Adds the specified delimited strings to the collection.
	 */
	AddDelimited(bstrDelimiter: string, bstrStrings: string): string;
	/**
	 * Returns the entire array of strings as a single string, delimited by the specified delimiter string.
	 */
	DelimitedItems(bstrDelimiter: string): string;
	/**
	 * Adds the strings of the specified collection.
	 */
	AddStrings(pStrings: ITCStrings): ITCStrings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCPropBagOnRegKey Interface

/**
 * Interface to create a property bag on a registry key.
 */
interface ITCPropBagOnRegKey extends IDispatch
{
	/**
	 * Creates or opens the specified registry key.
	 */
	CreateKey(bstrRegKey: string, bReadOnly: boolean): boolean;
	/**
	 * Opens the specified registry key.  Fails if the key does not exist.
	 */
	OpenKey(bstrRegKey: string, bReadOnly: boolean): boolean;
	/**
	 * Creates the object that is stored on the current registry key.
	 */
	CreateObject(): IUnknown;
	/**
	 * Creates the object, always on the local machine, that is stored on the current registry key.
	 */
	CreateLocalObject(): IUnknown;
	/**
	 * Creates the object, on the specified server, that is stored on the current registry key.
	 */
	CreateRemoteObject(bstrServer: string): IUnknown;
	/**
	 * Load the specified object from the values stored in the current registry key.
	 */
	LoadObject(punkObj: IUnknown): IUnknown;
	/**
	 * Saves the specified object to the current registry key.
	 */
	SaveObject(punkObj: IUnknown, bClearDirty: boolean, bSaveAllProperties: boolean, bSaveCreationInfo: boolean): boolean;
	/**
	 * Gets/sets the server on which the object stored on the current registry key will be created.
	 */
	Server(bstrServer: string): string;
	/**
	 * READONLY: Gets the string representation of the CLSID of the object stored on the current registry key.
	 */
	readonly ObjectCLSID: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCSessionInfo Interface

/**
 * ITCSessionInfo Interface
 */
interface ITCSessionInfo extends IDispatch
{
	/**
	 * READONLY: Gets the user name of this session.
	 */
	readonly UserName: string;
	/**
	 * READONLY: Gets the computer name of this session.
	 */
	readonly ComputerName: string;
	/**
	 * Gets/sets the application name of this session.
	 */
	ApplicationName(bstrAppName: [in]BSTR): [in]BSTR;
	/**
	 * READONLY: Returns the time the session was created.
	 */
	readonly TimeCreated: Date;
	/**
	 * READONLY: Returns the duration of the session.
	 */
	readonly Duration: Date;
	/**
	 * READONLY: Uniquely identifies this session within a process.
	 */
	readonly Cookie: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCSessionInfos Interface

/**
 * ITCSessionInfos interface
 */
interface ITCSessionInfos extends ITCCollection
{
	/**
	 * Returns a Session from the collection, or NULL if the session does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(index: object): ITCSessionInfo;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCCollectionPersistHelper Interface

/**
 * ITCCollectionPersistHelper interface
 */
interface ITCCollectionPersistHelper extends IDispatch
{
	/**
	 * Gets/sets the current thread priority.
	 */
	Collection1(pvarSafeArray: object): object;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITCUtility Interface

/**
 * ITCUtility interface
 */
interface ITCUtility extends IDispatch
{
	/**
	 * Creates the specified object on the specified computer.
	 */
	CreateObject(bstrProgID: string, bstrComputer: string): IUnknown;
	/**
	 * Create an object reference string.
	 */
	ObjectReference(pUnk: IUnknown): string;
	/**
	 * Sleeps for the specified number of milliseconds.
	 */
	Sleep(nDurationMS: number): number;
	/**
	 * Creates an object by binding the specified moniker display name.
	 */
	GetObject(bstrMoniker: string, bAllowUI: boolean, nTimeoutMS: number): IUnknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAdminSessionHelper Interface

/**
 * IAdminSessionHelper interface
 */
interface IAdminSessionHelper extends IDispatch
{
	/**
	 * READONLY: Indicates whether or not the Allegiance Game Server is currently running.
	 */
	readonly IsAllSrvRunning: boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCPrivate Interface

/**
 * Internally-used interface on an AGC object.
 */
interface IAGCPrivate extends IUnknown
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCCommon Interface

/**
 * IAGCCommon Interface
 */
interface IAGCCommon extends IDispatch
{
	/**
	 * READONLY: Every object in AGC has a unique id number and this is it.
	 */
	readonly Type: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCCollection Interface

/**
 * Base interface for AGC collection interfaces.
 */
interface IAGCCollection extends IAGCCommon
{
	/**
	 * READONLY: Returns the number of items in the collection.
	 */
	readonly Count: number;
	/**
	 * READONLY: Returns an enumerator object that implements IEnumVARIANT.
	 */
	readonly _NewEnum: IUnknown;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCBase Interface

/**
 * IAGCBase Interface
 */
interface IAGCBase extends IAGCCommon
{
	/**
	 * READONLY: 
	 */
	readonly ObjectType: Enums.AGCObjectType;
	/**
	 * READONLY: This returns the id of the object in terms of its type; use UniqueID() for a completely unique id.
	 */
	readonly ObjectID: AGCObjectID;
	/**
	 * READONLY: Returns the AGC game in which this object lives.
	 */
	readonly Game: IAGCGame;
	/**
	 * READONLY: Every object in AGC that has a base also has a unique id number and this is it.
	 */
	readonly UniqueID: AGCUniqueID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCModel Interface

/**
 * IAGCModel Interface
 */
interface IAGCModel extends IAGCBase
{
	/**
	 * READONLY: 
	 */
	readonly IsVisible: boolean;
	/**
	 * 
	 */
	IsSeenBySide(pTeam: IAGCTeam): boolean;
	/**
	 * READONLY: 
	 */
	readonly Position: IAGCVector;
	/**
	 * READONLY: 
	 */
	readonly Velocity: IAGCVector;
	/**
	 * READONLY: 
	 */
	readonly Orientation: IAGCOrientation;
	/**
	 * READONLY: 
	 */
	readonly Radius: number;
	/**
	 * READONLY: 
	 */
	readonly Team: IAGCTeam;
	/**
	 * READONLY: 
	 */
	readonly Mass: number;
	/**
	 * READONLY: 
	 */
	readonly Sector: IAGCSector;
	/**
	 * READONLY: 
	 */
	readonly Signature: number;
	/**
	 * READONLY: Gets the model's name
	 */
	readonly Name: string;
	/**
	 * READONLY: User can never pick the object
	 */
	readonly IsSelectable: boolean;
	/**
	 * READONLY: Can see other objects
	 */
	readonly IsScanner: boolean;
	/**
	 * READONLY: Send this object to a team when it is seen for the 1st time
	 */
	readonly IsPredictable: boolean;
	/**
	 * READONLY: Need to track its visibility with regard to side
	 */
	readonly IsScanRequired: boolean;
	/**
	 * READONLY: Can not move
	 */
	readonly IsStatic: boolean;
	/**
	 * READONLY: Can take damage
	 */
	readonly IsDamagable: boolean;
	/**
	 * READONLY: Goes into the collision KD tree
	 */
	readonly IsHitable: boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCModels Interface

/**
 * IAGCModels Interface
 */
interface IAGCModels extends IAGCCollection
{
	/**
	 * Returns an AGCModel from the collection, or NULL if the item does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCModel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCBuyable Interface

/**
 * Properties of an object that is buyable.
 */
interface IAGCBuyable extends IAGCBase
{
	/**
	 * READONLY: Gets the name of the object.
	 */
	readonly Name: string;
	/**
	 * READONLY: Gets the description of the object.
	 */
	readonly Description: string;
	/**
	 * READONLY: Gets the name of the model that buying this creates.
	 */
	readonly ModelName: string;
	/**
	 * READONLY: Gets price of the object.
	 */
	readonly Price: AGCMoney;
	/**
	 * READONLY: Gets time to build the object.
	 */
	readonly TimeToBuild: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCHullType Interface

/**
 * Interface to an AGC Hull Type object.
 */
interface IAGCHullType extends IAGCBuyable
{
	/**
	 * READONLY: Gets the Length.
	 */
	readonly Length: number;
	/**
	 * READONLY: Gets the MaxSpeed.
	 */
	readonly MaxSpeed: number;
	/**
	 * Gets the MaxTurnRate.
	 */
	MaxTurnRate(eAxis: Enums.AGCAxis): number;
	/**
	 * Gets the TurnTorque.
	 */
	TurnTorque(eAxis: Enums.AGCAxis): number;
	/**
	 * READONLY: Gets the Thrust.
	 */
	readonly Thrust: number;
	/**
	 * READONLY: Gets the SideMultiplier.
	 */
	readonly SideMultiplier: number;
	/**
	 * READONLY: Gets the BackMultiplier.
	 */
	readonly BackMultiplier: number;
	/**
	 * READONLY: Gets the ScannerRange.
	 */
	readonly ScannerRange: number;
	/**
	 * READONLY: Gets the MaxEnergy.
	 */
	readonly MaxEnergy: number;
	/**
	 * READONLY: Gets the RechargeRate.
	 */
	readonly RechargeRate: number;
	/**
	 * READONLY: Gets the HitPoints.
	 */
	readonly HitPoints: AGCHitPoints;
	/**
	 * Gets the PartMask.
	 */
	PartMask(et: Enums.AGCEquipmentType, mountID: AGCMount): AGCPartMask;
	/**
	 * READONLY: Gets the MaxWeapons.
	 */
	readonly MaxWeapons: AGCMount;
	/**
	 * READONLY: Gets the MaxFixedWeapons.
	 */
	readonly MaxFixedWeapons: AGCMount;
	/**
	 * READONLY: Gets the CanMount.
	 */
	readonly Mass: number;
	/**
	 * READONLY: Gets the Signature.
	 */
	readonly Signature: number;
	/**
	 * READONLY: Gets the Capabilities.
	 */
	readonly Capabilities: AGCHullAbilityBitMask;
	/**
	 * Determines if the object has the specified capabilities..
	 */
	HasCapability(habm: AGCHullAbilityBitMask): boolean;
	/**
	 * READONLY: Gets the MaxAmmo.
	 */
	readonly MaxAmmo: number;
	/**
	 * READONLY: Gets the MaxFuel.
	 */
	readonly MaxFuel: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCTreasure Interface

/**
 * 
 */
interface IAGCTreasure extends IAGCModel
{
	/**
	 * READONLY: 
	 */
	readonly Type: Enums.AGCTreasureType;
	/**
	 * READONLY: 
	 */
	readonly Amount: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCTreasures Interface

/**
 * IAGCTreasures Interface
 */
interface IAGCTreasures extends IAGCCollection
{
	/**
	 * Returns an AGCTreasure from the collection, or NULL if the game does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCTreasure;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCDamage Interface

/**
 * IAGCDamage Interface
 */
interface IAGCDamage extends IAGCModel
{
	/**
	 * READONLY: 
	 */
	readonly Fraction: number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCScanner Interface

/**
 * IAGCScanner Interface
 */
interface IAGCScanner extends IAGCDamage
{
	/**
	 * 
	 */
	InScannerRange(pModel: IAGCModel): boolean;
	/**
	 * 
	 */
	CanSee(pModel: IAGCModel): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCProbe Interface

/**
 * IAGCProbe Interface
 */
interface IAGCProbe extends IAGCScanner
{
	/**
	 * READONLY: Gets the type of projectiles fired, if any.
	 */
	readonly EmissionPoint: IAGCVector;
	/**
	 * READONLY: Gets the lifespan.
	 */
	readonly Lifespan: number;
	/**
	 * READONLY: Gets the weapon burst rate(?).
	 */
	readonly DtBurst: number;
	/**
	 * READONLY: Gets the weapon firing accuracy.
	 */
	readonly Accuracy: number;
	/**
	 * READONLY: Gets the indicator of the probe being a ripcord destination.
	 */
	readonly IsRipcord: boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCProbes Interface

/**
 * IAGCProbes Interface
 */
interface IAGCProbes extends IAGCCollection
{
	/**
	 * Returns an AGCProbe from the collection, or NULL if the item does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCProbe;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCGame Interface

/**
 * IAGCGame Interface
 */
interface IAGCGame extends IAGCCommon
{
	/**
	 * READONLY: Gets the game's name
	 */
	readonly Name: string;
	/**
	 * READONLY: Gets the collection of sectors in the game.
	 */
	readonly Sectors: IAGCSectors;
	/**
	 * READONLY: Gets the collection of teams in the game.
	 */
	readonly Teams: IAGCTeams;
	/**
	 * Gets the Ship associated with specified AGC ID.
	 */
	LookupShip(idAGC: AGCUniqueID): IAGCShip;
	/**
	 * READONLY: Gets the collection of the ships in the game.
	 */
	readonly Ships: IAGCShips;
	/**
	 * READONLY: Gets the collection of alephs in the game.
	 */
	readonly Alephs: IAGCAlephs;
	/**
	 * READONLY: Gets the collection of asteroids in the game.
	 */
	readonly Asteroids: IAGCAsteroids;
	/**
	 * READONLY: Gets the parameters used to create the game.
	 */
	readonly GameParameters: IAGCGameParameters;
	/**
	 * READONLY: Gets unique mission id.
	 */
	readonly GameID: AGCGameID;
	/**
	 * Gets the team associated with specified AGC ID.
	 */
	LookupTeam(idAGC: AGCObjectID): IAGCTeam;
	/**
	 * Sends the specified chat text to everyone in the Game.
	 */
	SendChat(bstrText: string, idSound: AGCSoundID): AGCSoundID;
	/**
	 * Sends the specified command to everyone in the Game.
	 */
	SendCommand(bstrCommand: string, pModelTarget: IAGCModel, idSound: AGCSoundID): AGCSoundID;
	/**
	 * READONLY: Gets the stage of the game.
	 */
	readonly GameStage: Enums.AGCGameStage;
	/**
	 * READONLY: Gets the collection of probes in the game.
	 */
	readonly Probes: IAGCProbes;
	/**
	 * READONLY: Gets the collection of buoys in the game.
	 */
	readonly Buoys: IAGCModels;
	/**
	 * READONLY: Gets the collection of treasures in the game.
	 */
	readonly Treasures: IAGCModels;
	/**
	 * READONLY: Gets the collection of mines in the game.
	 */
	readonly Mines: IAGCModels;
	/**
	 * READONLY: Gets the number of times this game has been 'reused'.
	 */
	readonly ReplayCount: number;
	/**
	 * READONLY: Gets a string that uniquely identifies the game context, within the game server.
	 */
	readonly ContextName: string;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCShip Interface

/**
 * IAGCShip Interface
 */
interface IAGCShip extends IAGCScanner
{
	/**
	 * Amount of ammo the ship has.
	 */
	Ammo(Val: number): number;
	/**
	 * Amount of fuel the ship has.
	 */
	Fuel(Val: number): number;
	/**
	 * Amount of energy the ship has.
	 */
	Energy(Val: number): number;
	/**
	 * Ship's Wing ID.
	 */
	WingID(Val: number): number;
	/**
	 * Sends the specified chat text to the ship.
	 */
	SendChat(bstrText: string, idSound: AGCSoundID): AGCSoundID;
	/**
	 * Sends the specified command to the ship.
	 */
	SendCommand(bstrCommand: string, pModelTarget: IAGCModel, idSound: AGCSoundID): AGCSoundID;
	/**
	 * Gets/sets the AutoDonate ship.
	 */
	AutoDonate(pShip: IAGCShip): IAGCShip;
	/**
	 * Gets/sets the ship's ShieldFraction
	 */
	ShieldFraction(pVal: number): number;
	/**
	 * READONLY: Gets the ship's Hull type.
	 */
	readonly HullType: IAGCHullType;
	/**
	 * READONLY: Gets the ship's base hull type.
	 */
	readonly BaseHullType: IAGCHullType;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCShips Interface

/**
 * IAGCShips Interface
 */
interface IAGCShips extends IAGCCollection
{
	/**
	 * Returns a AGCShip from the collection, or NULL if the item does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCShip;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCStation Interface

/**
 * IAGCStation Interface
 */
interface IAGCStation extends IAGCScanner
{
	/**
	 * Gets/sets the station's ShieldFraction
	 */
	ShieldFraction(pVal: number): number;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCStations Interface

/**
 * IAGCStations Interface
 */
interface IAGCStations extends IAGCCollection
{
	/**
	 * Returns a AGCStation from the collection, or NULL if the item does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCStation;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCAleph Interface

/**
 * IAGCAleph Interface
 */
interface IAGCAleph extends IAGCModel
{
	/**
	 * READONLY: Gets the destination aleph.
	 */
	readonly Destination: IAGCAleph;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCAlephs Interface

/**
 * IAGCAlephs Interface
 */
interface IAGCAlephs extends IAGCCollection
{
	/**
	 * Returns a AGCAleph from the collection, or NULL if the AGCAleph does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCAleph;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCSector Interface

/**
 * IAGCSector Interface
 */
interface IAGCSector extends IAGCBase
{
	/**
	 * READONLY: Gets the AGCSector's name
	 */
	readonly Name: string;
	/**
	 * Gets the collection of stations in the sector.
	 */
	Stations: IAGCStations;
	/**
	 * READONLY: Gets the collection of ships in the sector.
	 */
	readonly Ships: IAGCShips;
	/**
	 * READONLY: Gets the collection of alephs in the sector.
	 */
	readonly Alephs: IAGCAlephs;
	/**
	 * READONLY: Gets the collection of asteroids in the sector.
	 */
	readonly Asteroids: IAGCAsteroids;
	/**
	 * Sends the specified chat text to everyone in the Sector.
	 */
	SendChat(bstrText: string, bIncludeEnemies: boolean, idSound: AGCSoundID): AGCSoundID;
	/**
	 * Sends the specified command to everyone in the Sector.
	 */
	SendCommand(bstrCommand: string, pModelTarget: IAGCModel, bIncludeEnemies: boolean, idSound: AGCSoundID): AGCSoundID;
	/**
	 * READONLY: Gets sector's X position relative to other sectors.  This is used in the sector overview display.  This value never changes.
	 */
	readonly ScreenX: float ;
	/**
	 * READONLY: Gets sector's Y position relative to other sectors.  This is used in the sector overview display.  This value never changes.
	 */
	readonly ScreenY: float ;
	/**
	 * READONLY: Gets the collection of mines in the sector.
	 */
	readonly Mines: IAGCModels;
	/**
	 * READONLY: Gets the collection of missiles in the sector.
	 */
	readonly Missiles: IAGCModels;
	/**
	 * READONLY: Gets the collection of probes in the sector.
	 */
	readonly Probes: IAGCProbes;
	/**
	 * READONLY: Gets the collection of models in the sector.
	 */
	readonly Models: IAGCModels;
	/**
	 * READONLY: Gets the collection of selectable models in the sector.
	 */
	readonly SelectableModels: IAGCModels;
	/**
	 * READONLY: Gets the collection of treasures in the sector.
	 */
	readonly Treasures: IAGCModels;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCSectors Interface

/**
 * IAGCSectors Interface
 */
interface IAGCSectors extends IAGCCollection
{
	/**
	 * Returns a AGCSector from the collection, or NULL if the sector does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCSector;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCTeam Interface

/**
 * IAGCTeam Interface
 */
interface IAGCTeam extends IAGCBase
{
	/**
	 * READONLY: Gets the team's name
	 */
	readonly Name: string;
	/**
	 * Returns a collection of stations that belong to the team.
	 */
	Stations: IAGCStations;
	/**
	 * READONLY: Returns a Ships collection of the ships in the team.
	 */
	readonly Ships: IAGCShips;
	/**
	 * READONLY: Returns name of civ for team.
	 */
	readonly Civ: string;
	/**
	 * Sends the specified chat text to everyone on the Team.
	 */
	SendChat(bstrText: string, idWing: number, idSound: AGCSoundID): AGCSoundID;
	/**
	 * Sends the specified command to everyone on the Team.
	 */
	SendCommand(bstrCommand: string, pModelTarget: IAGCModel, idWing: number, idSound: AGCSoundID): AGCSoundID;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCTeams Interface

/**
 * IAGCTeams Interface
 */
interface IAGCTeams extends IAGCCollection
{
	/**
	 * Returns a AGCTeam from the collection, or NULL if the game does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCTeam;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCAsteroid Interface

/**
 * IAGCAsteroid Interface
 */
interface IAGCAsteroid extends IAGCDamage
{
	/**
	 * READONLY: Gets the amount of ore remaining on the asteroid.
	 */
	readonly Ore: number;
	/**
	 * READONLY: Gets the Capabilities bit mask of the object.
	 */
	readonly Capabilities: AGCAsteroidAbilityBitMask;
	/**
	 * Determines if the object has the specified capabilities.
	 */
	HasCapability(aabm: AGCAsteroidAbilityBitMask): boolean;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// IAGCAsteroids Interface

/**
 * IAGCAsteroids Interface
 */
interface IAGCAsteroids extends IAGCCollection
{
	/**
	 * Returns an AGCAsteroid from the collection, or NULL if the game does not exist. Takes an argument, index, which must be the index into the collection.
	 */
	Item(pvIndex: object): IAGCAsteroid;
}

