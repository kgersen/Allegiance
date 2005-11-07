
if "%1"=="" goto Usage

call %SCRIPTDIR%\server\servmain.bat %1 %LOGDIR% oblsql federation
goto End


:Usage
echo Usage: server.bat [build number]
goto End


:End



