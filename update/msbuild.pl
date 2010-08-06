use strict;
my ($path,$name) = @ARGV;
my $sse2 = ($name eq 'test1') ? "-SSE2" : '';
my $cmd = qq{msbuild.exe /m $path\\VS2010\\Allegiance.sln /logger:Rodemeyer.MsBuildToCCNet.MsBuildToCCNetLogger,$path\\update\\logger.dll;$path\\update\\msbuild.xml /noconlog /nologo /p:Configuration=FZRetail$sse2 /t:rebuild};
print "Executing $cmd on $path for slave $name\n";
my $ret = system($cmd);
if ($ret != 0) {
	print STDERR "Error, did not return OK: $ret\n";
	exit $ret;
}
print "Done\n";
exit 0;