#Imago <imagotrigger@gmail.com>
# Touch helper for durations in annoucnments

use strict;

my $open = ($ARGV[0]) ? 'start' : 'finish';

open(TOUCH,">C:\\$open");
print "\n";
close TOUCH;

exit 0;