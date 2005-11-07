#
# Makefile for FedSrv.exe
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

SRCROOT=FedSrv
!if defined(ALLSRV_STANDALONE)
STANDALONEDEFS=/D "ALLSRV_STANDALONE"
DESTROOT=AllSrv32
MAINTARGROOT=AllSrv32
!if !defined(retail) && !defined(sretail) && !defined(ALLSRV_INVISIBLE)
SUBSYSTEM=CONSOLE
STANDALONEDEFS=$(STANDALONEDEFS) /D "_CONSOLE"
!else
SUBSYSTEM=WINDOWS
!endif
!else
STANDALONEDEFS=/D "_CONSOLE"
DESTROOT=FedSrv
MAINTARGROOT=AllSrv
SUBSYSTEM=CONSOLE
!endif
LARGSLOCAL=/ENTRY:mainCRTStartup \
    /delayload:netapi32.dll      \
    /delayload:advapi32.dll
MAINTARGEXT=exe
CARGSLOCAL=/D"FEDSRV" /D _WIN32_WINNT=0x0400 $(STANDALONEDEFS) /D AGC_HOST
INCLUDELOCAL=$(FEDSRC)\ZLib;$(FEDSRC)\_Utility;$(FEDSRC)\Igc;$(DESTDIR)\..\igc;$(FEDSRC)\inc\*.h;$(FEDROOT)\extern\vc\atl\inc;$(FEDSRC)\AGC;$(DESTDIR);$(FEDROOT)\Objs\$(FLAVOR)\AGC;$(FEDSRC)\Test\Inc;$(FEDSRC)\lobby
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\_Utility\*.h $(FEDSRC)\Igc\*.h $(FEDSRC)\inc\*.h $(FEDSRC)\Zone\*.h $(FEDSRC)\lobby\messagesAll.h $(FEDSRC)\lobby\messagesLS.h
MSGFILE=FedMsg
SPLITSYMBOLS=1

IDLTARGS= $(DESTDIR)\AllSrvModuleIDL.h \
          $(DESTDIR)\AllSrvModuleIDL.tlb

!include "..\makefile.inc"


RARGS = /l 0x409 /d"FEDSRV" $(RARGSFLAVOR)

OBJSCNST= $(DESTDIR)\FedSrv.obj   \
#          $(DESTDIR)\SrvDbg.obj  \
          $(DESTDIR)\SqlHelp.obj  \
          $(DESTDIR)\SWMRG.obj    \
          $(DESTDIR)\BitArray.obj \
#          $(DESTDIR)\config.obj \
          $(DESTDIR)\fsship.obj \
          $(DESTDIR)\fsmission.obj \
          $(DESTDIR)\fsside.obj \
          $(DESTDIR)\fscluster.obj \
          $(DESTDIR)\AdminGame.obj \
          $(DESTDIR)\AdminGames.obj \
          $(DESTDIR)\AdminServer.obj \
          $(DESTDIR)\AdminUser.obj \
          $(DESTDIR)\AdminUsers.obj \
          $(DESTDIR)\AdminSession.obj \
          $(DESTDIR)\AdminSessionClass.obj \
          $(DESTDIR)\AdminSessionEventSink.obj \
          $(DESTDIR)\AdminEventLoggerHook.obj \
          $(DESTDIR)\AdminShip.obj \
          $(DESTDIR)\AllSrvModule.obj \
          $(DESTDIR)\fslobby.obj \
          $(DESTDIR)\srvqguids.obj


OBJS=$(OBJSCNST) $(OBJSFLAVOR) 

USERLIBS= \
    $(DESTDIR)\..\_utility\Utility.lib \
    $(DESTDIR)\..\zlib\zlib.lib \
    $(DESTDIR)\..\igc\igc.lib \
    $(DESTDIR)\..\sharemem\sharemem.lib \
    $(DESTDIR)\..\test\TCLib\TCLib.lib \
    $(DESTDIR)\..\agc\AGCGuid.lib 

RESFILE= $(DESTDIR)\FedSrv.res

$(DESTDIR)\AGC.DLL: $(FEDROOT)\objs\$(FLAVOR)\AGC\AGC.DLL
  copy $(FEDROOT)\objs\$(FLAVOR)\AGC\AGC.DLL $(DESTDIR)

AGC.tlb: $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGC.tlb
  @echo Copying AGC.tlb to local directory
  @copy $(FEDROOT)\Objs\$(FLAVOR)\AGC\AGC.tlb .

$(DESTDIR)\AllSrvModuleIDL.h   \
$(DESTDIR)\AllSrvModuleIDL.tlb : AGC.tlb AllSrvModuleIDL.idl ..\inc\AGCIDL.idl
  $(MIDL) $(MIDLARGS) .\AllSrvModuleIDL.idl 2>NUL

OTHEREXEDEPS= $(DESTDIR)\AGC.DLL

!include "../makeexe.inc"

$(PCHFILE)                           : $(MSGFILE)
$(DESTDIR)\FedSrv.obj                : $(PCHFILE) queries.h
#$(DESTDIR)\SrvDbg.obj                : $(PCHFILE)
$(DESTDIR)\SqlHelp.obj               : $(PCHFILE)
$(DESTDIR)\SWMRG.obj                 : $(PCHFILE)
$(DESTDIR)\BitArray.obj              : $(PCHFILE)
#$(DESTDIR)\config.obj                : $(PCHFILE)
$(DESTDIR)\fsship.obj                : $(PCHFILE)
$(DESTDIR)\fsmission.obj             : $(PCHFILE)
$(DESTDIR)\fsside.obj                : $(PCHFILE)
$(DESTDIR)\fscluster.obj             : $(PCHFILE)
$(DESTDIR)\AdminGame.obj             : $(PCHFILE)
$(DESTDIR)\AdminGames.obj            : $(PCHFILE)
$(DESTDIR)\AdminServer.obj           : $(PCHFILE)
$(DESTDIR)\AdminSession.obj          : $(PCHFILE)
$(DESTDIR)\AdminSessionClass.obj     : $(PCHFILE)
$(DESTDIR)\AdminSessionEventSink.obj : $(PCHFILE)
$(DESTDIR)\AdminEventLoggerHook.obj  : $(PCHFILE)
$(DESTDIR)\AdminUser.obj             : $(PCHFILE)
$(DESTDIR)\AdminUsers.obj            : $(PCHFILE)
$(DESTDIR)\AdminShip.obj             : $(PCHFILE)
$(DESTDIR)\AllSrvModule.obj          : $(PCHFILE)
$(DESTDIR)\fslobby.obj               : $(PCHFILE)
$(DESTDIR)\srvqguids.obj             : $(PCHFILE)

$(RESFILE): $(DESTDIR)\FedMsg.rc $(FEDSRC)\Inc\VerRes.rc fedsrv.rc $(DESTDIR)\AllSrvModuleIDL.tlb
   @copy fedsrv.rc $(DESTDIR)\$(MAINTARGROOT).rc
   @type $(DESTDIR)\FedMsg.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   @$(RC) $(RARGS) $(STANDALONEDEFS) /Fo$(RESFILE) $(DESTDIR)\$(MAINTARGROOT).rc

#.cpp{$(DESTDIR)}.obj:
#  $(CC) $(CARGS) $(SRCDIR)\$(@B).cpp

