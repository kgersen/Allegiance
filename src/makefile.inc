#############################################################################
#
# Common makefile definitions
#
#############################################################################

!IFNDEF SRCROOT
SRCROOT=$(MAINTARGROOT)
!ENDIF

!IFNDEF DESTROOT
DESTROOT=$(SRCROOT)
!ENDIF

!IFNDEF PROJCLEAN
PROJCLEAN=DefaultClean
!ENDIF

FEDSRC=$(FEDROOT)\src
FEDEXT=$(FEDROOT)\extern
SRCDIR=$(FEDSRC)\$(SRCROOT)
DESTDIR=$(FEDROOT)\Objs\$(FLAVOR)\$(DESTROOT)

!IFNDEF PCHINCLOCAL
PCHINCLOCAL=$(SRCDIR)\*.h
!ENDIF

!IFDEF DX
DXROOT=$(FEDEXT)\dx$(DX)
!ELSE
DXROOT=$(FEDEXT)\dx
!ENDIF

MAINTARG=$(MAINTARGROOT).$(MAINTARGEXT)
MAINTARGFULLPATH=$(DESTDIR)\$(MAINTARG)

LINK=link
BSCMAKE=bscmake
LIB=lib
MIDL=$(FEDEXT)\vc\bin\midl
MSDEV=devenv
REMDIR=$(FEDSRC)\tools\bin32\delnode -q
REGSVR=regsvr32 /s /c
SPLITSYMBOLSTOOL=$(FEDSRC)\tools\bin32\PrivateStrip


#############################################################################
# MIDL Build Rule Arguments

!IFNDEF MIDLINCLOCAL
MIDLINCLOCAL=
!ENDIF
MIDLINC =/I $(FEDEXT)\vc\inc
MIDLARGS=$(MIDLINC) $(MIDLINCLOCAL) /lcid LOCALE \
         /win32                             \
         /Oicf /no_def_idir /out $(DESTDIR) \
         /h       $(*B).h                   \
         /tlb     $(*B).tlb                 \
         /dlldata $(*B)_d.c                 \
         /iid     $(*B)_i.c                 \
         /proxy   $(*B)_p.c


#############################################################################
# Registration username/password

!if defined(user)
REGUSER=$(user)
REGPW=$(pw)
!else
REGUSER=Interactive User
REGPW=
!endif


#############################################################################
# Registration commands

REGEXE=if exist $(MAINTARGFULLPATH) $(MAINTARGFULLPATH) -RegServer "$(REGUSER)" $(REGPW)
UNREGEXE=if exist $(MAINTARGFULLPATH) $(MAINTARGFULLPATH) -UnregServer
REGDLL=if exist $(MAINTARGFULLPATH) $(REGSVR) $(MAINTARGFULLPATH)
UNREGDLL=if exist $(MAINTARGFULLPATH) $(REGSVR) /u $(MAINTARGFULLPATH)


#############################################################################
# What registry key to use

!if defined (ALLEGIANCE_DEV)
COMPILE_DEV_REGKEY=/D_ALLEGIANCE_DEV_
!else
COMPILE_DEV_REGKEY=
!endif


#############################################################################
# Use Zone emulation?

!if defined (USEAUTH)
COMPILEAUTH=$(COMPILEAUTH) /D"USEAUTH"
!endif


#############################################################################
#
# Path
#
#############################################################################

!include "path.mak"

#############################################################################
#
# Version info
#
#############################################################################

!IFDEF VER
RARGSVERSION=/D"VER=$(VER)"

!ELSE
RARGSVERSION=
!ENDIF

#############################################################################
#
# Optimization
#
#############################################################################

# /Og   global optimization
# /Os   small code sequences
# /Ot   fast code sequences
# /Oy   frame pointer omission
# /Ob1  inline
# /Ob2  automatic inline
# /Gs   stack probes
# /Gf   string pooling
# /Gy   function level linking
#
# /Oi   inline intrisics
# /Oa   assume no aliasing
# /Ow   assume aliasing across function calls
# /Gr   _fastcall
# /G5   Pentium
# /G6   Pentium Pro

OPTIMIZATION_FLAGS=/Os /Oy /Ob1 /Gs /Gy

