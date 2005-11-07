
rem
rem Set constants.
rem
set BUILDERROR=0

if not "%FEDROOT%"=="" set OLDFEDROOT=%FEDROOT%
rem
rem Net use
rem
set FEDSHARE=\\fedbuild\fedsrc

net use ? %FEDSHARE%
set FEDDRIVE=%=ExitCodeAscii%:
set FEDROOT=%FEDDRIVE%


set FEDDEBUGROOT=%FEDROOT%\objs\debug
set FEDRETAILROOT=%FEDROOT%\objs\retail
set FEDSOURCEROOT=%FEDROOT%\src
set SCRIPTDIR=%FEDSOURCEROOT%\build

if "%FEDDRIVE%"=="" call %SCRIPTDIR%\util\getdrive.bat %FEDROOT% FEDDRIVE

set PROPDIR=\\research\root\federation\builds
set PROPSRCDIR=\\research\root\federation\src
set DIRECTXDIR=\\research\root\federation\directx

rem
rem Modify the user's path to contain the build tools.
rem
if "%BUILDPATHSET%"=="1" goto BuildPathSet
set PATH=%PATH%;%FEDSOURCEROOT%\tools\build
set BUILDPATHSET=1

rem
rem If we're running as the system, we won't have TEMP set.
rem
if "%TEMP%"=="" set TEMP=C:\TEMP
if not exist %TEMP% mkdir %TEMP%

:BuildPathSet


