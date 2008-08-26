#
# Makefile for Sentinal.exe
#
# Environment variable Requirements:
#   
#    INCLUDE=foo           // can be set to anything, must be set
#    PATH=                 // must have proper nmake on path
#    FEDROOT=<path>        // MUST point to base of fed src enlistment
#
# nmake args:              // order of arguments is not relevent
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

#	use _BCHK=1 if you want to do a boundschecker build.
#   note:  you must have BC already installed locally and define BCBINDIR to
#          point to the directory where nmcl.exe and nmlink.exe reside.  You 
#          can use the BCOPTS variable to add options to the BC compile.

MAINTARGROOT=sentinal
MAINTARGEXT=exe
CARGSLOCAL=/D"SENTINAL" /D _WIN32_WINNT=0x0400
INCLUDELOCAL=$(FEDSRC)\ZLib;$(FEDSRC)\_Utility;$(FEDSRC)\Igc;$(DESTDIR)\..\igc;$(DESTDIR)
MSGFILE=sentmsg

!include "..\makefile.inc"

RARGS = /l 0x409 /d"SENTINAL" $(RARGSFLAVOR)

OBJSCNST=\
    $(DESTDIR)\config.obj  \
    $(DESTDIR)\dplaychk.obj  \
    $(DESTDIR)\sentinal.obj \
    $(DESTDIR)\srvdbg.obj

OBJS=$(OBJSCNST) $(OBJSFLAVOR)

LIBS= $(LIBSFLAVOR) \
     dplayx.lib kernel32.lib user32.lib gdi32.lib winspool.lib \
     comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib \
     uuid.lib odbc32.lib odbccp32.lib wsock32.lib comctl32.lib \
     urlmon.lib ddraw.lib d3drm.lib winmm.lib

USERLIBS= \
    $(DESTDIR)\..\_utility\Utility.lib \
    $(DESTDIR)\..\zlib\zlib.lib 

RESFILE= $(DESTDIR)\Sentinal.res

LARGS=/LIBPATH:$(DXROOT)\lib /LIBPATH:$(FEDEXT)\Vc\lib \
      /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE \
      $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO) \
      /MACHINE:I386

$(MAINTARGFULLPATH): $(DESTDIR) $(MSGFILE) $(OBJS) $(RESFILE) $(BSCTARG) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS)
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map

$(RESFILE): $(DESTDIR)\SentMsg.rc $(FEDSRC)\Inc\VerRes.rc $(MAINTARGROOT).rc
   @copy $(FEDSRC)\Sentinal\Sentinal.rc $(DESTDIR)
   @type $(DESTDIR)\SentMsg.rc >>$(DESTDIR)\Sentinal.rc
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\Sentinal.rc
   $(RC) $(RARGS) /Fo$(RESFILE) $(DESTDIR)\Sentinal.rc

.cpp{$(DESTDIR)}.obj:
  $(CC) $(CARGS) $(SRCDIR)\$(@B).cpp

.c{$(DESTDIR)}.obj:
  $(CC) $(CARGS) $(@B).c

$(BSC): $(DESTDIR)\*.sbr
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr

