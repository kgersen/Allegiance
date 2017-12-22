#pragma once

/////////////////////////////////////////////////////////////////////////////
// AboutDlg.h : Declaration of the CAboutDlg class.
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Dialog box used for App About
//
class CAboutDlg : public CDialog
{
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CAboutDlg();

// Overrides
protected:
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Message Handlers
protected:
  //{{AFX_MSG(CAboutDlg)
  //}}AFX_MSG

// Data Members
protected:
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA
};


/////////////////////////////////////////////////////////////////////////////

