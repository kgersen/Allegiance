#
# Makefile for Lobby.exe
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

#   use _BCHK=1 if you want to do a boundschecker build.
#   note:  you must have BC already installed locally and define BCBINDIR to
#          point to the directory where nmcl.exe and nmlink.exe reside.  You 
#          can use the BCOPTS variable to add options to the BC compile.

SRCROOT=Lobby
MAINTARGROOT=AllLobby
MAINTARGEXT=exe
SUBSYSTEM=CONSOLE
CARGSLOCAL=/D"LOBBY" /D _WIN32_WINNT=0x0400 /D _ATL_STATIC_REGISTRY
INCLUDELOCAL=$(FEDSRC)\ZLib;$(FEDSRC)\_Utility;$(FEDSRC)\inc\*.h;$(FEDROOT)\extern\vc\atl\inc;$(FEDROOT)\src\test\inc;$(DESTDIR);$(FEDSRC)\Inc
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\_Utility\*.h $(FEDSRC)\inc\*.h $(FEDSRC)\Zone\*.h $(FEDSRC)\lobby\*.h
MSGFILE=LobbyMsg
UNWINDCARG=/EHsc

#IDLOUTPUT=LobbyModuleIDL

!include "..\makefile.inc"

RARGS = /l 0x409 /d"LOBBY" $(RARGSFLAVOR)

OBJSCNST= $(DESTDIR)\lobby.obj  \
           $(DESTDIR)\lobbyapp.obj \
           $(DESTDIR)\server.obj \
           $(DESTDIR)\client.obj \
           $(DESTDIR)\mission.obj \
           $(DESTDIR)\AutoUpdate.obj \
           $(DESTDIR)\broadcast.obj \
           $(DESTDIR)\zgameinfo.obj \
           $(DESTDIR)\LobbyEvents.obj \
           $(DESTDIR)\LobQGuids.obj

OBJS=$(OBJSCNST) $(OBJSFLAVOR) 

LIBS= $(LIBSFLAVOR) \
     dplayx.lib kernel32.lib user32.lib gdi32.lib winspool.lib \
     comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib \
     uuid.lib odbc32.lib odbccp32.lib ws2_32.lib comctl32.lib \
     urlmon.lib winmm.lib version.lib zticket.lib zticketc.lib

USERLIBS= \
    $(DESTDIR)\..\_utility\Utility.lib \
    $(DESTDIR)\..\zlib\zlib.lib \
    $(DESTDIR)\..\test\TCLib\TCLib.lib \
    $(DESTDIR)\..\sharemem\sharemem.lib

RESFILE= $(DESTDIR)\$(MAINTARGROOT).res

DLLFLAGS = /def:lobby.def

LARGS=/LIBPATH:$(DXROOT)\lib \
      /LIBPATH:$(FEDEXT)\Vc\lib \
      /LIBPATH:$(FEDEXT)\Zone \
      /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE \
      $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO) \
      /MACHINE:I386

!include "../makeexe.inc"

$(DESTDIR)\Lobby.obj       : $(PCHFILE)
$(DESTDIR)\LobbyApp.obj    : $(PCHFILE)
$(DESTDIR)\Server.obj      : $(PCHFILE)
$(DESTDIR)\Client.obj      : $(PCHFILE)
$(DESTDIR)\Mission.obj     : $(PCHFILE)
$(DESTDIR)\AutoUpdate.obj  : $(PCHFILE)
$(DESTDIR)\broadcast.obj   : $(PCHFILE)
$(DESTDIR)\zgameinfo.obj   : $(PCHFILE)
$(DESTDIR)\LobbyEvents.obj : $(PCHFILE)
$(DESTDIR)\LobQGuids.obj   : $(PCHFILE)

$(RESFILE): $(DESTDIR)\$(MSGFILE).rc $(FEDSRC)\Inc\VerRes.rc $(MAINTARGROOT).rc
   @copy $(FEDSRC)\$(SRCROOT)\$(MAINTARGROOT).rc $(DESTDIR)
   @type $(DESTDIR)\$(MSGFILE).rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
#   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   @$(RC) $(RARGS) /Fo$(RESFILE) $(DESTDIR)\$(MAINTARGROOT).rc

