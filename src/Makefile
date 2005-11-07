#############################################################################
#
# Makefile for all major Federation Components
#
# Environment variable Requirements:
#   
#    INCLUDE=foo           // can be set to anything, must be set
#    PATH=                 // must have proper nmake on path
#    FEDROOT=<path>        // MUST point to base of fed src enlistment
#    FEDDSN                // IGCFiles: Data source name for SQL, NO DEFAULT
#    FEDDSNUSER            // IGCFiles: User for the SQL login, NO DEFAULT
#    FEDDSNPW              // IGCFiles: Password for the SQL login, NO DEFAULT
#    FEDSQLSRV             // Recreate: SQL server to be passed to recreate.bat, NO DEFAULT
#    FEDSQLDB              // Recreate: SQL database to be passed to recreate.bat, NO DEFAULT
#    FEDSQLPW              // Recreate: SQL database password to be passed to recreate.bat, NO DEFAULT
#
# Environment variable options:
#    ALLEGIANCE_DEV        // Set this variable to anything if you want to use a different registry key root from the installed builds
#
# nmake args:
#
#   Clean        // will delete all files in destination dir
#   Shell        // opens os-specific cmd/command shell (helps to debug build)
#   debug=       // builds a debug build (default)
#   retail=      // builds a retail build
#   training=    // builds a debug build with special testing code for training
#   test=        // retail but with the _DEBUG preprocessor symbol defined
#                // NOT valid in combination with BCHK=, TRUETIME=, or ICAP=
#   VER=         // must have this argument, \\msr\federation\builds\$(VER) is created/needed
#   BCHK=        // if you want to do a boundschecker build.
#                   note:  you must have BC already installed locally and define BCBINDIR to
#                          point to the directory where nmcl.exe and nmlink.exe reside.
#                          You can use the BCOPTS variable to add options to the BC compile.
#   TRUETIME=    // if you want to instrument the build using Numega TrueTime
#                   note:  you must have TrueTime already installed locally and define TTBINDIR to
#                          point to the directory where the TrueTime nmcl.exe and nmlink.exe reside.  
#                          You can use the TTOPTS variable to add options to the TT compile.
#   VERBOSE=     // if you want verbose build mode. (default is non-verbose)
#   DX5=         // will use dx5 instead of the default, which is dx6
#   DX7=         // will use dx7 instead of the default, which is dx6
#   copyclient=  // will copy the client (EACH AND EVERY FLAVOR BUILT) to the artwork directory.
#                   This is so that developers can easily a client on a second machine and automatically get updates
#
# command line arguments to cut a release:
#
#       1:      use ver= commandline argument
#       2:      use retail= if you want to do a retail build
#       3:      no targets will get you all exe's/dll's and setup files (exes target)
#       4:      use CopySrc if you want to copy the sources
#       5:      use CopyArt if you want to copy the art from \\fedsrv\wwwroot\artwork\256
#       6:      use Exes target to get exes with any of the above
#
#############################################################################

!ifdef DREAMCAST

!if defined(retail)
RETAILNMAKEARGS=retail=$(RETAIL)
OBJSDIR=dreamcast
!else
RETAILNMAKEARGS=
OBJSDIR=dreamcastd
!endif

!else

!if defined(sretail)
RETAILNMAKEARGS=sretail=$(RETAIL)
OBJSDIR=sretail
!elseif defined(stest)
RETAILNMAKEARGS=stest=
OBJSDIR=stest
!elseif defined(sdebug)
RETAILNMAKEARGS=sdebug=
OBJSDIR=sdebug
!elseif defined(retail)
RETAILNMAKEARGS=retail=$(RETAIL)
OBJSDIR=retail
!elseif defined(test)
RETAILNMAKEARGS=test=
OBJSDIR=test
!elseif defined(training)
RETAILNMAKEARGS=training=
OBJSDIR=debug
!elseif defined(icap)
RETAILNMAKEARGS=icap=
OBJSDIR=icap
!elseif defined(prefix)
RETAILNMAKEARGS=prefix=
OBJSDIR=prefix
!else
RETAILNMAKEARGS=
OBJSDIR=debug
!endif

!endif

!if defined(BCHK)
NUMEGANMAKEARGS=BCHK=$(BCHK)
!else if defined(TRUETIME)
NUMEGANMAKEARGS=TRUETIME=$(TRUETIME)
!else
NUMEGANMAKEARGS=
!endif

!if defined(VER)
VERNMAKEARGS=VER=$(VER)
!else
VERNMAKEARGS=
!endif

!if defined(copyclient)
COPYCLIENT=copyclient
!else
COPYCLIENT=
!endif

!if defined(VERBOSE)
VERBOSEAT=@
VERBOSENMAKEARGS=VERBOSE=$(VERBOSE)
!else
VERBOSEAT=
VERBOSENMAKEARGS=
!endif

RECURSNMAKEARGS=$(RETAILNMAKEARGS) $(NUMEGANMAKEARGS) $(VERNMAKEARGS) $(VERBOSENMAKEARGS)

!if defined(USERNAME)
!if "$(USERNAME)"=="a-markcu"
USERNAME=markcu
!endif
RECURSNMAKEARGS=$(RECURSNMAKEARGS) USERNAME=$(USERNAME)
!endif

!if defined(WIN95)
RECURSNMAKEARGS=$(RECURSNMAKEARGS) WIN95=$(WIN95)
!endif

!if defined(VERBOSE)
NMAKE=nmake
!else
NMAKE=@nmake /NOLOGO /S /C
!endif


#############################################################################
#
# Include system path macros
#
#############################################################################

!include "path.mak"


#############################################################################
#
# All: Build all of the components of one flavor.
#
#############################################################################

All: Client AllGuardApp Server Server32 AllSrvUI32App Lobby Club AutoUpdateApp Test


