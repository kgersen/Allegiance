#############################################################################
#
# Makefile for AllGuard.exe
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

SRCROOT=AllGuard
DESTROOT=AllGuard
MAINTARGROOT=AllGuard
MAINTARGEXT=exe
PROJCLEAN=ProjectClean

ARTPATH=$(FEDROOT)\Objs\Artwork
MACHINEEXE=$(ARTPATH)\machine.exe


#############################################################################
# Rules
#

!include "..\makefile.inc"


#############################################################################
#
# Dependencies
#

!if 0
First: $(MAINTARGFULLPATH) $(ARTPATH)\$(MAINTARGROOT).$(MAINTARGEXT) $(MACHINEEXE)

$(ARTPATH):
  @if not exist $(ARTPATH)\. md $(ARTPATH)
!endif

$(MAINTARGFULLPATH): $(DESTDIR) $(DESTDIR)\..\MSRGuard\MSRGuard.exe Allegiance.ini machine.ini
  $(DESTDIR)\..\MSRGuard\MSRGuard.exe /s /ini Allegiance.ini /mini machine.ini /bind $(MAINTARGFULLPATH) | more
  if exist $(DESTDIR)\..\MSRGuard\MSRGuard.sym copy $(DESTDIR)\..\MSRGuard\MSRGuard.sym $(DESTDIR)\AllGuard.*
  if exist $(DESTDIR)\..\MSRGuard\MSRGuard.pdb copy $(DESTDIR)\..\MSRGuard\MSRGuard.pdb $(DESTDIR)\AllGuard.*
  if exist $(DESTDIR)\..\MSRGuard\MSRGuard.dbg copy $(DESTDIR)\..\MSRGuard\MSRGuard.dbg $(DESTDIR)\AllGuard.*
  if exist $(DESTDIR)\..\MSRGuard\MSRGuard.map copy $(DESTDIR)\..\MSRGuard\MSRGuard.map $(DESTDIR)\AllGuard.*

!if 0
$(ARTPATH)\$(MAINTARGROOT).$(MAINTARGEXT): $(MAINTARGFULLPATH) $(ARTPATH)
  @echo Copying $(MAINTARGFULLPATH) to $(ARTPATH)\$(MAINTARGROOT).$(MAINTARGEXT)
  copy /Y $(MAINTARGFULLPATH) $(ARTPATH)\$(MAINTARGROOT).$(MAINTARGEXT)

$(MACHINEEXE): $(FEDROOT)\src\tools\build\machine.exe $(ARTPATH)
  @echo Copying $(FEDROOT)\src\tools\build\machine.exe to $(ARTPATH)
  copy /Y $(FEDROOT)\src\tools\build\machine.exe $(MACHINEEXE)
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

