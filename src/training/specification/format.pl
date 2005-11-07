#! perl

open (FILE, "TRAININGFINAL.csv") or die "couldn't open the file\n";

$line_length = 60;

while (<FILE>)
{
	my ($number, $description) = split (/,/, $_, 2);
	$number =~ s/\s//g;
	if ($number ne "")
	{
		print "\t\t// $number\n";
		$description =~ s/^"(.*)"$/$1/;
		$description =~ s/"/'/g;
		$description =~ s/\n$//g;
		$description = "\"$description\"";
		my $printable_description = "";
		while (length ($description) > $line_length)
		{
			my $tmp_length = $line_length;
			my $tmp_string = substr ($description, 0, $tmp_length);
			while ($tmp_string =~ /\S$/)
			{
				$tmp_length--;
				$tmp_string = substr ($description, 0, $tmp_length);
			}
			$printable_description = "$printable_description\t\t// $tmp_string\n";
			$description = substr ($description, $tmp_length);
		}
		$printable_description = "$printable_description\t\t// $description\n\n";
		print "$printable_description";
	}
}

close (FILE);