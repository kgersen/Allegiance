@echo off
cls

echo Welcome to MSR Federation Client Setup:
echo.
echo.

rem nt not supported at this time

if %OS%.==. goto win95

if %OS%==Windows_NT goto No_NT

:Win95

if not exist %windir%\System\d3drm.dll goto NeedDx5

set DestDir=%1

if "%1"=="" set DestDir=c:\federation

rem need default dir to be the install tree

if not exist setup.bat goto NeedDefaultDir

echo.
deltree %DestDir%

:CreateDir

echo Creating %DestDir%
md %DestDir%

echo Copying Igc.dll
copy Igc.dll %DestDir%

echo RegSvr32'ing Igc.dll
RegSvr32 /s %DestDir%\Igc.dll

echo Copying WinTrek.exe to %DestDir%
copy WinTrek.exe %DestDir%

if exist %windir%\System\Atl.dll goto RegAtlDll

echo Copying Atl.dll to %windir%\System
copy Atl.dll %windir%\System

:RegAtlDll

echo RegSvr32'ing %windir%\System\Atl.dll
RegSvr32 /s %windir%\System\Atl.dll

if exist msvcrt.dll goto RetailMfc

echo Copying MsvCrtD.dll to %windir%\System
copy msvcrtd.dll %windir%\System

goto PostMfc

:RetailMfc

echo Copying MsvCrt.dll to %windir%\System
copy msvcrt.dll %windir%\System

:PostMfc

echo.
echo Install complete.
echo.

goto AllDone

:NeedDefaultDir
echo.
echo.
echo.
echo Usage: When running Setup, the default directory must be set
echo        to the release tree.

goto TellAboutArgs

:Usage

echo.
echo.
echo.
echo Usage: 
echo.

:TellAboutArgs
echo.
echo   Setup <InstallPath>
echo.
echo         <InstallPath> should be the path to where you want the game installed.
echo         This directory need not exist... but it should be on a writable drive
echo         with sufficient free space.
echo.
echo.
echo.

goto AllDone

:No_NT
echo.
echo At this time Windows NT is not a supported platform.  Thank you for playing...
echo.
echo.

goto AllDone

:NeedDx5

echo.
echo You MUST install DX5 before you can install the Federation Client.
echo.
echo Also, you should install the 5.0a upgrade as well.
echo.
echo Please Install DX5/0a from http://www.microsoft.com/directx
echo.

goto AllDone

:AllDone

