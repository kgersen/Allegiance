
if "%2"=="" goto Usage

rem
rem Start perfmon using the .pml that's here.
rem
if exist %2 del %2
if not exist %1 goto End

copy %1 c:\temp\fedsrv.pml
start perfmon.exe c:\temp\fedsrv.pml
goto End


:Usage
echo perfmon.bat [pml file] [log file]
goto End


:End


