#
# Makefile for techtree.exe
#

MAINTARGROOT=techtree
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\ZLib;$(FEDSRC)\IGC;$(FEDSRC)\Inc;
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\IGC\*.h
NOBSC=1
SUBSYSTEM=CONSOLE

!include "../makefile.inc"

OBJS= \
      $(DESTDIR)\techtree.obj \
      $(DESTDIR)\techinit.obj \
      $(DESTDIR)\techitem.obj \
      $(DESTDIR)\sqlwrap.obj \
      $(DESTDIR)\base.obj 

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib

!include "../makeexe.inc"

$(DESTDIR)\techtree.obj:         $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr 
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr 
