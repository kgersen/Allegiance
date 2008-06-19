#
# Makefile for sound.lib
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


MAINTARGROOT=soundengine
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDSRC)\ZLib;$(FEDSRC)\Inc;
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h
UNWINDCARG=/EHsc

!include "..\makefile.inc"

OBJS= $(DESTDIR)\ds3dutil.obj            \
      $(DESTDIR)\ds3dbuffer.obj          \
      $(DESTDIR)\ds3dvirtualbuffer.obj   \
      $(DESTDIR)\ds3dengine.obj          \
      $(DESTDIR)\soundbase.obj           \
      $(DESTDIR)\soundutil.obj           \
      $(DESTDIR)\soundtemplates.obj      \
      $(DESTDIR)\dummysound.obj          \
      $(DESTDIR)\redbooksound.obj        \
      $(DESTDIR)\pch.obj

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)

$(DESTDIR)\ds3dutil.obj:           $(PCHFILE) soundbase.h ds3dutil.h 
$(DESTDIR)\ds3dbuffer.obj:         $(PCHFILE) soundbase.h ds3dutil.h ds3dbuffer.h 
$(DESTDIR)\ds3dvirtualbuffer.obj:  $(PCHFILE) soundbase.h ds3dutil.h ds3dbuffer.h ds3dvirtualbuffer.h
$(DESTDIR)\ds3dengine.obj:         $(PCHFILE) soundbase.h ds3dutil.h ds3dbuffer.h ds3dvirtualbuffer.h ds3dengine.h
$(DESTDIR)\soundbase.obj:          $(PCHFILE) soundbase.h ds3dutil.h ds3dbuffer.h ds3dvirtualbuffer.h ds3dengine.h
$(DESTDIR)\soundutil.obj:          $(PCHFILE) soundbase.h soundutil.h
$(DESTDIR)\soundtemplates.obj:     $(PCHFILE) soundbase.h soundutil.h soundtemplates.h
$(DESTDIR)\redbooksound.obj:       $(PCHFILE) soundbase.h redbooksound.h
$(DESTDIR)\dummysound.obj:         $(PCHFILE) soundbase.h
