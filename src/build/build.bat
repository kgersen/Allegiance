
@ECHO OFF

if "%DEBUGTHEBUILD%"=="1" @ECHO ON

call %FEDROOT%\src\build\init.bat


rem
rem Print out the last build number and input new build number.
rem
call %SCRIPTDIR%\util\nextbld.bat %PROPDIR% BUILD
call %SCRIPTDIR%\util\maxbld.bat  %DIRECTXDIR% DIRECTXBUILD
set LOGDIR=%PROPDIR%\%BUILD%\BVTLog
if not exist %LOGDIR% mkdir %LOGDIR%

rem
rem Send mail to everyone to indicate the build is starting.
rem
call %SCRIPTDIR%\mail\premail.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause


rem
rem Compile the code.
rem
call %SCRIPTDIR%\compile.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error


rem
rem Call installshield to build the client.
rem
call %SCRIPTDIR%\isbuild\isbuild.bat %BUILD% %DIRECTXDIR%\%DIRECTXBUILD%\retail
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error


rem
rem Copy the built binaries and source to the prop locations.
rem
call %SCRIPTDIR%\copy\copy.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error

echo Web pages not exported.
goto NoWebExport
rem
rem Create web pages to point to the build.
rem
call %SCRIPTDIR%\setup\buildweb.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error
:NoWebExport

rem
rem Create a batch file to point to the build.
rem
call %SCRIPTDIR%\setup\buildbat.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error

rem
rem Create the test pass in the SQL database.
rem
rem echomdy "~M/~D/~Y ~h:~m:~s" > %TEMP%\todaysdate.txt
echomdy "~M-~D-~Y" > %TEMP%\todaysdate.txt
call d:\fedsrc\src\build\util\todaysdate.bat %TEMP%\todaysdate.txt TODAYSDATE
call d:\fedsrc\src\build\setup\testpass.bat 1 %TODAYSDATE% oblsql TestCaseMgr
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error

rem
rem Send mail to everyone to indicate the build is over.
rem
call %SCRIPTDIR%\mail\postmail.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause
echo Success. > %LOGDIR%\bldsucc.txt

rem
rem Call the server bvt.
rem
call %SCRIPTDIR%\server\server.bat %BUILD%
if "%PAUSETHEBUILD%"=="1" pause
if "%BUILDERROR%"=="1" goto Error
echo Success. > %LOGDIR%\success.txt

goto End


:Error
echo An error occurred!
echo Failure. > %LOGDIR%\failure.txt

rem
rem Send out a page.
rem
pushd %FEDSOURCEROOT%\tools\build
tinypage.exe marksn Build %BUILD% failed.
popd
goto End


:End
rem
rem Generate a log indicating what happened during the build.
rem
call %SCRIPTDIR%\setup\buildlog.bat %BUILD%

set BUILD=
set DIRECTXBUILD=
set LOGDIR=
call %SCRIPTDIR%\term.bat
pause


