/////////////////////////////////////////////////////////////////////////////
// TCDeployment.cpp : Implementation of the TCDeployment class.

#include "pch.h"
#include "TCDeploy.h"
#include "TCDeploymentProcess.h"
#include "TCDeploymentVersion.h"
#include "TCDeployment.h"

/////////////////////////////////////////////////////////////////////////////
// TCDeployment

TC_OBJECT_EXTERN_IMPL(TCDeployment)


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

TCAutoCriticalSection TCDeployment::m_csCurDir;
TCAutoCriticalSection TCDeployment::m_csStatic;
bool                  TCDeployment::m_bNoExit = false;


/////////////////////////////////////////////////////////////////////////////
// TCDeployment::XWaitThreadArgs
//
class TCDeployment::XWaitThreadArgs
{
// Construction
public:
  XWaitThreadArgs(TCDeployment* pThis, TCDeploymentProcess* pProcess) :
    m_spThis(pThis->GetUnknown()),
    m_pThis(pThis),
    m_spProcess(pProcess->GetUnknown()),
    m_hProcess(pProcess->GetHandle())
  {
  }
// Attributes
public:
  IUnknownPtr             m_spThis;
  TCDeployment*           m_pThis;
  ITCDeploymentProcessPtr m_spProcess;
  HANDLE                  m_hProcess;
};


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT TCDeployment::CreateTheProcess(BSTR bstrCmdLine, BSTR bstrDirectory,
  BSTR bstrStdOut, BSTR bstrStdErr, BSTR bstrStdIn, ShowCmd eShowCmd,
  PROCESS_INFORMATION* pProcessInfo)
{
  USES_CONVERSION;

  // Initialize the [out] parameter
  assert(pProcessInfo);
  ZeroMemory(pProcessInfo, sizeof(*pProcessInfo));

  // Ensure that a command line was specified
  if (!BSTRLen(bstrCmdLine))
    return E_INVALIDARG;

  // Validate the specified ShowCmd parameter
  if (ShowCmd_Min > eShowCmd || eShowCmd > ShowCmd_Max)
    return E_INVALIDARG;

  // Set security under WinNT
  SECURITY_ATTRIBUTES* psa = NULL;
  SECURITY_DESCRIPTOR  sd;
  SECURITY_ATTRIBUTES  sa = {sizeof(sa), &sd, true};
  if (IsWinNT())
  {
    // Create a NULL dacl to give "everyone" access
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, true, NULL, FALSE);
    psa = &sa;
  }

  // Create a table of each redirection file to be possibly created
  TCHandle shStdOut, shStdErr, shStdIn;
  struct
  {
    BSTR   bstr;   TCHandle* psh; DWORD dwAccess; DWORD dwCreation;
  } table[] =
  {
    {bstrStdOut, shStdOut.This(),  GENERIC_WRITE,   CREATE_ALWAYS},
    {bstrStdErr, shStdErr.This(),  GENERIC_WRITE,   CREATE_ALWAYS},
    {bstrStdIn ,  shStdIn.This(),  GENERIC_READ ,   OPEN_EXISTING},
  };

  // Create each of the redirection handles specified
  for (int i = 0; i < sizeofArray(table); ++i)
  {
    // Do nothing for empty strings
    if (!BSTRLen(table[i].bstr))
      continue;

    // Special processing for StdErr
    if (table[i].bstr == bstrStdErr)
    {
      // If StdErr is the same file as StdOut, just re-use the handle
      if (!shStdOut.IsNull() && 0 == _wcsicmp(bstrStdErr, bstrStdOut))
      {
        if (!DuplicateHandle(GetCurrentProcess(), shStdOut,
          GetCurrentProcess(), &shStdErr, 0, true, DUPLICATE_SAME_ACCESS))
            return HRESULT_FROM_WIN32(::GetLastError());
        continue;
      }
    }

    // Split the file into its components
    OLECHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
    OLECHAR szFname[_MAX_FNAME], szExt[_MAX_EXT];
    _wsplitpath(table[i].bstr, szDrive, szDir, szFname, szExt);
 
    // If no path was specified, use bstrDirectory, if specified
    LPOLESTR psz = table[i].bstr;
    OLECHAR szPath[_MAX_PATH];
    if (BSTRLen(bstrDirectory) && L'\0' == *szDrive && L'\0' == *szDir)
      _wmakepath(psz = szPath, NULL, bstrDirectory, szFname, szExt);

    // Convert the name of the file to TEXT-width characters
    LPCTSTR pszRedir = OLE2CT(psz);

    // Attempt to open the specified file
    HANDLE hFile = CreateFile(pszRedir, table[i].dwAccess, FILE_SHARE_READ,
      psa, table[i].dwCreation, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
      return HRESULT_FROM_WIN32(::GetLastError());

    // Redirect the Std handle to the file
    *table[i].psh = hFile;
  }

  // Initialize the STARTUPINFO structure
  STARTUPINFO si = {sizeof(si)};
  si.dwFlags     = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = (WORD)eShowCmd;
  si.hStdInput   = shStdIn;
  si.hStdOutput  = shStdOut;
  si.hStdError   = shStdErr;

  // Set the working directory, if specified
  TCHAR szCurDirPrev[_MAX_PATH] = TEXT("");
  if (BSTRLen(bstrDirectory))
  {
    // Lock the critical section
    m_csCurDir.Lock();

    // Change the current directory
    GetCurrentDirectory(sizeofArray(szCurDirPrev), szCurDirPrev);
    SetCurrentDirectory(OLE2CT(bstrDirectory));
  }

  // Call CreateProcess
  HRESULT hr           = S_OK;
  LPTSTR  pszCmdLine   = OLE2T(bstrCmdLine);
  LPCTSTR pszDirectory = BSTRLen(bstrDirectory) ? OLE2CT(bstrDirectory):NULL;
  if (!CreateProcess(NULL, pszCmdLine, psa, psa, true,
    CREATE_NEW_CONSOLE, NULL, pszDirectory, &si, pProcessInfo))
      hr = HRESULT_FROM_WIN32(GetLastError());

  // Restore the previous current directory, if any
  if (TEXT('\0') != *szCurDirPrev)
  {
    SetCurrentDirectory(szCurDirPrev);
    m_csCurDir.Unlock();
  }

  // Output the parameters, _DEBUG build or not
  TCERRLOG_BEGIN
    TCERRLOG_PART0("TCDeployment::CreateTheProcess():\n");
    TCERRLOG_PART1("  CmdLine= \"%ls\"\n", bstrCmdLine);
    TCERRLOG_PART1("  Directory= \"%ls\"\n",
      BSTRLen(bstrDirectory) ? bstrDirectory : L"");
    TCERRLOG_PART1("  StdOut= \"%ls\"\n",
      BSTRLen(bstrStdOut) ? bstrStdOut : L"");
    TCERRLOG_PART1("  StdErr= \"%ls\"\n",
      BSTRLen(bstrStdErr) ? bstrStdErr : L"");
    TCERRLOG_PART1("  StdIn= \"%ls\"\n",
      BSTRLen(bstrStdIn) ? bstrStdIn : L"");
    TCERRLOG_PART1("  eShowCmd= %d\n", eShowCmd);
    TCERRLOG_PART2("  ProcessID= 0x%X (%d)\n\n",
      pProcessInfo->dwProcessId, pProcessInfo->dwProcessId);
  TCERRLOG_END

  // Return last result
  return hr;
}


