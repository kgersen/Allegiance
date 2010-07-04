#Imago <imagotrigger@gmail.com>
# Stops, Replaces, Reregisters and Starts AllSrv
#  This file is for host CDN

use strict;
use Win32::Process;

my $cmd = "C:\\AllegBeta\\AutoUpdate.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AutoUpdate shutdown",
	0,
	NORMAL_PRIORITY_CLASS,
	"C:\\AllegBeta") || die "failed to create autoupdate.exe process\n";
	
sleep(1);

my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /u /s";
`$cmd`;
	
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

my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /s";
`$cmd`;	

sleep(1);

my $cmd = "C:\\AllegBeta\\AllSrv.exe";
print "Starting AllSrv executable\n";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv -reregister",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta") || die "failed to create allsrv.exe reregister process\n";	

sleep(1);

my $cmd = "C:\\AllegBeta\\AllSrv.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta") || die "failed to create allsrv.exe process\n";
	

exit 0;


