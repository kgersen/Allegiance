#ifndef __SrvConfigSheet_h__
#define __SrvConfigSheet_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// SrvConfigDlg.h : header file
//

#include "propgeneral.h"
#include "propautodownload.h"
#include "propdatabase.h"
#include "PropEvents.h"


/////////////////////////////////////////////////////////////////////////////
// CSrvConfigSheet dialog

class CSrvConfigSheet : public CPropertySheet
{
// Declarations
protected:
    DECLARE_MESSAGE_MAP()

// Construction
public:
    CSrvConfigSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    BOOL Create();

// Implementation
protected:
  void InitSysMenu();
  void InitIcon();
  void InitButtons();

// Overrides
public:
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CSrvConfigSheet)
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

// Message Handlers
protected:
    // Generated message map functions
    //{{AFX_MSG(CSrvConfigSheet)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnOK();
    afx_msg void OnCancel();
	  //}}AFX_MSG

// Data Members
protected:
    CPropGeneral      m_pp1;
    CPropAutoDownload m_pp2;
    CPropDatabase     m_pp3;
    CPropEvents       m_pp4;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////

#endif // !__SrvConfigSheet_h__
