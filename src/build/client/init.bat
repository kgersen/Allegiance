
rem
rem Set constants.
rem
set BUILDERROR=0

if not "%FEDROOT%"=="" set OLDFEDROOT=%FEDROOT%
rem
rem Net use
rem
set FEDSHARE=\\fedbuild\fedsrc

rem
rem We need to set up a bunch of temp vars to get
rem the drive letter from the net use.
rem
set OLDPATH=%PATH%
set PATH=%PATH%;%FEDSHARE%\src\tools\build
set SCRIPTDIR=%FEDSHARE%\src\build

net use ? %FEDSHARE% > %TEMP%\netuse.out
call %FEDSHARE%\src\build\util\getdriv2.bat %TEMP%\netuse.out FEDDRIVE
set FEDROOT=%FEDDRIVE%
set PATH=%OLDPATH%
set OLDPATH=
set SCRIPTDIR=


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

:BuildPathSet

rem
rem Make sure the COMPUTERNAME variable is set.
rem
call %SCRIPTDIR%\util\computer.bat