HRESULT WINAPI TCDeployment::QI_TCDeploymentClass(void* pv, REFIID riid,
  void** ppv, DWORD dw)
{
  assert(IID_ITCDeploymentClass == riid);

  // Get the process name into which we're loaded
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(NULL, szModule, sizeofArray(szModule));

  // Split off the name and extension
  TCHAR szFname[_MAX_FNAME], szExt[_MAX_EXT];
  _tsplitpath(szModule, NULL, NULL, szFname, szExt);
  _tmakepath(szModule, NULL, NULL, szFname, szExt);

  // This interface not available unless running under the surrogate process
  if (0 != _tcsicmp(szModule, TEXT("dllhost.exe")))
    return E_NOINTERFACE;

  // Cast the void pointer as a TCDeployment*
  TCDeployment* pThis = reinterpret_cast<TCDeployment*>(pv);

  // Cast our class pointer as an ITCDeploymentClass
  ITCDeploymentClass* ptcdc = static_cast<ITCDeploymentClass*>(pThis);

  // Copy the interface pointer to the [out] parameter
  CLEAROUT(ppv, (void*)ptcdc);

  // AddRef the interface pointer
  ptcdc->AddRef();

  // Indicate success
  return S_OK;
}


HRESULT TCDeployment::RegUnregServer(BSTR bstrModule, BSTR* pbstrResult,
  LPSTR pszExport)
{
  // Initialize the [out] parameter, if any
  CLEAROUT_ALLOW_NULL(pbstrResult, (BSTR)NULL);

  // Validate the specified module parameter
  if (!BSTRLen(bstrModule))
    return E_INVALIDARG;

  // Attempt to load and register/unregister the specified module
  TCHAR szCurDirPrev[_MAX_PATH] = TEXT("");
  HRESULT hr;
  HRESULT hrEI = S_FALSE;
  IErrorInfo* pei = NULL;
  __try
  {
    // Convert the specified module name to TEXT
    USES_CONVERSION;
    LPCTSTR pszModule = OLE2CT(bstrModule);

    // Break the module name into pieces
    TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szPath[_MAX_PATH];
    _tsplitpath(pszModule, szDrive, szDir, NULL, NULL);
    _tmakepath(szPath, szDrive, szDir, NULL, NULL);

    // Set the working directory, if module is so qualified
    if (TEXT('\0') != *szPath)
    {
      // Lock the critical section
      m_csCurDir.Lock();

      // Change the current directory
      GetCurrentDirectory(sizeofArray(szCurDirPrev), szCurDirPrev);
      SetCurrentDirectory(szPath);
    }

    // Load the specified module
    HINSTANCE hinst = ::LoadLibrary(pszModule);
    if (!hinst)
      RaiseException(GetLastError(), 0, 0, NULL);

    // Get the specified export procedure address
    typedef HRESULT (CALLBACK *PFNREG)();
    PFNREG pfn = (PFNREG)GetProcAddress(hinst, pszExport);

    // Call the function
    hr = (*pfn)();

    // Save any error info if failed
    if (FAILED(hr))
      hrEI = ::GetErrorInfo(0, &pei);

    // Unload the specified module
    FreeLibrary(hinst);
  }
  __except(1)
  {
    hr = HRESULT_FROM_WIN32(GetExceptionCode());
  }

  // Format the result string, if an [out] parameter was specified
  if (pbstrResult)
  {
    OLECHAR szBuf[_MAX_PATH];
    if (SUCCEEDED(hr))
      swprintf(szBuf, OLESTR("%hs in %ls succeeded\n"), pszExport, bstrModule);
    else
    {
      // Format the system error into string
      LPSTR* pszMsgBuf = NULL;
      ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr,
        0, (LPSTR)&pszMsgBuf, 0, NULL);
      swprintf(szBuf, OLESTR("%hs in %ls failed: 0x%08X %hs\n"),
        pszExport, bstrModule, hr,
        (*pszMsgBuf && '\0' != **pszMsgBuf) ? *pszMsgBuf : "");
      LocalFree(pszMsgBuf);
    }

    // Allocate the result string
    *pbstrResult = SysAllocString(szBuf);
    if (!*pbstrResult)
      return E_OUTOFMEMORY;
  }

  // Restore the previous current directory, if any
  if (TEXT('\0') != *szCurDirPrev)
  {
    SetCurrentDirectory(szCurDirPrev);
    m_csCurDir.Unlock();
  }

  // Restore the error info, if any
  if (S_OK == hrEI)
  {
    ::SetErrorInfo(0, pei);
    if (pei)
      pei->Release();
  }

  // Return the last result
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// ITCDeploymentQuit Interface Methods

