#############################################################################
#
# Makefile for AGC.dll
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

SRCROOT=AGC
MAINTARGROOT=AGC
MAINTARGEXT=dll
INCLUDELOCAL=$(FEDSRC)\ZLib;$(FEDSRC)\Igc;$(FEDSRC)\Test\Inc;$(DESTDIR);$(FEDEXT)\dx6\include;$(FEDEXT)\Zone;$(FEDEXT)\sbn\include;$(FEDSRC)\Inc
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\Igc\*.h
PCHINCLOCAL=
PROJCLEAN=ProjectClean
MIDLINCLOCAL=/I ..\test\Inc /I ..\Inc
CARGSLOCAL=/D _ATL_STATIC_REGISTRY /D _WINDLL /D _USRDLL /EHsc




#############################################################################
# Rules
#
!include "..\makefile.inc"

#############################################################################
# Build Targets
#

RARGS=/l 0x409 $(RARGSFLAVOR)

IDLTARGS= $(DESTDIR)\AGCIDL.h      \
          $(DESTDIR)\AGC.h         \
          $(DESTDIR)\AGC.tlb

OBJS= $(DESTDIR)\AGC.obj                \
      $(DESTDIR)\AGCGlobal.obj          \
      $(DESTDIR)\AGCVector.obj          \
      $(DESTDIR)\AGCOrientation.obj     \
      $(DESTDIR)\AGCEvent.obj           \
      $(DESTDIR)\AGCEventData.obj       \
      $(DESTDIR)\AGCEventDef.obj        \
      $(DESTDIR)\AGCEventLogger.obj     \
      $(DESTDIR)\AGCEventIDRange.obj    \
      $(DESTDIR)\AGCEventIDRanges.obj   \
      $(DESTDIR)\AGCCommand.obj         \
      $(DESTDIR)\AGCVersionInfo.obj     \
      $(DESTDIR)\AGCDBParams.obj        \
      $(DESTDIR)\AGCGameParameters.obj  \
      $(DESTDIR)\TCNullStream.obj       \
      $(DESTDIR)\TCUtility.obj          \
      $(DESTDIR)\TCMarshalByValue.obj   \
      $(DESTDIR)\TCStrings.obj          \
      $(DESTDIR)\AdminSessionHelper.obj \
      $(DESTDIR)\AGCHullType.obj        \
      $(DESTDIR)\AGCModel.obj           \
      $(DESTDIR)\AGCModels.obj          \
      $(DESTDIR)\AGCProbe.obj           \
      $(DESTDIR)\AGCProbes.obj          \
      $(DESTDIR)\AGCGame.obj            \
      $(DESTDIR)\AGCShip.obj            \
      $(DESTDIR)\AGCShips.obj           \
      $(DESTDIR)\AGCStation.obj         \
      $(DESTDIR)\AGCStations.obj        \
      $(DESTDIR)\AGCAleph.obj           \
      $(DESTDIR)\AGCAlephs.obj          \
      $(DESTDIR)\AGCSector.obj          \
      $(DESTDIR)\AGCSectors.obj         \
      $(DESTDIR)\AGCTeam.obj            \
      $(DESTDIR)\AGCTeams.obj           \
      $(DESTDIR)\AGCAsteroid.obj        \
      $(DESTDIR)\AGCAsteroids.obj       \
      $(DESTDIR)\pch.obj


LIBS= $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib        \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib \
      version.lib delayimp.lib ws2_32.lib dplayx.lib  \
      dxguid.lib netapi32.lib oledb.lib comsvcs.lib \
      $(MFCLIBSFLAVOR)

USERLIBS= $(DESTDIR)\AGCGuid.lib                            \
          $(FEDROOT)\Objs\$(FLAVOR)\Test\TCLib\TCLib.lib    \
          $(FEDROOT)\Objs\$(FLAVOR)\ZLib\ZLib.lib           \
          $(FEDROOT)\Objs\$(FLAVOR)\_utility\Utility.lib    \
          $(FEDROOT)\Objs\$(FLAVOR)\igc\igc.lib             \


RESFILE = $(DESTDIR)\AGC.res

DLLFLAGS= /DLL /DEF:AGC.def

LARGS= /LIBPATH:$(FEDEXT)\Vc\lib                     \
       /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE   \
       $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO)          \
       /delayload:ole32.dll /delayload:oleaut32.dll \
       /delayload:user32.dll /delayload:advapi32.dll \
       /MACHINE:I386 $(DLLFLAGS)


