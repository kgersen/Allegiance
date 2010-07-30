copy %2objs10\FZRetail\FedSrv\AllSrv.exe %1 /Y
copy %2objs10\FZRetail\FedSrv\AllSrv.PDB %1 /Y
copy %2objs10\FZRetail\AllSrvUI\AllSrvUI.exe %1 /Y
copy %2objs10\FZRetail\AllSrvUI\AllSrvUI.PDB %1 /Y
copy %2objs10\FZRetail\Lobby\AllLobby.exe %1 /Y
copy %2objs10\FZRetail\Lobby\AllLobby.PDB %1 /Y
copy %2objs10\FZRetail\WinTrek\Allegiance.exe %1 /Y
copy %2objs10\FZRetail\WinTrek\Allegiance.PDB %1 /Y
copy %2objs10\FZRetail\AGC\AGC.dll %1 /Y
copy %2objs10\FZRetail\AGC\AGC.PDB %1 /Y
copy %2objs10\FZRetail\AutoUpdate\AutoUpdate.exe %1 /Y
copy %2objs10\FZRetail\AutoUpdate\AutoUpdate.pdb %1 /Y
copy %2objs10\FZRetail\Reloader\Reloader.exe %1 /Y
copy %2objs10\FZRetail\Reloader\Reloader.pdb %1 /Y
rem copy "%VS100COMNTOOLS%..\..\VC\redist\x86\Microsoft.VC100.ATL\atl100.dll" %1 /Y
rem copy "%VS100COMNTOOLS%..\..\VC\redist\x86\Microsoft.VC100.MFC\mfc100.dll" %1 /Y
rem copy "%VS100COMNTOOLS%..\..\VC\redist\x86\Microsoft.VC100.CRT\msvcp100.dll" %1 /Y
rem copy "%VS100COMNTOOLS%..\..\VC\redist\x86\Microsoft.VC100.CRT\msvcr100.dll" %1 /Y
copy %2src\license.txt %1 /Y
