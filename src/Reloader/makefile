#############################################################################
#
# Makefile for Reloader.exe
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

MAINTARGROOT=Reloader
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\vc\inc
PCHROOT=pch
PCHINC=
PCHINCLOCAL=
PROJCLEAN=ProjectClean
CARGSLOCAL=/D"Reloader" /D _WIN32_WINNT=0x0400


#############################################################################
# Rules
#

!include "..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS = /l 0x409 /d"Reloader" $(RARGSFLAVOR)

OBJS= $(DESTDIR)\Reloader.obj     

LIBS= $(MFCLIBSFLAVOR) $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib \
      odbccp32.lib 

USERLIBS= \
      

LARGS= \
!ifdef icap
       /LIBPATH:$(FEDEXT)\icecap4\lib       \
!endif
      /LIBPATH:$(DXROOT)\lib \
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
!ifndef NOMAP
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map
!endif

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\Reloader.obj    : $(PCHFILE)
