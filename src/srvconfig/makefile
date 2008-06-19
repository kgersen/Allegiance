#############################################################################
#
# Makefile for SrvConfig.exe
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


SRCROOT=SrvConfig
!if defined(ALLSRV_STANDALONE)
DESTROOT=SrvConfig32
MAINTARGROOT=SrvConfig32
STANDALONEDEFS=/D "ALLSRV_STANDALONE"
SERVERDEST=$(FEDROOT)\Objs\$(FLAVOR)\AllSrv32
!else
STANDALONEDEFS=
DESTROOT=SrvConfig
MAINTARGROOT=SrvConfig
SERVERDEST=$(FEDROOT)\Objs\$(FLAVOR)\FedSrv
!endif
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\vc\mfc\inc;$(FEDEXT)\vc\atl\inc;$(FEDSRC)\Test\Inc;$(FEDSRC)\Inc;$(FEDSRC)\Zlib;$(FEDSRC)\AGC;$(FEDROOT)\Objs\$(FLAVOR)\Test\TCObj;$(FEDROOT)\Objs\$(FLAVOR)\AGC;$(FEDROOT)\Objs\$(FLAVOR)\Allsrv;$(SERVERDEST);$(FEDEXT)\sbn\include
PCHROOT=stdafx
PCHINC=$(FEDSRC)\Inc\*.h
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
CARGSLOCAL=/D"SrvConfig" $(STANDALONEDEFS) /D AGC_HOST /D _AFXDLL


#############################################################################
# Rules
#

!include "..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS = /l 0x409 /d"SrvConfig" $(RARGSFLAVOR)

OBJS= $(DESTDIR)\SrvConfig.obj         \
      $(DESTDIR)\cdate.obj             \
      $(DESTDIR)\SrvConfigSheet.obj    \
      $(DESTDIR)\PropGeneral.obj       \
      $(DESTDIR)\PropAutoDownload.obj  \
      $(DESTDIR)\PropDatabase.obj      \
      $(DESTDIR)\PropEvents.obj        \
      $(DESTDIR)\DefRegKey.obj         \
      $(DESTDIR)\VerInfo.obj

LIBS= $(MFCLIBSFLAVOR) $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib            \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib     \
      version.lib netapi32.lib urlmon.lib delayimp.lib ws2_32.lib dplayx.lib \
      comsupp.lib atlsd.lib mfc90d.lib mfcs90.lib

USERLIBS= $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGCGuid.lib\
	$(FEDROOT)\Objs\$(FLAVOR)\Zlib\zlib.lib
RESFILE= $(DESTDIR)\SrvConfig.res

LARGS=/LIBPATH:$(DXROOT)\lib \
      /LIBPATH:$(FEDEXT)\Vc\lib \
      /LIBPATH:$(FEDEXT)\Vc\mfc\lib \
      /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:WINDOWS \
      $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO) \
      /MACHINE:I386


#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH): $(DESTDIR) $(MSGFILE) $(OBJS) $(RESFILE) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS) $(DESTDIR)\$(PCHROOT).obj
#  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\SrvConfig.obj         : $(PCHFILE)
$(DESTDIR)\cdate.obj             : $(PCHFILE)
$(DESTDIR)\SrvConfigSheet.obj    : $(PCHFILE)
$(DESTDIR)\PropGeneral.obj       : $(PCHFILE)
$(DESTDIR)\PropAutoDownload.obj  : $(PCHFILE)
$(DESTDIR)\PropDatabase.obj      : $(PCHFILE)
$(DESTDIR)\PropEvents.obj        : $(PCHFILE)
$(DESTDIR)\DefRegKey.obj         : $(PCHFILE)
$(DESTDIR)\VerInfo.obj           : $(PCHFILE)


$(RESFILE): $(FEDSRC)\Inc\VerRes.rc $(SRCROOT).rc
  @echo Compiling Resources...
#  @$(RC) $(RARGS) /Fo$(RESFILE) $(SRCROOT).rc
   @copy $(SRCROOT).rc $(DESTDIR)
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(SRCROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(SRCROOT).rc
   $(RC) $(RARGS) $(STANDALONEDEFS) /Fo$(DESTDIR)\$(SRCROOT).res $(DESTDIR)\$(SRCROOT).rc

