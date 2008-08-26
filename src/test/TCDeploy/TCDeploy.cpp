/////////////////////////////////////////////////////////////////////////////
// TCDeploy.cpp | Implementation of DLL Exports.
//

#include "pch.h"
#include "SrcInc.h"
#include "resource.h"
#include <TCDeploy.h>
#include "TCDeploy_i.c"
#include "dlldatax.h"
#include <TCLib.h>
#include <..\TCLib\AutoHandle.h>
#include <..\TCLib\WinApp.h>
#include <shlobj.h>
#include <comdef.h>

#if defined(_UNICODE) || defined(UNICODE)
  #define IShellLinkPtr IShellLinkWPtr
#else // defined(_UNICODE) || defined(UNICODE)
  #define IShellLinkPtr IShellLinkAPtr
#endif // defined(_UNICODE) || defined(UNICODE)


/////////////////////////////////////////////////////////////////////////////
// Note: Proxy/Stub Information
//    To merge the proxy/stub code into the object DLL, add the file 
//    dlldatax.c to the project.  Make sure precompiled headers 
//    are turned off for this file, and add _MERGE_PROXYSTUB to the 
//    defines for the project.  
//
//    If you are not running WinNT4.0 or Win95 with DCOM, then you
//    need to remove the following define from dlldatax.c
//    #define _WIN32_WINNT 0x0400
//
//    Further, if you are running MIDL without /Oicf switch, you also 
//    need to remove the following define from dlldatax.c.
//    #define USE_STUBLESS_PROXY
//
//    Modify the custom build rule for TCDeploy.idl by adding the following 
//    files to the Outputs.
//      TCDeploy_p.c
//      dlldata.c
//    To build a separate proxy/stub DLL, 
//    run nmake -f TCDeployps.mk in the project directory.


/////////////////////////////////////////////////////////////////////////////
// Global Initialization

#ifdef _MERGE_PROXYSTUB
  extern "C" HINSTANCE hProxyDll;
#endif

CDllModule   _Module;  // ATL mandatory symbol
TCWinAppDLL g_app;


/////////////////////////////////////////////////////////////////////////////
// External Declarations

TC_OBJECT_EXTERN(TCDeployment)
TC_OBJECT_EXTERN(TCDeploymentVersion)
TC_OBJECT_EXTERN_NON_CREATEABLE(TCDeploymentProcess)


/////////////////////////////////////////////////////////////////////////////
// Object Map

