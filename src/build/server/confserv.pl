#========================================================================
#
# Microsoft Travel Technologies - Travel Server Configuration Script
#
# Copyright <cp> 1997 Microsoft Corporation, All Rights Reserved
#
# Modification History:
#   ---Date---  -Blame-- -Description of change------------------------
#   1997.09.01  MarkSn   Created.
#
#========================================================================
{
    require "nt.ph";
    require "registry.pl";

    if (! &_vCSInitialize())
    {
        die;
    }

    &_vOpenLog($ENV{'temp'}, "$ENV{'COMPUTERNAME'}Conf");

    if (&_fGetDirections())
    {
        if ($g_fLogOpen)
        {
            &_vLogSilent("$g_sLogFilename");
            &_vLogSilent('-' x 50);
            &_vLogSilent("Microsoft Travel Technologies");
            &_vLogSilent("Travel Server Configuration Script - log file");
            &_vLogSilent('-' x 50);
        }

        my $sMessage = "\r\nStarting Travel Server configuration process.\r\nClean = '$g_fClean'\r\nQuiet = '$g_fQuiet'\r\nInput = '$g_sInputFilename'\r\nOutput = '$g_sOutputFilename'\r\nReport = '$g_sReportFilename'";
        &NTWriteEventLog( $ENV{"COMPUTERNAME"}, $0, EVENTLOG_INFORMATION_TYPE, 0, 0, &NULL, 0, $sMessage);

        &_vFollowDirections();

        # &NTWriteEventLog( $ENV{"COMPUTERNAME"}, $0, EVENTLOG_INFORMATION_TYPE, 0, 0, &NULL, 0, "\r\nFinished configuring Travel Server.\r\n");

        &_vCloseLog();
        print "Complete.";
    }
}


