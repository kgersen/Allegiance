
if "%1"=="" goto Usage

set VTTEMPLOG=clibvt.log
set VTERRLOG=%LOGDIR%\%COMPUTERNAME%.err

rem
rem First make sure the correct wintrek file version is here.
rem
call %SCRIPTDIR%\util\findpath.bat c:\wintrek.exe FILEPATH
if not "%FILEPATH%"=="" goto HavePath
call %SCRIPTDIR%\util\findpath.bat d:\wintrek.exe FILEPATH
if not "%FILEPATH%"=="" goto HavePath

echo Can not find wintrek.exe > %VTTEMPLOG%
set BUILDERROR=1
goto AfterCheck


:HavePath
call %SCRIPTDIR%\util\filever.bat %FILEPATH%\wintrek.exe FILEVER
if "%FILEVER%"=="1.0.%1.0" goto CorrectVersion
echo Wrong version of Wintrek (%FILEVER%)
set BUILDERROR=1
goto AfterCheck


:CorrectVersion
rem
rem Run the client BVT
rem
start %FILEPATH%\wintrek.exe
call %SCRIPTDIR%\util\runvt.bat %SCRIPTDIR%\client\clibvt.pcd
if "%BUILDERROR%"=="1" goto Error


rem
rem Check the BVT log for success.
rem
findstr FATAL %VTTEMPLOG% > %VTERRLOG%
call %SCRIPTDIR%\util\filesize.bat %VTERRLOG% FILESIZE
if not "%FILESIZE%"=="0" goto Error
echo No errors found. > %VTERRLOG%
goto AfterCheck


:Error
set BUILDERROR=1
goto AfterCheck


:AfterCheck
set VTTEMPLOG=
set VTERRLOG=
set FILESIZE=
set FILEPATH=
set FILEVER=
goto End


:Usage
echo Usage: vercli.bat [Build]
set BUILDERROR=1
goto End


:End


