#Imago <imagotrigger@gmail.com>
# Stops lobby/server, replaces objects, Starts lobby/server
#  This file is for host CDN

use strict;
use Win32::OLE;
use Net::Domain qw (hostname);
use Win32::Process;

my $bSstopped = 0;
my $bLstopped = 0;

my $s = Win32::OLE->GetObject("WinNT://".hostname()."/AllSrv,service");
if ($s && $s->Status() == 4) {
	#TODO block for up to an hour untill no running games
	print "Stopping AllSrv service\n";	
	$s->Stop();
	$bSstopped = 1;
	sleep(5);
}

my $sl = Win32::OLE->GetObject("WinNT://".hostname()."/AllLobby,service");
if ($sl && $sl->Status() == 4 && (!$s || $s->Status() != 4)) {
	print "Stopping AllLobby service\n";	
	$sl->Stop();
	$bLstopped = 1;
	sleep(3);
}

if (($s && $s->Status == 4) || ($sl && $sl->Status == 4)) {
	print "Services wouldn't shut down!\n";
	exit 1;	
}


my $bSkilled = 0;
my $bLkilled = 0;

open (PS,"C:\\pslist.exe 2>crap AllSrv |");
my $pid = 0;
while (<PS>) {
	if ($_ =~ /AllSrv\s+(\d+)/) {
		$pid = $1;
	}
}
close PS;

if ($pid != 0) {
	#TODO block for up to an hour untill no running games
	my $cmd = "C:\\pskill 2>crap $pid";
	print "Killing AllSrv executable $pid\n";
	`$cmd`;
	$bSkilled = 1;
	sleep(5);
	my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /u /s";
	`$cmd`;
}

open (PS,"C:\\pslist.exe 2>crap AllLobby |");
my $pid = 0;
while (<PS>) {
	if ($_ =~ /AllLobby\s+(\d+)/) {
		$pid = $1;
	}
}
close PS;

if ($pid != 0) {
	#TODO block for up to an hour untill no running games
	my $cmd = "C:\\pskill 2>crap $pid";
	print "Killing AllLobby executable $pid\n";
	`$cmd`;
	$bLkilled = 1;
	sleep(3);
}


	my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /u /s";
	`$cmd`;
	
	my $cmd = "regsvr32 C:\\AllegBeta\\AGC.dll /s";
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


if ($bLstopped && $sl) {
	print "Starting Lobby service\n";	
	$sl->Start();
	sleep(3);
}


if ($bSstopped && $s) {

	print "Starting AllSrv service\n";	
	$s->Start();
	sleep(5);
}

if ($bLkilled) {
	my $cmd = "C:\\AllegBeta\\AllLobby.exe";
	print "Starting AllLobby executable\n";	
	my $ProcessObj = "";
	Win32::Process::Create($ProcessObj,
					$cmd,
					"AllLobby",
					0,
					NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
					"C:\\AllegBeta")|| die ErrorReport();
	sleep(3);
}

#if ($bSkilled) {
	my $cmd = "C:\\AllegBeta\\AllSrv.exe";
	print "Starting AllSrv executable\n";
	my $ProcessObj = "";
	Win32::Process::Create($ProcessObj,
					$cmd,
					"AllSrv",
					0,
					NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
					"C:\\AllegBeta")|| die ErrorReport();

	sleep(5);	
#}


exit 0;


