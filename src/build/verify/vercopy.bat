
if "%1"=="" goto Usage

set COPYERRORLOG=%LOGDIR%\copy.err

rem
rem Sed requires an escaped string for replacing.
rem
findstr /C:"0 file(s) copied" %COPYLOG% > %COPYERRORLOG%

for %%f in ( fedsrv.exe fedsrv.pdb fedsrv.sym ) DO if not exist %PROPDIR%\%1\server\debug\%%f echo %%f (debug) not copied. >> %COPYERRORLOG%
for %%f in ( fedsrv.exe fedsrv.pdb fedsrv.sym ) DO if not exist %PROPDIR%\%1\server\retail\%%f echo %%f (retail) not copied. >> %COPYERRORLOG%
for %%f in ( fedperf.dll fedperf.pdb ) DO if not exist %PROPDIR%\%1\server\shared\%%f echo %%f not copied >> %COPYERRORLOG%
for %%f in ( pigs.exe pigs.pdb ) DO if not exist %PROPDIR%\%1\test\%%f echo %%f not copied >> %COPYERRORLOG%

rem
rem The error file should be 0 bytes if everything went well.
rem
call %SCRIPTDIR%\util\filesize.bat %COPYERRORLOG% FILESIZE
if "%FILESIZE%"=="0" goto Success
set BUILDERROR=1
goto AfterCheck


:Success
echo No errors found. > %COPYERRORLOG%
goto AfterCheck


:AfterCheck
set COPYERRORLOG=
set FILESIZE=
goto End


:Usage
echo Usage: vercopy [build number]
goto End


:End


