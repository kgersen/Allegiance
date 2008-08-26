#############################################################################
#
# Makefile for TCObj.dll
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

SRCROOT=Test\TCObj
MAINTARGROOT=TCObj
MAINTARGEXT=dll
INCLUDELOCAL=$(FEDEXT)\vc\atl\inc;$(FEDEXT)\vc\mfc\inc;$(FEDSRC)\Test\TCObj;$(FEDSRC)\Test\Inc;$(DESTDIR);$(FEDSRC)\ZLib
PCHROOT=pch
PCHINC=
PCHINCLOCAL=$(PCHROOT).h
PROJCLEAN=ProjectClean
MIDLINCLOCAL=/I ..\Inc
CARGSLOCAL=/D _ATL_STATIC_REGISTRY /D _WINDLL /D _USRDLL


#############################################################################
# Rules
#

!include "..\..\makefile.inc"


#############################################################################
# Build Targets
#

RARGS=/l 0x409 $(RARGSFLAVOR)

IDLTARGS= $(DESTDIR)\ScrRun.h          \
          $(DESTDIR)\TCIDL.h           \
          $(DESTDIR)\TCObj.h           \
          $(DESTDIR)\TCObj.tlb

OBJS= $(DESTDIR)\TCObj.obj             \
      $(DESTDIR)\SrcInc.obj            \
      $(DESTDIR)\TCNullStream.obj      \
      $(DESTDIR)\TCUtility.obj         \
      $(DESTDIR)\TCMarshalByValue.obj  \
      $(DESTDIR)\TCStrings.obj         \
      $(DESTDIR)\pch.obj

LIBS= $(LIBSFLAVOR) \
      kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib        \
      advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib \
      delayimp.lib

USERLIBS= $(FEDROOT)\Objs\$(FLAVOR)\Test\TCLib\TCLib.lib \
          $(FEDROOT)\Objs\$(FLAVOR)\ZLib\ZLib.lib

RESFILE = $(DESTDIR)\TCObj.res

DLLFLAGS= /DLL /DEF:TCObj.def

LARGS= /LIBPATH:$(FEDEXT)\Vc\lib                     \
       /OUT:$(MAINTARGFULLPATH) /SUBSYSTEM:CONSOLE   \
       $(LARGSFLAVOR) /PDB:$(PDB) $(NOLOGO)          \
       /delayload:ole32.dll /delayload:oleaut32.dll  \
       /delayload:user32.dll /delayload:advapi32.dll \
       /MACHINE:I386 $(DLLFLAGS)


#############################################################################
#
# Dependencies
#

$(MAINTARGFULLPATH) : $(DESTDIR) $(IDLTARGS) $(RESFILE) $(OBJS) $(USERLIBS)
  $(LINK) $(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS)
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map 2> NUL

!ifdef browse
$(BSCTARG) : $(DESTDIR)\*.sbr
  @echo Creating browse info file ($(MAINTARGROOT).bsc)...
  $(BSCMAKE) /nologo /Iu /o$(BSCTARG) $(DESTDIR)\*.sbr
!endif

$(PROJCLEAN):
  $(REMDIR) $(DESTDIR)

$(DESTDIR)\ScrRun.h : ..\Inc\ScrRun.idl
  $(MIDL) $(MIDLARGS) ..\Inc\ScrRun.idl

$(DESTDIR)\TCIDL.h : ..\Inc\TCIDL.idl $(DESTDIR)\ScrRun.h
  $(MIDL) $(MIDLARGS) ..\Inc\TCIDL.idl

$(DESTDIR)\TCObj.h : TCObj.idl $(DESTDIR)\TCIDL.h
  $(MIDL) $(MIDLARGS) .\TCObj.idl

$(DESTDIR)\TCObj.tlb : TCObj.idl $(DESTDIR)\TCIDL.h
  $(MIDL) $(MIDLARGS) .\TCObj.idl

$(DESTDIR)\TCObj.obj            : $(PCHFILE)
$(DESTDIR)\SrcInc.obj           : $(PCHFILE)
$(DESTDIR)\TCNullStream.obj     : $(PCHFILE)
$(DESTDIR)\TCUtility.obj        : $(PCHFILE)
$(DESTDIR)\TCMarshalByValue.obj : $(PCHFILE)
$(DESTDIR)\TCStrings.obj        : $(PCHFILE)

$(RESFILE): $(FEDSRC)\Inc\VerRes.rc $(MAINTARGROOT).rc resource.h *.rgs $(DESTDIR)\TCObj.tlb
   @echo Compiling Resources...
#  $(RC) $(RARGS) /D "DLL" /Fo$(RESFILE) $(MAINTARGROOT).rc
   @copy $(MAINTARGROOT).rc $(DESTDIR)
   @echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   @type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   $(RC) $(RARGS) /Fo$(DESTDIR)\$(MAINTARGROOT).res $(DESTDIR)\$(MAINTARGROOT).rc

