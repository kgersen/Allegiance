
if "%1"=="" goto Usage

if exist VTSignal.txt del VTSignal.txt
if exist VTError.txt  del VTError.txt

call mtrun %1 /C "/GO"


:Loop

sleep 300
if exist VTSignal.txt goto End
if exist VTError.txt goto Error
goto Loop


:Error
set BUILDERROR=1


:Usage
echo Usage: runvt.bat [VT PCD file]
goto End


:End


