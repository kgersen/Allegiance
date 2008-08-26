#
# Makefile for Engine.lib
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


MAINTARGROOT=Effect
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib
PCHINC=$(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h
PCHROOT=pch
NODEPENDENCIES=1

!include "..\makefile.inc"

OBJS= \
      $(DESTDIR)\ConeGeo.obj       \
      $(DESTDIR)\comboPane.obj     \
      $(DESTDIR)\ThingGeo.obj      \
      $(DESTDIR)\explosion.obj     \
      $(DESTDIR)\starimage.obj     \
      $(DESTDIR)\debrisgeo.obj     \
      $(DESTDIR)\alephgeo.obj      \
      $(DESTDIR)\particlegeo.obj   \
      $(DESTDIR)\efgeo.obj         \
      $(DESTDIR)\efimage.obj       \
      $(DESTDIR)\efpopup.obj       \
      $(DESTDIR)\efapp.obj         \
      $(DESTDIR)\efwindow.obj      \
      $(DESTDIR)\efpane.obj        \
      $(DESTDIR)\mmlpopup.obj      \
      $(DESTDIR)\gamestate.obj     \
      $(DESTDIR)\framedata.obj     \
      $(DESTDIR)\help.obj          \
      $(DESTDIR)\pagepane.obj      \
      $(DESTDIR)\navpane.obj       \
      $(DESTDIR)\ListPane.obj

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)

$(DESTDIR)\ConeGeo.obj:        $(PCHFILE)
$(DESTDIR)\ThingGeo.obj:       $(PCHFILE)
$(DESTDIR)\starimage.obj:      $(PCHFILE)
$(DESTDIR)\debrisgeo.obj:      $(PCHFILE)
$(DESTDIR)\alephgeo.obj:       $(PCHFILE)
$(DESTDIR)\particlegeo.obj:    $(PCHFILE)
$(DESTDIR)\efgeo.obj:          $(PCHFILE)
$(DESTDIR)\efimage.obj:        $(PCHFILE)
$(DESTDIR)\efpopup.obj:        $(PCHFILE)
$(DESTDIR)\efapp.obj:          $(PCHFILE)
$(DESTDIR)\efwindow.obj:       $(PCHFILE)
$(DESTDIR)\efpane.obj:         $(PCHFILE)
$(DESTDIR)\mmlpopup.obj:       $(PCHFILE)
$(DESTDIR)\gamestate.obj:      $(PCHFILE)
$(DESTDIR)\framedata.obj:      $(PCHFILE)
$(DESTDIR)\listpane.obj:       $(PCHFILE)
$(DESTDIR)\combopane.obj:      $(PCHFILE)
$(DESTDIR)\explosion.obj:      $(PCHFILE)
$(DESTDIR)\help.obj:           $(PCHFILE)
$(DESTDIR)\pagepane.obj:       $(PCHFILE)
$(DESTDIR)\navpane.obj:        $(PCHFILE)
