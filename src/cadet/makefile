#
# Makefile for cadet.exe
#

MAINTARGROOT=cadet
MAINTARGEXT=exe
CARGSLOCAL=
INCLUDELOCAL=$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;$(FEDSRC)\igc;$(FEDSRC)\drones;$(FEDSRC)\_utility;
PCHROOT=pch
PCHINC=$(FEDSRC)\Effect\*.h $(FEDSRC)\Engine\*.h $(FEDSRC)\ZLib\*.h $(FEDSRC)\igc\*.h $(FEDSRC)\drones\*.h $(FEDSRC)\_utility\*.h
NOBSC=1

!include "../makefile.inc"

OBJS= \
        $(DESTDIR)\cadetplay.obj \
        $(DESTDIR)\cadetradar.obj \
        $(DESTDIR)\terrain.obj \
        $(DESTDIR)\testscenes.obj \
        $(DESTDIR)\gamecore.obj \
        $(DESTDIR)\joystick.obj \
        $(DESTDIR)\sound.obj \
        $(DESTDIR)\mission.obj \
        $(DESTDIR)\cadetgamecontext.obj \
        $(DESTDIR)\cadetflightimage.obj \
        $(DESTDIR)\cadetscenes.obj

USERLIBS= \
      $(DESTDIR)\..\zlib\zlib.lib      \
      $(DESTDIR)\..\engine\Engine.lib  \
      $(DESTDIR)\..\drones\drones.lib  \
      $(DESTDIR)\..\igc\igc.lib  \
      $(DESTDIR)\..\_utility\utility.lib  \
      $(DESTDIR)\..\drones\drones.lib  \
#!ifdef DREAMCAST
#      $(DESTDIR)\..\dreamcast\dreamcast.lib  \
#!endif
      $(DESTDIR)\..\effect\Effect.lib  


!include "../makeexe.inc"

$(DESTDIR)\cadetplay.obj:          $(PCHFILE)
$(DESTDIR)\cadetradar.obj:          $(PCHFILE)
$(DESTDIR)\terrain.obj:            $(PCHFILE)
$(DESTDIR)\gamecore.obj:            $(PCHFILE)
$(DESTDIR)\joystick.obj:            $(PCHFILE)
$(DESTDIR)\sound.obj:               $(PCHFILE)
$(DESTDIR)\mission.obj:             $(PCHFILE)
$(DESTDIR)\cadetgamecontext.obj:    $(PCHFILE)
$(DESTDIR)\cadetflightimage.obj:   $(PCHFILE)
$(DESTDIR)\cadetscenes.obj:         $(PCHFILE)
$(DESTDIR)\testscenes.obj:         $(PCHFILE)

$(BSC): $(DESTDIR)\*.sbr $(DESTDIR)\..\effect\*.sbr $(DESTDIR)\..\engine\*.sbr $(DESTDIR)\..\igc\*.sbr $(DESTDIR)\..\drones\*.sbr
  $(BSCMAKE) $(NOLOGO) /Iu /o$(BSC) $(DESTDIR)\*.sbr $(DESTDIR)\..\effect\*.sbr $(DESTDIR)\..\engine\*.sbr $(DESTDIR)\..\igc\*.sbr $(DESTDIR)\..\drones\*.sbr
