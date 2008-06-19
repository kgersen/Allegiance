#
# Makefile for cvh.exe
#

SUBSYSTEM=console
MAINTARGROOT=cvh
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=
PCHROOT=pch
PCHINC=
NOBSC=1

!include "../makefile.inc"

OBJS=$(DESTDIR)\main.obj


!include "../makeexe.inc"

$(DESTDIR)\main.obj:         $(PCHFILE)

$(BSC):
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC)

