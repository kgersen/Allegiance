
if "%1"=="" goto Usage

call %SCRIPTDIR%\setup\initbat.bat

type %SCRIPTDIR%\setup\setup.sed | sed s/BUILDNUM/%1/ | sed s/COMPUTERNAME/%COMPUTERNAME%/ > %SETUPBATCHFILE%

type %SCRIPTDIR%\setup\setupsilent.sed | sed s/BUILDNUM/%1/ | sed s/COMPUTERNAME/%COMPUTERNAME%/ > %SETUPSILENTBATCHFILE%

call %SCRIPTDIR%\setup\termbat.bat
goto End


:Usage
echo Usage: buildbat.bat [build number]
goto End


:End



