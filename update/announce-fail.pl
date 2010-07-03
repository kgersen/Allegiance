#Imago <imagotrigger@gmail.com>
# Send a ms-build.pl failure announcment to TracBot

use strict;
use RPC::XML;
use RPC::XML::Client;

use Data::Dumper;

my $b = $ARGV[0];
my $c = $ARGV[1];

my $msg = "b$b oh noes FAILED compiling $c project! See http://trac.alleg.net/build/R6/$b for details";

my $string = qq{<struct><member><name>file_count</name><value><int>1</int></value></member><member><name>rev</name><value><int>534</int></value></member><member><name>url</name><value><string>http://trac.alleg.net/changeset/534</string></value></member><member><name>path</name><value><string>10</string></value></member><member><name>author</name><value><string>Imago</string></value></member><member><name>message</name><value><string>$msg</string></value></member><member><name>trac</name><value><struct><member><name>name</name><value><string>Allegiance</string></value></member><member><name>url</name><value><string>http://trac.alleg.net/</string></value></member><member><name>description</name><value><string>Free Allegiance</string></value></member></struct></value></member></struct>};

my $req = RPC::XML::ParserFactory->new()->parse($string);

my $cli = RPC::XML::Client->new('http://10.0.0.5:53312/');
my $resp = $cli->send_request('ircannouncer.notify','build',$req);
