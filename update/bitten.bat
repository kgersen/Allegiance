rem #Imago <imagotrigger@gmail.com> - Runs the bitten slave on a host

:wut
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
call C:\Python26\Scripts\bitten-slave -f C:\build\FAZR6\update\bitten.ini -i 120 --log=C:\bitten-Allegiance.log http://trac.alleg.net/builds
goto wut