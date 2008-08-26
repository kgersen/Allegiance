/////////////////////////////////////////////////////////////////////////////
// SrvConfig.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SrvConfig.h"
#include "SrvConfigSheet.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSrvConfigApp

BEGIN_MESSAGE_MAP(CSrvConfigApp, CWinApp)
    //{{AFX_MSG_MAP(CSrvConfigApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSrvConfigApp construction

CSrvConfigApp::CSrvConfigApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CSrvConfigApp object

CSrvConfigApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CSrvConfigApp initialization

BOOL CSrvConfigApp::InitInstance()
{
    // Initialize COM
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // Create the property sheet window
    CSrvConfigSheet* pps = new CSrvConfigSheet;

    // Create the property sheet as a modeless dialog box
    if (pps->Create())
      m_pMainWnd = pps;

    // Continue processing the application
    return true;
}

int CSrvConfigApp::ExitInstance()
{
  // Uninitialize COM
  CoUninitialize();

  // Perform default processing
  return CWinApp::ExitInstance();
}