#
# Makefile for IGC.lib
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


SRCROOT=clintlib
MAINTARGROOT=clintlib
INCLUDELOCAL=$(FEDSRC)\igc;$(FEDSRC)\Utility;$(FEDSRC)\ZLib;$(FEDSRC)\WinTrek;$(DESTDIR);$(FEDSRC)\jpeg;$(FEDSRC)\clintlib;$(FEDSRC)\Zone;$(FEDSRC)\training;$(FEDSRC)\lobby;$(FEDSRC)\engine
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\_Utility\*.h $(FEDSRC)\igc\*.h $(FEDSRC)\clintlib\*.h $(FEDSRC)\inc\*.h $(FEDSRC)\Zone\*.h $(FEDSRC)\lobby\messageslc.h $(FEDSRC)\lobby\messagesall.h $(FEDSRC)\club\clubmessages.h $(FEDSRC)\guids\*.h
PCHROOT=pch

MAINTARGEXT=lib

!include "..\makefile.inc"

OBJS= $(DESTDIR)\appmsg.obj \
      $(DESTDIR)\clintlib.obj \
      $(DESTDIR)\badwords.obj \
      $(DESTDIR)\AutoDownload.obj \
      $(DESTDIR)\pch.obj

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)


#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)

$(DESTDIR)\appmsg.obj:          $(PCHFILE)
$(DESTDIR)\clintlib.obj:        $(PCHFILE)
$(DESTDIR)\badwords.obj:        $(PCHFILE)
$(DESTDIR)\AutoDownload.obj:    $(PCHFILE)

