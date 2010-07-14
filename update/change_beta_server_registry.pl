#Imago <imagotrigger@gmail.com>
# Sets 1.1 ArtPath to value passed as command argument

use strict;
use Win32::Registry;

my $reg;

$::HKEY_LOCAL_MACHINE->Open("SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\1.1\\Server", $reg) 
	or die "Can't open reg: $^E";

my ($type, $value, $reserved);
$reg->SetValueEx("ArtPath", $reserved, REG_SZ, $ARGV[0]) or die "No write $^E";
$reg->QueryValueEx("ArtPath", $type, $value) or die "No read $^E";
print "ArtPath changed to $value\n";
$reg->SetValueEx("Game1", $reserved, REG_SZ, "CDN runs b".$ARGV[1]." r".$ARGV[2]) or die "No write $^E";
$reg->QueryValueEx("Game1", $type, $value) or die "No read $^E";
print "Game1 changed to $value\n";
exit 0;
