
if "%3"=="" goto Usage

set DBDATALOG=%LOGDIR%\verdbdta.log
set DBDATAWRN=%LOGDIR%\verdbdta.wrn
set TECHTREELOG=%LOGDIR%\techtree.log


rem
rem Run the BVT via ISQL.
rem
isql.exe -S %2 -E -d %3 -n -i %SCRIPTDIR%\verify\verdbdta.sql > %DBDATALOG%


rem
rem Run the techtree tool.
rem
%PROPDIR%\%1\test\techtree /a > %TECHTREELOG%
findstr Warning %TECHTREELOG% >> %DBDATALOG%

rem
rem The filesize should be zero if everything went well.
rem
call %SCRIPTDIR%\util\filesize.bat %DBDATALOG% FILESIZE
if "%FILESIZE%"=="0" goto Success
echo Warnings found in database log. > %DBDATAWRN%
type %DBDATALOG% >> %DBDATAWRN%
goto AfterCheck


:Success
echo No warnings found. > %DBDATAWRN%
goto AfterCheck


:AfterCheck
set DBDATATEMP=
set DBDATALOG=
set DBDATAWRN=
set FILESIZE=
goto End


:Usage
echo Usage: verdbdta.bat [Build] [SQL Server] [SQL Database]
set BUILDERROR=1
goto End


:End


