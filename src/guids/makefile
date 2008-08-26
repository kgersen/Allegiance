#
# Makefile for Guids.lib
#
# Environment variable Requirements:
#   
#    INCLUDE=foo           // can be set to anything, must be set
#    PATH=                 // must have proper nmake on path
#    FEDROOT=<path>        // MUST point to base of fed src enlistment
#
# nmake args:
#
#   Clean        // will delete all files in destination dir
#   Full         // will Clean, then build all derived files
#   Shell        // opens os-specific cmd/command shell (helps to debug build)
#   RETAIL=      // builds a retail build (default is DEBUG)
#                // no argument is used for debug builds
#
#
#  All derived files (temp files, .res, output of midl, *.obj, *.exe/.dll...
#  are placed directly into the appropriate tree under $(FEDROOT)\Objs.
#
#  The $(FEDROOT)\Objs tree is created on demand.
#  Since no derived files are placed in the source tree, multiple builds
#  can be run SIMULTANEOUSLY.

SRCROOT=Guids
MAINTARGROOT=FedGuids
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDSRC)\Guids

!include "..\makefile.inc"

OBJS=$(DESTDIR)\guids.obj

$(DESTDIR)\Guid.obj: $(FEDSRC)\$(SRCROOT)\Guids.cpp
  $(CC) $(CARGS) $(FEDSRC)\$(SRCROOT)\$(MAINTARGROOT).cpp

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)
