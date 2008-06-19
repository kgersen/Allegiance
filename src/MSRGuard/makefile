#############################################################################
#
# Makefile for MSRGuard.exe
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

# keep the debug info in the exe for retail builds, so we can strip it out and leave the FPO info

SRCROOT=MSRGuard
DESTROOT=MSRGuard
MAINTARGROOT=MSRGuard
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\vc\atl\inc;$(FEDSRC)\Test\Inc;$(FEDSRC)\Inc;$(FEDSRC)\Zlib
PCHROOT=pch
PCHINC=
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
CARGSLOCAL=/D"MSRGuard" /D _WIN32_WINNT=0x0400 $(STANDALONEDEFS)
UNWINDCARG=/EHsc

#############################################################################
# Rules
#

!include "..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS = /l 0x409 /d"MSRGuard" $(RARGSFLAVOR)

OBJS= $(DESTDIR)\MSRGuard.obj             \
      $(DESTDIR)\AppModeAttach.obj        \
      $(DESTDIR)\AppModeBind.obj          \
      $(DESTDIR)\AppModeCreate.obj        \
      $(DESTDIR)\AppModeHelp.obj          \
      $(DESTDIR)\LoadedModules.obj

LIBS= $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib                                     \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib    \
      version.lib netapi32.lib delayimp.lib comsupp.lib dbghelp.lib psapi.lib

USERLIBS= \
    $(DESTDIR)\..\zlib\zlib.lib         \
    $(DESTDIR)\..\test\tclib\tclib.lib  \
      

RESFILE= $(DESTDIR)\MSRGuard.res

LARGS=/LIBPATH:$(DXROOT)\lib \
      /LIBPATH:$(FEDEXT)\Vc\lib \
      /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:WINDOWS \
      $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO) \
      /MACHINE:I386 /ENTRY:mainCRTStartup \
      /DELAYLOAD:PSAPI.dll


#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH): $(DESTDIR) $(MSGFILE) $(OBJS) $(RESFILE) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS) $(DESTDIR)\$(PCHROOT).obj
#  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map 2> NUL

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\MSRGuard.obj           : $(PCHFILE)
$(DESTDIR)\AppModeAttach.obj      : $(PCHFILE)
$(DESTDIR)\AppModeBind.obj        : $(PCHFILE)
$(DESTDIR)\AppModeCreate.obj      : $(PCHFILE)
$(DESTDIR)\AppModeHelp.obj        : $(PCHFILE)
$(DESTDIR)\LoadedModules.obj      : $(PCHFILE)


$(RESFILE): $(FEDSRC)\Inc\VerRes.rc $(SRCROOT).rc *.ini *.htm $(FEDSRC)\setup\Machine.ini
  @echo Compiling Resources...
   @copy $(SRCROOT).rc $(DESTDIR)
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(SRCROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(SRCROOT).rc
   $(RC) $(RARGS) $(STANDALONEDEFS) /Fo$(DESTDIR)\$(SRCROOT).res $(DESTDIR)\$(SRCROOT).rc

