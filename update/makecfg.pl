#Imago <imagotrigger@gmail.com>
# Inserts CRC and size info into FAZBeta.cfg

use strict;
use File::Copy;

print "Updating FAZBeta.cfg\n";

unlink "C:\\build\\AutoUpdate\\Filelist.txt";
my $cmd3 = "C:\\crc32.exe C:\\betalist.txt";
unlink "C:\\betalist.txt_";
my $cmd4 = "C:\\mscompress.exe C:\\betalist.txt";
`$cmd4`;
move("C:\\betalist.txt_","C:\\build\\AutoUpdate\\Filelist.txt");
my $crc = `$cmd3`;
chomp $crc;
my $size = (stat("C:\\betalist.txt"))[7];

$cmd3 = "C:\\crc32.exe C:\\motdR6.mdl";
my $crc2 = `$cmd3`;
chomp $crc2;

$cmd3 = "C:\\crc32.exe C:\\serverlist.txt";
my $crc3 = `$cmd3`;
chomp $crc3;
my $size2 = (stat("C:\\serverlist.txt"))[7];

$cmd3 = "C:\\crc32.exe C:\\build\\AutoUpdate\\Game\\Server\\AutoUpdate.exe";
my $crc4 = `$cmd3`;
chomp $crc4;

open(CFG,'>C:\\FAZBeta.cfg');

print CFG qq{[Allegiance]
PublicLobby=fazdev.alleg.net
PublicMessageURL=http://fazdev.alleg.net/FAZ/motdR6.mdl
LobbyClientPort=2302
LobbyServerPort=2303
FilelistSite=http://fazdev.alleg.net
FilelistDirectory=/FAZ/AU
FileListCRC = $crc
FilelistSize = $size
PublicMessageCRC = $crc2
TrainingURL=http://www.freeallegiance.org/FAW/index.php/Learning_guide
ZoneAuthGUID={00000000-0000-0000-C000-000000000046}
ZAuth=fazdev.alleg.net
UsePassport=0
PassportUpdateURL=http://www.freeallegiance.org/
ZoneEventsURL=http://www.freeallegiance.org/
ZoneEventDetailsURL=http://fazdev.alleg.net/FAZ/motdR6.mdl
ClubLobby=fazdev.alleg.net
Club=fazdev.alleg.net
ClubMessageURL=http://fazdev.alleg.net/FAZ/motdR6.mdl
ClubMessageCRC=$crc2


[Cores]
zone_core=AZ 1.25
dn_000460=DN 4.60
GoDII_04=GoD II 0.4
sw_a103=Starwars 1.03a
rps55=RPS 5.5
RTc006a=EoR 6.0a
PC2_019=PookCore II b19
VoS000090=VoS
cc_09=CC 9
cc_09b=CC 9b
tcor_02=Test Core 2

[OfficialCores]
cc_09b=true
tcor_02=true

[OfficialServers]
FAZDev=12.96.40.28
CDN=216.17.7.9

[AllSrvUI]
Site=http://fazdev.alleg.net
AutoUpdateURL=http://fazdev.alleg.net/FAZ/AU/AutoUpdate.exe
AutoUpdateCRC=$crc4
Directory=/FAZ/AU
FileListCRC = $crc3
FilelistSize = $size2



; THIS IS A VALID CONFIG FILE

};
close CFG;
exit 0;