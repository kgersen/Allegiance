!if defined(VERBOSE)
!if [echo Checking include file dependencies...]
!endif
!endif

!if [$(MKDEP) -p $(DESTDIR)\ -f $(DESTDIR)\$(MAINTARGROOT).dep -!I $(INCLUDESYS) $(CARGS) $(CARGSPCH) $(SRCDIR)\*.cpp $(SRCDIR)\$(PCHROOT).cpp=.pch 2>NUL]
!endif

!if EXIST($(DESTDIR)\$(MAINTARGROOT).dep)
!include "$(DESTDIR)\$(MAINTARGROOT).dep"
!endif
