#Imago <imagotrigger@gmail.com>
# NSIS wrapper for allegsetup
#  2 modes - Upgrade (Beta) / Full (Production)
#   Supports multipler servers - don't forget to add your mirror to bitten

use strict;
use POSIX;
my ($ver, $build, $revision, $bfull) = @ARGV;

my $pdbkey ="";
my $artkey = "";
my $betavar = "";
my $filen = "Alleg";
my $shortname = "Alleg_lite";
my $artwork = "";
my $bbeta = "";
my $dlcode_art = ""; my $dlcode_pdb = "";
my $clientbinary = "ASGSClient.exe";
my @url = ("http://services.nirvanix.com/1-Planet/C70595-1/Shared", "http://build.alleg.net"); #Fuzz 07/18 - Use multiple servers!
my $retries = () = @url;	#Get number of elements in @url
my $cfgfile = "http://autoupdate.alleg.net/allegiance.cfg";

my $now = strftime("%Y/%m/%d %H:%M CDT",localtime(time));

my $asgsreg = qq{
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "EXE Path" "\$INSTDIR"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "EXE Path" "\$INSTDIR"
  };

if ($ver eq "1.1") {
	$filen = "AllegR6";
	$shortname = "R6";
	$bbeta = "R6";
	$asgsreg = "";
	$clientbinary = "Allegiance.exe";
	$cfgfile = "http://fazdev.alleg.net/FAZ/FAZbeta.cfg";
}

open(CMD,"C:\\md5sums.exe -up C:\\build\\Package\\Artwork.7z C:\\Inetpub\\wwwroot\\build\\Alleg${bbeta}PDB_b${build}_r$revision.exe |");
my @lines = <CMD>;
close CMD;
foreach my $line (@lines) {
	my ($md5,$file) = split(/\s/,$line);
	$artkey = $md5 if ($file =~ /Artwork/i);
	$pdbkey = $md5 if ($file =~ /PDB/i);
}

if ($bfull) {
	$shortname = "Alleg" ;
	$artwork = qq{
  File C:\\build\\Package\\Artwork.7z
  Nsis7z::ExtractWithCallback "Artwork.7z" \$R9
  GetFunctionAddress \$R9 CallbackTest
  Delete "\$OUTDIR\\Artwork.7z"
};
	$dlcode_art = qq{
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "CfgFile" "$cfgfile"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "CfgFile" "$cfgfile"
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "ArtPath" "\$INSTDIR\\Artwork"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "ArtPAth" "\$INSTDIR\\Artwork"	
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver\\Server" "ArtPath" "\$INSTDIR\\Artwork"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver\\Server" "ArtPAth" "\$INSTDIR\\Artwork"   
  $asgsreg
};
} else {
	# Fuzz 07/18 try multiple servers
	# Fuzz 07/26 make it so the user doesn't have to change this when they change the servers
	$betavar = "Var BetaSetupError";
	my $c = 0;
	my $pdbred = "";
	foreach $c(0..$retries-1){
		$pdbred .= "pdbred${c}:
	inetc::get /RESUME \$4 /CAPTION \$5 /POPUP \$6 \"@url[${c}]\$7\" \$8 /END
	goto pdbcmp\n";
	}
	my $pdbintcmp = "
	IntCmp 0 \$3 pdbred0";
	foreach $c(1..$retries-1){
		$pdbintcmp .= "
	Intcmp ${c} \$3 pdbred${c}";
	}
	$pdbintcmp .= " pdbreset pdbreset";
	$dlcode_pdb = qq{
	StrCpy \$4 "Problem connecting to server.  Please reconnect and click Retry to resume downloading or Cancel to try a different server"
	StrCpy \$5 "Program Database"
	StrCpy \$6 "Program database"
	StrCpy \$7 "/Alleg${bbeta}PDB_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.exe"
	StrCpy \$8 "\$INSTDIR\\PDB.7z"
	MessageBox MB_YESNO|MB_ICONQUESTION "Download program databases for debugging?\$\\nIf you don't know what this is, click No" /SD IDYES IDNO dontDL
pdbreset:
	IntFmt \$3 "%hu" 0
	LogEx::Write true true "Trying server \$3 for PDB..."
$pdbred
pdbcmp:
	Pop \$0
	IntOp \$3 \$3 + 1
	md5dll::GetMD5File \$8
	Pop \$1
	LogEx::Write true true "Download returned: \$0 md5: \$1"
	StrCmp \${PRODUCT_PDB_KEY} \$1 pdbmd5
	IntCmp $retries \$3 pdbfail
	LogEx::Write true true "Trying server \$3 for PDB..."
$pdbintcmp
pdbfail:
	LogEx::Write true true "Done retrying servers. Download of PDB failed..."
	goto dontDL
pdbmd5:
	LogEx::Write true true "Extracting PDB package..."
	Nsis7z::ExtractWithCallback "\$INSTDIR\\PDB.7z" \$R9
	GetFunctionAddress \$R9 CallbackTest
	Delete PDB.7z
dontDL:
};

########################  PDB
######################
########################  ART
	my $artred = "";
	foreach $c(0..$retries-1){
		$artred .= "artred${c}:
	inetc::get /RESUME \$4 /CAPTION \$5 /POPUP \$6 \"@url[${c}]\$7\" \$8 /END
	goto artcmp\n";
	}
	my $artintcmp = "
	IntCmp 0 \$3 artred0";
	foreach $c(1..$retries-1){
		$artintcmp .= "
	Intcmp ${c} \$3 artred${c}";
	}
	$artintcmp .= " artreset artreset";
	$dlcode_art = qq{
	StrCpy \$4 "Problem connecting to server.  Please reconnect and click Retry to resume downloading or Cancel to try a different server"
	StrCpy \$5 "Artwork"
	StrCpy \$6 "Artwork"
	StrCpy \$7 "/AllegR6ART_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.exe"
	StrCpy \$8 "\$INSTDIR\\ART.7z"
	
	ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
	StrCmp \$ARTPATH "" GotDLNoArt0
	goto GotDLArt
GotDLNoArt0:
	ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
	StrCmp \$ARTPATH "" GotDLNoArt1	
	goto GotDLArt
GotDLNoArt1:
	LogEx::Write true true "It appears you need all of the Artwork..."
	MessageBox MB_YESNO|MB_ICONQUESTION "Download complete Artwork?\$\\n Choose Yes unless you know what you're doing" /SD IDYES IDNO dontDL2
artreset:
	IntFmt \$3 "%hu" 0
	LogEx::Write true true "Trying server \$3 for ART..."
$artred
artcmp:
	Pop \$0
	IntOp \$3 \$3 + 1
	md5dll::GetMD5File \$8
	Pop \$1
	LogEx::Write true true "Download returned: \$0 md5: \$1"
	StrCmp \${PRODUCT_ART_KEY} \$1 artmd5
	IntCmp $retries \$3 artfail
	LogEx::Write true true "Trying server \$3 for ART..."
$artintcmp
artfail:
	LogEx::Write true true "Done retrying servers. Download of ART failed..."
	goto dontDL2
artmd5:
	LogEx::Write true true "Extracting ART package..."
	Nsis7z::ExtractWithCallback "\$INSTDIR\\ART.7z" \$R9
	GetFunctionAddress \$R9 CallbackTest
	Delete ART.7z
	WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "CfgFile" "http://fazdev.alleg.net/FAZ/FAZR6.cfg"
	WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "CfgFile" "http://fazdev.alleg.net/FAZ/FAZR6.cfg"
	WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "ArtPath" "\$INSTDIR\\Artwork"
	WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "ArtPAth" "\$INSTDIR\\Artwork"
	WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver\\Server" "ArtPath" "\$INSTDIR\\Artwork"
	WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver\\Server" "ArtPAth" "\$INSTDIR\\Artwork"  

	$asgsreg
	goto DL
dontDL2:
	File "C:\\betareghlpV4.exe"
tryagain:
	LogEx::Write true true "Browse for Folder - Set 1.1 ArtPath"
	ExecWait '"betareghlpV4.exe"' \$BetaSetupError
	ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
	StrCmp \$ARTPATH "" nextone
	goto DL
nextone:
	ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
	StrCmp \$ARTPATH "" tryagain
	LogEx::Write true true "Using \$ARTPATH"
DL:
	Delete "betareghlpV4.exe"
goto EndARTDL
GotDLArt:

IfFileExists "\$ARTPATH\\*.*" DLDirCheckOK
goto GotDLNoArt1


DLDirCheckOK:
; **************************************************************************************
; **************************************************************************************

\${Array} "FileList" 1024 32
\${ArrayFunc} Write
\${ArrayFunc} ReadFirst
\${ArrayFunc} ReadNext
\${ArrayFunc} ReadClose

 \${Array} "UrlList0" 1024 384
\${ArrayFunc} Write
\${ArrayFunc} Read
 
 \${Array} "UrlList1" 1024 384
\${ArrayFunc} Write
\${ArrayFunc} Read

 \${FileList->Init}
 \${UrlList0->Init}
 \${UrlList1->Init}
; Here is where we patch artwork:

; 1) download the filelist - this is always in sync with the beta list but it varies in format slightly

LogEx::Write true true "Downloading file list..."
NsisUrlLib::UrlOpen /NOUNLOAD "http://build.alleg.net:8080"
Pop \$0
\${StrStr} \$R4 \$0 "Succeeded"
LogEx::Write true true "Url open \$0..."
\${If} \$R4 != "Succeeded"
	MessageBox MB_ICONEXCLAMATION|MB_OK "\$0 - Unable to contact build server.  Skipping installer's auto updates!  If the error persists, check http://freeallegiance.org for details."
	goto EndARTDL
\${Endif}

; 2) CRC everything on the list (the installer could be out of date by the time they d/l it)

