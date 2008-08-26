@echo off
if "%1" == "" goto usage
if "%1" == "/?" goto usage
if "%1" == "-?" goto usage

set rcfeddb=Federation
set rcfeddbsrv=
set rcfeddbpw=/P
set sqlfile="%temp%\Events.sql"

rem check for specified database
if /i "%1"=="/d" set rcfeddb=%2%
if /i "%2"=="/d" set rcfeddb=%3%
if /i "%3"=="/d" set rcfeddb=%4%
if /i "%4"=="/d" set rcfeddb=%5%
if /i "%5"=="/d" set rcfeddb=%6%
if /i "%6"=="/d" set rcfeddb=%7%
if /i "%7"=="/d" set rcfeddb=%8%

rem check for specified server
if /i "%1"=="/s" set rcfeddbsrv=/S %2%
if /i "%2"=="/s" set rcfeddbsrv=/S %3%
if /i "%3"=="/s" set rcfeddbsrv=/S %4%
if /i "%4"=="/s" set rcfeddbsrv=/S %5%
if /i "%5"=="/s" set rcfeddbsrv=/S %6%
if /i "%6"=="/s" set rcfeddbsrv=/S %7%
if /i "%7"=="/s" set rcfeddbsrv=/S %8%

rem check for specified password
if /i "%1"=="/p" set rcfeddbpw=/P %2%
if /i "%2"=="/p" set rcfeddbpw=/P %3%
if /i "%3"=="/p" set rcfeddbpw=/P %4%
if /i "%4"=="/p" set rcfeddbpw=/P %5%
if /i "%5"=="/p" set rcfeddbpw=/P %6%
if /i "%6"=="/p" set rcfeddbpw=/P %7%
if /i "%7"=="/p" set rcfeddbpw=/P %8%

rem in sql7, bcp doesn't like /P when the password is blank, but osql does--go figure
set rcfedbcppw=%rcfeddbpw%
if /i "%rcfeddbpw%"=="/P" set rcfedbcppw=

echo USE %rcfeddb% > %sqlfile%
echo go >> %sqlfile%

:recreate
copy /b %sqlfile%+Events.sql %sqlfile%
osql /U sa /n %rcfeddbsrv% %rcfeddbpw% /i %sqlfile%
goto end

:usage
rem  ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8
echo Description: Creates the AGC Events Table/schema in an existing database.
echo. 
echo Usage:   CreateEvents [/S Server] [/P Password] [/D Database] [/L]
echo. 
echo Where:   Server is machine of database in which you want to create the table.
echo          Password is the password for the sa account
echo          Database is the name of the SQL database in which to create the table
echo          /L causes the log to be preserved. Default is to dump the log prior to
echo              nuking and recreating the database.
echo. 
echo Notes:   * You must leave a space between all switches and their values
echo          * All parameters in brackets are optional, and may appear in any order
echo          * Password only matters if server doesn't use trusted connections
echo          * The created table is ALWAYS named Events
echo. 
echo Example: CreateEvents /S ZONEAGGA01 /D Allegiance
echo. 
rem  ----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8

:end
set rcfeddb=
set rcfeddbsrv=
set rcfeddbpw=
set rcfeddbdumplog=
set rcfedbcppw=
set sqlfile=