#############################################################################
#
# Clean: Deletes all of the output of a flavor.
#
#############################################################################

Clean:
    $(FEDROOT)\src\tools\bin32\delnode -q $(FEDROOT)\Objs\$(OBJSDIR)

#############################################################################
#
#                 **** Is this ever used by anyone? ****
#
#############################################################################

Shell:
    $(OSCMDSHELL)


#############################################################################
#
# Full: Clean builds a flavor.
#
#############################################################################

Full: Clean All


#############################################################################
#
# Both: clean builds both flavors
#
#############################################################################

Both:
    $(NMAKE) Clean
    $(NMAKE) Clean retail=
    $(NMAKE)
    $(NMAKE) retail=

TrainingBoth:
    $(NMAKE) Clean training=
    $(NMAKE) Clean retail=
    $(NMAKE) training=
    $(NMAKE) retail=

CleanAllFlavors:
    $(NMAKE) Clean
    $(NMAKE) Clean retail=
    $(NMAKE) Clean test=
    $(NMAKE) Clean sretail=

AllFlavors:
    $(NMAKE)
    $(NMAKE) retail=
    $(NMAKE) test=
    
ClientAllFlavors:
    $(NMAKE) Client
    $(NMAKE) Client retail=
    $(NMAKE) Client test=

#############################################################################
#
# Daily: The daily build
#
#############################################################################

Daily: DailyNoArt Art

DailyNoArt: AllFlavors ReadMe
    $(NMAKE) sretail= AllGuardApp


#############################################################################
#
# Mikeke: Builds everything but igcfiles test
#
#############################################################################

mikeke: client server lobby art browse

#############################################################################
#
# parisman: Builds everything but igcfiles test
#
#############################################################################

parisman: client server lobby art browse

#############################################################################
#
# BrettonW: build stuff I care about
#
#############################################################################

BrettonW: Client AllSrvUI32App art

#############################################################################
#
# dpugh: Builds everything but igcfiles and test (same as mikeke)
#
#############################################################################

dpugh: client server lobby art browse

#############################################################################
#
# kendrics: Builds everything but igcfiles and test
#
#############################################################################

kendrics: client server club lobby art browse

#############################################################################
#
# Recreate: recreate the database from .dat files
#
#############################################################################

!if "$(FEDSQLSRV)" != ""
OPT_FEDSQLSRV=/S $(FEDSQLSRV)
!else
OPT_FEDSQLSRV=
!endif

!if "$(FEDSQLDB)" != ""
OPT_FEDSQLDB=/D $(FEDSQLDB)
!else
OPT_FEDSQLDB=
!endif

!if "$(FEDSQLPW)" != ""
OPT_FEDSQLPW=/P $(FEDSQLPW)
!else
OPT_FEDSQLPW=
!endif

Recreate:
    @echo
    @echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    @echo \\
    @echo \\ Recreating Database
    @echo \\
    @echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    $(VERBOSEAT)cd $(FEDROOT)\src\database
    $(VERBOSEAT).\recreate.bat $(OPT_FEDSQLSRV) $(OPT_FEDSQLDB) $(OPT_FEDSQLPW)
    $(VERBOSEAT)cd ..


#############################################################################
#
# IGCFiles: builds data files used as the IGC database. Note that server
#           should be configured before this is attempted
#
#############################################################################

IGCFiles: Server
    @echo
    @echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    @echo \\
    @echo \\ Building IGC Files
    @echo \\
    @echo \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    $(VERBOSEAT)$(FEDROOT)\objs\$(OBJSDIR)\FedSrv\allsrv.exe /encryptigcfiles $(FEDROOT)\objs\artwork /SQLSrc=$(FEDDSN) /SQLUser=$(FEDDSNUSER) /SQLPW=$(FEDDSNPW)


#############################################################################
#
# Browse: builds a browse file for everything but fedsrv and test.
#
#############################################################################

BSCDIR=$(FEDROOT)\objs\$(OBJSDIR)
BSC=$(BSCDIR)\wintrek\Allegiance.bsc
BSCFILES=                       \
    $(BSCDIR)\wintrek\*.sbr     \
    $(BSCDIR)\zlib\*.sbr        \
    $(BSCDIR)\_utility\*.sbr    \
    $(BSCDIR)\soundengine\*.sbr \
    $(BSCDIR)\engine\*.sbr      \
    $(BSCDIR)\effect\*.sbr      \
    $(BSCDIR)\igc\*.sbr         \
    $(BSCDIR)\clintlib\*.sbr    \
    $(BSCDIR)\training\*.sbr    \
#    $(BSCDIR)\fedsrv\*.sbr      \
    $(BSCDIR)\mdledit\*.sbr     \
    $(BSCDIR)\mdlc\*.sbr

#Server
Browse: Client MDLEdit MDLC
    @echo Building $(MAINTARGROOT).bsc
    $(FEDROOT)\extern\vc\bin\bscmake /nologo /Iu /o$(BSCDIR)\wintrek\Allegiance.bsc $(BSCFILES)


#############################################################################
#
# ZLibrary: a general-purpose windows library
# ZLibrary_Full: cleans all dependencies and builds ZLibrary
#
#############################################################################

ZLibrary:
    @cd zlib
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

ZLibrary_Clean:
    @cd zlib
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

ZLibrary_Full: ZLibrary_Clean ZLibrary


#############################################################################
#
# Utility: Helper stuff
#
#############################################################################

Utility: ZLibrary FedGuids
    @cd _utility
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Utility_Clean:
    @cd _utility
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Utility_Full: Utility_Clean Utility


#############################################################################
#
# FedGuids: All Guids used by the product
#
#############################################################################

FedGuids: 
    @cd Guids
#    @echo $(RECURSNMAKEARGS)
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

FedGuids_Clean: 
    @cd Guids
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

FedGuids_Full: FedGuids_Clean FedGuids


