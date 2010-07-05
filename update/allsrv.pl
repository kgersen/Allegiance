#Imago <imagotrigger@gmail.com>
# Starts AllSrv
#  This file is for host CDN

use strict;
#use Win32::Process;
use Win32::OLE;


print "Starting AllSrv service\n";	
my $s = Win32::OLE->GetObject("WinNT://cdn/AllSrv,service");
$s->Start();
sleep(12);
$s->Start();
sleep(6);
if ($s && $s->Status != 4) {
	print "Server wouldn't start!\n";
	exit 1;	
}

exit 0;

__END__
my $cmd = "C:\\AllegBeta\\AllSrv.exe";
my $ProcessObj = "";
Win32::Process::Create($ProcessObj,
	$cmd,
	"AllSrv",
	0,
	NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
	"C:\\AllegBeta") || die "failed to create allsrv.exe process\n";

