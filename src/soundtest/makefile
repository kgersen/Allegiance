#
# Makefile for soundtest.exe
#

SUBSYSTEM=console
MAINTARGROOT=Soundtest
MAINTARGEXT=exe
INCLUDELOCAL=$(FEDSRC)\soundengine;$(FEDSRC)\ZLib
PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\soundengine\*.h

!include "..\makefile.inc"

OBJS= $(DESTDIR)\soundtest.obj

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib      \
      $(DESTDIR)\..\soundengine\soundengine.lib   


!include "../makeexe.inc"

#
# Dependecies start here
#

$(DESTDIR)\soundtest.obj:      $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr $(DESTDIR)\..\zlib\*.sbr $(DESTDIR)\..\soundengine\*.sbr 
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr $(DESTDIR)\..\zlib\*.sbr $(DESTDIR)\..\soundengine\*.sbr 
