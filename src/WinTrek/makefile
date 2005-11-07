#
# Makefile for WinTrek.exe
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

#	use _BCHK=1 if you want to do a boundschecker build.
#   note:  you must have BC already installed locally and define BCBINDIR to
#          point to the directory where nmcl.exe and nmlink.exe reside.  You 
#          can use the BCOPTS variable to add options to the BC compile.

# keep the debug info in the exe for retail builds, so we can strip it out and leave the FPO info
!IFDEF retail
SPLITSYMBOLS=1
!endif

SRCROOT=Wintrek
MAINTARGROOT=Allegiance
MAINTARGEXT=exe
CARGSLOCAL=/D"WINTREK" /D "igc_static" /D "IGC_SHIP" /D "_USRDLL" /D"DLL" /I$(FEDROOT)\Objs\$(FLAVOR)\AGC /I$(FEDROOT)\Objs\$(FLAVOR)\AllSrv32
INCLUDELOCAL=$(FEDEXT)\vc\mfc\inc;$(FEDSRC)\Effect;$(FEDSRC)\Engine;$(FEDSRC)\ZLib;$(FEDSRC)\_Utility;$(FEDSRC)\Igc;$(DESTDIR)\..\igc;$(FEDSRC)\clintlib;$(FEDSRC)\SoundEngine;$(FEDSRC)\agc;$(FEDSRC)\FedSrv;$(FEDSRC)\Training;$(FEDSRC)\lobby;$(FEDSRC)\test\inc;$(FEDROOT)\Objs\$(FLAVOR)\AGC;$(FEDROOT)\Objs\$(FLAVOR)\AllSrv32

UNWINDCARG=/GX

PCHROOT=pch
PCHINC=                                    \
    $(FEDSRC)\Effect\*.h                   \
    $(FEDSRC)\Engine\*.h                   \
    $(FEDSRC)\ZLib\*.h                     \
    $(FEDSRC)\_Utility\*.h                 \
    $(FEDSRC)\Igc\*.h                      \
    $(FEDSRC)\inc\*.h                      \
    $(FEDSRC)\clintlib\*.h                 \
    $(FEDSRC)\soundengine\redbooksound.h   \
    $(FEDSRC)\soundengine\soundbase.h      \
    $(FEDSRC)\soundengine\soundengine.h    \
    $(FEDSRC)\soundengine\soundtemplates.h \
    $(FEDSRC)\soundengine\soundutil.h      \
    $(FEDSRC)\zone\*.h                     \
    $(FEDSRC)\lobby\messagesAll.h          \
    $(FEDSRC)\lobby\messagesLC.h           \
    $(FEDSRC)\Club\ClubMessages.h


!include "../makefile.inc"


# library of utility func's

OBJS= \
    $(DESTDIR)\soundinit.obj                \
    $(DESTDIR)\treksound.obj                \
    $(DESTDIR)\WinTrek.obj                  \
    $(DESTDIR)\CharInfoScreen.obj           \
    $(DESTDIR)\gamesite.obj                 \
    $(DESTDIR)\trekigc.obj                  \
    $(DESTDIR)\TeamScreen.obj               \
    $(DESTDIR)\TeamPane.obj                 \
    $(DESTDIR)\TeleportPane.obj             \
    $(DESTDIR)\Load.obj                     \
    $(DESTDIR)\GameScreen.obj               \
    $(DESTDIR)\IntroScreen.obj              \
    $(DESTDIR)\NewGameScreen.obj            \
    $(DESTDIR)\GameOverScreen.obj           \
    $(DESTDIR)\GameStartingScreen.obj       \
    $(DESTDIR)\ZoneClubScreen.obj           \
    $(DESTDIR)\SquadsScreen.obj             \
    $(DESTDIR)\TrekCtrls.obj                \
    $(DESTDIR)\Loadout.obj                  \
    $(DESTDIR)\hangarscreen.obj             \
    $(DESTDIR)\LeaderBoardScreen.obj        \
    $(DESTDIR)\TrekApp.obj                  \
    $(DESTDIR)\consoledata.obj              \
    $(DESTDIR)\console.obj                  \
    $(DESTDIR)\selpane.obj                  \
    $(DESTDIR)\indicator.obj                \
    $(DESTDIR)\inventoryimage.obj           \
    $(DESTDIR)\trekmdl.obj                  \
    $(DESTDIR)\RadarImage.obj               \
    $(DESTDIR)\trekInput.obj                \
    $(DESTDIR)\cmdview.obj                  \
    $(DESTDIR)\sectormap.obj                \
    $(DESTDIR)\chatpane.obj                 \
    $(DESTDIR)\logon.obj                    \
    $(DESTDIR)\passworddialog.obj           \
    $(DESTDIR)\cdkeydialog.obj              \
    $(DESTDIR)\videoscreen.obj              \
    $(DESTDIR)\trainingscreen.obj           \
    $(DESTDIR)\Slideshow.obj                \
    $(DESTDIR)\TrainingSlideshow.obj        \
    $(DESTDIR)\PostTrainingSlideshow.obj    \
    $(DESTDIR)\ZoneSquad.obj                \
    $(DESTDIR)\ZoneEventsScreen.obj         \
    $(DESTDIR)\netinfo.obj                  \
    $(DESTDIR)\GameTypes.obj                 


