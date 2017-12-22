#pragma once

/////////////////////////////////////////////////////////////////////////////
// PageServers.h : Declaration of the CPageServers class.
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigConfigSheet;


/////////////////////////////////////////////////////////////////////////////
// CPageServers dialog

class CPageServers : public CPropertyPage
{
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CPageServers(CPigConfigSheet* pSheet);

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CPageServers)
  public:
  virtual void OnOK();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  //}}AFX_VIRTUAL

// Implementation
protected:
  CPigConfigSheet* GetSheet();
  void UpdateUI();

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CPageServers)
  virtual BOOL OnInitDialog();
  afx_msg void OnSameAsLobby();
  afx_msg void OnMyComputer();
  afx_msg void OnThisComputer();
  //}}AFX_MSG

// Data Members
protected:
  //{{AFX_DATA(CPageServers)
	enum { IDD = IDD_SERVERS };
	CEdit	m_editAccountServer;
  CString m_strLobbyServer;
  BOOL  m_bZoneLobby;
  CString m_strAccountServer;
  int   m_nAccountServer;
	//}}AFX_DATA
protected:
  CPigConfigSheet* m_pSheet;
};


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigConfigSheet* CPageServers::GetSheet()
{
  return m_pSheet;
}


/////////////////////////////////////////////////////////////////////////////
