
if "%1"=="" goto Usage
if "%NOMAIL%"=="1" goto NoMail

call %SCRIPTDIR%\mail\initmail.bat

rem
rem Send the mail.
rem
%EXMAIL% -n %EXMAILSERVER% -m %MAILBOXFROM% -p %MAILBOXFROM% -t %MAILBOXTO% -y %MAILBOXTO% -s "Build %1 starting in 5 minutes"

rem
rem Wait five minutes
rem
title Waiting 5 minutes to start build %1
sleep 300
title Building...

call %SCRIPTDIR%\mail\termmail.bat
goto End


:Usage
echo Usage: premail.bat [build number]
set BUILDERROR=1
goto End


:NoMail
goto End


:End


