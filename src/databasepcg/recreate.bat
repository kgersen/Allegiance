@echo off
if "%1" == "/?" goto usage
if "%1" == "-?" goto usage

set rcfeddb=Federation
set rcfeddbsrv=
set rcfeddbpw=/P
set rcfedupgrade=N
set sqlfile="%temp%\recreate.sql"
set altdir=.

rem check for alternate data set
if /i "%1"=="/a" set altdir=%2%
if /i "%2"=="/a" set altdir=%3%
if /i "%3"=="/a" set altdir=%4%
if /i "%4"=="/a" set altdir=%5%
if /i "%5"=="/a" set altdir=%6%
if /i "%6"=="/a" set altdir=%7%
if /i "%7"=="/a" set altdir=%8%
if /i "%8"=="/a" set altdir=%9%

rem check for specified database
if /i "%1"=="/d" set rcfeddb=%2%
if /i "%2"=="/d" set rcfeddb=%3%
if /i "%3"=="/d" set rcfeddb=%4%
if /i "%4"=="/d" set rcfeddb=%5%
if /i "%5"=="/d" set rcfeddb=%6%
if /i "%6"=="/d" set rcfeddb=%7%
if /i "%7"=="/d" set rcfeddb=%8%
if /i "%8"=="/d" set rcfeddb=%9%

rem check for specified server
if /i "%1"=="/s" set rcfeddbsrv=/S %2%
if /i "%2"=="/s" set rcfeddbsrv=/S %3%
if /i "%3"=="/s" set rcfeddbsrv=/S %4%
if /i "%4"=="/s" set rcfeddbsrv=/S %5%
if /i "%5"=="/s" set rcfeddbsrv=/S %6%
if /i "%6"=="/s" set rcfeddbsrv=/S %7%
if /i "%7"=="/s" set rcfeddbsrv=/S %8%
if /i "%8"=="/s" set rcfeddbsrv=/S %9%

rem check for specified password
if /i "%1"=="/p" set rcfeddbpw=/P %2%
if /i "%2"=="/p" set rcfeddbpw=/P %3%
if /i "%3"=="/p" set rcfeddbpw=/P %4%
if /i "%4"=="/p" set rcfeddbpw=/P %5%
if /i "%5"=="/p" set rcfeddbpw=/P %6%
if /i "%6"=="/p" set rcfeddbpw=/P %7%
if /i "%7"=="/p" set rcfeddbpw=/P %8%
if /i "%8"=="/p" set rcfeddbpw=/P %9%

rem in sql7, bcp doesn't like /P when the password is blank, but osql does--go figure
set rcfedbcppw=%rcfeddbpw%
if /i "%rcfeddbpw%"=="/P" set rcfedbcppw=

rem check for /U
if /i "%1"=="/U" set rcfedupgrade=Y
if /i "%2"=="/U" set rcfedupgrade=Y
if /i "%3"=="/U" set rcfedupgrade=Y
if /i "%4"=="/U" set rcfedupgrade=Y
if /i "%5"=="/U" set rcfedupgrade=Y
if /i "%6"=="/U" set rcfedupgrade=Y
if /i "%7"=="/U" set rcfedupgrade=Y
if /i "%8"=="/U" set rcfedupgrade=Y
if /i "%9"=="/U" set rcfedupgrade=Y

rem make a backup of the dynamic stuff
if "%rcfedupgrade%" == "N" goto recreate
echo bcp %rcfeddb%..CharacterInfo out %temp%\CharacterInfo.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..CharacterInfo out %temp%\CharacterInfo.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..CharStats     out %temp%\CharStats.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..CharStats     out %temp%\CharStats.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..CharMedals    out %temp%\CharMedals.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..CharMedals    out %temp%\CharMedals.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..Medals        out %temp%\Medals.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Medals        out %temp%\Medals.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..Squads        out %temp%\Squads.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Squads        out %temp%\Squads.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..AccessCodes   out %temp%\AccessCodes.dat   /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..AccessCodes   out %temp%\AccessCodes.dat   /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..AccessCodeUsage out %temp%\AccessCodeUsage.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..AccessCodeUsage out %temp%\AccessCodeUsage.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..PSSInstances  out %temp%\PSSInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..PSSInstances  out %temp%\PSSInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%

