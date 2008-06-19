#pragma once

/////////////////////////////////////////////////////////////////////////////
// AppModeDebug.h: Declaration of the CAppModeDebug class creator.
//

#if defined(NOT_REALLY_INCLUDING_THESE)
  #include "pch.h"
  #include "MSRGuard.h"
#endif


#include "AppMode.h"
#include "LoadedModules.h"
#include "ToolHelp.h"

#pragma warning(disable:4867)

/////////////////////////////////////////////////////////////////////////////
// Macro useful for writing a ZString to an IStream pointer.
//
#define WRITE_STRING_TO_STREAM(p, s)                                        \
  RETURN_FAILED(p->Write((LPCTSTR)s, s.GetLength() * sizeof(TCHAR), NULL))


/////////////////////////////////////////////////////////////////////////////
//
//
template <class T>
class CAppModeDebug :
  public IAppMode
{
// Construction
public:
  CAppModeDebug();

// IAppMode Interface Methods
public:
  STDMETHODIMP Run(int argc, TCHAR* argv[]);

// Debug Event Handlers
private:
  DWORD HandleException        (bool* pbExit);
  DWORD HandleCreateThread     (bool* pbExit);
  DWORD HandleCreateProcess    (bool* pbExit);
  DWORD HandleExitThread       (bool* pbExit);
  DWORD HandleExitProcess      (bool* pbExit);
  DWORD HandleLoadDLL          (bool* pbExit);
  DWORD HandleUnloadDLL        (bool* pbExit);
  DWORD HandleOutputDebugString(bool* pbExit);
  DWORD HandleRIP              (bool* pbExit);
  DWORD HandleOther            (bool* pbExit);

// Implementation
private:
  DWORD ProcessDebugEvent(DEBUG_EVENT* pde, bool* pbExit);
  HRESULT ReportException();
  HRESULT ComposeExceptionReportParams();
  HRESULT CanonicalizeParamText(ZString strIn, ZString& strOut);
  HRESULT GetExceptionState(ZString& strOut);
  HRESULT GetMachineText(ZString& strOut);
  ZString ReadProcessString(void* pvAddr, bool fUnicode, int cch = -1);
  static LPCTSTR GetExceptionName(DWORD dwExceptionCode);
  ZString GetAddressModule(void* pvAddr);
  HRESULT FormatThreadState(IStream* pStm, DWORD dwThreadId, bool bFull);
  HRESULT FormatStackFrame(IStream* pStm, STACKFRAME& stk);
  HRESULT FormatModuleInfo(IStream* pStm, CLoadedModuleIt it);
  HRESULT FormatProcessesInfo(IStream* pStm);
  HRESULT FormatProcessInfo(IStream* pStm, PROCESSENTRY32* ppe);
  HRESULT FormatProcessInfo(IStream* pStm, LPCTSTR pszEXE);
  ZString GetSymbolPath();
  ZString GetSearchPath();
  ZString GetModuleWithPath(LPCTSTR pszModule);
  static BOOL CALLBACK ReadProcessMemoryProc(HANDLE hProcess, DWORD lpBaseAddress,
    PVOID lpBuffer, DWORD nSize, PDWORD lpNumberOfBytesRead);
  static PVOID CALLBACK FunctionTableAccessProc(HANDLE hProcess, DWORD AddrBase);
  static DWORD CALLBACK GetModuleBaseProc(HANDLE hProcess, DWORD Address);
  static DWORD CALLBACK TranslateAddressProc(HANDLE hProcess, HANDLE hThread,
    LPADDRESS lpaddr);

// Overrides
public:
  DWORD OnException        (bool* pbExit);
  DWORD OnCreateThread     (bool* pbExit);
  DWORD OnCreateProcess    (bool* pbExit);
  DWORD OnExitThread       (bool* pbExit);
  DWORD OnExitProcess      (bool* pbExit);
  DWORD OnLoadDLL          (bool* pbExit);
  DWORD OnUnloadDLL        (bool* pbExit);
  DWORD OnOutputDebugString(bool* pbExit);
  DWORD OnRIP              (bool* pbExit);
  DWORD OnOther            (bool* pbExit);

// Types
protected:
  typedef CAppModeDebug<T> CAppModeDebug_Base;
  typedef std::map<DWORD, CREATE_THREAD_DEBUG_INFO> XThreads;
  typedef XThreads::iterator                        XThreadIt;
  typedef std::map<ZString, ZString> XParams;
  typedef XParams::iterator          XParamIt;

// Data Members
protected:
  XThreads                  m_Threads;
  XParams                   m_Params;
  CLoadedModules            m_Images;
  CREATE_PROCESS_DEBUG_INFO m_cpdi;
  DWORD                     m_dwProcessId;
  HANDLE                    m_hProcessSym;
  DEBUG_EVENT*              m_pde;
  ZString                   m_strBrowser;
  ZString                   m_strRegGUID;
  ZString                   m_strAppGUID;
  ZString                   m_strSearchPath;
  TCHandle                  m_shEvent;
  CToolHelp                 m_libToolHelp;
};


/////////////////////////////////////////////////////////////////////////////

#include "MSRGuard.h"
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Construction

template <class T>
inline CAppModeDebug<T>::CAppModeDebug() :
  m_dwProcessId(0),
  m_hProcessSym(NULL),
  m_pde(NULL)
{
  ZeroMemory(&m_cpdi, sizeof(m_cpdi));
}


/////////////////////////////////////////////////////////////////////////////
// IAppMode Interface Methods

