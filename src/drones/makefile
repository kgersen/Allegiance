#
# Makefile for Drones.lib
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

#  The $(FEDROOT)\Objs tree is created on demand.
#  Since no derived files are placed in the source tree, multiple builds
#  can be run SIMULTANEOUSLY.


MAINTARGROOT=Drones
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDEXT)\vc\mfc\inc;$(FEDSRC)\ZLib;$(FEDSRC)\_Utility;$(FEDSRC)\Igc;$(DESTDIR)\..\igc;$(FEDSRC)\jpeg;$(FEDSRC)\clintlib
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\_Utility\*.h $(FEDSRC)\Igc\*.h
PCHROOT=pch

!include "..\makefile.inc"

OBJS= $(DESTDIR)\drones.obj			\
	  $(DESTDIR)\drone.obj			\
	  $(DESTDIR)\mining.obj			\
	  $(DESTDIR)\turret.obj			\
	  $(DESTDIR)\goal.obj			\
	  $(DESTDIR)\actionlib.obj

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
  lib $(LIBARGS) $(OBJS)

$(DESTDIR)\drones.obj:		$(PCHFILE)
$(DESTDIR)\drone.obj:	    $(PCHFILE)
$(DESTDIR)\mining.obj:	    $(PCHFILE)
$(DESTDIR)\turret.obj:	    $(PCHFILE)
$(DESTDIR)\goal.obj:			$(PCHFILE)
$(DESTDIR)\actionlib.obj:   $(PCHFILE)
