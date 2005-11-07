
set NOCOUNT on
/*
	Look for error rows in the data.
*/

/*
	Object Radius Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from expendables where radius <= 0.0
if (@cErrors > 0)
	select name, radius from expendables, missiles where (radius <= 0.0)
select @cErrors = COUNT(*) from projectiles where size_cm <= 0
if (@cErrors > 0)
	select projectileid, size_cm from projectiles where size_cm <= 0
select @cErrors = COUNT(*) from shiptypes where length <= 0
if (@cErrors > 0)
	select shiptypeid, name, length from shiptypes where length <= 0
select @cErrors = COUNT(*) from stationtypes where radius <= 0.0
if (@cErrors > 0)
	select stationtypeid, name, radius from stationtypes where radius <= 0
go


/*
	Object Mass Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from expendables where mass <= 0.0
if (@cErrors > 0)
	select expendableid, name, mass from expendables where mass <= 0.0
select @cErrors = COUNT(*) from parts where mass <= 0
if (@cErrors > 0)
	select partid, name, mass from parts where mass <= 0
select @cErrors = COUNT(*) from shiptypes where weight <= 0.0
if (@cErrors > 0)
	select shiptypeid, name, weight from shiptypes where weight <= 0.0
go



/*
	Hit Points Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from expendables where hitpoints < 0
if (@cErrors > 0)
	select expendableid, name, hitpoints from expendables where hitpoints < 0
select @cErrors = COUNT(*) from projectiles where hitpointsself < 0
if (@cErrors > 0)
	select projectileid, hitpointsself from projectiles where hitpointsself < 0
select @cErrors = COUNT(*) from shiptypes where basehitpoints < 0
if (@cErrors > 0)
	select shiptypeid, name, basehitpoints from shiptypes where basehitpoints < 0
select @cErrors = COUNT(*) from stationtypes where hitpointsarmor < 0
if (@cErrors > 0)
	select stationtypeid, name, hitpointsarmor from stationtypes where hitpointsarmor < 0
select @cErrors = COUNT(*) from stationtypes where hitpointsshield < 0
if (@cErrors > 0)
	select stationtypeid, name, hitpointsshield from stationtypes where hitpointsshield < 0
select @cErrors = COUNT(*) from shields where (shields.hitpoints < 0)
if (@cErrors > 0)
	select parts.partid, name, shields.hitpoints from parts, shields where (parts.partid = shields.partid) AND (shields.hitpoints < 0)
go


/*
    Price Test
*/
declare @cErrors    int

select @cErrors = COUNT(*) from developments where (price <= 0)
if (@cErrors > 0)
    select DevelopmentID, Name, Price from developments where (price <= 0)

select @cErrors = COUNT(*) from drones where (price <= 0) and (DroneType <> 6)
if (@cErrors > 0)
    select DroneID, Name, Price from drones where (price <= 0) and (DroneType <> 6)

select @cErrors = COUNT(*) from expendables where (price <= 0)
if (@cErrors > 0)
    select ExpendableId, Name, Price from expendables where (price <= 0)

select @cErrors = COUNT(*) from parts where (price <= 0)
if (@cErrors > 0)
    select PartID, Name, Price from parts where (price <= 0)

select @cErrors = COUNT(*) from shiptypes where (price <= 0) and (shiptypeid not in (select escapepodshiptypeid from civs))
if (@cErrors > 0)
    select ShipTypeID, Name, Price from shiptypes where (price <= 0) and (shiptypeid not in (select escapepodshiptypeid from civs))

select @cErrors = COUNT(*) from stationtypes where (price <= 0)
if (@cErrors > 0)
    select StationTypeID, Name, Price from stationtypes where (price <= 0)
go


/*
    Drone Test
*/
declare @cErrors    int

select @cErrors = COUNT(*) from drones where (dronetype > 6) or (dronetype = 5) or (dronetype < 0)
if (@cErrors > 0)
    select Name, dronetype from drones where (dronetype > 6) or (dronetype = 5) or (dronetype < 0)

