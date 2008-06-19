/////////////////////////////////////////////////////////////////////////////
// MSRGuard.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "MSRGuard.h"
#include "resource.h"

#include "AppMode.h"
#include "AppModeHelp.h"
#include "AppModeCreate.h"
#include "AppModeAttach.h"
#include "AppModeBind.h"


/////////////////////////////////////////////////////////////////////////////
// Application entry point
//
int _tmain(int argc, TCHAR* argv[])
{
  // Explicitly initialize the global module instance
  g.Init();

  // Simply delegate to the global module instance
  return g.DoMain(argc, argv);
}


/////////////////////////////////////////////////////////////////////////////
// CMSRGuardModule


/////////////////////////////////////////////////////////////////////////////
// Global Initialization

CMSRGuardModule g;


/////////////////////////////////////////////////////////////////////////////
// Static Initialization - Fixed for VC9 - Imago

const CMSRGuardModule::XOptionFlag CMSRGuardModule::s_OptionFlags[] =
{
  {TEXT("?"   ), -1, &CMSRGuardModule::OnOptionHelp  },
  {TEXT("help"),  1, &CMSRGuardModule::OnOptionHelp  },
  {TEXT("p"   ), -1, &CMSRGuardModule::OnOptionPID   },
  {TEXT("ini" ),  1, &CMSRGuardModule::OnOptionINI   },
  {TEXT("mini"),  1, &CMSRGuardModule::OnOptionMINI  },
  {TEXT("html"),  3, &CMSRGuardModule::OnOptionHTML  },
  {TEXT("bind"), -1, &CMSRGuardModule::OnOptionBind  },
  {TEXT("s"   ), -1, &CMSRGuardModule::OnOptionSilent},
};


/////////////////////////////////////////////////////////////////////////////
// Construction


/////////////////////////////////////////////////////////////////////////////
// Default constructor
//
CMSRGuardModule::CMSRGuardModule() :
  m_hinst(NULL),
  m_hinstResource(NULL),
  m_cOptionsEaten(0),
  m_pAppMode(NULL),
  m_bHitFirstBreakpoint(false)
{
}


/////////////////////////////////////////////////////////////////////////////
// Explicit initialization
//
void CMSRGuardModule::Init()
{
  // Get the HINSTANCE of the current module
  m_hinst = GetModuleHandle(NULL);

  // Resources, by default, come from the same module
  m_hinstResource = m_hinst;

  // Load the default title string
  ZString strTitle;
  ZSucceeded(g.LoadString(strTitle, IDS_TITLE));
  m_Configs["Title"] = strTitle;
}



/////////////////////////////////////////////////////////////////////////////
// Operations