#############################################################################
#
# MDLHeaders: Generates c++ header files from MDL source
#
#############################################################################

MDLHeaders: MDLC
    @cd artwork\256
    $(NMAKE) -f mdlheader.mak $(RECURSNMAKEARGS)
    @cd ..\..


#############################################################################
#
# Igc: Internet Game Core
#
#############################################################################

Igc: Utility MDLHeaders
    @cd Igc
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Igc_Clean:
    @cd Igc
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Igc_Full: Igc_Clean Igc


#############################################################################
#
# Dreamcast: Engine components for the Sega Dreamcast
#
#############################################################################

Dreamcast: ZLibrary
!ifdef DREAMCAST
    @cd dreamcast
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..
!endif

Dreamcast_Clean:
!ifdef DREAMCAST
    @cd dreamcast
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..
!endif

Dreamcast_Full: Dreamcast_Clean Dreamcast


#############################################################################
#
# Effect: Engine/effect components library
#
#############################################################################

Effect: Engine
    @cd Effect
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Effect_Clean:
    @cd Effect
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Effect_Full: Effect_Clean Effect


#############################################################################
#
# Engine: Engine components library
#
#############################################################################

Engine: Dreamcast FedGuids
    @cd engine
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Engine_Clean:
    @cd engine
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Engine_Full: Engine_Clean Engine


#############################################################################
#
# Common: stuff both client and server need
#
#############################################################################

Common: Igc

Common_Clean: Igc_Clean

Common_Full: Common_Clean Common


#############################################################################
#
# Training: builds the offline training library.
#
#############################################################################

Training: Common
    @cd Training
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Training_Clean:
    @cd Training
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Training_Full: Training_Clean Training


#############################################################################
#
# Clintlib: builds the shared client library.
#
#############################################################################

Clintlib: Igc FedGuids
    @cd clintlib
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Clintlib_Clean:
    @cd clintlib
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Clintlib_Full: Clintlib_Clean Clintlib


#############################################################################
#
# ReloaderAPP: builds the client's Reloader app
#
#############################################################################
Reloader: ClintLib
    @cd Reloader
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Reloader_Clean:
    @cd Reloader
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Reloader_Full: Reloader_Clean Reloader


#############################################################################
#
# Client: builds everything but fedsrv and test
#
#############################################################################

Client: Common Effect Clintlib Training SoundEngineLib Reloader Server32
    @cd wintrek
    $(NMAKE) $(COPYCLIENT) $(RECURSNMAKEARGS) 
    @cd ..


#############################################################################
#
# ICaped exes
#
#############################################################################

ICapMDLEdit: MDLEdit
    @set IPIgnoreCVError=1
    @cd ..\objs\icap\mdledit
    ..\..\..\extern\icecap4\icepick mdledit.exe
    @cd ..\..\..\src

ICapClient: Client
    @set IPIgnoreCVError=1
    @cd ..\objs\icap\wintrek
    ..\..\..\extern\icecap4\icepick allegiance.exe
    @cd ..\..\..\src

#############################################################################
#
# SoundEngineLib: builds the sound engine component
#
#############################################################################

SoundEngineLib: ZLibrary
    @cd SoundEngine
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

SoundEngineLib_Clean:
    @cd SoundEngine
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

SoundEngineLib_Full: SoundEngineLib_Clean SoundEngineLib


#############################################################################
#
# SoundTst: builds soundtest
#
#############################################################################

SoundTst: SoundEngineLib
    @cd soundtest
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

SoundTst_Clean:
    @cd soundtest
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

SoundTst_Full: SoundTst_Clean SoundTst


#############################################################################
#
# AGC: builds the Active Game Core DLL
#
#############################################################################

AGC: TCLib Igc
    @cd AGC
#    $(NMAKE) BuildAndReg $(RECURSNMAKEARGS)
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

AGC_Clean:
    @cd AGC
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

AGC_Full: AGC_Clean AGC


#############################################################################
#
# CliConfigApp: builds the server configuration utility.
#
#############################################################################

CliConfigApp:
    @cd CliConfig
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

CliConfigApp_Clean:
    @cd CliConfig
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

CliConfigApp_Full: CliConfigApp_Clean CliConfigApp


#############################################################################
#
# SrvConfigApp: builds the server configuration utility.
#
#############################################################################

SrvConfigApp: AGC AllSrvExe
    @cd srvconfig
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

SrvConfigApp_Clean:
    @cd srvconfig
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

SrvConfigApp_Full: SrvConfigApp_Clean SrvConfigApp


#############################################################################
#
# SrvConfig32App: builds the server configuration utility.
#
#############################################################################

SrvConfig32App: AGC AllSrv32Exe
    @cd SrvConfig
    $(NMAKE) $(RECURSNMAKEARGS) ALLSRV_STANDALONE=
    @cd ..

SrvConfig32App_Clean:
    @cd SrvConfig
    $(NMAKE) clean $(RECURSNMAKEARGS) ALLSRV_STANDALONE=
    @cd ..

SrvConfigApp32_Full: SrvConfigApp32_Clean SrvConfigApp32


#############################################################################
#
# AutoUpdateApp: builds the AutoUpdate utility.
#
############################################################################
AutoUpdateApp: Clintlib
    @cd AutoUpdate
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

AutoUpdateApp_Clean:
    @cd AutoUpdate
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

AutoUpdateApp_Full: AutoUpdateApp_Clean AutoUpdateApp

#############################################################################
#
# AllSrvUIApp: builds the server configuration utility.
#
#############################################################################

AllSrvUIApp: AGC AllSrvExe
    @cd AllSrvUI
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

AllSrvUIApp_Clean:
    @cd AllSrvUI
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

AllSrvUIApp_Full: AllSrvUIApp_Clean AllSrvUIApp


#############################################################################
#
# AllSrvUI32App: builds the server configuration utility.
#
#############################################################################

