
if "%1"=="" goto Usage
if "%NOMAIL%"=="1" goto NoMail

call %SCRIPTDIR%\mail\initmail.bat

rem
rem Create the mail and the batch file that will send the email.
rem
type %SCRIPTDIR%\mail\postmail.sed | sed.exe s/COMPUTERNAME/%COMPUTERNAME%/ | sed.exe s/BUILDNUM/%1/ > %POSTMAILFILE%

rem
rem Send the mail.
rem
%EXMAIL% -n %EXMAILSERVER% -m %MAILBOXFROM% -p %MAILBOXFROM% -t %MAILBOXTO% -y %MAILBOXTO% -s "Build %1 complete" -f %POSTMAILFILE%

call %SCRIPTDIR%\mail\termmail.bat
goto End


:Usage
echo Usage: postmail.bat [build number]
set BUILDERROR=1
goto End


:NoMail
goto End


:End


