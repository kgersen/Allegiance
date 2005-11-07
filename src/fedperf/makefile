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


MAINTARGROOT=fedperf
MAINTARGEXT=dll
INCLUDELOCAL=
PCHINC=

!include "..\makefile.inc"

OBJS= $(DESTDIR)\cntrmap.obj    \
    $(DESTDIR)\collect.obj      \
    $(DESTDIR)\perfdll.obj      \
    $(DESTDIR)\perfutil.obj     \
    $(DESTDIR)\pch.obj
    
LIBS= \
        $(LIBSFLAVOR)   \
        advapi32.lib    \
        kernel32.lib    \
        user32.lib

USERLIBS= \
    $(DESTDIR)\..\sharemem\sharemem.lib

DLLFLAGS=/DLL /DEF:$(MAINTARGROOT).def
LOCALARGS= $(NOLOGO) /pdb:$(PDB) /machine:I386 /nodefaultlib \
            /LIBPATH:$(FEDEXT)\Vc\Lib \
            /out:$(DESTDIR)\$(MAINTARGROOT).dll \

LARGS=$(LARGSFLAVOR) $(LOCALARGS) $(DLLFLAGS)

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    $(LINK) $(LARGS) $(LIBS) $(USERLIBS) $(OBJS)

$(DESTDIR)\cntrmap.obj:         $(PCHFILE)
$(DESTDIR)\collect.obj:         $(PCHFILE)
$(DESTDIR)\perfdll.obj:         $(PCHFILE)
$(DESTDIR)\perfutil.obj:        $(PCHFILE)