STDMETHODIMP TCDeployment::Quit()
{
  return E_NOTIMPL;
}


/////////////////////////////////////////////////////////////////////////////
// ITCDeploymentClass Interface Methods

STDMETHODIMP TCDeployment::put_NoExit(VARIANT_BOOL bNoExit)
{
  assert(ITCDeploymentClassPtr(GetUnknown()) != NULL);

  // Lock static variables for this
  XLockStatic lock(&m_csStatic);

  // Ensure that only one extra external reference is managed
  if (!bNoExit == !m_bNoExit)
    return S_FALSE;

  // Add/remove an extra external reference to the object
  RETURN_FAILED(CoLockObjectExternal(GetUnknown(), !!bNoExit, false));

  // Save the state of the external reference
  m_bNoExit = !!bNoExit;

  // Indicate success
  return S_OK;  
}


STDMETHODIMP TCDeployment::get_NoExit(VARIANT_BOOL* pbNoExit)
{
  // Lock static variables for this
  XLockStatic lock(&m_csStatic);
  CLEAROUT(pbNoExit, VARBOOL(m_bNoExit));
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo Interface Methods

STDMETHODIMP TCDeployment::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ITCDeployment,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ITCDeployment Interface Methods

STDMETHODIMP TCDeployment::CreateObject(BSTR bstrProgID, BSTR bstrComputer,
  IUnknown** ppUnk)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::CreateObject(bstrProgID, bstrComputer, ppUnk);
}