/////////////////////////////////////////////////////////////////////////////
//
int CMSRGuardModule::DoMain(int argc, TCHAR* argv[])
{
	int i; //-Imago
  // Loop through command-line arguments until a non-option is found
  for (i = 1; i < argc; ++i)
  {
    if (TEXT('-') != argv[i][0] && TEXT('/') != argv[i][0])
      break;

    int cEaten = ProcessCmdLineOption(i, argc, argv);
    if (-1 == cEaten)
      break;
    else if (cEaten < 0)
      return cEaten;
    i += cEaten;
  }

  // Save the number of command line arguments that were eaten by option flags
  m_cOptionsEaten = i - 1;

  // Get the remainder of the command-line as a single string
  int cchCmdLineMax = lstrlen(GetCommandLine());
  LPTSTR pszCmdLine = reinterpret_cast<LPTSTR>(_alloca(cchCmdLineMax));
  ZeroMemory(pszCmdLine, cchCmdLineMax);
  for (i; i < argc; ++i)
  {
    if (TEXT('\0') != *pszCmdLine)
      lstrcat(pszCmdLine, TEXT(" "));
    bool bHasSpace = !!_tcschr(argv[i], TEXT(' '));
    if (bHasSpace)
      lstrcat(pszCmdLine, TEXT("\""));
    lstrcat(pszCmdLine, argv[i]);
    if (bHasSpace)
      lstrcat(pszCmdLine, TEXT("\""));
  }

  // Save the remainder of the command-line
  m_Options[ZString()] = pszCmdLine;

  // Dump all of the Option flag values under debug builds
  #ifdef _DEBUG
    debugf("Map of Option flags:\n");
    for (XOptionMapIt it = m_Options.begin(); it != m_Options.end(); ++it)
      debugf("  '%s' %s\n", (LPCTSTR)it->first, (LPCTSTR)it->second);
    debugf("\n");
  #endif // _DEBUG

  // Load the configuration file
  HRESULT hr = LoadConfig();
  if (FAILED(hr))
    return static_cast<int>(hr);

  // Pre-load the HTML file and MINI file just to ensure that they can be
  ZString strTemp;
  if (FAILED(hr = LoadMINI(strTemp)))
    return static_cast<int>(hr);
  if (FAILED(hr = LoadHTML(strTemp)))
    return static_cast<int>(hr);
  strTemp.SetEmpty();

  // If m_pAppMode was not set by the options, set it now
  if (!m_pAppMode)
    SetAppMode(CreateAppModeCreate());

  // Turn control over to the AppMode object
  hr = m_pAppMode->Run(argc, argv);

  // Release the AppMode object
  m_pAppMode->Release();

  // Return the last HRESULT
  return static_cast<int>(hr);
}


