
if "%1"=="" goto Usage

set COMPILEERRLOG=%LOGDIR%\compile.err
set COMPILEWARNLOG=%LOGDIR%\compile.wrn

rem
rem Sed requires an escaped string for replacing.
rem
call %SCRIPTDIR%\util\escstr.bat %FEDROOT% ESCAPEDFEDROOT

rem
rem Extract all the error and warning messages out of the compilation log.
rem
findstr /I error   %COMPILELOG% > %TEMP%\compile.err
findstr /I warning %COMPILELOG% > %TEMP%\compile.wrn
type %SCRIPTDIR%\verify\compilee.sed | sed s/FEDROOT/%ESCAPEDFEDROOT%/ > %SCRIPTDIR%\verify\compile.err
type %SCRIPTDIR%\verify\compilew.sed | sed s/FEDROOT/%ESCAPEDFEDROOT%/ > %SCRIPTDIR%\verify\compile.wrn

diff %SCRIPTDIR%\verify\compile.err %TEMP%\compile.err > %COMPILEERRLOG%
diff %SCRIPTDIR%\verify\compile.wrn %TEMP%\compile.wrn > %COMPILEWARNLOG%

rem
rem The filesize should be zero if everything went well.
rem
call %SCRIPTDIR%\util\filesize.bat %COMPILEERRLOG% FILESIZE
if "%FILESIZE%"=="0" goto SuccessErr
set BUILDERROR=1
goto AfterErrCheck


:SuccessErr
echo No errors found. > %COMPILEERRLOG%
goto AfterErrCheck


:AfterErrCheck
rem
rem Check for warnings.
rem
call %SCRIPTDIR%\util\filesize.bat %COMPILEWARNLOG% FILESIZE
if "%FILESIZE%"=="0" echo No warnings found. > %COMPILEWARNLOG%


set COMPILEERRLOG=
set COMPILEWARNLOG=
set ESCAPEDFEDROOT=
goto End


:Usage
echo Usage: vercom.bat [build number]
set BUILDERROR=1
goto End


:End


