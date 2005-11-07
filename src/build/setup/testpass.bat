
if "%4"=="" goto Usage

type %SCRIPTDIR%\setup\testpass.sed | sed s/BUILDNUM/%1/ | sed s/TODAYSDATE/'%2'/ > %TEMP%\testpass.sql
isql.exe -S %3 -E -d %4 -n -i %TEMP%\testpass.sql

goto End


:Usage
echo Usage: testpass.bat [build number] [today's date] [SQL server] [SQL database]
goto End


:End



