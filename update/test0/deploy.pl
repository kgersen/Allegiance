#Imago <imagotrigger@gmail.com>
# Send files to test FTP /w a ChangeNotiy folder
#  Waits for remote completion notification

use strict;
use Net::FTP;
use Data::Dumper; 

my ($build,$rev) = @ARGV;

open(PASS,"C:\\test0.passwd");
my $pass = <PASS>;
close PASS;

print "Uploading files to fuzztest.dyndns.org\n";
my $ftp = Net::FTP->new("fuzztest.dyndns.org", Debug => 1, Port => 5903, Passive => 1) or die "Cannot connect to test0 $@";

$ftp->login("deploy",$pass);   
print "Uploading R6_b${build}_r${rev}.exe\n";
$ftp->binary;
$ftp->put("C:\\Inetpub\\wwwroot\\build\\R6_b${build}_r${rev}.exe") or die "put failed ", $ftp->message;       
  
print "Files uploaded OK\n";
$ftp->rename("notify/ready","notify/process") or die "notify failed ", $ftp->message;
$ftp->cwd('notify');
print "Waiting for tests to finish\n";
my $count = 0;
my $bfail = 0;
while (1) {
	$count++;
	my @dirs = $ftp->ls();
	print Dumper(\@dirs);
	if ($dirs[0] eq 'ok') {
		$ftp->delete('ok');
		last;
	}
	sleep(10);
	if ($count > 30) {
		$bfail = 1;
		last;
	}
}
$ftp->cwd('/');
$ftp->ascii;     
print "Grabbing result log...\n";
$ftp->get("test_process.log","C:\\test_process.log") or die "put failed ", $ftp->message;      

if ($bfail) {
	print "Remote deployment process did not return to a ready state\n";
	exit 1;
} else {
	print "Finished! Here is the log:\n";
	open(LOG,"C:\\test_process.log");
	while (<LOG>) {
		print "fuzztest.dyndns.org>\t".$_;
	}
	close LOG;
}

$ftp->quit();

exit 0;