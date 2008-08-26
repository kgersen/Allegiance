###################################################################
#
# Makefile for makeing an exe
#
###################################################################

LIB=

!ifndef SUBSYSTEM
SUBSYSTEM=windows
!endif

!ifdef DREAMCAST
LIBS= $(LIBSFLAVOR) \
      strig.lib shintr.lib fulllibc.lib coredll.lib dsound.lib ddraw.lib dplayx.lib dinput.lib\
      dxguid.lib ole32.lib amstrmid.lib
!else
LIBS= $(LIBSFLAVOR) \
      urlmon.lib kernel32.lib user32.lib   \
      gdi32.lib winspool.lib comdlg32.lib  \
      advapi32.lib shell32.lib ole32.lib   \
      oleaut32.lib uuid.lib odbc32.lib     \
      odbccp32.lib ddraw.lib dsound.lib    \
      dxguid.lib winmm.lib dplayx.lib      \
      d3drm.lib amstrmid.lib dinput.lib    \
      comctl32.lib version.lib  ws2_32.lib \
      delayimp.lib            \
      icqmapi.lib wininet.lib lz32.lib     \
      fedguids.lib netapi32.lib comsupp.lib oledb.lib
      
!endif

!ifdef SPLITSYMBOLS
LINKPDB= NONE
!else
LINKPDB= $(PDB)
!endif

!ifdef DREAMCAST
LARGS= $(LARGSFLAVOR) $(NOLOGO) /subsystem:$(CESubsystem) \
       /STACK:65536,4096 /nodefaultlib \
       /entry:"WinMainCRTStartup" /base:"0x00010000" /align:"4096"\
       /delayload:netapi32.dll \
       /delayload:advapi32.dll \
       /pdb:$(LINKPDB) \
       /machine:SH4 \
!ifdef RETAIL
       /LIBPATH:$(FEDEXT)\DreamCast\Lib\sh4\wince\retail \
!else
       /LIBPATH:$(FEDEXT)\DreamCast\Lib\sh4\wince\debug \
!endif
       /out:$(DESTDIR)\$(MAINTARGROOT).exe
!else
LARGS= $(LARGSFLAVOR) $(NOLOGO) /subsystem:$(SUBSYSTEM) \
       /pdb:$(LINKPDB)                      \
       /machine:I386                        \
!ifdef icap
       /LIBPATH:$(FEDEXT)\icecap4\lib       \
!endif
       /LIBPATH:$(DXROOT)\lib               \
       /LIBPATH:$(FEDEXT)\Vc\Lib            \
       /LIBPATH:$(FEDEXT)\Zone              \
       /LIBPATH:$(FEDEXT)\ICQAPI            \
       /LIBPATH:$(DESTDIR)\..\Guids         \
       /out:$(DESTDIR)\$(MAINTARGROOT).exe
!endif

LARGSALL=$(LARGS) $(OBJS) $(RESFILE) $(LIBS) $(USERLIBS) $(DESTDIR)\pch.obj $(LARGSLOCAL)

# the main (default) target in all its glory

$(MAINTARGFULLPATH): $(DESTDIR)  \
                     $(USERLIBS) \
                     $(OBJS)     \
                     $(MSGFILE)  \
                     $(RESFILE)  \
                     $(OTHEREXEDEPS)
#                     $(BSCTARG)
  $(LINK) $(LARGSALL)
!ifndef NOMAP
  $(MAPSYM) -o $(DESTDIR)\$(MAINTARGROOT).sym $(DESTDIR)\$(MAINTARGROOT).map
!endif
!ifdef SPLITSYMBOLS
#  $(SPLITSYMBOLSTOOL) $(DESTDIR)\$(MAINTARGROOT).exe $(DESTDIR)\$(MAINTARGROOT).dbg
!endif