template <class T>
STDMETHODIMP CAppModeDebug<T>::Run(int argc, TCHAR* argv[])
{
  // Determine the default Web browser
  {
    CRegKey key;
    long lr = key.Open(HKEY_CLASSES_ROOT, TEXT("http\\shell\\open\\command"),
      KEY_READ);
    if (ERROR_SUCCESS != lr)
      return g.HandleError(HRESULT_FROM_WIN32(lr), IDS_E_NODEFBROWSER);
    HRESULT hr = LoadRegString(key, NULL, m_strBrowser);
    if (FAILED(hr))
      return g.HandleError(hr, IDS_E_NODEFBROWSER);
    if (m_strBrowser.IsEmpty())
      return g.HandleError(E_UNEXPECTED, IDS_E_NODEFBROWSER);

    // If the Web browser command doesn't have a %1, concatenate one onto it
    if (!_tcsstr(m_strBrowser, TEXT("%1")))
      m_strBrowser += TEXT(" \"%1\"");
  }

  // Read the registry to determine if a GUID already exists in the registry
  HRESULT hr = S_OK;
  HKEY hkeyRoot = g.HKEYFromString(g.GetConfigString("RegRoot"));
  ZString strKey(g.GetConfigString("RegKey"));
  {
    CRegKey key;
    long lr = key.Open(hkeyRoot, strKey, KEY_READ);
    if (SUCCEEDED(hr = (ERROR_SUCCESS == lr) ? S_OK : HRESULT_FROM_WIN32(lr)))
      hr = LoadRegString(key, TEXT("GUID"), m_strRegGUID);
  }

  // Create a GUID and enter it into the registry
  if (FAILED(hr))
  {
    // Create/open the registry key for writing
    CRegKey key;
    long lr = key.Create(hkeyRoot, strKey);
    if (ERROR_SUCCESS == lr)
    {
      // Create a GUID to uniquely identify this machine
      GUID guidReg;
      if (FAILED(hr = ::CoCreateGuid(&guidReg)))
        return g.HandleError(hr, IDS_E_CREATEGUID);

      // Convert the GUID to a string  // See about using the new SetGUIDValue here --Imago
      OLECHAR szGUIDReg[64];
      StringFromGUID2(guidReg, szGUIDReg, sizeofArray(szGUIDReg));

      // Save the GUID string
      USES_CONVERSION;
      m_strRegGUID = OLE2CT(szGUIDReg);

      // Save the GUID string to the registry
      lr = key.SetStringValue(TEXT("GUID"), m_strRegGUID);
      if (ERROR_SUCCESS != lr)
        m_strRegGUID.SetEmpty();
    }
  }

  // Create a GUID to uniquely identify this session
  GUID guid;
  if (FAILED(hr = ::CoCreateGuid(&guid)))
    return g.HandleError(hr, IDS_E_CREATEGUID);

  // Convert the GUID to a string
  OLECHAR szGUID[64];
  StringFromGUID2(guid, szGUID, sizeofArray(szGUID));

  // Save the GUID string
  USES_CONVERSION;
  m_strAppGUID = OLE2CT(szGUID);

  // Loop on Debug events
  bool bExit;
  do
  {
    // Wait for the next debug event
    DEBUG_EVENT de;
    if (!::WaitForDebugEvent(&de, INFINITE))
      return g.HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_WAITFORDEBUGEVENT);

    // Process the debug event
    bExit = false;
    DWORD dwStatus = ProcessDebugEvent(&de, &bExit);

    // Continue the debug event
    if (!::ContinueDebugEvent(de.dwProcessId, de.dwThreadId, dwStatus))
      return g.HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_CONTINUEDEBUGEVENT);

  } while (!bExit);


  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Debug Event Handlers


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleException(bool* pbExit)
{
  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnException(pbExit);
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleCreateThread(bool* pbExit)
{
  // Save the thread information in the map
  m_Threads[m_pde->dwThreadId] = m_pde->u.CreateThread;

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  DWORD dw = pThis->OnCreateThread(pbExit);

  // Return the result of the most-derived class
  return dw;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleCreateProcess(bool* pbExit)
{
  // Save the process information
  m_dwProcessId = m_pde->dwProcessId;
  CopyMemory(&m_cpdi, &m_pde->u.CreateProcessInfo, sizeof(m_cpdi));

  // Save the thread information
  CREATE_THREAD_DEBUG_INFO ctdi =
  {
    m_cpdi.hThread,
    m_cpdi.lpThreadLocalBase,
    m_cpdi.lpStartAddress
  };
  m_Threads[m_pde->dwThreadId] = ctdi;

  // Save the process handle or id to be used for symbol table lookups
  m_hProcessSym = IsWin9x() ?
    reinterpret_cast<HANDLE>(m_dwProcessId) : m_cpdi.hProcess;

  // Resolve the image name, if available
  ZString strImageName;
  if (m_cpdi.lpImageName)
  {
    void* pvImageName = NULL;
    if (::ReadProcessMemory(m_cpdi.hProcess, m_cpdi.lpImageName,
      &pvImageName, sizeof(pvImageName), NULL) && pvImageName)
        strImageName = ReadProcessString(pvImageName, !!m_cpdi.fUnicode);
  }

  debugf("%08X - %s\n", m_cpdi.lpBaseOfImage, (LPCTSTR)strImageName);

  // Map the loaded address to the module name
  m_Images.Add(m_cpdi.lpBaseOfImage, strImageName);

  // Format an event name using the debuggee process ID
  TCHAR szEvent[24];
  _stprintf(szEvent, TEXT("MSRGuard_%08X"), m_pde->dwProcessId);

  // Create the named event to indicate that we're debugging the process
  m_shEvent = ::CreateEvent(NULL, false, false, szEvent);
  if (m_shEvent.IsNull())
  {
    g.HandleError(HRESULT_FROM_WIN32(::GetLastError()), IDS_E_CREATEEVENT);
    *pbExit = true;
    return DBG_CONTINUE;
  }

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  DWORD dw = pThis->OnCreateProcess(pbExit);

  // Close the file handle, since we don't use it
  ::CloseHandle(m_cpdi.hFile);

  // Return the result of the most-derived class
  return dw;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleExitThread(bool* pbExit)
{
  // Remove thread information from the map
  XThreadIt it = m_Threads.find(m_pde->dwThreadId);
  if (it != m_Threads.end())
    m_Threads.erase(it);

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnExitThread(pbExit);
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleExitProcess(bool* pbExit)
{
  // Destroy the named event to indicate that we're finished debugging
  m_shEvent = NULL;

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnExitProcess(pbExit);
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleLoadDLL(bool* pbExit)
{
  LOAD_DLL_DEBUG_INFO& lddi = m_pde->u.LoadDll;

  // Resolve the image name, if available
  ZString strImageName;
  if (lddi.lpImageName)
  {
    void* pvImageName = NULL;
    if (::ReadProcessMemory(m_cpdi.hProcess, lddi.lpImageName, &pvImageName,
      sizeof(pvImageName), NULL) && pvImageName)
        strImageName = ReadProcessString(pvImageName, !!lddi.fUnicode);
  }

  debugf("%08X - %s\n", lddi.lpBaseOfDll, (LPCTSTR)strImageName);

  // Map the loaded address to the module name
  m_Images.Add(lddi.lpBaseOfDll, strImageName);

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  DWORD dw = pThis->OnLoadDLL(pbExit);

  // Close the file handle, since we don't use it
  ::CloseHandle(lddi.hFile);

  // Return the result of the most-derived class
  return dw;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleUnloadDLL(bool* pbExit)
{
  // Remove the specified image from the map
  m_Images.Remove(m_pde->u.LoadDll.lpBaseOfDll);

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnUnloadDLL(pbExit);
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleOutputDebugString(bool* pbExit)
{
  OUTPUT_DEBUG_STRING_INFO& odsi = m_pde->u.DebugString;

  // Read the text string
  ZString str(ReadProcessString(odsi.lpDebugStringData, !!odsi.fUnicode,
    odsi.nDebugStringLength));
  debugf(str);

  // Determine if this is a message to MSRGuard
  const static TCHAR szMSRGuard[] = TEXT("$$MSRGuard:");
  const static int cchMSRGuard = sizeofArray(szMSRGuard) - 1;
  if (0 == _strnicmp(str, szMSRGuard, cchMSRGuard))
  {
    int iFind = str.Find(':', cchMSRGuard);
    if (-1 != iFind)
    {
      ZString strKeyword(LPCTSTR(str) + cchMSRGuard, iFind - cchMSRGuard);
      ZString strText(LPCTSTR(str) + iFind + 1);
      if (strText.Right(1) == "\n")
        strText = strText.Left(strText.GetLength() - 1);
      if (0 == _stricmp(strKeyword, "set"))
      {
        ZString strValue;
        if (-1 != (iFind = strText.Find('=')))
        {
          strValue = strText.RightOf(iFind + 1);
          strText = strText.Left(iFind);
        }
        m_Params[strText] = strValue;
      }
      else if (0 == _stricmp(strKeyword, "remove"))
      {
        XParamIt it = m_Params.find(strText);
        if (it != m_Params.end())
          m_Params.erase(it);
      }
    }
  }

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnOutputDebugString(pbExit);
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleRIP(bool* pbExit)
{
  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnRIP(pbExit);
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::HandleOther(bool* pbExit)
{
  debugf("CAppModeDebug<%hs>::HandleOther(): dwDebugEventCode = %d (0x%08X)\n",
    TCTypeName(T), m_pde->dwDebugEventCode, m_pde->dwDebugEventCode);

  // Delegate to most-derived class
  T* pThis = static_cast<T*>(this);
  return pThis->OnOther(pbExit);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::ProcessDebugEvent(DEBUG_EVENT* pde, bool* pbExit)
{
  // Save the current debug event
  m_pde = pde;

  // Types
  typedef DWORD (CAppModeDebug<T>::*XEventHandlerProc)(bool*);
  struct XEventHandler
  {
    DWORD             m_dwDebugEventCode;
    XEventHandlerProc m_pfn;
    LPCSTR            m_pszFn;
  };
  typedef const XEventHandler* XEventHandlerIt;

  // Macros
  #define CAppModeDebug_Handler(ev, handler) {ev, Handle##handler, #handler},

  // Static Initialization
  const static XEventHandler s_EventHandlers[] =
  {
    CAppModeDebug_Handler(EXCEPTION_DEBUG_EVENT     , Exception        )
    CAppModeDebug_Handler(CREATE_THREAD_DEBUG_EVENT , CreateThread     )
    CAppModeDebug_Handler(CREATE_PROCESS_DEBUG_EVENT, CreateProcess    )
    CAppModeDebug_Handler(EXIT_THREAD_DEBUG_EVENT   , ExitThread       )
    CAppModeDebug_Handler(EXIT_PROCESS_DEBUG_EVENT  , ExitProcess      )
    CAppModeDebug_Handler(LOAD_DLL_DEBUG_EVENT      , LoadDLL          )
    CAppModeDebug_Handler(UNLOAD_DLL_DEBUG_EVENT    , UnloadDLL        )
    CAppModeDebug_Handler(OUTPUT_DEBUG_STRING_EVENT , OutputDebugString)
    CAppModeDebug_Handler(RIP_EVENT                 , RIP              )
  };
  const static XEventHandlerIt itEnd =
    s_EventHandlers + sizeofArray(s_EventHandlers);

  // Find the handler for the specified debug event
  for (XEventHandlerIt it = s_EventHandlers; it != itEnd; ++it)
  {
    if (it->m_dwDebugEventCode == pde->dwDebugEventCode)
    {
      debugf("CAppModeDebug<%hs>::Handle%hs()\n", TCTypeName(T), it->m_pszFn);
      return (this->*it->m_pfn)(pbExit);
    }
  }
  return HandleOther(pbExit);
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::ReportException()
{
  // Initialize the symbol engine options
  DWORD dwOptions = SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);

  // Initialize the symbol engine
  ZString strSymbolPath(GetSymbolPath());
  int cch = strSymbolPath.GetLength() + 1;
  LPTSTR pszSymbolPath = (LPTSTR)_alloca(cch * sizeof(TCHAR));
  lstrcpy(pszSymbolPath, strSymbolPath);
  if (!::SymInitialize(m_hProcessSym, pszSymbolPath, true))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Enumerate the loaded modules
  if (m_Images.Enumerate(m_cpdi.hProcess, m_hProcessSym))
  {
    m_Images.Dump();
  }
  else
  {
    DWORD dwLastError = ::GetLastError();
    debugf("CLoadedModules::Enumerate failed, LastError = 0x%08X\n", dwLastError);
  }

  // Compose the exception report parameters
  HRESULT hr = ComposeExceptionReportParams();

  // Terminate the symbol engine
  ::SymCleanup(m_hProcessSym);

  // Terminate the debugee process
  ::TerminateProcess(m_cpdi.hProcess, UINT(-1));

  // Return if last call failed
  RETURN_FAILED(hr);

  // Create the HiddenFields string
  ZString strHiddenFields;
  for (XParamIt it = m_Params.begin(); it != m_Params.end(); ++it)
  {
    ZString str;
    RETURN_FAILED(g.FormatString(str, IDS_FMT_HIDDEN_FIELD,
      (LPCTSTR)it->first, (LPCTSTR)it->second));
    strHiddenFields += str;
  }

  // Get the HTML text
  ZString strHTML;
  ZSucceeded(g.LoadHTML(strHTML));

  // Replace %Title% in the HTML with the title
  cch = 1 + ::TCReplaceTextNoCase(strHTML, TEXT("%Title%"), g.GetTitle(), NULL, 0);
  LPTSTR psz = new TCHAR[cch];
  ::TCReplaceTextNoCase(strHTML, TEXT("%Title%"), g.GetTitle(), psz, cch);

  // Replace %URLBase% in the HTML with the URLBase
  cch = 1 + ::TCReplaceTextNoCase(psz, TEXT("%URLBase%"),
    g.GetConfigString("URLBase"), NULL, 0);
  LPTSTR psz2 = new TCHAR[cch];
  ::TCReplaceTextNoCase(psz, TEXT("%URLBase%"), g.GetConfigString("URLBase"), psz2, cch);
  delete [] psz;

  // Replace %HiddenFields% with the hidden fields
  cch = 1 + ::TCReplaceTextNoCase(psz2, TEXT("%HiddenFields%"), strHiddenFields, NULL, 0);
  LPTSTR psz3 = new TCHAR[cch];
  ::TCReplaceTextNoCase(psz2, TEXT("%HiddenFields%"), strHiddenFields, psz3, cch);
  delete [] psz2;

  // Save the converted HTML back to the ZString
  strHTML = psz3;
  delete [] psz3;

  // Create a temporary filename
  TCHAR szTempPath[_MAX_PATH * 2];
  if (!::GetTempPath(sizeofArray(szTempPath), szTempPath))
    return HRESULT_FROM_WIN32(::GetLastError());
  lstrcat(szTempPath, TEXT("MSRGReq.htm"));

  // Open the temporary file for writing
  DWORD dwFlags = FILE_ATTRIBUTE_TEMPORARY;
  TCHandle shFile = ::CreateFile(szTempPath, GENERIC_WRITE, FILE_SHARE_READ,
    NULL, CREATE_ALWAYS, dwFlags, NULL);
  if (INVALID_HANDLE_VALUE == shFile.GetHandle())
    return HRESULT_FROM_WIN32(::GetLastError());

  // Write the converted HTML to the file
  DWORD dwWritten;
  if (!::WriteFile(shFile, strHTML, strHTML.GetLength() * sizeof(TCHAR),
    &dwWritten, NULL))
      return HRESULT_FROM_WIN32(::GetLastError());

  // Close the file
  shFile = NULL;

  // Send the report to the web site, if specified
  if (!g.GetConfigBool("LogExceptionToURL"))
    return S_OK;

  // Format the browser URL command line string
  dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY;
  LPTSTR pszTempURL = szTempPath;
  LPTSTR pszCommand = NULL;
  ::FormatMessage(dwFlags, (PCC)m_strBrowser, 0, 0,
    reinterpret_cast<LPTSTR>(&pszCommand), 1, &pszTempURL);

  // Create the Web browser process
  hr = S_OK;
  PROCESS_INFORMATION pi;
  STARTUPINFO         si = {sizeof(si)};
  if (!::CreateProcess(NULL, pszCommand, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
    hr = HRESULT_FROM_WIN32(::GetLastError());

  // Free the formatted URL command line string
  ::LocalFree(pszCommand);

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::ComposeExceptionReportParams()
{
  // General parameters
  m_Params["Title"  ] = g.GetConfigString("Title");
  m_Params["AppName"] = g.GetConfigString("ApplicationName");
  m_Params["RegGUID"] = m_strRegGUID;
  m_Params["AppGUID"] = m_strAppGUID;
  m_Params["Action" ] = "Exception";

  // Get the full path and version of the this guard application
  TCHAR szGuardPath[_MAX_PATH * 2];
  ::GetModuleFileName(NULL, szGuardPath, sizeofArray(szGuardPath));
  m_Params["GuardPath"] = szGuardPath;

  // Get the VersionInfo of this guard application
  ZVersionInfo vi(szGuardPath);
  m_Params["GuardVer"] = vi.GetFileVersionString();

  // Get the full path and version of the debuggee application
  CLoadedModuleIt itApp = m_Images.find(m_cpdi.lpBaseOfImage);
  if (itApp != m_Images.end())
  {
    ZString strAppPath(itApp->second.m_strModuleName);
    m_Params["AppPath"] = strAppPath;

    // Fix the AppName, if needed
    if (m_Params["AppName"].IsEmpty())
    {
      TCHAR szAppName[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
      _tsplitpath(strAppPath, NULL, NULL, szFName, szExt);
      _tmakepath(szAppName, NULL, NULL, szFName, szExt);
      m_Params["AppName"] = szAppName;
    }

    // Get the VersionInfo of the debuggee application
    ZVersionInfo vi(strAppPath);
    m_Params["AppVer"] = vi.GetFileVersionString();
  }

  // Format the exception code and address
  TCHAR szBuf[16];
  _stprintf(szBuf, TEXT("%08X"), m_pde->u.Exception.ExceptionRecord.ExceptionCode);
  m_Params["XCode"] = szBuf;
  _stprintf(szBuf, TEXT("%08X"), m_pde->u.Exception.ExceptionRecord.ExceptionAddress);
  m_Params["XAddr"] = szBuf;

  // Find the module of the exception address
  m_Params["XModule"] = GetAddressModule(m_pde->u.Exception.ExceptionRecord.ExceptionAddress);

  // Get the exception name
  m_Params["XName"] = GetExceptionName(m_pde->u.Exception.ExceptionRecord.ExceptionCode);

  // Get the entire exception state
  ZString strExceptionState;
  RETURN_FAILED(GetExceptionState(strExceptionState));
  m_Params["XState"] = strExceptionState;

  // Get the machine.txt
  ZString strMachineText;
  HRESULT hr = GetMachineText(strMachineText);
  if (S_OK != hr)
  {
    strMachineText += "\r\n\r\n";
    strMachineText += g.GetErrorString(hr);
  }
  m_Params["Machine"] = strMachineText;

  // Canonicalize each value
  for (XParamIt it = m_Params.begin(); it != m_Params.end(); ++it)
  {
    ZString str;
    RETURN_FAILED(CanonicalizeParamText(it->second, str));
    it->second = str;
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::CanonicalizeParamText(ZString strIn, ZString& strOut)
{
  // Create an output stream
  IStreamPtr spStmOut;
  HGLOBAL hGlobal = ::GlobalAlloc(GHND, 0);
  RETURN_FAILED(::CreateStreamOnHGlobal(hGlobal, true, &spStmOut));

  // Loop through the input string, copying/transforming text to the output
  const LPCTSTR pszTokens = TEXT("\"&<>");
  const LPCTSTR pszEnd = (PCC)strIn + strIn.GetLength();
  for (LPCTSTR psz = strIn; psz != pszEnd; ++psz)
  {
    if (_istprint(*psz) && !_tcschr(pszTokens, *psz))
    {
      RETURN_FAILED(spStmOut->Write(psz, sizeof(TCHAR), NULL));
    }
    else
    {
      TCHAR szBuf[16];
      int cch = _stprintf(szBuf, TEXT("&#%02u;"), *psz);
      RETURN_FAILED(spStmOut->Write(szBuf, cch * sizeof(TCHAR), NULL));
    }
  }

  // Null-terminate the data in the stream
  TCHAR szNull = TEXT('\0');
  RETURN_FAILED(spStmOut->Write(&szNull, sizeof(szNull), NULL));

  // Get the HGLOBAL from the stream
  RETURN_FAILED(GetHGlobalFromStream(spStmOut, &hGlobal));

  // Lock the HGLOBAL into memory
  TCGlobalPtr spvStream = ::GlobalLock(hGlobal);

  // Create a ZString from the stream
  strOut = reinterpret_cast<LPCTSTR>(spvStream.GetHandle());

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::GetExceptionState(ZString& strOut)
{
  ZString str;

  // Create a memory stream
  IStreamPtr spStm;
  RETURN_FAILED(::CreateStreamOnHGlobal(NULL, true, &spStm));

  // Format the <State> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_STATE_BEGIN));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the <Threads> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_THREADS_BEGIN));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the full exception state for the exception thread first
  RETURN_FAILED(FormatThreadState(spStm, m_pde->dwThreadId, true));

  // Format exception state for the rest of the threads
  bool bFull = g.GetConfigBool("LogFullStateOfAllThreads");
  for (XThreadIt itTh = m_Threads.begin(); itTh != m_Threads.end(); ++itTh)
    if (itTh->first != m_pde->dwThreadId)
      RETURN_FAILED(FormatThreadState(spStm, itTh->first, bFull));

  // Format the </Threads> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_THREADS_END));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the <Modules> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_MODULES_BEGIN));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the module info for each loaded module
  for (CLoadedModuleIt it = m_Images.begin(); it != m_Images.end(); ++it)
    RETURN_FAILED(FormatModuleInfo(spStm, it));

  // Format the </Modules> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_MODULES_END));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the <Processes> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_PROCESSES_BEGIN));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the process infor for each process in the system
  RETURN_FAILED(FormatProcessesInfo(spStm));

  // Format the </Processes> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_PROCESSES_END));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Format the </State> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_STATE_END));
  WRITE_STRING_TO_STREAM(spStm, str);

  // Get the size of the IStream
  LARGE_INTEGER li = {0};
  ULARGE_INTEGER uli;
  RETURN_FAILED(spStm->Seek(li, STREAM_SEEK_END, &uli));
  assert(0 == uli.HighPart);

  // Get the HGLOBAL from the stream
  HGLOBAL hGlobal = NULL;
  RETURN_FAILED(::GetHGlobalFromStream(spStm, &hGlobal));

  // Lock the HGLOBAL
  LPCTSTR psz = reinterpret_cast<LPCTSTR>(::GlobalLock(hGlobal));
  assert(psz);

  // Create a string from the stream
  ZString strFromStream(psz, (int)uli.LowPart);

  // Unlock the global
  ::GlobalUnlock(hGlobal);

  // Copy the string to the [out] parameters
  strOut = strFromStream;

  // Write the XML to a file, if specified (usually for debugging)
  ZString strXMLFile(g.GetConfigString("LogXStateToFile"));
  if (!strXMLFile.IsEmpty() || "0" == strXMLFile)
  {
    TCHandle shFile = ::CreateFile(strXMLFile, GENERIC_WRITE,
      FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
    if (!shFile.IsNull() && INVALID_HANDLE_VALUE != shFile.GetHandle())
    {
      DWORD dwWritten;
      ::WriteFile(shFile, (LPCTSTR)strOut, strOut.GetLength(), &dwWritten, NULL);
    }
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::GetMachineText(ZString& strOut)
{
  // Load the MINI resource
  ZString strMINI;
  RETURN_FAILED(g.LoadMINI(strMINI));

  // Get the temporary pathname
  TCHAR szTempPath[_MAX_PATH * 2];
  if (!::GetTempPath(sizeofArray(szTempPath), szTempPath))
  {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    strOut = "GetTempPath";
    return hr;
  }

  // Create temporary filenames for Machine.exe, Machine.ini, and Machine.txt
  TCHAR szMachineExe[_MAX_PATH * 2];
  lstrcpy(szMachineExe, szTempPath);
  lstrcat(szMachineExe, TEXT("MSRGMach.exe"));
  TCHAR szMachineIni[_MAX_PATH * 2];
  lstrcpy(szMachineIni, szTempPath);
  lstrcat(szMachineIni, TEXT("Machine.ini"));
  TCHAR szMachineTxt[_MAX_PATH * 2];
  lstrcpy(szMachineTxt, szTempPath);
  lstrcat(szMachineTxt, TEXT("Machine.txt"));

  // Get the current module pathname
  TCHAR szModulePath[_MAX_PATH];
  ::GetModuleFileName(NULL, szModulePath, sizeofArray(szModulePath));

  // Attempt to find machine.exe in the same path as the current module
  TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
  _tsplitpath(szModulePath, szDrive, szDir, NULL, NULL);
  _tmakepath(szModulePath, szDrive, szDir, TEXT("machine"), TEXT(".exe"));

  // Load the 'Machine not available' prefix string, in case we need it
  ZString strMachineNA;
  RETURN_FAILED(g.LoadString(strMachineNA, IDS_E_MACHINE_NA));
  LPCTSTR pszMachineNA = strMachineNA;

  // Attempt to copy the machine.exe file to a temporary file
  if (!::CopyFile(szModulePath, szMachineExe, false))
  {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    g.FormatString(strOut, "%sCopyFile(%s, %s)", pszMachineNA, szModulePath,
      szMachineExe);
    return hr;
  }

  // Create the INI file for writing
  TCHandle shFile = ::CreateFile(szMachineIni, GENERIC_WRITE, 0, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
  if (INVALID_HANDLE_VALUE == shFile.GetHandle())
  {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    ::DeleteFile(szMachineExe);
    g.FormatString(strOut, "%sCreateFile(%s)", pszMachineNA, szMachineIni);
    return hr;
  }

  // Write the contents of the INI file
  DWORD dwWritten;
  if (!::WriteFile(shFile, (const void*)(LPCTSTR)strMINI,
    strMINI.GetLength() * sizeof(TCHAR), &dwWritten, NULL))
  {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    shFile = NULL;
    ::DeleteFile(szMachineExe);
    ::DeleteFile(szMachineIni);
    g.FormatString(strOut, "%sWriteFile(%s)", pszMachineNA, szMachineIni);
    return hr;
  }

  // Close the file
  shFile = NULL;

  // Delete the current machine.txt, if any
  ::DeleteFile(szMachineTxt);

  // Compose the machine.exe command line
  TCHAR szCommand[_MAX_PATH * 2];
  lstrcpy(szCommand, szMachineExe);
  lstrcat(szCommand, " -l");

  // Execute the machine.exe command line (hidden, if possible)
  PROCESS_INFORMATION pi;
  STARTUPINFO         si = {sizeof(si)};
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;
  if (!::CreateProcess(NULL, szCommand, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
  {
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    ::DeleteFile(szMachineExe);
    ::DeleteFile(szMachineIni);
    g.FormatString(strOut, "%sCreateProcess(%s)", pszMachineNA, szCommand);
    return hr;
  }

  // Wait for the machine.exe process to exit
  const DWORD c_nTimeOutSeconds = 30;
  const DWORD c_dwTimeOut = c_nTimeOutSeconds * 1000;
  if (WAIT_TIMEOUT == ::WaitForSingleObject(pi.hProcess, c_dwTimeOut))
  {
    ::TerminateProcess(pi.hProcess, 1);
    ::DeleteFile(szMachineExe);
    ::DeleteFile(szMachineIni);
    g.FormatString(strOut,
      "%sMachine.exe failed to complete within %d seconds",
      pszMachineNA, c_nTimeOutSeconds);
    return S_FALSE;
  }

  // Delete the temporary files
  ::DeleteFile(szMachineExe);
  ::DeleteFile(szMachineIni);

  // Open the machine.txt file
  {
    ZFile file(szMachineTxt);
    ZString strText((LPCTSTR)file.GetPointer(), file.GetLength());
    strOut = strText;
  }

  // Delete the machine.txt file
  ::DeleteFile(szMachineTxt);

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
ZString CAppModeDebug<T>::ReadProcessString(void* pvAddr, bool fUnicode, int cch)
{
  if (fUnicode)
  {
    const int cbChar = sizeof(WCHAR);
    if (-1 == cch)
    {
      cch = 0;
      WCHAR cChar;
      WCHAR* p = reinterpret_cast<WCHAR*>(pvAddr);
      while (true)
      {
        ::ReadProcessMemory(m_cpdi.hProcess, p, &cChar, cbChar, NULL);
        if (L'\0' == cChar)
          break;
        ++cch;
        ++p;
      }
    }

    WCHAR* psz = new WCHAR[cch + 1];
    ::ReadProcessMemory(m_cpdi.hProcess, pvAddr, psz, cch * cbChar, NULL);
    psz[cch] = L'\0';

    USES_CONVERSION;
    ZString str(W2CT(psz), cch);
    delete [] psz;
    return str;
  }
  else
  {
    const int cbChar = sizeof(CHAR);
    if (-1 == cch)
    {
      cch = 0;
      CHAR cChar;
      CHAR* p = reinterpret_cast<CHAR*>(pvAddr);
      while (true)
      {
        ::ReadProcessMemory(m_cpdi.hProcess, p, &cChar, cbChar, NULL);
        if ('\0' == cChar)
          break;
        ++cch;
        ++p;
      }
    }

    CHAR* psz = new CHAR[cch + 1];
    ::ReadProcessMemory(m_cpdi.hProcess, pvAddr, psz, cch * cbChar, NULL);
    psz[cch] = '\0';

    USES_CONVERSION;
    ZString str(A2CT(psz), cch);
    delete [] psz;
    return str;
  }
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
LPCTSTR CAppModeDebug<T>::GetExceptionName(DWORD dwExceptionCode)
{
  #define CAppModeDebug_ExceptionName(x) case EXCEPTION_##x: return TEXT(#x);

  switch (dwExceptionCode)
  {
    CAppModeDebug_ExceptionName(ACCESS_VIOLATION)
    CAppModeDebug_ExceptionName(ARRAY_BOUNDS_EXCEEDED)
    CAppModeDebug_ExceptionName(BREAKPOINT)
    CAppModeDebug_ExceptionName(DATATYPE_MISALIGNMENT)
    CAppModeDebug_ExceptionName(FLT_DENORMAL_OPERAND)
    CAppModeDebug_ExceptionName(FLT_DIVIDE_BY_ZERO)
    CAppModeDebug_ExceptionName(FLT_INEXACT_RESULT)
    CAppModeDebug_ExceptionName(FLT_INVALID_OPERATION)
    CAppModeDebug_ExceptionName(FLT_OVERFLOW)
    CAppModeDebug_ExceptionName(FLT_STACK_CHECK)
    CAppModeDebug_ExceptionName(FLT_UNDERFLOW)
    CAppModeDebug_ExceptionName(ILLEGAL_INSTRUCTION)
    CAppModeDebug_ExceptionName(IN_PAGE_ERROR)
    CAppModeDebug_ExceptionName(INT_DIVIDE_BY_ZERO)
    CAppModeDebug_ExceptionName(INT_OVERFLOW)
    CAppModeDebug_ExceptionName(INVALID_DISPOSITION)
    CAppModeDebug_ExceptionName(NONCONTINUABLE_EXCEPTION)
    CAppModeDebug_ExceptionName(PRIV_INSTRUCTION)
    CAppModeDebug_ExceptionName(SINGLE_STEP)
    CAppModeDebug_ExceptionName(STACK_OVERFLOW)
  }

  return TEXT("unrecognized exception");
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
ZString CAppModeDebug<T>::GetAddressModule(void* pvAddr)
{
  // Get the base load address of the module containing the specified address
  DWORD dwAddr = reinterpret_cast<DWORD>(pvAddr);
  DWORD dwBase = ::SymGetModuleBase(m_hProcessSym, dwAddr);
  pvAddr = reinterpret_cast<void*>(dwBase);

  // Find the module of the specified address
  CLoadedModuleIt itMod = m_Images.find(pvAddr);
  ZString strModule;
  if (itMod != m_Images.end())
  {
    TCHAR szModule[_MAX_PATH], szFName[_MAX_FNAME], szExt[_MAX_EXT];
    _tsplitpath(itMod->second.m_strModuleName, NULL, NULL, szFName, szExt);
    _tmakepath(szModule, NULL, NULL, szFName, szExt);
    strModule = szModule;
  }
  else
  {
    g.LoadString(strModule, IDS_MODULE_UNKNOWN);
  }
  return strModule;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::FormatThreadState(IStream* pStm, DWORD dwThreadId,
  bool bFull)
{
  ZString str;

  // Get the thread info of the specified thread
  XThreadIt it = m_Threads.find(dwThreadId);
  assert(it != m_Threads.end());

  // Attempt to find the module name for the thread start address
  ZString strModule(GetAddressModule(it->second.lpStartAddress));

  // Lookup the symbol name of the thread start address
  ZString strSymbol;
  DWORD dwDisplacement;
  BYTE symBuffer[sizeof(IMAGEHLP_SYMBOL) + 1024];
  PIMAGEHLP_SYMBOL sym = (PIMAGEHLP_SYMBOL) symBuffer;
  if (::SymGetSymFromAddr(m_hProcessSym,
    reinterpret_cast<DWORD>(it->second.lpStartAddress), &dwDisplacement, sym))
      strSymbol = sym->Name;
  else
    g.LoadString(strSymbol, IDS_NO_SYMBOLS);

  // Format the <Thread> XML tag for the specified thread
  RETURN_FAILED(g.FormatString(str, IDS_FMT_THREAD_BEGIN, dwThreadId,
    it->second.lpStartAddress, (LPCTSTR)strModule, (LPCTSTR)strSymbol));
  WRITE_STRING_TO_STREAM(pStm, str);

  // The rest is considered "Full" thread state
  if (bFull)
  {
    HANDLE hThread = it->second.hThread;

    // Get the thread context for the specified thread
    CONTEXT ctx = {CONTEXT_FULL};
    if (!::GetThreadContext(hThread, &ctx))
      return HRESULT_FROM_WIN32(::GetLastError());

    // Format the <Registers> XML tag for the specified thread
    RETURN_FAILED(g.FormatString(str, IDS_FMT_REGISTERS,
      ctx.Eax, ctx.Ebx, ctx.Ecx, ctx.Edx, ctx.Esi, ctx.Edi,
      ctx.Ebp, ctx.Esp, ctx.Eip, ctx.EFlags, ctx.SegCs, ctx.SegDs,
      ctx.SegSs, ctx.SegEs, ctx.SegFs, ctx.SegGs));
    WRITE_STRING_TO_STREAM(pStm, str);

    // Format some bytes from the stack
    const int c_cStackBytes = 320;
    TCHAR szBytes[c_cStackBytes * 2 + 1];

    // Read some bytes from the thread's stack
    BYTE   bBytes[c_cStackBytes];
    DWORD dwRead = 0;
    if (::ReadProcessMemory(m_cpdi.hProcess,
      reinterpret_cast<void*>(ctx.Esp), bBytes, sizeof(bBytes), &dwRead))
    {
      // Format the bytes from the stack -Imago declared j
      int j = 0;
      for (int i = 0, j = 0; i < c_cStackBytes; ++i, j += 2)
        _stprintf(szBytes + j, "%02X", bBytes[i]);
      szBytes[j] = TEXT('\0');
    }
    else
    {
      szBytes[0] = TEXT('\0');
    }

    // Format the <Stack> XML tag
    RETURN_FAILED(g.FormatString(str, IDS_FMT_STACK_BEGIN, szBytes));
    WRITE_STRING_TO_STREAM(pStm, str);

    // Initialize the STACKFRAME structure
    STACKFRAME stk;
    ZeroMemory(&stk, sizeof(stk));
    stk.AddrPC.Offset    = ctx.Eip;
    stk.AddrPC.Mode      = AddrModeFlat;
    stk.AddrStack.Offset = ctx.Esp;
    stk.AddrStack.Mode   = AddrModeFlat;
    stk.AddrFrame.Offset = ctx.Ebp;
    stk.AddrFrame.Mode   = AddrModeFlat;

    // Walk the stack
    DWORD dwPrevFrame = stk.AddrFrame.Offset;
    const int c_nMaxFrames = 100;
    for (int f = 0; f < c_nMaxFrames; ++f)
    {
      const DWORD c_dwMachineType = IMAGE_FILE_MACHINE_I386;
      bool bFrame = !!::StackWalk(c_dwMachineType,
        reinterpret_cast<HANDLE>(this), hThread, &stk, &ctx,
        ReadProcessMemoryProc, FunctionTableAccessProc, GetModuleBaseProc,
        TranslateAddressProc);
      if (!bFrame)
        break;

      // Format the stack frame
      RETURN_FAILED(FormatStackFrame(pStm, stk));

      // Avoid endlessly repeating frames
      if (f && dwPrevFrame == stk.AddrFrame.Offset)
        break;
      dwPrevFrame = stk.AddrFrame.Offset;
    }

    // Format the </Stack> XML tag
    RETURN_FAILED(g.LoadString(str, IDS_STACK_END));
    WRITE_STRING_TO_STREAM(pStm, str);
  }

  // Format the </Thread> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_THREAD_END))
  WRITE_STRING_TO_STREAM(pStm, str);

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::FormatStackFrame(IStream* pStm, STACKFRAME& stk)
{
  BYTE symBuffer[sizeof(IMAGEHLP_SYMBOL) + 1024];
  PIMAGEHLP_SYMBOL sym = (PIMAGEHLP_SYMBOL) symBuffer;

  // Attempt to lookup the symbol name for the frame
  ZString strSymbol;
  DWORD dwDisplacement;
  if (::SymGetSymFromAddr(m_hProcessSym, stk.AddrPC.Offset, &dwDisplacement, sym))
    strSymbol = sym->Name;
  else
    g.LoadString(strSymbol, IDS_NO_SYMBOLS);

  // Attempt to find the module name for the frame
  ZString strModule(GetAddressModule(reinterpret_cast<void*>(stk.AddrPC.Offset)));

  // Format the <Frm> XML tag
  ZString str;
  RETURN_FAILED(g.FormatString(str, IDS_FMT_FRAME_BEGIN, stk.AddrPC.Offset,
    stk.AddrFrame.Offset, stk.AddrReturn.Offset,
    stk.Params[0], stk.Params[1], stk.Params[2], stk.Params[3],
    (LPCTSTR)strModule, (LPCTSTR)strSymbol));
  WRITE_STRING_TO_STREAM(pStm, str);

  // Format the <FPO> XML tag, if a function pointer table entry exists
  if (stk.FuncTableEntry)
  {
    // Determine the frame type string
    FPO_DATA& fpo = *reinterpret_cast<FPO_DATA*>(stk.FuncTableEntry);
    LPCTSTR pszFrame;
    switch (fpo.cbFrame)
    {
      case FRAME_FPO   : pszFrame = "FPO"      ; break;
      case FRAME_TRAP  : pszFrame = "TRAP"     ; break;
      case FRAME_TSS   : pszFrame = "TSS"      ; break;
      case FRAME_NONFPO: pszFrame = "NONFPO"   ; break;
      default          : pszFrame = "(unknown)"; break;
    }

    // Format the <FPO> XML tag
    RETURN_FAILED(g.FormatString(str, IDS_FMT_FPO, fpo.ulOffStart,
      fpo.cbProcSize, fpo.cdwLocals, fpo.cdwParams, fpo.cbProlog, fpo.cbRegs,
      fpo.fHasSEH ? "1" : "0", fpo.fUseBP ? "1" : "0", pszFrame));
    WRITE_STRING_TO_STREAM(pStm, str);
  }

  // Format the </Frm> XML tag
  RETURN_FAILED(g.LoadString(str, IDS_FRAME_END));
  WRITE_STRING_TO_STREAM(pStm, str);

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::FormatModuleInfo(IStream* pStm, CLoadedModuleIt it)
{
  ZString strFileSize, strFileDate, strFileTime, strFileVer, strProdVer;
  ZString strIsDebug, strCompany, strDesc, strProdName, strCopy;

  // Ensure that we have a fully-qualified module name
  ZString strModule(GetModuleWithPath(it->second.m_strModuleName));

  // FileVer and ProdVer
  ZVersionInfo vi;
  if (vi.Load(strModule))
  {
    RETURN_FAILED(g.FormatString(strFileVer, IDS_FMT_FILE_VER,
      vi.GetFileVersionMSHigh(), vi.GetFileVersionMSLow(),
      vi.GetFileVersionLSHigh(), vi.GetFileVersionLSLow()));
    RETURN_FAILED(g.FormatString(strProdVer, IDS_FMT_PROD_VER,
      vi.GetProductVersionMSHigh(), vi.GetProductVersionMSLow(),
      vi.GetProductVersionLSHigh(), vi.GetProductVersionLSLow()));

    // IsDebug, Company, Desc, ProdName, Copy
    strIsDebug  = vi.IsDebug() ? "1" : "0";
    strCompany  = vi.GetCompanyName();
    strDesc     = vi.GetFileDescription();
    strProdName = vi.GetProductName();
    strCopy     = vi.GetLegalCopyright();
  }

  // Find the specified module file
  WIN32_FIND_DATA fd;
  TCFileFindHandle shFF = ::FindFirstFile(strModule, &fd);
  if (!shFF.IsNull() && INVALID_HANDLE_VALUE != shFF.GetHandle())
  {
    // FileSize
    RETURN_FAILED(g.FormatString(strFileSize, IDS_FMT_FILE_SIZE,
      fd.nFileSizeLow));

    // FileDate and FileTime
    FILETIME   ft;
    if (!::FileTimeToLocalFileTime(&fd.ftLastWriteTime, &ft))
      return HRESULT_FROM_WIN32(::GetLastError());
    SYSTEMTIME st;
    if (!::FileTimeToSystemTime(&ft, &st))
      return HRESULT_FROM_WIN32(::GetLastError());
    RETURN_FAILED(g.FormatString(strFileDate, IDS_FMT_FILE_DATE,
      st.wYear, st.wMonth, st.wDay));
    RETURN_FAILED(g.FormatString(strFileTime, IDS_FMT_FILE_TIME,
      st.wHour, st.wMinute, st.wSecond));
  }

  // Format the <Module> XML tag
  ZString str;
  RETURN_FAILED(g.FormatString(str, IDS_FMT_MODULE,
    (LPCTSTR)strModule, it->second.m_ModuleBase,
    it->second.m_ModuleSize, (LPCTSTR)strFileSize, (LPCTSTR)strFileDate,
    (LPCTSTR)strFileTime, (LPCTSTR)strFileVer, (LPCTSTR)strProdVer,
    (LPCTSTR)strIsDebug, (LPCTSTR)strCompany, (LPCTSTR)strDesc,
    (LPCTSTR)strProdName, (LPCTSTR)strCopy));
  WRITE_STRING_TO_STREAM(pStm, str);

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::FormatProcessesInfo(IStream* pStm)
{
  // TODO: Hook the NT4 case and use EnumProcesses

  // This will fail under Windows NT 4.0 or earlier
  if (!m_libToolHelp.IsLibraryLoadedAndResolved())
    return S_FALSE;

  // Get a snapshot of the processes currently running on the system
  TCHandle shSnapshot = m_libToolHelp.CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (shSnapshot.IsNull() || INVALID_HANDLE_VALUE == shSnapshot)
    return S_FALSE;

  // Iterate over each process (except for the debuggee process)
  PROCESSENTRY32 pe = {sizeof(pe)};
  bool bProcess = !!m_libToolHelp.Process32First(shSnapshot, &pe);
  while (bProcess)
  {
    // Format the process information
    if (pe.th32ProcessID != m_dwProcessId && pe.th32ProcessID != ::GetCurrentProcessId())
      RETURN_FAILED(FormatProcessInfo(pStm, &pe));

    // Get the next process
    ZeroMemory(pe.szExeFile, sizeof(pe.szExeFile));
    pe.dwSize = sizeof(pe);
    bProcess = !!m_libToolHelp.Process32Next(shSnapshot, &pe);
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::FormatProcessInfo(IStream* pStm, PROCESSENTRY32* ppe)
{
  // Paths are usually not fully-qualified under Windows 2000
  TCHAR szPath[_MAX_PATH * 2] = TEXT("");
  TCHAR szEXE[_MAX_PATH * 2];
  LPCTSTR pszEXE = ppe->szExeFile;
  static bool s_bIsWinNT = ::IsWinNT();
  if (s_bIsWinNT)
  {
    // Determine if the specified path is fully-qualified
    TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
    _tsplitpath(ppe->szExeFile, szDrive, szDir, NULL, NULL);
    if (TEXT('\0') == szDrive[0] && TEXT('\0') == szDir[0])
    {
      // Handle a special case
      if (0 == _tcsicmp(TEXT("csrss.exe"), pszEXE))
      {
        ::GetSystemDirectory(szPath, sizeofArray(szPath));
        if (TEXT('\\') != szPath[_tcslen(szPath) - 1])
          _tcscat(szPath, TEXT("\\"));
        _tcscat(szPath, pszEXE);
        _tcscpy(szEXE, szPath);
        pszEXE = szEXE;
      }
      else
      {
        // Attempt to open the process
        const DWORD dwAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
        TCHandle shProcess = ::OpenProcess(dwAccess, false, ppe->th32ProcessID);
        if (!shProcess.IsNull())
        {
          // Get the name of the first module in the process
          DWORD cb = 0;
          HMODULE hModule = NULL;
          if (::EnumProcessModules(shProcess, &hModule, sizeof(hModule), &cb))
          {
            if (::GetModuleFileNameEx(shProcess, hModule, szEXE, sizeofArray(szEXE)))
            {
              // Handle some special cases
              if (0 == _tcsnicmp(TEXT("\\SystemRoot\\"), szEXE, 12))
              {
                ::GetWindowsDirectory(szPath, sizeofArray(szPath));
                if (TEXT('\\') != szPath[_tcslen(szPath) - 1])
                  _tcscat(szPath, TEXT("\\"));
                _tcscat(szPath, szEXE + 12);
                _tcscpy(szEXE, szPath);
                pszEXE = szEXE;
              }
              else if (0 == _tcsncmp(TEXT("\\??\\"), szEXE, 4))
              {
                pszEXE = szEXE + 4;
              }
              else
              {
                pszEXE = szEXE;
              }
            }
          }
        }
      }
    }
  }

  // Delegate to the string-based formatting method
  return FormatProcessInfo(pStm, pszEXE);
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
HRESULT CAppModeDebug<T>::FormatProcessInfo(IStream* pStm, LPCTSTR pszEXE)
{
  ZString strFileSize, strFileDate, strFileTime, strFileVer, strProdVer;
  ZString strIsDebug, strCompany, strDesc, strProdName, strCopy;

  // Resolve as a long filename, if possible
  TCHAR szEXE[_MAX_PATH * 2];
  if (m_libToolHelp.Exists_GetLongPathName())
    if (m_libToolHelp.GetLongPathName(pszEXE, szEXE, sizeofArray(szEXE)))
      pszEXE = szEXE;

  // FileVer and ProdVer
  ZVersionInfo vi;
  if (vi.Load(pszEXE))
  {
    RETURN_FAILED(g.FormatString(strFileVer, IDS_FMT_FILE_VER,
      vi.GetFileVersionMSHigh(), vi.GetFileVersionMSLow(),
      vi.GetFileVersionLSHigh(), vi.GetFileVersionLSLow()));
    RETURN_FAILED(g.FormatString(strProdVer, IDS_FMT_PROD_VER,
      vi.GetProductVersionMSHigh(), vi.GetProductVersionMSLow(),
      vi.GetProductVersionLSHigh(), vi.GetProductVersionLSLow()));

    // IsDebug, Company, Desc, ProdName, Copy
    strIsDebug  = vi.IsDebug() ? "1" : "0";
    strCompany  = vi.GetCompanyName();
    strDesc     = vi.GetFileDescription();
    strProdName = vi.GetProductName();
    strCopy     = vi.GetLegalCopyright();
  }

  // Find the specified module file
  WIN32_FIND_DATA fd;
  TCFileFindHandle shFF = ::FindFirstFile(pszEXE, &fd);
  if (!shFF.IsNull() && INVALID_HANDLE_VALUE != shFF.GetHandle())
  {
    // FileSize
    RETURN_FAILED(g.FormatString(strFileSize, IDS_FMT_FILE_SIZE,
      fd.nFileSizeLow));

    // FileDate and FileTime
    FILETIME   ft;
    if (!::FileTimeToLocalFileTime(&fd.ftLastWriteTime, &ft))
      return HRESULT_FROM_WIN32(::GetLastError());
    SYSTEMTIME st;
    if (!::FileTimeToSystemTime(&ft, &st))
      return HRESULT_FROM_WIN32(::GetLastError());
    RETURN_FAILED(g.FormatString(strFileDate, IDS_FMT_FILE_DATE,
      st.wYear, st.wMonth, st.wDay));
    RETURN_FAILED(g.FormatString(strFileTime, IDS_FMT_FILE_TIME,
      st.wHour, st.wMinute, st.wSecond));
  }

  // Format the <Process> XML tag
  ZString str;
  RETURN_FAILED(g.FormatString(str, IDS_FMT_PROCESS,
    pszEXE, (LPCTSTR)strFileSize, (LPCTSTR)strFileDate,
    (LPCTSTR)strFileTime, (LPCTSTR)strFileVer, (LPCTSTR)strProdVer,
    (LPCTSTR)strIsDebug, (LPCTSTR)strCompany, (LPCTSTR)strDesc,
    (LPCTSTR)strProdName, (LPCTSTR)strCopy));
  WRITE_STRING_TO_STREAM(pStm, str);

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
ZString CAppModeDebug<T>::GetSymbolPath()
{
  TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
  TCHAR szPath[_MAX_PATH * 2];
  ZString str;

  // Get the directory of the debuggee application
  CLoadedModuleIt itApp = m_Images.find(m_cpdi.lpBaseOfImage);
  if (itApp != m_Images.end())
  {
    _tsplitpath(itApp->second.m_strModuleName, szDrive, szDir, NULL, NULL);
    _tmakepath(szPath, szDrive, szDir, NULL, NULL);
    str += szPath;
    str += ";";
  }

  // Get the current directory
  ::GetCurrentDirectory(sizeofArray(szPath), szPath);
  str += szPath;
  str += ";";

  // Get the directory of the guard application
  ::GetModuleFileName(NULL, szPath, sizeofArray(szPath));
  _tsplitpath(itApp->second.m_strModuleName, szDrive, szDir, NULL, NULL);
  _tmakepath(szPath, szDrive, szDir, NULL, NULL);
  str += szPath;
  str += ";";

  // Get the symbol search path
  if (::GetEnvironmentVariable(TEXT("_NT_SYMBOL_PATH"), szPath,
    sizeofArray(szPath)))
  {
    str += szPath;
    str += ";";
  }

  // Get the alternate symbol search path
  if (::GetEnvironmentVariable(TEXT("_NT_ALTERNATE_SYMBOL_PATH"), szPath,
    sizeofArray(szPath)))
  {
    str += szPath;
    str += ";";
  }

  // Get the system root path
  if (::GetEnvironmentVariable(TEXT("SYSTEMROOT"), szPath,
    sizeofArray(szPath)))
  {
    str += szPath;
    str += ";";
  }

  // Return the composed string
  return str;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
ZString CAppModeDebug<T>::GetSearchPath()
{
  TCHAR szPath[_MAX_PATH * 2];
  ZString str;

  // Get the directory of the debuggee application
  CLoadedModuleIt itApp = m_Images.find(m_cpdi.lpBaseOfImage);
  if (itApp != m_Images.end())
  {
    TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
    _tsplitpath(itApp->second.m_strModuleName, szDrive, szDir, NULL, NULL);
    _tmakepath(szPath, szDrive, szDir, NULL, NULL);
    str += szPath;
    str += ";";
  }

  // Get the current directory
  ::GetCurrentDirectory(sizeofArray(szPath), szPath);
  str += szPath;
  str += ";";

  // Get the system directory
  if (::GetSystemDirectory(szPath, sizeofArray(szPath)))
  {
    str += szPath;
    str += ";";
  }

  // Get the Windows directory
  if (::GetWindowsDirectory(szPath, sizeofArray(szPath)))
  {
    // Get the 16-bit system directory
    static bool s_bIsWinNT = IsWinNT();
    if (s_bIsWinNT)
    {
      str += szPath;
      if (TEXT('\\') != szPath[_tcslen(szPath) - 1])
        str += TEXT("\\");
      str += TEXT("SYSTEM;");
    }

    // Append the Windows directory
    str += szPath;
    str += ";";
  }

  // Get the PATH environment variable
  int cch = ::GetEnvironmentVariable(TEXT("PATH"), NULL, 0) + 1;
  LPTSTR pszPath = (LPTSTR)_alloca(cch * sizeof(TCHAR));
  if (::GetEnvironmentVariable(TEXT("PATH"), pszPath, cch))
  {
    str += pszPath;
    str += ";";
  }

  // Return the composed string
  return str;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
ZString CAppModeDebug<T>::GetModuleWithPath(LPCTSTR pszModule)
{
  // Determine if the specified path is fully-qualified
  TCHAR szModule[_MAX_PATH * 2], szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
  _tsplitpath(pszModule, szDrive, szDir, NULL, NULL);
  if (TEXT('\0') == szDrive[0] && TEXT('\0') == szDir[0])
  {
    // Get the search path once
    if (m_strSearchPath.IsEmpty())
      m_strSearchPath = GetSearchPath();

    // Use SearchPath to find the file
    LPTSTR pszFilePart;
    if (::SearchPath(m_strSearchPath, pszModule, NULL, sizeofArray(szModule),
      szModule, &pszFilePart))
    {
      pszModule = szModule;
    }
  }

  // Resolve as a long filename, if possible
  if (m_libToolHelp.Exists_GetLongPathName())
    if (m_libToolHelp.GetLongPathName(pszModule, szModule, sizeofArray(szModule)))
      pszModule = szModule;

  // Return as a ZString
  return pszModule;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
BOOL CALLBACK CAppModeDebug<T>::ReadProcessMemoryProc(HANDLE hProcess,
  DWORD lpBaseAddress, PVOID lpBuffer, DWORD nSize,
  PDWORD lpNumberOfBytesRead)
{
  // Reinterpret the specified process handle
  CAppModeDebug<T>* pThis = reinterpret_cast<CAppModeDebug<T>*>(hProcess);

  // Delegate to ReadProcessMemory, with the correct hProcess
  return ::ReadProcessMemory(pThis->m_cpdi.hProcess,
    reinterpret_cast<void*>(lpBaseAddress), lpBuffer, nSize,
    lpNumberOfBytesRead);
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
PVOID CALLBACK CAppModeDebug<T>::FunctionTableAccessProc(HANDLE hProcess,
  DWORD AddrBase)
{
  // Reinterpret the specified process handle
  CAppModeDebug<T>* pThis = reinterpret_cast<CAppModeDebug<T>*>(hProcess);

  // Delegate to SymFunctionTableAccess, with the correct hProcess
  return SymFunctionTableAccess(pThis->m_hProcessSym, AddrBase);
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CALLBACK CAppModeDebug<T>::GetModuleBaseProc(HANDLE hProcess, DWORD Address)
{
  // Reinterpret the specified process handle
  CAppModeDebug<T>* pThis = reinterpret_cast<CAppModeDebug<T>*>(hProcess);

  // Lookup the specified address
  return ::SymGetModuleBase(pThis->m_hProcessSym, Address);
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CALLBACK CAppModeDebug<T>::TranslateAddressProc(HANDLE, HANDLE, LPADDRESS)
{
  // We don't translate 16-bit addresses
  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Overrides


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
DWORD CAppModeDebug<T>::OnException(bool* pbExit)
{
  // Ignore the very first breakpoint exception
  if (EXCEPTION_BREAKPOINT == m_pde->u.Exception.ExceptionRecord.ExceptionCode)
  {
    if (!g.GetHitFirstBreakpoint())
    {
      // Flag that we've now hit the first breakpoint
      g.SetHitFirstBreakpoint(true);

      // Initialize USER32, to satisfy the app-starting cursor
      MSG msg;
      ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
      ::PostThreadMessage(::GetCurrentThreadId(), WM_APP, 0, 0);
      ::GetMessage(&msg, NULL, 0, 0);

      // Allow the debuggee to continue
      return DBG_CONTINUE;
    }

    // Ignore the breakpoint if specified
    if (g.GetConfigBool("IgnoreBreakpoints", true))
      return DBG_CONTINUE;
  }

  // Ignore first-chance exceptions
  if (m_pde->u.Exception.dwFirstChance)
    return DBG_EXCEPTION_NOT_HANDLED;

  // Report the exception as specified in the configuration
  HRESULT hr = ReportException();
  if (FAILED(hr))
  {
    g.HandleError(hr, IDS_E_FMT_UNEXPECTED,
      "CAppModeDebug<T>::OnException calling ReportException()");
  }
  *pbExit = true;
  return DBG_CONTINUE;
}


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnCreateThread(bool* pbExit)
{
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnCreateProcess(bool* pbExit)
{
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnExitThread(bool* pbExit)
{
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnExitProcess(bool* pbExit)
{
  *pbExit = true;
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnLoadDLL(bool* pbExit)
{
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnUnloadDLL(bool* pbExit)
{
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnOutputDebugString(bool* pbExit)
{
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnRIP(bool* pbExit)
{
  *pbExit = true;
  return DBG_CONTINUE;
}

/////////////////////////////////////////////////////////////////////////////
//
template <class T>
inline DWORD CAppModeDebug<T>::OnOther(bool* pbExit)
{
  *pbExit = true;
  return DBG_CONTINUE;
}

