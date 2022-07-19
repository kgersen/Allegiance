/////////////////////////////////////////////////////////////////////////////
// AllSrvUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "AllSrvUISheet.h"


#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// The one and only CAllSrvUIApp object

CAllSrvUIApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CAllSrvUIApp


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CAllSrvUIApp, CWinApp)
  //{{AFX_MSG_MAP(CAllSrvUIApp)
  //}}AFX_MSG
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CAllSrvUIApp::CAllSrvUIApp() :
  m_hrCoInit(E_FAIL)
{
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

void CAllSrvUIApp::GetArtPath(char * szArtPath)
{
  strcpy(szArtPath, ".\\Artwork\\");

  CRegKey key;
  if (ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE, HKLM_FedSrv, KEY_READ))
  {
    ZString strArtPath;
    if (SUCCEEDED(LoadRegString(key, "Artpath", strArtPath)))
    {
      // if reg value exists copy over default
      strncpy(szArtPath, PCC(strArtPath), MAX_PATH);
    }
    // ensure last character is a backslash
    int nLast = max(0, strlen(szArtPath)-1);
    if (szArtPath[nLast] != '\\' || szArtPath[nLast] != '/')
      szArtPath[nLast+1] = '\\';
    szArtPath[nLast+2] = 0;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

BOOL CAllSrvUIApp::InitInstance()
{
  // Determine if another instance is already running
  HANDLE hEvent = GetAllSrvUIEvent();
  if (hEvent)
  {
    // Signal the other instance to activate itself
    ::SetEvent(hEvent);

    // Close the event handle and exit
    ::CloseHandle(hEvent);
    return false;
  }

  // Ensure that the EULA has been agreed to
  // yp your_persona march 25 2006 : Remove EULA.dll dependency patch
  //HRESULT hr = FirstRunEula();
  //if (E_FAIL == hr)
  //{
  //  ::MessageBox(0, "Error while loading loading ebueula.dll", "Allegiance Server", 0);
  //  return false;
  //}
  //else if (S_FALSE == hr)
  //{
  //  ::MessageBox(0, "You must accept the End User License Agreement before running the Allegiance Server", "Allegiance Server", 0);
  //  return false;
  //}
  //else 
  //{
  //  assert(S_OK == hr);
  //}

  // Create the instance event
  CreateAllSrvUIEvent();

  // Initialize COM
  m_hrCoInit = CoInitializeEx(NULL, COINIT_MULTITHREADED);
//  m_hrCoInit = CoInitialize(NULL);

  // Create the property sheet window
  CAllSrvUISheet* pps = new CAllSrvUISheet;

  // Create the property sheet as a modeless dialog box
  if (pps->Create())
    m_pMainWnd = pps;

  // Continue processing the application
  return true;
}

int CAllSrvUIApp::ExitInstance()
{
  // Uninitialize COM
  if (SUCCEEDED(m_hrCoInit))
    CoUninitialize();

  // Perform default processing
  return CWinApp::ExitInstance();
}

BOOL CAllSrvUIApp::OnIdle(LONG lCount)
{
  HANDLE hEvents[] = {m_shEventSync};
  DWORD dwWait = ::MsgWaitForMultipleObjects(sizeofArray(hEvents), hEvents,
    false, INFINITE, QS_ALLINPUT);
  if (WAIT_OBJECT_0 == dwWait)
  {
    ::ResetEvent(m_shEventSync);
    m_pMainWnd->SetForegroundWindow();
    m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
  }
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

HANDLE CAllSrvUIApp::GetAllSrvUIEvent()
{
  // Open the mutex using the global name first
  DWORD dwAccess = EVENT_MODIFY_STATE | SYNCHRONIZE;
  HANDLE hEvent = ::OpenEvent(dwAccess, false, szAllSrvUIRunningGlobal);
  if (!hEvent)
    hEvent = ::OpenEvent(dwAccess, false, szAllSrvUIRunning);
  return hEvent;
}

void CAllSrvUIApp::CreateAllSrvUIEvent()
{
  // Create a NULL dacl to give "everyone" access
  SECURITY_ATTRIBUTES* psa = NULL;
  SECURITY_DESCRIPTOR   sd;
  SECURITY_ATTRIBUTES   sa = {sizeof(sa), &sd, false};
  if (IsWinNT())
  {
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, true, NULL, FALSE);
    psa = &sa;
  }

  // Create the event using the global name first
  HANDLE hEvent = ::CreateEvent(psa, true, false, szAllSrvUIRunningGlobal);
  if (!hEvent)
    hEvent = ::CreateEvent(psa, true, false, szAllSrvUIRunning);
  m_shEventSync = hEvent;
}

//
// EULA related files should be in the artwork folder so that they may be autoupdated
//
// yp your_persona march 25 2006 : Remove EULA.dll dependency patch
//
//HRESULT CAllSrvUIApp::FirstRunEula()
//{
//  TCHAR   szArtpath[MAX_PATH];
//  GetArtPath(szArtpath);
//  PathString strArtPath = szArtpath;
//  PathString strEulaRTF = strArtPath + "eula.rtf";
//
//  {
//    ZFile file(strEulaRTF);
//    int n = file.GetLength(); // -1 means error
//    if (n == -1)
//        return E_FAIL;
//  }
//
//  // don't use += operator cause it's buggy with PathString
//  strArtPath = strArtPath + "EBUEula.dll";
//
//  HINSTANCE hMod = LoadLibrary(PCC(strArtPath));
//  if (NULL == hMod)       // can't attach to DLL
//  {
//    // this time, search path
//    hMod = LoadLibrary("EBUEula.dll");
//    if (NULL == hMod)       // can't attach to DLL
//      return E_FAIL;
//  }
//
//  EBUPROC pfnEBUEula = (EBUPROC) GetProcAddress(hMod, "EBUEula");
//  if (NULL == pfnEBUEula)     // can't find entry point
//  {
//    FreeLibrary(hMod);
//    return E_FAIL;
//  }
//
//  /*
//  TCHAR   szWarranty[MAX_PATH];
//  LoadString(GetModuleHandle(), STR_EULAFILENAME, szEULA, sizeof(szEULA));
//  LoadString(GetModuleHandle(), STR_WARRANTYNAME, szWarranty, sizeof(szWarranty));
//
//  //
//  //This call enables both EULA and warranty accepting/viewing/printing.  If your
//  //game doesn't ship with a WARRANTY file, specifiy NULL instead of szWarranty…
//  //The code below, for instance, works with both OEM and retail builds…
//  //
//  TCHAR *pszWarrantyParam = 0xFFFFFFFF != GetFileAttributes(szWarranty) ? szWarranty : NULL;
//  */
//  bool bAllowGameToRun = pfnEBUEula(HKLM_AllSrvUI, PCC(strEulaRTF), NULL, TRUE) != 0;
//
//  FreeLibrary(hMod);
//
//  return (bAllowGameToRun ? S_OK : S_FALSE);
//}