AllSrvUI32App: AGC AllSrv32Exe
    @cd AllSrvUI
    $(NMAKE) $(RECURSNMAKEARGS) ALLSRV_STANDALONE=
    @cd ..

AllSrvUI32App_Clean:
    @cd AllSrvUI
    $(NMAKE) clean $(RECURSNMAKEARGS) ALLSRV_STANDALONE=
    @cd ..

AllSrvUIApp32_Full: AllSrvUIApp32_Clean AllSrvUIApp32


#############################################################################
#
# ShareMemLib: builds the shared memory library.
#
#############################################################################

ShareMemLib:
    @cd sharemem
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

ShareMemLib_Clean:
    @cd sharemem
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

ShareMemLib_Full: ShareMemLib_Clean ShareMemLib


#############################################################################
#
# FedPerfDll: builds the performance monitor DLL.
#
#############################################################################

FedPerfDll: ShareMemLib
    @cd fedperf
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

FedPerfDll_Clean:
    @cd fedperf
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

FedPerfDll_Full: FedPerfDll_Clean FedPerfDll


#############################################################################
#
# SentinalExe: builds the server sentinal application.
#
#############################################################################

SentinalExe: ZLibrary Utility IGC
    @cd sentinal
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

SentinalExe_Clean:
    @cd sentinal
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

SentinalExe_Full: SentinalExe_Clean SentinalExe


#############################################################################
#
# AllSrvExe: builds the Allegiance game server application.
#
#############################################################################

AllSrvExe: AGC Common FedPerfDll
    @cd fedsrv
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

AllSrvExe_Clean:
    @cd fedsrv
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

AllSrvExe_Full: AllSrvExe_Clean AllSrvExe


#############################################################################
#
# AllSrv32Exe: builds the Allegiance (stand-alone) game server application.
#
#############################################################################

AllSrv32Exe: AGC Common FedPerfDll
    @cd fedsrv
    $(NMAKE) $(RECURSNMAKEARGS) ALLSRV_STANDALONE=
    @cd ..

AllSrv32Exe_Clean:
    @cd fedsrv
    $(NMAKE) clean $(RECURSNMAKEARGS) ALLSRV_STANDALONE=
    @cd ..

AllSrv32Exe_Full: AllSrv32Exe_Clean AllSrv32Exe


#############################################################################
#
# Server: builds everything but client and test.
#
#############################################################################

Server: SrvConfigApp CliConfigApp SentinalExe AllSrvExe

Server_Clean: AGC Common FedPerfDll SrvConfigApp_Clean CliConfigApp_Clean SentinalExe_Clean

Server_Full: Server_Clean Server


#############################################################################
#
# Server32: builds everything but client and test (for stand-alone server).
#
#############################################################################

Server32: SrvConfig32App AllSrv32Exe

Server32_Clean: AGC Common FedPerfDll SrvConfig32App_Clean

Server32_Full: Server32_Clean Server32


#############################################################################
#
# Lobby: builds Lobby.
#
#############################################################################

Lobby: ShareMemLib Utility 
    @cd Lobby
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Lobby_Clean:
    @cd Lobby
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Lobby_Full: Lobby_Clean Lobby


#############################################################################
#
# Club: builds Zone Club server.
#
#############################################################################

Club: ShareMemLib Utility 
    @cd Club
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

Club_Clean:
    @cd Club
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

Club_Full: Club_Clean Club


#############################################################################
#
# MSRGuardApp: builds the MSRGuard crash guard utility.
#
#############################################################################

MSRGuardApp: ZLib TCLib
    @cd MSRGuard
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

MSRGuardApp_Clean:
    @cd MSRGuard
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

MSRGuardApp_Full: MSRGuardApp_Clean MSRGuardApp


#############################################################################
#
# AllGuardApp: builds the AllGuard crash guard utility.
#
#############################################################################

AllGuardApp: MSRGuardApp
    @cd AllGuard
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

AllGuardApp_Clean:
    @cd AllGuard
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

AllGuardApp_Full: AllGuardApp_Clean AllGuardApp


#############################################################################
#
# SymGuard: builds the Crash Guard symbol resoltion component DLL
#
#############################################################################

SymGuard: TCLib
    @cd SymGuard
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

SymGuard_Clean:
    @cd SymGuard
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..

SymGuard_Full: SymGuard_Clean SymGuard


#############################################################################
#
# Art: builds the art
#
#############################################################################

ArtBin: ConvexHull MDLC MDLEdit XMunger

Redbook: 
    @cd artwork\redbook
    -if not exist $(FEDROOT)\Objs\Redbook md $(FEDROOT)\Objs\Redbook
    @xcopy /D /Y tracks.lst $(FEDROOT)\Objs\Redbook
    @attrib -r $(FEDROOT)\Objs\Redbook\*.lst
    @xcopy /D /Y *.wav $(FEDROOT)\Objs\Redbook
    @attrib -r $(FEDROOT)\Objs\Redbook\*.wav
    @cd ..\..

Art: ArtBin Redbook
    @cd artwork\256
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

TrainingArt: ArtBin
    @cd artwork\training
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

SoundPac1: ArtBin
    @cd artwork\soundpac1
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

DevPlayExtras: TrainingArt SoundPac1
    @echo .
    @echo \\\\\\\\\\\\\\\\\\\\\\\\\\
    @echo \\
    @echo \\ Copying extra files
    @echo \\
    @echo \\\\\\\\\\\\\\\\\\\\\\\\\\
    @xcopy /Y /D $(FEDROOT)\Objs\TrainingArt\*.* $(FEDROOT)\Objs\Artwork
    @xcopy /Y /D $(FEDROOT)\Objs\soundpac1\*.* $(FEDROOT)\Objs\Artwork

#############################################################################
#
# Fonts: builds the fonts
#
#############################################################################