STDMETHODIMP TCDeployment::ObjectReference(IUnknown* pUnk,
  BSTR* pbstrObjectReference)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::ObjectReference(pUnk, pbstrObjectReference);
}


STDMETHODIMP TCDeployment::Sleep(long nDurationMS)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::Sleep(nDurationMS);
}


STDMETHODIMP TCDeployment::ExecuteAndWait(BSTR bstrCmdLine,
  BSTR bstrDirectory, BSTR bstrStdOut, BSTR bstrStdErr, BSTR bstrStdIn,
  ShowCmd eShowCmd, long* pnExitCode)
{
  // Initialize the [out] parameter
  CLEAROUT_ALLOW_NULL(pnExitCode, 0L);

  // Create the process
  PROCESS_INFORMATION pi;
  RETURN_FAILED(CreateTheProcess(bstrCmdLine, bstrDirectory, bstrStdOut,
    bstrStdErr, bstrStdIn, eShowCmd, &pi));

  ///////////////////////////////////////////////////////////////////////////
  // Wait for the process to exit
  //
  // Note: We call ::MsgWaitForMultipleObjects here instead of
  // ::WaitForSingleObject for a good reason -- COM will have created message
  // loops to handle Asynchronous Procedure Calls. We can't just put the
  // calling thread to sleep -- all the messages will just queue up, which
  // could be very bad if this waits for a long time.
  //
  DWORD dwWait;
  do
  {
    dwWait = ::MsgWaitForMultipleObjects(1, &pi.hProcess, false, INFINITE, QS_ALLINPUT);
    if ((WAIT_OBJECT_0 + 1) == dwWait)
    {
      // See if we woke up because of a message:
      MSG msg;
      if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
      }
    }
  } while (WAIT_OBJECT_0 != dwWait);

  // Copy the process exit code to the [out] parameter
  DWORD dwExitCode = 0;
  GetExitCodeProcess(pi.hProcess, &dwExitCode);
  CLEAROUT(pnExitCode, (long)dwExitCode);

  // Indicate success
  return S_OK;
}


STDMETHODIMP TCDeployment::Execute(BSTR bstrCmdLine, BSTR bstrDirectory,
  BSTR bstrStdOut, BSTR bstrStdErr, BSTR bstrStdIn, ShowCmd eShowCmd,
  ITCDeploymentProcess** ppProcess)
{
  // Initialize the [out] parameter
  CLEAROUT_ALLOW_NULL(ppProcess, (ITCDeploymentProcess*)NULL);

  // Create the process
  PROCESS_INFORMATION pi;
  RETURN_FAILED(CreateTheProcess(bstrCmdLine, bstrDirectory, bstrStdOut,
    bstrStdErr, bstrStdIn, eShowCmd, &pi));

  // Create the process COM object
  CComObject<TCDeploymentProcess>* pProcess = NULL;
  RETURN_FAILED(pProcess->CreateInstance(&pProcess));
  pProcess->Init(pi.hProcess);
  ITCDeploymentProcessPtr spProcess(pProcess->GetUnknown());

  // Detach the process COM object to the [out] parameter
  if (ppProcess)
    *ppProcess = spProcess.Detach();

  // Indicate success
  return S_OK;
}


STDMETHODIMP TCDeployment::RegServer(BSTR bstrModule, BSTR* pbstrResult)
{
  // Delegate to helper method
  return RegUnregServer(bstrModule, pbstrResult, "DllRegisterServer");
}


STDMETHODIMP TCDeployment::UnregServer(BSTR bstrModule, BSTR* pbstrResult)
{
  // Delegate to helper method
  return RegUnregServer(bstrModule, pbstrResult, "DllUnregisterServer");
}


STDMETHODIMP TCDeployment::get_Version(ITCDeploymentVersion** ppVersion)
{
  // Initialize the [out] parameter
  CLEAROUT(ppVersion, (ITCDeploymentVersion*)NULL);

  // Create an instance of the version object
  CComObject<TCDeploymentVersion>* pVersion = NULL;
  RETURN_FAILED(pVersion->CreateInstance(&pVersion));
  ITCDeploymentVersionPtr spVersion(pVersion);

  // Initialize the version object
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(_Module.GetModuleInstance(), szModule, sizeofArray(szModule));
  RETURN_FAILED(spVersion->put_FileName(CComBSTR(szModule)));

  // Detach the object to the [out] parameter
  *ppVersion = spVersion.Detach();

  // Indicate success
  return S_OK;
}

