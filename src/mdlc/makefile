#
# Makefile for mdlc.exe
#

SUBSYSTEM=console
MAINTARGROOT=mdlc
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;
PCHROOT=pch
PCHINC=$(FEDSRC)\effect\*.h $(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h
NODEPENDENCIES=1

!include "../makefile.inc"

OBJS=$(DESTDIR)\mdlc.obj

USERLIBS= \
      $(DESTDIR)\..\effect\effect.lib \
      $(DESTDIR)\..\engine\engine.lib \
      $(DESTDIR)\..\zlib\zlib.lib

!include "../makeexe.inc"

$(DESTDIR)\mdlc.obj:         $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr 
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr $(DESTDIR)\..\effect\*.sbr $(DESTDIR)\..\engine\*.sbr 