rem Map data
echo bcp %rcfeddb%..Maps             out %temp%\Maps.dat              /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Maps             out %temp%\Maps.dat              /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..SectorInfo       out %temp%\SectorInfo.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..SectorInfo       out %temp%\SectorInfo.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..Asteroids        out %temp%\Asteroids.dat         /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Asteroids        out %temp%\Asteroids.dat         /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..AlephInstances   out %temp%\AlephInstances.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..AlephInstances   out %temp%\AlephInstances.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..StationInstances out %temp%\StationInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..StationInstances out %temp%\StationInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..MineInstances    out %temp%\MineInstances.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..MineInstances    out %temp%\MineInstances.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..TreasureInstances out %temp%\TreasureInstances.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..TreasureInstances out %temp%\TreasureInstances.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%

rem Game Results data
echo bcp %rcfeddb%..GameResults      out %temp%\GameResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..GameResults      out %temp%\GameResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..TeamResults      out %temp%\TeamResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..TeamResults      out %temp%\TeamResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..PlayerResults    out %temp%\PlayerResults.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..PlayerResults    out %temp%\PlayerResults.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%


:recreate
echo declare @myerror as integer      	> %sqlfile%
echo Drop Database %rcfeddb%      	>> %sqlfile%
echo select @myerror=@@ERROR            >> %sqlfile%
echo if @myerror=3701 select @myerror=0 >> %sqlfile%
echo EXIT(select @myerror)              >> %sqlfile%
echo go 				>> %sqlfile%
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /i %sqlfile%
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /i %sqlfile%
if ERRORLEVEL 1 goto failure

echo Create Database %rcfeddb% 		> %sqlfile%
echo go 				>> %sqlfile%
echo USE %rcfeddb% 			>> %sqlfile%
echo go 				>> %sqlfile%
echo if not exists (select * from master..syslogins where name = N'ALServer') >> %sqlfile%
echo   exec sp_addlogin 'ALServer', 'oblivion', '%rcfeddb%' >> %sqlfile%
echo go 				>> %sqlfile%
echo if not exists (select * from sysusers where name = N'ALServer' and uid ^< 16382) >> %sqlfile%
echo   EXEC sp_grantdbaccess 'ALServer' >> %sqlfile%
echo go 				>> %sqlfile%
echo exec sp_addrolemember N'db_owner', N'ALServer' >> %sqlfile%
echo go 				>> %sqlfile%
copy /b %sqlfile%+fed-all.sql %sqlfile%
osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /i %sqlfile%
if ERRORLEVEL 1 goto failure

if "%altdir%" NEQ "." copy dumptables.bat %altdir%
if "%altdir%" NEQ "." cd %altdir%
echo call dumptables in %rcfeddbsrv% %rcfedbcppw% /D %rcfeddb%
call dumptables in %rcfeddbsrv% %rcfedbcppw% /D %rcfeddb%
if "%altdir%" NEQ "." cd ..

if "%rcfedupgrade%" == "N" goto success
rem Delete tables that we're going to restore
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..CharStats"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..CharStats"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..CharacterInfo"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..CharacterInfo"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..CharMedals"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..CharMedals"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Medals"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Medals"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Squads"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Squads"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..AccessCodes"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..AccessCodes"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..AccessCodeUsage"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..AccessCodeUsage"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..PSSInstances"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..PSSInstances"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Asteroids"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Asteroids"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..AlephInstances"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..AlephInstances"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..StationInstances"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..StationInstances"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..MineInstances"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..MineInstances"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..TreasureInstances"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..TreasureInstances"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..SectorInfo"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..SectorInfo"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..Maps"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..maps"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..GameResults"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..GameResults"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..TeamResults"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..TeamResults"
echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..PlayerResults"
     osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "Delete %rcfeddb%..PlayerResults"


