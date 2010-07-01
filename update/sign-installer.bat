rem #Imago <imagotrigger@gmail.com> Signs the build's installer

@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

signtool sign /v /s Mine /n "Free Allegiance Development Zone" /d "Allegiance 1.%2 Installer" /du "http://www.freeallegiance.org" /t "http://timestamp.verisign.com/scripts/timstamp.dll" C:\Inetpub\wwwroot\build\%1