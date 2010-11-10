#
# Makefile for me.exe
# 

MAINTARGROOT=me
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Engine;$(FEDSRC)\ZLib;
PCHROOT=pch
PCHINC=$(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h
NODEPENDENCIES=1

!include "../makefile.inc"

OBJS=                     \
    $(DESTDIR)\cursor.obj \
    $(DESTDIR)\me.obj    

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib      \
      $(DESTDIR)\..\engine\Engine.lib


!include "../makeexe.inc"

$(DESTDIR)\cursor.obj:     $(PCHFILE)
$(DESTDIR)\me.obj:         $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr $(DESTDIR)\..\engine\*.sbr
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr $(DESTDIR)\..\engine\*.sbr