# ===========================================================================
# This function determines what the user wants us to do.  This can come from
# command-line parameters, which could also specify an import file from which
# to get more directions, or we could walk the user through a set of dialogs
# to query for the instructions.  Returns $true if successful in getting the
# options necessary to continue, $false if not.
#
sub _fGetDirections
{
    if (($#ARGV+1) eq 0)
    {
        # no command line parameters means that the user wants us to ask what to do.
        if (&_cDialog("Main") eq 'x')
        {
            return $false;
        }
        
        $g_fClean          = (&_sGetOptionValueByName('Main', 'clean') eq 'X');
        $g_fNoChanges      = (&_sGetOptionValueByName('Main', 'report') eq 'X');
        $g_sReportFilename = &_sGetSettingValueByName('Main', 'repfil');
        $g_sInputFilename  = &_sGetSettingValueByName('Main', 'inpfil');
        $g_sOutputFilename = &_sGetSettingValueByName('Main', 'outfil');
    }
    else
    {
        # Parse the command line parameters to determine what the user
        # wants us to do.
        my $i = 0;
        for ($i=0; $i <= $#ARGV; $i++)
        {
            if ($ARGV[$i] =~ /-q()*/)
            {
                if ($g_fQuiet)
                {
                    print "Syntax:  '-q' specified twice.\n";
                    &_vPrintHelp();
                    return $false;
                }
                $g_fQuiet = $true;
            }
            elsif ($ARGV[$i] =~ /-c()*/)
            {
                $g_fClean = $true;
            }
            elsif ($ARGV[$i] =~ /-n()*/)
            {
                $g_fNoChanges = $true;
            }
            elsif ($ARGV[$i] =~ /-r()*/)
            {
                # parse for filename as next token
                $i++;
                if ($i gt $#ARGV)
                {
                    print "Syntax:  ran out of tokens for '-r' filename.\n";
                    &_vPrintHelp();
                    return $false;
                }
                if ($ARGV[$i] =~ /-()*/)
                {
                    print "Syntax:  missing or invalid filename for '-r': '$ARGV[$i]'.\n";
                    &_vPrintHelp();
                    return $false;
                }
                $g_sReportFilename = $ARGV[$i];
            }
            elsif ($ARGV[$i] =~ /-i()*/)
            {
                # parse for filename as next token
                $i++;
                if ($i gt $#ARGV)
                {
                    print "Syntax:  ran out of tokens for '-i' filename.\n";
                    &_vPrintHelp();
                    return $false;
                }
                if ($ARGV[$i] =~ /-()*/)
                {
                    print "Syntax:  missing or invalid filename for '-i': '$ARGV[$i]'.\n";
                    &_vPrintHelp();
                    return $false;
                }
                $g_sInputFilename = $ARGV[$i];
            }
            elsif ($ARGV[$i] =~ /-o()*/)
            {
                # parse for filename as next token
                $i++;
                if ($i gt $#ARGV)
                {
                    print "Syntax:  ran out of tokens for '-o' filename.\n";
                    &_vPrintHelp();
                    return $false;
                }
                if ($ARGV[$i] =~ /-()*/)
                {
                    print "Syntax:  missing or invalid filename for '-o': '$ARGV[$i]'.\n";
                    &_vPrintHelp();
                    return $false;
                }
                $g_sOutputFilename = $ARGV[$i];
            }
            elsif ($ARGV[$i] =~ /-h()*/ || $ARGV[$i] =~ /-\?()*/)
            {
                &_vPrintHelp();
                return $false;
            }
            else 
            {
                print "Syntax:  Unrecognized token.\n";
                &_vPrintHelp();
                return $false;
            }
        }
    }

    if (!$g_fClean)
    {   # input filename required if not clean
        if ($g_fQuiet)   # can't ask for input, so must error out
        {
            if ($g_sInputFilename eq "")
            {
                print "Input filename required.\n";
                return $false;
            }
            elsif(!-e $g_sInputFilename)
            {
                print "Missing or invalid input filename: '$g_sInputFilename'.\n";
                return $false;
            }
        }
        elsif ($g_sInputFilename eq "")
        {
            return (&_cQuestionnaire() ne 'x');
        }
        else
        {
            # open the input file and read the override settings.
            if (open INPFILE, "<$g_sInputFilename")
            {
                my $iLine = 0;
                # print "File open successful.\n";
                # the first line _may_ be "REGEDIT".  If so, ignore it.
                my $sFileFormat = <INPFILE>;
                chop $sFileFormat;
                if ($sFileFormat eq "REGEDIT4")
                {
                    $iLine++;
                    my $sHive = "";
                    my $sKey = "";
                    my $sLocation = "";
                    my $sValueName = "";
                    my $sValue = "";
                    my $sClass = "";
                    while(<INPFILE>)
                    {
                        $iLine++;
                        chop;             # remove the carriage return from the end of the line
                        next if (!/\S/);  # if line doesn't contain a non-whitespace character, skip it

                        # print "$_\n";

                        if (/\[(.+?)\\(.+)]/)
                        {
                            # print "Line $iLine: Found key descriptor: '$1' '$2'\n";
                            my $sx = $1;
                            my $sy = $2;

                            if ($sx eq "HKLM" || $sx eq "HKEY_LOCAL_MACHINE")
                            {
                                $sHive = &HKEY_LOCAL_MACHINE;
                                $sKey = $sy;
                            }
                            elsif ($sx eq "HKEY_CLASSES_ROOT")
                            {
                                $sHive = &HKEY_CLASSES_ROOT;
                                $sKey = $sy;
                            }
                            elsif ($sx eq "HKEY_CURRENT_USER")
                            {
                                $sHive = &HKEY_CURRENT_USER;
                                $sKey = $sy;
                            }
                            elsif ($sx eq "HKEY_USERS")
                            {
                                $sHive = &HKEY_USERS;
                                $sKey = $sy;
                            }
                            else
                            {
                                &_vLog("Line $iLine: Unrecognized hive descriptor '$sx'");
                                return $false;
                            }
                            $sLocation = &_sGetLocationFromHiveKey($sHive, $sKey);
                            # print "Location: '$sLocation'\n";
                        }
                        else
                        {
                            if (/"(.+?)"=(.+)/)
                            {
                                $sValueName = $1;
                                $_ = $2;
                                if (/"(.+)"/)  
                                {
                                    $sClass = &REG_SZ;
                                    $sValue = $1;
                                }
                                elsif ($_ eq "\"\"")
                                {
                                    $sClass = &REG_SZ;
                                    $sValue = "";
                                }
                                elsif (/dword:(.+)/)
                                {
                                    $sClass = &REG_DWORD;
                                    $sValue = hex($1);
                                }
                                else
                                {
                                    &_vLog("Line $iLine: Unsupported class for value '$_'");    
                                    return $false;
                                }

                                if ($sLocation eq "")
                                {
                                    &_vLog("Line $iLine: Value descriptor without hive");
                                    return $false;
                                }

                                # print "Line $iLine: Found value descriptor: '$sValueName'='$sValue'\n";
                                
                                my ($sTable,$iEntry) = _snGetTableIndexFromLocationValueName($sLocation, $sValueName);
                                if ($sTable eq "" && $iEntry eq 0)
                                {
                                    &_vLog( "Line $iLine: Value '$sValueName' not supported." );
                                    return $false;
                                }

                                if ($$sTable[$iEntry+2] eq $sClass)
                                {
                                    $$sTable[$iEntry+3] = $sValue;
                                }
                                else
                                {
                                    &_vLog( "Line $iLine: Input file class mismatch." );
                                    return $false;
                                }
                                                                
                            }
                            else
                            {
                                # any line that doesn't start with '[' or '"' is a comment
                                # &_vLog("Line $iLine: Unrecognized format.");
                                # return $false;
                            }
                        }
                    }
                }
                elsif ($sFileFormat eq "REGEDIT")
                {
                    &_vLog("REGEDIT file format not currently supported.");
                    return $false;
                }
                else
                {
                    &_vLog("Unrecognized file format not supported.");
                    return $false;
                }
            }
            else
            {
                &_vLog( "Unable to open input file, '$g_sInputFilename'" );
                return $false;
            }
        }
    }

    if ($g_sOutputFilename eq "")
    {
        $g_sOutputFilename = &_sGetUniqueFilename("$ENV{'TEMP'}\\ConfServOutput", "txt");
    }
    if ($g_sOutputFilename ne "")
    {
        # open output file and add the headers
        if (open OUTFILE, ">$g_sOutputFilename")
        {
            print OUTFILE "REGEDIT4\n";
            print OUTFILE "* This file created by ConfServ.pl\n";
            print OUTFILE "* Use this file to revert to the settings prior to running ConfServ.pl\n";
            print OUTFILE "\n";
        }
        else
        {
            &_vLog( "Unable to open output file, '$g_sOutputFilename'" );
            return $false;
        }
    }
    
    return $true;
}

# ===========================================================================
# This function asks the user for the mandatory registry settings.  Then
# they are asked if they want to edit any of the optional settings.  After
# that, they are asked if they want to add any of the optional-existing keys.
#
sub _cQuestionnaire
{
    my $iQ = 0;
    my $iMax = $#srRegMandatory;
    for ($iQ = 0; $iQ <= $iMax; $iQ+=5)
    { 
        my $sKey           = $srRegMandatory[$iQ];
        my $sHive          = &_sGetTableColValByColVal("srRegLocations", 3, 1, 0, $sKey);
        my $sHiveFriendly  = &_sGetHiveFriendlyName($sHive);
        my $sClass         = $srRegMandatory[$iQ+2];
        my $sClassFriendly = &_sGetClassFriendlyName($sClass);
        my $sTree          = &_sGetTableColValByColVal("srRegLocations", 3, 2, 0, $sKey);
        my ($fSucc,$sValue)= &_fsGetRegValue($sHive, $sTree, $srRegMandatory[$iQ+1]);

        &_vPrintHeading();
        print "- Required Settings ----------------------------------------------------\n";
        print "         Hive:   $sHiveFriendly\n";
        print "         Tree:   $sTree\n";
        print "     Key Name:   $srRegMandatory[$iQ+1]\n";
        print "        Class:   $sClassFriendly\n\n";

        my $sLabel = "  Description:   ";
        my $sDesc = $srRegMandatory[$iQ+4];
        my $iD = 0;
        for ($iD = 0; $iD < 5; $iD++)
        {
            $_ = $sDesc;
            ($sPortion, $sDesc) = /(.{1,55})(.*)/;
            print "$sLabel$sPortion\n";
            $sLabel = "                 ";
        }
        print "\n";

        if ($fSucc)
        {
            print "Current Value:   '$sValue'\n";
        }
        else
        {
            print "Current Value:   (key does not exist)\n";
        }
        if ($srRegMandatory[$iQ+3] ne $sValue)
        {
            print "Change Value To: '$srRegMandatory[$iQ+3]'\n\n";
        }
        else
        {
            print "\n\n";
        }
        print "- What do you want to do? ----------------------------------------------\n";
        print "(e) Edit Setting                  (b) Back       (n) Next       (x) Exit\n\n";
        $cChoice = &_cAskSet($true, "Please choose one:", "ebnx", "x");
        if ($cChoice eq 'x')
        {
            return $cChoice;
        }
        elsif ($cChoice eq 'e')
        {
            $srRegMandatory[$iQ+3] = &_sQueryAndConfirm($srRegMandatory[$iQ+3], "Value for '$srRegMandatory[$iQ+1]'");
            $iQ -= 5;
        }
        elsif ($cChoice eq 'b')
        {
            if ($iQ gt 0)
            {
                $iQ -= 10;
            }
            else
            {
                $iQ -= 5;
            }
        }
    }

    # TODO:  ask them about the big set of options.


    # TODO:  ask them about the last set of keys that don't even have to exist.


}

# ===========================================================================
# This function retrieves the specified value from the registry.
#
sub _fsGetRegValue
{
    local ($sHive, $sRegKey, $sRegValueName) = @_;

    if (&NTRegOpenKeyEx ($sHive, $sRegKey, 0, &KEY_ALL_ACCESS, $key))
    # if (&NTRegOpenKey ($sHive, $sRegKey, $key))
    {
        my $sRegValue = "";
        if (&NTRegQueryValueEx ($key, $sRegValueName, 0, $sClass, $sRegValue))
        {
            if ($sClass eq 1)
            {
    		    return ($true, $sRegValue);
            }
            elsif ($sClass eq 4)
            {
                my $sTest = unpack( "I", $sRegValue );
    		    return ($true, $sTest);
            }
            else
            {
                print "*** unsupported class found for $sRegKey\\$sRegValueName($sClass)!!!\n";
                return ($false, "");
            }
        }
        else
        {
            # print "NTRegQueryValueEx failed.\n";
        }
    }
    else
    {
        # print "NTRegOpenKeyEx failed.\n";
    }

    return ($false, "");
}

# ===========================================================================
# This function is passed a registry hive identifier and a key-value name,
# the value name.  The value will be deleted.
#
sub _fRemoveRegValue 
{
    local ($sHive, $sKey, $sValue) = @_;

    if (&NTRegOpenKeyEx ($sHive, $sKey, 0, &KEY_ALL_ACCESS, $key))
    {
        if (&NTRegDeleteValue($key, $sValue))
        {
            return $true;
        }
    }
    else
    {
        &_vLog( "Unable to open reg key to delete." );
    }

    return $false;
}

# ===========================================================================
# This function is passed a registry hive identifier and a key-value name,
# the value name, plus a new value.  The reg key is set to the new value.
#
sub _fChangeRegValue 
{
    local ($sHive, $sKey, $sClass, $sValueName, $sNewValue) = @_;

    if (&NTRegCreateKeyEx ($sHive, $sKey, &NULL, "mtt", &REG_OPTION_NON_VOLATILE, &KEY_ALL_ACCESS, &NULL, $hkey, $disposition))
    {
        if ($sClass eq &REG_SZ)
        {
            if (&NTRegSetValueEx( $hkey, $sValueName, &NULL, $sClass, $sNewValue))
            {
                return $true;
            }
        }
        elsif ($sClass eq &REG_DWORD)
        {
            #my $sTest = pack( "I", $sNewValue );
            if (&NTRegSetValueEx( $hkey, $sValueName, &NULL, $sClass, $sNewValue))
            {
                return $true;
            }
        }
        else
        {
            print "*** unsupported class found for $sKey\\$sValueName($sClass)!!!\n";
            return $false;
        }
    }
    else
    {
        print "Unable to create key.\n";
    }


    return $false;
}

# ===========================================================================
# This function acts on what the user specified they want us to do.
#
sub _vFollowDirections
{
    my $sLastKey = "";
    
    my $iQ = 0;
    my $iMax = $#srRegMandatory;
    for ($iQ = 0; $iQ <= $iMax; $iQ+=5)
    {
        if ($g_sOutputFilename ne "" && $sLastKey ne $srRegMandatory[$iQ])
        {
            $sLastKey = $srRegMandatory[$iQ];
            print OUTFILE "[]\n";
        }

        my $sAction = &_sUpdateRegistryValues( "srRegMandatory", $iQ );
        &_vLogSilent( $sAction );
    }

    $iMax = $#srRegAdvanced;
    for ($iQ = 0; $iQ <= $iMax; $iQ+=5)
    {
        my $sAction = &_sUpdateRegistryValues( "srRegAdvanced", $iQ );
        &_vLogSilent( $sAction );
    }

    $iMax = $#srRegOptional;
    for ($iQ = 0; $iQ <= $iMax; $iQ+=5)
    {
        my $sAction = &_sUpdateRegistryValues( "srRegOptional", $iQ );
        &_vLogSilent( $sAction );
    }
}

# ===========================================================================
# This function acts on what the user specified they want us to do.
#
sub _sUpdateRegistryValues
{
    local ($sTable, $iQ) = @_;

    my $sKey           = $$sTable[$iQ];
    my $sHive          = &_sGetTableColValByColVal("srRegLocations", 3, 1, 0, $sKey);
    my $sHiveFriendly  = &_sGetHiveFriendlyName($sHive);
    my $sClass         = $$sTable[$iQ+2];
    my $sClassFriendly = &_sGetClassFriendlyName($sClass);
    my $sTree          = &_sGetTableColValByColVal("srRegLocations", 3, 2, 0, $sKey);
    my ($fSucc,$sValue)= &_fsGetRegValue($sHive, $sTree, $$sTable[$iQ+1]);
    my $sNewValue      = $$sTable[$iQ+3];
    local $sAction     = "[$$sTable[$iQ+1]] ";

    if ($fSucc)
    {
        $sAction = $sAction . "found [$sValue], ";
 #       $sOutput = 

        if ($g_fClean)
        {
            $sAction = $sAction . "removing.";
            if (!&_fRemoveRegValue ($sHive, $sTree, $$sTable[$iQ+1]))
            {
                $sAction = $sAction . " ***FAILED***";
            }
        }
        else
        {
            if ($sValue eq $sNewValue)
            {
                $sAction = $sAction . "no change.";
            }
            else
            {
                $sAction = $sAction . "changes to [$sNewValue].";
                if (!$g_fNoChanges)
                {
                    if (&_fChangeRegValue ($sHive, $sTree, $sClass, $$sTable[$iQ+1], $sNewValue))
                    {
                        my ($fNewSucc, $sChangedValue) = &_fsGetRegValue($sHive, $sTree, $$sTable[$iQ+1]);
                        if (!$fNewSucc || ($sChangedValue ne $sNewValue))
                        {
                            $sAction = $sAction . " ***FAILED***";
                        }
                    }
                    else
                    {
                        $sAction = $sAction . " ***FAILED***";
                    }
                }
            }
        }
    }
    else
    {
        $sAction = $sAction . "not found, ";
    
        if ($g_fClean)
        {
            $sAction = $sAction . "so can't remove.";
        }
        else
        {
            $sAction = $sAction . "add value [$sNewValue].";
            if (!$g_fNoChanges)
            {
                if (&_fChangeRegValue ($sHive, $sTree, $sClass, $$sTable[$iQ+1], $sNewValue))
                {
                    my ($fNewSucc, $sChangedValue) = &_fsGetRegValue($sHive, $sTree, $$sTable[$iQ+1]);
                    if (!$fNewSucc || ($sChangedValue ne $sNewValue))
                    {
                        $sAction = $sAction . " ***FAILED***";
                    }
                }
                else
                {
                    $sAction = $sAction . " ***FAILED***";
                }
            }
        }
    }        

    return $sAction;
}

# ===========================================================================
# This function acts on what the user specified they want us to do.
#
sub _vPrintHelp
{
    # &_vPrintHeading();
    print "Format:  ConfigGateway [options]\n";
    print "  where [options] can be one or more of the following:\n";
    print "    -i(nput) <filename>\n";
    print "       Uses <filename> for override values.\n";
    print "    -o(utput) <filename> | NOFILE\n";
    print "       <filename> specifies location of backup settings file.\n";
    print "       'NOFILE' causes no output file to be generated.\n";
    print "    -c(lean)\n";
    print "       Clears all settings.\n";
    print "    -n(o changes)\n";
    print "       Generate report, but don't change registry.\n";
    print "    -r(eport) <filename>\n";
    print "       Outputs values for all settings in readable format.\n";
    print "    -q(uiet)\n";
    print "       Causes no output to be displayed on the screen.\n";
    print "       Syntax or other errors will still be displayed.\n";
    print "    -h(elp), -?\n";
    print "       Causes this help screen to be displayed.\n";
    return;
}

# ===========================================================================
# This dialog function asks what components to install, and calls other
# subroutines based on whether the component selected has more options
# that can be modified.
#
sub _cDialog
{
    local($sDialogName) = @_;
    &_vLogSilent("$sDialogName Dialog");

    my $sOptions      = "srOption$sDialogName";
    my $sSettings     = "srSetting$sDialogName";
    my $sValidChoices = "";
    my $cChoice       = "";
    my $cChoiceName   = "";

    while ($true)
    {
        &_vPrintHeading();
        $sValidChoices = "";

        print "- Install Settings -----------------------------------------------------\n";
        my $nSettings = ($#$sSettings)+1;
        my $i = 0;
        for ($i = 0; $i < $nSettings; $i+=4)
        {
            $sValidChoices .= $$sSettings[$i];
            print "   $$sSettings[$i]. $$sSettings[$i+2]  [$$sSettings[$i+3]]\n";
        }

        print "\n";
        print "- Install Options ------------------------------------------------------\n";
        local $nOptions = ($#$sOptions)+1;
        for ($i = 0; $i < $nOptions; $i+=4)
        {
            $sValidChoices .= $$sOptions[$i];
            $cActive = &_cGetActiveFlagByName($sDialogName, $$sOptions[$i+1]);
            print " $cActive $$sOptions[$i].  $$sOptions[$i+2]\n";
        }

        print "\n";
        print "- What do you want to do? ----------------------------------------------\n";
        print "Edit setting/option by letter/number             (n) Next       (x) Exit\n\n";
        $sValidChoices .= "nx";
        $cChoice = &_cAskSet($true, "Please choose one:", $sValidChoices, "x");
        if (index("nx", $cChoice) ge 0)
        {
            return $cChoice;
        }
        elsif ($cChoice =~ /[a-m]/) # valid settings can be a thru m
        {
            my $cChoiceName  = &_sGetSettingNameByIndex($sDialogName, $cChoice);
            my $sChoiceValue = &_sGetSettingValueByName($sDialogName, $cChoiceName);
            my $sChoiceDesc  = &_sGetSettingDescByName($sDialogName, $cChoiceName);

            $sChoiceValue    = &_sQueryAndConfirm($sChoiceValue, $sChoiceDesc);
            &_vSetSettingValueByName($sDialogName, $cChoiceName, $sChoiceValue);
        }
        else
        {
            my $cChoiceName  = &_sGetOptionNameByIndex($sDialogName, $cChoice);
            my $sChoiceDesc  = &_sGetOptionDescByName($sDialogName, $cChoiceName);
            my $sChoiceValue = &_sGetOptionValueByName($sDialogName, $cChoiceName);
            if ($sChoiceValue eq ' ' || $sChoiceValue eq 'X')
            {
                $sChoiceValue = ' ';
                if (&_fAskYN( $true, "Install $sChoiceDesc" ))
                {
                    $sChoiceValue = 'X';
                }
                &_vSetOptionValueByName($sDialogName, $cChoiceName, $sChoiceValue);
            }
            else
            {
                &_cDialog($sChoiceValue);
            }
        }
    }
}

# ===========================================================================
# Ask the user to confirm or re-enter the specified answer to the specified
# question.
#
sub _sQueryAndConfirm
{
    local($value, $title) = @_;

    &_vLogSilent("Option is [$title], value is [$value].");

    while (! &_fAskYN ($true, "$title [$value].  Okay?"))
    {
        print "Enter the alternate value now: ";
        $value = <STDIN>;
        chop $value;
        &_vLogSilent("Option is [$title], user changed value to [$value].");
    }

    $value;
}

# ===========================================================================
# This function is called recursively to determine what the setting of the
# item is: install, don't install, or install some.
#
sub _cGetActiveFlagByName
{
    my ($sDialogName, $sName) = @_;
    my $sOptActive    = &_sGetOptionValueByName($sDialogName, $sName);

    if ($sOptActive eq ' ' || $sOptActive eq 'X')
    {
        return $sOptActive;
    }
    else
    {
        # see if any of the active settings are on now for the sub-dialog
        my $fSomeOn         = $false;
        my $fSomeOff        = $false;

        my $sTable = "srOption$sOptActive";
        my $nItems = ($#$sTable)+1;
        my $i      = 0;
        for ($i = 0; $i < $nItems; $i+=4)
        {
            my $sTableName = $$sTable[$i+1];
            my $cSubActive = &_cGetActiveFlagByName($sOptActive, $sTableName);
            if ($cSubActive eq ' ')   { $fSomeOff = $true; }
            else                      { $fSomeOn = $true;  }
        }
        if ($fSomeOn)
        {
            if ($fSomeOff)                  { return '*'; }
            else                            { return 'X'; }
        }
        else                                { return ' '; }
    }
}

# ===========================================================================
# This subroutine is called when something bad happens that potentially means
# the script needs to stop.  The user is, however, given the choice to abort,
# retry or ignore.  If the user selects abort, the subroutine never returns.
# If retry, false is returned.  If ignore, true is returned.
#
sub _vDieAndLogReason
{
    local ($reason) = @_;
    open (CURDIR, "cd |");
    $curdir = <CURDIR>;
    close CURDIR;
    chop $curdir;

    &_vLog ("*** An unexpected error occurred!");
    &_vLog ("*** Explanation [$reason]");
    &_vLog ("*** Current dir [$curdir]");

    $resolution = &_cAskSet( $true, "Abort, retry or ignore and continue?", "ari", "a");

    if ($resolution eq "a")
    {
        &_vLog ("Log files can be found in $ENV{'TEMP'}");
        die $reason;
    }
    else
    {
        return ($resolution eq "i");
    }
}

# ===========================================================================
# If not pausing, always returns true. Else asks the user a yes/no question
# and returns the result
#
sub _fAskYN
{
    local($fAsk, $question) = @_;

    return (&_cAskSet( $fAsk, $question, "yn", "y" ) eq 'y');
}

# ===========================================================================
# This subroutine is passed a question to ask the user, in $question.  If
# the $fAsk flag is true, the question will be asked.  If false, the question
# will not be asked, but the default answer will be returned.  The default
# answer is passed in $default, and the set of valid answers is passed in $set.
# Note:  it is assumed that $default is one character, and $set contains
# a concatenation of one character possible valid user choices.
#
sub _cAskSet
{
    local($fAsk, $question, $set, $default) = @_;

    $fproceed = $default;
    if ($fAsk)
    {
        $fproceed = "";
        while ($fproceed eq "")
        {
            print "$question ($set): ";
            $fproceed = &_cQueryForCharFromSet($set);
        }
    }
    &_vLogSilent ("$question = $fproceed");

    # print "Answer is '$fproceed'.\n";
    $fproceed;
}

# ===========================================================================
# Get first character of response & eat rest of line.
#
sub _cQueryForCharFromSet
{
    local($getkeystring) = @_;
    $getchoice = getc;
    if ($getchoice ne "\n")
    {
        while (1)
        {
            $geta = getc;
            if ($geta eq "\n")
            {
                last;
            }
        }
    }

    if (index ($getkeystring, $getchoice) < 0)
    {
        print "Please select one of ($getkeystring)\n";
        $getchoice = "";
    }
    $getchoice;
}

# ===========================================================================
# Open the log file used to output messages as the script goes along.  If it
# works, the field $g_fLogOpen will be set to $true, and $g_sLogFilename will
# contain the filename of the log file.
#
sub _vOpenLog
{
    local($sDir, $sBase) = @_;

    $g_sLogFilename = &_sGetUniqueFilename("$sDir\\$sBase", "txt");
    my $fRC = $false;
    do { $fRC = open (PROPLOG, ">$g_sLogFilename") 
    } until ( $fRC || &_vDieAndLogReason ("create logfile '$g_sLogFilename'"));
    $g_fLogOpen = $fRC;

}

# ===========================================================================
# Create a unique filename based on the specified filename root and extension.
# This is done simply by adding a "-i" where 'i' is a number starting with '0'
# and incrementing by 1, until the filename does not exist.
#
sub _sGetUniqueFilename
{
    local($sRoot, $sExt) = @_;
    my $i = 0;
    while (-e "$sRoot-$i.$sExt")
    {
        $i = $i + 1;
    }
    return "$sRoot-$i.$sExt";
}

# ===========================================================================
# Place action information in log file, and echo to screen.  An action does
# not start with a comment character or the date/time.  Just the action code
# and the data.
#
sub _vLogAction
{
    local ($sMessage) = @_;

    if ($g_fLogOpen || $fLogAnyway)
    {
        ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime;
        $sLogDate = sprintf "%02d/%02d/%02d %02d:%02d:%02d", $mon+1, $mday, $year, $hour, $min, $sec;

        my $sContinued = '';
        while ($sMessage ne '')
        {
            $sLogString = "#$sLogDate $sContinued$sMessage";
            $sContinued = '+ ';

            $_ = $sLogString;
            ($sPortion, $sMessage) = /(.{1,79})(.*)/;

            if ($g_fLogOpen) # (defined(PROPLOG))
            {
                print PROPLOG "$sPortion\n";
            }
            print "$sPortion\n";
        }
    }
}

# ===========================================================================
# Place information in log file, and echo to screen.
#
sub _vLog
{
    local ($sMessage) = @_;

    if ($g_fLogOpen || $fLogAnyway)
    {
        ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime;
        $sLogDate = sprintf "%02d/%02d/%02d %02d:%02d:%02d", $mon+1, $mday, $year, $hour, $min, $sec;

        my $sContinued = '';
        while ($sMessage ne '')
        {
            $sLogString = "# $sLogDate $sContinued$sMessage";
            $sContinued = '+ ';

            $_ = $sLogString;
            ($sPortion, $sMessage) = /(.{1,79})(.*)/;

            if (defined(PROPLOG))
            {
                print PROPLOG "$sPortion\n";
            }
            print "$sPortion\n";
        }
    }
}

# ===========================================================================
# Place information in log file only.
#
sub _vLogSilent
{
    if ($fLogAnyway)
    {
        &_vLog(@_);
    }
    elsif ($g_fLogOpen)
    {
        ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime;
        $logdate = sprintf "%02d/%02d/%02d %02d:%02d:%02d", $mon+1, $mday, $year, $hour, $min, $sec;
        if (defined(PROPLOG))
        {
           printf PROPLOG "# $logdate @_\n";
        }
    }
}

# ===========================================================================
# Close the log file used to output messages as the script goes along.
#
sub _vCloseLog
{
    if ($g_fLogOpen)
    {
        close PROPLOG;
        $g_fLogOpen = $false;
    }
}

# ===========================================================================
# This subroutine simply displays that heading lines at the top of the screen.
sub _vPrintHeading
{
    system ("cls");
    print "$0\n";
    print '-' x 72;
    print "\nMicrosoft Travel Technologies - Travel Server Configuration Script\n";
    print "Copyright (c) 1997 Microsoft Corporation, All Rights Reserved.\n\n";
}

#========================================================================
# This function retrieves a row of data from the settings table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _ssssGetSettingByName
{
    local ($sDlg, $sName) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            return ($$sTable[$i], $$sTable[$i+1], $$sTable[$i+2], $$sTable[$i+3]);
        }
    }
    &_vLog("GetSettingByName: dialog='$sDlg', name='$sName' not found.");
    return ('', '', '', '');
}

#========================================================================
# This function retrieves a row of data from the options table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _ssssGetOptionByName
{
    local ($sDlg, $sName) = @_;

    local $sTable = "srOption$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            return ($$sTable[$i], $$sTable[$i+1], $$sTable[$i+2], $$sTable[$i+3]);
        }
    }
    &_vLog("GetOptionByName: dialog='$sDlg', name='$sName' not found.");
    return ('', '', '', '');
}

#========================================================================
# This function retrieves a row of data from the settings table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _sGetSettingNameByIndex
{
    local ($sDlg, $sIndex) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableIndex = $$sTable[$i];
        if ($sTableIndex eq $sIndex)
        {
            return $$sTable[$i+1];
        }
    }
    &_vLog("GetSettingNameByIndex: dialog='$sDlg', index='$sIndex' not found.");
    return '';
}

#========================================================================
# This function retrieves a row of data from the settings table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _sGetSettingValueByName
{
    local ($sDlg, $sName) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            return $$sTable[$i+3];
        }
    }
    &_vLog("GetSettingValueByName: dialog='$sDlg', name='$sName' not found.");
    return '';
}

#========================================================================
# This function retrieves a row of data from the settings table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _sGetSettingDescByName
{
    local ($sDlg, $sName) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            return $$sTable[$i+2];
        }
    }
    &_vLog("GetSettingDescByName: dialog='$sDlg', name='$sName' not found.");
    return '';
}

