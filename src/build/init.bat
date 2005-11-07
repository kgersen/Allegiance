
rem
rem Set constants.
rem
set BUILDERROR=0

set FEDDEBUGROOT=%FEDROOT%\objs\debug
set FEDRETAILROOT=%FEDROOT%\objs\retail
set FEDSOURCEROOT=%FEDROOT%\src
set SCRIPTDIR=%FEDSOURCEROOT%\build

rem
rem Set the FEDDRIVE enviornment variable based on FEDROOT.
rem
call %SCRIPTDIR%\util\getdrive.bat %FEDROOT% FEDDRIVE

set PROPDIR=\\research\root\federation\builds
set PROPSRCDIR=\\research\root\federation\src
set DIRECTXDIR=\\research\root\federation\directx

rem
rem Create things for the first time.
rem
if not exist %PROPDIR%    mkdir %PROPDIR%
if not exist %PROPSRCDIR% mkdir %PROPSRCDIR%

rem
rem Modify the user's path to contain the build tools.
rem
if "%BUILDPATHSET%"=="1" goto BuildPathSet
set PATH=%PATH%;%FEDSOURCEROOT%\tools\build
set BUILDPATHSET=1

:BuildPathSet

