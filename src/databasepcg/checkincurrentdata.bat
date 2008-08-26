@echo off
if "%1" == "/?" goto usage
if "%1" == "-?" goto usage
if "%1" == "" goto usage
if "%2" == "" goto usage

set Server=

if NOT "%1"=="" set Server=/s %1
if NOT "%2"=="" set Database=/d %2

out Afterburners.dat
out Ammo.dat
out AttachPoints.dat
out AttachPointTypes.dat
out chaff.dat
out Civs.dat
out cloaks.dat
out developments.dat
out drones.dat
out Effects.dat
out expendables.dat
out globalattributes.dat
out magazines.dat
out mines.dat
out missiles.dat
out Parts.dat
out PartTypes.dat
out probes.dat
out Projectiles.dat
out Shields.dat
out shipclasses.dat
out ShipTypes.dat
out stationclasses.dat
out stationtypes.dat
out techbits.dat
out treasurechances.dat
out treasuresets.dat
out turrets.dat
out Weapons.dat
call dumptables out %Server% %Database%
in -ofc "Current data"
pause
goto end

:usage
rem  12345678911234567892123456789312345678941234567895123456789612345678971234567898
echo Description: captures the data in the database and checks it in
echo. 
echo Usage:   CheckinCurrentData [Server [Database]]
echo. 
echo Where:   Server is machine of database you want to use, so you can use
echo              from a machine other than the one the database is on.
echo          Database is the name of the SQL database to use on the server specified
echo. 
echo Example: CheckinCurrentData OBLSQL Federation
echo. 

:end
