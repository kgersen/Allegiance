#Fuh-zz <fuzzylunkinz@gmail.com>
# Send ART and PDB installer components to egretfiles FTP as a mirror

use strict;
use Getopt::Long;
use Net::FTP;
use Digest::MD5;

my $server = "ftp.egretfiles.com";
my $user = "build\@egretfiles.com";

open(PASS,"C:\\egretfiles.passwd");
my $pass = <PASS>;
close PASS;

my $ART = "C:\\inetpub\\wwwroot\\build\\AllegR6ART_b";
my $PDB = "C:\\inetpub\\wwwroot\\build\\AllegR6PDB_b";
GetOptions("b=s" => \$build, "r=s" => \$revision, "v=s" => \$version);
if($version == "1.1"){
	$ART .= "$build_r$revision.exe";
	$PDB .= "$build_r$revision.exe";
}
else if($version == "1.0"){
	$ART .= "$build_r$revision.exe";
	$PDB .= "$build_r$revision.exe";
}
else{ die "No version specified! Command line switch '-v' either 1.1 or 1.0"; }

my $ftp = Net::FTP->new($server, Debug => 0, Port => 21, Passive => 1)
	or die "Cannot connect to '$server' $@";

$ftp->login($user,$pass)
	or die "Cannot login ", $ftp->message;
print "Uploading PDB to $server\n";
$ftp->put($PDB)
	or die "Put failed ", $ftp->message;    
print "Uploading ART to $server\n";
$ftp->put($ART)
	or die "Put failed ", $ftp->message; 

print "Files uploaded OK\n";
$ftp->quit();


exit 0;