IntOp \$R9 0 + 0
LogEx::Write true true "Analyzing local files, this may take a moment..."
SetDetailsPrint textonly
\${Do}
    Call GetNextFile
    Pop \$1
    StrCpy \$R8 \$1
    StrCmp \$1 "" skip
    \${StrStr} \$0 \$1 "|"
    \${If} \$0 == ""
	MessageBox MB_ICONEXCLAMATION|MB_OK "Error \$0 - Unable to contact build server.  Skipping installer's auto updates!  If the error persists, check http://freeallegiance.org for details."
	goto EndARTDL    
    \${Endif}

     Push 2
     Push \$R8
     Call SplitFile
     Pop \$3   
     StrCpy \$myArtCRC \$3

     Push 1
     Push \$R8
     Call SplitFile
     Pop \$3
  
     StrCpy \$myArtName \$3
     StrCpy \$myArt7z \$3
     StrCpy \$myArtLocal \$3
     
     \${StrTrimNewLines} \$4 \$myArtName
     StrCpy \$myArtName \$4
     
   \${StrStrip} ".zip" "\$myArtName" "\$myArt7z"

; UGLY HACK ALERT - everything not compressed (7zip LZMA) is .gz - 
;   this is to get a Content-Type HTTP header out of CloudNAS (ex. .mdl is not defined)
   \${StrStrip} ".gz" "\$myArt7z" "\$myArt7z"
   \${StrStrip} ".gz" "\$myArtName" "\$myArtLocal"

   ; Non-artwork files kept in sync /w src /clintlib/AutoDownload.h
   StrCmp \$myArt7z "Allegiance.exe" DozDir
   StrCmp \$myArt7z "Reloader.exe" DozDir
   StrCmp \$myArt7z "DbgHelp.dll" DozDir
   StrCmp \$myArt7z "AllSrvUI.exe" DozDir
   StrCmp \$myArt7z "AllSrv.exe" DozDir
   StrCmp \$myArt7z "AGC.dll" DozDir
   StrCmp \$myArt7z "AutoUpdate.exe" DozDir
   goto SkipDozDir
   ;    
   DozDir:
   	md5dll::GetMD5File "\$INSTDIR\\\$myArt7z"
	Pop \$1       
     StrCmp \$1 \$myArtCRC skip   
   goto DozPush
   SkipDozDir:
   	md5dll::GetMD5File "\$ARTPATH\\\$myArt7z"
	Pop \$1            
     StrCmp \$1 \$myArtCRC skip  
   DozPush:
   
; 3) Add mismatches to an inetc url array for download

     LogEx::Write false false "Queuing \$myArtName (\$myArtCRC) had \$myArtLocal (\$1)"     

     \${FileList->Write} \$R9 \$myArtName
	 \${UrlList0->Write} \$R9 "\$ARTPATH\\\$myArtLocal"
	 \${UrlList1->Write} \$R9 "http://services.nirvanix.com/1-Planet/C70595-1/Update/\$myArtName"
     IntOp \$R9 \$R9 + 1

skip:
StrLen \$2 \$R8
\${LoopUntil} \$2 = 0

SetDetailsPrint both

; rebuild the stack for inetc
${stack::ns_clear}
Push /END
IntOp \$R8 0 + 0
ClearErrors
 \${FileList->ReadFirst} \$R1 \$R2
 Loop3:
 IfErrors Done3
  \${UrlList0->Read} \$R3 \$R8
  \${UrlList1->Read} \$R4 \$R8
  Push \$R3
  Push \$R4
   IntOp \$R8 \$R8 + 1
  ClearErrors
  \${FileList->ReadNext} \$R1 \$R2
 Goto Loop3
 Done3:
 \${FileList->ReadClose} \$R1
 
LogEx::Write true true "Downloading \$R9 files...!"
\${If} \$R9 <> 0
	MessageBox MB_OK|MB_ICONINFORMATION "Allegiance Update has determined some of your files are out of date.\$\\nIf you've customized any of your Artwork, this is your chance to make a backup.  Click OK to begin the downloads that you need."
	inetc::get /RESUME "Click Retry to resume the download of the Auto update. - If the error persists, check your internet connection and try again." /CAPTION "Allegiance Update"
	Pop \$1
	\${If} \$1 != "OK"
		\${If} \$1 == "Cancelled"
		  MessageBox MB_ICONEXCLAMATION|MB_OK "You have cancelled the download of files Allegiance Update determined you needed.  Allegiance was not installed correctly!"
		  LogEx::Write true true "Downloads cancelled!"
		\${Else}
	      MessageBox MB_ICONEXCLAMATION|MB_OK "Error \$1 - Please try running the installer again in a few moments, If the error persists, check http://freeallegiance.org for details."
		  LogEx::Write true true "Downloads error \$1!"
	   \${Endif}
	\${Endif}
\${Endif}

; 4) Downloads pull from Cloud - TODO use multiple servers here!

LogEx::Write true true "Downloads complete!"

; 5) Utilize 7z for uncompressing certain files indicated in our file list

StrCpy \$switch_overwrite 1
SetOutPath \$ARTPATH

 ClearErrors
 \${FileList->ReadFirst} \$R1 \$R2
 Loop2:
 IfErrors Done2
   \${StrStr} \$0 \$R2 ".zip"
   StrCmp \$0 ".zip" DoExtract
   goto donext
   DoExtract:
    LogEx::Write false false "Extracting \$R2..."
    Nsis7z::ExtractWithCallback "\$ARTPATH\\\$R2" \$R9
    GetFunctionAddress \$R9 CallbackTest
    Delete "\$ARTPATH\\\$R2"
   donext:
   
   ; Non-artwork files kept in sync /w src /clintlib/AutoDownload.h
   StrCmp \$R2 "Allegiance.exe.zip" DozMove
   StrCmp \$R2 "Reloader.exe.zip" DozMove
   StrCmp \$R2 "DbgHelp.dll.zip" DozMove
   StrCmp \$R2 "AllSrvUI.exe.zip" DozMove
   StrCmp \$R2 "AllSrv.exe.zip" DozMove
   StrCmp \$R2 "AGC.dll.zip" DozMove
   StrCmp \$R2 "AutoUpdate.exe.zip" DozMove
   ; 
   goto donext2
   DozMove:
   \${StrStrip} ".zip" "\$R2" "\$1"
   !insertmacro MoveFile "\$ARTPATH\\\$1" "\$INSTDIR\\\$1"
   LogEx::Write false false "Moved \$ARTPATH\\\$1 to \$INSTDIR\\\$1"
  donext2:
  ClearErrors
  \${FileList->ReadNext} \$R1 \$R2
 Goto Loop2
 Done2:
 \${FileList->ReadClose} \$R1

