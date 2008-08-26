/////////////////////////////////////////////////////////////////////////////
// AppModeHelp.cpp : Declaration/implementation of the CAppModeHelp class.
//

#include "pch.h"
#include "MSRGuard.h"
#include "AppMode.h"
#include "AppModeHelp.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// External References

extern CMSRGuardModule g;


/////////////////////////////////////////////////////////////////////////////
//
//
class CAppModeHelp :
  public IAppMode
{
// IAppMode Interface Methods
public:
  STDMETHODIMP Run(int argc, TCHAR* argv[])
  {
    // Display the help string
    g.DisplayMessage(IDS_HELP);

    // Indicate success
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////
// Instantiation Method
//
IAppMode* CreateAppModeHelp()
{
  IAppMode* pAppMode = new CAppModeHelp;
  if (pAppMode)
    pAppMode->AddRef();
  return pAppMode;
}

