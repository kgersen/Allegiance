#Imago <imagotrigger@gmail.com>
# Defines 1.0 keys and/or VS2008 compat. in the solution

use strict;
use File::Copy;
use Data::Dumper;

my ($doprod,$do9) = @ARGV;

#my $solpath = "C:\\asvn\\VS2010";
my $solpath = "C:\\build\\FAZR6\\VS2010";

my @projs = glob "$solpath\\*.vcxproj";

foreach my $proj (@projs) {
	open(VCX,$proj);
	my @lines = <VCX>;
	close VCX;
	move("$proj","$proj-prev");
	open(VCX,">$proj");

	my $bfound = 0;
	my $bfound2 = 0;
	foreach my $line (@lines) {
		my $name = $proj;
		$name =~ s/$solpath//i;		
		if ($line =~ /\<ItemDefinitionGroup Condition="'\$\(Configuration\)\|\$\(Platform\)'=='FZRetail\|Win32'"\>/i) {
			$bfound = 1;
		}
		if ($bfound) {
			if ($doprod) {
				if ($line =~ s/\<PreprocessorDefinitions\>/\<PreprocessorDefinitions\>_ALLEGIANCE_PROD_;/i) {
					print "$name Updated to define _ALLEGIANCE_PROD_\n";
				}
			}
			if ($do9) {
				if ($name =~ /Lobby|Server/i) {
					if ($line =~ s/\<\/Link\>/\<AdditionalOptions\>\/SUBSYSTEM\:\"console\,5.00\"  \/OSVERSION\:5\.00 \%\(AdditionalOptions\)\<\/AdditionalOptions\>\<\/Link\>/i) { # 
						print "$name Updated to link /w WinVer 5.0 console\n";
					}
				}	
				if ($name =~ /AllSrvUI|AGC|Allegiance|Reloader|AutoUpdate/i) {
					if ($line =~ s/\<\/Link\>/\<AdditionalOptions\>\/SUBSYSTEM\:\"windows\,5.00\"  \/OSVERSION\:5\.00 \%\(AdditionalOptions\)\<\/AdditionalOptions\>\<\/Link\>/i) { # 
						print "$name Updated to link /w WinVer 5.0 windows\n";
					}
				}	
			}			
		}
		if ($do9) {
			if ($line =~ /\<PropertyGroup Condition="'\$\(Configuration\)\|\$\(Platform\)'=='FZRetail\|Win32'" Label="Configuration"\>/) {
				$bfound2 = 1;
			}
			
			if ($bfound2) {
				
				if ($line =~ s/\<\/PropertyGroup\>/\<PlatformToolset\>v90\<\/PlatformToolset\>\<\/PropertyGroup\>/i) {
					print "$name Updated to define vc9\n";
					$bfound2 = 0;
				}
			}
		}
			
		if ($line =~ /\<\/ItemDefinitionGroup\>/i) {
			$bfound = 0;
		}
		
		print VCX $line;
	}
}
	close VCX;
exit 0;