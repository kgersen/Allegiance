
if not "%COMPUTERNAME%"=="" goto Done

net config | awk -f %SCRIPTDIR%\util\computer.awk > %TEMP%\computer.bat
call %TEMP%\computer.bat

:Done

