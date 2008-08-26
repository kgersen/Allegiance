#############################################################################
#
# Makefile for PigSrv.dll
#
# Environment variable Requirements:
#   
#    INCLUDE=foo           // can be set to anything, must be set
#    PATH=                 // must have proper nmake on path
#    FEDROOT=<path>        // MUST point to base of fed src enlistment
#    BCBINDIR=<path>       // optional, unless using BCHK=
#    TTBINDIR=<path>       // optional, unless using TRUETIME=
#
# nmake args:
#
#   Clean        // will delete all files in destination dir
#   Full         // will Clean, then build all derived files
#   debug=       // builds a debug build (default)
#   retail=      // builds a retail build
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
#
#  All derived files (temp files, .res, output of midl, *.obj, *.exe/.dll...
#  are placed directly into the appropriate tree under $(FEDROOT)\Objs.
#
#  The $(FEDROOT)\Objs tree is created on demand.
#  Since no derived files are placed in the source tree, multiple builds
#  can be run SIMULTANEOUSLY.
#


#############################################################################
# Environment Variables
#

SRCROOT=Test\PigSrv
MAINTARGROOT=PigSrv
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\vc\atl\inc;$(FEDSRC)\Test\Inc;$(FEDSRC)\AGC;$(FEDROOT)\Objs\$(FLAVOR)\AGC;$(DESTDIR)\..\PigsLib;$(FEDEXT)\Zone;$(FEDSRC)\zlib;$(FEDSRC)\engine;$(FEDSRC)\Guids;$(FEDSRC)\Lobby
PCHROOT=pch
PCHINC=$(FEDSRC)\zlib\*.h
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
CARGSLOCAL=/D _ATL_STATIC_REGISTRY /D AGC_HOST


#############################################################################
# Rules
#

!include "..\..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS=/l 0x409 $(RARGSFLAVOR)

OBJS= $(DESTDIR)\SrcInc.obj                   \
      $(DESTDIR)\PigSrv.obj                   \
      $(DESTDIR)\PigSession.obj               \
      $(DESTDIR)\PigSessionEventSink.obj      \
      $(DESTDIR)\PigEngine.obj                \
      $(DESTDIR)\PigBehaviorHost.obj          \
      $(DESTDIR)\PigBehaviorScriptType.obj    \
      $(DESTDIR)\Pigs.obj                     \
      $(DESTDIR)\Pig.obj                      \
      $(DESTDIR)\PigBehaviorScript.obj        \
      $(DESTDIR)\PigBehaviorScriptMethods.obj \
      $(DESTDIR)\PigBehaviorStack.obj         \
      $(DESTDIR)\PigShip.obj                  \
      $(DESTDIR)\PigEventOwner.obj            \
      $(DESTDIR)\PigEvent.obj                 \
      $(DESTDIR)\PigShipEvent.obj             \
      $(DESTDIR)\PigTimer.obj                 \
      $(DESTDIR)\PigHullTypes.obj             \
      $(DESTDIR)\pch.obj

LIBS= $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib            \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib     \
      version.lib netapi32.lib urlmon.lib delayimp.lib ws2_32.lib dplayx.lib \
      wininet.lib lz32.lib comsupp.lib

USERLIBS= $(FEDROOT)\Objs\$(FLAVOR)\Test\TCLib\TCLib.lib       \
          $(FEDROOT)\Objs\$(FLAVOR)\Test\PigGUID\PigGUID.lib   \
          $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGCGuid.lib            \
          $(FEDROOT)\Objs\$(FLAVOR)\ZLib\ZLib.lib              \
          $(FEDROOT)\Objs\$(FLAVOR)\Engine\Engine.lib          \
          $(FEDROOT)\Objs\$(FLAVOR)\_utility\Utility.lib       \
          $(FEDROOT)\Objs\$(FLAVOR)\IGC\IGC.lib                \
          $(FEDROOT)\Objs\$(FLAVOR)\ClintLib\ClintLib.lib      \
          $(FEDROOT)\Objs\$(FLAVOR)\Guids\FedGuids.lib         \
          $(DXROOT)\lib\dxguid.lib

