#Imago <imagotrigger@gmail.com>
# The subprocess from service.pl
#  Extracts and positions uploads, runs upgrade.pl
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

mkdir ("Z:\\deploy\\temp");
my $cmd = "Z:\\Deploy\\7za.exe x -y -oZ:\\deploy\\temp Z:\\deploy\\Server.7z";
system($cmd);

my $cmd = "C:\\Perl\\bin\\perl.exe Z:\\deploy\\upgrade.pl";
system($cmd);

move("Z:\\deploy\\notify\\process", "Z:\\deploy\\notify\\ready");
unlink 'process.pid';
exit 0;