select @cErrors = COUNT(*) from drones, parts p1, parts p2, shiptypes where (drones.shiptypeid = shiptypes.shiptypeid) AND (p1.partid = drones.PartID_Weapon) AND (p2.partid = drones.PartID_Shield) AND (p1.drain + p2.drain > shiptypes.capacity)
if (@cErrors > 0)
    select drones.droneid, drones.name, p1.name, p1.drain, p2.name, p2.drain, shiptypes.capacity from drones, parts p1, parts p2, shiptypes where (drones.shiptypeid = shiptypes.shiptypeid) AND (p1.partid = drones.PartID_Weapon) AND (p2.partid = drones.PartID_Shield) AND (p1.drain + p2.drain > shiptypes.capacity)


go


/*
    Mine Test
*/
declare @cErrors    int

select @cErrors = COUNT(*) from mines where (munitioncount > 50)
if (@cErrors > 0)
    select expendableid, munitioncount from mines where (munitioncount > 50)
go


/*
	Projectile Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from projectiles where (timeduration < 250)
if (@cErrors > 0)
	select projectileid, timeduration from projectiles where (timeduration < 250)
select @cErrors = COUNT(*) from projectiles where (speedmax < 50) OR (speedmax > 1000)
if (@cErrors > 0)
	select projectileid, speedmax from projectiles where (speedmax < 50) OR (speedmax > 1000)
select @cErrors = COUNT(*) from projectiles where (hitpointsinflict < 0)
if (@cErrors > 0)
	select projectileid, hitpointsinflict from projectiles where (hitpointsinflict < 0)
go


/*
	Shield Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from shields where (regenrate < 0)
if (@cErrors > 0)
	select parts.partid, name, regenrate from parts, shields where (parts.partid = shields.partid) AND (regenrate < 0)
select @cErrors = COUNT(*) from shields where (percentreflectparticle < 0) OR (percentreflectparticle > 100)
if (@cErrors > 0)
	select parts.partid, name, percentreflectparticle from parts, shields where (parts.partid = shields.partid) AND ((percentreflectparticle < 0) OR (percentreflectparticle > 100))
select @cErrors = COUNT(*) from shields where (percentreflectenergy   < 0) OR (percentreflectenergy   > 100)
if (@cErrors > 0)
	select parts.partid, name, percentreflectenergy from parts, shields where (parts.partid = shields.partid) AND ((percentreflectenergy   < 0) OR (percentreflectenergy   > 100))
select @cErrors = COUNT(*) from shields where (percentleakageparticle < 0) OR (percentleakageparticle > 100)
if (@cErrors > 0)
	select parts.partid, name, percentleakageparticle from parts, shields where (parts.partid = shields.partid) AND ((percentleakageparticle < 0) OR (percentleakageparticle > 100))
select @cErrors = COUNT(*) from shields where (percentleakageenergy   < 0) OR (percentleakageenergy   > 100)
if (@cErrors > 0)
	select parts.partid, name, percentleakageenergy from parts, shields where (parts.partid = shields.partid) AND ((percentleakageenergy   < 0) OR (percentleakageenergy   > 100))
go


/*
	Ship Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from shiptypes where
	(length * length / 4) < (cockpitx * cockpitx) + (cockpity * cockpity) + (cockpitz * cockpitz)
if (@cErrors > 0)
	select shiptypeid, name, length / 2, cockpitx, cockpity, cockpitz
		from shiptypes where (length * length / 4) < (cockpitx * cockpitx) + (cockpity * cockpity) + (cockpitz * cockpitz)

select @cErrors = COUNT(*) from shiptypes where
    (shipclassid < 0) OR (shipclassid > 8)
if (@cErrors > 0)
    select shiptypeid, name, shipclassid from shiptypes where
        (shipclassid < 0) OR (shipclassid > 8)

/*
    AttachPoint test (this one's tricky).
*/
select COUNT(*) as 'AttachPoints' into #bvtattachpt from attachpoints group by shiptypeid, parttypeid having COUNT(*) > 4
select @cErrors = COUNT(*) from #bvtattachpt
if (@cErrors > 0)
    select shiptypeid, parttypeid, COUNT(*) as 'AttachPoints' from attachpoints group by shiptypeid, parttypeid having COUNT(*) > 4
