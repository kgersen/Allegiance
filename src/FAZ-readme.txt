FAZ Readme
==========
Load Allegiance.sln in Visual C++ 2005 and use the FZDebug and FZRetail configurations.
If you want to use Visual Studio .NET 2003, load the Allegiance2k3.sln file instead.

Build the solution and everything will be built and put into the \objs folder of your checkout's root.
The files will be built under configuration subfolders:
\objs\FZDebug
\objs\FZRetail

The files you'll be interested are:
...\Lobby\alllobby.exe		The Lobby server
...\AGC\AGC.dll			DLL needed by Allsrv and Allegiance
...\FedSrv\allsrv.exe		The Gameserver
...\AllsrvUI\allsrvui.exe	The UI for the gameserver in 'Desktop' mode
...\WinTrek\Allegiance.exe	The game client