#========================================================================
# This function sets the value for a row of data from the settings table
# specified.  The second parameter identifies the value in the 2nd column
# of the row to modify and the third parameter is the new value.
#
sub _vSetSettingValueByName
{
    local ($sDlg, $sName, $sNewValue) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            $$sTable[$i+3] = $sNewValue;
            return;
        }
    }
    &_vLog("SetSettingValueByName: dialog='$sDlg', name='$sName' not found.");
}

#========================================================================
# This function retrieves a row of data from the options table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _sGetOptionNameByIndex
{
    local ($sDlg, $sIndex) = @_;

    local $sTable = "srOption$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableIndex = $$sTable[$i];
        if ($sTableIndex eq $sIndex)
        {
            return $$sTable[$i+1];
        }
    }
    &_vLog("GetOptionNameByIndex: dialog='$sDlg', index='$sIndex' not found.");
    return '';
}

#========================================================================
# This function retrieves a row of data from the options table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _sGetOptionValueByName
{
    local ($sDlg, $sName) = @_;

    local $sTable = "srOption$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            return $$sTable[$i+3];
        }
    }
    &_vLog("GetOptionValueByName: dialog='$sDlg', name='$sName' not found.");
    return '';
}

#========================================================================
# This function retrieves a row of data from the options table specified.
# The second parameter identifies the value in the 2nd column of the
# row to return.
#
sub _sGetOptionDescByName
{
    local ($sDlg, $sName) = @_;

    local $sTable = "srOption$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            return $$sTable[$i+2];
        }
    }
    &_vLog("GetOptionDescByName: dialog='$sDlg', name='$sName' not found.");
    return '';
}

