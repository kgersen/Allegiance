
if "%1"=="" goto Usage

rem
rem Copy the server related bits.
rem
if not exist %PROPDIR%\%1\server mkdir %PROPDIR%\%1\server
if not exist %PROPDIR%\%1\server\config mkdir %PROPDIR%\%1\server\config
if not exist %PROPDIR%\%1\server\debug  mkdir %PROPDIR%\%1\server\debug
if not exist %PROPDIR%\%1\server\retail mkdir %PROPDIR%\%1\server\retail
if not exist %PROPDIR%\%1\server\shared mkdir %PROPDIR%\%1\server\shared

xcopy %FEDDEBUGROOT%\fedsrv\fedsrv.exe %PROPDIR%\%1\server\debug
xcopy %FEDDEBUGROOT%\fedsrv\fedsrv.pdb %PROPDIR%\%1\server\debug
xcopy %FEDDEBUGROOT%\fedsrv\fedsrv.sym %PROPDIR%\%1\server\debug
xcopy %FEDRETAILROOT%\fedsrv\fedsrv.exe %PROPDIR%\%1\server\retail
xcopy %FEDRETAILROOT%\fedsrv\fedsrv.pdb %PROPDIR%\%1\server\retail
xcopy %FEDRETAILROOT%\fedsrv\fedsrv.sym %PROPDIR%\%1\server\retail
xcopy %FEDRETAILROOT%\fedperf\fedperf.dll %PROPDIR%\%1\server\shared
xcopy %FEDRETAILROOT%\fedperf\fedperf.pdb %PROPDIR%\%1\server\shared
xcopy %FEDSOURCEROOT%\setup\zticket.dll %PROPDIR%\%1\server\debug
xcopy %FEDSOURCEROOT%\setup\zticketc.dll %PROPDIR%\%1\server\debug
xcopy %FEDSOURCEROOT%\setup\zticket.dll %PROPDIR%\%1\server\retail
xcopy %FEDSOURCEROOT%\setup\zticketc.dll %PROPDIR%\%1\server\retail
xcopy %FEDSOURCEROOT%\fedperf\util\* %PROPDIR%\%1\server\config
copy %FEDSOURCEROOT%\fedsrv\regini.txt %PROPDIR%\%1\server\config\fedsrv.txt

rem
rem Copy the database files.
rem
if not exist %PROPDIR%\%1\database mkdir %PROPDIR%\%1\database
xcopy /E %FEDSOURCEROOT%\database\* %PROPDIR%\%1\database


rem
rem Copy the test related bits.
rem
if not exist %PROPDIR%\%1\test mkdir %PROPDIR%\%1\test
xcopy %FEDDEBUGROOT%\test\pigs.exe %PROPDIR%\%1\test
xcopy %FEDDEBUGROOT%\test\pigs.pdb %PROPDIR%\%1\test
xcopy %FEDDEBUGROOT%\test\missctrl.exe %PROPDIR%\%1\test
xcopy %FEDDEBUGROOT%\test\missctrl.pdb %PROPDIR%\%1\test
xcopy %FEDSOURCEROOT%\test\sample\pigs\* %PROPDIR%\%1\test
type %PROPDIR%\%1\test\setup.sed | sed s/BUILDNUM/%1/ > %PROPDIR%\%1\test\setup.bat
xcopy %FEDDEBUGROOT%\test\techtree.exe %PROPDIR%\%1\test
xcopy %FEDDEBUGROOT%\test\techtree.pdb %PROPDIR%\%1\test
xcopy %FEDSOURCEROOT%\setup\zticketc.dll %PROPDIR%\%1\test
xcopy %FEDSOURCEROOT%\setup\zticketc.pdb %PROPDIR%\%1\test
xcopy %FEDSOURCEROOT%\tools\build\sed.exe %PROPDIR%\%1\test

goto End


:Usage
echo Usage: copybin [build number]
set BUILDERROR=1
goto End


:End