drop table #bvtattachpt

go


/*
	Signature Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from shiptypes where (basesignature < 50)
if (@cErrors > 0)
	select shiptypeid, name, basesignature from shiptypes where (basesignature < 50)
go


/*
	Station Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from stationtypes where (rateregenarmor < 0)
if (@cErrors > 0)
	select stationtypeid, name, rateregenarmor from stationtypes where (rateregenarmor < 0)

select @cErrors = COUNT(*) from stationtypes where (rateregenshield < 0)
if (@cErrors > 0)
	select stationtypeid, name, rateregenshield from stationtypes where (rateregenshield < 0)

select @cErrors = COUNT(*) from stationtypes where (UpgradeStationTypeID >= stationtypeid)
if (@cErrors > 0)
    select stationtypeid, name, upgradestationtypeid from stationtypes where (UpgradeStationTypeID >= stationtypeid)

select @cErrors = COUNT(*) from stationtypes where (upgradeprice = 0) AND (UpgradeName IS NOT NULL)
if (@cErrors > 0)
    select stationtypeid, name, upgradeprice, upgradename from stationtypes where (upgradeprice = 0) AND (UpgradeName IS NOT NULL)

select @cErrors = COUNT(*) from stationtypes where (capabilities % 2 = 1) and ((capabilities / 32) % 2 = 0)
if (@cErrors > 0)
    select stationtypeid, name, capabilities from stationtypes where (capabilities % 2 = 1) and ((capabilities / 32) % 2 = 0)

go


/*
    Station Upgrade Test
*/

declare     @iIndex         int
declare     @siStationType      smallint
declare     @siUpgradeType      smallint
declare     @szStationName      varchar(20)
declare     @szUpgradeName      varchar(20)
declare     @szStationEffect    TechBits
declare     @szUpgradeEffect    TechBits
declare     @iStationEffect     int
declare     @iUpgradeEffect     int
declare     @szStationLocal     TechBits
declare     @szUpgradeLocal     TechBits
declare     @iStationLocal      int
declare     @iUpgradeLocal      int

declare     curStations     scroll cursor for
                    select st1.stationtypeid, st2.stationtypeid,
                    RTRIM(st1.name), RTRIM(st2.name),
                    st1.techbitseffect, st2.techbitseffect,
                    st1.techbitslocal, st2.techbitslocal
                    from
                    stationtypes st1, stationtypes st2 where
                    (st1.upgradestationtypeid = st2.stationtypeid)
                    order by st1.stationtypeid

open        curStations                 

fetch next from curStations into @siStationType, 
                @siUpgradeType,
                @szStationName,
                @szUpgradeName,
                @szStationEffect,
                @szUpgradeEffect,
                @szStationLocal,
                @szUpgradeLocal
