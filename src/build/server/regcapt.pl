

require "registry.pl";

{
    $true  = 1;
    $false = '';
        
    &_vEnumerateAndPrintRegKeys($HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\MSTravelConfig", $true);
    &_vEnumerateAndPrintRegKeys($HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel", $true);
    &_vEnumerateAndPrintRegKeys($HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI", $true);

}

sub _sGetHiveFriendlyName
{
    local ($sHive) = @_;
    if ($sHive eq $HKEY_LOCAL_MACHINE)
    {
        return "HKLM";
    }
    elsif ($sHive eq $HKEY_CLASSES_ROOT)
    {
        return "HKCR";
    }
    elsif ($sHive eq $HKEY_CURRENT_USER)
    {
        return "HKCU";
    }
    elsif ($sHive eq $HKEY_USERS)
    {
        return "HKU";
    }
    return $sHive;
}

sub _vEnumerateAndPrintRegKeys
{
    local ($sHive, $sKey, $nRecurse) = @_;
    local $hKey        = '';    
    local $iSubKey     = '';
    local $sSubKeyName = '';
    local $sClass      = '';
    local $tmLastWrite = '';
    local $sHiveFriendly = &_sGetHiveFriendlyName($sHive);

    &NTRegOpenKeyEx($sHive, $sKey, 0, $KEY_READ | $KEY_SET_VALUE, $hKey) || print "fail\n";

    $hKey || print "fail\n";

    while($iSubKey >= 0) 
    {
	    if(!&NTRegEnumKeyEx($hKey, $iSubKey, $sSubKeyName, 0, $sClass, $tmLastWrite)) 
        {
		    # print "done.\n";
		    $iSubKey = -1;
	    }
	    else 
        {
            # print "--" x $nRecurse;
		    # print "$sSubKeyName($sClass)\n";

            if ($nRecurse)
            {
                _vEnumerateAndPrintRegKeys($sHive, "$sKey\\$sSubKeyName", $nRecurse+1);
            }

		    $iSubKey++;
	    }
    }

    $iSubKey = 0;
    while($iSubKey >= 0) 
    {
	    if(!&NTRegEnumValue($hKey, $iSubKey, $sSubKeyName, 0, $sClass, $vData)) 
        {
		    # print "done.\n";
		    $iSubKey = -1;
	    }
	    else 
        {
            # print ".." x $nRecurse;
            if ($sClass eq 1)
            {
    		    print "$sHiveFriendly\\$sKey\\$sSubKeyName($sClass) = '$vData'\n";
            }
            elsif ($sClass eq 4)
            {
                my $sTest = unpack( "I", $vData );
    		    print "$sHiveFriendly\\$sKey\\$sSubKeyName($sClass) = '$sTest'\n";
            }
            else
            {
                print "*** unsupported class found for $sSubKeyName($sClass)!!!\n";
            }
		    $iSubKey++;
	    }
    }

    &NTRegCloseKey($hkey);
}

