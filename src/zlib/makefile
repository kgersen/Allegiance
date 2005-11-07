#
# Makefile for ZLib.lib
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


MAINTARGROOT=ZLib
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDSRC)\ZLib
PCHROOT=pch
NODEPENDENCIES=1
UNWINDCARG=/GX-

!include "..\makefile.inc"

OBJS= $(DESTDIR)\zlib.obj         \
      $(DESTDIR)\ztime.obj         \
      $(DESTDIR)\token.obj        \
      $(DESTDIR)\zstring.obj      \
      $(DESTDIR)\window.obj       \
!IFDEF DREAMCAST
      $(DESTDIR)\dcbase.obj         \
!ELSE
#      $(DESTDIR)\dialog.obj       \
      $(DESTDIR)\base.obj         \
!ENDIF
      $(DESTDIR)\win32app.obj     \
      $(DESTDIR)\tlist.obj        \
      $(DESTDIR)\matrix.obj       \
      $(DESTDIR)\matrix2.obj      \
      $(DESTDIR)\orientation.obj  \
      $(DESTDIR)\quaternion.obj   \
      $(DESTDIR)\color.obj        \
      $(DESTDIR)\event.obj        \
      $(DESTDIR)\input.obj        \
      $(DESTDIR)\vector.obj       \
      $(DESTDIR)\VersionInfo.obj  \
      $(DESTDIR)\FTPSession.obj   \
      $(DESTDIR)\pch.obj

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    $(LIB) $(LIBARGS) $(OBJS)

$(DESTDIR)\zlib.obj:           $(PCHFILE)
$(DESTDIR)\ztime.obj:           $(PCHFILE)
$(DESTDIR)\token.obj:          $(PCHFILE)
$(DESTDIR)\zstring.obj:        $(PCHFILE)
$(DESTDIR)\tlist.obj:          $(PCHFILE)
$(DESTDIR)\window.obj:         $(PCHFILE)
$(DESTDIR)\dialog.obj:         $(PCHFILE)
$(DESTDIR)\win32app.obj:       $(PCHFILE)
$(DESTDIR)\matrix.obj:         $(PCHFILE)
$(DESTDIR)\matrix2.obj:        $(PCHFILE)
$(DESTDIR)\orientation.obj:    $(PCHFILE)
$(DESTDIR)\quaternion.obj:     $(PCHFILE)
$(DESTDIR)\color.obj:          $(PCHFILE)
$(DESTDIR)\event.obj:          $(PCHFILE)
$(DESTDIR)\base.obj:           $(PCHFILE)
$(DESTDIR)\input.obj:          $(PCHFILE)
$(DESTDIR)\VersionInfo.obj:    $(PCHFILE)
$(DESTDIR)\FTPSession.obj:     $(PCHFILE)
