#Imago <imagotrigger@gmail.com>
# efficently watches for a folder name change to trigger a subprocess

use strict;
use Win32::ChangeNotify;

my $mondir = "Z:\\deploy\\notify";
my $cmd = "C:\\perl\\bin\\perl.exe Z:\\deploy\\process.pl";

my $WatchDir;

$WatchDir = new Win32::ChangeNotify( $mondir ,1 , FILE_NOTIFY_CHANGE_DIR_NAME);
if( ! $WatchDir )
{
    exit();
}

$WatchDir->reset();
while (1) {
	        my $Result = $WatchDir->wait( 30 * 1000 );
        	if( $Result ) {
			if (-e $mondir."\\process") {
				open(LOG,">Z:\\deploy\\process.log");
				open(CMD,"$cmd |");
				while (<CMD>) {
					print LOG $_;
				}
				close CMD;
				close LOG;
			}
         		$WatchDir->reset();
        	}
		sleep(30);
	}
$WatchDir->close();

exit 0;

__END__