rem Do the restore
echo bcp %rcfeddb%..CharacterInfo in %temp%\CharacterInfo.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..CharacterInfo in %temp%\CharacterInfo.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..CharStats     in %temp%\CharStats.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..CharStats     in %temp%\CharStats.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..Medals        in %temp%\Medals.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Medals        in %temp%\Medals.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..CharMedals    in %temp%\CharMedals.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..CharMedals    in %temp%\CharMedals.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..Squads        in %temp%\Squads.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Squads        in %temp%\Squads.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..AccessCodes   in %temp%\AccessCodes.dat   /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..AccessCodes   in %temp%\AccessCodes.dat   /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..AccessCodeUsage in %temp%\AccessCodeUsage.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..AccessCodeUsage in %temp%\AccessCodeUsage.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..PSSInstances  in %temp%\PSSInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..PSSInstances  in %temp%\PSSInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%

rem Map data
echo bcp %rcfeddb%..Maps             in %temp%\Maps.dat              /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Maps             in %temp%\Maps.dat              /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..SectorInfo       in %temp%\SectorInfo.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..SectorInfo       in %temp%\SectorInfo.dat        /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..Asteroids        in %temp%\Asteroids.dat         /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..Asteroids        in %temp%\Asteroids.dat         /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..AlephInstances   in %temp%\AlephInstances.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..AlephInstances   in %temp%\AlephInstances.dat    /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..StationInstances in %temp%\StationInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..StationInstances in %temp%\StationInstances.dat  /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..MineInstances    in %temp%\MineInstances.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..MineInstances    in %temp%\MineInstances.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..TreasureInstances in %temp%\TreasureInstances.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..TreasureInstances in %temp%\TreasureInstances.dat /c /E /Usa %rcfeddbsrv% %rcfeddbpw%

rem Game Results data
echo bcp %rcfeddb%..GameResults      in %temp%\GameResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..GameResults      in %temp%\GameResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..TeamResults      in %temp%\TeamResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..TeamResults      in %temp%\TeamResults.dat       /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
echo bcp %rcfeddb%..PlayerResults    in %temp%\PlayerResults.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%
     bcp %rcfeddb%..PlayerResults    in %temp%\PlayerResults.dat     /c /E /Usa %rcfeddbsrv% %rcfeddbpw%

echo osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "exec %rcfeddb%..SyncZoneSquads"
osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /Q "exec %rcfeddb%..SyncZoneSquads"
rem leave %temp%\*.dat so they can compare
goto success

:failure
echo !!! RECREATE FAILED !!!
goto end

:success
echo !!! RECREATE SUCCEEDED !!!
goto end

:usage
rem  12345678911234567892123456789312345678941234567895123456789612345678971234567898
echo Description: nukes and recreates database schema and repopulates it with data
echo. 
echo Usage:   recreate [/A AltDir] [/S Server] [/P Password] [/D Database] [/L] [/U]
echo. 
echo Where:   AltDir is a subdirectory where the dat files to be loaded exist. The
echo              subdirectory MUST be directly off of this directory.
echo          Server is machine of database you want to use, so you can use
echo              dumptables from a machine other than the one the database is on
echo          Password is the password for the sa account
echo          Database is the name of the SQL database to use
echo          /L causes the log to be preserved. Default is to dump the log prior to
echo              nuking and recreating the database.
echo          /U Upgrade only. Preserves existing CharacterInfo table. Obviously
echo              there must not be any changes to the schema for that table.
echo              This will result in the creation of a file called 
echo              CharacterInfoUpg.dat which will be left for comparison, etc.
echo. 
echo Notes:   * You must leave a space between all switches and their values
echo          * All parameters in brackets are optional, and may appear in any order
echo          * Password only matters if server doesn't use trusted connections
echo. 
echo Example: recreate /S fedsrv /P mypassword
echo. 

:end
set rcfeddb=
set rcfeddbsrv=
set rcfeddbpw=
set rcfeddbdumplog=
set rcfedupgrade=
set rcfedbcppw=
set sqlfile=
set charupg=
set altdir=