# /O1 == /Og /Os /Oy /Ob1 /Gs /Gf /Gy
# /O2 == /Og /Ot /Oy /Ob1 /Gs /Gf /Gy /Oi
#OPTIMIZATION_FLAGS=/O1
#OPTIMIZATION_FLAGS=/O2
#OPTIMIZATION_FLAGS=/Od
#OPTIMIZATION_FLAGS=/Ox

#############################################################################
#
# Static Retail build
#
#############################################################################

!IFDEF sretail

FLAVOR=SRetail
STATICFLAVOR=SRetail

CARGSFLAVOR= $(COMPILE_DEV_REGKEY) /D "NDEBUG" /D "OPTIMIZED" /MT $(OPTIMIZATION_FLAGS) /GR-
RARGSFLAVOR= /d "NDEBUG" /D "OPTIMIZED" $(RARGSVERSION)
LIBSFLAVOR=libcmt.lib libcpmt.lib libcimt.lib
MFCLIBSFLAVOR=
LARGSFLAVOR=/debug /debugtype:cv /INCREMENTAL:NO /RELEASE /nodefaultlib /OPT:REF /OPT:NOICF

!ELSEIFDEF stest

#############################################################################
#
# Static Test build
#
#############################################################################
FLAVOR=STest
STATICFLAVOR=STest

CARGSFLAVOR= $(COMPILE_DEV_REGKEY) /D "_DEBUG" /D "DEBUG" /D "OPTIMIZED" /MT $(OPTIMIZATION_FLAGS) /GR
RARGSFLAVOR= /d "_DEBUG" /d "DEBUG" /D "OPTIMIZED" $(RARGSVERSION)
LIBSFLAVOR=libcmtd.lib libcpmtd.lib libcimtd.lib
MFCLIBSFLAVOR=nafxcwd.lib ctl3d32s.lib comctl32.lib
LARGSFLAVOR=/debug /debugtype:cv /INCREMENTAL:NO /nodefaultlib /OPT:REF /OPT:NOICF

!ELSEIFDEF sdebug

#############################################################################
#
# Static Debug build
#
#############################################################################
FLAVOR=SDebug
STATICFLAVOR=SDebug

CARGSFLAVOR = $(COMPILE_DEV_REGKEY) /D "_DEBUG" /D "DEBUG" /MTd /Od /GR /RTC1
RARGSFLAVOR= /d "_DEBUG" /d "DEBUG" $(RARGSVERSION)
LIBSFLAVOR=libcmtd.lib libcpmtd.lib libcimtd.lib
MFCLIBSFLAVOR=nafxcwd.lib ctl3d32s.lib comctl32.lib
LARGSFLAVOR=/debug /debugtype:cv /INCREMENTAL:NO /nodefaultlib

!ELSEIFDEF retail

#############################################################################
#
# Retail build - Get to work with /GL and /LTGC:PGOPTIMIZE
#
#############################################################################

FLAVOR=Retail

CARGSFLAVOR= $(COMPILE_DEV_REGKEY) $(COMPILEAUTH) /D "_CRT_SECURE_NO_WARNINGS" /D "NDEBUG" /D "OPTIMIZED" /MD $(OPTIMIZATION_FLAGS) /GR-
RARGSFLAVOR= /d "NDEBUG" /D "OPTIMIZED" /D"_AFXDLL" $(RARGSVERSION)
LIBSFLAVOR=msvcrt.lib msvcprt.lib
#DXLIBS=dxguid.lib d3d8.lib ddraw.lib dinput.lib
MFCLIBSFLAVOR=mfc90.lib mfcs90.lib comsupp.lib comsuppw.lib atls.lib
LARGSFLAVOR=/debugtype:cv /INCREMENTAL:NO /RELEASE /nodefaultlib /fixed:no /OPT:REF /OPT:NOICF

!ELSEIFDEF test

#############################################################################
#
# Test build
#
#############################################################################
FLAVOR=Test

CARGSFLAVOR= $(COMPILE_DEV_REGKEY) /D "_DEBUG" /D "DEBUG" /D "OPTIMIZED" /D"_AFXDLL" /MD $(OPTIMIZATION_FLAGS) /GR
RARGSFLAVOR= /d "_DEBUG" /d "DEBUG" /D "OPTIMIZED" /D"_AFXDLL" $(RARGSVERSION)
LIBSFLAVOR=msvcrtd.lib msvcprtd.lib
MFCLIBSFLAVOR=mfc42d.lib mfcs42d.lib mfco42d.lib ctl3d32.lib
LARGSFLAVOR=/debug /debugtype:cv /INCREMENTAL:NO /nodefaultlib /OPT:REF /OPT:NOICF

