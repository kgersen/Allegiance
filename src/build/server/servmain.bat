
if "%4"=="" goto Usage

set INSTALLSERVERERR=%2\instserv.err

rem
rem Install the server.
rem
%FEDDRIVE%
cd %SCRIPTDIR%\server
perl.exe %SCRIPTDIR%\server\instserv.pl -q -s %PROPDIR%\%1 -l %2

rem
rem Check the log for errors.
rem
findstr FATAL %TEMP%\instserv.log > %INSTALLSERVERERR%
call %SCRIPTDIR%\util\filesize.bat %INSTALLSERVERERR% FILESIZE
if "%FILESIZE%"=="0" goto InstallSuccess
set BUILDERROR=1
goto Error


:InstallSuccess
rem
rem Run the database BVT.
rem
call %SCRIPTDIR%\verify\verdbtbl.bat %1 %3 %4
if "%BUILDERROR%"=="1" goto Error
call %SCRIPTDIR%\verify\verdbdta.bat %1 %3 %4
if "%BUILDERROR%"=="1" goto Error

rem
rem Run the server BVT.
rem
call %SCRIPTDIR%\verify\verserv.bat %1 %COMPUTERNAME% %3 %4
if "%BUILDERROR%"=="1" goto Error
echo Success. > %2\servsucc.txt

goto End


:Error
echo An error occurred!
echo Failure. > %2\servfail.txt
goto End


:Usage
echo Usage: servmain.bat [build number] [log file dir] [db server] [db name]
goto End


:End
set FILESIZE=
set INSTALLSERVERERR=