USERLIBS= \
    $(DESTDIR)\..\zlib\zlib.lib         \
    $(DESTDIR)\..\soundengine\soundengine.lib         \
    $(DESTDIR)\..\_utility\Utility.lib   \
    $(DESTDIR)\..\engine\Engine.lib     \
    $(DESTDIR)\..\effect\Effect.lib     \
    $(DESTDIR)\..\igc\igc.lib           \
    $(DESTDIR)\..\clintlib\clintlib.lib \
    $(DESTDIR)\..\agc\AGCGuid.lib       \
    $(DESTDIR)\..\training\training.lib \
    $(DESTDIR)\$(MAINTARGROOT).res

!include "../makeexe.inc"

CopyClient: First
    copy $(DESTDIR)\allegiance.exe $(DESTDIR)\..\..\artwork
    copy $(DESTDIR)\allegiance.pdb $(DESTDIR)\..\..\artwork

$(DESTDIR)\netinfo.obj:              $(PCHFILE)
$(DESTDIR)\CharInfoScreen.obj:       $(PCHFILE)
$(DESTDIR)\gamesite.obj:             $(PCHFILE)
$(DESTDIR)\trekigc.obj:              $(PCHFILE)
$(DESTDIR)\WinTrek.obj:              $(PCHFILE)
$(DESTDIR)\soundinit.obj:            $(PCHFILE)
$(DESTDIR)\treksound.obj:            $(PCHFILE)
$(DESTDIR)\TeamScreen.obj:           $(PCHFILE)
$(DESTDIR)\TeamPane.obj:             $(PCHFILE)
$(DESTDIR)\TeleportPane.obj:         $(PCHFILE)
$(DESTDIR)\Load.obj:                 $(PCHFILE)
$(DESTDIR)\GameScreen.obj:           $(PCHFILE)
$(DESTDIR)\IntroScreen.obj:          $(PCHFILE)
$(DESTDIR)\NewGameScreen.obj:        $(PCHFILE)
$(DESTDIR)\GameOverScreen.obj:       $(PCHFILE)
$(DESTDIR)\GameStartingScreen.obj:   $(PCHFILE)
$(DESTDIR)\ZoneClubScreen.obj:       $(PCHFILE)
$(DESTDIR)\SquadsScreen.obj:         $(PCHFILE)
$(DESTDIR)\TrekCtrls.obj:            $(PCHFILE)
$(DESTDIR)\Loadout.obj:              $(PCHFILE)
$(DESTDIR)\hangarscreen.obj:         $(PCHFILE)
$(DESTDIR)\LeaderBoardScreen.obj:    $(PCHFILE)
$(DESTDIR)\TrekApp.obj:              $(PCHFILE)
$(DESTDIR)\consoledata.obj:          $(PCHFILE)
$(DESTDIR)\console.obj:              $(PCHFILE)
$(DESTDIR)\selpane.obj:              $(PCHFILE)
$(DESTDIR)\indicator.obj:            $(PCHFILE)
$(DESTDIR)\inventoryimage.obj:       $(PCHFILE)
$(DESTDIR)\trekmdl.obj:              $(PCHFILE)
$(DESTDIR)\RadarImage.obj:           $(PCHFILE)
$(DESTDIR)\trekInput.obj:            $(PCHFILE)
$(DESTDIR)\cmdview.obj:              $(PCHFILE)
$(DESTDIR)\sectormap.obj:            $(PCHFILE)
$(DESTDIR)\chatpane.obj:             $(PCHFILE)
$(DESTDIR)\logon.obj:                $(PCHFILE)
$(DESTDIR)\passworddialog.obj:          $(PCHFILE)
$(DESTDIR)\cdkeydialog.obj:          $(PCHFILE)
$(DESTDIR)\videoscreen.obj:          $(PCHFILE)
$(DESTDIR)\trainingscreen.obj:       $(PCHFILE)
$(DESTDIR)\Slideshow.obj:            $(PCHFILE)
$(DESTDIR)\TrainingSlideshow.obj:    $(PCHFILE)
$(DESTDIR)\PostTrainingSlideshow.obj:$(PCHFILE)
$(DESTDIR)\ZoneSquad.obj:            $(PCHFILE)
$(DESTDIR)\ZoneEventsScreen.obj:     $(PCHFILE)
$(DESTDIR)\GameTypes.obj:            $(PCHFILE)

RARGS = /l 0x409 $(RARGSFLAVOR) /d"WINTREK" /I$(FEDSRC)\Lang\NonIntl\$(MAINTARGROOT) /i$(FEDSRC)\Lang\$(LANG)\$(MAINTARGROOT)

$(DESTDIR)\$(MAINTARGROOT).res: $(FEDSRC)\Lang\$(LANG)\$(MAINTARGROOT)\$(MAINTARGROOT).rc \
                                $(FEDSRC)\Inc\VerRes.rc
   copy $(FEDSRC)\Lang\$(LANG)\$(MAINTARGROOT)\$(MAINTARGROOT).rc $(DESTDIR)
   echo #define FLAVOR "$(FLAVOR)" >>$(DESTDIR)\$(MAINTARGROOT).rc
   type $(FEDSRC)\Inc\VerRes.rc >>$(DESTDIR)\$(MAINTARGROOT).rc
   $(RC) $(RARGS) /Fo$(DESTDIR)\$(MAINTARGROOT).res $(DESTDIR)\$(MAINTARGROOT).rc
