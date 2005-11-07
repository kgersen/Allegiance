#========================================================================
#
# Microsoft Allegiance Server Setup Script
#
# Copyright <cp> 1997 Microsoft Corporation, All Rights Reserved
#
# Modification History:
#   ---Date---  -Blame-- -Description of change------------------------
#   1998.08.11  MarkSn   Created.
#
#========================================================================
{
    require "nt.ph";
    require "registry.pl";

    $true = (1 == 1);
    $false = (!$true);
    $fLogAnyway = $false; # $true;

    print "Initializing...\n";

    &_vInitializeSetupOptions();

    $logfilename = &_sOpenLog ($ENV{'temp'}, $ENV{'COMPUTERNAME'});

	do
	{
	    $cAction = &_cGetDirections();
	} until ($cAction eq 'n' || ($cAction eq 'x' && &_fAskYN($true, "Are you sure you want to exit without finishing the install")) || $cAction eq 'y');

    if ($cAction eq 'n')
    {
        &NTWriteEventLog( $ENV{'COMPUTERNAME'}, "instserv.pl", EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 0, "\nStarting Installation.");

        &_vSetSettingValuesReg("Components");
        &_vSetSettingValuesReg("ServerFiles");
        &_vSetSettingValuesReg("Database");
        &_vSetSettingValuesReg("Sentinal");

        # Walk through the installation functions for the various components
        my @srInstallFunctions = &_lGetInstallFunctions();
        foreach $item (@srInstallFunctions)
        {
            &_vPrintHeading();
            &$item;
        }

        &NTWriteEventLog( $ENV{'COMPUTERNAME'}, "instserv.pl", EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 0, "\nInstallation Complete.");
    }

    close PROPLOG;
    $fLogOpen = $false;
    if (&_sGetSettingValueByName('Components', 'logdir') ne "")
    {
        my $sExportLogDir = &_sGetSettingValueByName('Components', 'logdir');
        if ($sExportLogDir ne "" && &_fAskYN($true, "Export log to '$sExportLogDir'"))
        {
            &_fExecuteCmd ("copy $logfilename $sExportLogDir", "exporting log", "exporting log to: $sExportLogDir");
        }
    }
    else
    {
        &_vLog ("Installation results can be found in $logfilename");
    }

    print "\nGame over.  Thank you for playing.\n";
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

    my $bUserChangedValues = $false;
    my $bOptionsAreActive  = $false;

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
        $bOptionsAreActive  = $false;
        for ($i = 0; $i < $nOptions; $i+=4)
        {
            $sValidChoices .= $$sOptions[$i];
            $cActive = &_cGetActiveFlagByName($sDialogName, $$sOptions[$i+1]);
            if ($cActive ne ' ')
            {
                $bOptionsAreActive = $true;
            }
            print " $cActive $$sOptions[$i].  $$sOptions[$i+2]\n";
        }

        print "\n";
        print "- What do you want to do? ----------------------------------------------\n";
        print "Edit setting/option by letter/number             (n) Next       (x) Exit\n\n";
        $sValidChoices .= "nx";
        $cChoice = &_cAskSet($true, "Please choose one:", $sValidChoices, "x");
        if (index("nx", $cChoice) ge 0)
        {
            if (!$bUserChangedValues || $bOptionsAreActive || &_fAskYN( $true, "No action selected.  Continue?"))
            {
                return $cChoice;
            }
        }
        elsif ($cChoice =~ /[a-m]/) # valid settings can be a thru m
        {
            my $cChoiceName  = &_sGetSettingNameByIndex($sDialogName, $cChoice);
            my $sChoiceValue = &_sGetSettingValueByName($sDialogName, $cChoiceName);
            my $sChoiceDesc  = &_sGetSettingDescByName($sDialogName, $cChoiceName);

            $bUserChangedValues = $true;

            $sChoiceValue    = &_sQueryValue($sChoiceValue, $sChoiceDesc);
            &_vSetSettingValueByName($sDialogName, $cChoiceName, $sChoiceValue);
        }
        else
        {
            my $cChoiceName  = &_sGetOptionNameByIndex($sDialogName, $cChoice);
            my $sChoiceDesc  = &_sGetOptionDescByName($sDialogName, $cChoiceName);
            my $sChoiceValue = &_sGetOptionValueByName($sDialogName, $cChoiceName);
            if ($sChoiceValue eq ' ')
            {
                &_vSetOptionValueByName($sDialogName, $cChoiceName, 'X');
            }
            elsif($sChoiceValue eq 'X')
            {
                &_vSetOptionValueByName($sDialogName, $cChoiceName, ' ');
            }
            else
            {
                &_cDialog($sChoiceValue);
            }
        }
    }
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
# This function is called to return a list of functions to call to perform
# the installation of the components selected by the user.
#
sub _lGetInstallFunctions
{
    my @sr;
    my $fGotit = $false;
    foreach $compkey (keys %srInstallMainFunction)
    {
        $sCompName   = &_sGetOptionNameByIndex('Components', $compkey);
        $cCompActive = &_cGetActiveFlagByName('Components', $sCompName);
        if (!$fGotit && $cCompActive ne ' ')
        {            
            $fGotit = $true;    # TODO:  this needs to check if the function is already in the array or not
            push @sr, $srInstallMainFunction{$compkey};
        }
    }

    return @sr;
}

