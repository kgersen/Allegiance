#############################################################################
#

!IFNDEF TARGET
!ERROR TARGET variable was not defined.
!ENDIF

!IFDEF FULL
THETARGET=$(TARGET)_Full
!ELSE
THETARGET=$(TARGET)
!ENDIF

all:
  @CD $(FEDROOT)\Src
  @$(MAKE) /NOLOGO $(THETARGET) $(FLAVOR)