RESFILE = $(DESTDIR)\PigSrv.res

LARGS= /LIBPATH:$(FEDEXT)\Vc\lib                       \
       /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE     \
       $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO)            \
       /delayload:ole32.dll /delayload:oleaut32.dll    \
       /delayload:user32.dll /delayload:advapi32.dll   \
       /delayload:netapi32.dll                         \
       /MACHINE:I386


#############################################################################
#
# Dependencies
#

$(DESTDIR)\AGC.DLL: $(FEDROOT)\objs\$(FLAVOR)\AGC\AGC.DLL
  copy $(FEDROOT)\objs\$(FLAVOR)\AGC\AGC.DLL $(DESTDIR)

$(MAINTARGFULLPATH) : $(DESTDIR) $(RESFILE) $(OBJS) $(USERLIBS) $(DESTDIR)\AGC.DLL
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS)
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map 2> NUL

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN): UnregServer
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\SrcInc.obj                   : $(PCHFILE)
$(DESTDIR)\PigSrv.obj                   : $(PCHFILE)
$(DESTDIR)\PigSession.obj               : $(PCHFILE)
$(DESTDIR)\PigEngine.obj                : $(PCHFILE)
$(DESTDIR)\PigBehaviorHost.obj          : $(PCHFILE)
$(DESTDIR)\PigBehaviorScriptType.obj    : $(PCHFILE)
$(DESTDIR)\Pigs.obj                     : $(PCHFILE)
$(DESTDIR)\Pig.obj                      : $(PCHFILE)
$(DESTDIR)\PigBehaviorScript.obj        : $(PCHFILE)
$(DESTDIR)\PigBehaviorScriptMethods.obj : $(PCHFILE)
$(DESTDIR)\PigBehaviorStack.obj         : $(PCHFILE)
$(DESTDIR)\PigShip.obj                  : $(PCHFILE)
$(DESTDIR)\PigEventOwner.obj            : $(PCHFILE)
$(DESTDIR)\PigEvent.obj                 : $(PCHFILE)
$(DESTDIR)\PigShipEvent.obj             : $(PCHFILE)
$(DESTDIR)\PigTimer.obj                 : $(PCHFILE)
$(DESTDIR)\PigHullTypes.obj             : $(PCHFILE)

$(RESFILE): $(FEDSRC)\Inc\VerRes.rc resource.h $(MAINTARGROOT).rc $(MAINTARGROOT).rc2 *.rgs
  @echo Compiling Resources...
#  $(RC) $(RARGS) /Fo$(RESFILE) $(MAINTARGROOT).rc
   copy $(MAINTARGROOT).rc $(DESTDIR)
   echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   $(RC) $(RARGS) /Fo$(DESTDIR)\$(MAINTARGROOT).res $(DESTDIR)\$(MAINTARGROOT).rc

RegServer:
  $(REGEXE)

UnregServer:
  @echo Unregistering $(MAINTARGFULLPATH)...
  $(UNREGEXE)
  @if not exist $(MAINTARGFULLPATH) echo File $(MAINTARGFULLPATH) does not exist
  @if exist $(DESTDIR)\RegSvr32.trg del $(DESTDIR)\RegSvr32.trg

$(DESTDIR)\RegSvr32.trg:
  @echo Registering $(MAINTARGFULLPATH)...
  $(REGEXE)
  @echo regsvr32 execution time > $(DESTDIR)\RegSvr32.trg
  @if not exist $(MAINTARGFULLPATH) echo File $(MAINTARGFULLPATH) does not exist

BuildAndReg: $(MAINTARGFULLPATH) $(DESTDIR)\RegSvr32.trg
