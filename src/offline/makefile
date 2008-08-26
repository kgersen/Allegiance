#
# Makefile for cadet.exe
#

MAINTARGROOT=offline
MAINTARGEXT=lib
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;$(FEDSRC)\igc;$(FEDSRC)\drones;$(FEDSRC)\_utility;
PCHINC=$(FEDSRC)\Effect\*.h $(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h $(FEDSRC)\igc\*.h $(FEDSRC)\drones\*.h $(FEDSRC)\_utility\*.h
PCHROOT=pch

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)

!include "../makefile.inc"

OBJS=   \
    $(DESTDIR)\mission.obj
    
#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)


OBJS= \
        $(DESTDIR)\mission.obj

$(DESTDIR)\mission.obj:             $(PCHFILE)
