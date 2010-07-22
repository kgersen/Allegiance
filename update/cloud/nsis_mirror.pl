#Fuh-zz <fuzzylunkinz@gmail.com>
# Send ART and PDB installer components to egretfiles FTP as a mirror
#Imago 7/10 - Changed to FTP to the trac server via the virtual network which is rooted to Nirvanix's cloudNAS

use strict;
use Getopt::Long;
use Net::FTP;

my $build = "";
my $revision = "";
my $version = "";
my $ART = "C:\\inetpub\\wwwroot\\build\\AllegR6ART_b";
my $PDB = "C:\\inetpub\\wwwroot\\build\\Alleg";
my $installer = "C:\\inetpub\\wwwroot\\build\\"; #Imago 7/10
my $server = "10.0.0.5";
my $user = "cloudnas";

open(PASS,"C:\\trac.passwd");
my $pass = <PASS>;
close PASS;



GetOptions("b=s" => \$build, "r=s" => \$revision, "v=s" => \$version);
if($version eq "1.1"){
	$ART .= "${build}_r${revision}.exe";
	$PDB .= "R6PDB_b${build}_r${revision}.exe";
	$installer .= "R6_b${build}_r${revision}.exe"
}
elsif($version eq "1.0"){	# Fuzz 07/18 ART is already pushed in 1.1
	$PDB .= "PDB_b${build}_r${revision}.exe";
	$installer .= "Alleg_b${build}_r${revision}.exe"
}
else{ die "No version specified! Command line switch '-v' either 1.1 or 1.0"; }

my $ftp = Net::FTP->new($server, Debug => 0, Port => 21, Passive => 0)
	or die "Cannot connect to '$server' $@";

$ftp->login($user,$pass)
	or die "Cannot login ", $ftp->message;
$ftp->binary;
print "Uploading PDB to $server\n";
$ftp->put($PDB)
	or die "Put failed ", $ftp->message;
print "Uploading Installer to $server\n";	
$ftp->put($installer)
	or die "Put failed ", $ftp->message;	
if($version eq "1.1"){
	print "Uploading ART to $server\n";
	$ftp->put($ART)
		or die "Put failed ", $ftp->message;
}

print "Files uploaded OK\n";
$ftp->quit();


exit 0;