BEGIN_OBJECT_MAP(ObjectMap)
  TC_OBJECT_ENTRY(CLSID_TCDeployment, TCDeployment)
  TC_OBJECT_ENTRY(CLSID_TCDeploymentVersion, TCDeploymentVersion)
  TC_OBJECT_ENTRY_NON_CREATEABLE(CLSID_TCDeploymentProcess, TCDeploymentProcess)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  lpReserved;

  #ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
      return false;
  #endif

  if (DLL_PROCESS_ATTACH == dwReason)
  {
    TRACE_MODULE_INIT(hInstance, true);
    _Module.Init(ObjectMap, hInstance);
    DisableThreadLibraryCalls(hInstance);

    // Get the module path and file name
    TCHAR szModule[_MAX_PATH];
    GetModuleFileName(hInstance, szModule, sizeofArray(szModule));

    // Use the FNAME of the module as the temp file extension
    TCHAR szFname[_MAX_FNAME];
    _tsplitpath(szModule, NULL, NULL, szFname, NULL);

    // Get the path to the Windows directory
    TCHAR szPath[_MAX_PATH];
    ::GetWindowsDirectory(szPath, sizeofArray(szPath));

    // Use the process id to create a unique filename for a temp file
    TCHAR szPID[_MAX_EXT];
    wsprintf(szPID, TEXT("%u"), ::GetCurrentProcessId());
    _tmakepath(szModule, NULL, szPath, szPID, szFname);

    // Create a temporary file that gets deleted when the process exits
    DWORD dwAttr = FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE;
    HANDLE hFile = ::CreateFile(szModule, GENERIC_WRITE, FILE_SHARE_READ,
      NULL, CREATE_ALWAYS, dwAttr, NULL);
    // Note: We NEVER close this handle so that it gets closed and deleted
    //       when the OS terminates the process. This allows us to loop on
    //       its existence from a batch file to determine when it has been
    //       completely unloaded. Since multiple processes may have the
    //       module loaded, the temporary filename extension is the string
    //       form of the current process ID. Thus, a batch file can loop on
    //       the expression:
    //
    //       :WaitForDll
    //       if exist %windir%\*.TCDeploy goto WaitForDll
    //

    // Do NOT close this handle
    hFile = NULL;
  }
  else if (DLL_PROCESS_DETACH == dwReason)
  {
    _Module.EchoFlush();
    _Module.Term();
    TRACE_MODULE_INIT(hInstance, false);
  }
  return true;    // ok
}


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
  #ifdef _MERGE_PROXYSTUB
    if (S_OK != PrxDllCanUnloadNow())
      return S_FALSE;
  #endif

  return _Module.GetLockCount() ? S_FALSE : S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  #ifdef _MERGE_PROXYSTUB
    if (S_OK == PrxDllGetClassObject(rclsid, riid, ppv))
      return S_OK;
  #endif

  return _Module.GetClassObject(rclsid, riid, ppv);
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
  #ifdef _MERGE_PROXYSTUB
    RETURN_FAILED(PrxDllRegisterServer());
  #endif

  // registers object, typelib and all interfaces in typelib
  RETURN_FAILED(_Module.RegisterServer(TRUE));

  // Register the component category
  RETURN_FAILED(RegisterComponentCategory(CATID_TCDeploy,
    L"Allegiance Test Deployment Objects"));

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
//
STDAPI DllUnregisterServer(void)
{
  #ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
  #endif

  // Unregister the component category
  UnregisterComponentCategory(CATID_TCDeploy);

  _Module.UnregisterServer();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Command - An entry point usable by the RunDll32 utility.
//
void CALLBACK Command(HWND hwnd, HINSTANCE hinst, LPSTR pszCmdLine, int nCmdShow)
{
  // Set the error mode
  const UINT uErrorMode = SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX
    | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOOPENFILEERRORBOX;
  UINT uErrorModePrev = ::SetErrorMode(uErrorMode);

  // Enclose everything that could fail within an exception handler
  HRESULT hr = E_FAIL;
  HRESULT hrCoInit = E_FAIL;
  __try
  {
    // Just display syntax when the command-line is empty
    if (!pszCmdLine || '\0' == *pszCmdLine)
    {
      _Module.Syntax();
      hr = S_FALSE;
    }
    else
    {
      // Enter this thread into a COM STA
      hrCoInit = CoInitialize(NULL);

      // Get the path of this DLL
      char szModule[_MAX_PATH];
      DWORD cchModule = GetModuleFileNameA(_Module.GetModuleInstance(),
        szModule, sizeof(szModule));

      // Create a command line by concatenating the module name to arguments
      int cchSafety = 0x10;
      int cch = cchModule + cchSafety + pszCmdLine ? lstrlenA(pszCmdLine) : 0;
      LPTSTR psz = (LPTSTR)_alloca(cch * sizeof(TCHAR));
      wsprintf(psz, TEXT("%hs %hs"), szModule, pszCmdLine);

      // Parse the command line
      hr = _Module.ParseCommandLine(psz);
    }
  }
  __except(1)
  {
    hr = HRESULT_FROM_WIN32(GetExceptionCode());
  }

  // Report the results
  __try
  {
    // Remove this thread from the COM STA, if we entered it
    if (SUCCEEDED(hrCoInit))
      CoUninitialize();

    // Errors have already been reported, just flush the echo buffer
    _Module.EchoFlush();
  }
  __except(1)
  {
    // Not much we can do at this point
    OutputDebugStringA("TCDeploy.dll::Command(): Exception while reporting error.");
  }

  // Restore the previous error mode
  ::SetErrorMode(uErrorModePrev);
}


/////////////////////////////////////////////////////////////////////////////
// CleanTempFiles - An entry point usable by the RunDll32 utility.
//
void CALLBACK CleanTempFiles(HWND, HINSTANCE, LPSTR, int)
{
  // Set the error mode
  const UINT uErrorMode = SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX
    | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOOPENFILEERRORBOX;
  UINT uErrorModePrev = ::SetErrorMode(uErrorMode);

  // Enclose everything that could fail within an exception handler
  HANDLE hff = NULL;
  __try
  {
    // Get the module path and file name
    TCHAR szModule[_MAX_PATH];
    GetModuleFileName(_Module.GetModuleInstance(), szModule,
      sizeofArray(szModule));

    // Use the FNAME of the module as the temp file extension
    TCHAR szFname[_MAX_FNAME];
    _tsplitpath(szModule, NULL, NULL, szFname, NULL);

    // Get the path to the Windows directory
    TCHAR szPath[_MAX_PATH];
    ::GetWindowsDirectory(szPath, sizeofArray(szPath));

    // Create the wildcard specification
    _tmakepath(szModule, NULL, szPath, TEXT("*"), szFname);

    // Iterate thru all files that match the wildcard
    WIN32_FIND_DATA ffd = {0};
    hff = ::FindFirstFile(szModule, &ffd);
    bool bContinue = hff && INVALID_HANDLE_VALUE != hff;
    while (bContinue)
    {
      // Get the name portion of the filename
      _tsplitpath(ffd.cFileName, NULL, NULL, szFname, NULL);

      // Convert it to a process ID
      DWORD idProcess = _tcstoul(szFname, NULL, 10);
      if (idProcess && ::GetCurrentProcessId() != idProcess)
      {
        // Create the fully-qualified path name of the file
        _tmakepath(szModule, NULL, szPath, ffd.cFileName, NULL);
      
        // Delete the file, specifically ignoring any 'access denied' errors
        DeleteFile(szModule);
      }
    
      // Get the next file in the directory
      bContinue = !!::FindNextFile(hff, &ffd);
    }
  }
  __except(1)
  {
  }

  // Close the file find handle
  if (hff && INVALID_HANDLE_VALUE != hff)
    ::FindClose(hff);

  // Restore the previous error mode
  ::SetErrorMode(uErrorModePrev);
}


/////////////////////////////////////////////////////////////////////////////
// Operations


HRESULT CDllModule::OnNoExit(int argc, TCHAR* argv[], int* pnArgs)
{
  // Instantiate the TCDeployment object
  ITCDeploymentClassPtr spdc;
  RETURN_FAILED(spdc.CreateInstance(CLSID_TCDeployment, NULL,
    CLSCTX_LOCAL_SERVER));

  // Set the NoExit property
  RETURN_FAILED(spdc->put_NoExit(VARIANT_TRUE));
  return S_OK;
}


HRESULT CDllModule::OnExit(int argc, TCHAR* argv[], int* pnArgs)
{
  // Instantiate the TCDeployment object
  ITCDeploymentClassPtr spdc;
  RETURN_FAILED(spdc.CreateInstance(CLSID_TCDeployment, NULL,
    CLSCTX_LOCAL_SERVER));

  // Reset the NoExit property
  RETURN_FAILED(spdc->put_NoExit(VARIANT_FALSE));
  // TODO: RETURN_FAILED(spdc->Quit());
  return S_OK;
}


HRESULT CDllModule::OnInstall(int argc, TCHAR* argv[], int* pnArgs)
{
  // Get the name of the startup path
  TCHAR szStartup[_MAX_PATH];
  if (!SHGetSpecialFolderPath(NULL, szStartup, CSIDL_STARTUP, false))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Create the full filename of the shell link
  if (TEXT('\\') != szStartup[lstrlen(szStartup) - 1])
    lstrcat(szStartup, TEXT("\\"));
  lstrcat(szStartup, TEXT("TCDeployment Win9x Launch.lnk"));

  // Get the module name
  TCHAR szArgs[_MAX_PATH], szFname[_MAX_FNAME], szExt[_MAX_EXT];
  GetModuleFileName(_Module.GetModuleInstance(), szArgs, sizeofArray(szArgs));
  _tsplitpath(szArgs, NULL, NULL, szFname, szExt);
  _tmakepath(szArgs, NULL, NULL, szFname, szExt);

  // Create the command line arguments of the shell link
  lstrcat(szArgs, TEXT(",Command -NoExit"));

  // Create a shell link object
  IShellLinkPtr spLink;
  RETURN_FAILED(spLink.CreateInstance(CLSID_ShellLink));

  // Set the values of the shell link object
  RETURN_FAILED(spLink->SetPath(TEXT("RunDll32")));
  RETURN_FAILED(spLink->SetArguments(szArgs));
  RETURN_FAILED(spLink->SetDescription(TEXT("TCDeployment pre-launch")));

  // Save this shortcut
  USES_CONVERSION;
  IPersistFilePtr spFile(spLink);
  RETURN_FAILED(spFile->Save(T2COLE(szStartup), false));

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

HRESULT CDllModule::GetAccessSecurity(PSECURITY_DESCRIPTOR* psd, DWORD* pcb)
{
  // Perform default processing under NT
  if (IsWinNT())
    return TCComModuleBase::GetAccessSecurity(psd, pcb);

  // Initialize the [out] parameter
  if (pcb)
    *pcb = 0;

  // HACKHACK: Load the binary data for Win9x access permission
  void* pvData;
  DWORD cbData;
  RETURN_FAILED(LockAndLoadResource(_Module.GetResourceInstance(),
    TEXT("AccessPermission"), MAKEINTRESOURCE(IDR_AccessPermission_9X),
    &pvData, &cbData));

  // Allocate a buffer to be returned
  BYTE* pbData = new BYTE[cbData];
  if (!pbData)
    return E_OUTOFMEMORY;

  // Copy the data into the buffer
  CopyMemory(pbData, pvData, cbData);

  // Copy the values to the [out] parameters
  *psd = pbData;
  if (pcb)
    *pcb = cbData;
  return S_OK;
}

void CDllModule::DisplaySyntax()
{
  Echo(IsWin9x()? IDS_SYNTAX_9X : IDS_SYNTAX_NT);
  EchoFlush();
}


LPCTSTR CDllModule::GetRegInfo(UINT& idResource, bool& bRegTypeLib)
{
  bRegTypeLib = false;
  return TEXT("TCDeployment");
}