#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH) : $(DESTDIR) $(IDLTARGS) $(RESFILE) $(OBJS) $(USERLIBS) $(DESTDIR)\AGCGuid.lib
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS)
#  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN): UnregServer
  $(REMDIR) $(DESTDIR)

XMLTransforms: $(DESTDIR)\AGCEventsCPP.h   \
               $(DESTDIR)\AGCEventsIDL.idl \
               $(DESTDIR)\AGCEventsMC.mc   \
               $(DESTDIR)\AGCEventsRC2.rc2 \
               $(DESTDIR)\AGCEventsRCH.h   

$(DESTDIR)\AGCEventsCPP.h    : AGCEvents.xml AGCEventsCPP.xsl
  @echo Transforming AGCEvents.xml through AGCEventsCPP.xsl
  cscript //NoLogo XMLXForm.js AGCEvents.xml AGCEventsCPP.xsl $(DESTDIR)\AGCEventsCPP.h

$(DESTDIR)\AGCEventsIDL.idl  : AGCEvents.xml AGCEventsIDL.xsl
  @echo Transforming AGCEvents.xml through AGCEventsIDL.xsl
  cscript //NoLogo XMLXForm.js AGCEvents.xml AGCEventsIDL.xsl $(DESTDIR)\AGCEventsIDL.idl

$(DESTDIR)\AGCEventsMC.mc    : AGCEvents.xml AGCEventsMC.xsl
  @echo Transforming AGCEvents.xml through AGCEventsMC.xsl
  cscript //NoLogo XMLXForm.js AGCEvents.xml AGCEventsMC.xsl $(DESTDIR)\AGCEventsMC.mc

$(DESTDIR)\AGCEventsRC2.rc2  : AGCEvents.xml AGCEventsRC2.xsl
  @echo Transforming AGCEvents.xml through AGCEventsRC2.xsl
  cscript //NoLogo XMLXForm.js AGCEvents.xml AGCEventsRC2.xsl $(DESTDIR)\AGCEventsRC2.rc2

$(DESTDIR)\AGCEventsRCH.h    : AGCEvents.xml AGCEventsRCH.xsl
  @echo Transforming AGCEvents.xml through AGCEventsRCH.xsl
  cscript //NoLogo XMLXForm.js AGCEvents.xml AGCEventsRCH.xsl $(DESTDIR)\AGCEventsRCH.h

$(DESTDIR)\AGC0409.bin    \
$(DESTDIR)\AGCEventsMC.rc : $(DESTDIR)\AGCEventsMC.mc 
  mc.exe -r $(DESTDIR) -h $(DESTDIR) $(DESTDIR)\AGCEventsMC.mc

$(DESTDIR)\AGCIDL.h    \
$(DESTDIR)\AGCIDL_d.c  \
$(DESTDIR)\AGCIDL_p.c  \
$(DESTDIR)\AGCIDL_i.c  : $(DESTDIR)\AGCEventsIDL.idl ..\Inc\AGCIDL.idl \
                         ..\Test\Inc\ScrRun.idl  \
                         ..\Test\TCAtl\VersionInfoImpl.idl
  $(MIDL) $(MIDLARGS) ..\Inc\AGCIDL.idl
  
$(DESTDIR)\AGC.h    \
$(DESTDIR)\AGC.tlb  \
$(DESTDIR)\AGC_i.c  : AGC.idl $(DESTDIR)\AGCIDL.h
  $(MIDL) $(MIDLARGS) .\AGC.idl

$(DESTDIR)\AGCGuid.obj : AGCGuid.c $(DESTDIR)\AGCIDL_i.c $(DESTDIR)\AGC_i.c
  $(CC) $(NOLOGO) /c /Fo$(DESTDIR)\AGCGuid.obj AGCGuid.c

$(DESTDIR)\AGCGuid.lib : $(DESTDIR)\AGCGuid.obj
  $(LIB) $(NOLOGO) /out:$(DESTDIR)\AGCGuid.lib $(DESTDIR)\AGCGuid.obj

