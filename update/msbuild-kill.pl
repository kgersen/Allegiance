#Imago <imagotrigger@gmail.com>
# Used to terminate any latent visual studio build processes
#  Task kill throws error in OK conditions from bitten so this wraps it.

use strict;

my $cmd = "TASKKILL /IM mspdbsrv.exe /T /F";
system($cmd);
my $cmd = "TASKKILL /IM msbuild.exe /T /F";
system($cmd);

exit 0; #always OK