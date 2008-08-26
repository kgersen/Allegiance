#
# Makefile for engineapp.exe
#

MAINTARGROOT=engineapp
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;$(FEDSRC)\jpeg;
PCHINC=$(FEDSRC)\Effect\*.h $(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h

!include "../makefile.inc"

OBJS=$(DESTDIR)\engineapp.obj

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib      \
      $(DESTDIR)\..\engine\Engine.lib  \
      $(DESTDIR)\..\effect\Effect.lib  \
      $(DESTDIR)\..\jpeg\jpeg.lib

!include "../makeexe.inc"

$(DESTDIR)\engineapp.obj:         $(PCHFILE)