while (@@fetch_status <> -1)
begin
    if (@@fetch_status <> -2)
    begin
        /*
            First check the effect bits.
        */
        select @iIndex = 1
        while (@iIndex <= datalength(@szStationEffect))
        begin
            /*
                Pull off one character at a time and compare the
                two bitmasks.  Note that ASCII 0 == 48,
                ASCII A == 65, and ASCII a == 97.
            */
            select @iStationEffect =
                ASCII(SUBSTRING(@szStationEffect, @iIndex, 1)) - 48
            if (@iStationEffect > 9)
                select @iStationEffect = @iStationEffect - 7
            if (@iStationEffect > 15)
                select @iStationEffect = @iStationEffect - 32
            select @iUpgradeEffect =
                ASCII(SUBSTRING(@szUpgradeEffect, @iIndex, 1)) - 48
            if (@iUpgradeEffect > 9)
                select @iUpgradeEffect = @iUpgradeEffect - 7
            if (@iUpgradeEffect > 15)
                select @iUpgradeEffect = @iUpgradeEffect - 32

            if (@iStationEffect & @iUpgradeEffect <> @iStationEffect)
            begin
                select "Warning: Illegal upgrade from " +
                    @szStationName + " to " + @szUpgradeName +
                    " (effect)."
                select @iIndex = datalength(@szUpgradeEffect)
            end
            select @iIndex = @iIndex + 1
        end

        /*
            Now check the local bits.
        */
        select @iIndex = 1
        while (@iIndex <= datalength(@szStationLocal))
        begin
            /*
                Pull off one character at a time and compare the
                two bitmasks.  Note that ASCII 0 == 48,
                ASCII A == 65, and ASCII a == 97.
            */
            select @iStationLocal =
                ASCII(SUBSTRING(@szStationLocal, @iIndex, 1)) - 48
            if (@iStationLocal > 9)
                select @iStationLocal = @iStationLocal - 7
            if (@iStationEffect > 15)
                select @iStationLocal = @iStationLocal - 32
            select @iUpgradeLocal =
                ASCII(SUBSTRING(@szUpgradeLocal, @iIndex, 1)) - 48
            if (@iUpgradeLocal > 9)
                select @iUpgradeLocal = @iUpgradeLocal - 7
            if (@iUpgradeLocal > 15)
                select @iUpgradeLocal = @iUpgradeLocal - 32

            if (@iStationLocal & @iUpgradeLocal <> @iStationLocal)
            begin
                select "Warning: Illegal upgrade from " +
                    @szStationName + " to " + @szUpgradeName +
                    " (local)."
                select @iIndex = datalength(@szUpgradeLocal)
            end
            select @iIndex = @iIndex + 1
        end
    end
    fetch next from curStations into @siStationType,
                    @siUpgradeType,
                    @szStationName,
                    @szUpgradeName,
                    @szStationEffect,
                    @szUpgradeEffect,
                    @szStationLocal,
                    @szUpgradeLocal
end


close       curStations
deallocate  curStations
go


/*
    Tech Tree Test
*/
declare @cErrors    int

select @cErrors = COUNT(*) from civs where techbitsciv like '#%'
if (@cErrors > 0)
    select Name, techbitsciv from civs where techbitsciv like '#%'

select @cErrors = COUNT(*) from developments where techbitsrequired like '#%'
if (@cErrors > 0)
    select Name, techbitsrequired from developments where techbitsrequired like '#%'
select @cErrors = COUNT(*) from developments where techbitseffect like '#%'
if (@cErrors > 0)
    select Name, techbitseffect from developments where techbitseffect like '#%'

select @cErrors = COUNT(*) from drones where techbitsrequired like '#%'
if (@cErrors > 0)
    select Name, techbitsrequired from drones where techbitsrequired like '#%'
select @cErrors = COUNT(*) from drones where techbitseffect like '#%'
if (@cErrors > 0)
    select Name, techbitseffect from drones where techbitseffect like '#%'

select @cErrors = COUNT(*) from expendables where techbitsrequired like '#%'
if (@cErrors > 0)
    select Name, techbitsrequired from expendables where techbitsrequired like '#%'
select @cErrors = COUNT(*) from expendables where techbitseffect like '#%'
if (@cErrors > 0)
    select Name, techbitseffect from expendables where techbitseffect like '#%'

select @cErrors = COUNT(*) from parts where techbitsrequired like '#%'
if (@cErrors > 0)
    select Name, techbitsrequired from parts where techbitsrequired like '#%'
select @cErrors = COUNT(*) from parts where techbitseffect like '#%'
if (@cErrors > 0)
    select Name, techbitseffect from parts where techbitseffect like '#%'

select @cErrors = COUNT(*) from shiptypes where techbitsrequired like '#%'
if (@cErrors > 0)
    select Name, techbitsrequired from shiptypes where techbitsrequired like '#%'
select @cErrors = COUNT(*) from shiptypes where techbitseffect like '#%'
if (@cErrors > 0)
    select Name, techbitseffect from shiptypes where techbitseffect like '#%'