# ===========================================================================
# This function determines what the user wants us to do.  This can come from
# command-line parameters, which could also specify an import file from which
# to get more directions, or we could walk the user through a set of dialogs
# to query for the instructions.  Returns 'n' if successful in getting the
# options necessary to continue, 'x' if user wants to exit, and 'y' if a bad
# command line parameter was entered, implying exit.
#
sub _cGetDirections
{
    if (($#ARGV+1) eq 0)
    {
        # no command line parameters means that the user wants us to ask what to do.
		return &_cDialog('Components');
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
                    return 'y';
                }
                $g_fQuiet = $true;
            }
            elsif ($ARGV[$i] =~ /-s()*/)
            {
                # parse for filename as next token
                $i++;
                if ($i gt $#ARGV)
                {
                    print "Syntax:  ran out of tokens for '-s' source dir.\n";
                    &_vPrintHelp();
                    return 'y';
                }
                if ($ARGV[$i] =~ /-()*/)
                {
                    print "Syntax:  missing or invalid filename for '-s': '$ARGV[$i]'.\n";
                    &_vPrintHelp();
                    return 'y';
                }
				&_vSetSettingValueByName('Components', 'srcdir', $ARGV[$i])
            }
            elsif ($ARGV[$i] =~ /-l()*/)
            {
                # parse for filename as next token
                $i++;
                if ($i gt $#ARGV)
                {
                    print "Syntax:  ran out of tokens for '-l' log dir.\n";
                    &_vPrintHelp();
                    return 'y';
                }
                if ($ARGV[$i] =~ /-()*/)
                {
                    print "Syntax:  missing or invalid filename for '-l': '$ARGV[$i]'.\n";
                    &_vPrintHelp();
                    return 'y';
                }
				&_vSetSettingValueByName('Components', 'logdir', $ARGV[$i])
            }
            elsif ($ARGV[$i] =~ /-h()*/ || $ARGV[$i] =~ /-\?()*/)
            {
                &_vPrintHelp();
                return 'y';
            }
            else 
            {
                print "Syntax:  Unrecognized token.\n";
                &_vPrintHelp();
                return 'y';
            }
        }
    }

    return 'n';
}

# ===========================================================================
# This function acts on what the user specified they want us to do.
#
sub _vPrintHelp
{
    # &_vPrintHeading();
    print "Format:  Instserv [options]\n";
    print "  where [options] can be one or more of the following:\n";
    print "    -l(ogdir) <directory>\n";
    print "       The <directory> specified where logs will be copied.\n";
    print "    -s(ourcedir) <directory>\n";
    print "       The <directory> specifies location of setup bits.\n";
    print "    -q(uiet)\n";
    print "       Causes no output to be displayed on the screen.\n";
    print "       Syntax or other errors will still be displayed.\n";
    print "    -h(elp), -?\n";
    print "       Causes this help screen to be displayed.\n";
    return;
}

#============================================================================
# Allegiance Game Server
#============================================================================

sub _vInstallServer
{
    &_vLogSilent ("Installing Server Component.");

    my $sSourceDir = &_sGetSettingValueByName('Components',     'srcdir');
    my $fInstServ  = (&_sGetOptionValueByName('ServerFiles',    'srvr') eq 'X');
    my $sDestDir   = &_sGetSettingValueByName('ServerFiles',    'dstdir');
    my $sShareDir  = &_sGetSettingValueByName('ServerFiles',    'shrdir');
    my $sConfigDir = &_sGetSettingValueByName('ServerFiles',    'cfgdir');
    my $fDBUpdates = (&_sGetOptionValueByName('Database',		'dbupdt') eq 'X');
    my $sDBUpdDir  = &_sGetSettingValueByName('Database',		'dbudir');
    my $sDBMachine = &_sGetSettingValueByName('Database',		'sqlbox');
    my $sDBName    = &_sGetSettingValueByName('Database',		'dbname');

    #============================================================================
    # copy/extract files from the source dir to the new dir
    #
    if ($fInstServ)
    {
        my $rc = $false;

        do {  &_vNewDir ("$sDestDir.new", $true, $true);
        } until (-d "$sDestDir.new" || &_vDieAndLogReason ("create $sDestDir.new"));
        do { $rc = &_fExecuteCmd ("$system\\xcopy.exe $sSourceDir\\server\\debug\\*.* $sDestDir.new");
        } until ($rc eq 0 || &_vDieAndLogReason("copying '$sSourceDir\\server\\debug\\*.*' to '$sDestDir.new' ($rv)."));

        do { &_vNewDir ("$sShareDir.new", $true, $true);
        } until (-d "$sShareDir.new" || &_vDieAndLogReason ("create $sShareDir.new"));
        do { $rc = &_fExecuteCmd ("$system\\xcopy.exe $sSourceDir\\server\\shared\\*.* $sShareDir.new");
        } until ($rc eq 0 || &_vDieAndLogReason("copying '$sSourceDir\\server\\shared\\*.*' to '$sShareDir.new' ($rv)."));

        do { &_vNewDir ("$sConfigDir.new", $true, $true);
        } until (-d "$sConfigDir.new" || &_vDieAndLogReason ("create $sConfigDir.new"));
        do { $rc = &_fExecuteCmd ("$system\\xcopy.exe $sSourceDir\\server\\config\\*.* $sConfigDir.new");
        } until ($rc eq 0 || &_vDieAndLogReason("copying '$sSourceDir\\server\\config\\*.*' to '$sConfigDir.new' ($rv)."));
    }
    if ($fDBUpdates)
    {
        do { &_vNewDir ("$sDBUpdDir.new", $true, $true);
        } until (-d "$sDBUpdDir.new" || &_vDieAndLogReason ("create $sDBUpdDir.new"));
        do { $rc = &_fExecuteCmd ("$system\\xcopy.exe $sSourceDir\\database\\*.* $sDBUpdDir.new");
        } until ($rc eq 0 || &_vDieAndLogReason("copying '$sSourceDir\\database\\*.*' to '$sDBUpdDir.new'."));
    }

    #============================================================================
    # If we're updating the binaries or updating the database, then we need to 
    # first stop the service.
    #
    if ($fInstServ || $fDBUpdates)
    {
        my $fRunning = &_sGetProcessIdByServiceName("fedsrv");
        if ($fRunning ne '')
        {
            &_vLog("Allegiance Game Server currently running (pid=$fRunning).");

            print "\nWe now need to stop and remove the old version of the server.\n";
            print "It is up to you to make sure that you are ready for this to happen.\n";
            print "This is the time-critical stage, so make sure that the machine\n";
            print "being upgraded is ready to be taken offline.\n";
            if (&_fAskYN ($true, "Stop Server?"))
            {
                &_fStopService ("fedsrv", "fedsrv.exe", 10);
            }
        }
        else
        {
            &_vLog("Allegiance Game Server service not running.");
        }
    }

    #============================================================================
    # If we're updating the server binaries, then we need to uninstall
    # the currently installed service in preparation for the switch.
    #
    if ($fInstServ)
    {
        my $sServerPath = &_sGetServiceInstallPath( "fedsrv" );
        if ($sServerPath ne '')
        {
            &_vLog("Server currently installed at '$sServerPath'.");

            # try to remove the service, looking for the old version of the server
            if (-e "$sServerPath\\fedsrv.exe")
            {
                # previous install. Removal can fail. That's ok.
                &_fExecuteCmd ("$sServerPath\\fedsrv.exe -remove", "Removing previous version of service...", "OK");
            }
            else
            {
                &_vLog( "*** Allegiance Game Server service is installed, but the executable");
                &_vLog( "*** does not exist.  Unable to uninstall service.");
            }

            if ($sServerPath ne $sDestDir)
            {
                &_vLog( "*** Since the Allegiance Game Server service is being installed to a different");
                &_vLog( "*** path than before, you will need to remove the old files manually.");
            }
        }
        else
        {
            &_vLog("Allegiance Game Server service not currently installed.");
        }
    }

    #============================================================================
    # In the words of Michael Jackson, "Make that change."
    #
    if ($fInstServ)
    {
        &_sSwitchDir ($sDestDir);
        &_sSwitchDir ($sShareDir);
        &_sSwitchDir ($sConfigDir);

        &_vUpdateSystemFiles ($sShareDir, "$ENV{'windir'}\\System32");
        &_vChDriveDir ($sDestDir);
    }
    if ($fDBUpdates)
    {
        &_sSwitchDir ($sDBUpdDir);
    }

    #============================================================================
    # setup registry and install new services for this server
    #
    if ($fInstServ)
    {
        my $rc = $false;

        do { $rc = &_fExecuteCmd ("unlodctr.exe \"AllSrv\"", "Removing server performance counters"); }
        until ($true || &_vDieAndLogReason("Removing server performance counters"));

        do { $rc = &_fExecuteCmd ("unlodctr.exe \"AllSrvClients\"", "Removing client performance counters"); }
        until ($true || &_vDieAndLogReason("Removing client performance counters"));

        #
        # Need to be in the config directory to load the counters.
        #
        &_vChDriveDir("$sConfigDir");

        do { $rc = &_fExecuteCmd ("regini.exe fedsrv.txt", "Initializing server registry settings"); }
        until ($true || &_vDieAndLogReason("Initializing server registry settings"));

        do { $rc = &_fExecuteCmd ("regini.exe fedperf.txt", "Initializing server performance registry settings"); }
        until ($true || &_vDieAndLogReason("Initializing server performance registry settings"));

        do { $rc = &_fExecuteCmd ("lodctr.exe fedperfs.ini", "Adding server performance counters"); }
        until ($true || &_vDieAndLogReason("Adding server performance counters"));

        do { $rc = &_fExecuteCmd ("lodctr.exe fedperf.ini", "Adding client performance counters"); }
        until ($true || &_vDieAndLogReason("Adding client performance counters"));
    }

    #============================================================================
    # upgrade the database
    #
    if ($fDBUpdates && $sDBMachine ne '')
    {
        my $sSAPassword  = &_sGetSettingValueByName('Database', 'sapswd');

        print "\nReady to upgrade SQL database on [$sDBMachine] ";
        print "It is up to you to make sure that you are ready for this to happen.\n";
        print "If necessary, go make sure that all other modules using these\n";
        print "databases have been taken offline.\n";
        if (&_fAskYN ($true, "Proceed with database upgrade?"))
        {
            &_vUpgradeDatabase( $sDBMachine, $sDBName, "sa", $sSAPassword, "$sDBUpdDir" );
            &_vLog( "Database upgrades complete." );
        }
        else
        {
            &_vLog( "Database upgrades skipped." );
        }
    }

    #============================================================================
    # Install the new version of the server service.  Note that we try one
    # more time to remove the service, using the new version of the service.
    if ($fInstServ)
    {
        if (-e "$sDestDir\\fedsrv.exe")
        {
            # previous install. Removal can fail. That's ok.
            &_fExecuteCmd ("$sDestDir\\fedsrv.exe -remove", "Removing previous version of service...", "OK");
        }

        do { &_fExecuteCmd ("$sDestDir\\fedsrv.exe -install \\OBUser password", "Installing Allegiance Game Server service"); }
        until ((&_sGetServiceInstallPath("fedsrv") eq $sDestDir) || &_vDieAndLogReason("Installing Allegiance Game Server service"));
    }

    #============================================================================
    # start the new verion of the service
    #
    print "\nWe are now ready to restart the new version of the Allegiance Game Server.\n";
    print "It is up to you to make sure that you are ready for this to happen.\n";
    print "If necessary, go make sure that the SQL database upgrade has completed\n";
    print "before continuing.\n";
    if (&_fAskYN($true, "Start Allegiance Game Server?"))
    {
        while (! (&_fStartService( "fedsrv", "fedsrv.exe" ) || &_vDieAndLogReason("Start Allegiance Game Server service")) ) {}
    }
    else
    {
        &_vLog("Allegiance Game Server NOT started, as you requested.");
    }

    #============================================================================
    # All done!
    #
    &_vLog ("Installation complete!");
}

#============================================================================
# Sentinal SERVICE
#============================================================================

sub _vInstallSentinal
{
    &_vLogSilent ("Installing Sentinal Service Component.");

    my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
    my $fSentinal  = (&_sGetOptionValueByName('Sentinal',   'dofile') eq 'X');
    my $sDestDir   = &_sGetSettingValueByName('Sentinal',   'fildir');

    #============================================================================
    # copy files from the source dir to the new dir
    #
    if ($fSentinal)
    {
        my $rc = $false;

        &_vLog ("Copying Sentinal service files...");
        do {  &_vNewDir ("$sDestDir.new", $true, $true);
        } until (-d "$sDestDir.new" || &_vDieAndLogReason ("create $sDestDir.new"));
        do { $rc = &_fExecuteCmd ("$system\\xcopy.exe $sSourceDir\\sentinal\\*.* $sDestDir.new");
        } until ($rc || &_vDieAndLogReason("copying '$sSourceDir\\sentinal\\*.*' to '$sDestDir.new'."));
    }

    #============================================================================
    # If we're updating the binaries or updating the database, then we need to 
    # first stop the service.
    #
    if ($fSentinal)
    {
        my $fRunning = _sGetProcessIdByServiceName("Sentinal");
        if ($fRunning ne '')
        {
            &_vLog("Sentinal currently running (pid=$fRunning).");

            if (&_fAskYN ($true, "Stop Allegiance Game Server Monitoring Service (Sentinal)?"))
            {
                &_fStopService("Sentinal", "Sentinal.exe", 10);
            }
        }
        else
        {
            &_vLog("Sentinal service not running.");
        }
    }

    #============================================================================
    # If we're updating the binaries, then we need to uninstall the currently
    # installed service in preparation for the switch.
    #
    if ($fSentinal)
    {
        my $sCurPath = &_sGetServiceInstallPath( "Sentinal" );
        if ($sCurPath ne "")
        {
            &_vLog("Sentinal currently installed at '$sCurPath'.");

            if (-e "$sCurPath\\Sentinal.exe")
            {
                # previous install. Removal can fail. That's ok.
                &_fExecuteCmd ("$sCurPath\\Sentinal.exe -remove", "Removing previous version of Sentinal service...", "OK");
            }
            else
            {
                &_vLog( "*** Sentinal service is installed, but the executable does not");
                &_vLog( "*** exist.  Unable to uninstall service.");
            }

            if ($sCurPath ne $sDestDir)
            {
                &_vLog( "*** Since the Sentinal service is being installed to a different");
                &_vLog( "*** path than before, you will need to remove the old files manually.");
            }
        }
    }

    #============================================================================
    # In the words of Michael Jackson, "Make that change."
    #
    if ($fSentinal)
    {
        &_sSwitchDir ($sDestDir);
        do { &_fExecuteCmd ("$sDestDir\\Sentinal.exe -install", "Installing service"); }
        until ((&_sGetServiceInstallPath("Sentinal") eq $sDestDir) || &_vDieAndLogReason("Installing Sentinal service"));
    }

    #============================================================================
    # setup registry for this service
    #
    if ($fSentinal)
    {
		my $rc = false;

        do { $rc = &_fExecuteCmd ("$system\\regini.exe $sDestDir\\sentinal.ini", "Initializing Sentinal registry settings"); }
        until ($true || &_vDieAndLogReason("Initializing Sentinal registry settings"));
    }

    #============================================================================
    # start the new verion of the service
    #
    if ($fSentinal && $fStartSvc)
    {
        if (&_fAskYN($true, "Attempt to start Sentinal Service?"))
        {
            while (! (&_fStartService( "Sentinal", "Sentinal.exe" ) || &_vDieAndLogReason("Start Sentinal service")) ) {}
        }
        else
        {
            &_vLog("Sentinal service startup not attempted, as you requested.");
        }
    }

    #============================================================================
    # All done!
    #
    &_vLog ("Sentinal installation complete!\n");
}

# ===========================================================================
# This subroutine simply displays that heading lines at the top of the screen.
sub _vPrintHeading
{
    # system ("cls");
    # print "$0\n";
    print '-' x 72;
    print "\nMicrosoft Research - Allegiance Setup Script\n";
    print "Copyright (c) 1998 Microsoft Corporation, All Rights Reserved.\n\n";
}

# ===========================================================================
# This function copies the files in the source directory to the destination
# directory specified, renaming the files that already exist first.  This
# mechanism should work even if the file that exists is in-use by the system.
#
sub _vSafeCopy
{
    local($sSource, $sDest) = @_;

    my $fOpened = 0;
    my $iIndex = 0;

    do {
        $fOpened = opendir THISDIR, "$sSource";
    } until ( $fOpened || &_vDieAndLogReason ("delete old shared file '$sDest\\$sFile.old'"));

    if ($fOpened)
    {
        @allfiles = grep !/^\.\.?$/, readdir THISDIR;
        closedir THISDIR;

        foreach $sFile (@allfiles)
        {
            &_vLogSilent( "Safe-copying '$sFile' from '$sSource' to '$sDest'." );
        
            #
            # This code has been disabled.  The main reason is
            # that someone could be running perfmon from a remote
            # location and as a result we'll never be able to
            # delete it.
            #

            #
            # The current behavior is to try to delete it and if
            # it doesn't work, we rename it to something that
            # definately does not exist.
            #
            if (-e "$sDest\\$sFile.old")
            {
                &_vLogSilent( "  - .old file exists, deleting." );
                &_fExecuteCmd( "erase $sDest\\$sFile.old", "deleting old file", "deleting old file" );
            }

            if (-e "$sDest\\$sFile.old")
            {
                #
                # We failed to delete the file.  Rename it something
                # that doesn't already exist.
                #
                &_vLogSilent( "  - .old file being moved into temp." );
                while (-e "$ENV{'temp'}\\$sFile$iIndex.old" )
                {
                    $iIndex = $iIndex + 1;
                }

                &_fExecuteCmd( "move $sDest\\$sFile.old $ENV{'temp'}\\$sFile$iIndex.old", "moving old file", "moving old file" );
            }

            if (-e "$sDest\\$sFile.old")
            {
                &_vDieAndLogReason("moving old file $sDest\\$sFile.old")
            }


            do {            
                if (-e "$sDest\\$sFile")
                {
                    &_vLogSilent( "  - file exists, renaming." );
                    &_fExecuteCmd( "rename $sDest\\$sFile $sFile.old", "renaming existing file", "renaming existing file" );
                }
            } until ( (!-e "$sDest\\$sFile") || &_vDieAndLogReason ("rename existing shared file '$sDest\\$sFile'"));

            do {
                &_fExecuteCmd( "copy $sSource\\$sFile $sDest", "copying file $sFile to system dir", "copying file $sFile to system dir" );
            } until ( (-e "$sDest\\$sFile" && ((-s $sFile1) eq (-s $sFile2)) && ((-M $sFile1) eq (-M $sFile2))) || &_vDieAndLogReason ("verifying shared file after safe copy '$sDest\\$sFile'"));
        }
    }
}

#
# ===========================================================================
# This function overrides execution death and logs a fatal error message.
#
sub _OverrideDeath
{
    local($message) = @_;

    &_vLog ("FATAL: $message");
    die;
    return($true);
}

# ===========================================================================
# A front end for the system function call prints status and returns true on
# success and false on failure.
#
sub _fExecuteCmd
{
    local($cmd, $progmsg, $errmsg) = @_;

    if ($progmsg ne "")
    {
        &_vLog ("$progmsg...");
    }

    &_vLogSilent ("[$cmd]");

    open(SAVEOUT, ">&STDOUT");
    open(SAVEERR, ">&STDERR");

    open(STDOUT, ">$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't redirect stdout");
    open(STDERR, ">&STDOUT") || &_OverrideDeath("Can't dup stdout");

    select(STDERR); $| = 1;         # make unbuffered
    select(STDOUT); $| = 1;         # make unbuffered

    $rv = system ($cmd);

    close(STDOUT);
    close(STDERR);
    open(STDOUT, ">&SAVEOUT");
    open(STDERR, ">&SAVEERR");

    open (FILE, "$ENV{'TEMP'}\\~LastExecute.log");
    while (<FILE>)
    {
        chop;
        &_vLogSilent ($_);
    }
    close (FILE);

    return $rv;
}

# ===========================================================================
# ensure that the specified directory exists and is empty, and make it the
# current directory.
#
sub _vNewDir
{
    local($dirname, $fdelete, $change) = @_;

    &_vLogSilent ("Creating directory $dirname");

    # first, delete the existing directory, if necessary
    if (-d $dirname)
    {
        &_vLog("Directory $dirname already exists.");
    }
    else
    {
        &_vLog("Directory $dirname does not yet exist.");
    }

    if ($fdelete && (-d $dirname))
    {
        &_vLog ("Emptying existing directory first");
        my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
        &_fExecuteCmd ("delnode.exe /q $dirname", "", "deleting $dirname");
        if (-d $dirname)
        {
            &_vLog ("Warning: Directory $dirname delnoded, but still exists");
        }
    }

    # create the directory
    do
    {
        mkdir ($dirname, 0777);
        if (-d $dirname)
        {
            &_vLog("Called mkdir and now $dirname exists.");
        }
        else
        {
            &_vLog("Called mkdir, but $dirname still doesn't exist.");
        }
    } until (-d $dirname || &_vDieAndLogReason ("creating dir $dirname"));

    # change to the directory, if necessary
    if ($change)
    {
        $_ = $dirname;
        if (s/^([A-Z]):.*$/\1/i)
        {
            while (! (chdir ("$_:") || &_vDieAndLogReason ("CD'ing to drive $_:")) ) {}
        }
        while (! (chdir ($dirname) || &_vDieAndLogReason ("CD'ing to $dirname")) ) {}
    }
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
# Executes extract, stripping off the standard file extraction message
#
sub _fExtract
{
    local($sSourceFile, $sDestDir) = @_;
    my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
    my $sTempDir   = $ENV{'TEMP'};

    if(! -e $sDestDir)
    {
        &_vLog( "Directory '$sDestDir' does not exist.  Cound not extract '$sSourceFile'.");
        return $false;
    }

    &_fExecuteCmd ("extract.exe /A /Y /E $sSourceFile /L $sDestDir >$sTempDir\\~LastExtract.log 2>&1", "", $errmsg);

    while (! (open (BUILD, "$sTempDir\\~LastExtract.log") || &_vDieAndLogReason ("opening extract log file")) ) {}
    while (<BUILD>)
    {
        chop;
        if (!/^Extracting/)
        {
            &_vLog (">$_");
        }
    }
    close (BUILD);
    unlink "$sTempDir\\~LastExtract.log";
}

# ===========================================================================
# Stop a running service, ignoring errors that are ok (like not running) and
# available only from the output of the command.
#
sub _fStopService
{
    local($svcname, $exename, $wait) = @_;
	my $i=0;
	my $j=0;
	my $sLogmsg = "";
    $logname = "$ENV{'TEMP'}\\~LastStopSvc.log";

    if (_sGetProcessIdByServiceName($exename) ne '')
    {
        &_vLog ("Stopping $svcname service...");
        system ("net stop $svcname >$logname 2>&1");
        open (STOP, "<$logname");
        while (<STOP>)
        {
            chop;

            if (   !/^$/
                && !/error 1060/
                && !/does not exist as an installed service/
                && !/service is not started/
                && !/More help is available/
                && !/service was stopped successfully/
                && !/service is stopping/
                )
            {
                &_vLog ("Unexpected Message: $_");
            }
            &_vLogSilent ($_);
        }
        close (STOP);
    }
    else
    {
        &_vLog("Service $svcname not running.");
    }

    $sLogmsg = sprintf "...waiting for service to stop (up to %d seconds).", $wait*10
	&_vLog ($sLogmsg);
    for ($i=0; $i lt $wait && _sGetProcessIdByServiceName($exename) ne ''; $i++)
    {
        sleep(10);
    }
    if (_sGetProcessIdByServiceName($exename) ne '')
    {
        my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
        &_vLog ("waited long enough...killing service.");
        &_fExecuteCmd ("$sSourceName\\kill.exe $exename", "", "OK");
    }

	$j = _sGetProcessIdByServiceName("msdev.exe");
	for ($i=0; $i lt 10 && $j ne ''; $i++)
	{
		$sLogmsg = sprintf "...waiting for DevStudio (pid=%s) to stop (up to %d seconds).", $j, $wait*10
		&_vLog ($sLogmsg);
		&_fExecuteCmd ("$sSourceName\\kill.exe $j", "", "OK");
	}

    return 1;
}

# ===========================================================================
# Starts a service, ignoring errors that are ok (like already running) and
# available only from the output of the command.
#
sub _fStartService
{
    local($svcname, $exename) = @_;
    $logname = "$ENV{'TEMP'}\\~LastStartSvc.log";

    my $fRC = $true;

    if (_sGetProcessIdByServiceName($exename) eq '')
    {
        &_vLog ("Starting $svcname service...");
        system ("net start $svcname >$logname 2>&1");
        open (STOP, "<$logname");
        while (<STOP>)
        {
            chop;

            if (   /^$/
                || /service is starting/
                || /service was started successfully/
               )
            {
            }
            elsif (/error/
                || /does not exist as an installed service/
                || /service is not started/
                || /More help is available/
                || /service is stopping/
                  )
            {
                $fRC = $false;
            }
            else
            {
                &_vLog ("Unexpected Message: $_");
            }
            &_vLogSilent ($_);
        }
        close (STOP);
    }
    else
    {
        &_vLog("Service $svcname is already running.");
    }

    return $fRC;
}

# ===========================================================================
# If not pausing, always returns true. Else asks the user a yes/no question
# and returns the result
#
sub _fAskYN
{
    local($fAsk, $question) = @_;

    if (&_cAskSet( $fAsk, $question, "yn", "y" ) eq 'y')
    {
        return 1;
    }
    else
    {
        return 0;
    }
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
			if ($g_fQuiet)
			{
				$fproceed = $default;
			}
			else
			{
				print "$question ($set): ";
				$fproceed = &_cQueryForCharFromSet($set);
			}
        }
    }
    &_vLogSilent ("$question = $fproceed");

    # print "Answer is '$fproceed'.\n";
    $fproceed;
}