!ELSEIFDEF training

#############################################################################
#
# Debug Training Missions build
#
#############################################################################
FLAVOR=Training

CARGSFLAVOR = $(COMPILE_DEV_REGKEY)  /D "_DEBUG_TRAINING" /D "_DEBUG" /D "DEBUG" /D"_AFXDLL" /MDd /Od /GR /GZ
RARGSFLAVOR= /d "_DEBUG" /d "DEBUG" /D"_AFXDLL" $(RARGSVERSION)
LIBSFLAVOR=msvcrtd.lib msvcprtd.lib
MFCLIBSFLAVOR=mfc42d.lib mfcs42d.lib mfco42d.lib ctl3d32.lib
LARGSFLAVOR=/debug /debugtype:cv /INCREMENTAL:NO /nodefaultlib

!ELSEIFDEF icap

#############################################################################
#
# Visual Studio Developer Edition profiling build
# This needs to be updated?
#     http://msdn.microsoft.com/en-us/library/bb385832.aspx#ProfilingTools
#
#############################################################################

FLAVOR=ICap
RARGSFLAVOR= /d "_DEBUG" /d "DEBUG" /D "OPTIMIZED" /D"_AFXDLL" $(RARGSVERSION)
MFCLIBSFLAVOR=mfc42.lib mfcs42.lib ctl3d32.lib
NOMAP=1

icepick=1
!ifdef icepick
CARGSFLAVOR= $(COMPILE_DEV_REGKEY) /D "ICAP" /D "NDEBUG" /D "OPTIMIZED" /D"_AFXDLL" /ML $(OPTIMIZATION_FLAGS) /GR- /Zi
LARGSFLAVOR= /INCREMENTAL:NO /RELEASE /nodefaultlib /fixed:no /dbgimplib /debug /debugtype:cv,fixup /opt:ref
LIBSFLAVOR=libc.lib libcp.lib icecap.lib
!else
CARGSFLAVOR= $(COMPILE_DEV_REGKEY) /D "NDEBUG" /D "OPTIMIZED" /D"_AFXDLL" /ML /GR- /Zi /Osyb1 /Gs /Gf /Gy /fastcap
LARGSFLAVOR= /INCREMENTAL:NO /RELEASE /nodefaultlib /fixed:no /debug /debugtype:cv
LIBSFLAVOR=libc.lib libcp.lib icecap.lib
!endif

!ELSE

#############################################################################
#
# Debug build
#
#############################################################################

FLAVOR=Debug

CARGSFLAVOR = $(COMPILE_DEV_REGKEY) $(COMPILEAUTH) /D _CRT_SECURE_NO_WARNINGS /D "_DEBUG" /D "DEBUG" /MDd /Od /GR /RTC1
RARGSFLAVOR= /d "_DEBUG" /d "DEBUG" /D"_AFXDLL" $(RARGSVERSION)
LIBSFLAVOR=msvcrtd.lib msvcprtd.lib
MFCLIBSFLAVOR=mfc90d.lib mfcs90d.lib comsuppd.lib comsuppwd.lib atlsd.lib
LARGSFLAVOR=/debug /debugtype:cv /INCREMENTAL:NO /nodefaultlib
#DXLIBS=dxguid.lib d3d8.lib ddraw.lib dinput.lib dplayx.lib dsound.lib amstrmid.lib

!ENDIF

#############################################################################
#
#  Compuware DevPartner Suite BoundsChecker build
#
#############################################################################

!ifdef BCHK
!ifndef BCBINDIR
!error For BC build, specify location of nmcl.exe in BCBINDIR variable!
!endif

!ifdef STATICFLAVOR

!IFDEF sretail
FLAVOR = SBChecker
!else
FLAVOR = SBCheckerD
!endif

!else

!IFDEF retail
FLAVOR = BChecker
!else
FLAVOR = BCheckerD
!endif

!endif