\${FileList->Delete}
\${UrlList0->Delete}
\${UrlList1->Delete}

SetOutPath \$INSTDIR
; **************************************************************************************
; **************************************************************************************
EndARTDL:
};
}

#######################
####################### Begin NSIS
#######################


my $nsis = <<END_NSIS;
SetCompressor lzma
!define PRODUCT_NAME "Allegiance"
!define PRODUCT_VERSION "$ver"
!define PRODUCT_PUBLISHER "Free Allegiance"
!define PRODUCT_WEB_SITE "http://freeallegiance.org"
!define PRODUCT_DIR_REGKEY "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Allegiance\\$ver"
!define PRODUCT_UNINST_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\\${PRODUCT_PUBLISHER}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:AllegStartMenuDir"

!define LOCALE_ILANGUAGE '0x1' ;System Language Resource ID     
!define LOCALE_SLANGUAGE '0x2' ;System Language & Country
!define LOCALE_SABBREVLANGNAME '0x3' ;System abbreviated language
!define LOCALE_SNATIVELANGNAME '0x4' ;System native language name
!define LOCALE_ICOUNTRY '0x5' ;System country code     
!define LOCALE_SCOUNTRY '0x6' ;System Country
!define LOCALE_SABBREVCTRYNAME '0x7' ;System abbreviated country name
!define LOCALE_SNATIVECTRYNAME '0x8' ;System native country name
!define LOCALE_IDEFAULTLANGUAGE '0x9' ;System default language ID
!define LOCALE_IDEFAULTCOUNTRY  '0xA' ;System default country code
!define LOCALE_IDEFAULTCODEPAGE '0xB' ;System default oem code page

!include LogicLib.nsh
!include NSISArray.nsh
!include "Stack.nsh"

Function StrStrip
Exch \$R0 #string
Exch
Exch \$R1 #in string
Push \$R2
Push \$R3
Push \$R4
Push \$R5
 StrLen \$R5 \$R0
 StrCpy \$R2 -1
 IntOp \$R2 \$R2 + 1
 StrCpy \$R3 \$R1 \$R5 \$R2
 StrCmp \$R3 "" +9
 StrCmp \$R3 \$R0 0 -3
  StrCpy \$R3 \$R1 \$R2
  IntOp \$R2 \$R2 + \$R5
  StrCpy \$R4 \$R1 "" \$R2
  StrCpy \$R1 \$R3\$R4
  IntOp \$R2 \$R2 - \$R5
  IntOp \$R2 \$R2 - 1
  Goto -10
  StrCpy \$R0 \$R1
Pop \$R5
Pop \$R4
Pop \$R3
Pop \$R2
Pop \$R1
Exch \$R0
FunctionEnd
!macro StrStrip Str InStr OutVar
 Push '\${InStr}'
 Push '\${Str}'
  Call StrStrip
 Pop '\${OutVar}'
!macroend
!define StrStrip '!insertmacro StrStrip'

!define StrStr "!insertmacro StrStr"
 
!macro StrStr ResultVar String SubString
  Push `\${String}`
  Push `\${SubString}`
  Call StrStr
  Pop `\${ResultVar}`
!macroend
 
Function StrStr
  Exch \$R0
  Exch
  Exch \$R1
  Push \$R2
  Push \$R3
  Push \$R4
  Push \$R5
 
  ;Get "String" and "SubString" length
  StrLen \$R2 \$R0
  StrLen \$R3 \$R1
  ;Start "StartCharPos" counter
  StrCpy \$R4 0
 
  ;Loop until "SubString" is found or "String" reaches its end
  \${Do}
    ;Remove everything before and after the searched part ("TempStr")
    StrCpy \$R5 \$R1 \$R2 \$R4
 
    ;Compare "TempStr" with "SubString"
    \${IfThen} \$R5 == \$R0 \${|} \${ExitDo} \${|}
    ;If not "SubString", this could be "String"'s end
    \${IfThen} \$R4 >= \$R3 \${|} \${ExitDo} \${|}
    ;If not, continue the loop
    IntOp \$R4 \$R4 + 1
  \${Loop}
 
  StrCpy \$R0 \$R1 `` \$R4
 
  Pop \$R5
  Pop \$R4
  Pop \$R3
  Pop \$R2
  Pop \$R1
  Exch \$R0
FunctionEnd
 
Function IsSilent
  Push \$0
  Push \$CMDLINE
  Push "/S"
  Call StrStr
  Pop \$0
  StrCpy \$1 \$0
  StrCpy \$0 \$0 3
  StrCmp \$0 "/S" silent
  StrCmp \$0 "/S " silent
    StrCpy \$0 0
    Goto notsilent
  silent: StrCpy \$0 1
  ;this is a special mode for the build server to setup a symbol path
	  StrCpy \$1 \$1 6
	  StrCmp \$1 "/S /D" dump
	  StrCmp \$1 "/S /D " dump  
	  Goto notsilent
	  dump: StrCpy \$0 2
  notsilent: Exch \$0
FunctionEnd


!define StrTrimNewLines "!insertmacro StrTrimNewLines"

!macro StrTrimNewLines ResultVar String
  Push "\${String}"
  Call StrTrimNewLines
  Pop "\${ResultVar}"
!macroend

Function StrTrimNewLines
/*After this point:
  ------------------------------------------
  \$R0 = String (input)
  \$R1 = TrimCounter (temp)
  \$R2 = Temp (temp)*/

  ;Get input from user
  Exch \$R0
  Push \$R1
  Push \$R2

  ;Initialize trim counter
  StrCpy \$R1 0

  loop:
  ;Subtract to get "String"'s last characters
  IntOp \$R1 \$R1 - 1

  ;Verify if they are either \$\\r or \$\\n
  StrCpy \$R2 \$R0 1 \$R1
  \${If} \$R2 == `\$\\r`
  \${OrIf} \$R2 == `\$\\n`
    Goto loop
  \${EndIf}

  ;Trim characters (if needed)
  IntOp \$R1 \$R1 + 1
  \${If} \$R1 < 0
    StrCpy \$R0 \$R0 \$R1
  \${EndIf}

/*After this point:
  ------------------------------------------
  \$R0 = ResultVar (output)*/

  ;Return output to user
  Pop \$R2
  Pop \$R1
  Exch \$R0
FunctionEnd

Function GetNextFile
     NsisUrlLib::IterateLine /NOUNLOAD
     Pop \$1
     Exch \$1
FunctionEnd

Function SplitFile
Exch \$R2
Exch
Exch \$R3
Push \$R0
Push \$R1


 StrCpy \$R0 0
 StrCmp \$R3 1 getpart1_loop_\$R3
 StrCmp \$R3 2 getpart2_top_\$R3
Goto error_\$R3

getpart1_loop_\$R3:
 IntOp \$R0 \$R0 - 1
 StrCpy \$R1 \$R2 1 \$R0
  StrCmp \$R1 "" error_\$R3
  StrCmp \$R1 "|" 0 getpart1_loop_\$R3

 IntOp \$R0 \$R0 + 1
 StrCpy \$R0 \$R2 "" \$R0
Goto done_\$R3

getpart2_top_\$R3:
 StrLen \$R0 \$R2
getpart2_loop_\$R3:
 IntOp \$R0 \$R0 - 1
 StrCpy \$R1 \$R2 1 -\$R0
  StrCmp \$R1 "" error_\$R3
  StrCmp \$R1 "|" 0 getpart2_loop_\$R3

 StrCpy \$R0 \$R2 -\$R0
Goto done_\$R3

error_\$R3:
 StrCpy \$R0 error

done_\$R3:

Pop \$R1
Pop \$R3
Exch \$R0
FunctionEnd


!include 'FileFunc.nsh'
!insertmacro Locate
 
Var /GLOBAL switch_overwrite

!macro MoveFile sourceFile destinationFile

!define MOVEFILE_JUMP \${__LINE__}

