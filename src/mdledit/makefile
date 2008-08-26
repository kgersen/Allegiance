#
# Makefile for mdledit.exe
#

MAINTARGROOT=mdledit
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;
PCHROOT=pch
PCHINC=$(FEDSRC)\Effect\*.h $(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h
NODEPENDENCIES=1

!include "../makefile.inc"

OBJS=$(DESTDIR)\mdledit.obj

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib      \
      $(DESTDIR)\..\engine\Engine.lib  \
      $(DESTDIR)\..\effect\Effect.lib


!include "../makeexe.inc"

$(DESTDIR)\mdledit.obj:         $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr $(DESTDIR)\..\effect\*.sbr $(DESTDIR)\..\engine\*.sbr 
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr $(DESTDIR)\..\effect\*.sbr $(DESTDIR)\..\engine\*.sbr 