Fonts: 
    @cd artwork\256
    $(NMAKE) Fonts $(RECURSNMAKEARGS)
    @cd ..\..


#############################################################################
#
# Filelist: creates the filelist.txt file
#
#############################################################################

Filelist:
    -if not exist $(FEDROOT)\Objs\Filelist md $(FEDROOT)\Objs\Filelist
    -if exist $(FEDROOT)\objs\filelist\filelistfull.txt erase $(FEDROOT)\objs\filelist\filelistfull.txt

# Directory Change
    @cd $(FEDROOT)\objs\retail\wintrek
    @copy allegiance.exe $(FEDROOT)\objs\artwork\allegianceretail.exe

# Directory Change
    @cd $(FEDROOT)\objs\retail\cliconfig
    @echo This is not an exe. >$(FEDROOT)\objs\artwork\cliconfig.exe

# Directory Change
    @cd $(FEDROOT)\objs\retail\reloader
    @copy reloader.exe $(FEDROOT)\objs\artwork\reloader.exe

# Directory Change
    @cd "$(FEDROOT)\build\IS\Allegiance\Setup Files\Uncompressed Files\Disk1"
    @copy readme.txt $(FEDROOT)\objs\artwork\readme.txt

# Directory Change
    @cd $(FEDROOT)\src\setup
    @copy ebueula.dll $(FEDROOT)\objs\artwork\ebueula.dll
    @copy allbad.dll $(FEDROOT)\objs\artwork\allbad.dll
    @copy machine.exe $(FEDROOT)\objs\artwork\machine.exe
    @copy machine.ini $(FEDROOT)\objs\artwork\machine.ini
    @copy dbghelp.dll $(FEDROOT)\objs\artwork\dbghelp.dll

# Directory Change
    @cd $(FEDROOT)\objs\sretail\AllGuard
    @copy AllGuard.exe $(FEDROOT)\objs\artwork\AllGuard.exe

# Make the filelist
    @$(FEDROOT)\src\tools\build\makefilelist.exe $(FEDROOT)\objs\artwork $(FEDROOT)\objs\filelist\filelistfull.txt

# erase the files that reloader knows about--these don't ultimately belong in the artwork folder
    @erase $(FEDROOT)\objs\artwork\allegianceretail.exe
    @erase $(FEDROOT)\objs\artwork\cliconfig.exe
    @erase $(FEDROOT)\objs\artwork\reloader.exe
    @erase $(FEDROOT)\objs\artwork\readme.txt

# DON'T erase the following files!
# These stay in the artwork directory so that Setup puts them in the artwork folder
#    @erase $(FEDROOT)\objs\artwork\ebueula.dll
#    @erase $(FEDROOT)\objs\artwork\allbad.dll
#    @erase $(FEDROOT)\objs\artwork\machine.exe
#    @erase $(FEDROOT)\objs\artwork\machine.ini
#    @erase $(FEDROOT)\objs\artwork\dbghelp.dll
#    @erase $(FEDROOT)\objs\artwork\AllGuard.exe


#############################################################################
#
# FilelistS: creates the filelist.txt file
#
#############################################################################

FilelistS:
    -if not exist $(FEDROOT)\Objs\FilelistS md $(FEDROOT)\Objs\FilelistS
    -if exist $(FEDROOT)\objs\filelistS\filelistfull.txt erase $(FEDROOT)\objs\filelistS\filelistfull.txt

    -if not exist $(FEDROOT)\Objs\ArtworkS md $(FEDROOT)\Objs\ArtworkS
    -@erase /Q $(FEDROOT)\Objs\ArtworkS\*.*
    -@copy $(FEDROOT)\Objs\Artwork\*.cvh $(FEDROOT)\Objs\ArtworkS
    -@copy $(FEDROOT)\Objs\Artwork\*.igc $(FEDROOT)\Objs\ArtworkS
#    -@copy $(FEDROOT)\extern\dpmono\*.*  $(FEDROOT)\Objs\ArtworkS

# Directory Change
    @cd $(FEDROOT)\objs\retail\AGC
    @copy AGC.dll $(FEDROOT)\objs\artworkS\agcretail.dll

# Directory Change
    @cd $(FEDROOT)\objs\retail\allsrv32
    @copy allsrv32.exe $(FEDROOT)\objs\artworkS\allsrv32retail.exe

# Directory Change
    @cd $(FEDROOT)\objs\retail\allsrvui32
    @copy allsrvui32.exe $(FEDROOT)\objs\artworkS\allsrvui32retail.exe

# Directory Change
    @cd $(FEDROOT)\objs\retail\srvconfig32
    @copy srvconfig32.exe $(FEDROOT)\objs\artworkS\srvconfig32retail.exe

# Directory Change
    @cd $(FEDROOT)\src\setup
    @copy ebueula.dll $(FEDROOT)\objs\artworkS\ebueula.dll

# Directory Change
    @cd $(FEDROOT)\objs\retail\AutoUpdate
    @copy AutoUpdate.exe $(FEDROOT)\objs\artworkS\AutoUpdate.exe

# Make the filelist
    @$(FEDROOT)\src\tools\build\makefilelist.exe $(FEDROOT)\objs\artworkS $(FEDROOT)\objs\filelistS\filelistfull.txt
    -@rd $(FEDROOT)\Objs\ArtworkS

#############################################################################
#
# ArtComp: compresses the already-built art
#
#############################################################################
#
#  The way this works:  First, we make sure the client is built.  Then we
#  build the artwork which places all the built art into the \objs\artwork
#  folder.  Next, we need three folders: a) artworkc, which contains the
#  uncompressed artwork that will get included with the client installshield
#  setup program; b) artworkcs, which contains the uncompressed artwork that
#  will get included with the standalone server installshield setup; and
#  c) artworkc2, which contains the compressed client and standalone server
#  artwork that will go into a cab and ultimately will get put on a web
#  server for both clients and standalone servers to update themselves from.
#  The artworkc2 folder needs \client and \standalone folders that hold their
#  respective filelist.txt files.  Since they both have to be named 
#  filelist.txt, they can't live in the same folder out on the web server.
#