# ===========================================================================
# Place information in log file, and echo to screen.
#
sub _vLog
{
    local ($sMessage) = @_;

    if ($fLogOpen || $fLogAnyway)
    {
        ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime;
        $sLogDate = sprintf "%02d/%02d/%02d %02d:%02d:%02d", $mon+1, $mday, $year, $hour, $min, $sec;

        my $sContinued = '';
        while ($sMessage ne '')
        {
            $sLogString = ">$sLogDate $sContinued$sMessage";
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
    elsif ($fLogOpen)
    {
        ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime;
        $logdate = sprintf "%02d/%02d/%02d %02d:%02d:%02d", $mon+1, $mday, $year, $hour, $min, $sec;
        if (defined(PROPLOG))
        {
           printf PROPLOG ">$logdate @_\n";
        }
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
        &_OverrideDeath($reason)
    }
    else
    {
        return ($resolution eq "i");
    }
}

# ===========================================================================
# Copy across new system files.  This would be trivial except for the fact
# that the file might be in use.  So what we need to do is check if the file
# has changed.  If not, cool.  If it has, we try to copy the file over the
# one there.  If it works, cool.  If not, we have to erase <filename>.old, 
# rename <filename> to <filename>.old, copy the file to <filename> and then
# make it real clear to the installer that they may need to reboot.
#
sub _vUpdateSystemFiles
{
    local($source, $system) = @_;

    # eliminate any network sonnections that could be open.
    if (open (NETFILES, "net file|"))
    {
        @outline = <NETFILES>;
        close (NETFILES);
        foreach $outline (@outline)
        {
            chop ($outline);
            $outline =~ s/\D*(\d*).*/\1/;
            if ($outline ne "")
            {
                &_fExecuteCmd ("net file $outline /close", "closing net file connection", "closing net file connection: $outline");
            }
        }
    }

    # not used anymore--passed in as a parameter
    # my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');

    # used to do it this way, but it's prone to "can't copy over file that's in-use" errors
    # &_fExecuteCmd ("$system\\xcopy.exe $source $system", "copying system files", "OK");

    &_vSafeCopy( $source, $system );
}

# ===========================================================================
# Activate new directory, renaming old.
#
sub _sCopyDir
{
    local($src, $dest) = @_;

    &_vLog ("Attempting to copy from $src to $dest");

    if (!-e "$dest")
    {
        mkdir ($dest, 0777);
        if (!-d $dest)
        {
            &_vDieAndLogReason("creating dir $dest");
        }
    }

    &_fExecuteCmd ("copy $src\\\* $dest", "copying files", "copying files to: $dest");
}


sub _sSwitchDir
{
    local($path) = @_;

    # make sure we're not in the directory tree
    &_vLog ("Switching $path directory");
    if ($path =~ /^[a-zA-Z]:/)
    {
        $drive = $path;
        $drive =~ s/^([a-zA-Z]:).*/\1/;
        $path =~ s/^[a-zA-Z]:(.*)/\1/;
        while (! (chdir ($drive) || &_vDieAndLogReason ("change to drive $drive")) ) {}
    }
    chdir ("\\");

    #
    # delete the old backup dir
    #
    if (-e "$drive$path.old")
    {
        my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
        do { &_fExecuteCmd ("delnode.exe /q $drive$path.old", "deleting $drive$path.old", "deleting $drive$path.old");
        } until ( (! -d "$drive$path.old") || &_vDieAndLogReason ("delnode $drive$path.old"));
    }

    #
    # switch directory names
    #
    if (-e "$drive$path")
    {
        if (! (rename ("$drive$path", "$drive$path.old")))
        {
            &_vLog ("Failed to rename $drive$path to $drive$path.old");
            &_sCopyDir("$drive$path", "$drive$path.old");
        }
    }

    if (! (rename ("$drive$path.new", "$drive$path")))
    {
        &_vLog ("Failed to rename $drive$path.new to $drive$path");
        &_sCopyDir("$drive$path.new", "$drive$path");
    }

    return "$drive$path";
}

# ===========================================================================
# Change current drive and directory.
#
sub _vChDriveDir
{
    local($arg) = @_;
    $dir = $arg;
    if ($dir =~ /^[a-zA-Z]:/)
    {
        $drive = $arg;
        $drive =~ s/^([a-zA-Z]:).*/\1/;
        $dir =~ s/^[a-zA-Z]:(.*)/\1/;
        while (! (chdir ($drive) || &_vDieAndLogReason ("change to $drive:")) ) {}
    }
    while (! (chdir ($dir) || &_vDieAndLogReason ("change to $dir")) ) {}
}


# ===========================================================================
# Check for directory present and non-empty.
#
sub _fHaveDirData
{
    local($dir) = @_;
    if (-e $dir)
    {
        while (! (opendir (DIR, $dir) || &_vDieAndLogReason ("open $dir")) ) {}
        @files = readdir (DIR);
        closedir (DIR);
        foreach $file (@files)
        {
            if ($file ne "." && $file ne "..")
            {
                return $true;
            }
        }
    }
    $false;
}

# ===========================================================================
# Ask the user to re-enter the specified answer to the specified
# question.
#
sub _sQueryValue
{
    local($value, $title) = @_;

    print "\n$title=[$value]\nEnter the new value: ";
    my $sNewValue = <STDIN>;
    chop $sNewValue;
    if ($sNewValue eq '' && &_fAskYN($true, "No value entered.  Keep previous value?"))
    {
        return $value;
    }
        
    &_vLogSilent("Option is [$title], user changed value from [$value] to [$sNewValue].");

    return $sNewValue;
}

# ===========================================================================
# Open the log file used to output messages as the prop goes along.
#
sub _sOpenLog
{
    local($dir, $machine) = @_;

    # my $sLogname = &_sGetUniqueFilename("$dir\\~$machine", "log");
    my $sLogname = "$dir\\instserv.log";
    open (PROPLOG, ">$sLogname") || &_OverrideDeath("Could not open $sLogname.\n  Stopping");

    $fLogOpen = $true;

    return $sLogname;
}

# ===========================================================================
# Open the log file used to output messages as the prop goes along.
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
# Determines if the database specified exists on the specified machine.
#
sub _fDatabaseExists
{
    local ($sMachineName, $sUserId, $sPassword, $sDatabaseName) = @_;

    my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
    $cmd = "isql.exe /S $sMachineName /U $sUserId /P $sPassword /d master /n /Q \"select 'found', name from master..sysdatabases where name = '$sDatabaseName'\"";
    # $cmd = "isql.exe /S $sMachineName /U $sUserId /P $sPassword /d master /n /Q \"select 'found', name from $sDatabaseName..sysobjects where name = 'gbDatabaseVersion'\"";

    $fFound = $false;

    open(SAVEOUT, ">&STDOUT");
    open(SAVEERR, ">&STDERR");

    open(STDOUT, ">$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't redirect stdout");
    open(STDERR, ">&STDOUT") || &_OverrideDeath("Can't dup stdout");

    select(STDERR); $| = 1;         # make unbuffered
    select(STDOUT); $| = 1;         # make unbuffered

    &_vLogSilent ("[$cmd]");
    system ($cmd);

    close(STDOUT);
    close(STDERR);
    open(STDOUT, ">&SAVEOUT");
    open(STDERR, ">&SAVEERR");

    open(FILE, "$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeth("Can't open output from command");
    while (<FILE>)
    {
        if (/$sDatabaseName/i)
        # if (/\s+found\sgbDatabaseVersion/)
        {
            $fFound = $true;
	        &_vLogSilent ("Eureka!");
        }
        chop;
        &_vLogSilent ($_);
    }
    close(FILE);

    return $fFound;
}

# ===========================================================================
# Determines the physical location of the backup device for the specified
# database.  For this to work, the name of the backup device MUST be the name
# of the database with '_dump' added to the end.  What will be returned is
# the physical name of the folder to which the device refers for its dump and
# load files.
#
sub _sGetDatabaseBackupDevice
{
    local ($sMachineName, $sUserId, $sPassword, $sDatabaseName) = @_;

    my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
    $cmd = "isql.exe /S $sMachineName /U $sUserId /P $sPassword /d $sDatabaseName /n /Q \"select phyname from master..sysdevices where name = '${sDatabaseName}_dump'\"";

    my $sDevicePath = "";
    do
    {
        open(SAVEOUT, ">&STDOUT");
        open(SAVEERR, ">&STDERR");

        open(STDOUT, ">$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't redirect stdout");
        open(STDERR, ">&STDOUT") || &_OverrideDeath("Can't dup stdout");

        select(STDERR); $| = 1;         # make unbuffered
        select(STDOUT); $| = 1;         # make unbuffered

        &_vLogSilent ("[$cmd]");
        system ($cmd);

        close(STDOUT);
        close(STDERR);
        open(STDOUT, ">&SAVEOUT");
        open(STDERR, ">&SAVEERR");

        open(FILE, "$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't open output from command");
        while (<FILE>)
        {
            # we want the line that contains the path
            if (/(\w):(.*?)\s/)
            {
                if ($sMachineName eq $ENV{'COMPUTERNAME'})
                {
                    $sDevicePath = $1 . ":" . $2;
                }
                else
                {
                    $sDevicePath = "\\\\" . $sMachineName . "\\" . $1 . "\$" . $2;
                }
            }
            chop;
            &_vLogSilent ($_);
        }
        close(FILE);

    } until ( ($sDevicePath ne '') || &_vDieAndLogReason ("retrieving backup device location for '$sDatabaseName'"));

    return $sDevicePath;
}

#========================================================================
# This function is called to upgrade a database, from files that have
#
sub _sGetProcessIdByServiceName
{
    local ($sName) = @_;
    $sName = "\L$sName";

    open(SAVEOUT, ">&STDOUT");
    open(SAVEERR, ">&STDERR");

    open(STDOUT, ">$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't redirect stdout");
    open(STDERR, ">&STDOUT") || &_OverrideDeath("Can't dup stdout");

    select(STDERR); $| = 1;         # make unbuffered
    select(STDOUT); $| = 1;         # make unbuffered

    my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
    &_vLogSilent ("[$cmd]");
    system ("tlist.exe");

    close(STDOUT);
    close(STDERR);
    open(STDOUT, ">&SAVEOUT");
    open(STDERR, ">&SAVEERR");

    my $rc = '';
    open (FILE, "$ENV{'TEMP'}\\~LastExecute.log");
    while (<FILE>)
    {
        $sLine = "\L$_";
        chop;
        if ($sLine =~ / *([0-9]*) *$sName/)
        {
            if ($rc ne '')
            {
                $rc .= ',';
            }
            $rc .= $1;
        }
    }
    close (FILE);

    return $rc;
}

#========================================================================
# This function is called to upgrade a database, from files that have
# already been extracted and put in the right place.  For example, if
# the function is being called to upgrade the database, it
# expects the updatedb.cmd files to be in the "database" directory
# off of the $sDBDestDir directory.
#
sub _vUpgradeDatabase
{
    local ( $sDBMachine, $sDatabase, $sSAId, $sSAPassword, $sDBFiles ) = @_;

    if ($sDBMachine eq '' || $sDatabase eq '')
    {
        return;
    }

    &_vChDriveDir ("$sDBFiles");

	# add isql directory to path
    my $g_sOriginalPath = $ENV{'PATH'};
	my $sSQLToolsDir = &_sGetSettingValueByName('Components',  'srcdir');
	$ENV{'PATH'} = $g_sOriginalPath . ";" . $sSQLToolsDir;
		
    &_vLog( "Checking for existence of [$sDatabase] on [$sDBMachine]." );
    if (&_fDatabaseExists($sDBMachine, $sSAId, $sSAPassword, $sDatabase))
    {
        &_vLog( "Database '$sDatabase' found on '$sDBMachine'" );

        # see if we should call first time database setup routine
        # this is done by executing the following select statement on the target database
        # "select case count(*) when 0 then 'found' else 'not found' end from sysobjects where type='u'"
        if (&_fDatabaseSetupRequired( $sDBMachine, $sSAId, $sSAPassword, $sDatabase ))
        {
            my $sDBSetupFilename  = "recreate.bat";
            if (-e "$sDBSetupFilename")
            {
                &_vLog( "found db setup file [$sDBSetupFilename]" );
                &_fExecuteCmd( "$sDBSetupFilename /s $sDBMachine /d $sDatabase /p $sSAPassword", "initialize $sDatabase on $sDBMachine");
            }
            else
            {
                &_vLog( "First-time db setup file [$sDBSetupFilename] not found!" );
            }
        }
        else
        {
            my $sDBUpdateFilename = "recreate.bat";
            if (-e "$sDBUpdateFilename")
            {
                &_vLog( "found db update file [$sDBUpdateFilename]" );
                &_fExecuteCmd( "$sDBUpdateFilename /U /s $sDBMachine /d $sDatabase /p $sSAPassword", "updating $sDatabase on $sDBMachine");
            }
            else
            {
                &_vLog( "Database update file [$sDBSetupFilename] not found!" );
            }
        }
    }
    else
    {
        &_vLog( "Database '$sDatabase' not found on '$sDBMachine'!  Call for support." );
    }

    $ENV{'PATH'} = $g_sOriginalPath;
}

#========================================================================
# This function is called to determine if we need to run setupdb on the
# specified database or not.
sub _fDatabaseSetupRequired
{
    local ($sMachineName, $sUserId, $sPassword, $sDatabaseName) = @_;

    # send "select case count(*) when 0 then 'xxMISSINGxx' else 'xxFOUNDxx' end from sysobjects where type='u'"
    # and parse the output for 'xxFOUNDxx' or 'xxMISSINGxx' and return TRUE if 'found'
    my $sSourceDir = &_sGetSettingValueByName('Components', 'srcdir');
    $cmd = "isql.exe /S $sMachineName /U $sUserId /P $sPassword /d $sDatabaseName /n /Q \"select case count(*) when 0 then 'xxMISSINGxx' else 'xxFOUNDxx' end from sysobjects where type='u'\"";

    my $rc = $true;
    open(SAVEOUT, ">&STDOUT");
    open(SAVEERR, ">&STDERR");

    open(STDOUT, ">$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't redirect stdout");
    open(STDERR, ">&STDOUT") || &_OverrideDeath("Can't dup stdout");

    select(STDERR); $| = 1;         # make unbuffered
    select(STDOUT); $| = 1;         # make unbuffered

    &_vLogSilent ("[$cmd]");
    system ($cmd);

    close(STDOUT);
    close(STDERR);
    open(STDOUT, ">&SAVEOUT");
    open(STDERR, ">&SAVEERR");

    open(FILE, "$ENV{'TEMP'}\\~LastExecute.log") || &_OverrideDeath("Can't open output from command");
    while (<FILE>)
    {
        if (/xxFOUNDxx/)
        {
            &_vLogSilent("Eureka!!!");
            $rc = $false;
        }
        chop;
        &_vLogSilent ($_);
    }
    close(FILE);

    return $rc;
}

#========================================================================
# This function is given the name of an NT service, and attempts to 
# retrieve the path where that service is installed from the registry,
# as well as the name of the executable for that NT service.
sub _sGetServiceInstallPath
{
    local ($sServiceName) = @_;

    my $sRegKey = "System\\CurrentControlSet\\Services\\$sServiceName";
 
    if (&NTRegOpenKeyEx ($HKEY_LOCAL_MACHINE, $sRegKey, 0, $KEY_ALL_ACCESS, $key))
    {
        my $sRegValue = "";
        if (&NTRegQueryValueEx ($key, "ImagePath", 0, $type, $sRegValue))
        {
            $_ = $sRegValue;
            /(.+)\\/;
            my $sPath = $1;
            my $sExe  = $';
            
            return $sPath;
        }
        else
        {
            &_vLog("Unable to read $sRegKey\\ImagePath.");
        }
    }
    else
    {
        &_vLog("Unable to open $sRegKey.");
    }

    return "";
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
# This function retrieves data for each row of the settings table specified
# from the registry.
#
sub _vGetSettingValuesReg
{
    local ($sDlg) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];
        
        # get the registry value from HKLM\Software\Microsoft\Allegiance\Install\Server\$sDlg\$sTableName
        my $fRC = $false;
        my $sValue = "";
        ($fRC, $sValue) = &_fsGetRegValue( &HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Allegiance\\Install\\Server\\$sDlg", $sTableName );
        if ($fRC)
        {
            $$sTable[$i+3] = $sValue;
        }
    }
}

#========================================================================
# This function places data from each row of the settings table specified
# back into the registry for use next time the script is run.
#
sub _vSetSettingValuesReg
{
    local ($sDlg) = @_;

    local $sTable = "srSetting$sDlg";
    local $nItems = ($#$sTable)+1;
    for ($i = 0; $i < $nItems; $i+=4)
    {
        local $sTableName = $$sTable[$i+1];

        # set the registry value HKLM\Software\Microsoft\Allegiance\Install\Server\$sDlg\$sTableName
        # to what's in $$sTable[$i+3] 
        _fChangeRegValue (&HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Allegiance\\Install\\Server\\$sDlg", &REG_SZ, $sTableName, $$sTable[$i+3]);
    }
}

# ===========================================================================
# This function retrieves the specified value from the registry.  It returns
# a flag indicating success (TRUE) or failue (FALSE) and the value retrieved
# if the flag is TRUE, "" otherwise.
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
            # print "NTRegQueryValueEx failed '$sRegKey', '$sRegValueName'.\n";
        }
    }
    else
    {
        # print "NTRegOpenKeyEx failed '$sRegKey', '$sRegValueName'.\n";
    }

    return ($false, "");
}

# ===========================================================================
# This function is passed a registry hive identifier and a key-value name,
# the value name, plus a new value.  The reg key is set to the new value.
#
sub _fChangeRegValue 
{
    local ($sHive, $sKey, $sClass, $sValueName, $sNewValue) = @_;

    if (&NTRegCreateKeyEx ($sHive, $sKey, &NULL, "Allegiance", &REG_OPTION_NON_VOLATILE, &KEY_ALL_ACCESS, &NULL, $hkey, $disposition))
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

#========================================================================
# This section interacts with the installer to determine what components
# should be installed.
#
sub _vInitializeSetupOptions
{
    $fLogOpen    = $false;
    $g_fQuiet    = $false;
    $buildnum    = '';

    if (-e "version.dat")
    {
        # get the build number, presumably carried in the prop as version.dat
        open (SRCDIR, "version.dat") || &_OverrideDeath("Could not open version.dat");
        $buildnum = <SRCDIR>;
        close (SRCDIR);
        chop $buildnum;                        # strips newline char
        $buildnum =~ s/([0-9A-Za-z]*).*/\1/;   # we think this strips non-alphanum characters
    }

    #----------------------------------------------------------------------------------
    # a section like these below represents the data for one Install dialog.
    # It consists of a Description, a function to call to query for more advanced
    # options, and a status field to indicate what selection the user made.
    #----------------------------------------------------------------------------------

    # This section only appears for the Components dialog, and specifies what setup
    # functions to call for each component once all of the user preferences have been
    # collected.
    %srInstallMainFunction =
    ( # must specify the name of the install function
        '1', '_vInstallServer',
        '2', '_vInstallSentinal',
    );

    #----------------------------------------------------------------------------------
    # Dialog - Components Dialog
    @srSettingComponents =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        'a',  'srcdir',   'Source directory          ',           "x:\\$buildnum",
        'b',  'logdir',   'Log directory             ',           "w:\\",
    );
    @srOptionComponents =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        '1',  'dofile',   'Install Allegiance Server Files...',     'ServerFiles',
        '2',  'dodb',     'Update Allegiance Database...     ',     'Database',
    );

    #----------------------------------------------------------------------------------
    # Dialog - Database Options Dialog
    @srSettingDatabase =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        'a',  'sapswd',   'User sa password                 ',    '""',
        'b',  'dbudir',   'Database update files temp dir   ',    'C:\\database',
        'c',  'sqlbox',   'SQL machine name                 ',    $ENV{'COMPUTERNAME'},
        'd',  'dbname',   'SQL database name                ',    'federation',
    );
    @srOptionDatabase =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        '1',  'dbupdt',   'Allegiance Game Server database updates',       'X',
    );

    #----------------------------------------------------------------------------------
    # Dialog - ServerFiles Options Dialog
    @srSettingServerFiles =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        'a',  'dstdir',   'Destination directory     ',           'C:\\OblivServer',
        'b',  'shrdir',   'Shared files directory    ',           'C:\\OblivShared',
        'c',  'cfgdir',   'Config files directory    ',           'C:\\OblivConfig',
    );
    @srOptionServerFiles =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        '1',  'srvr',   'Allegiance Game Server service    ',       'X',
    );

    #----------------------------------------------------------------------------------
    # Dialog Template for Sentinal Options Dialog
    @srSettingSentinal =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        'a',  'fildir',   'Sentinal directory ',                  "c:\\Sentinal", # "$ENV{'windir'}\\System32",
    );
    @srOptionSentinal =
    ( # idx   --Name---   --Description-----------------------    --Value--------------
        '1',  'dofile',   'Sentinal Files',                       ' ',
    );

    # if the install has been run before, then all these values should exist in the 
    # registry.  go retrieve them now.
    
    &_vGetSettingValuesReg("Components");
    &_vGetSettingValuesReg("ServerFiles");
    &_vGetSettingValuesReg("Database");
    &_vGetSettingValuesReg("Sentinal");

}


