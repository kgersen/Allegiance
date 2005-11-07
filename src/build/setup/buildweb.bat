
if "%1"=="" goto Usage

call %SCRIPTDIR%\setup\initweb.bat

type %SCRIPTDIR%\setup\latestbuild.sed    | sed.exe s/BUILDNUM/%1/ > %WEBFILE1%
type %SCRIPTDIR%\setup\latestbuildlog.sed | sed.exe s/BUILDNUM/%1/ > %WEBFILE2%

call %SCRIPTDIR%\setup\termweb.bat
goto End


:Usage
echo Usage: buildweb.bat [build number]
goto End


:End



