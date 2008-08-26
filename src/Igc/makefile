#
# Makefile for IGC.lib
#
# Environment variable Requirements:
#   
#    INCLUDE=foo           // can be set to anything, must be set
#    PATH=                 // must have proper nmake on path
#    FEDROOT=<path>        // MUST point to base of fed src enlistment
#
# nmake args:
#
#   Clean        // will delete all files in destination dir
#   Full         // will Clean, then build all derived files
#   Shell        // opens os-specific cmd/command shell (helps to debug build)
#   RETAIL=      // builds a retail build (default is DEBUG)
#                // no argument is used for debug builds
#
#
#  All derived files (temp files, .res, output of midl, *.obj, *.exe/.dll...
#  are placed directly into the appropriate tree under $(FEDROOT)\Objs.

#  The $(FEDROOT)\Objs tree is created on demand.
#  Since no derived files are placed in the source tree, multiple builds
#  can be run SIMULTANEOUSLY.


SRCROOT=IGC
MAINTARGROOT=IGC
INCLUDELOCAL=$(FEDSRC)\igc;$(FEDSRC)\_Utility;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;$(FEDSRC)\inc;$(FEDSRC)\clintlib;$(FEDSRC)\SoundEngine;$(FEDSRC)\WinTrek;$(FEDSRC)\offline;$(DESTDIR)
PCHINC=$(FEDSRC)\ZLib\*.h $(FEDSRC)\_Utility\*.h $(FEDSRC)\igc\*.h
PCHROOT=pch

MAINTARGEXT=lib

!include "..\makefile.inc"

OBJS= $(DESTDIR)\ClusterIGC.obj  \
      $(DESTDIR)\missionIGC.obj  \
      $(DESTDIR)\HullTypeIGC.obj  \
      $(DESTDIR)\ModelIGC.obj  \
      $(DESTDIR)\PartTypeIGC.obj  \
      $(DESTDIR)\asteroidIGC.obj  \
      $(DESTDIR)\ProjectileTypeIGC.obj  \
      $(DESTDIR)\WeaponIGC.obj  \
      $(DESTDIR)\Private.obj  \
      $(DESTDIR)\ProjectileIGC.obj  \
      $(DESTDIR)\ShipIGC.obj  \
      $(DESTDIR)\WarpIGC.obj \
      $(DESTDIR)\BuoyIGC.obj \
      $(DESTDIR)\ShieldIGC.obj \
      $(DESTDIR)\CloakIGC.obj \
      $(DESTDIR)\StationIGC.obj \
      $(DESTDIR)\TreasureIGC.obj \
      $(DESTDIR)\AfterburnerIGC.obj \
      $(DESTDIR)\packigc.obj \
      $(DESTDIR)\magazineigc.obj \
      $(DESTDIR)\dispenserigc.obj \
      $(DESTDIR)\missileTypeigc.obj \
      $(DESTDIR)\probeigc.obj \
      $(DESTDIR)\probetypeigc.obj \
      $(DESTDIR)\mineigc.obj \
      $(DESTDIR)\mineTypeigc.obj \
      $(DESTDIR)\missileigc.obj \
      $(DESTDIR)\launchertypeigc.obj \
      $(DESTDIR)\chafftypeigc.obj \
      $(DESTDIR)\chaffigc.obj \
      $(DESTDIR)\common.obj \
      $(DESTDIR)\civilizationIGC.obj \
      $(DESTDIR)\sideIGC.obj \
      $(DESTDIR)\stationTypeIGC.obj \
      $(DESTDIR)\droneTypeIGC.obj \
      $(DESTDIR)\developmentIGC.obj \
      $(DESTDIR)\bucketIGC.obj \
      $(DESTDIR)\treasuresetigc.obj \
      $(DESTDIR)\mapmakerigc.obj \
      $(DESTDIR)\pch.obj

LIBARGS= $(NOLOGO) /out:$(MAINTARGFULLPATH)

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)

$(DESTDIR)\ClusterIGC.obj:        $(PCHFILE)
$(DESTDIR)\missionIGC.obj:        $(PCHFILE)
$(DESTDIR)\HullTypeIGC.obj:       $(PCHFILE)
$(DESTDIR)\ModelIGC.obj:          $(PCHFILE)
$(DESTDIR)\PartTypeIGC.obj:       $(PCHFILE)
$(DESTDIR)\asteroidIGC.obj:       $(PCHFILE)
$(DESTDIR)\ProjectileTypeIGC.obj: $(PCHFILE)
$(DESTDIR)\WeaponIGC.obj:         $(PCHFILE)
$(DESTDIR)\Private.obj:           $(PCHFILE)
$(DESTDIR)\ProjectileIGC.obj:     $(PCHFILE)
$(DESTDIR)\ShipIGC.obj:           $(PCHFILE)
$(DESTDIR)\WarpIGC.obj:           $(PCHFILE)
$(DESTDIR)\BuoyIGC.obj:           $(PCHFILE)
$(DESTDIR)\ShieldIGC.obj:         $(PCHFILE)
$(DESTDIR)\CloakIGC.obj:          $(PCHFILE)
$(DESTDIR)\StationIGC.obj:		  $(PCHFILE)
$(DESTDIR)\TreasureIGC.obj:		  $(PCHFILE)

$(DESTDIR)\AfterburnerIGC.obj:	  $(PCHFILE)
$(DESTDIR)\PackIGC.obj:	          $(PCHFILE)
$(DESTDIR)\MagazineIGC.obj:	      $(PCHFILE)
$(DESTDIR)\DispenserIGC.obj:      $(PCHFILE)
$(DESTDIR)\MissileTypeIGC.obj:    $(PCHFILE)
$(DESTDIR)\ProbeIGC.obj:          $(PCHFILE)
$(DESTDIR)\ProbeTypeIGC.obj:      $(PCHFILE)
$(DESTDIR)\MineIGC.obj:           $(PCHFILE)
$(DESTDIR)\MineTypeIGC.obj:       $(PCHFILE)
$(DESTDIR)\launcherTypeIGC.obj:   $(PCHFILE)
$(DESTDIR)\MissileIGC.obj:        $(PCHFILE)
$(DESTDIR)\common.obj:	          $(PCHFILE)
$(DESTDIR)\BuildingEffectIGC.obj: $(PCHFILE)
$(DESTDIR)\chaffTypeigc.obj:      $(PCHFILE)
$(DESTDIR)\chaffigc.obj:          $(PCHFILE)
$(DESTDIR)\civilizationIGC.obj:	  $(PCHFILE)
$(DESTDIR)\stationTypeIGC.obj:	  $(PCHFILE)
$(DESTDIR)\sideIGC.obj:	          $(PCHFILE)
$(DESTDIR)\droneTypeIGC.obj:      $(PCHFILE)
$(DESTDIR)\bucketIGC.obj:	      $(PCHFILE)
$(DESTDIR)\mapmakerIGC.obj:	      $(PCHFILE)
$(DESTDIR)\treasuresetigc.obj:    $(PCHFILE)
$(DESTDIR)\developmentIGC.obj:    $(PCHFILE)