#========================================================================
# This function sets the value for a row of data from the options table
# specified.  The second parameter identifies the value in the 2nd column
# of the row to modify and the third parameter is the new value.
#
sub _vSetOptionValueByName
{
    local ($sDlg, $sName, $sNewValue) = @_;

    local $sTable = "srOption$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        if ($sTableName eq $sName)
        {
            $$sTable[$i+3] = $sNewValue;
            return;
        }
    }
    &_vLog("SetOptionValueByName: dialog='$sDlg', name='$sName' not found.");
}

#========================================================================
# This function returns the row number and column value based on the 
# column, column value and number of columns in the table.
#
sub _sGetTableColValByColVal
{
    local ($sTable, $nNumCols, $nRetCol, $nCol, $sColVal) = @_;

    my $i = 0;
    my $iTable = $#$sTable;
    for ($i=0; $i <= $iTable; $i += $nNumCols)
    {
        my $sTableColVal = $$sTable[$i+$nCol];
        # print "_sGetTableColValByColVal: i=$i, $nNumCols, comparing '$sColVal' to '$sTableColVal'.\n";
        if ($sTableColVal eq $sColVal)
        {
            my $sRetColVal = $$sTable[$i+$nRetCol];
            return $sRetColVal;
        }
    }
    return "";
}

