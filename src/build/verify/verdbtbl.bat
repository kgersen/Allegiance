
if "%3"=="" goto Usage

set DBTABLETEMP=%TEMP%\verdbtbl.err
set DBTABLELOG=%LOGDIR%\verdbtbl.err
if exist %DBTABLETEMP% del %DBTABLETEMP%

rem
rem The first step is to make sure we have one or more records in
rem every table.
rem
set ISQL=isql.exe -S %2 -E -d %3 -n -e
set QUERY=-Q "select COUNT(*) from %%t"

for %%t in ( afterburners, alephdef, alephinstances, ammo, attachpoints, characterinfo, civs, developments drones, effects ) DO %ISQL% %QUERY% >> %DBTABLETEMP%
for %%t in ( globalattributes, magazines, missiles, objecttype, parts, parttypes, planetinstances, posterinstances ) DO %ISQL% %QUERY% >> %DBTABLETEMP%
for %%t in ( projectiles, sectorinfo, shields, shipclasses, shiptypes, stationclasses, stationtypes, weapons ) DO %ISQL% %QUERY% >> %DBTABLETEMP%

type %DBTABLETEMP% | awk -f %SCRIPTDIR%\verify\verdbtbl.awk > %DBTABLELOG%

rem
rem The filesize should be zero if everything went well.
rem
call %SCRIPTDIR%\util\filesize.bat %DBTABLELOG% FILESIZE
if "%FILESIZE%"=="0" goto Success
set BUILDERROR=1
goto AfterCheck


:Success
echo No errors found. > %DBTABLELOG%
goto AfterCheck


:AfterCheck
set DBTABLETEMP=
set DBTABLELOG=
set ISQL=
set QUERY=
set FILESIZE=
goto End


:Usage
echo Usage: verdbbat [Build] [SQL Server] [SQL Database]
set BUILDERROR=1
goto End


:End


