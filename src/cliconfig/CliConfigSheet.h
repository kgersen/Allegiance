#ifndef __CliConfigSheet_h__
#define __CliConfigSheet_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CliConfigDlg.h : header file
//

#include "PropGeneral.h"
#include "PropSound.h"
#include "PropZoneLaunch.h"


/////////////////////////////////////////////////////////////////////////////
// CCliConfigSheet dialog

class CCliConfigSheet : public CPropertySheet
{
// Declarations
protected:
    DECLARE_MESSAGE_MAP()

// Construction
public:
    CCliConfigSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    BOOL Create();

// Implementation
protected:
  void InitSysMenu();
  void InitIcon();
  void InitButtons();

// Overrides
public:
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CCliConfigSheet)
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

// Message Handlers
protected:
    // Generated message map functions
    //{{AFX_MSG(CCliConfigSheet)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnOK();
    afx_msg void OnCancel();
    //}}AFX_MSG

// Data Members
protected:
    CPropGeneral      m_PageGeneral;
    CPropSound        m_PageSound;
    CPropZoneLaunch   m_PageZoneLaunch;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////

#endif // !__CliConfigSheet_h__