ArtComp: ClientAllFlavors Art ReadMe Filelist FilelistS 
    -if not exist $(FEDROOT)\Objs\ArtworkC md $(FEDROOT)\Objs\ArtworkC

# the files currently in the artwork folder all get compressed into the
# artworkc folder.
# Directory Change
    @cd $(FEDROOT)\objs\artwork
    $(FEDROOT)\build\compress -d *.* $(FEDROOT)\objs\ArtworkC
    -@for %%i in (*.*) do @$(FEDROOT)\src\tools\build\isbigger.exe "%%i" "$(FEDROOT)\objs\artworkc\%%i" && copy "%%i" "$(FEDROOT)\objs\artworkc\%%i"

# Directory Change
    @cd $(FEDROOT)\objs\artworkS
    $(FEDROOT)\build\compress -d *.dll $(FEDROOT)\objs\ArtworkC
    -@for %%i in (*.dll) do @$(FEDROOT)\src\tools\build\isbigger.exe "%%i" "$(FEDROOT)\objs\artworkc\%%i" && copy "%%i" "$(FEDROOT)\objs\artworkc\%%i"

# compress the client filelist, but we don't actually use it because
# build.vbs comes along and replaces it with the incremental filelist later.
# Directory Change
    @cd $(FEDROOT)\objs\filelist
    -if not exist $(FEDROOT)\Objs\ArtworkC\Client md $(FEDROOT)\Objs\ArtworkC\Client
    $(FEDROOT)\build\compress -d $(FEDROOT)\objs\filelist\filelistfull.txt $(FEDROOT)\objs\ArtworkC\Client\filelist.txt
    -@($(FEDROOT)\src\tools\build\isbigger.exe "filelistfull.txt" "$(FEDROOT)\objs\artworkc\client\filelist.txt" && @copy "filelistfull.txt" "$(FEDROOT)\objs\artworkc\client\filelist.txt")

# Directory Change
    @cd $(FEDROOT)\objs\retail\wintrek
    $(FEDROOT)\build\compress -d allegiance.exe $(FEDROOT)\objs\ArtworkC\allegianceretail.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "allegiance.exe" "$(FEDROOT)\objs\artworkc\allegianceretail.exe" && @copy "allegiance.exe" "$(FEDROOT)\objs\artworkc\allegianceretail.exe")

# Directory Change
    @cd $(FEDROOT)\objs\retail\cliconfig
    @echo This is not an exe. >$(FEDROOT)\objs\retail\cliconfig\cliconfigfake.exe
    $(FEDROOT)\build\compress -d cliconfigfake.exe $(FEDROOT)\objs\ArtworkC\cliconfig.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "cliconfigfake.exe" "$(FEDROOT)\objs\artworkc\cliconfig.exe" && @copy "cliconfigfake.exe" "$(FEDROOT)\objs\artworkc\cliconfig.exe")

# Directory Change
    @cd $(FEDROOT)\objs\retail\reloader
    $(FEDROOT)\build\compress -d reloader.exe $(FEDROOT)\objs\ArtworkC\reloader.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "reloader.exe" "$(FEDROOT)\objs\artworkc\reloader.exe" && @copy "reloader.exe" "$(FEDROOT)\objs\artworkc\reloader.exe")

# Directory Change
    @cd "$(FEDROOT)\build\IS\Allegiance\Setup Files\Uncompressed Files\Disk1"
    $(FEDROOT)\build\compress -d readme.txt $(FEDROOT)\objs\ArtworkC\readme.txt
    -@($(FEDROOT)\src\tools\build\isbigger.exe "readme.txt" "$(FEDROOT)\objs\artworkc\readme.txt" && @copy "readme.txt" "$(FEDROOT)\objs\artworkc\readme.txt")

# Directory Change
    @cd $(FEDROOT)\src\setup
    $(FEDROOT)\build\compress -d ebueula.dll $(FEDROOT)\objs\ArtworkC\ebueula.dll
    -@($(FEDROOT)\src\tools\build\isbigger.exe "ebueula.dll" "$(FEDROOT)\objs\artworkc\ebueula.dll" && @copy "ebueula.dll" "$(FEDROOT)\objs\artworkc\ebueula.dll")
    $(FEDROOT)\build\compress -d allbad.dll $(FEDROOT)\objs\ArtworkC\allbad.dll
    -@($(FEDROOT)\src\tools\build\isbigger.exe "allbad.dll" "$(FEDROOT)\objs\artworkc\allbad.dll" && @copy "allbad.dll" "$(FEDROOT)\objs\artworkc\allbad.dll")
    $(FEDROOT)\build\compress -d machine.exe $(FEDROOT)\objs\ArtworkC\machine.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "machine.exe" "$(FEDROOT)\objs\artworkc\machine.exe" && @copy "machine.exe" "$(FEDROOT)\objs\artworkc\machine.exe")
    $(FEDROOT)\build\compress -d machine.ini $(FEDROOT)\objs\ArtworkC\machine.ini
    -@($(FEDROOT)\src\tools\build\isbigger.exe "machine.ini" "$(FEDROOT)\objs\artworkc\machine.ini" && @copy "machine.ini" "$(FEDROOT)\objs\artworkc\machine.ini")
    $(FEDROOT)\build\compress -d dbghelp.dll $(FEDROOT)\objs\ArtworkC\dbghelp.dll
    -@($(FEDROOT)\src\tools\build\isbigger.exe "dbghelp.dll" "$(FEDROOT)\objs\artworkc\dbghelp.dll" && @copy "dbghelp.dll" "$(FEDROOT)\objs\artworkc\dbghelp.dll")

