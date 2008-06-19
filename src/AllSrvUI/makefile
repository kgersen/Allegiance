#############################################################################
#
# Makefile for AllSrvUI.exe
#
# Environment variable Requirements:
#   
#    INCLUDE=foo           // can be set to anything, must be set
#    PATH=                 // must have proper nmake on path
#    FEDROOT=<path>        // MUST point to base of fed src enlistment
#    BCBINDIR=<path>       // optional, unless using BCHK=
#    TTBINDIR=<path>       // optional, unless using TRUETIME=
#
# nmake args:
#
#   Clean        // will delete all files in destination dir
#   Full         // will Clean, then build all derived files
#   debug=       // builds a debug build (default)
#   retail=      // builds a retail build
#   test=        // retail but with the _DEBUG preprocessor symbol defined
#                // NOT valid in combination with BCHK=, TRUETIME=, or ICAP=
#   VER=         // must have this argument, \\msr\federation\builds\$(VER) is created/needed
#   BCHK=        // if you want to do a boundschecker build.
#                   note:  you must have BC already installed locally and define BCBINDIR to
#                          point to the directory where nmcl.exe and nmlink.exe reside.
#                          You can use the BCOPTS variable to add options to the BC compile.
#   TRUETIME=    // if you want to instrument the build using Numega TrueTime
#                   note:  you must have TrueTime already installed locally and define TTBINDIR to
#                          point to the directory where the TrueTime nmcl.exe and nmlink.exe reside.  
#                          You can use the TTOPTS variable to add options to the TT compile.
#   VERBOSE=     // if you want verbose build mode. (default is non-verbose)
#
#  All derived files (temp files, .res, output of midl, *.obj, *.exe/.dll...
#  are placed directly into the appropriate tree under $(FEDROOT)\Objs.
#
#  The $(FEDROOT)\Objs tree is created on demand.
#  Since no derived files are placed in the source tree, multiple builds
#  can be run SIMULTANEOUSLY.
#


#############################################################################
# Environment Variables
#


SRCROOT=AllSrvUI
!if defined(ALLSRV_STANDALONE)
DESTROOT=AllSrv32UI
MAINTARGROOT=AllSrv32UI
STANDALONEDEFS=/D "ALLSRV_STANDALONE"
SERVERDEST=$(FEDROOT)\Objs\$(FLAVOR)\AllSrv
!else
STANDALONEDEFS=
DESTROOT=AllSrvUI
MAINTARGROOT=AllSrvUI
SERVERDEST=$(FEDROOT)\Objs\$(FLAVOR)\FedSrv
!endif

MAINTARGEXT=exe
INCLUDELOCAL=$(FEDEXT)\vc\mfc\inc;$(FEDEXT)\vc\atl\inc;$(FEDSRC)\Test\Inc;$(FEDSRC)\Inc;$(FEDSRC)\AGC;$(FEDROOT)\Objs\$(FLAVOR)\Test\TCObj;$(FEDROOT)\Objs\$(FLAVOR)\Allsrv;$(FEDROOT)\Objs\$(FLAVOR)\FedSrv;$(FEDROOT)\Objs\$(FLAVOR)\AGC;$(SERVERDEST);$(FEDEXT)\sbn\include;$(FEDSRC)\Zlib
PCHROOT=stdafx
PCHINC=*.h
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
CARGSLOCAL=/D"AllSrvUI" /D _AFXDLL $(STANDALONEDEFS)
UNWINDCARG=/EHsc

#############################################################################
# Rules
#

!include "..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS = /l 0x409 /d"AllSrvUI" $(RARGSFLAVOR)

OBJS= $(DESTDIR)\AllSrvUI.obj             \
      $(DESTDIR)\AllSrvUISheet.obj        \
        $(DESTDIR)\DlgAbout.obj           \
        $(DESTDIR)\DlgGameSelect.obj      \
      $(DESTDIR)\PageDummy.obj            \
      $(DESTDIR)\PageConnect.obj          \
      $(DESTDIR)\PageGameCreate.obj       \
        $(DESTDIR)\GameParamsSheet.obj    \
        $(DESTDIR)\GamePageTeam.obj       \
        $(DESTDIR)\GamePageWin.obj        \
        $(DESTDIR)\GamePageSpecifics.obj  \
      $(DESTDIR)\PageChat.obj             \
        $(DESTDIR)\AdminPageThreadWnd.obj \
        $(DESTDIR)\DlgChatPreferences.obj \
      $(DESTDIR)\PagePlayers.obj          \
      $(DESTDIR)\AutoSizer.obj


LIBS= $(MFCLIBSFLAVOR) $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib            \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib     \
      version.lib netapi32.lib urlmon.lib delayimp.lib ws2_32.lib dplayx.lib \
      wininet.lib comsupp.lib comsuppw.lib atlsd.lib mfc90d.lib mfcs90d.lib

USERLIBS= \
    $(DESTDIR)\..\zlib\zlib.lib         \
    $(DESTDIR)\..\_Utility\Utility.lib  \
    $(DESTDIR)\..\AGC\AGCGuid.lib
      

RESFILE= $(DESTDIR)\AllSrvUI.res

LARGS=/LIBPATH:$(DXROOT)\lib \
      /LIBPATH:$(FEDEXT)\Vc\lib \
      /LIBPATH:$(FEDEXT)\Vc\mfc\lib \
      /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:WINDOWS \
      $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO) \
      /MACHINE:I386


#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH): $(DESTDIR) $(MSGFILE) $(OBJS) $(RESFILE) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS) $(DESTDIR)\$(PCHROOT).obj
#  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\$(PCHROOT).obj         : $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGC.tlb \
                                    $(SERVERDEST)\AllSrvModuleIDL.tlb
$(DESTDIR)\AllSrvUI.obj           : $(PCHFILE)       \
                                    AllSrvUISheet.h  \
                                    PageGameCreate.h \
                                    PageChat.h
$(DESTDIR)\AllSrvUISheet.obj      : $(PCHFILE)       \
                                    PageGameCreate.h \
                                    PageChat.h
$(DESTDIR)\DlgAbout.obj           : $(PCHFILE)
$(DESTDIR)\DlgGameSelect.obj      : $(PCHFILE)
$(DESTDIR)\PageDummy.obj          : $(PCHFILE)
$(DESTDIR)\PageConnect.obj        : $(PCHFILE)
$(DESTDIR)\PageGameCreate.obj     : $(PCHFILE)
$(DESTDIR)\GameParamsSheet.obj    : $(PCHFILE)
$(DESTDIR)\GamePageTeam.obj       : $(PCHFILE)
$(DESTDIR)\GamePageWin.obj        : $(PCHFILE)
$(DESTDIR)\GamePageSpecifics.obj  : $(PCHFILE)
$(DESTDIR)\PageChat.obj           : $(PCHFILE)
$(DESTDIR)\AdminPageThreadWnd.obj : $(PCHFILE)
$(DESTDIR)\DlgChatPreferences.obj : $(PCHFILE)
$(DESTDIR)\PagePlayers.obj        : $(PCHFILE)
$(DESTDIR)\AutoSizer.obj          : $(PCHFILE)

$(RESFILE): $(FEDSRC)\Inc\VerRes.rc $(SRCROOT).rc
  @echo Compiling Resources...
   @copy $(SRCROOT).rc $(DESTDIR)
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(SRCROOT).rc
#   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(SRCROOT).rc
   $(RC) $(RARGS) $(STANDALONEDEFS) /Fo$(DESTDIR)\$(SRCROOT).res $(DESTDIR)\$(SRCROOT).rc