; Check source actually exists

    IfFileExists "\${sourceFile}" +3 0
    SetErrors
    goto done_\${MOVEFILE_JUMP}

; Add message to details-view/install-log

    DetailPrint "Moving/renaming file: \${sourceFile} to \${destinationFile}"

; If destination does not already exists simply move file

    IfFileExists "\${destinationFile}" +3 0
    rename "\${sourceFile}" "\${destinationFile}"
    goto done_\${MOVEFILE_JUMP}

; If overwriting without 'ifnewer' check

    \${If} \$switch_overwrite == 1
	delete "\${destinationFile}"
	rename "\${sourceFile}" "\${destinationFile}"
	delete "\${sourceFile}"
	goto done_\${MOVEFILE_JUMP}
    \${EndIf}

; If destination already exists

    Push \$R0
    Push \$R1
    Push \$R2
    push \$R3

    GetFileTime "\${sourceFile}" \$R0 \$R1
    GetFileTime "\${destinationFile}" \$R2 \$R3

    IntCmp \$R0 \$R2 0 older_\${MOVEFILE_JUMP} newer_\${MOVEFILE_JUMP}
    IntCmp \$R1 \$R3 older_\${MOVEFILE_JUMP} older_\${MOVEFILE_JUMP} newer_\${MOVEFILE_JUMP}

    older_\${MOVEFILE_JUMP}:
    delete "\${sourceFile}"
    goto time_check_done_\${MOVEFILE_JUMP}

    newer_\${MOVEFILE_JUMP}:
    delete "\${destinationFile}"
    rename "\${sourceFile}" "\${destinationFile}"
    delete "\${sourceFile}" ;incase above failed!

    time_check_done_\${MOVEFILE_JUMP}:

    Pop \$R3
    Pop \$R2
    Pop \$R1
    Pop \$R0

done_\${MOVEFILE_JUMP}:

!undef MOVEFILE_JUMP

!macroend



!macro GetCleanDir INPUTDIR
  !define Index_GetCleanDir 'GetCleanDir_Line\${__LINE__}'
  Push \$R0
  Push \$R1
  StrCpy \$R0 "\${INPUTDIR}"
  StrCmp \$R0 "" \${Index_GetCleanDir}-finish
  StrCpy \$R1 "\$R0" "" -1
  StrCmp "\$R1" "\\" \${Index_GetCleanDir}-finish
  StrCpy \$R0 "\$R0\\"
\${Index_GetCleanDir}-finish:
  Pop \$R1
  Exch \$R0
  !undef Index_GetCleanDir
!macroend
 
!macro RemoveFilesAndSubDirs DIRECTORY
  !define Index_RemoveFilesAndSubDirs 'RemoveFilesAndSubDirs_\${__LINE__}'
   Push \$R0
  Push \$R1
  Push \$R2
   !insertmacro GetCleanDir "\${DIRECTORY}"
  Pop \$R2
  FindFirst \$R0 \$R1 "\$R2*.*"
\${Index_RemoveFilesAndSubDirs}-loop:
  StrCmp \$R1 "" \${Index_RemoveFilesAndSubDirs}-done
  StrCmp \$R1 "." \${Index_RemoveFilesAndSubDirs}-next
  StrCmp \$R1 ".." \${Index_RemoveFilesAndSubDirs}-next
  IfFileExists "\$R2\$R1\\*.*" \${Index_RemoveFilesAndSubDirs}-directory
  ; file
  Delete "\$R2\$R1"
  goto \${Index_RemoveFilesAndSubDirs}-next
\${Index_RemoveFilesAndSubDirs}-directory:
  ; directory
  RMDir /r "\$R2\$R1"
\${Index_RemoveFilesAndSubDirs}-next:
  FindNext \$R0 \$R1
  Goto \${Index_RemoveFilesAndSubDirs}-loop
\${Index_RemoveFilesAndSubDirs}-done:
  FindClose \$R0
  Pop \$R2
  Pop \$R1
  Pop \$R0
  !undef Index_RemoveFilesAndSubDirs
!macroend



;begin GameUX 7/10
Var GameExplorer_ContextId
;!define GAME_EXPLORER_HELPER_PATH "C:\\"
!define GameExplorer_AddGame "!insertmacro GameExplorer_AddGame"
!define GameExplorer_AddPlayTask "!insertmacro GameExplorer_AddPlayTask"
!define GameExplorer_AddSupportTask "!insertmacro GameExplorer_AddSupportTask"
!define GameExplorer_RemoveGame "!insertmacro GameExplorer_RemoveGame"
 
!macro GameExplorer_AddGame CONTEXT GDF INSTDIR RUNPATH RUNARGS SAVEGAMEEXT
  Push \$0
  Push \$1
  Push \$2
  Push \$3
  Push \$4
  SetOutPath \$PLUGINSDIR
  !ifndef GAME_EXPLORER_DLL_EXISTS
    !ifdef GAME_EXPLORER_HELPER_PATH
      File "/oname=GameuxInstallHelper.dll" "\${GAME_EXPLORER_HELPER_PATH}"
    !else
      File "C:\\GameuxInstallHelper.dll"
    !endif
    !define GAME_EXPLORER_DLL_EXISTS
  !endif
  !if "\${CONTEXT}" == "current"
    StrCpy \$GameExplorer_ContextId 2
  !else if  "\${CONTEXT}" == "all"
    StrCpy \$GameExplorer_ContextId 3
  !else
    !error 'Context must be "current" or "all"'
  !endif
  System::Call 'GameuxInstallHelper::GenerateGUID(g .r0)'
  !ifndef GAME_EXPLORER_GUID_DECLARED
    Var /GLOBAL GameExplorer_GUID
    !define GAME_EXPLORER_GUID_DECLARED
  !endif
  \${If} \$0 != "{00000000-0000-0000-0000-000000000000}"
    StrCpy \$GameExplorer_GUID \$0
    StrCpy \$1 "\${GDF}"
    StrCpy \$2 "\${INSTDIR}"
    System::Call "GameuxInstallHelper::AddToGameExplorerA(t r1, t r2, i \$GameExplorer_ContextId, g r0)"
    StrCpy \$3 "\${RUNPATH}"
    StrCpy \$4 "\${RUNARGS}"
    !ifndef GAME_EXPLORER_PLAYTASK_NUM_DECLARED
      Var /GLOBAL GameExplorer_PlaytaskNum
      !define GAME_EXPLORER_PLAYTASK_NUM_DECLARED
    !endif
    StrCpy \$GameExplorer_PlaytaskNum 0
    !ifndef GAME_EXPLORER_SUPPORTTASK_NUM_DECLARED
      Var /GLOBAL GameExplorer_SupporttaskNum
      !define GAME_EXPLORER_SUPPORTTASK_NUM_DECLARED
    !endif
    StrCpy \$GameExplorer_SupporttaskNum 0
    System::Call "GameuxInstallHelper::RegisterWithMediaCenterA(t r1, t r2, i \$GameExplorer_ContextId, t r3, t r4, i 1)" 
    !if "\${SAVEGAMEEXT}" != ""
      StrCpy \$2 "\${SAVEGAMEEXT}"
      !if "\${RUNARGS}" != ""
        StrCpy \$4 "\${RUNARGS} \$\"%1\$\""
      !else
        StrCpy \$4 '"%1"'
      !endif
      System::Call "GameuxInstallHelper::SetupRichSavedGamesA(t r2, t r3, t r4)"
    !endif
  \${EndIf}
  Pop \$4
  Pop \$3
  Pop \$2
  Pop \$1
  Pop \$0
!macroend
 
