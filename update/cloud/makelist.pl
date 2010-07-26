#Imago <imagotrigger@gmail.com>
# Creates client custom file list for and prepares 7z zip files
#  targets the nsis.pl and use on Nirvanix CloudNAS

use strict;
use File::Copy;

my %dupes = ();

print "Packaging deployment\n";

open(LIST,">C:\\Inetpub\\wwwroot\\build\\Update\\filelist.html");

opendir(DIR, "C:\\build\\Package\\Artwork\\");
my @art = readdir(DIR); 
closedir DIR;

opendir(DIR, "C:\\build\\FAZR6\\Artwork\\");
my @svn = readdir(DIR); 
closedir DIR;

opendir(DIR, "C:\\build\\Package\\tmp\\expand");
my @tmp = readdir(DIR); 
closedir DIR;

#always include svn artwork on the list
foreach my $file (@svn) { 
	next if ($file =~ /^\./);
	my $cmd = "C:\\crc32.exe C:\\build\\FAZR6\\Artwork\\$file";
	my $cmd2 = "\"C:\\Program Files\\7-Zip\\7z.exe\" a -t7z C:\\build\\Package\\tmp\\cloud\\$file.zip C:\\build\\FAZR6\\Artwork\\$file -mx9";
	my ($size)= (stat("C:\\build\\FAZR6\\Artwork\\$file"))[7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\FAZR6\\Artwork\\$file","C:\\build\\Package\\tmp\\cloud\\$file");
		print LIST "$crc2|$file\n";
	} else {
		`$cmd2`;
		print LIST "$crc2|$file.zip\n";
	}
}

#of course the latest build
my @objs = ("C:\\Allegiance.exe","C:\\Reloader.exe");
foreach my $file (@objs) {
	my $cmd = "C:\\crc32.exe $file";
	my $cmd2 = "\"C:\\Program Files\\7-Zip\\7z.exe\" a -t7z C:\\build\\Package\\tmp\\cloud\\$file.zip $file -mx9";
	my ($size)= (stat("$file"))[7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	my $bin = "Gurgle.crap";
	if ($file =~ /.*\\([^\\]+$)/) {
		$bin = $1;
	}		
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("$file","C:\\build\\Package\\tmp\\cloud\\$file");
		print LIST "$crc2|$bin\n";
	} else {
		`$cmd2`;
		print LIST "$crc2|$bin.zip\n";
	}
}

#include production AU artwork on the list!11!11 
foreach my $file (@tmp) { 
	next if ($file =~ /^\./);
	my $cmd = "C:\\crc32.exe C:\\build\\Package\\tmp\\expand\\$file";
	my $cmd2 = "\"C:\\Program Files\\7-Zip\\7z.exe\" a -t7z C:\\build\\Package\\tmp\\cloud\\$file.zip C:\\build\\Package\\tmp\\expand\\$file -mx9";
	my ($size)= (stat("C:\\build\\Package\\tmp\\expand\\$file"))[7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\Package\\tmp\\expand\\$file","C:\\build\\Package\\tmp\\cloud\\$file");
		print LIST "$crc2|$file\n";
	} else {
		`$cmd2`;
		print LIST "$crc2|$file.zip\n";
	}
}

#client list
foreach my $file (@art) {
	next if ($file =~ /^\./);
	next if ($file !~ /cc_09|tcor_|bgrnd/i); #demonstrates how to target specific client artwork using regexp filter
	my $cmd = "C:\\crc32.exe C:\\build\\Package\\Artwork\\$file";
	my $cmd2 = "\"C:\\Program Files\\7-Zip\\7z.exe\" a -t7z C:\\build\\Package\\tmp\\cloud\\$file.zip C:\\build\\Package\\Artwork\\$file -mx9";
	my ($size)= (stat("C:\\build\\Package\\Artwork\\$file"))[7];
	next if (!$size);
	next if ($dupes{client}{$file} == 1);
	$dupes{client}{$file} = 1;	
	my $crc = `$cmd`;
	chomp $crc;
	my $crc2 = "0" x ( 8 - length($crc) ) . $crc; 
	
	if ($size < 2048 || $file =~ /\.avi|\.ogg|\.png/i) {
		copy("C:\\build\\Package\\Artwork\\$file","C:\\build\\Package\\tmp\\cloud\\$file");
		print LIST "$crc2|$file\n";
	} else {
		`$cmd2`;
		print LIST "$crc2|$file.zip\n";
	}
}

close LIST;
exit 0;