STDMETHODIMP TCDeployment::GetObject(BSTR bstrMoniker, VARIANT_BOOL bAllowUI,
  long nTimeoutMS, IUnknown** ppUnk)
{
  // Delegate to TCUtilImpl class method
  return TCUtilImpl::GetObject(bstrMoniker, bAllowUI, nTimeoutMS, ppUnk);
}


STDMETHODIMP TCDeployment::WriteStreamToFile(BSTR bstrFileName,
  VARIANT* pvarStream, VARIANT_BOOL bOverwrite)
{
  // Fail if the specified filename is empty
  if (!BSTRLen(bstrFileName))
    return E_INVALIDARG;

  // Fail if the specified variant is NULL
  if (!pvarStream)
    return E_POINTER;

  // Fail if the specified variant is not an IUnknown, IDispatch, or IStream
  const VARTYPE vt = V_VT(pvarStream);
  if (VT_UNKNOWN != vt && VT_DISPATCH != vt && VT_STREAM != vt)
    return E_INVALIDARG;

  // Fail if the specified variant does not support IStream
  IStreamPtr spStream(V_UNKNOWN(pvarStream));
  if (NULL == spStream)
    return E_NOINTERFACE;

  // Open the specified file
  USES_CONVERSION;
  TCHandle shFile = ::CreateFile(OLE2CT(bstrFileName), GENERIC_WRITE,
    FILE_SHARE_READ, NULL, bOverwrite ? CREATE_ALWAYS : CREATE_NEW,
    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  if (shFile.IsNull() || INVALID_HANDLE_VALUE == shFile.GetHandle())
    return HRESULT_FROM_WIN32(::GetLastError());

  // Get the size of the stream
  LARGE_INTEGER li = {0};
  ULARGE_INTEGER uli;
  RETURN_FAILED(spStream->Seek(li, STREAM_SEEK_END, &uli));
  DWORD cbStream = uli.LowPart;
  if (uli.HighPart)
    return E_UNEXPECTED;

  // Seek to the beginning of the stream
  RETURN_FAILED(spStream->Seek(li, STREAM_SEEK_SET, NULL));

  // Copy bytes from the stream to the file
  BYTE pbData[4096];
  do
  {
    ULONG cbData = min(sizeof(pbData), cbStream);
    cbStream -= cbData;
    RETURN_FAILED(spStream->Read(pbData, cbData, NULL));
    DWORD cbWritten;
    if (!::WriteFile(shFile, pbData, cbData, &cbWritten, NULL))
      return HRESULT_FROM_WIN32(::GetLastError());

  } while (cbStream);

  // Indicate success
  return S_OK;
}


STDMETHODIMP TCDeployment::GetProcessID(BSTR bstrName, long* pdwProcessID)
{
  // Fail if the specified filename is empty
  if (!BSTRLen(bstrName))
    return E_INVALIDARG;

  // Fail if running under Win9x
  if (IsWin9x())
    return E_FAIL;
  
  // in case we don't find the process
  *pdwProcessID = 0;

  // if there are more than 256 processes running, we may not find ours
  DWORD dwProcess[256]; 
  DWORD cbRet=0;
  if (0 == EnumProcesses(dwProcess, sizeof(dwProcess), &cbRet))
    return E_FAIL;

  for (int i=0;i<cbRet/sizeof(dwProcess[0]);i++)
  {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcess[i]);
    if (hProcess)  // if we couldn't get a process handle for the id, ignore it...might've been timing
    {
      HMODULE hModule;
      DWORD cbRetModule=0;
      EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbRetModule);
      if (cbRetModule > 0)  // not an error to have none...might've been timing
      {
        OLECHAR szModuleBaseName[64];
        DWORD szModuleBaseNameLen = GetModuleBaseNameW(hProcess, hModule, szModuleBaseName, sizeofArray(szModuleBaseName));

        if (szModuleBaseNameLen > 0)
        {
          if (lstrcmpiW(szModuleBaseName,bstrName)==0)
          {
            *pdwProcessID = static_cast<long>(dwProcess[i]);
            return S_OK;
          }
        }
      }
    }
  }
  
  // no errors, we just didn't find what they were looking for.
  return S_FALSE;
}
