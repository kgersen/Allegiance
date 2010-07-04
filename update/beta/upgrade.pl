#Imago <imagotrigger@gmail.com>
# Stops Server, Stops Lobby, Replaces, Reregisters, Starts Lobby, Starts Server
#  This file is for host BETA

use strict;
use Win32::OLE;
use Win32::Process;

my $cmd = "copy Z:\\wwwroot\\FAZ\\AU\\AutoUpdate.exe C:\\Allegiance\\Server\\AutoUpdate.exe /Y";
system($cmd);

sleep(1);

my $cmd = "C:\\Allegiance\\Server\\AutoUpdate.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AutoUpdate shutdown",
	0,
	NORMAL_PRIORITY_CLASS,
	"C:\\Allegiance\\Server") || die "failed to create autoupdate.exe process\n";
	
$ProcessObj->Wait(INFINITE);
sleep(1);

#
# TODO Wait for any OTHER servers still connected to leave!
#

my $sl = Win32::OLE->GetObject("WinNT://beta.alleg.net/AllLobby,service");
print "Stopping AllLobby service\n";	
$sl->Stop();
sleep(6);

if ($sl && $sl->Status == 4) {
	print "Lobby wouldn't stop!\n";
	exit 1;	
}

my $cmd = "regsvr32 C:\\Allegiance\\Server\\AGC.dll /u /s";
system($cmd);

sleep(3);

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

my $cmd = "regsvr32 C:\\Allegiance\\Server\\AGC.dll /s";
system($cmd);

sleep(3);

my $cmd = "C:\\Allegiance\\Server\\AllSrv.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv -service",
	0,
	NORMAL_PRIORITY_CLASS,
	"C:\\Allegiance\\Server") || die "failed to create allsrv.exe reregister process\n";


print "Starting Lobby service\n";	
$sl->Start();
sleep(6);

print "Starting AllSrv service\n";	
my $s = Win32::OLE->GetObject("WinNT://beta.alleg.net/AllSrv,service");
$s->Start();
sleep(6);

if ($sl && $sl->Status != 4) {
	print "Lobby wouldn't start!\n";
	exit 1;	
}

exit 0;


