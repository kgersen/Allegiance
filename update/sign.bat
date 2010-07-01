rem #Imago <imagotrigger@gmail.com> Signs the build's objects

@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86

signtool sign -a -v -s Mine -n "Free Allegiance Development Zone" -t http://timestamp.verisign.com/scripts/timestamp.dll C:\build\FAZR6\objs10\%1\AllSrvUI\AllSrvUI.exe
signtool sign -a -v -s Mine -n "Free Allegiance Development Zone" -t http://timestamp.verisign.com/scripts/timestamp.dll C:\build\FAZR6\objs10\%1\FedSrv\Allsrv.exe
signtool sign -a -v -s Mine -n "Free Allegiance Development Zone" -t http://timestamp.verisign.com/scripts/timestamp.dll C:\build\FAZR6\objs10\%1\Lobby\AllLobby.exe
signtool sign -a -v -s Mine -n "Free Allegiance Development Zone" -t http://timestamp.verisign.com/scripts/timestamp.dll C:\build\FAZR6\objs10\%1\WinTrek\Allegiance.exe
signtool sign -a -v -s Mine -n "Free Allegiance Development Zone" -t http://timestamp.verisign.com/scripts/timestamp.dll C:\build\FAZR6\objs10\%1\AGC\AGC.dll