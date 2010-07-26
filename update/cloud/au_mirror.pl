#imagotrigger@gmail.com
# upload a dir of files...

use strict;
use Net::FTP;
my $server = "10.0.0.5";
my $user = "cloudnas2";
open(PASS,"C:\\trac.passwd");
my $pass = <PASS>;
close PASS;
print "Uploading to CloudNAS\n";
my $ftp = Net::FTP->new($server, Debug => 0, Port => 21, Passive => 0)
	or die "Cannot connect to '$server' $@";

$ftp->login($user,$pass)
	or die "Cannot login ", $ftp->message;
	
$ftp->binary;

my $dir = "C:\\build\\Package\\tmp\\cloud\\";


opendir DIR, $dir;
my @files = readdir(DIR);
closedir DIR;

foreach (@files) {
	next if ($_ =~ /^\./);
	$ftp->put("$dir"."$_") or die "Put failed ", $ftp->message;
}
print "Files uploaded OK\n";
$ftp->quit();

exit 0;