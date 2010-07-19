#Imago <imagotrigger@gmail.com>
# efficently watches for a folder name change to trigger a subprocess

use strict;
use Win32::ChangeNotify;

my $mondir = "C:\\Allegiance\\deploy\\notify";
my $cmd = "C:\\perl\\bin\\perl.exe C:\\Allegiance\\deploy\\process.pl";

my $WatchDir;

$WatchDir = new Win32::ChangeNotify( $mondir ,1 , FILE_NOTIFY_CHANGE_DIR_NAME);
if( ! $WatchDir )
{
    exit();
}

$WatchDir->reset();
while (1) {
	        my $Result = $WatchDir->wait( 120 * 1000 );
        	if( $Result ) {
			if (-e $mondir."\\process") {
				open(LOG,">C:\\Allegiance\\deploy\\test_process.log");
				open(CMD,"$cmd |");
				while (<CMD>) {
					print LOG $_;
				}
				close CMD;
				close LOG;
			}
         		$WatchDir->reset();
        	}
		sleep(60);
	}
$WatchDir->close();

exit 0;

__END__