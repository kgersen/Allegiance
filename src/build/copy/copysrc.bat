
if "%1"=="" goto Usage

if exist %PROPSRCDIR%\%1 delnode /q %PROPSRCDIR%\%1

set COPYDIRLIST=(clintlib drones effect engine fedperf fedsrv igc inc jpeg lang sharemem test\core test\inc test\pigs _utility wintrek zlib)


rem
rem Copy the source code.
rem
if not exist %PROPSRCDIR%\%1 mkdir %PROPSRCDIR%\%1
for %%d in %COPYDIRLIST% do if not exist %PROPSRCDIR%\%1\%%d mkdir %PROPSRCDIR%\%1\%%d
for %%d in %COPYDIRLIST% do xcopy /E %FEDSOURCEROOT%\%%d\* %PROPSRCDIR%\%1\%%d

set COPYDIRLIST=
goto End


:Usage
echo Usage: copysrc [build number]
goto End


:End


