/////////////////////////////////////////////////////////////////////////////
// PigConfig.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "PigConfig.h"
#include "PigConfigSheet.h"


/////////////////////////////////////////////////////////////////////////////
// CPigConfigApp

BEGIN_MESSAGE_MAP(CPigConfigApp, CWinApp)
    //{{AFX_MSG_MAP(CPigConfigApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPigConfigApp construction

CPigConfigApp::CPigConfigApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CPigConfigApp object

CPigConfigApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CPigConfigApp initialization

BOOL CPigConfigApp::InitInstance()
{
  // Initialize COM
  CoInitializeEx(NULL, COINIT_MULTITHREADED);

  // Create the property sheet window
  CPigConfigSheet* pps = new CPigConfigSheet;

  // Create the property sheet as a modeless dialog box
  if (pps->Create())
    m_pMainWnd = pps;

  // Continue processing the application
  return true;
}

int CPigConfigApp::ExitInstance()
{
  // Uninitialize COM
  CoUninitialize();

  // Perform default processing
  return CWinApp::ExitInstance();
}