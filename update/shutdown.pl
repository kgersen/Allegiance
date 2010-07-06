#Imago <imagotrigger@gmail.com>
# Shuts down a server /w timer message
#  This is almost identical to AutoUpdate.exe shutdown

use strict;
use Win32::OLE;

my $utl  = Win32::OLE->new("AllSrv.AdminSession");
my $srv = $utl->Server;
my $games = $srv->Games;

my %stats;

my $biggameid = -1; my $bigcnt = 0; my $hasships = 0;
 foreach my $value (in $games) {
	 my $gameid = $value->GameID;
	 $stats{$gameid}{name} = $value->Name;
	my $teams = $value->Teams;
	 foreach my $team (in $teams) {
			my $stations = $team->Stations;
			my $count =  $stations->Count;
			$stats{$gameid}{numteams}++ if ($count);
			$stats{$gameid}{numstations} += $count;
	 }	 
	 my $cnt = 0;
	 my $ships = $value->Ships;
	 foreach my $ship (in $ships) {
		$cnt++;
	 }
	 if ($cnt > $bigcnt) {
		 $biggameid = $value->GameID;
		 $bigcnt = $cnt;
		 $hasships = 1;
	 }

}

if ($hasships && $stats{$biggameid}) {
	my $stations = $stats{$biggameid}{numstations};
	if (!$stations) {
		print "Players are connected but idle...\n";
		$srv->SendMsg("Shutting down in one minute.");
		sleep(30);
		$srv->SendMsg("Shutting down in 30 seconds.");
		sleep(25);		
		$srv->SendMsg("Shutting down in 5 seconds.");
		sleep(2);	
		$srv->SendMsg("Shutting down in 3 seconds.");
		sleep(1);				
		$srv->SendMsg("Shutting down in 2 seconds.");
		sleep(1);				
		$srv->SendMsg("Shutting down in 1 seconds.");
		sleep(1);
		$utl->Stop;
	} else {
		if ($stats{$biggameid}{numteams} > 1) {
			my $game = $stats{$biggameid}{name};
			$game .= ' Game' if ($game !~ / game$/i);
			$game = 'The '.$game if ($game !~ /^the /i);
			print "Players are in a $game...\n";
			$srv->SendMsg("Shutting down after $game is over.");
			my $i = 0;
			while(GetStations($biggameid) != 0) {
				sleep(5);
				$i++;
				if ($i >= 2160) {
					$srv->SendMsg("Aborting shutdown, epic game!");
					print "Aborting!\n";
					exit 1;
				}
				if ($i % 84 == 0) {
					print "Still waiting for $game to finish...\n";
					$srv->SendMsg("Shutting down after $game is over.");
				}
			}
			print "Finished\n";
			$srv->SendMsg("Shutting down in four minutes.");
			sleep(120);					
			$srv->SendMsg("Shutting down in two minutes.");
			sleep(90);
			$srv->SendMsg("Shutting down in 30 seconds.");
			sleep(25);		
			$srv->SendMsg("Shutting down in 5 seconds.");
			sleep(2);	
			$srv->SendMsg("Shutting down in 3 seconds.");
			sleep(1);				
			$srv->SendMsg("Shutting down in 2 seconds.");
			sleep(1);				
			$srv->SendMsg("Shutting down in 1 seconds.");
			sleep(1);
			$utl->Stop;					
		} else {
			print "Player(s) are by themselves, give them a few...\n";
			$srv->SendMsg("Shutting down in two minutes.");
			sleep(90);					
			$srv->SendMsg("Shutting down in 30 seconds.");
			sleep(25);		
			$srv->SendMsg("Shutting down in 5 seconds.");
			sleep(2);	
			$srv->SendMsg("Shutting down in 3 seconds.");
			sleep(1);				
			$srv->SendMsg("Shutting down in 2 seconds.");
			sleep(1);				
			$srv->SendMsg("Shutting down in 1 seconds.");
			sleep(1);
			$utl->Stop;
		}
	}
} else {
	$utl->Stop;
}

print "Stopping AllSrv service\n";	
my $s = Win32::OLE->GetObject("WinNT://cdn/AllSrv,service");
$s->Stop();

exit(0);




sub GetStations {
	my $ret = 0;
	my $gameid = shift;
	my $srv = $utl->Server;
	my $games = $srv->Games;
	foreach my $value (in $games) {
		next if ($value->GameID != $gameid);
		my $teams = $value->Teams;
		foreach my $team (in $teams) {
			my $stations = $team->Stations;
			$ret +=  $stations->Count;
		 }			
	}
	return $ret;
}

__END__
