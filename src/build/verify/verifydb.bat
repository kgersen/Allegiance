
@ECHO OFF
if "%2"=="" goto Usage

echo Database Errors
echo ---------------

rem
rem Run the BVT via ISQL.
rem
isql.exe -S %1 -E -d %2 -n -i verdbdta.sql
goto End


:Usage
echo Usage: verifydb.bat [SQL Server] [SQL Database]
goto End


:End


