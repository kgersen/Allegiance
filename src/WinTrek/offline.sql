select
	'EFFECT',
	RTRIM(FileSound),
	EffectId,
	IsLooping
from
	Effects


select
	'PROJECTILE',
	ProjectileId,
	HitPointsInflict,
	SpeedMax,
	IsAbsoluteSpeed,
	RTRIM(FileModel),
	RTRIM(FileTexture),
	convert(float, TimeDuration) / 1000.0,
	convert(float, size_cm) / 100.0,
	RateRotation,
	convert(float, percentRed) / 100.0,
	convert(float, percentGreen) / 100.0,
	convert(float, percentBlue) / 100.0,
	convert(float, percentAlpha) / 100.0,
	DamageType
from
	Projectiles




declare @szAttachPoint		char(255)
declare @szTemp			char(255)
declare @szCopy			char(255)
declare @iMaxShipTypeID		int
declare @iMaxAttachPoint	int
declare @iIndex			int
declare @iAPIndex		int

select @iMaxShipTypeID = MAX(shiptypeID) from shiptypes
select @iIndex = 0

while (@iIndex <= @iMaxShipTypeID)
	BEGIN
		if (select COUNT(*) from shiptypes where shiptypeid = @iIndex) = 1
			BEGIN
				select @szAttachPoint = ' '
				select @iMaxAttachPoint = MAX(attachpointID) from attachpoints where shiptypeid = @iIndex
				select @iAPIndex = 1
				while(@iAPIndex <= @iMaxAttachPoint)
					BEGIN
						select
							@szTemp = RTRIM(framename) + char(9) + Mountclass
						from
							attachpoints
						where
							(attachpointid = @iAPIndex) AND
							(shiptypeid = @iIndex)

						if (@szAttachPoint = ' ')
							select @szCopy = @szTemp
						else
							select @szCopy = @szAttachPoint + char(9) + @szTemp
						select @szAttachPoint = @szCopy
						select @iAPIndex = @iAPIndex + 1
					END

				if (@iMaxAttachPoint IS NULL)
					select @iMaxAttachPoint = 0

				select
					'HULLTYPE',
					Available1 + (2 * Available2) + 
						(4 * Available3) + (8 * Available4) +
						(16 * Available5) + (32 * Available6),
					RTRIM(Model),
					RTRIM(Name),
					RTRIM(Description),
					Price,
					Weight,
					BaseSignature,
					ShipTypeID,
					BaseMaxSpeed,
					RollRate,
					TurnRate,
					Acceleration,
					BaseScannerRange,
					FrameLandingBegin,
					FrameLandingEnd,
					FrameIdle2LandingBegin,
					FrameIdle2LandingEnd,
					FrameIdleBegin,
					FrameIdleEnd,
					FrameIdle2SpecialBegin,
					FrameIdle2SpecialEnd,
					FrameSpecialBegin,
					FrameSpecialEnd,
					CockpitX,
					CockpitY,
					CockpitZ,
					BaseHitPoints,
					Capacity,
					Length,
					'',
					NoseX,
					NoseY,
					WingX,
					WingY,
					TailX,
					TailY,
					@iMaxAttachPoint,
					EnergyMax,
					RateRechargeEnergy,
					@szAttachPoint
				from
					shiptypes
				where
					shiptypeid = @iIndex
			END
		select @iIndex = @iIndex + 1
	END


select
	'WEAPON',
	Available1 + (2 * Available2) + 
		(4 * Available3) + (8 * Available4) +
		(16 * Available5) + (32 * Available6),
	RTRIM(FileModel),
	RTRIM(Name),
	RTRIM(Description),
	Price,
	Mass,
	dSignatureMounted,
	Parts.PartID,
	Drain,
	dTimeReady,
	dTimeBurstShots,
	EnergyPerShot,
	AmmoType,
	cBulletsPerShot,
	ProjectileID1,
	EffectID
from
	Parts,
	Weapons
where
	Parts.PartID = Weapons.PartID

select
	'SHIELD',
	Available1 + (2 * Available2) + 
		(4 * Available3) + (8 * Available4) +
		(16 * Available5) + (32 * Available6),
	RTRIM(FileModel),
	RTRIM(Name),
	RTRIM(Description),
	Price,
	Mass,
	dSignatureMounted,
	Parts.PartID,
	Drain,
	RegenRate,
	Shields.HitPoints,
	PercentReflectParticle,
	PercentReflectEnergy
from
	Parts,
	Shields
where
	Parts.PartID = Shields.PartID

select
	'AFTERBURNER',
	Available1 + (2 * Available2) + 
		(4 * Available3) + (8 * Available4) +
		(16 * Available5) + (32 * Available6),
	RTRIM(FileModel),
	RTRIM(Name),
	RTRIM(Description),
	Price,
	Mass,
	dSignatureMounted,
	Parts.PartID,
	Drain,
	BurnRate,
	MaxThrust,
	rateOn,
	rateOff,
	HullThrustMultiplier
from
	Parts,
	Afterburners
where
	Parts.PartID = Afterburners.PartID


select
	'SECTOR',
	SectorID,
	RTRIM(SectorName),
	DebrisCount,
	StarsCount,
	randseed,
	LightX,
	LightY,
	LightZ,
	LightColor
from
	SectorInfo

select
	'ALEPH',
	AlephID,
	AlephIDTarget,
	RTRIM(Texture),
	Radius,
	SectorID,
	RTRIM(AlephName),
	LocationX,
	LocationY,
	LocationZ,
	OrientationYx,
	OrientationYy,
	OrientationYz,
	OrientationZx,
	OrientationZy,
	OrientationZz,
	RotationAx,
	RotationAy,
	RotationAz,
	RotationR

from
	AlephInstances,
	AlephDef
where
	AlephInstances.AlephTypeID = AlephDef.AlephTypeID

select
	'PLANET',
	PlanetID,
	WhatThisIs,
	RTRIM(Texture),
	RTRIM(Texture),
	Radius,
	SectorID,
	RTRIM(PlanetName),
	LocationX,
	LocationY,
	LocationZ,
	OrientationYx,
	OrientationYy,
	OrientationYz,
	OrientationZx,
	OrientationZy,
	OrientationZz,
	RotationAx,
	RotationAy,
	RotationAz,
	RotationR
from
	PlanetInstances
	
select
	'STATION',
	StationID,
	HitPoints,
	HitPoints / 2,
	RateRegen,
	RateRegen * 2,
	RTRIM(Model),
	RTRIM(Model),
	Radius,
	FrameIdleBegin,
	FrameIdleEnd,
	SectorID,
	TeamNum,
	RTRIM(Name),
	LocationX,
	LocationY,
	LocationZ,
	OrientationYx,
	OrientationYy,
	OrientationYz,
	OrientationZx,
	OrientationZy,
	OrientationZz,
	RotationAx,
	RotationAy,
	RotationAz,
	RotationR
from
	Stations
	

select
	'POSTER',
	RTRIM(Texture),
	Latitude,
	Longitude,
	Radius,
	SectorID
from
	PosterInstances

