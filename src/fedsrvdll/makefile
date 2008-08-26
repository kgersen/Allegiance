#
# Makefile for AllSrv.dll
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

SRCROOT=Fedsrvdll
MAINTARGROOT=AllSrv
MAINTARGEXT=dll
CARGSLOCAL=/D"ALLSRV" /D _WIN32_WINNT=0x0400
INCLUDELOCAL=$(FEDEXT)\vc\atl\inc;$(FEDROOT)\src\zlib;$(DESTDIR)
#TLB=$(DESTDIR)\allsrv.h
TLB=fedsrv
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\_Utility\*.h 

!include "..\makefile.inc"

RARGS = /l 0x409 /d"ALLSRV" $(RARGSFLAVOR)

OBJSCNST= $(DESTDIR)\AllegianceController.obj   \
          $(DESTDIR)\FedSrv.obj 

OBJS=$(OBJSCNST) $(OBJSFLAVOR)

LIBS= $(LIBSFLAVOR) \
     kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib \
	 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib 

USERLIBS= \
    $(DESTDIR)\..\_utility\Utility.lib \
    $(DESTDIR)\..\zlib\zlib.lib

RESFILE= $(DESTDIR)\FedSrv.res

DLLFLAGS=/DLL /DEF:fedsrv.def

LARGS=/LIBPATH:$(DXROOT)\lib \
      /LIBPATH:$(FEDEXT)\Vc\lib \
#      /LIBPATH:$(FEDROOT)\Objs\$(FLAVOR)\zlib \
      /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE \
      $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO) \
      /MACHINE:I386 \
	   $(DLLFLAGS)

$(MAINTARGFULLPATH): $(DESTDIR) $(TLB) $(OBJS) $(RESFILE) $(BSCTARG) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS) $(DESTDIR)\pch.obj 
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map

$(DESTDIR)\AllegianceController.obj       : $(DESTDIR)\$(MAINTARGROOT).h $(PCHFILE)
$(DESTDIR)\FedSrv.obj       : $(PCHFILE)
$(DESTDIR)\$(MAINTARGROOT).h: fedsrv.idl

$(RESFILE): $(FEDSRC)\Inc\VerRes.rc $(FEDSRC)\FedSrvdll\FedSrv.rc
   copy $(FEDSRC)\FedSrvDll\FedSrv.rc $(DESTDIR)
   type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\FedSrv.rc
   $(RC) $(RARGS) /Fo$(RESFILE) $(DESTDIR)\FedSrv.rc

#.cpp{$(DESTDIR)}.obj:
#  $(CC) $(CARGS) $(SRCDIR)\$(@B).cpp

$(BSC): $(DESTDIR)\*.sbr
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr


