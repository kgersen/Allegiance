#Imago <imagotrigger@gmail.com>
# The subprocess from service.pl
#  runs tests
#   Sets notification folder back to ready state


use strict;
use File::Copy;

open(PID,'process.pid');
my $pid = <PID>;
close(PID);
exit -1 if ($pid && $pid != $$);
open(PID,'>process.pid');
print PID $$;
close(PID);

mkdir ("C:\\Allegiance\\deploy\\temp");

my $cmd = "C:\\Perl\\bin\\perl.exe C:\\Allegiance\\deploy\\test_installer.pl";
system($cmd);

my $cmd = "C:\\Perl\\bin\\perl.exe C:\\Allegiance\\deploy\\test_sanity.pl";
system($cmd);

my $cmd = "C:\\Perl\\bin\\perl.exe C:\\Allegiance\\deploy\\test_launch.pl";
system($cmd);

my $cmd = "C:\\Perl\\bin\\perl.exe C:\\Allegiance\\deploy\\test_uimod.pl";
system($cmd);

move("C:\\Allegiance\\deploy\\notify\\process", "C:\\Allegiance\\deploy\\notify\\ready");
unlink 'process.pid';
exit 0;