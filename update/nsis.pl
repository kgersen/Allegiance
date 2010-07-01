#Imago <imagotrigger@gmail.com>
# NSIS wrapper for allegsetup
#  2 modes - Upgrade (Beta) / Full (Production)

use strict;
my ($ver, $build, $revision, $bfull) = @ARGV;

my $pdbkey ="";
my $artkey = "";
my $betavar = "";
my $filen = "Alleg";
my $shortname = "Alleg_lite";
my $artwork = "";
my $bbeta = "";
my $dlcode = "";
my $clientbinary = "ASGSClient.exe";
my $url = "http://alleg.builtbygiants.net"; #can be FTP:// 
my $cfgfile = "http://autoupdate.alleg.net/allegiance.cfg";

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
	$dlcode = qq{
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "CfgFile" "$cfgfile"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "CfgFile" "$cfgfile"
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "ArtPath" "\$INSTDIR\\Artwork"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "ArtPAth" "\$INSTDIR\\Artwork"	
  WriteRegStr HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver\\Server" "ArtPath" "\$INSTDIR\\Artwork"
  WriteRegStr HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver\\Server" "ArtPAth" "\$INSTDIR\\Artwork"   
  $asgsreg
};
} else {
	$betavar = "Var BetaSetupError";
	$dlcode = qq{
 MessageBox MB_YESNO|MB_ICONQUESTION "Download program databases for debugging?\$\\nIf you don't know what this is, click No" /SD IDYES IDNO dontDL
pdbredl:
    inetc::get /RESUME "Network connection problem.  Please reconnect and click Retry to resume downloading" /CAPTION "Program Database" /POPUP "Program database" "$url/Alleg${bbeta}PDB_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.exe" "\$INSTDIR\\PDB.7z" /END
	Pop \$0
  md5dll::GetMD5File "\$INSTDIR\\PDB.7z"
  Pop \$1
  LogEx::Write true true "Download returned: \$0 md5: \$1"
  StrCmp \${PRODUCT_PDB_KEY} \$1 pdbmd5
  MessageBox MB_YESNO|MB_ICONEXCLAMATION "Corrupted download!\$\\n\$\\nWould you like to retry?" /SD IDYES IDNO dontDL
  goto pdbredl
  pdbmd5:
  LogEx::Write true true "Extracting PDB package..."
  Nsis7z::ExtractWithCallback "\$INSTDIR\\PDB.7z" \$R9
  GetFunctionAddress \$R9 CallbackTest
  Delete PDB.7z
  dontDL:
	
  MessageBox MB_YESNO|MB_ICONQUESTION "Download build Artwork?\$\\nThis release contains new artwork files! Choose Yes unless you know what you're doing" /SD IDYES IDNO dontDL2
  artredl:
    inetc::get /RESUME "Network connection problem.  Please reconnect and click Retry to resume downloading" /CAPTION "Artwork" /POPUP "Artwork" "$url/AllegR6ART_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.exe" "\$INSTDIR\\ART.7z" /END
	Pop \$0
  md5dll::GetMD5File "\$INSTDIR\\ART.7z"
  Pop \$1
  LogEx::Write true true "Download returned: \$0 md5: \$1"
  StrCmp \${PRODUCT_ART_KEY} \$1 artmd5
  MessageBox MB_YESNO|MB_ICONEXCLAMATION "Corrupted download!\$\\n\$\\nWould you like to retry?" /SD IDYES IDNO dontDL2
  goto artredl
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
	 File "C:\\betareghlpV3.exe"
	tryagain:
	 LogEx::Write true true "Browse for Folder - Set 1.1 ArtPath"
	ExecWait '"betareghlpV3.exe"' \$BetaSetupError
	ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
	StrCmp \$ARTPATH "" nextone
	goto DL
	nextone:
	ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
	StrCmp \$ARTPATH "" tryagain
	LogEx::Write true true "Using \$ARTPATH"
 DL:
Delete "betareghlpV3.exe"
};
}




my $nsis = <<END_NSIS;
!define PRODUCT_NAME "Allegiance"
!define PRODUCT_VERSION "$ver"
!define PRODUCT_PUBLISHER "Free Allegiance"
!define PRODUCT_WEB_SITE "http://freeallegiance.org"
!define PRODUCT_DIR_REGKEY "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Allegiance\\$ver"
!define PRODUCT_UNINST_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\\${PRODUCT_PUBLISHER}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"
!include LogicLib.nsh
!define CERT_QUERY_OBJECT_FILE 1
!define CERT_QUERY_CONTENT_FLAG_ALL 16382
!define CERT_QUERY_FORMAT_FLAG_ALL 14
!define CERT_STORE_PROV_SYSTEM 10
!define CERT_STORE_OPEN_EXISTING_FLAG 0x4000
!define CERT_SYSTEM_STORE_LOCAL_MACHINE 0x20000
!define CERT_STORE_ADD_ALWAYS 4
 
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

