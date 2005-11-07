
@ECHO OFF

rem
rem This batchfile should run setup on the latest available build.
rem

\\research\root\oblivion\BUILDNUM\setup.exe -s -f1\\research\root\oblivion\BUILDNUM\setup.iss -f2%WINDIR%\setup.log

pause

start notepad %WINDIR%\setup.log