/////////////////////////////////////////////////////////////////////////////
//
//
HRESULT CMSRGuardModule::HandleErrorV(HRESULT hr, UINT idFmt, va_list argptr)
{
  ZString strMsg;
  ZSucceeded(FormatStringV(strMsg, idFmt, argptr));

  // Concatenate the error string onto the message
  strMsg += TEXT("\n\n");
  strMsg += GetErrorString(hr);

  // Display a message box
  if (GetOptionExists("s"))
    _putts(strMsg);
  else
    ::MessageBox(NULL, strMsg, GetTitle(), MB_OK | MB_ICONERROR);

  // Return the specified HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
//
HRESULT CMSRGuardModule::HandleError(HRESULT hr, UINT idFmt, ...)
{
  va_list argptr;
  va_start(argptr, idFmt);
  HandleErrorV(hr, idFmt, argptr);
  va_end(argptr);
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::DisplayMessageV(UINT idFmt, va_list argptr)
{
  ZString strMsg;
  ZSucceeded(FormatStringV(strMsg, idFmt, argptr));

  if (GetOptionExists("s"))
    _putts(strMsg);
  else
    ::MessageBox(NULL, strMsg, GetTitle(), MB_OK | MB_ICONINFORMATION);

  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::DisplayMessage(UINT idFmt, ...)
{
  va_list argptr;
  va_start(argptr, idFmt);
  DisplayMessageV(idFmt, argptr);
  va_end(argptr);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
ZString CMSRGuardModule::GetErrorString(HRESULT hr)
{
  ZString str;

  // Format the specified error code into a string
  LPTSTR pszError = NULL;
  if (::FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL, hr, 0, reinterpret_cast<LPTSTR>(&pszError), 0, NULL))
  {
    str = pszError;
    ::LocalFree(pszError);
  }
  else
  {
    ZSucceeded(FormatString(str, IDS_E_FMT_UNKNOWN_HR, hr));
  }

  return str;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::LoadString(ZString& strOut, UINT idRes)
{
  TCHAR szBuf[16 * 1024];
  if (!::LoadString(GetResourceInstance(), idRes, szBuf, sizeofArray(szBuf)))
  {
    strOut.SetEmpty();
    return HRESULT_FROM_WIN32(::GetLastError());
  }
  strOut = szBuf;
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::FormatStringV(ZString& strOut, UINT idFmt, va_list argptr)
{
  ZString strFmt;
  RETURN_FAILED(LoadString(strFmt, idFmt));

  // Delegate to another method
  return FormatStringV(strOut, strFmt, argptr);
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::FormatString(ZString& strOut, UINT idFmt, ...)
{
  va_list argptr;
  va_start(argptr, idFmt);
  HRESULT hr = FormatStringV(strOut, idFmt, argptr);
  va_end(argptr);
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::FormatStringV(ZString& strOut, LPCTSTR pszFmt, va_list argptr)
{
  // Initialize the specified output string
  strOut.SetEmpty();

  // Attempt to use a reasonably-sized stack buffer first
  TCHAR szBuf[64 * 1024];
  int cch = sizeofArray(szBuf);
  if (0 <= _vsntprintf(szBuf, cch, pszFmt, argptr))
  {
    strOut = szBuf;
    return S_OK;
  }

  // Attempt to find a large-enough buffer
  const int c_cchMax = (4 * 1024 * 1024) / sizeof(TCHAR);
  cch *= 2;
  HANDLE hHeap = ::GetProcessHeap();
  if (!hHeap)
    return HRESULT_FROM_WIN32(::GetLastError());
  TCHAR* pszBuf = reinterpret_cast<TCHAR*>(::HeapAlloc(hHeap, 0, cch));
  if (!pszBuf)
    return HRESULT_FROM_WIN32(::GetLastError());
  do
  {
    int cchActual = ::HeapSize(hHeap, 0, pszBuf);
    if (0 <= _vsnprintf(pszBuf, cchActual, pszFmt, argptr))
    {
      strOut = pszBuf;
      ::HeapFree(hHeap, 0, pszBuf);
      return S_OK;
    }

    // Reallocate a larger buffer
    cch *= 2;
    TCHAR* pszBuf2 = reinterpret_cast<TCHAR*>(
      ::HeapReAlloc(hHeap, 0, pszBuf, cch));
    if (pszBuf2)
    {
      pszBuf = pszBuf2;
    }
    else
    {
      DWORD dwLastError = ::GetLastError();
      ::HeapFree(hHeap, 0, pszBuf);
      return HRESULT_FROM_WIN32(dwLastError);
    }

  } while (cch < c_cchMax);

  // We put our foot down at 4MB!
  return E_OUTOFMEMORY;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::FormatString(ZString& strOut, LPCTSTR pszFmt, ...)
{
  va_list argptr;
  va_start(argptr, pszFmt);
  HRESULT hr = FormatStringV(strOut, pszFmt, argptr);
  va_end(argptr);
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::LoadMINI(ZString& strOut)
{
  ZString strMINIPath;
  if (GetOptionValue("MINI", strMINIPath))
  {
    ZFile file(strMINIPath);
    ZString strMINI((LPCTSTR)file.GetPointer(), file.GetLength());
    strOut = strMINI;
  }
  else
  {
    LPCTSTR pszData;
    DWORD cbData;
    RETURN_FAILED(LockAndLoadResource(GetResourceInstance(), TEXT("MINI"),
      MAKEINTRESOURCE(1), (void**)&pszData, &cbData));
    ZString strMINI(pszData, (int)cbData);
    strOut = strMINI;
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT CMSRGuardModule::LoadHTML(ZString& strOut)
{
  ZString strHTMLPath;
  if (GetOptionValue("HTML", strHTMLPath))
  {
    ZFile file(strHTMLPath);
    ZString strHTML((LPCTSTR)file.GetPointer(), file.GetLength());
    strOut = strHTML;
  }
  else
  {
    LPCTSTR pszData;
    DWORD cbData;
    RETURN_FAILED(LockAndLoadResource(GetResourceInstance(), RT_HTML,
      MAKEINTRESOURCE(1), (void**)&pszData, &cbData));
    ZString strHTML(pszData, (int)cbData);
    strOut = strHTML;
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
HKEY CMSRGuardModule::HKEYFromString(const ZString& strHKEY)
{
  // Types
  struct XStringToHKEY
  {
    LPCTSTR m_pszHKEY;
    HKEY    m_hkey;
  };
  typedef const XStringToHKEY* XStringToHKEYIt;

  // Static Initialization
  const static XStringToHKEY s_HKEYs[] =
  {
    // Most common values
    {TEXT("HKEY_LOCAL_MACHINE"   ), HKEY_LOCAL_MACHINE   },
    {TEXT("HKLM"                 ), HKEY_LOCAL_MACHINE   },
    {TEXT("HKEY_CURRENT_USER"    ), HKEY_CURRENT_USER    },
    {TEXT("HKCU"                 ), HKEY_CURRENT_USER    },
    // Seldom-used, but supported non-the-less
    {TEXT("HKEY_CLASSES_ROOT"    ), HKEY_CLASSES_ROOT    },
    {TEXT("HKCR"                 ), HKEY_CLASSES_ROOT    },
    {TEXT("HKEY_CURRENT_CONFIG"  ), HKEY_CURRENT_CONFIG  },
    {TEXT("HKCC"                 ), HKEY_CURRENT_CONFIG  },
    {TEXT("HKEY_USERS"           ), HKEY_USERS           },
    {TEXT("HKU"                  ), HKEY_USERS           },
    {TEXT("HKEY_PERFORMANCE_DATA"), HKEY_PERFORMANCE_DATA},
    {TEXT("HKPD"                 ), HKEY_PERFORMANCE_DATA},
    {TEXT("HKEY_DYN_DATA"        ), HKEY_DYN_DATA        },
    {TEXT("HKDD"                 ), HKEY_DYN_DATA        },
  };
  const static XStringToHKEYIt itEnd =
    s_HKEYs + sizeofArray(s_HKEYs);

  // Find the handler for the specified debug event
  for (XStringToHKEYIt it = s_HKEYs; it != itEnd; ++it)
  {
    if (0 == _tcsicmp(it->m_pszHKEY, strHKEY))
      return it->m_hkey;
  }

  // Returns NULL if it didn't find the specified string
  return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
//
int CMSRGuardModule::ProcessCmdLineOption(int i, int argc, TCHAR* argv[])
{
  // Get the option string past the '-' or '/' character
  LPCTSTR pszOption = argv[i] + 1;
  int cchOption = lstrlen(pszOption);

  // Look for the command-line option in the table
  XOptionFlagIt itEnd = s_OptionFlags + sizeofArray(s_OptionFlags);
  for (XOptionFlagIt it = s_OptionFlags; it != itEnd; ++it)
  {
    // Compute the minimum number of significant characters
    int cchEntry = lstrlen(it->m_szOptionFlag);
    int cchMin = (it->m_cchSignificant <= 0) ?
      cchEntry : it->m_cchSignificant;

    // Copy the strings to work buffers
    LPTSTR pszEntry   = (LPTSTR)_alloca(cchEntry  + 1);
    LPTSTR pszCompare = (LPTSTR)_alloca(cchOption + 1);
    lstrcpy(pszEntry  , it->m_szOptionFlag);
    lstrcpy(pszCompare, pszOption);

    // Compare down to the minimum number of significant characters
    int cch = cchOption;
    while (cch-- >= cchMin)
    {
      // Truncate the entry by one character
      *(pszEntry + min(cch + 1, cchEntry)) = TEXT('\0');

      // Process the option flag if it matches
      if (!_tcsicmp(pszCompare, pszEntry))
      {
        int cEaten = (this->*it->m_pfn)(i, argc, argv);
        if (cEaten >= 0)
        {
          ZString strOption(it->m_szOptionFlag);
          ZString strParam;
          for (int j = 0; j < cEaten; ++j)
          {
            if (j)
              strParam += " ";
            strParam += argv[i + j + 1];
          }
          XOptionMapIt itFind = m_Options.find(strOption);
          if (itFind != m_Options.end())
            strParam = itFind->second + " " + strParam;
          m_Options[strOption] = strParam;
        }

        // Indicate the number of extra parameters eaten or error code
        return cEaten;
      }

      // Truncate the option by one character
//      *(pszCompare + cch) = TEXT('\0');
    }
  }

  // Option flag not recognized
  return static_cast<int>(-1);
};


/////////////////////////////////////////////////////////////////////////////
//
//
HRESULT CMSRGuardModule::LoadConfig()
{
  bool bIsTemp = false;
  ZString strINI, str;
  if (!GetOptionValue(TEXT("INI"), strINI) || GetOptionValue(TEXT("bind"), str))
  {
    bIsTemp = true;

    // Load the CONFIG resource
    void* pvData = NULL;
    DWORD cbData = 0;
    HRESULT hr = LockAndLoadResource(GetResourceInstance(), TEXT("CONFIG"),
      MAKEINTRESOURCE(1), &pvData, &cbData);
    if (FAILED(hr))
      return HandleError(hr, IDS_E_LOADCONFIGRES);

    // Create a temporary filename
    TCHAR szTempPath[_MAX_PATH];
    if (!::GetTempPath(sizeofArray(szTempPath), szTempPath))
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()), IDS_E_TEMPPATH);
    TCHAR szTempFile[_MAX_PATH];
    if (!::GetTempFileName(szTempPath, TEXT("MSRG"), 0, szTempFile))
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()), IDS_E_TEMPFILENAME);
    strINI = szTempFile;

    // Open the temporary file for writing
    DWORD dwFlags = FILE_ATTRIBUTE_TEMPORARY;
    TCHandle shFile = ::CreateFile(strINI, GENERIC_WRITE, FILE_SHARE_READ,
      NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | dwFlags, NULL);
    if (INVALID_HANDLE_VALUE == shFile.GetHandle())
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()), IDS_E_CREATETEMP);

    // Write the data to the file
    DWORD cbWritten;
    if (!::WriteFile(shFile, pvData, cbData, &cbWritten, NULL))
    {
      HRESULT hr = HandleError(HRESULT_FROM_WIN32(::GetLastError()), IDS_E_CREATETEMP);
      shFile = NULL;
      ::DeleteFile(strINI);
      return hr;
    }
  }

  // Read all of the key names
  TCHAR szBuffer[32 * 1024];
  ::GetPrivateProfileString(TEXT("MSRGuard"), NULL, TEXT(""), szBuffer,
    sizeofArray(szBuffer), strINI);

  // Add the value of each key to the config map
  LPCSTR psz = szBuffer;
  int cch;
  do
  {
    if (0 != (cch = lstrlen(psz)))
    {
      TCHAR szValue[_MAX_PATH * 4];
      ::GetPrivateProfileString(TEXT("MSRGuard"), psz, TEXT(""), szValue,
        sizeofArray(szValue), strINI);
      m_Configs[psz] = szValue;
      psz += cch + 1;
    }
  } while (cch);

  // Delete the file, !!! ONLY if it's a temporary !!!
  if (bIsTemp)
    ::DeleteFile(strINI);

  // Dump all of the Config values under debug builds
  #ifdef _DEBUG
    debugf("Map of configuration settings:\n");
    for (XConfigMapIt it = m_Configs.begin(); it != m_Configs.end(); ++it)
      debugf("  '%s'=%s\n", (LPCTSTR)it->first, (LPCTSTR)it->second);
    debugf("\n");
  #endif // _DEBUG

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
//
void CMSRGuardModule::SetAppMode(IAppMode* pAppMode)
{
  if (m_pAppMode)
    m_pAppMode->Release();
  m_pAppMode = pAppMode;
}


/////////////////////////////////////////////////////////////////////////////
// Option Flag Handlers


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionHelp(int i, int argc, TCHAR* argv[])
{
  // Set the app mode to display help
  SetAppMode(CreateAppModeHelp());

  // We eat all the rest of the parameters
  return argc - 1 - i;
}


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionPID(int i, int argc, TCHAR* argv[])
{
  // Ensure that there is another argument after this one
  if ((argc - 1) <= i)
    return E_INVALIDARG;

  // Set the app mode to attach to the specified running process
  SetAppMode(CreateAppModeAttach());

  // We eat the next parameter
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionINI(int i, int argc, TCHAR* argv[])
{
  // Ensure that there is another argument after this one
  if ((argc - 1) <= i)
    return E_INVALIDARG;

  // Ensure that the file is fully-qualified
  static TCHAR s_szFullPath[_MAX_PATH * 2];
  LPTSTR pszFilePart;
  if (!::GetFullPathName(argv[i + 1], sizeofArray(s_szFullPath),
    s_szFullPath, &pszFilePart))
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_READINIFILE, argv[i + i]);
  argv[i + 1] = s_szFullPath;

  // Ensure that the specified file can be read
  TCHandle shFile = ::CreateFile(argv[i + 1], GENERIC_READ, FILE_SHARE_READ,
    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (shFile.IsNull() || INVALID_HANDLE_VALUE == shFile.GetHandle())
    return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
      IDS_E_FMT_READINIFILE, argv[i + i]);

  // We eat the next parameter
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionMINI(int i, int argc, TCHAR* argv[])
{
  // Ensure that there is another argument after this one
  if ((argc - 1) <= i)
    return E_INVALIDARG;

  // Ensure that the file is fully-qualified
  static TCHAR s_szFullPath[_MAX_PATH * 2];
  LPTSTR pszFilePart;
  if (!::GetFullPathName(argv[i + 1], sizeofArray(s_szFullPath),
    s_szFullPath, &pszFilePart))
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_READINIFILE, argv[i + i]);
  argv[i + 1] = s_szFullPath;

  // Ensure that the specified file can be read
  TCHandle shFile = ::CreateFile(argv[i + 1], GENERIC_READ, FILE_SHARE_READ,
    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (shFile.IsNull() || INVALID_HANDLE_VALUE == shFile.GetHandle())
    return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
      IDS_E_FMT_READINIFILE, argv[i + i]);

  // We eat the next parameter
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionHTML(int i, int argc, TCHAR* argv[])
{
  // Ensure that there is another argument after this one
  if ((argc - 1) <= i)
    return E_INVALIDARG;

  // Ensure that the file is fully-qualified
  static TCHAR s_szFullPath[_MAX_PATH * 2];
  LPTSTR pszFilePart;
  if (!::GetFullPathName(argv[i + 1], sizeofArray(s_szFullPath),
    s_szFullPath, &pszFilePart))
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_READHTMLFILE, argv[i + i]);
  argv[i + 1] = s_szFullPath;

  // Ensure that the specified file can be read
  TCHandle shFile = ::CreateFile(argv[i + 1], GENERIC_READ, FILE_SHARE_READ,
    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (shFile.IsNull() || INVALID_HANDLE_VALUE == shFile.GetHandle())
    return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
      IDS_E_FMT_READHTMLFILE, argv[i + i]);

  // We eat the next parameter
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionBind(int i, int argc, TCHAR* argv[])
{
  // Do not allow on Win9x
  if (IsWin9x())
    return HandleError(E_INVALIDARG, IDS_E_BINDWIN9X);

  // Ensure that there is another argument after this one
  if ((argc - 1) <= i)
    return E_INVALIDARG;

  // Ensure that the file is fully-qualified
  static TCHAR s_szFullPath[_MAX_PATH * 2];
  LPTSTR pszFilePart;
  if (!::GetFullPathName(argv[i + 1], sizeofArray(s_szFullPath),
    s_szFullPath, &pszFilePart))
      return HandleError(HRESULT_FROM_WIN32(::GetLastError()),
        IDS_E_FMT_READINIFILE, argv[i + i]);
  argv[i + 1] = s_szFullPath;

  // Set the app mode to perform the bind
  SetAppMode(CreateAppModeBind());

  // We eat the next parameter
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
//
//
int CMSRGuardModule::OnOptionSilent(int i, int argc, TCHAR* argv[])
{
  // We don't eat any parameters
  return 0;
}