Function IsSilent
  Push \$0
  Push \$CMDLINE
  Push "/S"
  Call StrStr
  Pop \$0
  StrCpy \$0 \$0 3
  StrCmp \$0 "/S" silent
  StrCmp \$0 "/S " silent
    StrCpy \$0 0
    Goto notsilent
  silent: StrCpy \$0 1
  notsilent: Exch \$0
FunctionEnd

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


!include "MUI2.nsh"
!define MUI_ABORTWARNING
!define MUI_ICON "C:\\allegg.ico"
!define MUI_UNICON "C:\\allegr.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "\${NSISDIR}\\Contrib\\Graphics\\Wizard\\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "\${NSISDIR}\\Contrib\\Graphics\\Wizard\\orange-uninstall.bmp"

!define MUI_WELCOMEPAGE_TITLE 'Welcome to the Allegiance $ver Setup Wizard'
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
!insertmacro MUI_LANGUAGE "English"

;
;******************************************
;

Name "\${PRODUCT_NAME} \${PRODUCT_VERSION} (b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE})"
Var DirectXSetupError
var ArtPath
var bSilent

InstallDirRegKey HKLM "\${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

OutFile "C:\\Inetpub\\wwwroot\\build\\${shortname}_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.exe"

InstallDir "C:\\$filen"

BrandingText "Free Allegiance - http://www.freeallegiance.org" 

; Request application privileges for Windows Vista/Win7
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

; The stuff to install
Section "" ;No components page, name is not important

  SetOutPath \$INSTDIR

 LogEx::Init true "${shortname}_b\${PRODUCT_BUILD}_r\${PRODUCT_CHANGE}.log"

nsisos::osversion
StrCpy \$R0 \$0
StrCpy \$R1 \$1

  LogEx::Write true true "Windows OSVersion: \$R0.\$R1"

  Call IsSilent
  Pop \$0
  StrCpy \$bSilent \$0

LogEx::Write true true "SilentMode: \$bSilent"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "\$SMPROGRAMS\\\$ICONS_GROUP"
  CreateShortCut "\$SMPROGRAMS\\\$ICONS_GROUP\\Free Allegiance.lnk" "\$INSTDIR\\$clientbinary"
  CreateShortCut "\$SMPROGRAMS\\\$ICONS_GROUP\\Allegiance Learning Guide.lnk" "\$INSTDIR\\Allegiance Learning Guide.lnk"
  CreateShortCut "\$DESKTOP\\Free Allegiance.lnk" "\$INSTDIR\\$clientbinary"
  !insertmacro MUI_STARTMENU_WRITE_END

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

ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
StrCmp \$ARTPATH "\$INSTDIR\\Artwork" foundok
ReadRegStr \$ARTPATH HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" ArtPath
foundok:
IfFileExists "\$ARTPATH\\*.*" DontDelReg
LogEx::Write true true "Deleting bad Art Path registry entry..."
DeleteRegValue HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Artpath"
DeleteRegValue HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "Artpath"
DontDelReg:

; Install Trusted Certificate Authority - To issue a developer certificate you'll need h0tp0ck3t$  };-)
;File C:\\FAOCA10.cer
;Push \$INSTDIR\\FAOCA10.cer
;Call AddCertificateToStore
;Pop \$0
;LogEx::Write true true "Installing the FAO CA returned: \$0"
;Delete FAOCA10.cer

DeleteRegValue HKLM "SOFTWARE\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "MoveInProgress"
DeleteRegValue HKLM "SOFTWARE\\Wow6432Node\\Microsoft\\Microsoft Games\\Allegiance\\$ver" "MoveInProgress"

  $dlcode

nsExec::Exec "regsvr32 /s \$INSTDIR\\AGC.dll"
nsExec::Exec "\$INSTDIR\\AllSrv.exe -RegServer"
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
SectionIn RO
 SetOutPath "\$TEMP"
 File "C:\\dxwebsetup.exe"
 LogEx::Write true true "Running DirectX Setup..."
 ExecWait '"\$TEMP\\dxwebsetup.exe"' \$DirectXSetupError
 LogEx::Write true true "Finished DirectX Setup"
Delete "\$TEMP\\dxwebsetup.exe"

LogEx::Close

SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "\$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove \$(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
nsExec::Exec "\$INSTDIR\\AllSrv.exe -UnRegServer"
nsExec::Exec "regsvr32 /u /s \$INSTDIR\\AGC.dll"

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
  SetAutoClose true
SectionEnd
END_NSIS

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
