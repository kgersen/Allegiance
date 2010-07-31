#!/usr/bin/perl

# Parses the msbuild.xml as build log for bitten recipe
# i.e msbuild /logger:Rodemeyer.MsBuildToCCNet.MsBuildToCCNetLogger,C:\logger.dll;C:\msbuild.xml
# Imago <imagotrigger@gmail.com>

use strict;
use XML::Simple;
use Win32::AbsPath qw(Relative2Absolute);
use Data::Dumper;

my $xml = new XML::Simple;
my $data = $xml->XMLin("C:\\build\\FAZR6\\update\\msbuild.xml");
my $ignore = "MSB8012|LNK4221";
#print Dumper(\$data);
my $basepath = $ARGV[0];
my $build = $ARGV[1];
my $projcount = 0;
my $lasterr;
foreach my $project (keys %{$data->{project}}) {
	next if ($project =~ /\.metaproj|.sln|MSBuild/);
	my $sproject = $project; $sproject =~ s/\.csproj|\.vcxproj|\.vcproj//i;
	my $projdir = $data->{project}{$project}{dir};
	delete $data->{project}{$project}{dir};

	my $notok = 0; 
	foreach my $key (keys %{$data->{project}{$project}}) {
		my $level = $key;
		$notok = 1 if ($level eq 'error');
		next if ($key eq 'message');
		if (!$data->{project}{$project}{$key}{name}) {
			foreach my $file (keys %{$data->{project}{$project}{$key}}) {
				if ($data->{project}{$project}{$key}{$file}{code} !~ /$ignore/i ) {
					my $path = $data->{project}{$project}{$key}{$file}{dir}.'\\'.$file;
					my $filepos;
					if (-e $path) {
						Relative2Absolute $path; $path =~ s/C:\\build\\fazr6/$basepath/i; $path =~ s/\\/\//g;
						$filepos = '@ '.$path.$data->{project}{$project}{$key}{$file}{pos};
					} else {
						$filepos = ($file ne '') ? "\@ $file$data->{project}{$project}{$key}{$file}{pos}" : "";
					}
					($level eq 'error') ?
						print STDERR "\n$data->{project}{$project}{$key}{$file}{code}: $data->{project}{$project}{$key}{$file}{message} $filepos\n" :
						print "\n$data->{project}{$project}{$key}{$file}{code}: $data->{project}{$project}{$key}{$file}{message} $filepos\n";



				}
			}
		} else {
			if ($data->{project}{$project}{$key}{code} !~ /$ignore/i ) {
				my $path = $data->{project}{$project}{$key}{dir} .'\\'.$data->{project}{$project}{$key}{name};
				my $filepos;
				if (-e $path) {
					Relative2Absolute $path; $path =~ s/C:\\build\\fazr6/$basepath/i; $path =~ s/\\/\//g;
					$filepos = '@ '. $path.$data->{project}{$project}{$key}{pos};
				} else {
					$filepos = '\@ '. $data->{project}{$project}{$key}{name}.$data->{project}{$project}{$key}{pos};
				}
				($level eq 'error') ?
					print STDERR "\n$data->{project}{$project}{$key}{code}: $data->{project}{$project}{$key}{message} $filepos\n" :
					print "\n$data->{project}{$project}{$key}{code}: $data->{project}{$project}{$key}{message} $filepos\n";
			}
		}
	}
	if ($notok) {
		print STDERR "\n$sproject...ERROR!\n";
		$lasterr = $sproject;
	} else {
		print "$sproject...OK!\n";
	}
	$projcount++;
}

print "Compiled $projcount projects with $data->{error_count} error(s)\n";
if ($lasterr) {
	my $cmd = "perl C:\\build\\FAZR6\\update\\announce-fail.pl $build $lasterr";
	`$cmd`;
}
exit;