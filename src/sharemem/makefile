#
# Makefile for sharemem
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


MAINTARGROOT=sharemem
MAINTARGEXT=lib
CARGSLOCAL=/D _CRT_NON_CONFORMING_SWPRINTFS

!include "..\makefile.inc"

OBJS= $(DESTDIR)\sharemem.obj   \
        $(DESTDIR)\pch.obj

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LFLAVORARGS) $(LIBARGS) $(OBJS)

$(DESTDIR)\sharemem.obj:            $(PCHFILE)