sub _sGetHiveFriendlyName
{
    local ($sHive) = @_;
    if ($sHive eq &HKEY_LOCAL_MACHINE)
    {
        return "HKLM";
    }
    elsif ($sHive eq &HKEY_CLASSES_ROOT)
    {
        return "HKCR";
    }
    elsif ($sHive eq &HKEY_CURRENT_USER)
    {
        return "HKCU";
    }
    elsif ($sHive eq &HKEY_USERS)
    {
        return "HKU";
    }
    return $sHive;
}

sub _sGetClassFriendlyName
{
    local ($sClass) = @_;
    if ($sClass eq '1')
    {
        return "REG_SZ";
    }
    elsif ($sClass eq '4')
    {
        return "REG_DWORD";
    }
    return "I don't know what the heck it is";
}

sub _sGetLocationFromHiveKey
{
    local ($sHive,$sKey) = @_;

    my $i = 0;
    my $iTable = $#srRegLocations;
    for ($i=0; $i <= $iTable; $i += 3)
    {
        # print "Comparing '\L$srRegLocations[$i+2]\E' to '\L$sKey\E'.\n";
        if ($srRegLocations[$i+1] eq $sHive && "\L$srRegLocations[$i+2]\E" eq "\L$sKey\E")
        {
            return $srRegLocations[$i];
        }
    }
    return "";
}

sub _snGetTableIndexFromLocationValueName
{
    local ($sLocation,$sValueName) = @_;

    # print "Looking for '$sLocation', '$sValueName'.\n";

    @srTables =
    ( 
        'srRegMandatory',
        'srRegAdvanced',
        'srRegOptional',
        'srRegObsolete'
    );

    my $j = 0;
    my $jTable = $#srTables;
    for ($j=0; $j <= $jTable; $j++)
    {
        my $i = 0;
        my $sTableName = $srTables[$j];
        my $iTable = $#$sTableName;
        for ($i=0; $i <= $iTable; $i += 5)
        {
            # print "Comparing against ($i) '$$sTableName[$i]', '$$sTableName[$i+1]'.\n";
            if ($$sTableName[$i] eq $sLocation && "\L$$sTableName[$i+1]\E" eq "\L$sValueName\E")
            {
                # print "Match!\n";
                return ($sTableName, $i);
            }
        }
    }
    return "", 0;
}

