#Imago <imagotrigger@gmail.com>
# Creates client and server filelists
#  Archives the list and targets for deploy.pl

use strict;
use POSIX qw(strftime);
use File::Copy;

my %dupes = ();

print "Packaging deployment\n";

open(LIST,">C:\\betalist.txt");

opendir(DIR, "C:\\build\\Package\\Artwork\\");
my @art = readdir(DIR); 
closedir DIR;

opendir(DIR, "C:\\build\\FAZR6\\Artwork\\");
my @svn = readdir(DIR); 
closedir DIR;

opendir(DIR, "C:\\build\\Package\\tmp\\");
my @tmp = readdir(DIR); 
closedir DIR;

my $offset = strftime("%z", localtime());
if ($offset =~ /Daylight/i) {
	$offset = 5;
} else {
	$offset = 6;
}


#always include svn artwork on the list
foreach my $file (@svn) { 
	next if ($file =~ /^\./);
	my $cmd = "C:\\crc32.exe C:\\build\\FAZR6\\Artwork\\$file";
	my $cmd2 = "C:\\mscompress.exe C:\\build\\FAZR6\\Artwork\\$file";
	my ($modtime,$size)= (stat("C:\\build\\FAZR6\\Artwork\\$file"))[9,7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	print LIST "$dt $size $crc2 $file\n";
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\FAZR6\\Artwork\\${file}","C:\\build\\AutoUpdate\\Game\\$file");
	} else {
		`$cmd2`;
		move("C:\\build\\FAZR6\\Artwork\\${file}_","C:\\build\\AutoUpdate\\Game\\$file");
	}
}


#client list
foreach my $file (@art) {
	next if ($file =~ /^\./);
	next if ($file !~ /cc_09|tcor_|bgrnd/i); #demonstrates how to target specific client artwork using regexp filter
	my $cmd = "C:\\crc32.exe C:\\build\\Package\\Artwork\\$file";
	my $cmd2 = "C:\\mscompress.exe C:\\build\\Package\\Artwork\\$file";
	my ($modtime,$size)= (stat("C:\\build\\Package\\Artwork\\$file"))[9,7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	print LIST "$dt $size $crc2 $file\n";
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\Package\\Artwork\\${file}","C:\\build\\AutoUpdate\\Game\\$file");
	} else {
		`$cmd2`;
		move("C:\\build\\Package\\Artwork\\${file}_","C:\\build\\AutoUpdate\\Game\\$file");
	}
}

#include production AU artwork on the list!11!11 
foreach my $file (@tmp) { 
	next if ($file =~ /^\./);
	my $cmd = "C:\\crc32.exe C:\\build\\Package\\tmp\\expand\\$file";
	my $cmd2 = "C:\\mscompress.exe C:\\build\\Package\\tmp\\expand\\$file";
	my $cmd3 = "expand C:\\build\\Package\\tmp\\$file C:\\build\\Package\\tmp\\expand\\$file";
	`$cmd3`;
	my ($modtime,$size)= (stat("C:\\build\\Package\\tmp\\expand\\$file"))[9,7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	print LIST "$dt $size $crc2 $file\n";
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\Package\\tmp\\expand\\${file}","C:\\build\\AutoUpdate\\Game\\$file");
	} else {
		`$cmd2`;
		my $csize = (stat("C:\\build\\Package\\tmp\\expand\\${file}_"))[7];
		if ($csize > $size) {
			unlink "C:\\build\\Package\\tmp\\expand\\${file}_";
			copy("C:\\build\\Package\\tmp\\expand\\${file}","C:\\build\\AutoUpdate\\Game\\$file");
		}		
		move("C:\\build\\Package\\tmp\\expand\\${file}_","C:\\build\\AutoUpdate\\Game\\$file");
	}
}


#of course the latest build
my @objs = ("C:\\Allegiance.exe","C:\\Reloader.exe");
foreach my $file (@objs) {
	my $cmd = "C:\\crc32.exe $file";
	my $cmd2 = "C:\\mscompress.exe $file";
	my ($modtime,$size)= (stat("$file"))[9,7];
	next if (!$size);
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	my $bin = "Gurgle.crap";
	if ($file =~ /.*\\([^\\]+$)/) {
		$bin = $1;
	}	
	print LIST "$dt $size $crc2 $bin\n";
	`$cmd2`;
	move("${file}_","C:\\build\\AutoUpdate\\Game\\$bin");
}

