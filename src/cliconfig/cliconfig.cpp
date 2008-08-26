/////////////////////////////////////////////////////////////////////////////
// CliConfig.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CliConfig.h"
#include "CliConfigSheet.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCliConfigApp

BEGIN_MESSAGE_MAP(CCliConfigApp, CWinApp)
    //{{AFX_MSG_MAP(CCliConfigApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCliConfigApp construction

CCliConfigApp::CCliConfigApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CCliConfigApp object

CCliConfigApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CCliConfigApp initialization

BOOL CCliConfigApp::InitInstance()
{
    // Create the property sheet window
    CCliConfigSheet* pps = new CCliConfigSheet;

    // Create the property sheet as a modeless dialog box
    if (pps->Create())
      m_pMainWnd = pps;

    // Continue processing the application
    return true;
}

