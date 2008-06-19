#
# Makefile for xmunge.exe
#

SUBSYSTEM=console
MAINTARGROOT=xmunge
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Engine;$(FEDSRC)\ZLib;
PCHROOT=pch
PCHINC=$(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h
NOBSC=1

!include "../makefile.inc"

OBJS=$(DESTDIR)\xmunge.obj

USERLIBS= \
      $(DESTDIR)\..\engine\engine.lib \
      $(DESTDIR)\..\zlib\zlib.lib\
      $(FEDSRC)\xmunge\d3drm.lib 
      
!include "../makeexe.inc"

$(DESTDIR)\xmunge.obj:         $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr $(DESTDIR)\..\jpeg\*.sbr
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr $(DESTDIR)\..\engine\*.sbr $(DESTDIR)\..\jpeg\*.sbr