# Directory Change
    @cd $(FEDROOT)\Objs\sretail\AllGuard
    $(FEDROOT)\build\compress -d AllGuard.exe $(FEDROOT)\objs\ArtworkC\AllGuard.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "AllGuard.exe" "$(FEDROOT)\objs\artworkc\AllGuard.exe" && @copy "AllGuard.exe" "$(FEDROOT)\objs\artworkc\AllGuard.exe")

# compress the standalone server filelist, but we don't actually use it because
# build.vbs comes along and replaces it with the incremental filelist later.
# Directory Change
    @cd $(FEDROOT)\objs\filelists
    -if not exist $(FEDROOT)\Objs\ArtworkC\Standalone md $(FEDROOT)\Objs\ArtworkC\Standalone
    $(FEDROOT)\build\compress -d filelistfull.txt $(FEDROOT)\objs\ArtworkC\standalone\filelist.txt
    -@($(FEDROOT)\src\tools\build\isbigger.exe "filelistfull.txt" "$(FEDROOT)\objs\artworkc\standalone\filelist.txt" && @copy "filelistfull.txt" "$(FEDROOT)\objs\artworkc\standalone\filelist.txt")

# Directory Change
    @cd $(FEDROOT)\objs\retail\agc
    $(FEDROOT)\build\compress -d agc.dll $(FEDROOT)\objs\ArtworkC\agcretail.dll
    -@($(FEDROOT)\src\tools\build\isbigger.exe "agc.dll" "$(FEDROOT)\objs\artworkc\agcretail.dll" && @copy "agc.dll" "$(FEDROOT)\objs\artworkc\agcretail.dll")

# Directory Change
    @cd $(FEDROOT)\objs\retail\allsrv32
    $(FEDROOT)\build\compress -d allsrv32.exe $(FEDROOT)\objs\ArtworkC\allsrv32retail.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "allsrv32.exe" "$(FEDROOT)\objs\artworkc\allsrv32retail.exe" && @copy "allsrv32.exe" "$(FEDROOT)\objs\artworkc\allsrv32retail.exe")

# Directory Change
    @cd $(FEDROOT)\objs\retail\allsrvui32
    $(FEDROOT)\build\compress -d allsrvui32.exe $(FEDROOT)\objs\ArtworkC\allsrvui32retail.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "allsrvui32.exe" "$(FEDROOT)\objs\artworkc\allsrvui32retail.exe" && @copy "allsrvui32.exe" "$(FEDROOT)\objs\artworkc\allsrvui32retail.exe")

# Directory Change
    @cd $(FEDROOT)\objs\retail\srvconfig32
    $(FEDROOT)\build\compress -d srvconfig32.exe $(FEDROOT)\objs\ArtworkC\srvconfig32retail.exe
    -@($(FEDROOT)\src\tools\build\isbigger.exe "srvconfig32.exe" "$(FEDROOT)\objs\artworkc\srvconfig32retail.exe" && @copy "srvconfig32.exe" "$(FEDROOT)\objs\artworkc\srvconfig32retail.exe")

# Directory Change - NOTE: Intentionally NOT compressing AutoUpdate.exe
    @cd $(FEDROOT)\objs\retail\autoupdate
    -@copy "autoupdate.exe" "$(FEDROOT)\objs\artworkc\autoupdate.exe"

# Directory Change
    @cd $(FEDROOT)


#############################################################################
#
# build mdl art files
#
#############################################################################

artmdl: MDLC
    @cd artwork\256
    $(NMAKE) $(RECURSNMAKEARGS) mdls
    @cd ..\..

#############################################################################
#
# build bmps
#
#############################################################################

artbmp: MDLC
    @cd artwork\256
    $(NMAKE) $(RECURSNMAKEARGS) bmps
    @cd ..\..

#############################################################################
#
# Quick Art: does a quick build of the art
#            i.e. it just copies files
#
#############################################################################

QuickArt: MDLC
    @cd artwork\256
    $(NMAKE) $(RECURSNMAKEARGS) quick
    @cd ..\..

#############################################################################
#
# CleanArt: Deletes all art output files
#
#############################################################################

CleanArt:
    $(FEDROOT)\src\tools\bin32\delnode -q $(FEDROOT)\Objs\Artwork
    $(FEDROOT)\src\tools\bin32\delnode -q $(FEDROOT)\Objs\TrainingArt
    $(FEDROOT)\src\tools\bin32\delnode -q $(FEDROOT)\Objs\SoundPac1
    $(FEDROOT)\src\tools\bin32\delnode -q $(FEDROOT)\Objs\artbuild

#############################################################################
#
# XMunge: builds XMunge
#
#############################################################################

XMunger: Engine
    @cd xmunge
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# Patch: builds Patch
#
#############################################################################

Patch: FedGuids ZLibrary
    @cd patch
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..

#############################################################################
#
# cvh: builds cvh
#
#############################################################################

ConvexHull: FedGuids
    @cd cvh
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# MDLC: builds MDLC
#
#############################################################################

MDLC: Effect
    @cd mdlc
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# MDLEdit: builds MDLEdit
#
#############################################################################

MDLEdit: Effect
    @cd mdledit
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# ME: builds me.exe
#
#############################################################################

ME: Engine
    @cd me
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# TCLib: builds Test Common library
#
#############################################################################

TCLib: ZLibrary
    @cd test\TCLib
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

TCLib_Clean:
    @cd test\TCLib
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

TCLib_Full: TCLib_Clean TCLib


#############################################################################
#
# PigsLib: builds Pigs COM object client DLL
#
#############################################################################

PigsLib: ClintLib TCLib AGC
    @cd test\PigsLib
#    $(NMAKE) BuildAndReg $(RECURSNMAKEARGS)
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

PigsLib_Clean:
    @cd test\PigsLib
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

PigsLib_Full: PigsLib_Clean PigsLib


