

create rule ZeroToOne as @Z >=0 AND @Z <=1























GO
create rule PercentByte as @P >= 0 AND @P <= 100























GO
setuser N'dbo'
GO

EXEC sp_addtype N'Percent', N'tinyint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'ShipID', N'int', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'ZeroToOne', N'real', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[ZeroToOne]', N'[ZeroToOne]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'AccessCode', N'char (32)', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'AttachpointID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'BuyableGroupID', N'tinyint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'Description', N'char (200)', N'null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'EffectID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'ExpendableID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'FileName', N'char (12)', N'null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'GlobalAttributeID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'HitPoints', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'MapID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'Name', N'char (24)', N'null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'PartID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'PercentByte', N'tinyint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[PercentByte]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'ProjectileID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'SectorID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'ShipTypeID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'SortOrderType', N'int', N'null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'SoundID', N'smallint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'StationClassID', N'tinyint', N'not null'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_addtype N'TechBits', N'char (256)', N'null'
GO

setuser
GO

CREATE TABLE [dbo].[AccessCodes] (
	[AccessCode] [AccessCode] NOT NULL ,
	[Status] [smallint] NOT NULL ,
	[CodeReassignedFrom] [AccessCode] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AccessCodeUsage] (
	[AccessCode] [AccessCode] NOT NULL ,
	[ZoneName] [Name] NOT NULL ,
	[FirstUsage] [smalldatetime] NOT NULL ,
	[LastUsage] [smalldatetime] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Chaff] (
	[ExpendableID] [ExpendableID] NOT NULL ,
	[Strength] [real] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[CharacterInfo] (
	[CharacterID] [int] NOT NULL ,
	[CharacterName] [Name] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[CreationDate] [datetime] NOT NULL ,
	[CanCheat] [bit] NOT NULL ,
	[ICQID] [int] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Constants] (
	[PointsWin] [float] NOT NULL ,
	[PointsWarpsSpotted] [float] NOT NULL ,
	[PointsAsteroidsSpotted] [float] NOT NULL ,
	[PointsMinerKills] [float] NOT NULL ,
	[PointsBuilderKills] [float] NOT NULL ,
	[PointsLayerKills] [float] NOT NULL ,
	[PointsBaseKills] [float] NOT NULL ,
	[PointsBaseCaptures] [float] NOT NULL ,
	[PointsTechsRecovered] [float] NOT NULL ,
	[PointsPlayerKills] [float] NOT NULL ,
	[PointsDeaths] [float] NOT NULL ,
	[PointsPilotBaseKills] [float] NOT NULL ,
	[PointsPilotBaseCaptures] [float] NOT NULL ,
	[PointsFlagCapture] [float] NOT NULL ,
	[PointsArtifactCapture] [float] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Drones] (
	[DroneID] [smallint] NOT NULL ,
	[Name] [char] (16) NOT NULL ,
	[Description] [Description] NOT NULL ,
	[Price] [int] NOT NULL ,
	[ShipTypeID] [ShipTypeID] NOT NULL ,
	[TechBitsRequired] [TechBits] NOT NULL ,
	[TechBitsEffect] [TechBits] NOT NULL ,
	[SecondsToBuild] [smallint] NOT NULL ,
	[DroneType] [int] NOT NULL ,
	[GroupID] [BuyableGroupID] NULL ,
	[SortOrder] [SortOrderType] NULL ,
	[ExpendableID] [ExpendableID] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Effects] (
	[EffectID] [EffectID] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[FileSound] [FileName] NOT NULL ,
	[IsLooping] [bit] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Events] (
	[EventID] [int] IDENTITY (1, 1) NOT NULL ,
	[Event] [int] NOT NULL ,
	[DateTime] [datetime] NULL ,
	[ComputerName] [varchar] (15) NOT NULL ,
	[Subject] [int] NOT NULL ,
	[SubjectName] [varchar] (32) NOT NULL ,
	[Context] [varchar] (24) NOT NULL ,
	[ObjectRef] [varchar] (4000) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Expendables] (
	[ExpendableID] [ExpendableID] NOT NULL ,
	[Price] [int] NOT NULL ,
	[FileModel] [FileName] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[TechBitsRequired] [TechBits] NOT NULL ,
	[TechBitsEffect] [TechBits] NOT NULL ,
	[SignatureShip] [real] NOT NULL ,
	[Mass] [real] NOT NULL ,
	[LoadTime] [real] NOT NULL ,
	[LifeSpan] [real] NOT NULL ,
	[SignatureInstance] [real] NOT NULL ,
	[HitPoints] [HitPoints] NOT NULL ,
	[FileModelInstance] [FileName] NOT NULL ,
	[FileTextureInstance] [FileName] NOT NULL ,
	[Radius] [real] NOT NULL ,
	[RateRotation] [real] NOT NULL ,
	[percentRed] [PercentByte] NOT NULL ,
	[percentGreen] [PercentByte] NOT NULL ,
	[percentBlue] [PercentByte] NOT NULL ,
	[percentAlpha] [PercentByte] NOT NULL ,
	[PRIcon] [FileName] NULL ,
	[PRIconInstance] [FileName] NULL ,
	[PartMask] [smallint] NOT NULL ,
	[DefenseType] [tinyint] NOT NULL ,
	[SecondsToBuild] [smallint] NOT NULL ,
	[GroupID] [BuyableGroupID] NULL ,
	[ExpendableSize] [smallint] NOT NULL ,
	[Capabilities] [smallint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[GameResults] (
	[GameID] [char] (17) NOT NULL ,
	[Name] [char] (64) NOT NULL ,
	[WinningTeam] [Name] NOT NULL ,
	[WinningTeamID] [smallint] NOT NULL ,
	[IsGoalConquest] [bit] NOT NULL ,
	[IsGoalCountdown] [bit] NOT NULL ,
	[IsGoalTeamKills] [bit] NOT NULL ,
	[IsGoalProsperity] [bit] NOT NULL ,
	[IsGoalArtifacts] [bit] NOT NULL ,
	[IsGoalFlags] [bit] NOT NULL ,
	[GoalConquest] [smallint] NOT NULL ,
	[GoalCountdown] [int] NOT NULL ,
	[GoalTeamKills] [smallint] NOT NULL ,
	[GoalProsperity] [real] NOT NULL ,
	[GoalArtifacts] [smallint] NOT NULL ,
	[GoalFlags] [smallint] NOT NULL ,
	[Duration] [int] NOT NULL ,
	[StartTime] AS (dateadd(second,(((-[Duration]))),[EndTime])) ,
	[EndTime] [datetime] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[GlobalAttributes] (
	[GlobalAttributeID] [GlobalAttributeID] NOT NULL ,
	[MaxSpeed] [smallint] NOT NULL ,
	[Thrust] [smallint] NOT NULL ,
	[TurnRate] [smallint] NOT NULL ,
	[TurnTorque] [smallint] NOT NULL ,
	[MaxArmorStation] [smallint] NOT NULL ,
	[ArmorRegenerationStation] [smallint] NOT NULL ,
	[MaxShieldStation] [smallint] NOT NULL ,
	[ShieldRegenerationStation] [smallint] NOT NULL ,
	[MaxArmorShip] [smallint] NOT NULL ,
	[MaxShieldShip] [smallint] NOT NULL ,
	[ShieldRegenerationShip] [smallint] NOT NULL ,
	[ScanRange] [smallint] NOT NULL ,
	[Signature] [smallint] NOT NULL ,
	[MaxEnergy] [smallint] NOT NULL ,
	[SpeedAmmo] [smallint] NOT NULL ,
	[LifespanEnergy] [smallint] NOT NULL ,
	[TurnRateMissile] [smallint] NOT NULL ,
	[MiningRate] [smallint] NOT NULL ,
	[MiningYield] [smallint] NOT NULL ,
	[RipcordTime] [smallint] NOT NULL ,
	[DamageGuns] [smallint] NOT NULL ,
	[DamageMissiles] [smallint] NOT NULL ,
	[DevelopmentCost] [smallint] NOT NULL ,
	[DevelopmentTime] [smallint] NOT NULL ,
	[MiningCapacity] [smallint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[InvitationLists] (
	[ListID] [int] NOT NULL ,
	[TeamIndex] [tinyint] NOT NULL ,
	[SubjectName] [Name] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Maps] (
	[MapID] [smallint] NOT NULL ,
	[MapName] [Name] NULL ,
	[FileName] [FileName] NOT NULL ,
	[Description] [Description] NULL ,
	[IsTrainingOnly] [bit] NOT NULL ,
	[IsUserOption] [bit] NOT NULL ,
	[NumTeams] [tinyint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Medals] (
	[MedalID] [int] NOT NULL ,
	[MedalName] [char] (48) NOT NULL ,
	[MedalDescription] [Description] NOT NULL ,
	[MedalSortOrder] [int] NOT NULL ,
	[MedalBitmap] [char] (12) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[MineInstances] (
	[MapID] [MapID] NOT NULL ,
	[MineID] [smallint] NOT NULL ,
	[MineTypeID] [smallint] NOT NULL ,
	[SideID] [tinyint] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[LocationX] [real] NOT NULL ,
	[LocationY] [real] NOT NULL ,
	[LocationZ] [real] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PartTypes] (
	[PartTypeID] [tinyint] NOT NULL ,
	[PartTypeName] [char] (16) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PlayerResults] (
	[GameID] [char] (17) NOT NULL ,
	[TeamID] [smallint] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[PlayerKills] [smallint] NOT NULL ,
	[BuilderKills] [smallint] NOT NULL ,
	[LayerKills] [smallint] NOT NULL ,
	[MinerKills] [smallint] NOT NULL ,
	[BaseKills] [smallint] NOT NULL ,
	[BaseCaptures] [smallint] NOT NULL ,
	[PilotBaseKills] [smallint] NOT NULL ,
	[PilotBaseCaptures] [smallint] NOT NULL ,
	[Deaths] [smallint] NOT NULL ,
	[Ejections] [smallint] NOT NULL ,
	[Rescues] [smallint] NOT NULL ,
	[Flags] [smallint] NOT NULL ,
	[Artifacts] [smallint] NOT NULL ,
	[TechsRecovered] [smallint] NOT NULL ,
	[AlephsSpotted] [smallint] NOT NULL ,
	[AsteroidsSpotted] [smallint] NOT NULL ,
	[CombatRating] [real] NOT NULL ,
	[Score] [real] NOT NULL ,
	[TimePlayed] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ProbeInstances] (
	[MapID] [MapID] NOT NULL ,
	[ProbeID] [smallint] NOT NULL ,
	[ProbeTypeID] [smallint] NOT NULL ,
	[SideID] [tinyint] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[LocationX] [real] NOT NULL ,
	[LocationY] [real] NOT NULL ,
	[LocationZ] [real] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Projectiles] (
	[ProjectileID] [ProjectileID] NOT NULL ,
	[TimeDuration] [smallint] NOT NULL ,
	[HitPointsInflict] [real] NOT NULL ,
	[DamageType] [tinyint] NOT NULL ,
	[FileModel] [FileName] NOT NULL ,
	[FileTexture] [FileName] NOT NULL ,
	[SpeedMax] [smallint] NOT NULL ,
	[Acceleration] [smallint] NOT NULL ,
	[Size_cm] [smallint] NOT NULL ,
	[RadiusBlast_m] [tinyint] NOT NULL ,
	[percentPenetration] [PercentByte] NOT NULL ,
	[RateRotation] [real] NOT NULL ,
	[IsAbsoluteSpeed] [bit] NOT NULL ,
	[IsDirectional] [bit] NOT NULL ,
	[IsExplodeOnExpire] [bit] NOT NULL ,
	[IsProximityFuse] [bit] NOT NULL ,
	[percentRed] [PercentByte] NOT NULL ,
	[percentGreen] [PercentByte] NOT NULL ,
	[percentBlue] [PercentByte] NOT NULL ,
	[percentAlpha] [PercentByte] NOT NULL ,
	[BlastPower] [real] NOT NULL ,
	[BlastRadius] [real] NOT NULL ,
	[WidthOverHeight] [real] NOT NULL ,
	[PRIcon] [FileName] NULL ,
	[AmbientSound] [SoundID] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PSSInstances] (
	[AccessCode] [AccessCode] NOT NULL ,
	[TrackingID] [char] (20) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Ranks] (
	[Rank] [smallint] NOT NULL ,
	[CivID] [smallint] NULL ,
	[Requirement] [int] NOT NULL ,
	[Name] [Name] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ShipClasses] (
	[ShipClassID] [smallint] NOT NULL ,
	[ShipClassName] [Name] NOT NULL ,
	[ShipClassDescription] [Description] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Squads] (
	[SquadID] [int] NOT NULL ,
	[SquadName] [Name] NOT NULL ,
	[Score] [smallint] NOT NULL ,
	[Wins] [smallint] NOT NULL ,
	[Losses] [smallint] NOT NULL ,
	[OwnershipLog] [varchar] (500) NOT NULL ,
	[CivID] [smallint] NULL ,
	[GamesPlayed] [smallint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[StationClasses] (
	[StationClassID] [smallint] NOT NULL ,
	[StationClassName] [Name] NOT NULL ,
	[StationClassDescription] [Description] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[StationTypes] (
	[StationTypeID] [smallint] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[Model] [FileName] NOT NULL ,
	[Description] [Description] NULL ,
	[Radius] [real] NOT NULL ,
	[HitPointsArmor] [HitPoints] NOT NULL ,
	[HitPointsShield] [HitPoints] NOT NULL ,
	[RateRegenShield] [real] NOT NULL ,
	[RateRegenArmor] [real] NOT NULL ,
	[Price] [int] NOT NULL ,
	[Signature] [smallint] NOT NULL ,
	[SecondsToBuild] [smallint] NOT NULL ,
	[TechBitsLocal] [TechBits] NOT NULL ,
	[TechBitsRequired] [TechBits] NOT NULL ,
	[TechBitsEffect] [TechBits] NOT NULL ,
	[UpgradeStationTypeID] [smallint] NULL ,
	[Capabilities] [smallint] NOT NULL ,
	[ScanRange] [real] NOT NULL ,
	[AsteroidDiscountMask] [smallint] NOT NULL ,
	[Income] [int] NOT NULL ,
	[PRIcon] [FileName] NULL ,
	[BuildLocationSound] [SoundID] NOT NULL ,
	[ConstructorAttackedSound] [SoundID] NOT NULL ,
	[ConstructorDestroyedSound] [SoundID] NOT NULL ,
	[CompletedSound] [SoundID] NOT NULL ,
	[ExteriorSound] [SoundID] NOT NULL ,
	[InteriorSound] [SoundID] NOT NULL ,
	[InteriorAlertSound] [SoundID] NOT NULL ,
	[UnderAttackSound] [SoundID] NOT NULL ,
	[CriticalSound] [SoundID] NOT NULL ,
	[DestroyedSound] [SoundID] NOT NULL ,
	[CapturedSound] [SoundID] NOT NULL ,
	[EnemyDestroyedSound] [SoundID] NOT NULL ,
	[EnemyCapturedSound] [SoundID] NOT NULL ,
	[GroupID] [BuyableGroupID] NOT NULL ,
	[ClassID] [StationClassID] NOT NULL ,
	[SecondsToBuy] [smallint] NOT NULL ,
	[DefenseTypeArmor] [tinyint] NOT NULL ,
	[DefenseTypeShield] [tinyint] NOT NULL ,
	[BuilderName] [Name] NULL ,
	[ConstructionDroneTypeID] [smallint] NOT NULL
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[TeamResults] (
	[GameID] [char] (17) NOT NULL ,
	[TeamID] [smallint] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[CivID] [smallint] NOT NULL ,
	[Techs] [char] (100) NOT NULL ,
	[PlayerKills] [smallint] NOT NULL ,
	[BaseKills] [smallint] NOT NULL ,
	[BaseCaptures] [smallint] NOT NULL ,
	[Deaths] [smallint] NOT NULL ,
	[Ejections] [smallint] NOT NULL ,
	[Flags] [smallint] NOT NULL ,
	[Artifacts] [smallint] NOT NULL ,
	[ConquestPercent] [smallint] NOT NULL ,
	[ProsperityPercentBought] [smallint] NOT NULL ,
	[ProsperityPercentComplete] [smallint] NOT NULL ,
	[TimeEndured] [int] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[TechBits] (
	[BitID] [int] NOT NULL ,
	[BitShortName] [char] (35) NOT NULL ,
	[BitLongName] [char] (48) NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[TreasureInstances] (
	[MapID] [MapID] NOT NULL ,
	[Lifespan] [real] NOT NULL ,
	[TreasureID] [smallint] NOT NULL ,
	[LocationX] [real] NOT NULL ,
	[LocationY] [real] NOT NULL ,
	[LocationZ] [real] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[Amount] [smallint] NOT NULL ,
	[TreasureCode] [smallint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[TreasureSets] (
	[TreasureSetName] [char] (40) NOT NULL ,
	[TreasureSetID] [smallint] NOT NULL ,
	[IsZoneOnly] [bit] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Developments] (
	[Price] [int] NOT NULL ,
	[LoadoutBitmap] [FileName] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[TechBitsRequired] [TechBits] NOT NULL ,
	[TechBitsEffect] [TechBits] NOT NULL ,
	[DevelopmentID] [smallint] NOT NULL ,
	[SecondsToBuild] [smallint] NOT NULL ,
	[GlobalAttributeID] [GlobalAttributeID] NULL ,
	[PRIcon] [char] (12) NULL ,
	[GroupID] [BuyableGroupID] NULL ,
	[SortOrder] [SortOrderType] NULL ,
	[CompletedSound] [SoundID] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Mines] (
	[ExpendableID] [ExpendableID] NOT NULL ,
	[PlacementRadius] [real] NOT NULL ,
	[MunitionCount] [tinyint] NOT NULL ,
	[Power] [real] NOT NULL ,
	[DamageType] [tinyint] NOT NULL ,
	[FileTextureMunition] [FileName] NOT NULL ,
	[percentRedMunition] [PercentByte] NOT NULL ,
	[percentGreenMunition] [PercentByte] NOT NULL ,
	[percentBlueMunition] [PercentByte] NOT NULL ,
	[percentAlphaMunition] [PercentByte] NOT NULL ,
	[PRIconMunition] [FileName] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Missiles] (
	[ExpendableID] [ExpendableID] NOT NULL ,
	[Acceleration] [real] NOT NULL ,
	[TurnRate] [real] NOT NULL ,
	[InitialSpeed] [real] NOT NULL ,
	[ArmTime] [real] NOT NULL ,
	[LockTime] [real] NOT NULL ,
	[ReadyTime] [real] NOT NULL ,
	[Dispersion] [real] NOT NULL ,
	[LockAngle] [real] NOT NULL ,
	[Power] [real] NOT NULL ,
	[DamageType] [tinyint] NOT NULL ,
	[BlastPower] [real] NOT NULL ,
	[BlastRadius] [real] NOT NULL ,
	[IsDirectional] [bit] NOT NULL ,
	[WidthOverHeight] [real] NOT NULL ,
	[MaxLock] [real] NOT NULL ,
	[ChaffResistance] [real] NOT NULL ,
	[LaunchSound] [SoundID] NOT NULL ,
	[FlightSound] [SoundID] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Parts] (
	[PartID] [PartID] NOT NULL ,
	[Price] [int] NOT NULL ,
	[FileModel] [FileName] NOT NULL ,
	[FileTexture] [FileName] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[PartTypeID] [tinyint] NOT NULL ,
	[LastUpdated] [smalldatetime] NULL ,
	[Mass] [smallint] NOT NULL ,
	[dSignatureMounted] [smallint] NOT NULL ,
	[dSignatureActive] [smallint] NOT NULL ,
	[TechBitsRequired] [TechBits] NOT NULL ,
	[TechBitsEffect] [TechBits] NOT NULL ,
	[ChanceTreasure] [PercentByte] NOT NULL ,
	[PRIcon] [FileName] NULL ,
	[PartMask] [smallint] NOT NULL ,
	[SuccessorPartID] [PartID] NULL ,
	[InventoryLineMDL] [FileName] NOT NULL ,
	[SortOrder] [SortOrderType] NULL ,
	[SecondsToBuild] [smallint] NOT NULL ,
	[GroupID] [BuyableGroupID] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Probes] (
	[ExpendableID] [ExpendableID] NOT NULL ,
	[ScanRange] [real] NOT NULL ,
	[ProjectileID] [ProjectileID] NULL ,
	[dtimeBurst] [smallint] NOT NULL ,
	[accuracy] [PercentByte] NOT NULL ,
	[dispersion] [float] NOT NULL ,
	[AmbientSound] [SoundID] NOT NULL ,
	[Ammo] [smallint] NOT NULL ,
	[dtRipcord] [tinyint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[SectorInfo] (
	[MapID] [MapID] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[SectorName] [Name] NOT NULL ,
	[LightX] [real] NOT NULL ,
	[LightY] [real] NOT NULL ,
	[LightZ] [real] NOT NULL ,
	[LightColor] [int] NOT NULL ,
	[StarsCount] [smallint] NOT NULL ,
	[DebrisCount] [smallint] NOT NULL ,
	[starSeed] [int] NOT NULL ,
	[ScreenX] [real] NOT NULL ,
	[ScreenY] [real] NOT NULL ,
	[PosterFile] [FileName] NULL ,
	[PlanetFile] [FileName] NULL ,
	[PlanetSinLatitude] [PercentByte] NULL ,
	[PlanetLongitude] [PercentByte] NULL ,
	[PlanetRadius] [tinyint] NULL ,
	[IsHomeSector] [bit] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Afterburners] (
	[PartID] [PartID] NOT NULL ,
	[BurnRate] [real] NOT NULL ,
	[MaxThrust] [real] NOT NULL ,
	[HullThrustMultiplier] [real] NOT NULL ,
	[rateOn] [real] NOT NULL ,
	[rateOff] [real] NOT NULL ,
	[InteriorSound] [SoundID] NOT NULL ,
	[ExteriorSound] [SoundID] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AlephInstances] (
	[AlephID] [int] NOT NULL ,
	[MapID] [MapID] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[LocationX] [real] NOT NULL ,
	[LocationY] [real] NOT NULL ,
	[LocationZ] [real] NOT NULL ,
	[TargetAlephID] [int] NOT NULL ,
	[RotationR] [real] NOT NULL ,
	[ForwardX] [real] NOT NULL ,
	[ForwardY] [real] NULL ,
	[ForwardZ] [real] NULL ,
	[Radius] [smallint] NOT NULL ,
	[Signature] [real] NOT NULL ,
	[Texture] [FileName] NOT NULL ,
	[PRIcon] [FileName] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Ammo] (
	[PartID] [PartID] NOT NULL ,
	[Qty] [smallint] NOT NULL ,
	[AmmoType] [tinyint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Asteroids] (
	[MapID] [MapID] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[AsteroidID] [smallint] NOT NULL ,
	[Ore] [real] NOT NULL ,
	[AbilitiesMask] [smallint] NOT NULL ,
	[HitPoints] [HitPoints] NOT NULL ,
	[Radius] [smallint] NOT NULL ,
	[FileModel] [FileName] NOT NULL ,
	[FileTexture] [FileName] NULL ,
	[FileIcon] [FileName] NOT NULL ,
	[Mass] [real] NOT NULL ,
	[Signature] [real] NOT NULL ,
	[LocationX] [real] NOT NULL ,
	[LocationY] [real] NOT NULL ,
	[LocationZ] [real] NOT NULL ,
	[UpX] [real] NULL ,
	[UpY] [real] NULL ,
	[UpZ] [real] NULL ,
	[ForwardX] [real] NULL ,
	[ForwardY] [real] NULL ,
	[ForwardZ] [real] NULL ,
	[RotationAx] [real] NULL ,
	[RotationAy] [real] NULL ,
	[RotationAz] [real] NULL ,
	[RotationR] [real] NULL ,
	[Name] [Name] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Civs] (
	[CivID] [smallint] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[EscapePodShipTypeID] [ShipTypeID] NOT NULL ,
	[TechBitsCiv] [char] (800) NOT NULL ,
	[StartingGlobalAttributeID] [GlobalAttributeID] NULL ,
	[InitialStationTypeID] [smallint] NOT NULL ,
	[IconName] [FileName] NOT NULL ,
	[StationHUD] [FileName] NOT NULL ,
	[PreferredPartID15] [PartID] NULL ,
	[PreferredPartID14] [PartID] NULL ,
	[PreferredPartID13] [PartID] NULL ,
	[PreferredPartID12] [PartID] NULL ,
	[PreferredPartID11] [PartID] NULL ,
	[PreferredPartID10] [PartID] NULL ,
	[PreferredPartID9] [PartID] NULL ,
	[PreferredPartID8] [PartID] NULL ,
	[PreferredPartID7] [PartID] NULL ,
	[PreferredPartID6] [PartID] NULL ,
	[PreferredPartID5] [PartID] NULL ,
	[PreferredPartID4] [PartID] NULL ,
	[PreferredPartID3] [PartID] NULL ,
	[PreferredPartID2] [PartID] NULL ,
	[PreferredPartID1] [PartID] NULL ,
	[TechBitsCivNoDev] [char] (800) NOT NULL ,
	[BonusMoney] [real] NOT NULL ,
	[IncomeMoney] [real] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Cloaks] (
	[PartID] [PartID] NOT NULL ,
	[EnergyConsumption] [real] NOT NULL ,
	[MaxCloaking] [real] NOT NULL ,
	[OnRate] [real] NOT NULL ,
	[OffRate] [real] NOT NULL ,
	[EngageSound] [SoundID] NOT NULL ,
	[DisengageSound] [SoundID] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Magazines] (
	[PartID] [smallint] NOT NULL ,
	[Amount] [smallint] NOT NULL ,
	[ExpendableID] [ExpendableID] NOT NULL ,
	[SuccessorPartID] [PartID] NULL ,
	[LaunchCount] [smallint] NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[PosterInstances] (
	[PosterID] [int] NOT NULL ,
	[PosterName] [Name] NOT NULL ,
	[Texture] [FileName] NOT NULL ,
	[MapID] [MapID] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[Radius] [int] NOT NULL ,
	[LocationX] [int] NOT NULL ,
	[LocationY] [int] NOT NULL ,
	[LocationZ] [int] NOT NULL ,
	[Color] [int] NOT NULL ,
	[Longitude] [smallint] NOT NULL ,
	[Latitude] [smallint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Shields] (
	[PartID] [PartID] NOT NULL ,
	[RegenRate] [real] NOT NULL ,
	[HitPoints] [HitPoints] NOT NULL ,
	[ActivateSound] [SoundID] NOT NULL ,
	[DeactivateSound] [SoundID] NOT NULL ,
	[DefenseType] [tinyint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[ShipTypes] (
	[Name] [char] (20) NOT NULL ,
	[ShipTypeID] [ShipTypeID] NOT NULL ,
	[Model] [FileName] NOT NULL ,
	[Length] [real] NOT NULL ,
	[Weight] [real] NOT NULL ,
	[Price] [int] NOT NULL ,
	[BaseMaxSpeed] [int] NOT NULL ,
	[BaseHitPoints] [HitPoints] NOT NULL ,
	[BaseScannerRange] [int] NOT NULL ,
	[Description] [Description] NOT NULL ,
	[MaxRollRate] [real] NOT NULL ,
	[MaxPitchRate] [real] NOT NULL ,
	[MaxYawRate] [real] NOT NULL ,
	[DriftRoll] [real] NOT NULL ,
	[DriftPitch] [real] NOT NULL ,
	[DriftYaw] [real] NOT NULL ,
	[Acceleration] [real] NOT NULL ,
	[AccelSideMultiplier] [real] NOT NULL ,
	[AccelBackMultiplier] [real] NOT NULL ,
	[BaseSignature] [smallint] NOT NULL ,
	[EnergyMax] [real] NOT NULL ,
	[RateRechargeEnergy] [real] NOT NULL ,
	[TechBitsRequired] [TechBits] NOT NULL ,
	[TechBitsEffect] [TechBits] NOT NULL ,
	[Capabilities] [smallint] NOT NULL ,
	[MaxAmmo] [smallint] NOT NULL ,
	[MaxFuel] [smallint] NOT NULL ,
	[PRIcon] [FileName] NOT NULL ,
	[ShieldPartMask] [smallint] NOT NULL ,
	[AfterburnerPartMask] [smallint] NOT NULL ,
	[CloakPartMask] [smallint] NOT NULL ,
	[MagazinePartMask] [smallint] NOT NULL ,
	[DispenserPartMask] [smallint] NOT NULL ,
	[ChaffPartMask] [smallint] NOT NULL ,
	[MagazineCapacity] [smallint] NOT NULL ,
	[DispenserCapacity] [smallint] NOT NULL ,
	[ChaffCapacity] [smallint] NOT NULL ,
	[SuccessorHullID] [ShipTypeID] NULL ,
	[PilotHUD] [FileName] NOT NULL ,
	[ObserverHUD] [FileName] NOT NULL ,
	[InteriorSound] [SoundID] NOT NULL ,
	[ExteriorSound] [SoundID] NOT NULL ,
	[ThrustInteriorSound] [SoundID] NOT NULL ,
	[ThrustExteriorSound] [SoundID] NOT NULL ,
	[TurnInteriorSound] [SoundID] NOT NULL ,
	[TurnExteriorSound] [SoundID] NOT NULL ,
	[SortOrder] [SortOrderType] NULL ,
	[PreferredPartID14] [PartID] NULL ,
	[PreferredPartID13] [PartID] NULL ,
	[PreferredPartID12] [PartID] NULL ,
	[PreferredPartID11] [PartID] NULL ,
	[PreferredPartID10] [PartID] NULL ,
	[PreferredPartID9] [PartID] NULL ,
	[PreferredPartID8] [PartID] NULL ,
	[PreferredPartID7] [PartID] NULL ,
	[PreferredPartID6] [PartID] NULL ,
	[PreferredPartID5] [PartID] NULL ,
	[PreferredPartID4] [PartID] NULL ,
	[PreferredPartID3] [PartID] NULL ,
	[PreferredPartID2] [PartID] NULL ,
	[PreferredPartID1] [PartID] NULL ,
	[DefenseType] [tinyint] NOT NULL ,
	[SecondsToBuild] [smallint] NOT NULL ,
	[GroupID] [BuyableGroupID] NULL ,
	[RipcordSpeed] [real] NOT NULL ,
	[ECM] [real] NOT NULL ,
	[RipcordCost] [real] NOT NULL
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[StationInstances] (
	[MapID] [MapID] NOT NULL ,
	[StationID] [smallint] NOT NULL ,
	[Name] [Name] NOT NULL ,
	[SideID] [tinyint] NOT NULL ,
	[SectorID] [SectorID] NOT NULL ,
	[LocationX] [real] NOT NULL ,
	[LocationY] [real] NOT NULL ,
	[LocationZ] [real] NOT NULL ,
	[RotationAx] [real] NOT NULL ,
	[RotationAy] [real] NOT NULL ,
	[RotationAz] [real] NOT NULL ,
	[RotationR] [real] NOT NULL ,
	[ForwardX] [real] NOT NULL ,
	[ForwardY] [real] NOT NULL ,
	[ForwardZ] [real] NOT NULL ,
	[UpX] [real] NOT NULL ,
	[UpY] [real] NOT NULL ,
	[UpZ] [real] NOT NULL ,
	[StationTypeID] [smallint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[TreasureChances] (
	[ObjectID] [smallint] NOT NULL ,
	[Chance] [tinyint] NOT NULL ,
	[TreasureSetID] [smallint] NOT NULL ,
	[TreasureCode] [tinyint] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[Weapons] (
	[PartID] [PartID] NOT NULL ,
	[dTimeBurstShots] [smallint] NOT NULL ,
	[dTimeReady] [smallint] NOT NULL ,
	[EnergyPerShot] [real] NOT NULL ,
	[ProjectileID1] [ProjectileID] NOT NULL ,
	[cBulletsPerShot] [smallint] NOT NULL ,
	[ActivateSound] [SoundID] NOT NULL ,
	[SingleShotSound] [SoundID] NOT NULL ,
	[BurstShotSound] [SoundID] NOT NULL ,
	[Dispersion] [real] NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[AttachPoints] (
	[ShipTypeID] [ShipTypeID] NOT NULL ,
	[AttachPointID] [AttachpointID] NOT NULL ,
	[FrameName] [FileName] NOT NULL ,
	[PartTypeID] [tinyint] NOT NULL ,
	[FieldOfFire] [smallint] NOT NULL ,
	[PartMask] [smallint] NOT NULL ,
	[GunnerHUD] [FileName] NOT NULL ,
	[InteriorSound] [SoundID] NOT NULL ,
	[TurnSound] [SoundID] NOT NULL ,
	[LocationAbreviation] [char] (8) NOT NULL 
) ON [PRIMARY]
GO

CREATE TABLE [dbo].[CharMedals] (
	[CharacterID] [int] NOT NULL ,
	[MedalID] [int] NOT NULL ,
	[CivID] [smallint] NULL ,
	[SpecificInfo] [char] (48) NULL 
)
GO

CREATE TABLE [dbo].[CharStats] (
	[CharacterID] [int] NOT NULL ,
	[CharacterName] [Name] NOT NULL ,
	[CivID] [smallint] NULL ,
	[Rank] [smallint] NOT NULL ,
	[Score] [real] NOT NULL ,
	[MinutesPlayed] [int] NOT NULL ,
	[BaseKills] [smallint] NOT NULL ,
	[BaseCaptures] [smallint] NOT NULL ,
	[Kills] [smallint] NOT NULL ,
	[Rating] [smallint] NOT NULL ,
	[GamesPlayed] [smallint] NOT NULL ,
	[Deaths] [smallint] NOT NULL ,
	[Wins] [smallint] NOT NULL ,
	[Losses] [smallint] NOT NULL ,
	[WinsCmd] [smallint] NOT NULL ,
	[LossesCmd] [smallint] NOT NULL ,
	[LastPlayed] [smalldatetime] NOT NULL 
) ON [PRIMARY]
GO

 CREATE  CLUSTERED  INDEX [IX_CharMedals] ON [dbo].[CharMedals]([CharacterID], [CivID]) ON [PRIMARY]
GO

ALTER TABLE [dbo].[AccessCodes] WITH NOCHECK ADD 
	CONSTRAINT [DF_AccessCodes_Status] DEFAULT (0) FOR [Status],
	CONSTRAINT [PK_AccessCodes] PRIMARY KEY  NONCLUSTERED 
	(
		[AccessCode]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AccessCodeUsage] WITH NOCHECK ADD 
	CONSTRAINT [DF_AccessCodeUsage_FirstUsage] DEFAULT (getdate()) FOR [FirstUsage],
	CONSTRAINT [DF_AccessCodeUsage_LastUsage] DEFAULT (getdate()) FOR [LastUsage],
	CONSTRAINT [PK_AccessCodeUsage] PRIMARY KEY  NONCLUSTERED 
	(
		[AccessCode],
		[ZoneName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Chaff] WITH NOCHECK ADD 
	CONSTRAINT [DF_Chaff_Strength] DEFAULT (1) FOR [Strength],
	CONSTRAINT [PK_Chaff] PRIMARY KEY  NONCLUSTERED 
	(
		[ExpendableID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[CharacterInfo] WITH NOCHECK ADD 
	CONSTRAINT [DF_CharacterInfo_Description] DEFAULT ('') FOR [Description],
	CONSTRAINT [DF__character__creat__0F8D3381] DEFAULT (getdate()) FOR [CreationDate],
	CONSTRAINT [DF_CharacterInfo_CanCheat] DEFAULT (0) FOR [CanCheat],
	CONSTRAINT [PK_CharacterInfo] PRIMARY KEY  NONCLUSTERED 
	(
		[CharacterID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Drones] WITH NOCHECK ADD 
	CONSTRAINT [DF_Drones_TechBitsRequired] DEFAULT (0) FOR [TechBitsRequired],
	CONSTRAINT [DF_Drones_TechBitsEffect] DEFAULT (0) FOR [TechBitsEffect],
	CONSTRAINT [DF_Drones_SecondsToBuild] DEFAULT (30) FOR [SecondsToBuild],
	CONSTRAINT [DF_Drones_GroupID] DEFAULT (0) FOR [GroupID],
	CONSTRAINT [PK_Drones] PRIMARY KEY  NONCLUSTERED 
	(
		[DroneID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Drones] CHECK ([Price] > 0 or [DroneType] = 6)
GO

ALTER TABLE [dbo].[Effects] WITH NOCHECK ADD 
	CONSTRAINT [PK_Effects] PRIMARY KEY  NONCLUSTERED 
	(
		[EffectID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Events] WITH NOCHECK ADD 
	CONSTRAINT [DF_Events_DateTime] DEFAULT (getdate()) FOR [DateTime]
GO

ALTER TABLE [dbo].[Expendables] WITH NOCHECK ADD 
	CONSTRAINT [DF_Expendables_TechBitsRequired] DEFAULT ('') FOR [TechBitsRequired],
	CONSTRAINT [DF_Expendables_TechBitsEffect] DEFAULT ('') FOR [TechBitsEffect],
	CONSTRAINT [DF_Expendable_ShipSignatureMod] DEFAULT (1) FOR [SignatureShip],
	CONSTRAINT [DF_Expendables_PartSize] DEFAULT (0) FOR [PartMask],
	CONSTRAINT [DF_Expendables_SecondsToBuild] DEFAULT (0) FOR [SecondsToBuild],
	CONSTRAINT [DF_Expendables_GroupID] DEFAULT (0) FOR [GroupID],
	CONSTRAINT [PK_Expendable] PRIMARY KEY  NONCLUSTERED 
	(
		[ExpendableID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Expendables] CHECK ([Price] >= 0 and [Mass] > 0 and [Radius] > 0 and [LoadTime] >= 0 and [LifeSpan] >= 0 and [HitPoints] > 0)
GO

ALTER TABLE [dbo].[GameResults] WITH NOCHECK ADD 
	CONSTRAINT [DF__GameResul__EndTi__36B12243] DEFAULT (getdate()) FOR [EndTime]
GO

ALTER TABLE [dbo].[GlobalAttributes] WITH NOCHECK ADD 
	CONSTRAINT [DF_GlobalAttributes_MaxSpeed] DEFAULT (0) FOR [MaxSpeed],
	CONSTRAINT [DF_GlobalAttributes_Thrust] DEFAULT (0) FOR [Thrust],
	CONSTRAINT [DF_GlobalAttributes_TurnRate] DEFAULT (0) FOR [TurnRate],
	CONSTRAINT [DF_GlobalAttributes_TurnTorque] DEFAULT (0) FOR [TurnTorque],
	CONSTRAINT [DF_GlobalAttributes_MaxArmorStation] DEFAULT (0) FOR [MaxArmorStation],
	CONSTRAINT [DF_GlobalAttributes_ArmorRegenerationStation] DEFAULT (0) FOR [ArmorRegenerationStation],
	CONSTRAINT [DF_GlobalAttributes_MaxShieldStation] DEFAULT (0) FOR [MaxShieldStation],
	CONSTRAINT [DF_GlobalAttributes_ShieldRegenerationStation] DEFAULT (0) FOR [ShieldRegenerationStation],
	CONSTRAINT [DF_GlobalAttributes_MaxArmorShip] DEFAULT (0) FOR [MaxArmorShip],
	CONSTRAINT [DF_GlobalAttributes_MaxShieldShip] DEFAULT (0) FOR [MaxShieldShip],
	CONSTRAINT [DF_GlobalAttributes_ShieldRegenerationShip] DEFAULT (0) FOR [ShieldRegenerationShip],
	CONSTRAINT [DF_GlobalAttributes_ScanRange] DEFAULT (0) FOR [ScanRange],
	CONSTRAINT [DF_GlobalAttributes_Signature] DEFAULT (0) FOR [Signature],
	CONSTRAINT [DF_GlobalAttributes_MaxEnergy] DEFAULT (0) FOR [MaxEnergy],
	CONSTRAINT [DF_GlobalAttributes_SpeedAmmo] DEFAULT (0) FOR [SpeedAmmo],
	CONSTRAINT [DF_GlobalAttributes_LifespanEnergy] DEFAULT (0) FOR [LifespanEnergy],
	CONSTRAINT [DF_GlobalAttributes_TurnRateMissile] DEFAULT (0) FOR [TurnRateMissile],
	CONSTRAINT [DF_GlobalAttributes_MiningRate] DEFAULT (0) FOR [MiningRate],
	CONSTRAINT [DF_GlobalAttributes_MiningYield] DEFAULT (0) FOR [MiningYield],
	CONSTRAINT [DF_GlobalAttributes_RipcordTime] DEFAULT (0) FOR [RipcordTime],
	CONSTRAINT [DF_GlobalAttributes_DamageGuns] DEFAULT (0) FOR [DamageGuns],
	CONSTRAINT [DF_GlobalAttributes_DamageMissiles] DEFAULT (0) FOR [DamageMissiles],
	CONSTRAINT [DF_GlobalAttributes_DevelopmentCost] DEFAULT (0) FOR [DevelopmentCost],
	CONSTRAINT [DF_GlobalAttributes_DevelopmentTime] DEFAULT (0) FOR [DevelopmentTime],
	CONSTRAINT [DF_GlobalAttributes_MiningCapacity] DEFAULT (0) FOR [MiningCapacity],
	CONSTRAINT [PK_GlobalAttributes] PRIMARY KEY  NONCLUSTERED 
	(
		[GlobalAttributeID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Maps] WITH NOCHECK ADD 
	CONSTRAINT [DF_Maps_IsUserMap] DEFAULT (0) FOR [IsUserOption],
	CONSTRAINT [DF_Maps_NumTeams] DEFAULT (2) FOR [NumTeams],
	CONSTRAINT [PK_Maps] PRIMARY KEY  NONCLUSTERED 
	(
		[MapID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Medals] WITH NOCHECK ADD 
	CONSTRAINT [DF_Medals_MedalRanking] DEFAULT (100000) FOR [MedalSortOrder],
	CONSTRAINT [PK_Medals] PRIMARY KEY  NONCLUSTERED 
	(
		[MedalID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PartTypes] WITH NOCHECK ADD 
	CONSTRAINT [PK_PartTypes] PRIMARY KEY  NONCLUSTERED 
	(
		[PartTypeID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Projectiles] WITH NOCHECK ADD 
	CONSTRAINT [DF_Projectiles_DamageType] DEFAULT (0) FOR [DamageType],
	CONSTRAINT [DF_Projectiles_BlastPower] DEFAULT (0) FOR [BlastPower],
	CONSTRAINT [DF_Projectiles_BlastRadius] DEFAULT (0) FOR [BlastRadius],
	CONSTRAINT [DF_Projectiles_WidthOverHeight] DEFAULT (1) FOR [WidthOverHeight],
	CONSTRAINT [DF_Projectiles_AmbientSound] DEFAULT ((-1)) FOR [AmbientSound],
	CONSTRAINT [PK_Projectiles] PRIMARY KEY  NONCLUSTERED 
	(
		[ProjectileID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Projectiles] CHECK ([Size_cm] > 0 and [TimeDuration] >= 200 and [SpeedMax] <= 5000 and [SpeedMax] > 50)
GO

ALTER TABLE [dbo].[PSSInstances] WITH NOCHECK ADD 
	CONSTRAINT [PK_PSSInstances] PRIMARY KEY  NONCLUSTERED 
	(
		[AccessCode],
		[TrackingID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Ranks] WITH NOCHECK ADD 
	CONSTRAINT [DF_Ranks_Requirement] DEFAULT (0) FOR [Requirement],
	CONSTRAINT [DF_Ranks_Name] DEFAULT ('') FOR [Name],
	CONSTRAINT [IX_Ranks] UNIQUE  NONCLUSTERED 
	(
		[CivID],
		[Rank]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ShipClasses] WITH NOCHECK ADD 
	CONSTRAINT [PK_ShipClasses] PRIMARY KEY  NONCLUSTERED 
	(
		[ShipClassID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Squads] WITH NOCHECK ADD 
	CONSTRAINT [DF_Squads_Score] DEFAULT (0) FOR [Score],
	CONSTRAINT [DF_Squads_Wins] DEFAULT (0) FOR [Wins],
	CONSTRAINT [DF_Squads_Losses] DEFAULT (0) FOR [Losses],
	CONSTRAINT [DF_Squads_OwnershipLog] DEFAULT ('') FOR [OwnershipLog],
	CONSTRAINT [DF_Squads_GamesPlayed] DEFAULT (0) FOR [GamesPlayed],
	CONSTRAINT [PK_Squads] PRIMARY KEY  NONCLUSTERED 
	(
		[SquadID]
	)  ON [PRIMARY] ,
	CONSTRAINT [IX_Squads_Name] UNIQUE  NONCLUSTERED 
	(
		[SquadName]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[StationClasses] WITH NOCHECK ADD 
	CONSTRAINT [PK_StationClasses] PRIMARY KEY  NONCLUSTERED 
	(
		[StationClassID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[StationTypes] WITH NOCHECK ADD 

	CONSTRAINT [PK_StationTypes] PRIMARY KEY  NONCLUSTERED 
	(
		[StationTypeID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_StationTypes] CHECK ([Radius] > 0 and [HitPointsArmor] > 0 and [HitPointsShield] >= 0 and [RateRegenArmor] >= 0 and [RateRegenShield] >= 0 and [Price] > 0 and [Signature] >= 0 and [SecondsToBuild] > 0 and [ScanRange] >= 0 and [UpgradeStationTypeID] < [StationTypeID])
GO

ALTER TABLE [dbo].[TechBits] WITH NOCHECK ADD 
	CONSTRAINT [PK_TechBits] PRIMARY KEY  NONCLUSTERED 
	(
		[BitShortName]
	)  ON [PRIMARY] ,
	CONSTRAINT [IX_TechBits_ID] UNIQUE  NONCLUSTERED 
	(
		[BitID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[TreasureSets] WITH NOCHECK ADD 
	CONSTRAINT [DF_TreasureSets_IsZoneOnly] DEFAULT (0) FOR [IsZoneOnly],
	CONSTRAINT [PK_TreasureSets] PRIMARY KEY  NONCLUSTERED 
	(
		[TreasureSetID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Developments] WITH NOCHECK ADD 
	CONSTRAINT [DF_Developments_Price] DEFAULT (1000) FOR [Price],
	CONSTRAINT [DF_Development_TechBitsReq] DEFAULT (0) FOR [TechBitsRequired],
	CONSTRAINT [DF_Developments_TechBitsEffect] DEFAULT (0) FOR [TechBitsEffect],
	CONSTRAINT [DF_Developments_SecondsToBuild] DEFAULT (30) FOR [SecondsToBuild],
	CONSTRAINT [DF_Developments_GroupID] DEFAULT (1) FOR [GroupID],
	CONSTRAINT [PK_Developments] PRIMARY KEY  NONCLUSTERED 
	(
		[DevelopmentID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Developments] CHECK ([Price] > 0)
GO

ALTER TABLE [dbo].[Mines] WITH NOCHECK ADD 
	CONSTRAINT [DF_Mines_DamageType] DEFAULT (0) FOR [DamageType],
	CONSTRAINT [PK_Mines] PRIMARY KEY  NONCLUSTERED 
	(
		[ExpendableID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Mines] CHECK ([MunitionCount] <= 50)
GO

ALTER TABLE [dbo].[Missiles] WITH NOCHECK ADD 
	CONSTRAINT [DF_Missiles_BlastPower] DEFAULT (0) FOR [BlastPower],
	CONSTRAINT [DF_Missiles_BlastRadius] DEFAULT (0) FOR [BlastRadius],
	CONSTRAINT [DF_Missiles_WidthOverHeight] DEFAULT (1) FOR [WidthOverHeight],
	CONSTRAINT [DF_Missiles_MaxLock] DEFAULT (1) FOR [MaxLock],
	CONSTRAINT [DF_Missiles_ChaffResistance] DEFAULT (1) FOR [ChaffResistance],
	CONSTRAINT [DF_Missiles_LaunchSound] DEFAULT (500) FOR [LaunchSound],
	CONSTRAINT [DF_Missiles_FlightSound] DEFAULT (540) FOR [FlightSound],
	CONSTRAINT [PK_Missiles] PRIMARY KEY  NONCLUSTERED 
	(
		[ExpendableID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Parts] WITH NOCHECK ADD 
	CONSTRAINT [DF_Parts_dSignatureMounted] DEFAULT (0) FOR [dSignatureMounted],
	CONSTRAINT [DF_Parts_dSignatureActive] DEFAULT (0) FOR [dSignatureActive],
	CONSTRAINT [DF_Parts_TechBitsRequired] DEFAULT (0) FOR [TechBitsRequired],
	CONSTRAINT [DF_Parts_TechBitsEffect] DEFAULT (0) FOR [TechBitsEffect],
	CONSTRAINT [DF_Parts_ChangeTreasure] DEFAULT (5) FOR [ChanceTreasure],
	CONSTRAINT [DF_Parts_PartSize] DEFAULT (0) FOR [PartMask],
	CONSTRAINT [DF_Parts_InventoryLineMDL] DEFAULT ('invdefault') FOR [InventoryLineMDL],
	CONSTRAINT [DF_Parts_SecondsToBuild] DEFAULT (0) FOR [SecondsToBuild],
	CONSTRAINT [DF_Parts_GroupID] DEFAULT (0) FOR [GroupID],
	CONSTRAINT [PK_Parts] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Parts] CHECK ([Price] >= 0 and [Mass] > 0)
GO

ALTER TABLE [dbo].[Probes] WITH NOCHECK ADD 
	CONSTRAINT [DF_Probes_dtimeBurst] DEFAULT (200) FOR [dtimeBurst],
	CONSTRAINT [DF_Probes_accuracy] DEFAULT (50) FOR [accuracy],
	CONSTRAINT [DF_Probes_dispersion] DEFAULT (0.01) FOR [dispersion],
	CONSTRAINT [DF_Probes_AmbientSound] DEFAULT (720) FOR [AmbientSound],
	CONSTRAINT [DF_Probes_Ammo] DEFAULT (0) FOR [Ammo],
	CONSTRAINT [DF_Probes_dtRipcord] DEFAULT (255) FOR [dtRipcord],
	CONSTRAINT [PK_Probes] PRIMARY KEY  NONCLUSTERED 
	(
		[ExpendableID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[SectorInfo] WITH NOCHECK ADD 
	CONSTRAINT [DF_SectorInfo_LightX] DEFAULT (1) FOR [LightX],
	CONSTRAINT [DF_SectorInfo_LightY] DEFAULT (1) FOR [LightY],
	CONSTRAINT [DF_SectorInfo_LightZ] DEFAULT (1) FOR [LightZ],
	CONSTRAINT [DF_SectorInfo_LightColor] DEFAULT (10000000) FOR [LightColor],
	CONSTRAINT [DF_SectorInfo_StarsCount] DEFAULT (500) FOR [StarsCount],
	CONSTRAINT [DF_SectorInfo_DebrisCount] DEFAULT (500) FOR [DebrisCount],
	CONSTRAINT [DF_SectorInfo_starSeed] DEFAULT (1) FOR [starSeed],
	CONSTRAINT [PK_SectorInfo] PRIMARY KEY  NONCLUSTERED 
	(
		[MapID],
		[SectorID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Afterburners] WITH NOCHECK ADD 
	CONSTRAINT [DF_Afterburners_InteriorSound] DEFAULT (680) FOR [InteriorSound],
	CONSTRAINT [DF_Afterburners_ExteriorSound] DEFAULT (700) FOR [ExteriorSound],
	CONSTRAINT [PK_Afterburners] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[AlephInstances] WITH NOCHECK ADD 
	CONSTRAINT [DF_AlephInstances_RotationR] DEFAULT (1) FOR [RotationR],
	CONSTRAINT [DF_AlephInstances_Radius] DEFAULT (90) FOR [Radius],
	CONSTRAINT [DF_AlephInstances_Signature] DEFAULT (1.0) FOR [Signature],
	CONSTRAINT [DF_AlephInstances_Texture] DEFAULT ('plnt42') FOR [Texture],
	CONSTRAINT [DF_AlephInstances_PRIcon] DEFAULT ('aleph') FOR [PRIcon],
	CONSTRAINT [PK_AlephInstances] PRIMARY KEY  NONCLUSTERED 
	(
		[AlephID],
		[MapID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Ammo] WITH NOCHECK ADD 
	CONSTRAINT [DF_Ammo_AmmoType] DEFAULT (0) FOR [AmmoType],
	CONSTRAINT [PK_Ammo] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Asteroids] WITH NOCHECK ADD 
	CONSTRAINT [DF_Asteroids_FileModel] DEFAULT ('bgrnd03') FOR [FileModel],
	CONSTRAINT [DF_Asteroids_FileIcon] DEFAULT ('meteoricon') FOR [FileIcon],
	CONSTRAINT [PK_Asteroids] PRIMARY KEY  NONCLUSTERED 
	(
		[MapID],
		[AsteroidID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Civs] WITH NOCHECK ADD 
	CONSTRAINT [DF_Civs_TechBitsCiv] DEFAULT (0) FOR [TechBitsCiv],
	CONSTRAINT [DF_Civs_StationHUD] DEFAULT ('dialog') FOR [StationHUD],
	CONSTRAINT [DF_Civs_TechBitsCivNoDev] DEFAULT (0) FOR [TechBitsCivNoDev],
	CONSTRAINT [DF_Civs_BonusMoney] DEFAULT (0.0) FOR [BonusMoney],
	CONSTRAINT [DF_Civs_IncomeMoney] DEFAULT (0.0) FOR [IncomeMoney],
	CONSTRAINT [PK_Civs] PRIMARY KEY  NONCLUSTERED 
	(
		[CivID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Cloaks] WITH NOCHECK ADD 
	CONSTRAINT [DF_Cloaks_EngageSound] DEFAULT (640) FOR [EngageSound],
	CONSTRAINT [DF_Cloaks_DisengageSound] DEFAULT (660) FOR [DisengageSound],
	CONSTRAINT [PK_Cloaks] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Magazines] WITH NOCHECK ADD 
	CONSTRAINT [PK_Magazines] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[PosterInstances] WITH NOCHECK ADD 
	CONSTRAINT [PK_PosterInstances] PRIMARY KEY  NONCLUSTERED 
	(
		[PosterID],
		[MapID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Shields] WITH NOCHECK ADD 
	CONSTRAINT [DF_Shields_ActivateSound] DEFAULT (580) FOR [ActivateSound],
	CONSTRAINT [DF_Shields_DeactivateSound] DEFAULT (620) FOR [DeactivateSound],
	CONSTRAINT [PK_Shields] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[ShipTypes] WITH NOCHECK ADD 
	CONSTRAINT [DF_ShipTypes_MaxRollRate] DEFAULT (50) FOR [MaxRollRate],
	CONSTRAINT [DF_ShipTypes_MaxPitchRate] DEFAULT (50) FOR [MaxPitchRate],
	CONSTRAINT [DF_ShipTypes_MaxYawRate] DEFAULT (50) FOR [MaxYawRate],
	CONSTRAINT [DF_ShipTypes_DriftRoll] DEFAULT (50) FOR [DriftRoll],
	CONSTRAINT [DF_ShipTypes_DriftPitch] DEFAULT (50) FOR [DriftPitch],
	CONSTRAINT [DF_ShipTypes_DriftYaw] DEFAULT (50) FOR [DriftYaw],
	CONSTRAINT [DF_ShipTypes_AccelSideMultiplier] DEFAULT (1) FOR [AccelSideMultiplier],
	CONSTRAINT [DF_ShipTypes_AccelBackMultiplier] DEFAULT (1) FOR [AccelBackMultiplier],
	CONSTRAINT [DF_ShipTypes_EnergyMax] DEFAULT (10) FOR [EnergyMax],
	CONSTRAINT [DF_ShipTypes_RateRecharg] DEFAULT (1) FOR [RateRechargeEnergy],
	CONSTRAINT [DF_ShipTypes_TechBitsRequired] DEFAULT (0) FOR [TechBitsRequired],
	CONSTRAINT [DF_ShipTypes_TechBitsEffect] DEFAULT (0) FOR [TechBitsEffect],
	CONSTRAINT [DF_ShipTypes_ShieldPartSize] DEFAULT (0) FOR [ShieldPartMask],
	CONSTRAINT [DF_ShipTypes_AfterburnerPartSize] DEFAULT ((-1)) FOR [AfterburnerPartMask],
	CONSTRAINT [DF_ShipTypes_CloakPartSize] DEFAULT (0) FOR [CloakPartMask],
	CONSTRAINT [DF_ShipTypes_MagazinePartSize] DEFAULT (0) FOR [MagazinePartMask],
	CONSTRAINT [DF_ShipTypes_DispenserPartSize] DEFAULT (0) FOR [DispenserPartMask],
	CONSTRAINT [DF_ShipTypes_ChaffPartSize] DEFAULT (0) FOR [ChaffPartMask],
	CONSTRAINT [DF_ShipTypes_MagazineCapacity] DEFAULT (0) FOR [MagazineCapacity],
	CONSTRAINT [DF_ShipTypes_DispenserCapacity] DEFAULT (0) FOR [DispenserCapacity],
	CONSTRAINT [DF_ShipTypes_ChaffCapacity] DEFAULT (0) FOR [ChaffCapacity],
	CONSTRAINT [DF_ShipTypes_PilotHUD] DEFAULT ('dialog') FOR [PilotHUD],
	CONSTRAINT [DF_ShipTypes_ObserverHUD] DEFAULT ('dialog') FOR [ObserverHUD],
	CONSTRAINT [DF_ShipTypes_InteriorSound] DEFAULT (140) FOR [InteriorSound],
	CONSTRAINT [DF_ShipTypes_ExteriorSound] DEFAULT (180) FOR [ExteriorSound],
	CONSTRAINT [DF_ShipTypes_ThrustInteriorSound] DEFAULT (220) FOR [ThrustInteriorSound],
	CONSTRAINT [DF_ShipTypes_ThrustExteriorSound] DEFAULT (260) FOR [ThrustExteriorSound],
	CONSTRAINT [DF_ShipTypes_TurnInteriorSound] DEFAULT (300) FOR [TurnInteriorSound],
	CONSTRAINT [DF_ShipTypes_TurnExteriorSound] DEFAULT (340) FOR [TurnExteriorSound],
	CONSTRAINT [DF_ShipTypes_SecondsToBuild] DEFAULT (0) FOR [SecondsToBuild],
	CONSTRAINT [DF_ShipTypes_GroupID] DEFAULT (0) FOR [GroupID],
	CONSTRAINT [PK_ShipStats] PRIMARY KEY  NONCLUSTERED 
	(
		[ShipTypeID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_ShipTypes] CHECK ([Length] > 0 and [Weight] > 0 and [Price] >= 0 and [BaseMaxSpeed] > 0 and [BaseHitPoints] > 0 and [BaseScannerRange] > 0 and [MaxRollRate] >= 0 and [MaxPitchRate] >= 0 and [DriftRoll] > 0 and [DriftPitch] > 0 and [DriftYaw] > 0 and [Acceleration] > 0 and [BaseSignature] >= 10 and [EnergyMax] >= 0 and [RateRechargeEnergy] >= 0 and [MaxAmmo] >= 0 and [MaxFuel] >= 0)
GO

ALTER TABLE [dbo].[StationInstances] WITH NOCHECK ADD 
	CONSTRAINT [DF_StationInstances_RotationAx] DEFAULT (0) FOR [RotationAx],
	CONSTRAINT [DF_StationInstances_RotationAy] DEFAULT (1) FOR [RotationAy],
	CONSTRAINT [DF_StationInstances_RotationAz] DEFAULT (0) FOR [RotationAz],
	CONSTRAINT [DF_StationInstances_RotationR] DEFAULT (0) FOR [RotationR],
	CONSTRAINT [PK_StationInstances] PRIMARY KEY  NONCLUSTERED 
	(
		[MapID],
		[StationID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[TreasureChances] WITH NOCHECK ADD 
	CONSTRAINT [DF_TreasureChances_Chance] DEFAULT (0) FOR [Chance],
	CONSTRAINT [DF_TreasureChances_TreasureCode] DEFAULT (1) FOR [TreasureCode],
	CONSTRAINT [PK_TreasureChances] PRIMARY KEY  NONCLUSTERED 
	(
		[TreasureSetID],
		[ObjectID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[Weapons] WITH NOCHECK ADD 
	CONSTRAINT [DF_WeaponTypes_RateOfFire] DEFAULT (0) FOR [dTimeBurstShots],
	CONSTRAINT [DF_Weapons_TimeBetween] DEFAULT (0) FOR [EnergyPerShot],
	CONSTRAINT [DF_WeaponTypes_ProjectileID1] DEFAULT (0) FOR [ProjectileID1],
	CONSTRAINT [DF_Weapons_ActivateSound] DEFAULT (80) FOR [ActivateSound],
	CONSTRAINT [DF_Weapons_BurstShotSound] DEFAULT ((-1)) FOR [BurstShotSound],
	CONSTRAINT [DF_Weapons_Dispersion] DEFAULT (0.005) FOR [Dispersion],
	CONSTRAINT [PK_WeaponTypes] PRIMARY KEY  NONCLUSTERED 
	(
		[PartID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_Weapons] CHECK ([dTimeBurstShots] > 0 and [dTimeReady] > 0 and [EnergyPerShot] >= 0 and [cBulletsPerShot] >= 0 and [Dispersion] >= 0 and [Dispersion] <= 0.15)
GO

ALTER TABLE [dbo].[AttachPoints] WITH NOCHECK ADD 
	CONSTRAINT [DF_AttachPoints_FieldOfFire] DEFAULT (120) FOR [FieldOfFire],
	CONSTRAINT [DF_AttachPoints_PartSize] DEFAULT (0) FOR [PartMask],
	CONSTRAINT [DF_AttachPoints_GunnerHUD] DEFAULT ('dialog') FOR [GunnerHUD],
	CONSTRAINT [DF_AttachPoints_InteriorSound] DEFAULT (140) FOR [InteriorSound],
	CONSTRAINT [DF_AttachPoints_TurnSound] DEFAULT (121) FOR [TurnSound],
	CONSTRAINT [DF_AttachPoints_LocationAbreviation] DEFAULT (' ') FOR [LocationAbreviation],
	CONSTRAINT [PK_AttachPoints] PRIMARY KEY  NONCLUSTERED 
	(
		[ShipTypeID],
		[AttachPointID]
	)  ON [PRIMARY] 
GO

ALTER TABLE [dbo].[CharStats] WITH NOCHECK ADD 
	CONSTRAINT [DF_CharStats_Rank] DEFAULT (1) FOR [Rank],
	CONSTRAINT [DF_CharStats_Overall Rating] DEFAULT (0) FOR [Score],
	CONSTRAINT [DF_CharStats_MinutesPlayed] DEFAULT (0) FOR [MinutesPlayed],
	CONSTRAINT [DF_CharStats_BaseKills] DEFAULT (0) FOR [BaseKills],
	CONSTRAINT [DF_CharStats_BaseCaptures] DEFAULT (0) FOR [BaseCaptures],
	CONSTRAINT [DF_CharStats_Kills] DEFAULT (0) FOR [Kills],
	CONSTRAINT [DF_CharStats_Combat Rating] DEFAULT (1000) FOR [Rating],
	CONSTRAINT [DF_CharStats_GamesPlayed] DEFAULT (0) FOR [GamesPlayed],
	CONSTRAINT [DF_CharStats_Deaths] DEFAULT (0) FOR [Deaths],
	CONSTRAINT [DF_CharStats_Wins] DEFAULT (0) FOR [Wins],
	CONSTRAINT [DF_CharStats_Losses] DEFAULT (0) FOR [Losses],
	CONSTRAINT [DF_CharStats_WinsCmd] DEFAULT (0) FOR [WinsCmd],
	CONSTRAINT [DF_CharStats_LossesCmd] DEFAULT (0) FOR [LossesCmd],
	CONSTRAINT [IX_CharStats] UNIQUE  NONCLUSTERED 
	(
		[CharacterID],
		[CivID]
	)  ON [PRIMARY] ,
	CONSTRAINT [CK_CharStats] CHECK ([CivID] > 0)
GO

 CREATE  INDEX [IX_AccessCodes_ReassignedFrom] ON [dbo].[AccessCodes]([CodeReassignedFrom]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_AccessCodeUsage_ZoneName] ON [dbo].[AccessCodeUsage]([ZoneName]) ON [PRIMARY]
GO

 CREATE  UNIQUE  INDEX [IX_CharacterName] ON [dbo].[CharacterInfo]([CharacterName]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_GameResults_GameID] ON [dbo].[GameResults]([GameID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_PlayerResults_GameID] ON [dbo].[PlayerResults]([GameID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_PlayerResults_Name] ON [dbo].[PlayerResults]([Name]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_TeamResults_GameID] ON [dbo].[TeamResults]([GameID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_GameResults_EndTime] ON [dbo].[GameResults]([EndTime]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_PSSInstances_AccessCode] ON [dbo].[PSSInstances]([AccessCode]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_PSSInstances_TrackingID] ON [dbo].[PSSInstances]([TrackingID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Ranks_Requirement] ON [dbo].[Ranks]([CivID], [Requirement]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Squads_Score] ON [dbo].[Squads]([Score]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Squads_Wins] ON [dbo].[Squads]([Wins]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Squads_Losses] ON [dbo].[Squads]([Losses]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_AlephInstances_SectorID] ON [dbo].[AlephInstances]([SectorID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_Asteroids] ON [dbo].[Asteroids]([SectorID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_StationInstances] ON [dbo].[StationInstances]([SectorID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_TreasureChance_PartID] ON [dbo].[TreasureChances]([ObjectID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_TreasureChance_SetID] ON [dbo].[TreasureChances]([TreasureSetID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_GamesPlayed] ON [dbo].[CharStats]([CivID], [GamesPlayed], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Deaths] ON [dbo].[CharStats]([CivID], [Deaths], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Wins] ON [dbo].[CharStats]([CivID], [Wins], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Losses] ON [dbo].[CharStats]([CivID], [Losses], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_WinsCmd] ON [dbo].[CharStats]([CivID], [WinsCmd], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_LossesCmd] ON [dbo].[CharStats]([CivID], [LossesCmd], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Score] ON [dbo].[CharStats]([CivID], [Score], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_MinutesPlayed] ON [dbo].[CharStats]([CivID], [MinutesPlayed], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_BaseKills] ON [dbo].[CharStats]([CivID], [BaseKills], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_BaseCaptures] ON [dbo].[CharStats]([CivID], [BaseCaptures], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Kills] ON [dbo].[CharStats]([CivID], [Kills], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Rating] ON [dbo].[CharStats]([CivID], [Rating], [CharacterID]) ON [PRIMARY]
GO

 CREATE  INDEX [IX_CharStats_Name] ON [dbo].[CharStats]([CivID], [CharacterName], [CharacterID]) ON [PRIMARY]
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Expendables].[percentAlpha]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Expendables].[percentBlue]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Expendables].[percentGreen]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Expendables].[percentRed]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Projectiles].[percentAlpha]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Projectiles].[percentBlue]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Projectiles].[percentGreen]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Projectiles].[percentPenetration]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Projectiles].[percentRed]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Mines].[percentAlphaMunition]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Mines].[percentBlueMunition]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Mines].[percentGreenMunition]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Mines].[percentRedMunition]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Parts].[ChanceTreasure]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[Probes].[accuracy]'
GO

setuser
GO

setuser N'dbo'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[SectorInfo].[PlanetLongitude]'
GO

EXEC sp_bindrule N'[dbo].[PercentByte]', N'[SectorInfo].[PlanetSinLatitude]'
GO

setuser
GO

ALTER TABLE [dbo].[StationTypes] ADD 
	CONSTRAINT [FK_StationTypes_Drones] FOREIGN KEY 
	(
		[ConstructionDroneTypeID]
	) REFERENCES [dbo].[Drones] (
		[DroneID]
	),
	CONSTRAINT [FK_StationTypes_StationTypes] FOREIGN KEY 
	(
		[UpgradeStationTypeID]
	) REFERENCES [dbo].[StationTypes] (
		[StationTypeID]
	)
GO

ALTER TABLE [dbo].[Developments] ADD 
	CONSTRAINT [FK_Developments_GlobalAttributes] FOREIGN KEY 
	(
		[GlobalAttributeID]
	) REFERENCES [dbo].[GlobalAttributes] (
		[GlobalAttributeID]
	)
GO

ALTER TABLE [dbo].[Mines] ADD 
	CONSTRAINT [FK_Mines_Expendable] FOREIGN KEY 
	(
		[ExpendableID]
	) REFERENCES [dbo].[Expendables] (
		[ExpendableID]
	)
GO

ALTER TABLE [dbo].[Missiles] ADD 
	CONSTRAINT [FK_Missiles_Expendable] FOREIGN KEY 
	(
		[ExpendableID]
	) REFERENCES [dbo].[Expendables] (
		[ExpendableID]
	)
GO

ALTER TABLE [dbo].[Parts] ADD 
	CONSTRAINT [FK_Parts_Parts] FOREIGN KEY 
	(
		[SuccessorPartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Parts_PartTypes] FOREIGN KEY 
	(
		[PartTypeID]
	) REFERENCES [dbo].[PartTypes] (
		[PartTypeID]
	)
GO

ALTER TABLE [dbo].[Probes] ADD 
	CONSTRAINT [FK_Probes_Expendable] FOREIGN KEY 
	(
		[ExpendableID]
	) REFERENCES [dbo].[Expendables] (
		[ExpendableID]
	),
	CONSTRAINT [FK_Probes_Projectiles] FOREIGN KEY 
	(
		[ProjectileID]
	) REFERENCES [dbo].[Projectiles] (
		[ProjectileID]
	)
GO

ALTER TABLE [dbo].[SectorInfo] ADD 
	CONSTRAINT [FK_SectorInfo_Maps] FOREIGN KEY 
	(
		[MapID]
	) REFERENCES [dbo].[Maps] (
		[MapID]
	)
GO

ALTER TABLE [dbo].[Afterburners] ADD 
	CONSTRAINT [FK_Afterburners_Parts] FOREIGN KEY 
	(
		[PartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	)
GO

ALTER TABLE [dbo].[AlephInstances] ADD 
	CONSTRAINT [FK_AlephInstances_SectorInfo] FOREIGN KEY 
	(
		[MapID],
		[SectorID]
	) REFERENCES [dbo].[SectorInfo] (
		[MapID],
		[SectorID]
	)
GO

ALTER TABLE [dbo].[Ammo] ADD 
	CONSTRAINT [FK_Ammo_Parts] FOREIGN KEY 
	(
		[PartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	)
GO

ALTER TABLE [dbo].[Asteroids] ADD 
	CONSTRAINT [FK_Asteroids_SectorInfo] FOREIGN KEY 
	(
		[MapID],
		[SectorID]
	) REFERENCES [dbo].[SectorInfo] (
		[MapID],
		[SectorID]
	)
GO

ALTER TABLE [dbo].[Civs] ADD 
	CONSTRAINT [FK_Civs_Parts] FOREIGN KEY 
	(
		[PreferredPartID1]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts1] FOREIGN KEY 
	(
		[PreferredPartID2]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts10] FOREIGN KEY 
	(
		[PreferredPartID11]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts11] FOREIGN KEY 
	(
		[PreferredPartID12]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts12] FOREIGN KEY 
	(
		[PreferredPartID13]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts13] FOREIGN KEY 
	(
		[PreferredPartID14]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts14] FOREIGN KEY 
	(
		[PreferredPartID15]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts2] FOREIGN KEY 
	(
		[PreferredPartID3]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts3] FOREIGN KEY 
	(
		[PreferredPartID4]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts4] FOREIGN KEY 
	(
		[PreferredPartID5]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts5] FOREIGN KEY 
	(
		[PreferredPartID6]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts6] FOREIGN KEY 
	(
		[PreferredPartID7]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts7] FOREIGN KEY 
	(
		[PreferredPartID8]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts8] FOREIGN KEY 
	(
		[PreferredPartID9]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Civs_Parts9] FOREIGN KEY 
	(
		[PreferredPartID10]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	)
GO

ALTER TABLE [dbo].[Cloaks] ADD 
	CONSTRAINT [FK_Cloaks_Parts] FOREIGN KEY 
	(
		[PartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	)
GO

ALTER TABLE [dbo].[Magazines] ADD 
	CONSTRAINT [FK_Magazines_Expendable] FOREIGN KEY 
	(
		[ExpendableID]
	) REFERENCES [dbo].[Expendables] (
		[ExpendableID]
	),
	CONSTRAINT [FK_Magazines_Magazines] FOREIGN KEY 
	(
		[SuccessorPartID]
	) REFERENCES [dbo].[Magazines] (
		[PartID]
	),
	CONSTRAINT [FK_Magazines_Parts] FOREIGN KEY 
	(
		[PartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	)
GO

ALTER TABLE [dbo].[PosterInstances] ADD 
	CONSTRAINT [FK_PosterInstances_SectorInfo] FOREIGN KEY 
	(
		[MapID],
		[SectorID]
	) REFERENCES [dbo].[SectorInfo] (
		[MapID],
		[SectorID]
	)
GO

ALTER TABLE [dbo].[Shields] ADD 
	CONSTRAINT [FK_Shields_Parts] FOREIGN KEY 
	(
		[PartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	)
GO

ALTER TABLE [dbo].[ShipTypes] ADD 
	CONSTRAINT [FK_ShipTypes_Parts] FOREIGN KEY 
	(
		[PreferredPartID1]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts1] FOREIGN KEY 
	(
		[PreferredPartID2]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts10] FOREIGN KEY 
	(
		[PreferredPartID11]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts11] FOREIGN KEY 
	(
		[PreferredPartID12]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts12] FOREIGN KEY 
	(
		[PreferredPartID13]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts13] FOREIGN KEY 
	(
		[PreferredPartID14]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts2] FOREIGN KEY 
	(
		[PreferredPartID3]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts3] FOREIGN KEY 
	(
		[PreferredPartID4]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts4] FOREIGN KEY 
	(
		[PreferredPartID5]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts5] FOREIGN KEY 
	(
		[PreferredPartID6]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts6] FOREIGN KEY 
	(
		[PreferredPartID7]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts7] FOREIGN KEY 
	(
		[PreferredPartID8]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts8] FOREIGN KEY 
	(
		[PreferredPartID9]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_Parts9] FOREIGN KEY 
	(
		[PreferredPartID10]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_ShipTypes_ShipTypes] FOREIGN KEY 
	(
		[SuccessorHullID]
	) REFERENCES [dbo].[ShipTypes] (
		[ShipTypeID]
	)
GO

ALTER TABLE [dbo].[StationInstances] ADD 
	CONSTRAINT [FK_StationInstances_SectorInfo] FOREIGN KEY 
	(
		[MapID],
		[SectorID]
	) REFERENCES [dbo].[SectorInfo] (
		[MapID],
		[SectorID]
	)
GO

ALTER TABLE [dbo].[TreasureChances] ADD 
	CONSTRAINT [FK_TreasureChance_Parts] FOREIGN KEY 
	(
		[ObjectID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_TreasureChance_TreasureSets] FOREIGN KEY 
	(
		[TreasureSetID]
	) REFERENCES [dbo].[TreasureSets] (
		[TreasureSetID]
	)
GO

ALTER TABLE [dbo].[Weapons] ADD 
	CONSTRAINT [FK_Weapons_Parts] FOREIGN KEY 
	(
		[PartID]
	) REFERENCES [dbo].[Parts] (
		[PartID]
	),
	CONSTRAINT [FK_Weapons_Projectiles] FOREIGN KEY 
	(
		[ProjectileID1]
	) REFERENCES [dbo].[Projectiles] (
		[ProjectileID]
	)
GO

ALTER TABLE [dbo].[AttachPoints] ADD 
	CONSTRAINT [FK_AttachPoints_ShipTypes] FOREIGN KEY 
	(
		[ShipTypeID]
	) REFERENCES [dbo].[ShipTypes] (
		[ShipTypeID]
	)
GO

ALTER TABLE [dbo].[CharMedals] ADD 
	CONSTRAINT [FK_CharMedals_CharacterInfo] FOREIGN KEY 
	(
		[CharacterID]
	) REFERENCES [dbo].[CharacterInfo] (
		[CharacterID]
	),
	CONSTRAINT [FK_CharMedals_Civs] FOREIGN KEY 
	(
		[CivID]
	) REFERENCES [dbo].[Civs] (
		[CivID]
	),
	CONSTRAINT [FK_CharMedals_Medals] FOREIGN KEY 
	(
		[MedalID]
	) REFERENCES [dbo].[Medals] (
		[MedalID]
	)
GO

ALTER TABLE [dbo].[CharStats] ADD 
	CONSTRAINT [FK_CharStats_CharacterInfo] FOREIGN KEY 
	(
		[CharacterID]
	) REFERENCES [dbo].[CharacterInfo] (
		[CharacterID]
	),
	CONSTRAINT [FK_CharStats_Civs] FOREIGN KEY 
	(
		[CivID]
	) REFERENCES [dbo].[Civs] (
		[CivID]
	)
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO











CREATE Procedure GetCharSquads
(
	@characterID int
)
As
set nocount on 

create table #playerTeams
(
	vc_team_name varchar(30),
	i_status int,
	i_team_id int,
	i_secondary_status int
)	

insert into #playerTeams
	exec profile.profile.dbo.p_get_player_teams_aleg @characterID

select rtrim(s.SquadName), i_status, i_team_id, i_secondary_status 
	from #playerTeams p, Squads s where s.squadID = p.i_team_id



















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure IsTechBitsSuperset
(
    @tb1 TechBits, /* Is THIS techbits fully superceded by ...*/
    @tb2 TechBits, /* ...THIS techbits? */
    @ok  bit Output
)
As
set nocount on

select @ok = 1 /* ok until we find a char that's not */

return 























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO










CREATE Procedure SquadAcceptJoinRequest

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int

exec profile.profile.dbo.p_grant_petitions_aleg @SquadID, @MemberID, @ErrorCode output, @ErrorMsg output

if @ErrorCode is null select 0
else select @ErrorCode

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadDemoteToMember

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int

exec profile.profile.dbo.p_edit_team_member_status2_aleg @SquadID, @MemberID, 0, @ErrorCode output, @ErrorMsg output

select @ErrorCode

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO

CREATE Procedure SquadDetailsPlayers

	(
		@SquadID int
	)

As

set nocount on

CREATE TABLE #members (
	[i_account_id] [int] NOT NULL ,
	[vc_member_id] [varchar] (30) NOT NULL ,
	[i_status] [int] NOT NULL ,
	[i_secondary_status] [int] NULL ,
	[dt_granted] [datetime] NOT NULL 
)

insert into #members
	exec profile.profile.dbo.p_get_team_members_aleg @SquadID

select m.i_account_id, m.vc_member_id, m.i_status, m.i_secondary_status, m.dt_granted, c.rank
	from #members m, charstats c 
	where c.characterid=m.i_account_id and c.civid is null

drop table #members
return 

GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadGetDudeXSquads
(
	@characterID int
)
As
set nocount on

create table #playerTeamsX
(
	vc_team_name varchar(30),
	i_status int,
	i_team_id int,
	i_secondary_status int
)

insert #playerTeamsX
	exec profile.profile.dbo.p_get_player_teams_aleg @characterID

select s.* from Squads s, #playerTeamsX p where s.squadID=p.i_team_ID

return 




















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure SquadMakeJoinRequest

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int

exec profile.profile.dbo.p_petition_team_aleg @SquadID, @MemberID, @ErrorCode output, @ErrorMsg output

if @ErrorCode is null select 0
else select @ErrorCode

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadPromoteToASL

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int

exec profile.profile.dbo.p_edit_team_member_status2_aleg @SquadID, @MemberID, 1, @ErrorCode output, @ErrorMsg output

select @ErrorCode

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadRejectJoinRequest

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int


exec profile.profile.dbo.p_deny_petitions_aleg @SquadID, @MemberID, @ErrorCode output, @ErrorMsg output

if @ErrorCode is null select 0
else select @ErrorCode

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO






















Create Procedure CreateLotsOfRandomSquads
	(
		@startingnumber int,
		@numsquads int
	)
As
set nocount on

declare @i int

select @i=0
while (@i < @numsquads)
begin
	declare @squadname char(12)
	declare @squadid int
	declare @score smallint
	declare @wins smallint
	declare @losses smallint
	select @wins=convert(smallint, 1000 * rand())
	select @losses=convert(smallint, 1000 * rand())
	select @score=1000 * @wins / (@wins + @losses)
	
	select @squadname='Random' + convert(char(6), @i + @startingnumber)
	insert into squads (SquadID, SquadName, Score, Wins, Losses) 
		values (@i, @squadname, @score, @wins, @losses)
	
	select @i=@i+1
end

return 























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Procedure CreatePigsMachine (@szComputerName Name)
As
declare @szTemp Name
declare @szNumber char(10)
declare @i int
select @i = 0
while @i < 30
BEGIN
    select @szNumber = convert(varchar, @i)
    if (@i < 10)
    	select @szNumber = '0' + convert(varchar, @szNumber)
    if (@i < 100)
    	select @szNumber = '0' + convert(varchar, @szNumber)
    select @szTemp = (rtrim(convert(varchar, @szComputerName)) + '-' + convert(varchar, @szNumber))
    insert into characterinfo (CharacterName) values(@szTemp)
    select @i = @i + 1
END























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO











CREATE Procedure GetNewRank
(
	@CivID smallint,
	@Score int,
	@Rank smallint output /* IN: old rank, OUT: new rank */
)
As
set nocount on
declare @requiredScore int

/* Advance rank if 
	a) there is another rank to advance to, and 
	b) reached the requirement for it */
select @requiredScore=Requirement from Ranks 
	where (civid=@civid or (@civID is null and civid is null)) and Rank=@Rank+1

if (@requiredScore is not null) and @Score>=@requiredScore
	select @Rank=@Rank+1
	
return 























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Procedure IsValidTechBits
(
    @TechBitsStr char(800),
    @fok         bit OUTPUT
)
As
/* Determine whether every techbit specified is a valid techbit */
/* Assumption: The list of tech bits are space delimited, with only one space between each tech bit */
set nocount on

select @fok = 0 /* assume failure until all tech bits have been verified */

declare @itb int
declare @lenBits int
declare @ispace int
declare @tech char(40) /* sync up with size of short name in TechBits */

Select @itb=1
Select @lenbits=LEN(@TechBitsStr) + 1 /* zero-based */

while (@itb < @lenbits)
begin
    select @ispace=charindex(' ', @TechBitsStr, @itb) /* find the next space */
    if (@ispace=0) /* no space found, last token */
    	select @ispace=@lenbits+1
    select @tech=SubString(@TechBitsStr, @itb, @ispace-@itb)
    if (Exists(Select * From TechBits Where BitShortName=@tech))
        select @itb=@ispace+1
    else
        return /* failure */
end
Select @fok=1

return 























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO
















CREATE Procedure SquadAddNewLeaderToLog

	(
		@squadid int,
		@leadername name
	)

As
set nocount on

update squads 
  set ownershiplog=convert(char(8), GetDate(), 1) + ';' + rtrim(@leadername) + ';' + cast(ownershiplog as varchar(465)) where squadid = @squadid
	
return 















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO



















CREATE Procedure SquadDetails

	(
		@SquadID int
	)

As

set nocount on

-- todo include rank and civid

create table #temp
(
	vc_team_id			 varchar(30),
	vc_member_id		 varchar(30),
	vc_description		 varchar(510),
	vc_favorite_game1	 varchar(4), 
	vc_favorite_game2	 varchar(4),  
	vc_favorite_game3	 varchar(4),  
	vc_favorite_game4	 varchar(4),  
	vc_favorite_game5	 varchar(4),  
	vc_favorite_link	 varchar(255),
	b_closed			 bit,
	b_award				 bit,
	i_team_id			 int,  
	vc_edit_restrictions varchar(8),
	dt_created		     datetime
)

insert #temp
  exec profile.profile.dbo.p_get_team_profile_aleg @SquadID

select #temp.*, s.civid/*, s.rank*/ from #temp, Squads s where s.SquadID = @SquadID 

declare @c_rows as int
select @c_rows = count(*) from #temp
if @c_rows = 0 delete from squads where squadid = @squadID

return 




















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadEdit


	(
		@SquadID			int,
		@Description		varchar(510),
		@URL				varchar(255),
		@civID				smallint
	)

As

set nocount on

select @Description = rtrim(@Description)
select @URL = rtrim(@URL) 

declare	@ErrorCode	int		 	 
declare	@ErrorMsg	varchar(128)

exec profile.profile.dbo.p_edit_team_profile_aleg @SquadID, @Description, @URL, @ErrorCode output, @ErrorMsg output

	if @ErrorCode = 0
	begin
		-- now update federation database
		update squads
			set civid = @civid 
			where squadid = @squadid
	end

select @ErrorCode, @ErrorMsg





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















Create Procedure SquadGetNearLosses
(
	@squadID int,   -- If they specify squadID=-1, then they want the top
	@squadname char(30) = '' -- (only) if they specify a name, we need to look up the squadID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Losses real
declare @firstordinal int

-- If they specify a name, then we need to look up the squadID
if @squadname<>''
begin
	select @squadid=NULL
	select @squadid=squadid from squads where squadname=@squadname
	if @squadid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select * 
			from squads
			where (0=1)
		return
	end
end

-- If they specify squadID=-1, then they want the top
if @squadID=-1
begin
	select @firstordinal=1
	select top 35 @firstordinal as 'FirstOrdinal', *
		from squads
		order by Losses desc, squadid desc
end
else
begin
	select @Losses=Losses from squads 
		where squadID=@squadid 

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from squads 
		where (Losses>@Losses or (Losses=@Losses and squadid>=@squadID or @Losses is null)) 

	select top 35 * into #higher 
		from squads
		where (Losses>@Losses or (Losses=@Losses and squadid>=@squadID) or @Losses is null) 
		order by Losses, squadid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 34 * into #lower 
		from squads
		where (Losses<@Losses or (Losses=@Losses and squadid<@squadID)) 
		order by Losses desc, squadid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Losses desc, squadid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadGetNearNames
(
	@SquadID int,   -- If they specify squadID=-1, then they want the top
	@SquadName Name = '' -- (only) if they specify a name, we need to look up the squadID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the SquadGetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @firstordinal int

-- If they specify a name, then we need to look up the squadID
if @SquadName<>''
begin
	select @squadid=NULL
	select top 1 @squadid=squadid from squads
		where squadname >= @SquadName order by Squadname
	if @SquadId is null
	begin
		-- Make sure we still create a result set of the expectant type
		select * 
			from squads s
			where (0=1)
		return
	end
end

-- If they specify SquadID=-1, then they want the top
if @SquadID=-1
begin
	select @firstordinal=1
	select top 35 @firstordinal as 'FirstOrdinal', *
		from squads
		order by SquadName
end
else
begin
	if @SquadName = ''
		select @SquadName=SquadName from squads where SquadID=@Squadid

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=1
	--	don't need to spend this cost, since ordinals for names don't mean much, but Currie wants it :-)
	select @firstordinal=count(*) from Squads
		where (SquadName<=@SquadName) 

	select top 35 * into #higher
		from squads
		where SquadName<=@SquadName
		order by SquadName desc

	select @firstordinal=@firstordinal - count(*) + 1 from #higher
	
	select top 34 * into #lower 
		from squads
		where SquadName>@SquadName
		order by SquadName

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by SquadName

	drop table #higher
	drop table #lower
end




















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















Create Procedure SquadGetNearScore
(
	@squadID int,   -- If they specify squadID=-1, then they want the top
	@squadname char(30) = '' -- (only) if they specify a name, we need to look up the squadID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Score real
declare @firstordinal int

-- If they specify a name, then we need to look up the squadID
if @squadname<>''
begin
	select @squadid=NULL
	select @squadid=squadid from squads where squadname=@squadname
	if @squadid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select * 
			from squads
			where (0=1)
		return
	end
end

-- If they specify squadID=-1, then they want the top
if @squadID=-1
begin
	select @firstordinal=1
	select top 35 @firstordinal as 'FirstOrdinal', *
		from squads
		order by Score desc, squadid desc
end
else
begin
	select @Score=Score from squads 
		where squadID=@squadid 

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from squads 
		where (Score>@Score or (Score=@Score and squadid>=@squadID or @Score is null)) 

	select top 35 * into #higher 
		from squads
		where (Score>@Score or (Score=@Score and squadid>=@squadID) or @Score is null) 
		order by Score, squadid 

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 34 * into #lower 
		from squads
		where (Score<@Score or (Score=@Score and squadid<@squadID)) 
		order by Score desc, squadid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Score desc, squadid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















Create Procedure SquadGetNearWins
(
	@squadID int,   -- If they specify squadID=-1, then they want the top
	@squadname char(30) = '' -- (only) if they specify a name, we need to look up the squadID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Wins real
declare @firstordinal int

-- If they specify a name, then we need to look up the squadID
if @squadname<>''
begin
	select @squadid=NULL
	select @squadid=squadid from squads where squadname=@squadname
	if @squadid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select * 
			from squads
			where (0=1)
		return
	end
end

-- If they specify squadID=-1, then they want the top
if @squadID=-1
begin
	select @firstordinal=1
	select top 35 @firstordinal as 'FirstOrdinal', *
		from squads
		order by Wins desc, squadid desc
end
else
begin
	select @Wins=Wins from squads 
		where squadID=@squadid 

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from squads 
		where (Wins>@Wins or (Wins=@Wins and squadid>=@squadID or @Wins is null)) 

	select top 35 * into #higher 
		from squads
		where (Wins>@Wins or (Wins=@Wins and squadid>=@squadID) or @Wins is null) 
		order by Wins, squadid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 34 * into #lower 
		from squads
		where (Wins<@Wins or (Wins=@Wins and squadid<@squadID)) 
		order by Wins desc, squadid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Wins desc, squadid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure GetNearWinsCmd
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @WinsCmd smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, WinsCmd desc, cs.characterid desc
end
else
begin
	select @WinsCmd=WinsCmd from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (WinsCmd>@WinsCmd or (WinsCmd=@WinsCmd and characterid>=@characterID or @WinsCmd is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (WinsCmd>@WinsCmd or (WinsCmd=@WinsCmd and cs.characterid>=@characterID) or @WinsCmd is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, WinsCmd, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (WinsCmd<@WinsCmd or (WinsCmd=@WinsCmd and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.WinsCmd desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by WinsCmd desc, characterid desc

	drop table #higher
	drop table #lower
end




















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO










CREATE Procedure SquadReportGame
(
        @squadIDWon     int,
        @squadIDLost1   int,
        @squadIDLost2   int = -1,
        @squadIDLost3   int = -1,
        @squadIDLost4   int = -1,
        @squadIDLost5   int = -1
)
As
set nocount on

declare @scoreWinner smallint
declare @scoreLoser smallint
declare @k real

select @scoreWinner = Score from Squads where SquadID=@squadIDWon
select @scoreLoser  = Score from Squads where SquadID=@squadIDLost1
select @scoreWinner
select @scoreLoser

select @k = (1.25 + (@scoreLoser - @scoreWinner) / 1000.0) / 50.0;
select @k

update Squads set GamesPlayed = GamesPlayed + 1, 
				  Wins   = Wins+1,     
                  Score = @scoreWinner * (1 - @k) + @k * 1000
	Where SquadID = @squadIDWon

update Squads set GamesPlayed = GamesPlayed + 1, 
				  Losses = Losses + 1, 
				  Score = @scoreLoser  * (1 - @k)             
	Where SquadID = @squadIDLost1

return



















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO

















Create Procedure SyncZoneSquads
As
set nocount on
Declare cur Cursor Local For
    Select characterID From CharacterInfo
Open cur

create table #playerTeams
(
	vc_team_name varchar(30),
	i_status int,
	i_team_id int,
	i_secondary_status int
)	

Declare curTeam Cursor Local For
    select i_team_id, vc_team_name from #playerTeams

declare @characterID int
declare @vc_team_name varchar(30)
declare @i_status int
declare @i_team_id int
declare @i_secondary_status int

Fetch Next From cur Into @characterID

While @@FETCH_STATUS = 0
Begin

	insert into #playerTeams
		exec profile.profile.dbo.p_get_player_teams_aleg @characterID
	
	Open curTeam

	declare @teamID int
	declare @squadName varchar(30)
	Fetch Next From curTeam Into @i_team_id, @vc_team_name
	
	While @@FETCH_STATUS = 0
	Begin
		if not exists (select * from Squads where SquadID=@i_team_id)
				insert into squads (squadid, squadname) 
					values (@i_team_id, @vc_team_name) 
			Fetch Next From curTeam Into @i_team_id, @vc_team_name
	End
	
	Close curTeam
	delete from #playerTeams
	Fetch Next From cur Into @characterID
End

Close cur
return 

















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadBootMember

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int
declare @status as int
declare @newowner as varchar(30)

exec profile.profile.dbo.p_withdraw_team_aleg @SquadID, @MemberID, @status output, @newowner output, @ErrorCode output, @ErrorMsg output

if @status = -1 delete from squads where squadid = @squadid
else if @newowner is not null exec SquadAddNewLeaderToLog @SquadID, @newowner

if (@status is null) select @ErrorCode, 0
else select @ErrorCode, @status -- status is -1 if team was destroyed because last person left

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadCancelJoinRequest

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int
declare @status as int
declare @newowner as varchar(30)

exec profile.profile.dbo.p_withdraw_team_aleg @SquadID, @MemberID, @status output, @newowner output, @ErrorCode output, @ErrorMsg output

-- this shouldn't ever happen, but let's be safe
if @status = -1 delete from squads where squadid = @squadid
else if @newowner is not null exec SquadAddNewLeaderToLog @SquadID, @newowner

if @ErrorCode is null select 0
else select @ErrorCode

return






















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO











CREATE Procedure SquadCreate

	(
		@SquadName			varchar(30),
		@Description		varchar(510),
		@URL				varchar(255),
		@OwnerID			int,
		@civID				smallint
--		@SquadID			int		     OUTPUT,
--		@ErrorCode			int		 	 OUTPUT,
--		@ErrorMsg			varchar(128) OUTPUT
	)

As

set nocount on

-- trim these down so we don't waste space
select @squadname = rtrim(@squadname)
select @Description = rtrim(@Description)
select @URL = rtrim(@URL) 

declare	@SquadID	int		     
declare	@ErrorCode	int		 	 
declare	@ErrorMsg	varchar(128)

exec profile.profile.dbo.p_create_team_profile_aleg @SquadName, @Description, @URL, @OwnerID, @SquadID output, @ErrorCode output, @ErrorMsg output
/*
-- This is a workaround until the p_create_team_profile_aleg returns a squadid properly
create table #temp2
(
  vc_team_name varchar(30),
  i_status int,
  i_team_id int,
  i_status2 int
)

insert #temp2
  exec profile.profile.dbo.p_get_player_teams_aleg @OwnerID

select @squadID = i_team_id from #temp2 where vc_team_name = @SquadName
*/
	if @ErrorCode = 0
	begin
		delete from squads where SquadID = @SquadID or squadname = @squadname -- remove existing, if any
		insert Squads 
		-- TODO make more robust using pairing of Columns and Values 
		-- select @SquadID, @SquadName, 0, 0, 0, (select CharacterName from CharacterInfo where CharacterID = @OwnerID), @civID
		select @SquadID, @SquadName, 0, 0, 0, '', @civID, 0
		
		-- add new leader to ownership log 
    	declare @charname name
		select @charname = charactername from characterinfo where characterid = @ownerid
		if @charname is not null exec SquadAddNewLeaderToLog @SquadID, @charname
	end

select @SquadID, @ErrorCode, @ErrorMsg

















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadGetNear

	(
		@SortColumn     int = 1,  -- 1 is score, 2 is name, etc. defined Squad.h
		@SquadID        int = -1, -- used for positioning; -1 means top
		@CharacterID    int = -1  -- used for DudeX; -1 means any/all characters
	)

As

set nocount on 

 --  TODO convert ifs into switch/case if SQL has such a thing
 if @SortColumn = 1 exec SquadGetNearNames @SquadID 
 else if @SortColumn = 2 exec SquadGetNearScore @SquadID
 else if @SortColumn = 3 exec SquadGetNearWins @SquadID
 else if @SortColumn = 4 exec SquadGetNearLosses @SquadID
-- TODO else if @SortColumn = 5 exec SquadGetNearHours @SquadID 



















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadQuit

	(
		@MemberID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int
declare @status as int
declare @newowner as varchar(30)

exec profile.profile.dbo.p_withdraw_team_aleg @SquadID, @MemberID, @status output, @newowner output, @ErrorCode output, @ErrorMsg output

if @status = -1 delete from squads where squadid = @squadid
else if @newowner is not null exec SquadAddNewLeaderToLog @SquadID, @newowner

if (@status is null) select @ErrorCode, 0
else select @ErrorCode, @status -- status is -1 if team was destroyed because last person left

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure SquadTransferControl

	(
		@NewOwnerID int,
		@SquadID int
	)

As
set nocount on 

declare @ErrorMsg as varchar(128)
declare @ErrorCode as int

exec profile.profile.dbo.p_change_team_ownership_aleg @SquadID, @NewOwnerID, @ErrorCode output, @ErrorMsg output

-- add new leader to ownership log 
if @errorcode = 0
begin
	declare @charname name
	select @charname = charactername from characterinfo where characterid = @newownerid
	if @charname is not null exec SquadAddNewLeaderToLog @SquadID, @charname
end 

select @ErrorCode 

return





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearBaseCaptures
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @BaseCaptures smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, BaseCaptures desc, cs.characterid desc
end
else
begin
	select @BaseCaptures=BaseCaptures from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (BaseCaptures>@BaseCaptures or (BaseCaptures=@BaseCaptures and characterid>=@characterID or @BaseCaptures is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (BaseCaptures>@BaseCaptures or (BaseCaptures=@BaseCaptures and cs.characterid>=@characterID) or @BaseCaptures is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, BaseCaptures, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (BaseCaptures<@BaseCaptures or (BaseCaptures=@BaseCaptures and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.BaseCaptures desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by BaseCaptures desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearBaseKills
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @BaseKills smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, BaseKills desc, cs.characterid desc
end
else
begin
	select @BaseKills=BaseKills from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (BaseKills>@BaseKills or (BaseKills=@BaseKills and characterid>=@characterID or @BaseKills is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (BaseKills>@BaseKills or (BaseKills=@BaseKills and cs.characterid>=@characterID) or @BaseKills is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, BaseKills, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (BaseKills<@BaseKills or (BaseKills=@BaseKills and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.BaseKills desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by BaseKills desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearDeaths
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Deaths smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, Deaths desc, cs.characterid desc
end
else
begin
	select @Deaths=Deaths from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (Deaths>@Deaths or (Deaths=@Deaths and characterid>=@characterID or @Deaths is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Deaths>@Deaths or (Deaths=@Deaths and cs.characterid>=@characterID) or @Deaths is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, Deaths, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Deaths<@Deaths or (Deaths=@Deaths and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.Deaths desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Deaths desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearGamesPlayed
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @GamesPlayed smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, GamesPlayed desc, cs.characterid desc
end
else
begin
	select @GamesPlayed=GamesPlayed from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (GamesPlayed>@GamesPlayed or (GamesPlayed=@GamesPlayed and characterid>=@characterID or @GamesPlayed is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (GamesPlayed>@GamesPlayed or (GamesPlayed=@GamesPlayed and cs.characterid>=@characterID) or @GamesPlayed is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, GamesPlayed, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (GamesPlayed<@GamesPlayed or (GamesPlayed=@GamesPlayed and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.GamesPlayed desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by GamesPlayed desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearKills
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Kills smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, Kills desc, cs.characterid desc
end
else
begin
	select @Kills=Kills from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (Kills>@Kills or (Kills=@Kills and characterid>=@characterID or @Kills is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Kills>@Kills or (Kills=@Kills and cs.characterid>=@characterID) or @Kills is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, Kills, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Kills<@Kills or (Kills=@Kills and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.Kills desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Kills desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearLosses
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Losses smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, Losses desc, cs.characterid desc
end
else
begin
	select @Losses=Losses from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (Losses>@Losses or (Losses=@Losses and characterid>=@characterID or @Losses is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Losses>@Losses or (Losses=@Losses and cs.characterid>=@characterID) or @Losses is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, Losses, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Losses<@Losses or (Losses=@Losses and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.Losses desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Losses desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearMinutesPlayed
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @MinutesPlayed int
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, MinutesPlayed desc, cs.characterid desc
end
else
begin
	select @MinutesPlayed=MinutesPlayed from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (MinutesPlayed>@MinutesPlayed or (MinutesPlayed=@MinutesPlayed and characterid>=@characterID or @MinutesPlayed is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (MinutesPlayed>@MinutesPlayed or (MinutesPlayed=@MinutesPlayed and cs.characterid>=@characterID) or @MinutesPlayed is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, MinutesPlayed, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (MinutesPlayed<@MinutesPlayed or (MinutesPlayed=@MinutesPlayed and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.MinutesPlayed desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by MinutesPlayed desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearNames
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@CharacterName Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @CharacterName<>''
begin
	select @characterid=NULL
	select top 1 @characterid=characterid from characterinfo 
		where charactername like RTRIM(@CharacterName) + '%' order by charactername
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, CharacterName desc, cs.characterid desc
end
else
begin
	if @CharacterName = ''
		select @CharacterName=CharacterName from charstats 
			where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=1
/*	don't need to spend this cost, since ordinals for names don't mean much
	select @firstordinal=count(*) from charstats 
		where (CharacterName<=@CharacterName) 
			and (civid=@civID or (@civID is null and civid is null)) 
*/

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (CharacterName<=@CharacterName) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid desc, CharacterName desc, cs.characterid desc

--	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (CharacterName>@CharacterName) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid, cs.CharacterName, cs.characterid

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by CharacterName

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















Create Procedure GetNearNamesOld
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*               +-----+
	This proc is | NOT | exactly the same as the other GetNear* procs, due to the fact 
	             +-----+
	that we're not sorting on a charstats columns. There are some subtle differences.
	
	We also supprt partial name matching here, which we do not in the others.
*/

declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	-- find a close match
	select top 1 @characterid=characterid 
		from characterinfo 
		where charactername like RTRIM(@charname) + '%'
		order by CharacterName
	if @characterid is null
	begin
		begin
			-- Make sure we still create a result set of the expectant type
			select cs.* 
				from charstats cs inner join characterinfo ci on ci.characterid=cs.characterid
				where (0=1)
			return
		end
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', ci.CharacterName, ci.CharacterID, cs.CivID, 
			cs.Rank, cs.Score, cs.MinutesPlayed, 
			cs.BaseKills, cs.BaseCaptures, cs.Kills, cs.Rating, cs.GamesPlayed, cs.Deaths, 
			cs.Wins, cs.Losses, cs.LastPlayed 
		from charstats cs right outer join characterinfo ci on cs.characterid=ci.characterid
			and (cs.civid=@civID or (@civID is null and cs.civid is null))
		order by ci.CharacterName
end
else
begin
	-- Map the id to a name if they didn't pass in a name
	if @charname=''
		select @charname=CharacterName from CharacterInfo where CharacterId=@characterID

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from CharacterInfo 
		where (CharacterName <= @charname) 

	select top 42 ci.CharacterName, ci.CharacterID, cs.CivID, cs.Rank, cs.Score, cs.MinutesPlayed, 
			cs.BaseKills, cs.BaseCaptures, cs.Kills, cs.Rating, cs.GamesPlayed, cs.Deaths, 
			cs.Wins, cs.Losses, cs.LastPlayed 
		into #higher 
		from charstats cs right outer join characterinfo ci on cs.characterid=ci.characterid
			and (cs.civid=@civID or (@civID is null and cs.civid is null))
		where (ci.CharacterName <= @charname) 
		order by ci.CharacterName desc
	
	select @firstordinal=@firstordinal - count(*) + 1 from #higher
	
	select top 42 ci.CharacterName, ci.CharacterID, cs.CivID, cs.Rank, cs.Score, cs.MinutesPlayed, 
			cs.BaseKills, cs.BaseCaptures, cs.Kills, cs.Rating, cs.GamesPlayed, cs.Deaths, 
			cs.Wins, cs.Losses, cs.LastPlayed 
		into #lower
		from charstats cs right outer join characterinfo ci on cs.characterid=ci.characterid
			and (cs.civid=@civID or (@civID is null and cs.civid is null))
		where (ci.CharacterName > @charname) 
		order by ci.CharacterName

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by CharacterName

	drop table #higher
	drop table #lower
end




















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearRating
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Rating smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, Rating desc, cs.characterid desc
end
else
begin
	select @Rating=Rating from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (Rating>@Rating or (Rating=@Rating and characterid>=@characterID or @Rating is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Rating>@Rating or (Rating=@Rating and cs.characterid>=@characterID) or @Rating is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, Rating, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Rating<@Rating or (Rating=@Rating and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.Rating desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Rating desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearScore
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Score real
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, Score desc, cs.characterid desc
end
else
begin
	select @Score=Score from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (Score>@Score or (Score=@Score and characterid>=@characterID or @Score is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Score>@Score or (Score=@Score and cs.characterid>=@characterID) or @Score is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, Score, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Score<@Score or (Score=@Score and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.Score desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Score desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO





















CREATE Procedure GetNearWins
(
	@characterID int,   -- If they specify characterID=-1, then they want the top
	@civID smallint,    -- If they specify -1, then they meant NULL
	@charname Name = '' -- (only) if they specify a name, we need to look up the characterID (ignoring what was passed in)
)
As
set nocount on 

/*
	All the GetNear* functions are exactly the same, except for the sort column, and the type of the sort column.
	
	Any changes made to one needs to be made to all the others. I typically just modify one, then cut & paste
	the whole thing to all others, then use search & replace on the single 
*/

declare @Wins smallint
declare @firstordinal int

-- Map -1 to NULL since passing NULL from ODBC is a pain
if @civID = -1
	select @civID = NULL

-- If they specify a name, then we need to look up the characterID
if @charname<>''
begin
	select @characterid=NULL
	select @characterid=characterid from characterinfo where charactername=@charname
	if @characterid is null
	begin
		-- Make sure we still create a result set of the expectant type
		select cs.* 
			from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
			where (0=1)
		return
	end
end

-- If they specify characterID=-1, then they want the top
if @characterID=-1
begin
	select @firstordinal=1
	select top 56 @firstordinal as 'FirstOrdinal', cs.*
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where civid=@civID or (@civID is null and civid is null)
		order by civid desc, Wins desc, cs.characterid desc
end
else
begin
	select @Wins=Wins from charstats 
		where CharacterID=@characterid And (civid=@civid or (@civID is null and civid is null))

	-- it's not quite the first ordinal yet, just count for now. Will patch up to be ordinal down below
	select @firstordinal=count(*) from charstats 
		where (Wins>@Wins or (Wins=@Wins and characterid>=@characterID or @Wins is null)) 
			and (civid=@civID or (@civID is null and civid is null)) 

	select top 42 cs.* into #higher 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Wins>@Wins or (Wins=@Wins and cs.characterid>=@characterID) or @Wins is null) 
			and (civid=@civID or (@civID is null and civid is null))
		order by civid, Wins, cs.characterid

	select @firstordinal=@firstordinal - count(*) + 1 from #higher

	select top 42 cs.* into #lower 
		from charstats cs --inner join characterinfo ci on ci.characterid=cs.characterid
		where (Wins<@Wins or (Wins=@Wins and cs.characterid<@characterID)) 
			and (civid=@civID or (@civID is null and civid is null)) 
		order by civid desc, cs.Wins desc, cs.characterid desc

	select @firstordinal as 'FirstOrdinal', o.* from 
	(
		select * from #higher
		union all
		select* from #lower
	) AS o 
	order by Wins desc, characterid desc

	drop table #higher
	drop table #lower
end





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure LogonUser
	@characterName Name,
	@characterID int,
	@fCanCheat bit output
As
set nocount on

-- Called anytime we want to validate a username/id

declare @charID1 int
declare @name1 Name

Select @name1 = characterName, @fCanCheat = CanCheat, @charID1 = characterID
	from CharacterInfo 
	Where characterID=@characterID or charactername=@characterName

if @@rowcount <> 1 or @name1 <> @characterName or @charID1 <> @characterID --everything's fine
begin
	delete from CharStats		where characterID=@characterID or charactername=@charactername
	delete from CharMedals		where characterID=@characterID or characterid=@charID1
	delete from CharacterInfo	where characterID=@characterID or charactername=@charactername
	Insert Into CharacterInfo (CharacterName, CharacterID) Values(@characterName, @characterID)
	select @fCanCheat=0
end

return 










GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO

CREATE Procedure SetCharacterStats
(
--	The caller will be sending the values for ONE GAME ONLY, except combat rating, which is overall
	@bScoresCount   bit,
	@CharacterId	int,
	@CivId			smallint,
	@Rating			smallint, -- Combat rating (overall)

-- Stuff that affects Score (values from only one game):
    @WarpsSpotted	real,
    @AsteroidsSpotted real,
    @TechsRecovered real,
    @MinerKills		real,
    @BuilderKills	real,
    @LayerKills		real,
    @PlayerKills	real,
    @BaseKills		real,
    @BaseCaptures	real,
    @Deaths			smallint,
    @PilotBaseKills smallint,
    @PilotBaseCaptures smallint,
	@MinutesPlayed	int,
	@bWin			bit,
	@bLose			bit,
	@bWinCmd		bit,
	@bLoseCmd		bit,
	@RankOld		smallint,
	@Score			real		 -- score that they got FOR THIS GAME
)
As
set nocount on
declare @Kills smallint -- how do all the types of kills get mapped to THE kills stat?
declare @TotalScore real
declare @RankNew smallint

/* We could do this, but let's not due to perf
if @civid not in (select civID from Civs)
Begin
	RaisError ('SetCharStats: You must specify a valid civ to update', 18, 1) WITH NOWAIT
	return
End
*/

if @bScoresCount=1
begin
	if @Score < 0
		select @Score = 0

	Select @Kills=@PlayerKills

	--Check for Civ rank advancement
	select @TotalScore=Score from CharStats where CharacterId=@characterid and civid=@civid
	if @TotalScore is null
		select @TotalScore=0
	select @TotalScore=@TotalScore + @Score

	select @RankNew = max(r.rank) from ranks r 
			where (r.civid=@CivId or (r.civid is null and @CivId is null)) and r.requirement <= @TotalScore
end

declare @charname Name
-- not doing begin tran because it's just too slow!
-- Update the civ-specific stats
if exists (select * from CharStats where CharacterId=@characterid and civid=@civid)
	if @bScoresCount=1
		update CharStats set 
				Rank=@RankNew, Score=@TotalScore, MinutesPlayed=MinutesPlayed+@MinutesPlayed, 
				BaseKills=BaseKills+@BaseKills, BaseCaptures=BaseCaptures+@BaseCaptures, 
				Kills=Kills+@Kills, Rating=@Rating, GamesPlayed=GamesPlayed+1, 
				Deaths=Deaths+@Deaths, Wins=Wins+@bWin, Losses=Losses+@bLose, 
				WinsCmd=WinsCmd+@bWinCmd, LossesCmd=LossesCmd+@bLoseCmd, LastPlayed=GetDate()
			where characterid=@characterid and civid=@civid 
	else
		update CharStats set 
				MinutesPlayed=MinutesPlayed+@MinutesPlayed, 
				GamesPlayed=GamesPlayed+1, LastPlayed=GetDate()
			where characterid=@characterid and civid=@civid 
else
begin
	select @charname=CharacterName from CharacterInfo Where CharacterId=@CharacterID
	if @bScoresCount=1
		insert into CharStats (
				CharacterId, CharacterName, CivID, Rank, Score, MinutesPlayed, BaseKills, 
				BaseCaptures, Kills, Rating, GamesPlayed, Deaths, Wins, Losses, WinsCmd, LossesCmd, LastPlayed) 
			Values(@CharacterId, @charname, @CivID, @RankNew, @TotalScore, @MinutesPlayed, @BaseKills, 
				@BaseCaptures, @Kills, @Rating, 1, @Deaths, @bWin, @bLose, @bWinCmd, @bLoseCmd, GetDate()) 
	else
		insert into CharStats (
				CharacterId, CharacterName, CivID, MinutesPlayed, GamesPlayed, LastPlayed) 
			Values(@CharacterId, @charname, @CivID, @MinutesPlayed, 1, GetDate()) 
end

-- Update overall stats (ignore scores count, because we're just aggregating the per civ stuff
declare	@GamesPlayed smallint, @Wins smallint, @Losses smallint, @WinsCmd smallint, @LossesCmd smallint, 
		@OverallScore real, @OverallRank smallint
-- First calculate them. We know we have at least one civ at this point, since we just added/updated it
select	@OverallScore=Sum(Score), @MinutesPlayed=Sum(MinutesPlayed), 
		@BaseKills=Sum(BaseKills), @BaseCaptures=Sum(BaseCaptures), @Kills=Sum(Kills), 
		@Rating=Max(Rating), 
		@GamesPlayed=Sum(GamesPlayed), @Deaths=Sum(Deaths), @Wins=Sum(Wins), @Losses=Sum(Losses),
		@WinsCmd=Sum(WinsCmd), @LossesCmd=Sum(LossesCmd)
	from charstats 
	where characterid=@characterid and not (civid is NULL)

-- Now update them (getting the new rank for overall)
select @OverallRank = max(r.rank) from ranks r where r.civid is null and r.requirement <= @OverallScore

if exists (select * from CharStats where CharacterId=@characterid and civid is null)
	update CharStats set 
			Rank=@OverallRank, Score=@OverallScore, MinutesPlayed=@MinutesPlayed, 
			BaseKills=@BaseKills, BaseCaptures=@BaseCaptures, Kills=@Kills, Rating=@Rating, 
			GamesPlayed=@GamesPlayed, Deaths=@Deaths, Wins=@Wins, Losses=@Losses, 
			WinsCmd=@WinsCmd, LossesCmd=@LossesCmd, LastPlayed=GetDate()
		where characterid=@characterid and civid is null
else
begin
	if @charname is null	
		select @charname=CharacterName from CharacterInfo Where CharacterId=@CharacterID
	insert into CharStats (
			CharacterId, CharacterName, CivID, Rank, Score, MinutesPlayed, BaseKills, 
			BaseCaptures, Kills, Rating, GamesPlayed, Deaths, Wins, Losses, WinsCmd, LossesCmd, LastPlayed) 
		values(@CharacterId, @charname, null, @OverallRank, @Score, @MinutesPlayed, @BaseKills, 
			@BaseCaptures, @Kills, @Rating, @GamesPlayed, @Deaths, @Wins, @Losses, @WinsCmd, @LossesCmd, GetDate()) 
end	
-- commit tran

return 

GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO















CREATE Procedure GetLobbyLogonInfo
(
	@characterName Name,
	@characterID int,
	@accesscode AccessCode,
	@fValidCode bit output,
	@fCanCheat bit output
)
As
set nocount on 

select @fValidCode=0
select @fCanCheat=0

-- We're gonna assume if they got this far they're kosher, so add 'em if they're not there already
exec LogonUser @characterName, @characterID, @fCanCheat output

declare @status smallint

/* Status Codes
0: code never been used. There must NOT be any rows in AccessCodeUsage for this code
1: code in use (normal case). 
2: code has been reassigned. Invalid code now
3: code has been disabled for whatever random reason. Invalid code now
4: code is reserved for future use. Invalid code at this time
*/

select @status=status from AccessCodes where accesscode=@accesscode

if @@rowcount=1 and @status <= 1 --valid access code
begin	
	if @status = 0 -- code never been used
		update AccessCodes set status = 1 where accesscode=@accesscode

	-- Assume that more often than not, the code will be used by a name that's used it before
	update AccessCodeUsage set LastUsage=getdate() where AccessCode=@accesscode and ZoneName=@characterName

	if @@rowcount=0 --new zone name used with this key
		insert into AccessCodeUsage (AccessCode, ZoneName) values (@accesscode, @characterName)
	
	select @fValidCode=1
end

-- We need to tell them which squads they're in. This generates a rowset
-- I can't really skip this if the access code is invalid because 
--		a) the lobby might not be enforcing access codes, and 
--		b) the lobby will require a properly formed rowset anyway
exec GetCharSquads @characterID










GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO











CREATE Procedure GetLogonStats
(
	@characterName Name,
	@characterID int,
	@fCanCheat bit output
)
As
set nocount on 

-- We're gonna assume if they got this far they're kosher, so add 'em if they're not there already
exec LogonUser @characterName, @characterID, @fCanCheat output

Select ISNULL(CivID, -1), Rank, Score, MinutesPlayed, BaseKills, BaseCaptures, 
		Kills, Rating, GamesPlayed, Deaths, Wins, Losses 
	From CharStats Where CharacterId=@characterID











GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO






















CREATE Procedure SetRandomStats
(
	@charID int,
	@civID smallint
)
As
	declare @Rank	smallint
	declare @Score			real
	declare @MinutesPlayed	smallint
	declare @Kills			smallint
	declare @Deaths			smallint
	declare @Wins			smallint
	declare @Losses			smallint
	declare @Rating			smallint
	declare @Warps			real
	declare @Asteroids		real
	declare @Techs			real
	declare @MinerKills		real
	declare @BuilderKills	real
	declare @LayerKills		real
	declare @PlayerKills	real
	declare @BaseKills		real
	declare @BaseCaptures	real
	declare @PilotBaseKills smallint
	declare @PilotBaseCaptures smallint
	declare @bWin			bit
	
	select @MinutesPlayed=convert(smallint, 300 * rand())
	select @kills=convert(smallint, 100 * rand())
	select @Deaths=convert(smallint, 10 * rand())
	select @Wins=convert(smallint, 1000 * rand())
	select @Losses=convert(smallint, 1000 * rand())
	select @Rating=convert(smallint, 1000 * rand())
	select @Warps=convert(real, 10 * rand())
	select @Asteroids=convert(real, 10 * rand())
	select @Techs=convert(real, 5 * rand())
	select @MinerKills=convert(real, 3 * rand())
	select @BuilderKills=convert(real, 1 * rand())
	select @LayerKills=convert(real, 1 * rand())
	select @PlayerKills=convert(real, 10 * rand())
	select @BaseKills=convert(real, 1 * rand())
	select @BaseCaptures=convert(real, 2 * rand())
	select @PilotBaseKills=convert(smallint, 3 * rand())
	select @PilotBaseCaptures=convert(smallint, 2 * rand())
	select @bWin=convert(smallint, 2 * rand())
	select @Rank=1
	
	exec SetCharacterStats
		@CharID,
		@CivId,
		@Rating,
		@Warps,
		@Asteroids,
		@Techs,
		@MinerKills,
		@BuilderKills,
		@LayerKills,
		@PlayerKills,
		@BaseKills		,
		@BaseCaptures	,
		@Deaths			,
		@PilotBaseKills,
		@PilotBaseCaptures,
		@MinutesPlayed	,
		@bWin,
		@Rank output,
		@Score output
	return 





















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO




















CREATE Procedure CreateLotsOfRandomUsers
	(
		@startingnumber int,
		@numusers int,
		@civid1 smallint,
		@civid2 smallint = NULL,
		@civid3 smallint = NULL
	)
As
set nocount on

declare @i int

select @i=0
while (@i < @numusers)
begin
	declare @charname char(12)
	declare @charid int
	
	select @charname='Random' + convert(char(6), @i + @startingnumber)
	insert into characterinfo (CharacterID, CharacterName) values (@i + @startingnumber, @charname)
	select @charid=characterid from characterinfo where charactername=@charname
	
	--exec SetRandomStats @charID, @CivID1	
	
	if @civID2 is not null
		exec SetRandomStats @charID, @CivID2

	if @civID3 is not null
		exec SetRandomStats @charID, @CivID3
	
	select @i=@i+1
end

return 























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO















Create Trigger Constants_Insert
On dbo.Constants
For Insert
As
	If (Select count(*) from Constants) > 1 
   	Begin
   		Rollback Transaction
   		RaisError ('Only one row is allowed in the Constants table', 18, 1) WITH NOWAIT
   	End















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























Create Trigger Drones_Trigger_InsUpd
On dbo.Drones
For Insert, Update
As
If Update (TechBitsRequired) or  Update (TechBitsEffect)
Begin
    declare @okR bit, @okE bit
    declare @techbitsR TechBits, @techbitsE TechBits
    select @okR = 0
    select @okE = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsRequired, TechBitsEffect From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsR, @techbitsE
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsR, @okR Output
    	If @okR=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsRequired', 18, 1) WITH NOWAIT
    	End
    	Execute IsValidTechBits @techbitsE, @okE Output
    	If @okE = 0 
    	Begin
    	Rollback Transaction
    		RaisError ('Invalid tech in TechBitsEffect', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsR, @techbitsE
    End

    Close curIns
End























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger Expendables_Trigger_InsUpd
On dbo.Expendables
For Insert, Update
As
If Update (TechBitsRequired) or  Update (TechBitsEffect)
Begin
    declare @ok bit
    declare @techbitsR TechBits, @techbitsE TechBits
    select @ok = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsRequired, TechBitsEffect From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsR, @techbitsE
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsR, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsRequired', 18, 1) WITH NOWAIT
    	End
    	Execute IsValidTechBits @techbitsE, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsEffect', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsR, @techbitsE
    End

    Close curIns
End























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO
















CREATE Trigger StationTypes_Trigger_InsUpd
On dbo.StationTypes
For Insert, Update
As

declare @ok bit

If Update (TechBitsRequired) or  Update (TechBitsEffect) or Update(TechBitsLocal)
Begin
    declare @techbitsR TechBits, @techbitsE TechBits, @techbitsL TechBits
    select @ok = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsRequired, TechBitsEffect, TechBitsLocal From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsR, @techbitsE, @techbitsL
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsR, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsRequired', 18, 1) WITH NOWAIT
    	End
    	Execute IsValidTechBits @techbitsE, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsEffect', 18, 1) WITH NOWAIT
    	End
    	Execute IsValidTechBits @techbitsL, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsLocal', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsR, @techbitsE, @techbitsL
    End

    Close curIns
End

/* why the freal won't nested comments work??? */
/* why can't I use false in logical expressions??? */
If (0=1 and (Update (TechBitsLocal) or Update(TechBitsEffect)))
Begin
    declare     @iIndex				int
    declare     @siStationType      smallint
    declare     @siUpgradeType      smallint
    declare     @szStationEffect    TechBits
    declare     @szUpgradeEffect    TechBits
    declare     @iStationEffect     int
    declare     @iUpgradeEffect     int
    declare     @szStationLocal     TechBits
    declare     @szUpgradeLocal     TechBits
    declare     @iStationLocal      int
    declare     @iUpgradeLocal      int
    declare     @szProblemBits      char(20)

    declare     curStations cursor local for
    	                select st1.stationtypeid, st2.stationtypeid,
    	                st1.techbitseffect, st2.techbitseffect,
    	                st1.techbitslocal, st2.techbitslocal
    	                from inserted st1, stationtypes st2 
    	                where (st1.upgradestationtypeid = st2.stationtypeid) or
    						  (st1.stationtypeid = st2.upgradestationtypeid)
    	                order by st1.stationtypeid

    open        curStations                 

    fetch next from curStations into 
    				@siStationType, 
    	            @siUpgradeType,
    	            @szStationEffect,
    	            @szUpgradeEffect,
    	            @szStationLocal,
    	            @szUpgradeLocal
    while (@@fetch_status <> -1)
    begin
    	if (@@fetch_status <> -2)
    	begin
    		/* the inserted table could have either the base OR upgraded station */
    		If (@siStationType < @siUpgradeType)
    		Begin
    			/* need to rearrange the stations */
    			declare @siT smallint
    			Select @siT = @siStationType
    			Select @siStationType = @siUpgradeType
    			Select @siUpgradeType = @siT
    			declare @tbT TechBits
    			Select @tbT = @szStationEffect
    			Select @szStationEffect = @szUpgradeEffect
    			Select @szUpgradeEffect = @tbT
    			Select @tbT = @szStationLocal
    			Select @szStationLocal = @szUpgradeLocal
    			Select @szUpgradeLocal = @tbT
    		End

    		Execute IsTechBitsSuperset @szStationEffect, @szUpgradeEffect, @ok Output
    		if @ok=0
    			select @szProblemBits = '(Effect bits)'
    		else
    		Begin
    			Execute IsTechBitsSuperset @szStationLocal, @szUpgradeLocal, @ok Output
    			if @ok=0
    				select @szProblemBits = '(Local bits)'
    		End
    		
    		if @ok=0
    		Begin
    			Rollback Transaction
    			declare @msg varchar(100)
    			select @msg = 'Warning: Illegal upgrade from station (' +
    	   Convert(char(3), @siStationType) + ') to station (' +
    	                Convert(char(3), @siUpgradeType) + ') ' + @szProblemBits
    			RaisError (@msg, 18, 1) WITH NOWAIT
    		End
   end
    	fetch next from curStations into @siStationType,
    	                @siUpgradeType,
    	                @szStationEffect,
    	                @szUpgradeEffect,
    	                @szStationLocal,
    	                @szUpgradeLocal
    end

    close       curStations
    deallocate  curStations

End
















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO
















Create Trigger Developments_Trigger_InsUpd
On dbo.Developments
For Insert, Update
As
If Update (TechBitsRequired) or  Update (TechBitsEffect)
Begin
    declare @okR bit, @okE bit
    declare @techbitsR TechBits, @techbitsE TechBits
    select @okR = 0
    select @okE = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsRequired, TechBitsEffect From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsR, @techbitsE
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsR, @okR Output
    	If @okR=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsRequired', 18, 1) WITH NOWAIT
    	End
    	Execute IsValidTechBits @techbitsE, @okE Output
    	If @okE=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsEffect', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsR, @techbitsE
    End

    Close curIns
End
















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger PartsLastUpdated
On dbo.Parts
For Insert, Update
As
    UPDATE Parts Set LastUpdated=GetDate()
    WHERE Parts.PartID IN (SELECT PartID FROM inserted)























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger Parts_Trigger_InsUpd
On dbo.Parts
For Insert, Update
As
If Update (TechBitsRequired) or  Update (TechBitsEffect)
Begin
    declare @ok bit
    declare @techbitsR TechBits, @techbitsE TechBits
    select @ok = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsRequired, TechBitsEffect From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsR, @techbitsE
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsR, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsRequired', 18, 1) WITH NOWAIT
    	End
    	Execute IsValidTechBits @techbitsE, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsEffect', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsR, @techbitsE
    End

    Close curIns
End























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger Aleph_InsUpd
On dbo.AlephInstances
For Insert, Update
As
    DECLARE @dupes integer

    SELECT @dupes=count(*) FROM Inserted WHERE
    		AlephID=TargetAlephID
    IF @dupes > 0
    BEGIN
    	RaisError('Do you really want an aleph to lead to itself? No? I didn''t think you would, so fix it.', 16, -1)
    	ROLLBACK TRANSACTION
    END























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger Civs_Trigger_InsUpd
On dbo.Civs
For Insert, Update
As
If Update (TechBitsCiv)
Begin
    declare @ok bit
    declare @techbits char(800)
    select @ok = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsCiv From Inserted
    Open curIns

    Fetch Next From curIns Into @techbits
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbits, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsCiv', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbits
    End

    Close curIns
End
If Update (TechBitsCivNoDev)
Begin
    declare @okNoDev bit
    declare @techbitsNoDev char(800)
    select @okNoDev = 0
    /* need to see if ANY modified row has bogus techbits */
    Declare curIns Cursor Local For
    	Select TechBitsCivNoDev From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsNoDev
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsNoDev, @okNoDev Output
    	If @okNoDev=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsCivNoDev', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsNoDev
    End

    Close curIns
End






















GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger ShipTypes_Trigger_InsUpd
On dbo.ShipTypes
For Insert, Update
As

If Update (TechBitsRequired) or Update (TechBitsEffect)
Begin
    declare @ok bit
    declare @techbitsR TechBits, @techbitsE TechBits
    select @ok = 0
    Declare curIns Cursor Local For
    	Select TechBitsRequired, TechBitsEffect From Inserted
    Open curIns

    Fetch Next From curIns Into @techbitsR, @techbitsE
    
    While @@FETCH_STATUS = 0
    Begin
    	Execute IsValidTechBits @techbitsR, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsRequired', 18, 1) WITH NOWAIT
    	End
    	
    	Execute IsValidTechBits @techbitsE, @ok Output
    	If @ok=0
    	Begin
    		Rollback Transaction
    		RaisError ('Invalid tech in TechBitsEffect', 18, 1) WITH NOWAIT
    	End
        Fetch Next From curIns Into @techbitsR, @techbitsE
    End

    Close curIns
End























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























Create Trigger StationInstances_Trigger_InsUpd
On dbo.StationInstances
For Insert, Update
As
        DECLARE @zerocount integer, @coincidentcount integer
        SELECT @zerocount=count(*) FROM Inserted WHERE
                        (UpX=0 AND UpY=0 AND UpZ=0) OR
                        (ForwardX=0 AND ForwardY=0 AND ForwardZ=0)
        SELECT @coincidentcount=count(*) FROM Inserted WHERE
                        (UpX=ForwardX) AND
                        (UpY=ForwardY) AND
                        (UpZ=ForwardZ)
        IF @zerocount > 0
        BEGIN
                RaisError('Cannot have zero length orientation vectors', 16, -1)
                ROLLBACK TRANSACTION
        END
        IF @coincidentcount > 0
        BEGIN
                RaisError('Cannot have coincident Forward and Up orientation vectors', 16, -1)
                ROLLBACK TRANSACTION
        END























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  ON    SET ANSI_NULLS  ON 
GO























CREATE Trigger AttachPoints_Trigger_InsUpd
On dbo.AttachPoints
For Insert, Update
As
If Update (ShipTypeID)
Begin
    /* Let's just be lazy and create a cursor over the Inserted table, and 
    	rather just check the whole regular table, since it won't be that big.
    	Only trick will be that if bad data DOES get in, then ANY change 
    	(involving a shiptypeid) will cause the transaction to be rejected.s
    */
    If exists (Select * from attachpoints group by shiptypeid, parttypeid having COUNT(*) > 4)
    Begin
    	Rollback Transaction
    	RaisError ('Each ship can have no more than 4 attach points per part type', 18, 1) WITH NOWAIT
    End
End























GO
SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

