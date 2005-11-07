#
# Makefile for jpeg.lib
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


MAINTARGROOT=jpeg
MAINTARGEXT=lib

!include "..\makefile.inc"

OBJS= $(DESTDIR)\decom.obj        \
      $(DESTDIR)\jcomapi.obj      \
      $(DESTDIR)\jdapimin.obj     \
      $(DESTDIR)\jdapistd.obj     \
      $(DESTDIR)\jdcoefct.obj     \
      $(DESTDIR)\jdcolor.obj      \
      $(DESTDIR)\jddctmgr.obj     \
      $(DESTDIR)\jdhuff.obj       \
      $(DESTDIR)\jdinput.obj      \
      $(DESTDIR)\jdmainct.obj     \
      $(DESTDIR)\jdmarker.obj     \
      $(DESTDIR)\jdmaster.obj     \
      $(DESTDIR)\jdpostct.obj     \
      $(DESTDIR)\jdsample.obj     \
      $(DESTDIR)\jerror.obj       \
      $(DESTDIR)\jidctflt.obj     \
      $(DESTDIR)\jmemmgr.obj      \
      $(DESTDIR)\jmemnobs.obj     \
      $(DESTDIR)\jpegd.obj        \
      $(DESTDIR)\jutils.obj

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
  lib $(LIBARGS) $(OBJS)

.cpp{$(DESTDIR)}.obj: 
  $(CC) $(CARGS) $(SRCDIR)\$(@B).cpp

.c{$(DESTDIR)}.obj: 
  $(CC) $(CARGS) $(@B).c