select @cErrors = COUNT(*) from stationtypes where techbitslocal like '#%'
if (@cErrors > 0)
    select Name, techbitslocal from stationtypes where techbitslocal like '#%'
select @cErrors = COUNT(*) from stationtypes where techbitsrequired like '#%'
if (@cErrors > 0)
    select Name, techbitsrequired from stationtypes where techbitsrequired like '#%'
select @cErrors = COUNT(*) from stationtypes where techbitseffect like '#%'
if (@cErrors > 0)
    select Name, techbitseffect from stationtypes where techbitseffect like '#%'

go


/*
	Weapon Test
*/
declare @cErrors	int

select @cErrors = COUNT(*) from weapons where (cBulletsPerShot <= 0) AND (energypershot <= 0)
if (@cErrors > 0)
	select parts.partid, name, cBulletsPerShot from parts, weapons where (parts.partid = weapons.partid) AND (cBulletsPerShot <= 0) AND (energypershot <= 0)
select @cErrors = COUNT(*) from weapons where (dispersion > 0.05)
if (@cErrors > 0)
    select parts.partid, name, dispersion from parts, weapons where
        (parts.partid = weapons.partid) AND (dispersion > 0.05)

select @cErrors = COUNT(*) from weapons, projectiles where (weapons.projectileid1 = projectiles.projectileid) AND (weapons.energypershot = 0) AND (projectiles.damagetype % 2 = 1)
if (@cErrors > 0)
    select parts.name, projectileid, energypershot, cbulletspershot, damagetype from parts, weapons, projectiles where (parts.partid = weapons.partid) AND (weapons.projectileid1 = projectiles.projectileID) AND (weapons.energypershot = 0) and (projectiles.damagetype % 2 = 1)

select @cErrors = COUNT(*) from weapons, projectiles where (weapons.projectileid1 = projectiles.projectileid) AND (weapons.cbulletspershot = 0) AND (projectiles.damagetype % 2 = 0)
if (@cErrors > 0)
    select parts.name, projectileid, energypershot, cbulletspershot, damagetype from parts, weapons, projectiles where (parts.partid = weapons.partid) AND (weapons.projectileid1 = projectiles.projectileID) AND (weapons.cbulletspershot = 0) AND (projectiles.damagetype % 2 = 0)

go



declare @cWeapons		int
declare @cWeaponParts		int
declare @cShields		int
declare @cShieldParts		int
declare @cAfterburners		int
declare @cAfterburnerParts	int
declare @cAmmo			int
declare @cAmmoParts		int

select @cWeapons = COUNT(*) from weapons
select @cWeaponParts = COUNT(*) from parts where parttypeid = 1
select @cShields = COUNT(*) from shields
select @cShieldParts = COUNT(*) from parts where parttypeid = 4
select @cAfterburners = COUNT(*) from afterburners
select @cAfterburnerParts = COUNT(*) from parts where parttypeid = 7
select @cAmmo = COUNT(*) from ammo
select @cAmmoParts = COUNT(*) from parts where parttypeid = 6

if (@cWeapons <> @cWeaponParts)
BEGIN
	select "ERROR: Wrong number of weapons (" + 
		CONVERT(varchar, @cWeapons) + " vs " + 
		CONVERT(varchar, @cWeaponParts) + ")"
END

if (@cShields <> @cShieldParts)
BEGIN
	select "ERROR: Wrong number of shields (" + 
		CONVERT(varchar, @cShields) + " vs " + 
		CONVERT(varchar, @cShieldParts) + ")"
END

if (@cAfterburners <> @cAfterburnerParts)
BEGIN
	select "ERROR: Wrong number of afterburners (" + 
		CONVERT(varchar, @cAfterburners) + " vs " + 
		CONVERT(varchar, @cAfterburnerParts) + ")"
END

if (@cAmmo <> @cAmmoParts)
BEGIN
	select "ERROR: Wrong number of ammo (" + 
		CONVERT(varchar, @cAmmo) + " vs " + 
		CONVERT(varchar, @cAmmoParts) + ")"
END
