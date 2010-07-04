#Imago <imagotrigger@gmail.com>
# Send files to fazdev FTP /w a ChangeNotiy folder
#  Waits for remote completion notification
#   IMPORTANT!!!!  NYI: call an external to insert the build's Beta 1.1 Allegiance.exe ASGS token

use strict;
use Net::FTP;
use Digest::MD5;


open(PASS,"C:\\fazdev.passwd");
my $pass = <PASS>;
close PASS;

print "Uploading files to fazdev\n";
my $file = "C:\\Allegiance.exe";
open(FILE, $file) or die "Can't open '$file': $!";
binmode(FILE);
my $hash = Digest::MD5->new->addfile(*FILE)->hexdigest;
close FILE;
my $size= (stat("C:\\Allegiance.exe"))[7];
my $ftp = Net::FTP->new("fazdev.alleg.net", Debug => 0, Port => 21122)
      or die "Cannot connect to beta $@";

    $ftp->login("deploy",$pass)
      or die "Cannot login ", $ftp->message;
       $ftp->put("C:\\FAZBeta.cfg")
      or die "put failed ", $ftp->message;
       $ftp->put("C:\\motdR6.mdl")
      or die "put failed ", $ftp->message;      
       $ftp->put("C:\\serverlist.txt")
      or die "put failed ", $ftp->message;      
       $ftp->binary;     
       $ftp->put("C:\\build\\AutoUpdate\\Filelist.txt")
      or die "put failed ", $ftp->message;
       $ftp->put("C:\\build\\AutoUpdate\\Game.7z")
      or die "put failed ", $ftp->message;     
       $ftp->put("C:\\build\\AutoUpdate\\Server.7z")
      or die "put failed ", $ftp->message; 
       $ftp->put("C:\\build\\FAZR6\\objs10\\FZRetail\\Lobby\\AllLobby.exe")
      or die "put failed ", $ftp->message;       
  
 print "Files uploaded OK\n";
$ftp->rename("notify/ready","notify/process") or die "notify failed ", $ftp->message;
print "Waiting for fazdev to upgrade\n";
my $count = 0;
my $bfail = 0;
while (1) {
	$count++;
	my @dirs = $ftp->ls('notify');
	if ($dirs[0] eq 'ready') {
		last;
	}
	sleep(10);
	if ($count > 30) {
		$bfail = 1;
		last;
	}
}

 $ftp->ascii;     
 
        $ftp->get("process.log","C:\\process.log")
      or die "put failed ", $ftp->message;      

if ($bfail) {
	print "Remote deployment process did not return to a ready state\n";
	exit 1;
} else {
	print "Finished! Here is the log:\n";
	open(LOG,"C:\\process.log");
	while (<LOG>) {
		print "fazdev.alleg.net>\t".$_;
	}
	close LOG;
}

$ftp->quit();

#print "Allegiance.exe - hash: $hash size: $size\n";

exit 0;