close LIST;

unlink "C:\\build\\AutoUpdate\\Game.7z";

my $cmd3 = "\"C:\\Program Files\\7-Zip\\7z.exe\" a -t7z C:\\build\\AutoUpdate\\Game.7z C:\\build\\AutoUpdate\\Game\\* -xr!*Server -mx9";
system($cmd3);

#now the server list, always the build..
open(LIST,">C:\\serverlist.txt");
my @objs = ("C:\\AllSrv.exe","C:\\AGC.dll", "C:\\AllSrvUI.exe","C:\\build\\External\\dbghelp.dll");
foreach my $file (@objs) {
	my $cmd = "C:\\crc32.exe $file";
	my $cmd2 = "C:\\mscompress.exe $file";
	my ($modtime,$size)= (stat("$file"))[9,7];
	next if (!$size);
	next if ($dupes{server}{$file} == 1);
	$dupes{server}{$file} = 1;		
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	my $bin = "Gurgle.crap";
	if ($file =~ /.*\\([^\\]+$)/) {
		$bin = $1;
	}	
	print LIST "$dt $size $crc2 $bin\n";
	`$cmd2`;
	move("${file}_","C:\\build\\AutoUpdate\\Game\\Server\\$bin");
}

#always include svn artwork on the server list as well
foreach my $file (@svn) { 
	next if ($file =~ /^\./);
	my $cmd = "C:\\crc32.exe C:\\build\\FAZR6\\Artwork\\$file";
	my $cmd2 = "C:\\mscompress.exe C:\\build\\FAZR6\\Artwork\\$file";
	my ($modtime,$size)= (stat("C:\\build\\FAZR6\\Artwork\\$file"))[9,7];
	next if (!$size);
	next if ($dupes{server}{$file} == 1);
	$dupes{server}{$file} = 1;		
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	print LIST "$dt $size $crc2 $file\n";
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\FAZR6\\Artwork\\${file}","C:\\build\\AutoUpdate\\Game\\Server\\$file");
	} else {
		`$cmd2`;
		move("C:\\build\\FAZR6\\Artwork\\${file}_","C:\\build\\AutoUpdate\\Game\\Server\\$file");
	}
}

#lastly include whatever is on production AU
foreach my $file (@tmp) { 
	next if ($file =~ /^\./);
	my $cmd = "C:\\crc32.exe C:\\build\\Package\\tmp\\expand\\$file";
	my $cmd2 = "C:\\mscompress.exe C:\\build\\Package\\tmp\\expand\\$file";
	my $cmd3 = "expand C:\\build\\Package\\tmp\\$file C:\\build\\Package\\tmp\\expand\\$file";
	`$cmd3`;
	my ($modtime,$size)= (stat("C:\\build\\Package\\tmp\\expand\\$file"))[9,7];
	next if (!$size);
	next if ($dupes{server}{$file} == 1);
	$dupes{server}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	$size = sprintf("%09d",$size);
	my $dt = strftime("%Y/%m/%d %H:%M:%S",localtime($modtime + (3600 * $offset)));
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	print LIST "$dt $size $crc2 $file\n";
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\Package\\tmp\\expand\\${file}","C:\\build\\AutoUpdate\\Game\\Server\\$file");
	} else {
		`$cmd2`;
		my $csize = (stat("C:\\build\\Package\\tmp\\expand\\${file}_"))[7];
		if ($csize > $size) {
			unlink "C:\\build\\Package\\tmp\\expand\\${file}_";
			copy("C:\\build\\Package\\tmp\\expand\\${file}","C:\\build\\AutoUpdate\\Game\\Server\\$file");
		}		
		move("C:\\build\\Package\\tmp\\expand\\${file}_","C:\\build\\AutoUpdate\\Game\\Server\\$file");
	}
}

close LIST;


my $cmd0 = "C:\\upx.exe -q -9 -f -o C:\\build\\AutoUpdate\\Game\\Server\\AutoUpdate.exe C:\\AutoUpdate.exe";
`$cmd0`;

unlink "C:\\build\\AutoUpdate\\Server.7z";

my $cmd3 = "\"C:\\Program Files\\7-Zip\\7z.exe\" a -t7z C:\\build\\AutoUpdate\\Server.7z C:\\build\\AutoUpdate\\Game\\Server\\* -mx9";
system($cmd3);
exit 0;