CC     = "$(BCBINDIR)\nmcl.exe" $(BCOPTS)
LINK   = "$(BCBINDIR)\nmlink.exe"

!endif

#############################################################################
#
# Compuware DevPartner Suite TrueTime build
#
#############################################################################

!ifdef TRUETIME
!ifndef TTBINDIR
!error For TT build, specify location of nmcl.exe in TTBINDIR variable!
!endif

!ifdef STATICFLAVOR

!IFDEF sretail
FLAVOR = STrueTime
!else
FLAVOR = STrueTimeD
!endif

!else

!IFDEF retail
FLAVOR = TrueTime
!else
FLAVOR = TrueTimeD
!endif

!endif

CC     = "$(TTBINDIR)\nmcl.exe" /NMttOn $(TTOPTS)
LINK   = "$(TTBINDIR)\nmlink.exe" /NMttOn

!endif

!if defined(VERBOSE)
NOLOGO=
!else
CC=@$(CC)
LINK=@$(LINK)
LIB=@$(LIB)
MIDL=@$(MIDL)
BSCMAKE=@$(BSCMAKE)
MSDEV=@$(MSDEV)
REMDIR=@$(REMDIR)
MKDEP=@$(MKDEP)
NOLOGO=/nologo
REGEXE=@$(REGEXE)
UNREGEXE=@$(UNREGEXE)
REGDLL=@$(REGDLL)
UNREGDLL=@$(UNREGDLL)
!endif


#############################################################################
#
# SEGA DreamCast Windows Embedded build (lol)
#
#############################################################################

!ifdef DREAMCAST

!IFDEF retail
FLAVOR=DreamCast
!else
FLAVOR=DreamCastD
!endif

CESubsystem=windowsce,2.10
CEVersion=210
CEConfigName=Maxall (WWE) 2.10
CECrt=C
CECrtDebug=C
CECrtMT=C
CECrtMTDebug=C
CEx86NoDefaultLib=libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib\
 /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib
CEx86Corelibc=corelibc.lib

CARGSFLAVOR=/nologo /Qsh4 /MC /W3 /Zi /Oi /Qs /Qfast /c\
    /D "UNICODE" /D "STRICT"\
    /D "SHx" /D "SH4" /D "_SH4_"\
    /D "DRAGON" /D "DREAMCAST"\
    /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D UNDER_CE=$(CEVersion)\
    /I $(FEDSRC)\dreamcast\
    /Fo$(DESTDIR)\ /Fd$(PDB)

!IFDEF retail
CARGSFLAVOR= $(CARGSFLAVOR) /O2 /D "NDEBUG"
!else
CARGSFLAVOR= $(CARGSFLAVOR) /Od /D "_DEBUG" /D "DEBUG"
!endif


LIBSFLAVOR=

CC=shcl
LIB=link.exe -lib

!endif

#############################################################################
#
# Win95 (lol)
#
#############################################################################

!IFDEF WIN95
CARGSFLAVOR= $(CARGSFLAVOR) /D"WIN95"
!ENDIF

#############################################################################
#
# BSC options
#
#############################################################################

BSC=$(DESTDIR)\$(MAINTARGROOT).bsc

!IFDEF retail

# don't build browse info for retail builds

BSCARG=
BSCTARG=

!else

!IFNDEF NOBSC
BSCARG=/FR$*
BSCTARG=$(BSC)
!ELSE
BSCARG=
BSCTARG=
!ENDIF

!endif

#############################################################################
#
# Compiler options
#
#############################################################################

PDB=$(DESTDIR)\$(MAINTARGROOT).pdb

!ifdef DREAMCAST
CARGSINC=  $(CARGSFLAVOR)
!else
CARGSINC=  $(CARGSFLAVOR) \
           $(BSCARG) \
           $(NOLOGO) /W3 /WX /Zi /c \
           /Gm- \
           /D "WIN32" /D "_WINDOWS" /D "STRICT" \
           /Fo$(DESTDIR)\ /FD /Fd$(PDB)
!endif
CARGS=$(CARGSLOCAL) $(CARGSINC) $(UNWINDCARG)

