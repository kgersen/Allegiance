#Fuh-zz <fuzzylunkinz@gmail.com>
# Send files to egretfiles FTP as a mirror

use strict;
use Getopt::Long;
use Net::FTP;
use Digest::MD5;

my $server = "ftp.egretfiles.com";
my $user = "build@egretfiles.com";

open(PASS,"C:\\egretfiles.passwd");
my $pass = <PASS>;
close PASS;

my $local = "";
GetOptions("b=s" => \$build, "r=s" => \$revision, "v=s" => \$version);
if($version == "1.1")
	$local = "C:\\inetpub\\wwwroot\\build\\AllegR6PDB_b$build_r$revision.exe";
else if($version == "1.0")
	$local = "C:\\inetpub\\wwwroot\\build\\AllegPDB_b$build_r$revision.exe";
else
	die "No version specified! Command line switch '-v' either 1.1 or 1.0";

print "Uploading files to $server\n";
open(FILE, $local) or die "Can't open '$local': $!";
binmode(FILE);
my $hash = Digest::MD5->new->addfile(*FILE)->hexdigest;
close FILE;
my $size= (stat($local))[7];

my $ftp = Net::FTP->new($server, Debug => 0, Port => 21, Passive => 1)
	or die "Cannot connect to '$server' $@";

$ftp->login($user,$pass)
	or die "Cannot login ", $ftp->message;
$ftp->put($local)
	or die "Put failed ", $ftp->message;    

print "Files uploaded OK\n";
$ftp->quit();

print "$local - hash: $hash size: $size\n";

exit 0;