#Imago <imagotrigger@gmail.com>
# Stops, Replaces, Reregisters AllSrv
#  This file is for host CDN

use strict;
use Win32::Process;

print "updating autoupdate...\n";

my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\AutoUpdate\\AutoUpdate.exe C:\\AllegBeta\\AutoUpdate.exe /Y";
`$cmd`;

sleep(3);

print "executing autoupdate shutdown...\n";

my $cmd = "C:\\AllegBeta\\AutoUpdate.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AutoUpdate shutdown",
	0,
	NORMAL_PRIORITY_CLASS,
	"C:\\AllegBeta") || die "failed to create autoupdate.exe process\n";
	
$ProcessObj->Wait(INFINITE);
sleep(3);

print "executing un regsvr32 AGC.dll...\n";

my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /u /s";
`$cmd`;

sleep(3);

print "copying new objects...\n";

my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\AGC\\AGC.dll C:\\AllegBeta\\AGC.dll /Y";
`$cmd`;
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\FedSrv\\AllSrv.exe C:\\AllegBeta\\AllSrv.exe /Y";
`$cmd`;
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\Lobby\\AllLobby.exe C:\\AllegBeta\\AllLobby.exe /Y";
`$cmd`;
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\AGC\\AGC.pdb C:\\AllegBeta\\AGC.pdb /Y";
`$cmd`;
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\FedSrv\\AllSrv.pdb C:\\AllegBeta\\AllSrv.pdb /Y";
`$cmd`;
my $cmd = "copy C:\\build\\FAZR6\\objs10\\FZDebug\\Lobby\\AllLobby.pdb C:\\AllegBeta\\AllLobby.pdb /Y";
`$cmd`;

print "executing regsvr32 AGC.dll...\n";

my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /s";
`$cmd`;	

sleep(3);

print "executing allsrv rereg...\n";

my $cmd = "C:\\AllegBeta\\AllSrv.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv -reregister",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta") || die "failed to create allsrv.exe reregister process\n";	

exit 0;

__END__
	