#========================================================================
# This section interacts with the installer to determine what components
# should be installed.
#
sub _vCSInitialize
{
    # print "Initializing...\n";

    $true  = (1 == 1);
    $false = (!$true);
    $fLogAnyway  = $false; # $true;
    $g_fLogOpen  = $false;
    $g_fQuiet          = $false;
    $g_fClean          = $false;
    $g_fNoChanges      = $false;
    $g_sInputFilename  = "";
    $g_sOutputFilename = "";
    $g_sReportFilename = "";


    #----------------------------------------------------------------------------------
    # Dialog - Main Dialog
    @srSettingMain =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        'a',  'inpfil',   'Input filename   ',                    "",
        'b',  'outfil',   'Output filename  ',                    "",
        'c',  'repfil',   'Report filename  ',                    "",
    );
    @srOptionMain =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        '1',  'clean',    'Clear all settings',                   ' ',
        '2',  'report',   'Report only (no changes)',             ' ',
    );


    #----------------------------------------------------------------------------------
    # This section is for specifying the registry options that will be setup for each
    # of the various components.
    #----------------------------------------------------------------------------------

    # &REG_SZ   = '1';
    # &REG_DWORD   = '4';
    
    my $fRC = $false;
    my $sSQLServerDriverLoc = "xxx";
    ($fRC, $sSQLServerDriverLoc) = &_fsGetRegValue( &HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBCINST.INI\\SQL Server", "Driver");
    if (!$fRC || $sSQLServerDriverLoc eq "")
    {
        print "Unable to locate SQL Server Driver!!!  Can't continue.\n";
        return $false;
    }

    @srRegLocations =
    ( # --Name----- --Hive-------------  --Location------------------------------------
        'shr',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\shared",
        'x25',      &HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Travel\\shared\\x25comm",
        'svr',      &HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Travel\\Server",
        'dsn',      &HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Travel\\Server\\Database\\DataSourceName",
        'mst',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\MSTravelObject",
        'usg',      &HKEY_LOCAL_MACHINE, "Software\\Microsoft\\travel\\Server\\MSTravelObject\\UsageLog",
        'ccd',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\CreditCards",
        'ssp',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Sabresp",
        'ss1',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Sabresp\\1",
        'ss2',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Sabresp\\9001",
        'ssm',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\SSM",
        'lfs',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\LFS",
        'wsp',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Wssp",
        'wsh',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Wssp\\Hot",
        'wse',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Wssp\\Hot\\ExcludeCarriers",          
        'wsc',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Wssp\\Hot\\PlatingCarriers",          
        'wsv',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Wssp\\Hot\\ExcludeVerbiageUnknown",   
        'wsx',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\travel\\Server\\Wssp\\Hot\\ExcludeVerbiageOperatedBy",
        'ods',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources",
        'ohf',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\HotelFinder",
        'ocl',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\CRSLog",
        'ose',      &HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ServerETA",
    );

    # These values are always queried for during configuration, or a value is 
    # expected if running from an import file.  The value in the registry is 
    # always replaced by what is specified.
    @srRegMandatory =
    ( # --Loc--  --Name-------------                       --Class----   --Value---------------    --Description-------------------------
        'mst',   'LoadSPsOnStartup',                       &REG_SZ,      "1",                      "ServiceProvider's to load (1=WorldSpan,2=SABRE,3=Cabbage,4=Apollo,5=Galileo,6=SystemOne,7=Amadeus,8=AmexLFS,9=HotelInfo,10=CabbageCon)",
        'mst',   'LoadRoutersOnStartup',                   &REG_SZ,      "1,3,4,10001,10002",      "Products Ids supported (1=Expedia,2=Wilbur,3=ExpUK,4=ExpCA,5=ExpAU,9001+=CorpTest,10001=NWBrand,10002=COBrand,100010+=CorpReal).",
        'x25',   "NoX25Card",                              &REG_DWORD,   0,                        "Enter 1 if the machine is using an Eicon card, 0 if not.",
        'x25',   "SocketX25PrimaryServer",                 &REG_SZ,      "$ENV{'COMPUTERNAME'}",   "Enter the primary X25 server this gateway server will reference.  If this machine has an Eicon card, you will generally want to specify this machine's name.",
        'x25',   "SocketX25BackupServers",                 &REG_SZ,      "",                       "List the server(s) that this gateway server should reference as backup in case of communications problems with the primary X25 server.",
        'x25',   "X25Socket",                              &REG_DWORD,   1023,                     "Use '1023' for MOSWest and Saturn, '7123' for Rasta.",
        'x25',   "X25Countries",                           &REG_DWORD,   4,                        "Enter the number of countries that the X25 server's communication line supports.",
        'shr',   "MailServers",                            &REG_SZ,      "207.68.149.229",         "Use 'trav' for Rasta, '131.107.17.21' or 'travbeta.microsoft.com' for Saturn, or '207.68.149.229' for moswest.",
        'ohf',   "Server",                                 &REG_SZ,      "TRAVSQL",                "SQL server where the hotel database lives",
        'ocl',   "Server",                                 &REG_SZ,      "MTTDEV02",               "SQL server where the CRS log database lives",
        'ose',   "Server",                                 &REG_SZ,      "MTTDEV02",               "SQL server where the travel server database lives",
    );

    # get the initial value for each key from the registry
    {
        my $iQ = 0;
        my $iMax = $#srRegMandatory;
        for ($iQ = 0; $iQ <= $iMax; $iQ+=5)
        { 
            my $sKey           = $srRegMandatory[$iQ];
            my $sHive          = &_sGetTableColValByColVal("srRegLocations", 3, 1, 0, $sKey);
            my $sClass         = $srRegMandatory[$iQ+2];
            my $sTree          = &_sGetTableColValByColVal("srRegLocations", 3, 2, 0, $sKey);
            my ($fSucc,$sValue)= &_fsGetRegValue($sHive, $sTree, $srRegMandatory[$iQ+1]);
            if ($fSucc)
            {
                $srRegMandatory[$iQ+3] = $sValue;
            }
        }
    }

    # These values are never asked for, but may be overridden from a value
    # in an import file.  The value in the registry is always replaced by 
    # what is specified.
    @srRegAdvanced =
    ( # --Loc--  --Name-------------                       --Class----   -Val-                     --Description-------------------------
        'x25',   "SocketSweepInterval",                    &REG_DWORD,   5000,                     "",
        'x25',   "SocketTransactionExpirationInterval",    &REG_DWORD,   270000,                   "",
        'x25',   "PingInterval",                           &REG_DWORD,   600000,                   "",
        'x25',   "X25ErrorsBeforeReset",                   &REG_DWORD,   5,                        "",
        'x25',   "X25ErrorResetInterval",                  &REG_DWORD,   60000,                    "",
        'x25',   "X25ResetTryInterval",                    &REG_DWORD,   60000,                    "",
        'x25',   "X25NoResponseIntervalTilReset",          &REG_DWORD,   270000,                   "",
        'x25',   "PongTimeout",                            &REG_DWORD,   10000,                    "",
        'x25',   "WaitForEiconInit",                       &REG_DWORD,   60000,                    "",
        'svr',   "Datastore_CreatePerfCounters",           &REG_DWORD,   1,                        "",
        'svr',   "PerfLogIntervalMS",                      &REG_DWORD,   300000,                   "",
        'svr',   "EnableAutoPassword",                     &REG_DWORD,   0,                        "1=enable the Automated Password Reminder script, 0=disable with nice message 0xFFFFFFFF=disable with hack abort",
        'x25',   "EiconPort",                              &REG_DWORD,   1,                        "",
        'x25',   "SweepInterval",                          &REG_DWORD,   5000,                     "",
        'x25',   "TransactionExpirationInterval",          &REG_DWORD,   240000,                   "",
        'x25',   "TransactionExpIntervalQueue0",           &REG_DWORD,   120000,                   "",
        'x25',   "TransactionExpIntervalQueue1",           &REG_DWORD,   100000,                   "",
        'x25',   "TransactionExpIntervalQueue2",           &REG_DWORD,   15000,                    "",
        'x25',   "ReceiveBufferSize",                      &REG_DWORD,   8192,                     "",
        'x25',   "NumThreads",                             &REG_DWORD,   1,                        "",
        'x25',   "ipmask",                                 &REG_SZ,      "255.255.255.255",        "",
        'x25',   "SMIQueue",                               &REG_DWORD,   5,                        "",
        'x25',   "SMIQueueRecheckCount",                   &REG_DWORD,   10,                       "",
        'x25',   "fHeavyInstrument",                       &REG_DWORD,   1,                        "1=Enable heavy instrumentation",
        'x25',   "X25TimeoutErrorThreshold",               &REG_DWORD,   60,                       "",
        'x25',   "ShortCircuitTimeout",                    &REG_DWORD,   210000,                   "",
        'x25',   "HopelesslyLostTime",                     &REG_DWORD,   (15*60*1000),             "",
        'x25',   "QueueServiceRate",                       &REG_DWORD,   3,                        "",
        'x25',   "RemoteDTEST0",                           &REG_SZ,      "234523456789",           "",
        'x25',   "LocalDTEST0",                            &REG_SZ,      "234523456788",           "",
        'x25',   "RemoteDTESMI0",                          &REG_SZ,      "234523456786",           "",
        'x25',   "LocalDTESMI0",                           &REG_SZ,      "234523456788",           "",
        'x25',   "STQueue0",                               &REG_DWORD,   36,                       "",
        'x25',   "STSimultaneousTimeoutsMax0",             &REG_DWORD,   18,                       "",
        'x25',   "RemoteDTEST1",                           &REG_SZ,      "234523456789",           "",
        'x25',   "LocalDTEST1",                            &REG_SZ,      "234523456788",           "",
        'x25',   "RemoteDTESMI1",                          &REG_SZ,      "234523456784",           "",
        'x25',   "LocalDTESMI1",                           &REG_SZ,      "234523456788",           "",
        'x25',   "STQueue1",                               &REG_DWORD,   36,                       "",
        'x25',   "STSimultaneousTimeoutsMax1",             &REG_DWORD,   18,                       "",
        'x25',   "RemoteDTEST2",                           &REG_SZ,      "234523456789",           "",
        'x25',   "LocalDTEST2",                            &REG_SZ,      "234523456788",           "",
        'x25',   "RemoteDTESMI2",                          &REG_SZ,      "234523456788",           "",
        'x25',   "LocalDTESMI2",                           &REG_SZ,      "234523456788",           "",
        'x25',   "STQueue2",                               &REG_DWORD,   36,                       "",
        'x25',   "STSimultaneousTimeoutsMax2",             &REG_DWORD,   18,                       "",
        'x25',   "RemoteDTEST3",                           &REG_SZ,      "234523456790",           "",
        'x25',   "LocalDTEST3",                            &REG_SZ,      "234523456788",           "",
        'x25',   "RemoteDTESMI3",                          &REG_SZ,      "234523456791",           "",
        'x25',   "LocalDTESMI3",                           &REG_SZ,      "234523456788",           "",
        'x25',   "STQueue3",                               &REG_DWORD,   10,                       "",
        'x25',   "STSimultaneousTimeoutsMax3",             &REG_DWORD,   5,                        "",
        'x25',   "RemoteDTEST4",                           &REG_SZ,      "234523456792",           "",
        'x25',   "LocalDTEST4",                            &REG_SZ,      "234523456788",           "",
        'x25',   "RemoteDTESMI4",                          &REG_SZ,      "234523456780",           "",
        'x25',   "LocalDTESMI4",                           &REG_SZ,      "234523456788",           "",
        'x25',   "STQueue4",                               &REG_DWORD,   10,                       "",
        'x25',   "STSimultaneousTimeoutsMax4",             &REG_DWORD,   5,                        "",
        'x25',   "RemoteDTEST5",                           &REG_SZ,      "234523456789",           "",
        'x25',   "LocalDTEST5",                            &REG_SZ,      "234523456788",           "",
        'x25',   "RemoteDTESMI5",                          &REG_SZ,      "234523456778",           "",
        'x25',   "LocalDTESMI5",                           &REG_SZ,      "234523456788",           "",
        'x25',   "STQueue5",                               &REG_DWORD,   10,                       "",
        'x25',   "STSimultaneousTimeoutsMax5",             &REG_DWORD,   5,                        "",
        'x25',   "STSimultaneousTimeoutsMax0",             &REG_DWORD,   18,                       "",
        'x25',   "ThrottleMaxPendingST0",                  &REG_DWORD,   18,                       "",
        'x25',   "ThrottleMaxPendingSMI0",                 &REG_DWORD,   18,                       "",
        'x25',   "ThrottleMaxPendingST1",                  &REG_DWORD,   18,                       "",
        'x25',   "ThrottleMaxPendingSMI1",                 &REG_DWORD,   4,                        "",
        'x25',   "ThrottleMaxPendingST2",                  &REG_DWORD,   18,                       "",
        'x25',   "ThrottleMaxPendingSMI2",                 &REG_DWORD,   20,                       "20 for MOSWest and Saturn, 5 for Rasta",
        'x25',   "ThrottleMaxPendingST3",                  &REG_DWORD,   3,                        "",
        'x25',   "ThrottleMaxPendingSMI3",                 &REG_DWORD,   5,                        "",
        'x25',   "ThrottleMaxPendingST4",                  &REG_DWORD,   18,                       "18 for MOSWest and Saturn, 5 for Rasta",
        'x25',   "ThrottleMaxPendingSMI4",                 &REG_DWORD,   5,                        "5 for MOSWest and Saturn, 20 for Rasta",
        'x25',   "ThrottleMaxPendingST5",                  &REG_DWORD,   5,                        "",
        'x25',   "ThrottleMaxPendingSMI5",                 &REG_DWORD,   5,                        "",
        'x25',   "ThrottleMaxPendingEffect",               &REG_DWORD,   70,                       "",
        'x25',   "ThrottleMaxQueueEffect",                 &REG_DWORD,   100,                      "",        
        'dsn',   "ServerETA",                              &REG_SZ,      "ServerETA",              "",
        'dsn',   "ServerFare",                             &REG_SZ,      "ServerFare",             "",
        'dsn',   "CRSLog",                                 &REG_SZ,      "CRSLog",                 "",
        'dsn',   "HotelFinder",                            &REG_SZ,      "HotelFinder",            "",
        'usg',   "Enable",                                 &REG_DWORD,   1,                        "",
        'ccd',   "Card0DigitCheck",                        &REG_DWORD,   1,                        "Card0 (Visa) - do the mod 10 digit check",
        'ccd',   "Card0StartDigitLow0",                    &REG_SZ,      "4",                      "Card0 (Visa) - starting digits in range of 4",
        'ccd',   "Card0StartDigitHigh0",                   &REG_SZ,      "4",                      "Card0 (Visa) -   to 4...",
        'ccd',   "Card0NumValidDigits0",                   &REG_DWORD,   13,                       "Card0 (Visa) - can have 13 digits",
        'ccd',   "Card0NumValidDigits1",                   &REG_DWORD,   16,                       "Card0 (Visa) - can have 16 digits",
        'ccd',   "Card0AirlinesRefuse",                    &REG_SZ,      "6U,LZ,WD,6F,WT,PK,RO",   "Not accepted by Air Ukraine, Balkan Bulgarian, Halisa Air, Laker Airways, Nigeria Airways, Pakistan Intl, Tarom Romanian",
        'ccd',   "Card1DigitCheck",                        &REG_DWORD,   1,                        "Card1 (MC)   - do the mod 10 digit check",
        'ccd',   "Card1StartDigitLow0",                    &REG_SZ,      "51",                     "Card1 (MC)   - starting digits in range of 51",
        'ccd',   "Card1StartDigitHigh0",                   &REG_SZ,      "55",                     "Card1 (MC)   -   to 55...",
        'ccd',   "Card1NumValidDigits0",                   &REG_DWORD,   13,                       "Card1 (MC)   - can have 13 digits",
        'ccd',   "Card1NumValidDigits1",                   &REG_DWORD,   16,                       "Card1 (MC)   - can have 16 digits",
        'ccd',   "Card1AirlinesRefuse",                    &REG_SZ,      "6U,GF,WD,6F,WT,PK,RO",   "Not accepted by Air Ukraine, Gulf Air, Halisa Air, Laker Airways, Nigeria Airways, Pakistan Intl, Tarom Romanian",
        'ccd',   "Card2DigitCheck",                        &REG_DWORD,   1,                        "Card2 (AmEx) - do the mod 10 digit check",
        'ccd',   "Card2StartDigitLow0",                    &REG_SZ,      "34",                     "Card2 (AmEx) - starting digits in range of 34",
        'ccd',   "Card2StartDigitHigh0",                   &REG_SZ,      "34",                     "Card2 (AmEx) -   to 34...",
        'ccd',   "Card2StartDigitLow1",                    &REG_SZ,      "37",                     "Card2 (AmEx) - or starting digits in range of 37",
        'ccd',   "Card2StartDigitHigh1",                   &REG_SZ,      "37",                     "Card2 (AmEx) -   to 37...",
        'ccd',   "Card2NumValidDigits0",                   &REG_DWORD,   15,                       "Card2 (AmEx) - can have 15 digits",
        'ccd',   "Card2AirlinesRefuse",                    &REG_SZ,      "NK,RO",                  "Not accepted by Spirit Airlines, Tarom Romanian",
        'ccd',   "Card3DigitCheck",                        &REG_DWORD,   1,                        "Card3 (Discover) - do the mod 10 digit check",
        'ccd',   "Card3StartDigitLow0",                    &REG_SZ,      "6011",                   "Card3 (Discover) - starting digits in range of 6011",
        'ccd',   "Card3StartDigitHigh0",                   &REG_SZ,      "6011",                   "Card3 (Discover) -   to 6011...",
        'ccd',   "Card3NumValidDigits0",                   &REG_DWORD,   16,                       "Card3 (Discover) - can have 16 digits",
        'ccd',   "Card3AirlinesRefuse",                    &REG_SZ,      "EI,SU,AF,AI,CW,FJ,UK,6U,NH,IW,OZ,OS,LZ,BA,CP,C8,CI,MS,LY,EK,BR,AY,GA,GH,GF,GY,WD,JL,KL,KU,6F,LO,8M,MH,MA,OA,PK,PR,PH,RJ,SN,SV,SK,SA,GD,TP,RO,TR,T3,RG,OD", "Not accepted by a bunch of airlines...",
        'ccd',   "Card4DigitCheck",                        &REG_DWORD,   0,                        "Card4 (Diners Card) - do the mod 10 digit check",
        'ccd',   "Card4StartDigitLow0",                    &REG_SZ,      "38",                     "Card4 (Diners Card) - starting digits in range of 38",
        'ccd',   "Card4StartDigitHigh0",                   &REG_SZ,      "38",                     "Card4 (Diners Card) -   to 38...",
        'ccd',   "Card4NumValidDigits0",                   &REG_DWORD,   14,                       "Card4 (Diners Card) - can have 14 digits",
        'ccd',   "Card5DigitCheck",                        &REG_DWORD,   0,                        "Card5 (UATP Card) - bugbug: what's the right check?",
        'ccd',   "Card6DigitCheck",                        &REG_DWORD,   0,                        "Card6 (Carte Blanche) - bugbug: what's the right check?",
        'ssp',   "MaxHotelReturn",                         &REG_DWORD,   25,                       "The maximum number of hotels to return to the UI.",
        'ssp',   "MaxHotelRetrieve",                       &REG_DWORD,   100,                      "The maximum number of hotels to return to the UI.",
        'ssp',   "MaxFareReturn",                          &REG_DWORD,   50,                       "The maximum number of hotels to return to the UI.",
        'ssp',   "CreditCheckDisabled",                    &REG_DWORD,   0,                        "1 = enabled, 0 = disabled",
        'ssp',   "BogusCreditCard",                        &REG_SZ,      "1234999987654321",       "The bogus credit card number",
        'ssp',   "BogusCreditCardEnabled",                 &REG_DWORD,   1,                        "not used",
        'ssp',   "IATA000",                                &REG_SZ,      "1153818",                "",
        'ssp',   "IATA001",                                &REG_SZ,      "1153818",                "",
        'ssp',   "IATA002",                                &REG_SZ,      "1153818",                "",
        'ss1',   "AAA",                                    &REG_SZ,      "R7W4",                   "",
        'ss1',   "PrimarySTAR",                            &REG_SZ,      "",                       "",
        'ss1',   "SecondarySTAR",                          &REG_SZ,      "",                       "",
        'ssm',   "ServerPort",                             &REG_DWORD,   7127,                     "Socket port used to communicate with SSM Server.",
        'ssm',   "Server",                                 &REG_SZ,      "travsab1",               "SSM Server name.",
        'ssm',   "BackupServers",                          &REG_SZ,      "",                       "Comma separated list of SSM Server's to use if the main one goes down (or its UDR dies)...",
        'ssm',   "GDSPort",                                &REG_DWORD,   6969,                     "Port on UDR for GDS transactions.",
        'ssm',   "TextPort",                               &REG_DWORD,   6966,                     "Port on UDR for native host transactions (i.e. for SabreTPF).",
        'ssm',   "PendingInterval",                        &REG_DWORD,   15000,                    "When we have a session 'out' we wake up this often to see if it should be expired.",
        'ssm',   "KeepAliveInterval",                      &REG_DWORD,   60000,                    "Wake up this often to see if any of our 'in' sessions need to be 're-primed', lest they timeout on Sabre.",
        'ssm',   "RePrimeInterval",                        &REG_DWORD,   420000,                   "When the keep-alive interval kicks off, it looks for sessions that haven't been used in this long, and re-primes them.",
        'ssm',   "MaxIdleInterval",                        &REG_DWORD,   420000,                   "When a session is out, the pending thread will expire it if it hasn't been used in this long.",
        'ssm',   "ComTimeoutInterval",                     &REG_DWORD,   60000,                    "When waiting for a response from Sabre, if it doesn't come back in this time, we time it out.",
        'ssm',   "StealInterval",                          &REG_DWORD,   10000,                    "If blocked in GetSession(), we try to steal sessions from the other port this often.",
        'ssm',   "RetryPrimaryInterval",                   &REG_DWORD,   180000,                   "When we lose our connection to the primary server, we try to reconnect this often.",
        'ssm',   "StartingSessions",                       &REG_DWORD,   10,                       "When the server init's, it primes this many sessions for use (will grow dynamically, if necessary/possible).",
        'ssm',   "UDRServer",                              &REG_SZ,      "157.54.208.53",          "UDR's IP or server name.",
        'ssm',   "NumCIDs",                                &REG_DWORD,   1,                        "# of Countries supported by SSM (each must have a registry entry PseudoCityForCIDx (where x is 0 through NumCIDs-1).  Must be in same order as TravelCountryId...",
        'ssm',   "PseudoCityForCID0",                      &REG_SZ,      "R7W4",                   "PseudoCity for CID # 0 (USA).",
        'wse',   "ExcludeCarriers",                        &REG_SZ,      "LZ 2G ZK GF 6F PK NK RB NE RN J7 5C UP 8M M5", "",
        'wsc',   "PlatingCarriers",                        &REG_SZ,      "2R 3M 7H AA AC AD AF AI AM AN AQ AR AS AT AV AY AZ BA BD BR BW CF CI CM CO CP CW CX DL EH EI EK F9 FF FI FJ FL FQ GA GD GH GU GY HA HP IB IW JI JL JM JR KE KL KP KS KU KW KX LA LB LH LM LO LR LT LY MA MH ML MP MS MX NH NW NZ OA OK OS OZ PH PL PR PY PZ QF QQ RG RJ RK RO RV SA SK SN SQ SR SU SV SX T3 TA TE TG TK TP TR TW TZ UA UC UK UN US VC VE VP VS VX WV YV YX", "",
        'wsv',   "ExcludeVerbiageUnknown",                 &REG_SZ,      "\tCOND STOPOVER TRAF\tNO LOCAL TRAFFIC\tSUBJ TO GOVT APPROVAL\t", "",
        'wsx',   "ExcludeVerbiageOperatedBy",              &REG_SZ,      "",                       "",
        'wsp',   "ThreadHungInterval",                     &REG_DWORD,   (45*60*1000),             "(45 min) response processing threads that live longer than this are assumed hung",
        'wsp',   "PerfDumpIntervalMS",                     &REG_DWORD,   (15*60*1000),             "(15 min) wssp performanec logging interval",
        'lfs',   "CRSLoggingOn",                           &REG_DWORD,   1,                        "Log LFS transactions?",
        'lfs',   "LFSServer",                              &REG_SZ,      "travsab1",               "LFS server name or IP.",
        'lfs',   "LFSPort",                                &REG_DWORD,   3000,                     "Port on LFS server.",
        'lfs',   "LFSTimeout",                             &REG_DWORD,   480000,                   "We will wait this many milliseconds for a response from LFS or timeout from pending thread (this value should be gt LFSExpirePending).",
        'lfs',   "LFSWakeupPending",                       &REG_DWORD,   60000,                    "Pending thread wakes up every this many milliseconds (when there are any transactions pending).",
        'lfs',   "LFSExpirePending",                       &REG_DWORD,   420000,                   "Pending thread times out transactions > than this many millseconds.",
        'lfs',   "LFSReconnectInterval",                   &REG_DWORD,   10000,                    "When we lose the socket to the LFS server, we'll retry to connect every this often.",
        'ods',   "CRSLog",                                 &REG_SZ,      "SQL Server",             "",
        'ods',   "HotelFinder",                            &REG_SZ,      "SQL Server",             "",
        'ods',   "ServerETA",                              &REG_SZ,      "SQL Server",             "",
        'ohf',   "Description",                            &REG_SZ,      "Travel Group Database",  "",
        'ohf',   "Driver",                                 &REG_SZ,      $sSQLServerDriverLoc,     "",
        'ohf',   "UseProcForPrepare",                      &REG_SZ,      "No",                     "",
        'ohf',   "QuotedId",                               &REG_SZ,      "Yes",                    "",
        'ohf',   "AnsiNPW",                                &REG_SZ,      "Yes",                    "",
        'ohf',   "OemToAnsi",                              &REG_SZ,      "No",                     "",
        'ohf',   "LastUser",                               &REG_SZ,      "",                       "",
        'ohf',   "Trusted_Connection",                     &REG_SZ,      "",                       "",
        'ocl',   "Description",                            &REG_SZ,      "Travel Group Database",  "",
        'ocl',   "Driver",                                 &REG_SZ,      $sSQLServerDriverLoc,     "",
        'ocl',   "UseProcForPrepare",                      &REG_SZ,      "No",                     "",
        'ocl',   "QuotedId",                               &REG_SZ,      "Yes",                    "",
        'ocl',   "AnsiNPW",                                &REG_SZ,      "Yes",                    "",
        'ocl',   "OemToAnsi",                              &REG_SZ,      "No",                     "",
        'ocl',   "LastUser",                               &REG_SZ,      "",                       "",
        'ocl',   "Trusted_Connection",                     &REG_SZ,      "",                       "",
        'ose',   "Driver",                                 &REG_SZ,      $sSQLServerDriverLoc,     "",
        'ose',   "Description",                            &REG_SZ,      "Travel Group Database",  "",
        'ose',   "UseProcForPrepare",                      &REG_SZ,      "No",                     "",
        'ose',   "QuotedId",                               &REG_SZ,      "Yes",                    "",
        'ose',   "AnsiNPW",                                &REG_SZ,      "Yes",                    "",
        'ose',   "OemToAnsi",                              &REG_SZ,      "No",                     "",
        'ose',   "Trusted_Connection",                     &REG_SZ,      "",                       "",
        'ose',   "LastUser",                               &REG_SZ,      "",                       "",
    );

    # These values are never asked for.  The value may not be specified in an
    # import file.  If present in the registry, it will be deleted.
    @srRegObsolete =
    ( # --Loc--  --Name-------------                       --Class----   -Val-                     --Description-------------------------
    );

    # These values are never asked for.  The value may be overridden by an
    # option in an import file.  Unless overridden, the registry value will
    # not be modified.
    @srRegOptional =
    ( # --Loc--  --Name-------------                       --Class----   -Val-                     --Description-------------------------
        'wsp',   "LOGSPXX000",                             &REG_DWORD,   0,                        "kNone",
        'wsp',   "LOGSPXX001",                             &REG_DWORD,   0,                        "kAirAvail",
        'wsp',   "LOGSPXX002",                             &REG_DWORD,   1,                        "kPassthrough",
        'wsp',   "LOGSPXX003",                             &REG_DWORD,   1,                        "kPricing",
        'wsp',   "LOGSPXX004",                             &REG_DWORD,   0,                        "kFareRules",
        'wsp',   "LOGSPXX005",                             &REG_DWORD,   1,                        "kBooking",
        'wsp',   "LOGSPXX006",                             &REG_DWORD,   0,                        "kCarAvail",
        'wsp',   "LOGSPXX007",                             &REG_DWORD,   0,                        "kCarRules",
        'wsp',   "LOGSPXX008",                             &REG_DWORD,   0,                        "kPowerShopper",
        'wsp',   "LOGSPXX009",                             &REG_DWORD,   0,                        "kFare",
        'wsp',   "LOGSPXX010",                             &REG_DWORD,   1,                        "kHotelAvail",
        'wsp',   "LOGSPXX011",                             &REG_DWORD,   1,                        "kCredAvail",
        'wsp',   "LOGSPXX012",                             &REG_DWORD,   1,                        "kFlightReserve",
        'wsp',   "LOGSPXX013",                             &REG_DWORD,   1,                        "kCarPurchase",
        'wsp',   "LOGSPXX014",                             &REG_DWORD,   1,                        "kFlightPurchase",
        'wsp',   "LOGSPXX015",                             &REG_DWORD,   0,                        "kCurrencyConvert",
        'wsp',   "LOGSPXX016",                             &REG_DWORD,   1,                        "kFlightCancel",
        'wsp',   "LOGSPXX017",                             &REG_DWORD,   1,                        "kFlightChange",
        'wsp',   "LOGSPXX018",                             &REG_DWORD,   1,                        "kCurrentItin",
        'wsp',   "LOGSPXX019",                             &REG_DWORD,   1,                        "kHotelBook",
        'wsp',   "LOGSPXX020",                             &REG_DWORD,   1,                        "kCarCancel",
        'wsp',   "LOGSPXX021",                             &REG_DWORD,   0,                        "kCarDetails",
        'wsp',   "LOGSPXX022",                             &REG_DWORD,   0,                        "kCarAllDetails",
        'wsp',   "LOGSPXX023",                             &REG_DWORD,   1,                        "kTravelRecordCancel",
        'wsp',   "LOGSPXX024",                             &REG_DWORD,   0,                        "kSeatAssign",
        'wsp',   "LOGSPXX025",                             &REG_DWORD,   1,                        "kOpenPNR",
        'wsp',   "LOGSPXX026",                             &REG_DWORD,   1,                        "kPlacePNRInQueue",
        'wsp',   "LOGSPXX027",                             &REG_DWORD,   1,                        "kCommitChanges",
        'wsp',   "LOGSPXX028",                             &REG_DWORD,   1,                        "kHotelRules",
        'wsp',   "LOGSPXX029",                             &REG_DWORD,   1,                        "kHotelCancel",
        'wsp',   "LOGSPXX030",                             &REG_DWORD,   0,                        "kCarFetch",
        'wsp',   "LOGSPXX031",                             &REG_DWORD,   1,                        "kHotelFetch",
        'wsp',   "LOGSPXX032",                             &REG_DWORD,   0,                        "kZoneDiff",
        'wsp',   "LOGSPXX033",                             &REG_DWORD,   0,                        "kSetCustRefNum",
        'wsp',   "LOGSPXX034",                             &REG_DWORD,   1,                        "kPricePNR",
        'wsp',   "LOGSPXX035",                             &REG_DWORD,   1,                        "kSMIPassthrough",
        'wsp',   "LOGSPXX036",                             &REG_DWORD,   1,                        "kNativeMode",
        'wsp',   "LOGSPXX037",                             &REG_DWORD,   1,                        "kFlightOptions",
        'wsp',   "LOGSPXX038",                             &REG_DWORD,   1,                        "kSetItinInfo",
        'wsp',   "LOGSPXX039",                             &REG_DWORD,   0,                        "kAirSegmentData",
        'wsp',   "LOGSPXX040",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX041",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX042",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX043",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX044",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX045",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX046",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX047",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX048",                             &REG_DWORD,   0,                        "",
        'wsp',   "LOGSPXX049",                             &REG_DWORD,   0,                        "",
    );

    return $true
}