!macro GameExplorer_AddPlayTask TASKNAME RUNPATH RUNARGS
  Push \$0
  Push \$1
  Push \$2
  StrCpy \$0 "\${TASKNAME}"
  StrCpy \$1 "\${RUNPATH}"
  StrCpy \$2 "\${RUNARGS}"
  !ifndef GAME_EXPLORER_GUID_DECLARED
    Var /GLOBAL GameExplorer_GUID
    !define GAME_EXPLORER_GUID_DECLARED
  !endif
  !ifndef GAME_EXPLORER_PLAYTASK_NUM_DECLARED
    Var /GLOBAL GameExplorer_PlaytaskNum
    !define GAME_EXPLORER_PLAYTASK_NUM_DECLARED
  !endif
  System::Call "GameuxInstallHelper::CreateTaskA(i \$GameExplorer_ContextId, g '\$GameExplorer_GUID', i 0, i \$GameExplorer_PlaytaskNum, t r0, t r1, t r2)"
  IntOp \$GameExplorer_PlaytaskNum \$GameExplorer_PlaytaskNum + 1
  Pop \$2
  Pop \$1
  Pop \$0
!macroend
 
!macro GameExplorer_AddSupportTask TASKNAME SUPPORTPATH
  Push \$0
  Push \$1
  StrCpy \$0 "\${TASKNAME}"
  StrCpy \$1 "\${SUPPORTPATH}"
  !ifndef GAME_EXPLORER_GUID_DECLARED
    Var /GLOBAL GameExplorer_GUID
    !define GAME_EXPLORER_GUID_DECLARED
  !endif
  !ifndef GAME_EXPLORER_SUPPORTTASK_NUM_DECLARED
    Var /GLOBAL GameExplorer_SupporttaskNum
    !define GAME_EXPLORER_SUPPORTTASK_NUM_DECLARED
  !endif
  System::Call "GameuxInstallHelper::CreateTaskA(i \$GameExplorer_ContextId, g '\$GameExplorer_GUID', i 0, i \$GameExplorer_SupporttaskNum, t r0, t r1, '')"
  IntOp \$GameExplorer_SupporttaskNum \$GameExplorer_SupporttaskNum + 1
  Pop \$1
  Pop \$0
!macroend
 
!macro GameExplorer_RemoveGame CONTEXT GDF INSTDIR RUNPATH SAVEGAMEEXT
  Push \$0
  Push \$1
  Push \$2
  Push \$3
  !if "\${CONTEXT}" == "current"
    StrCpy \$GameExplorer_ContextId 2
  !else if  "\${CONTEXT}" == "all"
    StrCpy \$GameExplorer_ContextId 3
  !else
    !error 'Context must be "current" or "all"'
  !endif
  SetOutPath \$PLUGINSDIR
  !ifndef UNGAME_EXPLORER_DLL_EXISTS
    !ifdef GAME_EXPLORER_HELPER_PATH
      File "/oname=GameuxInstallHelper.dll" "\${GAME_EXPLORER_HELPER_PATH}"
    !else
      File "C:\\GameuxInstallHelper.dll"
    !endif
    !define UNGAME_EXPLORER_DLL_EXISTS
  !endif
  StrCpy \$1 "\${GDF}"
  System::Call "GameuxInstallHelper::RetrieveGUIDForApplicationA(t r1, g .r0)"
  System::Call "GameuxInstallHelper::RemoveTasks(g r0)"
  System::Call "GameuxInstallHelper::RemoveFromGameExplorer(g r0)"
  StrCpy \$2 "\${INSTDIR}"
  StrCpy \$3 "\${RUNPATH}"
  System::Call "GameuxInstallHelper::UnRegisterWithMediaCenterA(t r2, i \$GameExplorer_ContextId, t r3, i 0)"
  !if "\${SAVEGAMEEXT}" != ""
    StrCpy \$2 "\${SAVEGAMEEXT}"
    System::Call "GameuxInstallHelper::RemoveRichSavedGamesA(t r2)"
  !endif
  Pop \$3
  Pop \$2
  Pop \$1
  Pop \$0
!macroend

; End GameUX - 7/10


!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_ICON "C:\\allegg.ico"
!define MUI_UNICON "C:\\allegr.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "\${NSISDIR}\\Contrib\\Graphics\\Wizard\\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "\${NSISDIR}\\Contrib\\Graphics\\Wizard\\orange-uninstall.bmp"

!define MUI_WELCOMEPAGE_TITLE 'Welcome to the Allegiance $ver Setup Wizard'
!define MUI_FINISHPAGE_TITLE 'Completed the Allegiance $ver Setup Wizard'
;!define MUI_WELCOMEPAGE_TITLE_3LINES

!insertmacro MUI_PAGE_WELCOME

!insertmacro MUI_PAGE_LICENSE "C:\\EULA.rtf"
!insertmacro MUI_PAGE_DIRECTORY

