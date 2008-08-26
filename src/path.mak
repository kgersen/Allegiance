###################################################################
#
# Path
#
###################################################################

FEDEXT=$(FEDROOT)\extern

!IFDEF WINBOOTDIR

OSPATHS=$(WINBOOTDIR)\system;$(WINBOOTDIR);$(WINBOOTDIR)\Command
OSCMDSHELL=command

!ELSE

OSPATHS=$(SYSTEMROOT)\system32;$(WINDIR)
OSCMDSHELL=cmd

!ENDIF

!IFDEF DREAMCAST
PATH=$(FEDEXT)\dreamcast\vc\bin;$(OSPATHS);$(FEDEXT)\dreamcast\SharedIDE\Bin;$(FEDEXT)\dreamcast\WCE\Bin
!ELSE
PATH=$(FEDEXT)\vc\bin;$(OSPATHS);$(FEDEXT)\SharedIDE\Bin;$(FEDROOT)\src\setup;$(FEDROOT)\src\tools\build
!ENDIF
