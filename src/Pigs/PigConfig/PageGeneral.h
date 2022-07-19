#pragma once

/////////////////////////////////////////////////////////////////////////////
// PageGeneral.h : Declaration of the CPageGeneral class.
//

#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigConfigSheet;


/////////////////////////////////////////////////////////////////////////////
// CPageGeneral dialog

class CPageGeneral : public CPropertyPage
{
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CPageGeneral(CPigConfigSheet* pSheet);

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CPageGeneral)
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
  //{{AFX_MSG(CPageGeneral)
  virtual BOOL OnInitDialog();
  afx_msg void OnUpdatesPerSecondDefault();
  afx_msg void OnChangeUpdatesPerSecond();
  afx_msg void OnLimitPigs();
  //}}AFX_MSG

// Data Members
protected:
  //{{AFX_DATA(CPageGeneral)
  enum { IDD = IDD_GENERAL };
  CSpinButtonCtrl m_spinMaxPigs;
  CEdit m_editMaxPigs;
  CStatic m_staticMaxPigs;
  CString m_strArtPath;
  CString m_strPigletDir;
  BOOL  m_bLimitPigs;
  CString m_strVersion;
  CString m_strFlavor;
  long  m_cMaxPigs;
  //}}AFX_DATA
protected:
  CPigConfigSheet* m_pSheet;
};


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigConfigSheet* CPageGeneral::GetSheet()
{
  return m_pSheet;
}


/////////////////////////////////////////////////////////////////////////////