!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Free Allegiance"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "\${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "\${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "\${PRODUCT_STARTMENU_REGVAL}"
Var ICONS_GROUP
!insertmacro MUI_PAGE_STARTMENU Application \$ICONS_GROUP
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_SHOWREADME "\$INSTDIR\\Readme.rtf"
!define MUI_FINISHPAGE_RUN '\$INSTDIR\\$clientbinary'
!define MUI_FINISHPAGE_RUN_TEXT 'Launch Allegiance'
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_LINK 'Explore Install'
!define MUI_FINISHPAGE_LINK_LOCATION '\$INSTDIR'
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "SpanishInternational"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Swedish"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "NorwegianNynorsk"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "Greek"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Ukrainian"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Croatian"
  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Thai"
  !insertmacro MUI_LANGUAGE "Romanian"
  !insertmacro MUI_LANGUAGE "Latvian"
  !insertmacro MUI_LANGUAGE "Macedonian"
  !insertmacro MUI_LANGUAGE "Estonian"
  !insertmacro MUI_LANGUAGE "Turkish"
  !insertmacro MUI_LANGUAGE "Lithuanian"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "SerbianLatin"
  !insertmacro MUI_LANGUAGE "Arabic"
  !insertmacro MUI_LANGUAGE "Farsi"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Indonesian"
  !insertmacro MUI_LANGUAGE "Mongolian"
  !insertmacro MUI_LANGUAGE "Luxembourgish"
  !insertmacro MUI_LANGUAGE "Albanian"
  !insertmacro MUI_LANGUAGE "Breton"
  !insertmacro MUI_LANGUAGE "Belarusian"
  !insertmacro MUI_LANGUAGE "Icelandic"
  !insertmacro MUI_LANGUAGE "Malay"
  !insertmacro MUI_LANGUAGE "Bosnian"
  !insertmacro MUI_LANGUAGE "Kurdish"
  !insertmacro MUI_LANGUAGE "Irish"
  !insertmacro MUI_LANGUAGE "Uzbek"
  !insertmacro MUI_LANGUAGE "Galician"
  !insertmacro MUI_LANGUAGE "Afrikaans"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Esperanto"

!include WordFunc.nsh
!insertmacro VersionCompare
;
;******************************************
;

Name "\${PRODUCT_NAME} \${PRODUCT_VERSION} (b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE})"
Var DirectXSetupError
var ArtPath
var bSilent
var OSver
Var InstallDotNET
var myArtName
var myArtCRC
var myArt7z
var myArtLocal
var VCLink
var NetLink
var MyLang

VIAddVersionKey /LANG=\${LANG_ENGLISH} "ProductName" "Free Allegiance Installer"
VIAddVersionKey /LANG=\${LANG_ENGLISH} "Comments" "Created by build.alleg.net on $now"
VIAddVersionKey /LANG=\${LANG_ENGLISH} "CompanyName" "Free Allegiance Organization"
VIAddVersionKey /LANG=\${LANG_ENGLISH} "LegalTrademarks" "Allegiance is a trademark of Microsoft Corporation"
VIAddVersionKey /LANG=\${LANG_ENGLISH} "LegalCopyright" "© 1995-2000 Microsoft Corporation.  All rights reserved."
VIAddVersionKey /LANG=\${LANG_ENGLISH} "FileDescription" "The Microsoft© Allegiance Installer"
VIAddVersionKey /LANG=\${LANG_ENGLISH} "FileVersion" "1.$ver.\${PRODUCT_BUILD}.0.\${PRODUCT_CHANGE}"
VIProductVersion "$ver.0.0"


InstallDirRegKey HKLM "\${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

OutFile "C:\\Inetpub\\wwwroot\\build\\${shortname}_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.exe"

InstallDir "C:\\$filen"

BrandingText "Free Allegiance - http://www.freeallegiance.org" 

; Request application privileges for Windows Vista/Win7
XPStyle On
RequestExecutionLevel admin

!addplugindir "..\\Release"
!addplugindir "."

Function CallbackTest
  Pop \$R8
  Pop \$R9
  SetDetailsPrint textonly
  DetailPrint "Installing \$R8 / \$R9..."
  SetDetailsPrint both
FunctionEnd

Function .onInit
StrCpy \$MyLang \$LANGUAGE
WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Language" "\$MyLang" 
!define MUI_LANGDLL_REGISTRY_ROOT HKLM
!define MUI_LANGDLL_REGISTRY_KEY "Software\\Microsoft\\Microsoft Games\\Allegiance\\$ver"
!define MUI_LANGDLL_REGISTRY_VALUENAME Language
!insertmacro MUI_LANGDLL_DISPLAY

;TOP 10 HAVE LOCALIZED VC (INCL ENGLISH)
; http://msdn.microsoft.com/en-us/library/0h88fahh%28VS.85%29.aspx

\${If} \$MyLang == "1031"
	StrCpy \$VCLink "0/a/5/0a5ce308-1e15-4806-964c-72dbf88de86d" 	;GERMAN
\${Elseif} \$MyLang == "3079"
	StrCpy \$VCLink "0/a/5/0a5ce308-1e15-4806-964c-72dbf88de86d" 	;GERMAN
\${Elseif} \$MyLang == "5127"
	StrCpy \$VCLink "0/a/5/0a5ce308-1e15-4806-964c-72dbf88de86d" 	;GERMAN
\${Elseif} \$MyLang == "4103"
	StrCpy \$VCLink "0/a/5/0a5ce308-1e15-4806-964c-72dbf88de86d" 	;GERMAN
\${Elseif} \$MyLang == "2055"
	StrCpy \$VCLink "0/a/5/0a5ce308-1e15-4806-964c-72dbf88de86d" 	;GERMAN
	
\${Elseif} \$MyLang == "1036"
	StrCpy \$VCLink "9/e/d/9edd8390-011b-4c6d-9806-d8dc2b10c0fb" 	;FRENCH
\${Elseif} \$MyLang == "2060"
	StrCpy \$VCLink "9/e/d/9edd8390-011b-4c6d-9806-d8dc2b10c0fb" 	;FRENCH
\${Elseif} \$MyLang == "3084"
	StrCpy \$VCLink "9/e/d/9edd8390-011b-4c6d-9806-d8dc2b10c0fb" 	;FRENCH
\${Elseif} \$MyLang == "5132"
	StrCpy \$VCLink "9/e/d/9edd8390-011b-4c6d-9806-d8dc2b10c0fb" 	;FRENCH
\${Elseif} \$MyLang == "4108"
	StrCpy \$VCLink "9/e/d/9edd8390-011b-4c6d-9806-d8dc2b10c0fb" 	;FRENCH
	
\${Elseif} \$MyLang == "1034"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "11274"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "16394"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "13322"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "9226"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "5130"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "7178"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "12298"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "4106"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "18442"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "2058"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "19466"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "6154"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "10250"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "20490"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "15370"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "17418"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH	
\${Elseif} \$MyLang == "14346"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH
\${Elseif} \$MyLang == "8202"
	StrCpy \$VCLink "2/e/6/2e6b8e03-59ff-492e-a4fd-b443845379b4" 	;SPANISH	
	

\${Elseif} \$MyLang == "3076"
	StrCpy \$VCLink "5/9/e/59e74271-2b59-49a1-b955-96b69cc34f38" 	;GB Chinese 
\${Elseif} \$MyLang == "5124"
	StrCpy \$VCLink "5/9/e/59e74271-2b59-49a1-b955-96b69cc34f38" 	;GB Chinese 
	
\${Elseif} \$MyLang == "2052"
	StrCpy \$VCLink "3/7/2/372b298d-0b9d-488f-905e-162f27d93895" 	;BIG5 Chinese 
\${Elseif} \$MyLang == "4100"
	StrCpy \$VCLink "3/7/2/372b298d-0b9d-488f-905e-162f27d93895" 	;BIG5 Chinese 
\${Elseif} \$MyLang == "1028"
	StrCpy \$VCLink "3/7/2/372b298d-0b9d-488f-905e-162f27d93895" 	;BIG5 Chinese 
	
\${Elseif} \$MyLang == "1041"
	StrCpy \$VCLink "1/b/2/1b2312a1-b2e5-479c-9103-d9616e6761d9" 	;Japanese
	
\${Elseif} \$MyLang == "1042"
	StrCpy \$VCLink "5/6/3/563256db-7faf-440e-839e-c12efe19388d" 	;Korean
	
\${Elseif} \$MyLang == "1040"
	StrCpy \$VCLink "8/7/f874831f-2dbf-45cf-8d8c-c1a442b2fbbb" 	;Italian
\${Elseif} \$MyLang == "2064"
	StrCpy \$VCLink "8/7/f874831f-2dbf-45cf-8d8c-c1a442b2fbbb" 	;Italian 	

	
\${Elseif} \$MyLang == "1049"
	StrCpy \$VCLink "8/E/F/8EF60E68-2D69-4377-AC1E-3BAF4D701ED1" 	;Russian
\${Elseif} \$MyLang == "2073"
	StrCpy \$VCLink "8/E/F/8EF60E68-2D69-4377-AC1E-3BAF4D701ED1" 	;Russian	

\${Else}
	StrCpy \$VCLink "d/d/9/dd9a82d0-52ef-40db-8dab-795376989c03" 	;English
\${Endif}

StrCpy \$NetLink "c/6/e/c6e88215-0178-4c6c-b5f3-158ff77b1f38" ;OK 7/10	

FunctionEnd

Function GetDotNETVersion
	Push \$0
	Push \$1

	System::Call "mscoree::GetCORVersion(w .r0, i \${NSIS_MAX_STRLEN}, *i) i .r1"
	StrCmp \$1 "error" 0 +2
	StrCpy \$0 "not found"

	Pop \$1
	Exch \$0
FunctionEnd


;******************************** The stuff to install
Section "" ;No components page, name is not important
  
  Call IsSilent
  Pop \$0
  StrCpy \$bSilent \$0

\${If} \$bSilent == 1
	SetOutPath \$EXEDIR\\${shortname}_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}
\${ElseIf} \$bSilent == 2
	SetOutPath C:\\build\\Dumps\\temp
\${Else}
	SetOutPath \$INSTDIR
\${EndIf}

;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;	*
;;;;;;;;;;;;;;;;;;;;;;;;;  BEGIN
;;;;;;;;;;;;;;;;;;;;;;;;;	*
;;;;;;;;;;;;;;;;;;;;;;;;;

LogEx::Init true "${shortname}_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}_install.log"
LogEx::Write true true "Allegiance ${shortname}_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE} $ver installer starting..."
LogEx::Write true true "Keyed with PDB \${PRODUCT_PDB_KEY} & ART \${PRODUCT_ART_KEY} at $now"
LogEx::Write true true "Language: \$MyLang"
LogEx::Write true true "SilentMode? \$bSilent"

  ; Check .NET version
  StrCpy \$InstallDotNET "No"
  Call GetDotNETVersion
  Pop \$0
  
  \${If} \$0 == "not found"
        StrCpy \$InstallDotNET "Yes"
  \${EndIf}

  StrCpy \$0 \$0 "" 1 # skip "v"

  \${VersionCompare} \$0 "2.0" \$1
  \${If} \$1 == 2
        StrCpy \$InstallDotNET "Yes"
  \${EndIf}  

LogEx::Write true true "Need .NET Version 2.0+? \$InstallDotNET Current version: \$0"

 nsisos::osversion
StrCpy \$R0 \$0
StrCpy \$R1 \$1
LogEx::Write true true "Windows OSVersion: \$R0.\$R1"

  StrCpy \$OSver \$R0
  
  SetCompress off
  LogEx::Write true true "Extracting packages..."
  SetDetailsPrint listonly

  File C:\\build\\Package\\Release.7z
  Nsis7z::ExtractWithCallback "Release.7z" \$R9
  GetFunctionAddress \$R9 CallbackTest
  Delete "\$OUTDIR\\Release.7z"
  $artwork
  File C:\\build\\Package\\External.7z
  Nsis7z::ExtractWithCallback "External.7z" \$R9
  GetFunctionAddress \$R9 CallbackTest
  Delete "\$OUTDIR\\External.7z"
  SetCompress auto
  SetDetailsPrint both
