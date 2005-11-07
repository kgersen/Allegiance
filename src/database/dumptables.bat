@echo off
if /i "%1" == "in" goto direction_ok
if /i "%1" NEQ "out" goto usage

:direction_ok
set feddb=federation
set feddbsrv=
set feddbpw=

rem check for specified database
if /i "%2"=="/d" set feddb=%3%
if /i "%4"=="/d" set feddb=%5%
if /i "%6"=="/d" set feddb=%7%

rem check for specified server
if /i "%2"=="/s" set feddbsrv=/S%3%
if /i "%4"=="/s" set feddbsrv=/S%5%
if /i "%6"=="/s" set feddbsrv=/S%7%

rem check for specified password
if /i "%2"=="/p" set feddbpw=%3%
if /i "%4"=="/p" set feddbpw=%5%
if /i "%6"=="/p" set feddbpw=%7%

rem ************************************************************
rem This list is sorted in dependancy order. 
rem I.e. table X comes after all tables which table X depends on
rem ************************************************************
echo Dumping Effects
bcp %feddb%..Effects          %1 Effects.dat          /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping PartTypes
bcp %feddb%..PartTypes        %1 PartTypes.dat        /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Parts
bcp %feddb%..Parts            %1 Parts.dat            /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Expendables
bcp %feddb%..Expendables      %1 Expendables.dat      /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Afterburners
bcp %feddb%..Afterburners     %1 Afterburners.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Maps
bcp %feddb%..Maps             %1 Maps.dat             /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping SectorInfo
bcp %feddb%..SectorInfo       %1 SectorInfo.dat       /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping AlephInstances
bcp %feddb%..AlephInstances   %1 AlephInstances.dat   /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping ShipTypes
bcp %feddb%..ShipTypes        %1 ShipTypes.dat        /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping AttachPoints
bcp %feddb%..AttachPoints     %1 AttachPoints.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping CharacterInfo
bcp %feddb%..CharacterInfo    %1 CharacterInfo.dat    /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Civs
bcp %feddb%..Civs             %1 Civs.dat             /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping StationTypes
bcp %feddb%..StationTypes     %1 StationTypes.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Asteroids
bcp %feddb%..Asteroids        %1 Asteroids.dat        /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping PosterInstances
bcp %feddb%..PosterInstances  %1 PosterInstances.dat  /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Projectiles
bcp %feddb%..Projectiles      %1 Projectiles.dat      /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Shields
bcp %feddb%..Shields          %1 Shields.dat          /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Weapons
bcp %feddb%..Weapons          %1 Weapons.dat          /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Ammo
bcp %feddb%..Ammo             %1 Ammo.dat             /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Drones
bcp %feddb%..Drones           %1 Drones.dat           /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Missiles
bcp %feddb%..Missiles         %1 Missiles.dat         /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Magazines
bcp %feddb%..Magazines        %1 Magazines.dat        /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Developments
bcp %feddb%..Developments     %1 Developments.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping GlobalAttributes
bcp %feddb%..GlobalAttributes %1 GlobalAttributes.dat /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping TechBits
bcp %feddb%..TechBits         %1 TechBits.dat         /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping StationClasses
bcp %feddb%..StationClasses   %1 StationClasses.dat   /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping ShipClasses
bcp %feddb%..ShipClasses      %1 ShipClasses.dat      /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Cloaks
bcp %feddb%..Cloaks           %1 Cloaks.dat           /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Probes
bcp %feddb%..Probes           %1 Probes.dat           /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Mines
bcp %feddb%..Mines            %1 Mines.dat            /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping MineInstances
bcp %feddb%..MineInstances    %1 MineInstances.dat    /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping TreasureSets
bcp %feddb%..TreasureSets     %1 TreasureSets.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping TreasureChances
bcp %feddb%..TreasureChances  %1 TreasureChances.dat  /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping TreasureInstances
bcp %feddb%..TreasureInstances %1 TreasureInstances.dat /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Chaff
bcp %feddb%..Chaff            %1 Chaff.dat            /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping StationInstances
bcp %feddb%..StationInstances %1 StationInstances.dat /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping ProbeInstances
bcp %feddb%..ProbeInstances %1 ProbeInstances.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping CharMedals	
bcp %feddb%..CharMedals       %1 CharMedals.dat       /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping CharStats
bcp %feddb%..CharStats        %1 CharStats.dat        /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Medals
bcp %feddb%..Medals           %1 Medals.dat           /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Ranks
bcp %feddb%..Ranks            %1 Ranks.dat            /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Constants
bcp %feddb%..Constants        %1 Constants.dat        /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping AccessCodes
bcp %feddb%..AccessCodes      %1 AccessCodes.dat      /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping AccessCodeUsage
bcp %feddb%..AccessCodeUsage  %1 AccessCodeUsage.dat  /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping PSSInstances
bcp %feddb%..PSSInstances     %1 PSSInstances.dat     /c /E /Usa %feddbsrv% /P%feddbpw%
echo Dumping Squads
bcp %feddb%..Squads           %1 Squads.dat           /c /E /Usa %feddbsrv% /P%feddbpw%

goto end

:usage
rem  12345678911234567892123456789312345678941234567895123456789612345678971234567898
echo . 
echo Usage:   dumptables Direction [/S Server] [/P Password] [/D Database]
echo . 
echo Where:   Direction is {in^|out} (in = from file to database, out = from
echo              database to file).
echo          Server is machine of database you want to use, so you can use
echo              dumptables from a machine other than the one the database is on
echo          Password is the password for the sa account
echo          Database is the name of the SQL database to use
echo . 
echo Notes:   * You must leave a space between all switches and their values
echo          * All parameters in brackets are optional, and may appear in any order 
echo          * Password only matters if server doesn't use trusted connections
echo . 
echo Example: dumptables in /S fedsrv /P mypassword
echo . 

:end
