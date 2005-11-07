@echo off
echo @echo off
echo cls
echo.
echo echo Welcome to MSR Federation Client Setup:
echo echo.
echo echo.
echo.
echo rem nt not supported at this time
echo.
echo if %%OS%%.==. goto win95
echo.
echo if %%OS%%==Windows_NT goto No_NT
echo.
echo :Win95
echo.
echo if not exist %%windir%%\System\d3drm.dll goto NeedDx5
echo.
echo set DestDir=%%1
echo.
echo if "%%1"=="" set DestDir=C:\Federation
echo.
echo.
echo echo.
echo deltree %%DestDir%%
echo.
echo :CreateDir
echo.
echo echo Creating %%DestDir%%
echo md %%DestDir%%
echo.
echo echo Copying %1\WinTrek.exe/.pdb to %%DestDir%%
echo copy %1\WinTrek.exe %%DestDir%%
echo copy %1\WinTrek.pdb %%DestDir%%
echo.
echo echo Installing MsvCrt(d).dll to %%windir%%\System
echo copy %1\msvcrtd.dll %%windir%%\System
echo copy %1\msvcrt.dll  %%windir%%\System
echo.
echo echo Installing Initial Artwork
echo md %%DestDir%%\Artwork
echo md %%DestDir%%\Artwork\256
echo call %1\CopyInitialArt %1\..\Src\Artwork\256 %%DestDir%%\Artwork\256
echo echo.
echo echo Install attempt complete.
echo echo.
echo.
echo goto AllDone
echo.
echo :NeedDefaultDir
echo echo.
echo echo.
echo echo.
echo echo Usage: When running Setup, the default directory must be set
echo echo        to the release tree.
echo.
echo goto TellAboutArgs
echo.
echo :Usage
echo.
echo echo.
echo echo.
echo echo.
echo echo Usage:
echo echo.
echo.
echo :TellAboutArgs
echo echo.
echo echo   Setup InstallPath
echo echo.
echo echo         InstallPath should be the path to where you want the game installed
echo echo         This directory need not exist... but it should be on a writable drive
echo echo         with sufficient free space.
echo echo.
echo echo.
echo echo.
echo.
echo goto AllDone
echo.
echo :No_NT
echo echo.
echo echo At this time Windows NT is not a supported platform.  Thank you for playing...
echo echo.
echo echo.
echo.
echo goto AllDone
echo.
echo :NeedDx5
echo.
echo echo.
echo echo You MUST install DX5 before you can install the Federation Client.
echo echo.
echo echo Also, you should install the 5.0a upgrade as well.
echo echo.
echo echo Please Install DX5/0a from http://www.microsoft.com/directx
echo echo.
echo.
echo goto AllDone
echo.
echo :AllDone