LogEx::Write true true "Creating shortcuts..."
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "\$SMPROGRAMS\\\$ICONS_GROUP"
  CreateShortCut "\$SMPROGRAMS\\\$ICONS_GROUP\\Free Allegiance.lnk" "\$INSTDIR\\$clientbinary"
  CreateShortCut "\$SMPROGRAMS\\\$ICONS_GROUP\\Allegiance Learning Guide.lnk" "\$INSTDIR\\Allegiance Learning Guide.lnk"
  CreateShortCut "\$DESKTOP\\Free Allegiance.lnk" "\$INSTDIR\\$clientbinary"
  !insertmacro MUI_STARTMENU_WRITE_END

ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
StrCmp \$ARTPATH "\$INSTDIR\\Artwork" foundok
ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
foundok:
IfFileExists "\$ARTPATH\\*.*" DontDelReg
LogEx::Write true true "Deleting bad Art Path registry entry..."
DeleteRegValue HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Artpath"
DeleteRegValue HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Artpath"
DontDelReg:

; Install Trusted Certificate Authority - To issue a developer certificate you need h0tp0ck3t$  };-)
;File C:\\FAOCA10.cer
;Push \$INSTDIR\\FAOCA10.cer
;Call AddCertificateToStore
;Pop \$0
;LogEx::Write true true "Installing the FAO CA returned: \$0"
;Delete FAOCA10.cer

DeleteRegValue HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "MoveInProgress"
DeleteRegValue HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "MoveInProgress"


ReadRegStr \$R0 HKLM "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{{1F1C2DFC-2D24-3E06-BCB8-725134ADF989}}" Publisher
StrCmp \$R0 "Microsoft Corporation" VCOK
ReadRegStr \$R0 HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{{1F1C2DFC-2D24-3E06-BCB8-725134ADF989}}" Publisher
StrCmp \$R0 "Microsoft Corporation" VCOK

ReadRegStr \$R0 HKLM "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{9A25302D-30C0-39D9-BD6F-21E6EC160475}" Publisher
StrCmp \$R0 "Microsoft Corporation" VCOK
ReadRegStr \$R0 HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{9A25302D-30C0-39D9-BD6F-21E6EC160475}" Publisher
StrCmp \$R0 "Microsoft Corporation" VCOK

LogEx::Write true true "Downloading VC9..."

;
; TODO MULTI-7 LOCALE
;
\${If} \$bSilent == 0
	SetDetailsView hide
	inetc::get /RESUME "Click Retry to resume download of the Visual C++ 2008 Redistributable. - If the error persists, install it from http://microsoft.com/downloads and restart the installer." /CAPTION "Visual C++ 2008 Redistributable" /POPUP "Visual C++ 2008 Redistributable" "http://download.microsoft.com/download/\$VCLink/vcredist_x86.exe" "\$INSTDIR\\vcredist_x86.exe" /end
	Pop \$1

	\${If} \$1 != "OK"
	   Delete "\$INSTDIR\\vcredist_x86.exe"
	   MessageBox MB_ICONEXCLAMATION|MB_OK "Visual C++ 2008 Redistributable installation failed. - Install manually from http://microsoft.com/downloads and restart the installer"
	   ;LogEx::Write true true "Download error: \$1"
	\${EndIf}

	ExecWait "\$INSTDIR\\vcredist_x86.exe /q:a"
	Delete "\$INSTDIR\\vcredist_x86.exe"
	LogEx::Write true true "Installed OK"
	SetDetailsView show

VCOK:
	\${If} \$InstallDotNET == "Yes"
	LogEx::Write true true "Downloading .NET..."
	SetDetailsView hide
	inetc::get /RESUME "Click Retry to resume download of the .NET Framework 2.0. - If the error persists, install it from http://microsoft.com/downloads and restart the installer." /CAPTION ".NET Framework 2.0" /POPUP ".NET Framework 2.0" "http://download.microsoft.com/download/\$NetLink/NetFx20SP2_x86.exe" "\$INSTDIR\\NetFx20SP2_x86.exe" /end
	Pop \$1

	\${If} \$1 != "OK"
	   Delete "\$INSTDIR\\NetFx20SP2_x86.exe"
	   MessageBox MB_ICONEXCLAMATION|MB_OK ".NET Framework 2.0 installation failed. - Install manually from http://microsoft.com/downloads and restart the installer"
	   ;LogEx::Write true true "Download error: \$1"
	\${EndIf}

	ExecWait "\$INSTDIR\\NetFx20SP2_x86.exe /quiet /norestart"
	Delete "\$INSTDIR\\NetFx20SP2_x86.exe"
	LogEx::Write true true "Installed OK"
	SetDetailsView show
	\${EndIf} 
\${Endif}
;
; ----^^ TODO MULTIPLE LOCALE
;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; removed dlcode_pdb ;;;

\${If} \$bSilent <> 2
	\${If} \$bSilent == 1
		IfFileExists "\$OUTDIR\\Artwork\\*.*" SilentSkipArt
	\${EndIf}
	$dlcode_art
SilentSkipArt:
\${EndIf}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


\${If} \$bSilent <> 2
	LogEx::Write true true "Registering AGC..."
	nsExec::Exec "regsvr32 /s \$INSTDIR\\AGC.dll"
	nsExec::Exec "\$INSTDIR\\AllSrv.exe -RegServer"
\${EndIf}


\${If} "$ver" == "1.1"
	ReadRegStr \$R0 HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\1.0" ArtPath
	StrLen \$R1 \$R0
	\${If} \$R1 <> 0 
		goto matchedArt2
	\${Endif}
	ReadRegStr \$R0 HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\1.0" ArtPath
	StrLen \$R1 \$R0
	\${If} \$R1 <> 0 
		goto matchedArt2
	\${Endif}
	goto skipreg2
	matchedArt2:
	\${GetTime}  "\$R0\\inputmap1.mdl" "M" \$1 \$2 \$3 \$4 \$5 \$6 \$7
	\${GetTime}  "\$ARTPATH\\inputmap1.mdl" "M" \$R1 \$R2 \$R3 \$R4 \$R5 \$R6 \$R7
	\${If} \$3 > \$R3
		goto CopyInputMap
	\${Elseif} \$3 == \$R3
		\${If} \$2 > \$R2
			goto CopyInputMap
		\${Elseif} \$2 == \$R2
			\${If} \$1 > \$R1
				goto CopyInputMap
			\${Elseif} \$1 == \$R1
				\${If} \$5 > \$R5
					goto CopyInputMap
				\${Elseif} \$5 == \$R5
					\${If} \$6 > \$R6
						goto CopyInputMap
					\${Elseif} \$6 == \$R6
						\${If} \$7 > \$R7
							goto CopyInputMap ;looks pretty neat
						\${Endif}
					\${Endif}
				\${Endif}
			\${Endif}
		\${Endif}	
	\${Endif}
goto skipreg2
CopyInputMap:
	LogEx::Write true true "Found newer 1.0 input map, copying that over for you..."
	CopyFiles "\$R0\\inputmap1.mdl" "\$ARTPATH\\inputmap1.mdl"
skipreg2:
\${Endif}



;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;	*
;;;;;;;;;;;;;;;;;;;;;;;;;  END
;;;;;;;;;;;;;;;;;;;;;;;;;	*
;;;;;;;;;;;;;;;;;;;;;;;;;
SectionEnd ; end the section



Section -AdditionalIcons
  WriteIniStr "\$INSTDIR\\\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "\${PRODUCT_WEB_SITE}"
