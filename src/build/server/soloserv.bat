
@ECHO OFF

if "%DEBUGTHEBUILD%"=="1" @ECHO ON

call \\fedbuild\fedsrc\src\build\server\init.bat

set INSTALLSERVERERR=%PROPDIR%\%BUILD%\instserv.err

rem
rem Print out the last build number and input new build number.
rem
call %SCRIPTDIR%\util\maxbld.bat  %PROPDIR% BUILD
call %SCRIPTDIR%\util\maxbld.bat  %DIRECTXDIR% DIRECTXBUILD

:WaitLoop
if exist %PROPDIR%\%BUILD%\bldsucc.txt goto Success
if exist %PROPDIR%\%BUILD%\failure.txt goto End

sleep 600
goto WaitLoop


:Success
rem
rem Install the server.
rem
perl.exe %SCRIPTDIR%\server\instserv.pl -q -s %PROPDIR%\%BUILD% -l %PROPDIR%\%BUILD%

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
call %SCRIPTDIR%\verify\verdbtbl.bat %BUILD% %COMPUTERNAME%
if "%BUILDERROR%"=="1" goto Error
call %SCRIPTDIR%\verify\verdbdta.bat %BUILD% %COMPUTERNAME%
if "%BUILDERROR%"=="1" goto Error

rem
rem Run the server BVT.
rem
call %SCRIPTDIR%\verify\verserv.bat %BUILD% %COMPUTERNAME%
if "%BUILDERROR%"=="1" goto Error
echo Success. > %PROPDIR%\%BUILD%\servsucc.txt

goto End


:Error
echo An error occurred!
echo Failure. > %PROPDIR%\%BUILD%\servfail.txt
goto End


:End
set FILESIZE=
set INSTALLSERVERERR=
call %SCRIPTDIR%\server\term.bat



