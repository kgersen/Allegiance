#Imago <imagotrigger@gmail.com>
# Starts AllSrv
#  This file is for host CDN

use strict;
use Win32::Process;

my $cmd = "C:\\AllegBeta\\AllSrv.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta") || die "failed to create allsrv.exe process\n";
	
exit 0;