;  CreateShortCut "\$SMPROGRAMS\\Free Allegiance\\Website.lnk" "\$INSTDIR\\\${PRODUCT_NAME}.url"
;  CreateShortCut "\$SMPROGRAMS\\Free Allegiance\\Uninstall.lnk" "\$INSTDIR\\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "\$INSTDIR\\uninst.exe"
  WriteRegStr HKLM "\${PRODUCT_DIR_REGKEY}" "" "\$INSTDIR\\Allegiance.exe"

  WriteRegStr \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}" "DisplayName" "\$(^Name)"
  WriteRegStr \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}" "UninstallString" "\$INSTDIR\\uninst.exe"
  WriteRegStr \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}" "DisplayIcon" "\$INSTDIR\\Allegiance.exe"
  WriteRegStr \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}" "DisplayVersion" "\${PRODUCT_VERSION}"
  WriteRegStr \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}" "URLInfoAbout" "\${PRODUCT_WEB_SITE}"
  WriteRegStr \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}" "Publisher" "\${PRODUCT_PUBLISHER}"
   
SectionEnd

Section "DirectX Install" SEC_DIRECTX

\${If} \$OSver >= 5
LogEx::Write true true "Registering /w Games Explorer..."
\${GameExplorer_AddGame} all "\$INSTDIR\\Allegiance.exe" "\$INSTDIR" "\$INSTDIR\\Allegiance.exe" "" ""
\${GameExplorer_AddPlayTask} "Safe Mode" "\$INSTDIR\\Allegiance.exe" "-software"
\${GameExplorer_AddSupportTask} "Home Page" "http://www.alleg.net/"
Delete \$PLUGINSDIR\\GameuxInstallHelper.dll
\${EndIf}

  ; set file permissions to install dir and reg (thanks pkk)
LogEx::Write true true "Setting permissions..."  
  AccessControl::GrantOnFile "\$INSTDIR" "(BU)" "GenericRead + GenericWrite"
  AccessControl::GrantOnFile "\$INSTDIR\\Artwork" "(BU)" "GenericRead + GenericWrite"
  AccessControl::GrantOnFile "\$ARTPATH" "(BU)" "GenericRead + GenericWrite"
  AccessControl::GrantOnRegKey HKLM "Software\\Microsoft\\Microsoft Games\\Allegiance" "(BU)" "FullAccess"
  AccessControl::GrantOnRegKey HKLM "Software\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance" "(BU)" "FullAccess"
  
  
; Check if the firewall is enabled
SimpleFC::IsFirewallEnabled 
Pop \$0
Pop \$1
\${If} \$1 == 1
SimpleFC::AddApplication "Allegiance" "\$INSTDIR\\Allegiance.exe" 0 2 "" 1
Pop \$0
LogEx::Write true true "Adding exception to /w Windows Firewall...\$0"
\${Endif}
  
  

\${If} \$bSilent == 0

IfFileExists "\$SYSDIR\\D3DX9_43.dll" DXOK
IfFileExists "\$%SystemRoot%\\System32\\D3DX9_43.dll" DXOK
IfFileExists "\$%windir%\\System32\\D3DX9_43.dll" DXOK
LogEx::Write true true "Installing DX..."
SectionIn RO
 SetOutPath "\$TEMP"
 File "C:\\dxwebsetup.exe"
 LogEx::Write true true "Running DirectX Setup..."
 ExecWait '"\$TEMP\\dxwebsetup.exe"' \$DirectXSetupError
 LogEx::Write true true "Finished DirectX Setup"
Delete "\$TEMP\\dxwebsetup.exe"
\${EndIf}
DXOK:
LogEx::Write true true "Everything is OK"
LogEx::Close

SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "\$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
 !insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove \$(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
nsExec::Exec "\$INSTDIR\\AllSrv.exe -UnRegServer"
nsExec::Exec "regsvr32 /u /s \$INSTDIR\\AGC.dll"

  ; Remove the firewall exception list
    SimpleFC::RemoveApplication "Allegiance"
  Pop \$0 ;

 !insertmacro MUI_STARTMENU_GETFOLDER "Application" \$ICONS_GROUP
IfFileExists "\$INSTDIR\\Artwork\\*.*" DeleteReg
goto skipreg
DeleteReg:
ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
StrCmp \$ARTPATH "\$INSTDIR\\Artwork" matchedArt
StrCmp \$ARTPATH "\$INSTDIR\\Artwork\\" matchedArt
ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
StrCmp \$ARTPATH "\$INSTDIR\\Artwork" matchedArt
StrCmp \$ARTPATH "\$INSTDIR\\Artwork\\" matchedArt
goto skipreg
matchedArt:
DeleteRegValue HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Artpath"
DeleteRegValue HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Artpath"

skipreg:
StrCmp \$INSTDIR "" skipdel
!insertmacro RemoveFilesAndSubDirs "\$INSTDIR\\"
skipdel:

  Delete "\$DESKTOP\\Free Allegiance.lnk"
  Delete "\$SMPROGRAMS\\\$ICONS_GROUP\\Free Allegiance.lnk"
  Delete "\$SMPROGRAMS\\\$ICONS_GROUP\\Allegiance Learning Guide.lnk"
  RMDir "\$SMPROGRAMS\\\$ICONS_GROUP"
  RMDir "\$INSTDIR"

  DeleteRegKey \${PRODUCT_UNINST_ROOT_KEY} "\${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "\${PRODUCT_DIR_REGKEY}"
  
\${If} \$OSver >= 5  
\${GameExplorer_RemoveGame} all "\$INSTDIR\\Allegiance.exe" "\$INSTDIR" "\$INSTDIR\\Allegiance.exe" ""
\${EndIf}
  
  SetAutoClose true
SectionEnd
END_NSIS

#'
###################################
###################################
###################################

$nsis = qq{
!define PRODUCT_BUILD "$build"
!define PRODUCT_CHANGE "$revision"
!define PRODUCT_PDB_KEY "$pdbkey"
!define PRODUCT_ART_KEY "$artkey"
$betavar
$nsis
};




open(NSIS,"| C:\\NSIS\\makensis.exe /V2 - ");
#open(NSIS,">nsis.nsi");
print NSIS $nsis;
close NSIS;











#this was removed for bard untill we get a real cert (so now we won't need it) - Imago 6/10
__END__

!define CERT_QUERY_OBJECT_FILE 1
!define CERT_QUERY_CONTENT_FLAG_ALL 16382
!define CERT_QUERY_FORMAT_FLAG_ALL 14
!define CERT_STORE_PROV_SYSTEM 10
!define CERT_STORE_OPEN_EXISTING_FLAG 0x4000
!define CERT_SYSTEM_STORE_LOCAL_MACHINE 0x20000
!define CERT_STORE_ADD_ALWAYS 4

Function AddCertificateToStore
  Exch \$0
  Push \$1
  Push \$R0
  System::Call "crypt32::CryptQueryObject(i \${CERT_QUERY_OBJECT_FILE}, w r0, i \${CERT_QUERY_CONTENT_FLAG_ALL}, i \${CERT_QUERY_FORMAT_FLAG_ALL}, i 0, i 0, i 0, i 0, i 0, i 0, *i .r0) i .R0"
  \${If} \$R0 <> 0
    System::Call "crypt32::CertOpenStore(i \${CERT_STORE_PROV_SYSTEM}, i 0, i 0, i \${CERT_STORE_OPEN_EXISTING_FLAG}|\${CERT_SYSTEM_STORE_LOCAL_MACHINE}, w 'ROOT') i .r1"
    \${If} \$1 <> 0
      System::Call "crypt32::CertAddCertificateContextToStore(i r1, i r0,i \${CERT_STORE_ADD_ALWAYS}, i 0) i .R0"
      System::Call "crypt32::CertFreeCertificateContext(i r0)"
      \${If} \$R0 = 0
        StrCpy \$0 "Unable to add certificate to certificate store"
      \${Else}
        StrCpy \$0 "success"
      \${EndIf}
      System::Call "crypt32::CertCloseStore(i r1, i 0)"
    \${Else}
      System::Call "crypt32::CertFreeCertificateContext(i r0)"
      StrCpy \$0 "Unable to open certificate store"
    \${EndIf}
  \${Else}
    StrCpy \$0 "Unable to open certificate file"
  \${EndIf}
  Pop \$R0
  Pop \$1
  Exch \$0
FunctionEnd
