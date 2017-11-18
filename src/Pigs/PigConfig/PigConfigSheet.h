#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigConfigSheet.h : Declaration of the CPigConfigSheet class.
//

#include "PageGeneral.h"
#include "PageServers.h"
#include "PageEvents.h"


/////////////////////////////////////////////////////////////////////////////
// CPigConfigSheet dialog

class CPigConfigSheet : public CPropertySheet
{
// Declarations
protected:
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CPigConfigSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
  bool Create();

// Attributes
public:
  IPigSession* GetSession();

// Implementation
protected:
  void InitSysMenu();
  void InitIcon();
  void InitButtons();

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CPigConfigSheet)
  virtual void PostNcDestroy();
  //}}AFX_VIRTUAL

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CPigConfigSheet)
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnOK();
  afx_msg void OnCancel();
  //}}AFX_MSG

// Data Members
protected:
  IPigSessionPtr  m_spSession;
  CPageGeneral    m_PageGeneral;
  CPageServers    m_PageServers;
  CPageEvents     m_PageEvents;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline IPigSession* CPigConfigSheet::GetSession()
{
  return m_spSession;
}


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////
