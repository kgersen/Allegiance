!if defined(VERBOSE)
!if [echo Checking include file dependencies...]
!endif
!endif

!if defined(VERBOSE)
!if [echo $(MKDEP) -p $(DESTDIR)\ -f $(DESTDIR)\$(MAINTARGROOT).dep -!I $(INCLUDESYS) $(CARGS) $(CARGSPCH) $(SRCDIR)\*.cpp $(SRCDIR)\$(PCHROOT).cpp=.pch 2>NUL]
!endif
!endif


!if [$(MKDEP) -p $(DESTDIR)\ -f $(DESTDIR)\$(MAINTARGROOT).dep -!I $(INCLUDESYS) $(CARGS) $(CARGSPCH) $(SRCDIR)\*.cpp $(SRCDIR)\$(PCHROOT).cpp=.pch]
!endif

!if EXIST($(DESTDIR)\$(MAINTARGROOT).dep)
!include "$(DESTDIR)\$(MAINTARGROOT).dep"
!endif
