#############################################################################
#
# Makefile for TCLib.lib
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

SRCROOT=Test\TCLib
MAINTARGROOT=TCLib
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDSRC)\Test\TCLib;$(FEDSRC)\Test\Inc;$(FEDSRC)\ZLib
PCHROOT=pch
PCHINC=
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
UNWINDCARG=/EHsc


#############################################################################
# Rules
#

!include "..\..\makefile.inc"


#############################################################################
# Build Targets
#

OBJS= $(DESTDIR)\TCLib.obj             \
      $(DESTDIR)\DynaLib.obj           \
      $(DESTDIR)\ExcepInfo.obj         \
      $(DESTDIR)\InvokeArgs.obj        \
      $(DESTDIR)\PropertyPageSite.obj  \
      $(DESTDIR)\RelPath.obj           \
      $(DESTDIR)\StrManip.obj          \
      $(DESTDIR)\TCThread.obj          \
      $(DESTDIR)\UtilImpl.obj          \
      $(DESTDIR)\UtilityThread.obj     \
      $(DESTDIR)\WorkerThread.obj      \
      $(DESTDIR)\pch.obj
      
      

#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    $(LIB) $(LIBARGS) $(OBJS)

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!ENDIF

$(DESTDIR)\TCLib.obj             : $(PCHFILE)
$(DESTDIR)\DynaLib.obj           : $(PCHFILE)
$(DESTDIR)\ExcepInfo.obj         : $(PCHFILE)
$(DESTDIR)\InvokeArgs.obj        : $(PCHFILE)
$(DESTDIR)\PropertyPageSite.obj  : $(PCHFILE)
$(DESTDIR)\RelPath.obj           : $(PCHFILE)
$(DESTDIR)\StrManip.obj          : $(PCHFILE)
$(DESTDIR)\TCThread.obj          : $(PCHFILE)
$(DESTDIR)\UtilImpl.obj          : $(PCHFILE)
$(DESTDIR)\UtilityThread.obj     : $(PCHFILE)
$(DESTDIR)\WorkerThread.obj      : $(PCHFILE)

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

