#############################################################################
#
# Makefile for PigConfig.dll
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

SRCROOT=Test\PigConfig
MAINTARGROOT=PigConfig
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\sbn\include;$(FEDEXT)\vc\mfc\inc;$(FEDEXT)\vc\atl\inc;$(FEDSRC)\Test\Inc;$(FEDSRC)\AGC;$(FEDROOT)\Objs\$(FLAVOR)\AGC;$(DESTDIR)\..\PigsLib;$(FEDEXT)\Zone;$(FEDSRC)\zlib;$(FEDSRC)\engine;$(FEDSRC)\Guids;$(FEDSRC)\Lobby
PCHROOT=pch
PCHINC=$(FEDSRC)\zlib\*.h $(FEDSRC)\test\TCLib\*.h
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
CARGSLOCAL=


#############################################################################
# Rules
#

!include "..\..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS=/l 0x409 $(RARGSFLAVOR) /I $(FEDEXT)\vc\mfc\inc

OBJS= $(DESTDIR)\AboutDlg.obj                    \
      $(DESTDIR)\PageGeneral.obj                 \
      $(DESTDIR)\PageServers.obj                 \
      $(DESTDIR)\PageEvents.obj                  \
      $(DESTDIR)\PigConfig.obj                   \
      $(DESTDIR)\PigConfigSheet.obj              \
      $(DESTDIR)\pch.obj

LIBS= $(MFCLIBSFLAVOR) $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib            \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib     \
      version.lib netapi32.lib urlmon.lib delayimp.lib ws2_32.lib dplayx.lib \
      wininet.lib lz32.lib comctl32.lib

USERLIBS= $(FEDROOT)\Objs\$(FLAVOR)\Test\PigGUID\PigGUID.lib   \
          $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGCGuid.lib            \
          $(FEDROOT)\Objs\$(FLAVOR)\Test\TCLib\TCLib.lib       \
          $(FEDROOT)\Objs\$(FLAVOR)\ZLib\ZLib.lib

RESFILE = $(DESTDIR)\PigConfig.res

LARGS= /LIBPATH:$(FEDEXT)\Vc\lib                       \
       /LIBPATH:$(FEDEXT)\Vc\mfc\lib                   \
       /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:WINDOWS     \
       $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO)            \
#       /delayload:ole32.dll /delayload:oleaut32.dll    \
#       /delayload:user32.dll /delayload:advapi32.dll   \
       /MACHINE:I386


#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH) : $(DESTDIR) $(RESFILE) $(OBJS) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS)
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map 2> NUL

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\AboutDlg.obj                 : $(PCHFILE)
$(DESTDIR)\PageGeneral.obj              : $(PCHFILE)
$(DESTDIR)\PageServers.obj              : $(PCHFILE)
$(DESTDIR)\PageEvents.obj               : $(PCHFILE)
$(DESTDIR)\PigConfig.obj                : $(PCHFILE)
$(DESTDIR)\PigConfigSheet.obj           : $(PCHFILE)

$(RESFILE): $(FEDSRC)\Inc\VerRes.rc resource.h *.rc *.rc2 *.ico
  @echo Compiling Resources...
  @copy $(MAINTARGROOT).rc $(DESTDIR)
  @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
  @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
  $(RC) $(RARGS) /Fo$(DESTDIR)\$(MAINTARGROOT).res $(DESTDIR)\$(MAINTARGROOT).rc