!ifdef DREAMCAST
INCLUDESYS=$(FEDEXT)\dreamcast\inc
INCLUDEINC=$(FEDEXT)\dreamcast\inc;$(FEDSRC)\Inc;
INCLUDE=$(INCLUDELOCAL);$(INCLUDEINC);
!else
INCLUDESYS=$(DXROOT)\include;$(FEDEXT)\vc\inc;$(FEDEXT)\vc\inc\sys;$(FEDEXT)\vc\atl\inc;
INCLUDEINC=$(DXROOT)\include;$(FEDEXT)\vc\inc;$(FEDEXT)\vc\inc\sys;$(FEDEXT)\vc\atl\inc;$(FEDEXT)\icecap\include;$(FEDEXT)\Zone;$(FEDSRC)\Zone;$(FEDEXT)\icqapi;$(FEDSRC)\Guids
INCLUDE=$(INCLUDELOCAL);$(INCLUDEINC);$(FEDROOT)\FreeZoneAuth
!endif

#############################################################################
#
# Resources
#
#############################################################################

!if defined(VERBOSE)
RC=RC
!else
RC=@RC
!endif
LANG=Usa

#############################################################################
#
# Message Files
#
#############################################################################

!if defined(VERBOSE)
MC=MC
!else
MC=@MC
!endif

#############################################################################
#
# Link
#
#############################################################################

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)

!if defined(VERBOSE)
MAPSYM=$(FEDEXT)\vc\bin\MAPSYM
!else
MAPSYM=@MAPSYM -nologo
!endif

#############################################################################
#
# Targets
#
#############################################################################

!MESSAGE -----------------------------------------------------------------------------
!MESSAGE Building $(MAINTARG), Flavor: $(FLAVOR), Language: $(LANG)

First: $(DESTDIR) $(IDLTARGS) $(MAINTARGFULLPATH) 

DefaultClean: $(DESTDIR)
  del $(DESTDIR)\*.h <$(FEDSRC)\tools\yes.txt
  del $(DESTDIR)\*.c <$(FEDSRC)\tools\yes.txt
  del $(DESTDIR)\*.obj <$(FEDSRC)\tools\yes.txt
  del $(DESTDIR)\*.sbr <$(FEDSRC)\tools\yes.txt
  del $(DESTDIR)\*.pch <$(FEDSRC)\tools\yes.txt
  del $(DESTDIR)\*.pdb <$(FEDSRC)\tools\yes.txt
  del $(DESTDIR)\*.res <$(FEDSRC)\tools\yes.txt

Clean: $(PROJCLEAN)

Full: Clean $(MAINTARGFULLPATH)

$(DESTDIR):
  -if not exist $(FEDROOT)\Objs           md $(FEDROOT)\Objs
  -if not exist $(FEDROOT)\Objs\$(FLAVOR) md $(FEDROOT)\Objs\$(FLAVOR)
  -if not exist $(DESTDIR)                md $(DESTDIR)

#############################################################################
#
# Precompiled headers
#
#############################################################################

!ifdef PCHROOT
PCHFILE=$(DESTDIR)\$(PCHROOT).pch
PCHHEADER=$(PCHROOT).h
!if defined(prefix)
CARGSPCH=
!else
CARGSPCH=/Yu$(PCHHEADER) /Fp$(PCHFILE)
!endif
$(PCHFILE): $(PCHINCLOCAL) $(PCHINC)
    $(CC) $(CARGS) /Yc$(PCHHEADER) /Fp$(PCHFILE) $(SRCDIR)\$(@B).cpp
!else
CARGSPCH=
!endif


!ifdef MSGFILE
MSGRCFILE=$(DESTDIR)\$(MSGFILE).rc
$(MSGFILE): $(MSGRCFILE)
$(MSGRCFILE): $(SRCDIR)\$(MSGFILE).msg
!if defined(VERBOSE)
  $(MC) $(SRCDIR)\$(@B).msg -r $(DESTDIR) -h $(DESTDIR) 2>&1
!else
  $(MC) $(SRCDIR)\$(@B).msg -r $(DESTDIR) -h $(DESTDIR)
!endif
#    $(MC) $(SRCDIR)\$(@B).msg
!endif

.cpp{$(DESTDIR)}.obj:
    $(CC) $(CARGS) $(CARGSPCH) $(SRCDIR)\$(@B).cpp

#.c{$(DESTDIR)}.obj:
#    $(CC) $(CARGS) $(SRCDIR)\$(@B).c

# End of makefile.inc