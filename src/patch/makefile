#
# Makefile for patch.exe
#

SUBSYSTEM=console
MAINTARGROOT=patch
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\ZLib;
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h
NOBSC=1

!include "../makefile.inc"

OBJS=$(DESTDIR)\patch.obj

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib

!include "../makeexe.inc"

$(DESTDIR)\patch.obj:         $(PCHFILE)
