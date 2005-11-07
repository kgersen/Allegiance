/////////////////////////////////////////////////////////////////////////////
// AppModeCreate.cpp : Declaration/implementation of the CAppModeCreate class.
//

#include "pch.h"
#include "AppModeDebug.h"
#include "AppModeCreate.h"


/////////////////////////////////////////////////////////////////////////////
//
//
class CAppModeCreate :
  public CAppModeDebug<CAppModeCreate>
{
// Construction
public:
  CAppModeCreate()
  {
  }

// IAppMode Interface Methods
public:
  STDMETHODIMP Run(int argc, TCHAR* argv[])
  {
    // Create the process command-line
    ZString strCommandLine(g.GetConfigString(TEXT("ApplicationName")));

    // Append any command-line-specified parameters
    ZString strParams(g.GetOptionString(ZString()));
    if (!strParams.IsEmpty())
    {
      if (!strCommandLine.IsEmpty())
        strCommandLine += TEXT(" ");
      strCommandLine += strParams;
    }

    // Copy the command line to a non-const buffer
    LPTSTR pszCommandLine = (LPTSTR)_alloca(strCommandLine.GetLength() + sizeof(TCHAR));
    lstrcpy(pszCommandLine, strCommandLine);

    // Create the debuggee process
    STARTUPINFO sui = {sizeof(sui)};
    PROCESS_INFORMATION pi;
    DWORD dwCreationFlags = DEBUG_ONLY_THIS_PROCESS;
    if (!::CreateProcess(NULL, pszCommandLine, NULL, NULL, false,
      dwCreationFlags, NULL, NULL, &sui, &pi))
    {
      HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
      return g.HandleError(hr, IDS_E_FMT_CREATEPROCESS, pszCommandLine);
    }

    // Delegate to base class
    return CAppModeDebug_Base::Run(argc, argv);
  }
};


/////////////////////////////////////////////////////////////////////////////
// Instantiation Method
//
IAppMode* CreateAppModeCreate()
{
  IAppMode* pAppMode = new CAppModeCreate;
  if (pAppMode)
    pAppMode->AddRef();
  return pAppMode;
}

