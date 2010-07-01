#Imago <imagotrigger@gmail.com>
# Process wrapper for failure alarm and cleanup

use strict;
use Win32::Process qw(STILL_ACTIVE NORMAL_PRIORITY_CLASS INFINITE);

  sub ErrorReport{
        print Win32::FormatMessage( Win32::GetLastError() );
    }
my $ProcessObj ; my $exitcode;
    my $wut = Win32::Process::Create($ProcessObj,
                                $ARGV[0],
                                $ARGV[1],
                                0,
                                NORMAL_PRIORITY_CLASS,
                                $ARGV[2]) || die ErrorReport();

    my $wut = ($ARGV[3]) ? $ProcessObj->Wait(INFINITE) : $ProcessObj->Wait(30 * 1000);
    my $pid = $ProcessObj->GetProcessID();
    my $huh = $ProcessObj->GetExitCode($exitcode);
    if ($exitcode == STILL_ACTIVE) {
		print STDERR "FATAL: Had to kill a process!\n";
		$ProcessObj->Kill(1);
		sleep(3);
		my $huh = $ProcessObj->GetExitCode($exitcode);
		if ($exitcode == STILL_ACTIVE) {
			my $cmd = "TASKKILL /PID $pid /T /F";
			`$cmd`;
		}
		exit 1;
    }
    exit 0;