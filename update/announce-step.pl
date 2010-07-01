#Imago <imagotrigger@gmail.com>
# Send a step completed announcment to TracBot

use strict;
use RPC::XML;
use RPC::XML::Client;

my $b = $ARGV[0];
my $c = $ARGV[1];
my $step= $ARGV[2];
my $msg; my $duration;

if ($step) {
my $modtime = (stat("C:\\stepstart"))[9];
my $thetime = (stat("C:\\stepend"))[9];

$duration = $thetime - $modtime;
if ($duration >= 60) {
	$duration = sprintf("%u",$duration / 60);
	if ($duration == 1) {
		$duration = $duration." minute";
	} else {
		$duration = $duration." minutes";
	}
} else {
	$duration = sprintf("%u",$duration);
	if ($duration == 1) {
		$duration = $duration." second";
	} else {
		$duration = $duration." seconds";
	}	
}
}
my $open = 'stepstart';
open(TOUCH,">C:\\$open");
print "\n";
close TOUCH;

if ($step)  {
	$msg = "b$b Om nom nom took $duration - $step";
} else {
	$msg = "b$b Sending a slave to work on revision $c at http://alleg.scarybugs.net/build/R6/$b";	
}

my $string = qq{<struct><member><name>file_count</name><value><int>1</int></value></member><member><name>rev</name><value><int>534</int></value></member><member><name>url</name><value><string>http://alleg.scarybugs.net/changeset/534</string></value></member><member><name>path</name><value><string>10</string></value></member><member><name>author</name><value><string>Imago</string></value></member><member><name>message</name><value><string>$msg</string></value></member><member><name>trac</name><value><struct><member><name>name</name><value><string>Allegiance</string></value></member><member><name>url</name><value><string>http://alleg.scarybugs.net/</string></value></member><member><name>description</name><value><string>Free Allegiance</string></value></member></struct></value></member></struct>};

my $req = RPC::XML::ParserFactory->new()->parse($string);

my $cli = RPC::XML::Client->new('http://10.0.0.5:53312/');
my $resp = $cli->send_request('ircannouncer.notify','build',$req);	

