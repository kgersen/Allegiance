
if "%1"=="" goto Usage
if "%NOMAIL%"=="1" goto NoMail

call %SCRIPTDIR%\mail\initmail.bat

set MAILBOXTO=marksn;jfink

rem
rem Send the mail.
rem
%EXMAIL% -n %EXMAILSERVER% -m %MAILBOXFROM% -p %MAILBOXFROM% -t %MAILBOXTO% -y %MAILBOXTO% -c mspager -s %1

title Aborting...

call %SCRIPTDIR%\mail\termmail.bat
goto End


:Usage
echo Usage: errmail.bat
set BUILDERROR=1
goto End


:NoMail
goto End


:End


