#############################################################################
#
# Makefile for PigAccts.dll
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

SRCROOT=Test\PigAccts
MAINTARGROOT=PigAccts
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\vc\atl\inc;$(FEDSRC)\Test\Inc;$(DESTDIR)\..\..\AGC;$(DESTDIR)\..\PigsLib;$(FEDSRC)\ZLib;$(DESTDIR)\..\..\AGC;$(FEDSRC)\AGC
PCHROOT=pch
PCHINC=
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
CARGSLOCAL=/D _ATL_STATIC_REGISTRY


#############################################################################
# Rules
#

!include "..\..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS=/l 0x409 $(RARGSFLAVOR)

OBJS= $(DESTDIR)\PigAccts.obj            \
      $(DESTDIR)\SrcInc.obj              \
      $(DESTDIR)\PigAccount.obj          \
      $(DESTDIR)\PigAccountDispenser.obj \
      $(DESTDIR)\pch.obj

LIBS= $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib        \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib \
      version.lib netapi32.lib delayimp.lib comsupp.lib

USERLIBS= $(FEDROOT)\Objs\$(FLAVOR)\Test\TCLib\TCLib.lib     \
          $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGCGuid.lib          \
          $(FEDROOT)\Objs\$(FLAVOR)\Test\PigGUID\PigGUID.lib \
          $(FEDROOT)\Objs\$(FLAVOR)\ZLib\Zlib.lib

RESFILE = $(DESTDIR)\PigAccts.res

LARGS= /LIBPATH:$(FEDEXT)\Vc\lib                     \
       /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE   \
       $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO)          \
       /delayload:user32.dll /delayload:advapi32.dll \
       /delayload:netapi32.dll                       \
       /MACHINE:I386


#############################################################################
#
# Dependencies
#

$(DESTDIR)\PigAccts.ini :
  @echo Copying PigAccts.ini to $(DESTDIR)...
  copy PigAccts.ini $(DESTDIR)

$(MAINTARGFULLPATH) : $(DESTDIR) $(RESFILE) $(OBJS) $(USERLIBS) $(DESTDIR)\PigAccts.ini
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS)
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map 2> NUL

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN): UnregServer
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\PigAccts.obj            : $(PCHFILE)
$(DESTDIR)\SrcInc.obj              : $(PCHFILE)
$(DESTDIR)\PigAccount.obj          : $(PCHFILE)
$(DESTDIR)\PigAccountDispenser.obj : $(PCHFILE)

$(RESFILE): $(FEDSRC)\Inc\VerRes.rc resource.h $(MAINTARGROOT).rc $(MAINTARGROOT).rc2 *.rgs
   @echo Compiling Resources...
#  $(RC) $(RARGS) /Fo$(RESFILE) $(MAINTARGROOT).rc
   @copy $(MAINTARGROOT).rc $(DESTDIR)
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
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
