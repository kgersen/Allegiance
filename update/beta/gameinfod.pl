#Imago <imagotrigger@gmail.com
# Lobby reports it's game info to itself, this is a daemon to deal /w it
#  Let's store the info in some sharedmem

use strict;
use IO::Socket;
use Convert::Binary::C;
use Win32::MMF;

my $sock = IO::Socket::INET->new(LocalPort => 2000, Proto => 'udp') or die "socket init: $@";      
my $mem = new Win32::MMF;
my $c = new Convert::Binary::C;
$c->parse_file("Z:\\Deploy\\gameinfod.h");
$c->parse_file("Z:\\Deploy\\zgameinfo.h");
$c->configure(ByteOrder => 'BigEndian', LongSize  => 4, ShortSize => 2, UnsignedChars => 1,  Alignment => 4);
die "Couldn't load structure\n" if (!$c->def('ZGameServerInfoMsg'));
while (1) {
	open(LOG,">>Z:\\Deploy\\gameinfod.log");
	print LOG (localtime)."> Info: Listening...\n";
	close LOG;
	$sock ||= IO::Socket::INET->new(LocalPort => 2000, Proto => 'udp') or die "socket rest: $@";
	my $newmsg;
	while ($sock->recv($newmsg, 4340)) {
	    my $perl = $c->unpack('ZGameServerInfoMsg', $newmsg);
	    my %hash = %$perl;
	    my @info = @{$hash{info}};
	    my $lobby = $info[0];
	    $mem->setvar('gameinfod', $lobby);
	} 
	open(LOG,">Z:\\Deploy\\gameinfod.log");
	print LOG (localtime). "> Error: recv: $!\n";
	close LOG;
}
exit 0;