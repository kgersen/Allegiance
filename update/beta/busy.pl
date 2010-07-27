#Imago <imagotrigger@gmail.com
# Waits for lobby to become idle before exiting OK
#  Uses gameinfod's sharedmem

use strict;
use Win32::MMF;

my $mem = new Win32::MMF;
my $lobby = $mem->getvar('gameinfod');

my $numnotplaying = $lobby->{numNotPlaying};    
my $numplayers = $lobby->{numPlayers};     
if ($numplayers > $numnotplaying) {
	my $itrs = 0;
	do {
		exit 1 if ($itrs > 514); #exit ERROR after 3 hours
		$lobby = $mem->getvar('gameinfod');
		$numnotplaying = $lobby->{numNotPlaying};    
		$numplayers = $lobby->{numPlayers};  
		sleep(21) if ($numplayers > $numnotplaying);
		$itrs++;
	} while ($numplayers > $numnotplaying);
}
exit 0;