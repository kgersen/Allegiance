#Imago <imagotrigger@gmail.com>
# Stops, Replaces, Reregisters AllSrv
#  This file is for host CDN

use strict;
use Win32::Process;


my $cmd = "TASKKILL /IM mspdbsrv.exe /T /F";
`$cmd`;

print "executing autoupdate shutdown...\n";

my $cmd = "C:\\AllegBeta\\AutoUpdate.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AutoUpdate shutdown",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta");
	
$ProcessObj->Wait(INFINITE);
sleep(3);

print "executing un regsvr32 AGC.dll...\n";

my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /u /s";
`$cmd`;

sleep(6);

print "copying new AGC.dll...\n";

my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\AGC\\AGC.dll C:\\AllegBeta\\AGC.dll /Y";
`$cmd`;
print "copying new AllSrv.exe...\n";
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\FedSrv\\AllSrv.exe C:\\AllegBeta\\AllSrv.exe /Y";
`$cmd`;
print "copying new AllLobby.exe...\n";
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\Lobby\\AllLobby.exe C:\\AllegBeta\\AllLobby.exe /Y";
`$cmd`;
print "copying new AGC.pdb...\n";
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\AGC\\AGC.pdb C:\\AllegBeta\\AGC.pdb /Y";
`$cmd`;
print "copying new AllSrv.pdb...\n";
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\FedSrv\\AllSrv.pdb C:\\AllegBeta\\AllSrv.pdb /Y";
`$cmd`;
print "copying new AllLobby.pdb...\n";
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\Lobby\\AllLobby.pdb C:\\AllegBeta\\AllLobby.pdb /Y";
`$cmd`;

print "updating autoupdate...\n";

my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\AutoUpdate\\AutoUpdate.exe C:\\AllegBeta\\AutoUpdate.exe /Y";
`$cmd`;

print "executing regsvr32 AGC.dll...\n";

my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /s";
`$cmd`;	

sleep(6);

print "executing allsrv rereg...\n";

my $cmd = "C:\\AllegBeta\\AllSrv.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv -reregister",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta");	

exit 0;

__END__
	




