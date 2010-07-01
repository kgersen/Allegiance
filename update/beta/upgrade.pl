#Imago <imagotrigger@gmail.com>
#Stops lobby/server, replaces objects, Starts lobby/server
#BETA

use strict;
use Win32::OLE;

my $bSstopped = 0;
my $bLstopped = 0;

my $s = Win32::OLE->GetObject("WinNT://beta.alleg.net/AllSrv,service");
if ($s && $s->Status() == 4) {
	#TODO block for up to an hour untill no running games
	print "Stopping AllSrv service\n";	
	$s->Stop();
	$bSstopped = 1;
	sleep(10);
}

my $sl = Win32::OLE->GetObject("WinNT://beta.alleg.net/AllLobby,service");
if ($sl && $sl->Status() == 4 && (!$s || $s->Status() != 4)) {
	print "Stopping AllLobby service\n";	
	$sl->Stop();
	$bLstopped = 1;
	sleep(6);
}

if (($s && $s->Status == 4) || ($sl && $sl->Status == 4)) {
	print "Services wouldn't shut down!\n";
	exit 1;	
}


my $cmd = "expand Z:\\wwwroot\\FAZ\\AU\\AGC.dll C:\\Allegiance\\Server\\AGC.dll";
system($cmd);
my $cmd = "expand Z:\\wwwroot\\FAZ\\AU\\AllSrvUI.exe C:\\Allegiance\\Server\\AllSrvUI.exe";
system($cmd);
my $cmd = "expand Z:\\wwwroot\\FAZ\\AU\\AllSrv.exe C:\\Allegiance\\Server\\AllSrv.exe";
system($cmd);
my $cmd = "expand Z:\\wwwroot\\FAZ\\AU\\dbghelp.dll C:\\Allegiance\\Server\\dbghelp.dll";
system($cmd);
my $cmd = "copy Z:\\deploy\\AllLobby.exe C:\\Allegiance\\Lobby\\AllLobby.exe /Y";
system($cmd);
my $cmd = "expand Z:\\wwwroot\\FAZ\\AU\\dbghelp.dll C:\\Allegiance\\Lobby\\dbghelp.dll";
system($cmd);
my $cmd = "regsvr32 C:\\Allegiance\\Server\\AGC.dll /u /s";
system($cmd);
my $cmd = "regsvr32 C:\\Allegiance\\Server\\AGC.dll /s";
system($cmd);


#if ($bLstopped && $sl) {
	print "Starting Lobby service\n";	
	$sl->Start();
	sleep(6);
#}


#if ($bSstopped && $s) {
	print "Starting AllSrv service\n";	
	$s->Start();
	sleep(10);
#}

exit 0;