#############################################################################
#
# PigGUID: builds the GUID library for Pigs COM object client DLL
#
#############################################################################

PigGUID: PigsLib
    @cd test\PigGUID
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

PigGUID_Clean:
    @cd test\PigGUID
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

PigGUID_Full: PigGUID_Clean PigGUID


#############################################################################
#
# PigAccts: builds Pigs Accounts COM object server EXE
#
#############################################################################

PigAccts: TCLib AGC PigsLib PigGUID
    @cd test\PigAccts
#    $(NMAKE) BuildAndReg $(RECURSNMAKEARGS)
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

PigAccts_Clean:
    @cd test\PigAccts
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

PigAccts_Full: PigAccts_Clean PigAccts


#############################################################################
#
# PigSrv: builds Pigs COM object server EXE
#
#############################################################################

PigSrv: Clintlib TCLib AGC PigsLib PigGUID
    @cd test\PigSrv
#    $(NMAKE) BuildAndReg $(RECURSNMAKEARGS)
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

PigSrv_Clean:
    @cd test\PigSrv
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

PigSrv_Full: PigSrv_Clean PigSrv


#############################################################################
#
# PigConfig: builds Pig Server Configuration Utility EXE
#
#############################################################################

PigConfig: ZLib TCLib AGC PigsLib PigGUID
    @cd test\PigConfig
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

PigConfig_Clean:
    @cd test\PigConfig
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

PigConfig_Full: PigConfig_Clean PigConfig


#############################################################################
#
# Pigs: builds the entire Pigs system
# Pigs_Full: cleans all dependencies and builds Pigs
#
#############################################################################

Pigs: PigAccts PigSrv


#############################################################################
#
# Test: builds everything in test.
# Test_Full: cleans all dependencies and builds Test
#
#############################################################################

Test: Common Clintlib Pigs
    @cd techtree
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# TCDeploy: builds Test Deployment COM object DLL
# TCDeploy_Full: cleans all dependencies and builds TCDeploy
# TCDeploy_Export: exports the Test Deployment COM object DLL to the network
#
# Note that these are NOT included in the Test target, since it is not part
# of the normal build process.
#
#############################################################################

TCDeploy: TCLib ZLibrary
    @cd test\TCDeploy
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..\..

TCDeploy_Clean:
    @cd test\TCDeploy
    $(NMAKE) clean $(RECURSNMAKEARGS)
    @cd ..\..

TCDeploy_Full: TCDeploy_Clean TCDeploy

TCDeploy_Export: TCDeploy
    @cd test\TCDeploy
    $(NMAKE) export $(RECURSNMAKEARGS)
    @cd ..\..


#############################################################################
#
# Cadet: builds Cadet
#
#############################################################################

Cadet: Common Effect
    @cd cadet
    $(NMAKE) $(RECURSNMAKEARGS)
    @cd ..


#############################################################################
#
# Component Object registration/unregistration targets:
#
#     RegAGC       : Registers AGC
#     UnregAGC     : Unregisters AGC
#     RegPigsLib   : Registers PigsLib
#     UnregPigsLib : Unregisters PigsLib
#     RegPigSrv    : Registers PigSrv
#     UnregPigSrv  : Unregisters PigSrv
#     RegPigAccts  : Registers PigAccts
#     UnregPigAccts: Unregisters PigAccts
#     RegPigs      : Registers PigSrv and PigAccts
#     UnregPigs    : Unregisters PigSrv and PigAccts
#
# The RegPigSrv, RegPigAccts, and RegPigs targets all make use of the
# following defines to register the corresponding COM server(s) to run under
# the user account specified:
#
#     user=[domain\]<user>  // user account name with optional domain
#                           // Can also be "Interactive User"
#     pw=[pw]               // password for the specified user account
#                           // Must be empty or not specified if
#                           // "Interactive User" is specifed for user=
#
# If these are not specified, "Interactive User" is used.
#
#############################################################################

RegAGC:
    @cd AGC
    $(NMAKE) RegServer $(RECURSNMAKEARGS)
    @cd ..

UnregAGC:
    @cd AGC
    $(NMAKE) UnregServer $(RECURSNMAKEARGS)
    @cd ..

RegPigsLib: RegAGC
    @cd test\PigsLib
    $(NMAKE) RegServer $(RECURSNMAKEARGS)
    @cd ..\..

UnregPigsLib:
    @cd test\PigsLib
    $(NMAKE) UnregServer $(RECURSNMAKEARGS)
    @cd ..\..

RegPigSrv: RegPigsLib
    @cd test\PigSrv
    $(NMAKE) RegServer $(RECURSNMAKEARGS)
    @cd ..\..

UnregPigSrv:
    @cd test\PigSrv
    $(NMAKE) UnregServer $(RECURSNMAKEARGS)
    @cd ..\..

RegPigAccts: RegPigsLib
    @cd test\PigAccts
    $(NMAKE) RegServer $(RECURSNMAKEARGS)
    @cd ..\..

UnregPigAccts:
    @cd test\PigAccts
    $(NMAKE) UnregServer $(RECURSNMAKEARGS)
    @cd ..\..

RegPigs: RegPigSrv RegPigAccts

UnregPigs: UnregPigAccts UnregPigSrv


#############################################################################
#
# ReadMe.txt file version and date stamping.
#
#############################################################################

# Warning: before reenabling this, talk to someone about the allguard crash it
#          will cause for people running 1881 who auto updated from 1876
# marksn,3/16/2000: reenabling this for pre-gold build 2011 because we don't 
#          care about pre-1881 people anymore. :)
ReadMe:
    @cscript.exe ..\build\VerDateStamp.js ^"$(FEDROOT)\doc\pm\ReadMe.txt^" ^"$(FEDROOT)\build\IS\Allegiance\Setup Files\Uncompressed Files\Disk1\ReadMe.txt^" $(VER)
