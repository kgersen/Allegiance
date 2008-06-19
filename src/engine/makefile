#
# Makefile for Engine.lib
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


MAINTARGROOT=Engine
MAINTARGEXT=lib
INCLUDELOCAL=$(FEDSRC)\Engine;$(FEDSRC)\ZLib

# Imago -  lol
CARGSLOCAL=/D DIRECTINPUT_VERSION=0x700

PCHROOT=pch
PCHINC=$(FEDSRC)\ZLib\*.h
NODEPENDENCIES=1

!include "..\makefile.inc"

OBJS= $(DESTDIR)\dderror.obj       \
      $(DESTDIR)\dinput.obj        \
      $(DESTDIR)\videoimage.obj    \
      $(DESTDIR)\geometry.obj      \
      $(DESTDIR)\bspgeo.obj        \
      $(DESTDIR)\image.obj         \
      $(DESTDIR)\bounds.obj        \
      $(DESTDIR)\surface.obj       \
      $(DESTDIR)\ddsurface.obj     \
      $(DESTDIR)\font.obj          \
      $(DESTDIR)\raster.obj        \
      $(DESTDIR)\softraster.obj    \
!IFDEF DREAMCAST
      $(DESTDIR)\dcdevice3d.obj    \
!ELSE
      $(DESTDIR)\device3d.obj      \
!ENDIF
      $(DESTDIR)\context.obj       \
      $(DESTDIR)\d3ddevice.obj     \
      $(DESTDIR)\dddevice.obj      \
      $(DESTDIR)\engine.obj        \
      $(DESTDIR)\material.obj      \
      $(DESTDIR)\palette.obj       \
      $(DESTDIR)\transform.obj     \
      $(DESTDIR)\transform2.obj    \
      $(DESTDIR)\keyframe.obj      \
      $(DESTDIR)\xfile.obj         \
      $(DESTDIR)\mdl.obj           \
      $(DESTDIR)\mdlbin.obj        \
      $(DESTDIR)\caption.obj       \
      $(DESTDIR)\engineapp.obj     \
      $(DESTDIR)\enginewindow.obj  \
      $(DESTDIR)\value.obj         \
      $(DESTDIR)\namespace.obj     \
      $(DESTDIR)\modeler.obj       \
      $(DESTDIR)\pixelformat.obj   \
      $(DESTDIR)\camera.obj        \
      $(DESTDIR)\viewport.obj      \
      $(DESTDIR)\pane.obj          \
      $(DESTDIR)\toppane.obj       \
      $(DESTDIR)\button.obj        \
      $(DESTDIR)\controls.obj      \
      $(DESTDIR)\popup.obj         \
      $(DESTDIR)\menu.obj          \
      $(DESTDIR)\paneimage.obj     \
      $(DESTDIR)\frameimage.obj    \
      $(DESTDIR)\pch.obj

#
# Dependecies start here
#

$(MAINTARGFULLPATH): $(DESTDIR) $(OBJS)
    lib $(LIBARGS) $(OBJS)

$(DESTDIR)\dinput.obj:         $(PCHFILE)
$(DESTDIR)\value.obj:          $(PCHFILE)
$(DESTDIR)\namespace.obj:      $(PCHFILE)
$(DESTDIR)\modeler.obj:        $(PCHFILE)
$(DESTDIR)\dderror.obj:        $(PCHFILE)
$(DESTDIR)\geometry.obj:       $(PCHFILE)
$(DESTDIR)\bspgeo.obj:         $(PCHFILE)
$(DESTDIR)\image.obj:          $(PCHFILE)
$(DESTDIR)\matrix.obj:         $(PCHFILE)
$(DESTDIR)\bounds.obj:         $(PCHFILE)
$(DESTDIR)\transform.obj:      $(PCHFILE)
$(DESTDIR)\transform2.obj:     $(PCHFILE)
$(DESTDIR)\keyframe.obj:       $(PCHFILE)
$(DESTDIR)\xfile.obj:          $(PCHFILE)
$(DESTDIR)\camera.obj:         $(PCHFILE)
$(DESTDIR)\viewport.obj:       $(PCHFILE)
$(DESTDIR)\pane.obj:           $(PCHFILE)
$(DESTDIR)\toppane.obj:        $(PCHFILE)
$(DESTDIR)\button.obj:         $(PCHFILE)
$(DESTDIR)\controls.obj:       $(PCHFILE)
$(DESTDIR)\popup.obj:          $(PCHFILE)
$(DESTDIR)\menu.obj:           $(PCHFILE)
$(DESTDIR)\paneimage.obj:      $(PCHFILE)
$(DESTDIR)\pixelformat.obj:    $(PCHFILE)
$(DESTDIR)\caption.obj:        $(PCHFILE)
$(DESTDIR)\engineapp.obj:      $(PCHFILE)
$(DESTDIR)\enginewindow.obj:   $(PCHFILE)
$(DESTDIR)\d3ddevice.obj:      $(PCHFILE)
$(DESTDIR)\dddevice.obj:       $(PCHFILE)
$(DESTDIR)\engine.obj:         $(PCHFILE)
$(DESTDIR)\surface.obj:        $(PCHFILE)
$(DESTDIR)\ddsurface.obj:      $(PCHFILE)
$(DESTDIR)\font.obj:           $(PCHFILE)
$(DESTDIR)\raster.obj:         $(PCHFILE)
$(DESTDIR)\softraster.obj:     $(PCHFILE)
!IFDEF DREAMCAST
$(DESTDIR)\dcdevice3D.obj:     $(PCHFILE)
!ELSE
$(DESTDIR)\device3D.obj:       $(PCHFILE)
!ENDIF
$(DESTDIR)\context.obj:        $(PCHFILE)
$(DESTDIR)\material.obj:       $(PCHFILE)
$(DESTDIR)\palette.obj:        $(PCHFILE)
$(DESTDIR)\mdl.obj:            $(PCHFILE)
$(DESTDIR)\mdlbin.obj:         $(PCHFILE)
$(DESTDIR)\frameimage.obj:     $(PCHFILE)
