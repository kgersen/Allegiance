/////////////////////////////////////////////////////////////////////////////
// AppModeAttach.cpp : Declaration/implementation of the CAppModeAttach class.
//

#include "pch.h"
#include "AppModeDebug.h"
#include "AppModeAttach.h"


/////////////////////////////////////////////////////////////////////////////
//
//
class CAppModeAttach :
  public CAppModeDebug<CAppModeAttach>
{
// Construction
public:
  CAppModeAttach()
  {
  }

// IAppMode Interface Methods
public:
  STDMETHODIMP Run(int argc, TCHAR* argv[])
  {
    ZString strPID;
    bool bFound = g.GetOptionValue("P", strPID);

    // Get the process ID from the string
    DWORD pid = _tcstoul(strPID, NULL, 0);

    // Attach to the specified process ID
    if (!::DebugActiveProcess(pid))
    {
      HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
      return g.HandleError(hr, IDS_E_FMT_DEBUGACTIVEPROCESS, (LPCTSTR)strPID);
    }

    // Delegate to base class
    return CAppModeDebug_Base::Run(argc, argv);
  }
};


/////////////////////////////////////////////////////////////////////////////
// Instantiation Method
//
IAppMode* CreateAppModeAttach()
{
  IAppMode* pAppMode = new CAppModeAttach;
  if (pAppMode)
    pAppMode->AddRef();
  return pAppMode;
}