$(DESTDIR)\AGC.obj               : $(PCHFILE)
$(DESTDIR)\AGCGlobal.obj         : $(PCHFILE)
$(DESTDIR)\AGCVector.obj         : $(PCHFILE)
$(DESTDIR)\AGCOrientation.obj    : $(PCHFILE)
$(DESTDIR)\AGCEvent.obj          : $(PCHFILE)
$(DESTDIR)\AGCEventData.obj      : $(PCHFILE)
$(DESTDIR)\AGCEventDef.obj       : $(PCHFILE) $(DESTDIR)\AGCEventsCPP.h
$(DESTDIR)\AGCEventLogger.obj    : $(PCHFILE)
$(DESTDIR)\AGCEventIDRange.obj   : $(PCHFILE)
$(DESTDIR)\AGCEventIDRanges.obj  : $(PCHFILE)
$(DESTDIR)\AGCCommand.obj        : $(PCHFILE)
$(DESTDIR)\AGCVersionInfo.obj    : $(PCHFILE)
$(DESTDIR)\AGCDBParams.obj       : $(PCHFILE)
$(DESTDIR)\AGCGameParameters.obj : $(PCHFILE)
$(DESTDIR)\TCNullStream.obj      : $(PCHFILE)
$(DESTDIR)\TCUtility.obj         : $(PCHFILE)
$(DESTDIR)\TCMarshalByValue.obj  : $(PCHFILE)
$(DESTDIR)\TCStrings.obj         : $(PCHFILE)
$(DESTDIR)\AdminSessionHelper.obj: $(PCHFILE)
$(DESTDIR)\AGCHullType.obj       : $(PCHFILE)
$(DESTDIR)\AGCModel.obj          : $(PCHFILE)
$(DESTDIR)\AGCModels.obj         : $(PCHFILE)
$(DESTDIR)\AGCProbe.obj          : $(PCHFILE)
$(DESTDIR)\AGCProbes.obj         : $(PCHFILE)
$(DESTDIR)\AGCGame.obj           : $(PCHFILE)
$(DESTDIR)\AGCShip.obj           : $(PCHFILE)
$(DESTDIR)\AGCShips.obj          : $(PCHFILE)
$(DESTDIR)\AGCStation.obj        : $(PCHFILE)
$(DESTDIR)\AGCStations.obj       : $(PCHFILE)
$(DESTDIR)\AGCAleph.obj          : $(PCHFILE)
$(DESTDIR)\AGCAlephs.obj         : $(PCHFILE)
$(DESTDIR)\AGCSector.obj         : $(PCHFILE)
$(DESTDIR)\AGCSectors.obj        : $(PCHFILE)
$(DESTDIR)\AGCTeam.obj           : $(PCHFILE)
$(DESTDIR)\AGCTeams.obj          : $(PCHFILE)
$(DESTDIR)\AGCAsteroid.obj       : $(PCHFILE)
$(DESTDIR)\AGCAsteroids.obj      : $(PCHFILE)



$(RESFILE): $(FEDSRC)\Inc\VerRes.rc $(MAINTARGROOT).rc $(MAINTARGROOT).rc2 resource.h *.rgs \
            $(DESTDIR)\AGCEventsRC2.rc2 $(DESTDIR)\AGCEventsRCH.h   \
            $(DESTDIR)\AGCEventsMC.rc $(DESTDIR)\AGC.tlb
   @echo Compiling Resources...
   @copy $(MAINTARGROOT).rc $(DESTDIR)
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
#   echo [$(RC) $(RARGS) /D "DLL" /Fo$(DESTDIR)\$(MAINTARGROOT).res $(DESTDIR)\$(MAINTARGROOT).rc]
   $(RC) $(RARGS) /D "DLL" /Fo$(DESTDIR)\$(MAINTARGROOT).res $(DESTDIR)\$(MAINTARGROOT).rc

RegServer:
!if defined(sretail) || defined(retail)   
    @$(FEDSRC)\tools\bin32\upx -9 -q $(DESTDIR)\$(MAINTARGROOT).dll
!endif
  $(REGDLL)

UnregServer:
  @echo Unregistering $(MAINTARGFULLPATH)...
 $(UNREGDLL)
  @if not exist $(MAINTARGFULLPATH) echo File $(MAINTARGFULLPATH) does not exist
  @if exist $(DESTDIR)\RegSvr32.trg del $(DESTDIR)\RegSvr32.trg

$(DESTDIR)\RegSvr32.trg:
  @echo Registering $(MAINTARGFULLPATH)...
  $(REGDLL)
  @echo regsvr32 execution time > $(DESTDIR)\RegSvr32.trg
  @if not exist $(MAINTARGFULLPATH) echo File $(MAINTARGFULLPATH) does not exist

BuildAndReg: $(MAINTARGFULLPATH) $(DESTDIR)\RegSvr32.trg

