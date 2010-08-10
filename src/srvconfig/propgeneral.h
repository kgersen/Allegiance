#if !defined(AFX_PROPGENERAL_H__8883EB88_B79D_4AD4_B009_CC29F0205F37__INCLUDED_)
#define AFX_PROPGENERAL_H__8883EB88_B79D_4AD4_B009_CC29F0205F37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropGeneral dialog

class CPropGeneral : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropGeneral)

// Construction
public:
    CPropGeneral();
    ~CPropGeneral();

// Dialog Data
    //{{AFX_DATA(CPropGeneral)
	enum { IDD = IDD_GENERAL };
    CButton m_btnUpdatesPerSecondDefault;
    CSpinButtonCtrl m_spinUpdatesPerSecond;
    CString   m_strFileArtDir;
    CString   m_strLobbyServer;
    BOOL      m_bTimeout;
    BOOL      m_bWantInt3;
    int       m_nUpdatesPerSecond;
	BOOL	m_bProtocol;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropGeneral)
    public:
    virtual void OnOK();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropGeneral)
    virtual BOOL OnInitDialog();
    afx_msg void OnUpdatesPerSecondDefault();
    afx_msg void OnChangeUpdatesPerSecond();
  //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

// Constants
protected:
    enum
    {
      c_nUpdatesPerSecondDefault = 10,
      c_nUpdatesPerSecondMin     =  1,
      c_nUpdatesPerSecondMax     = 30,
    };

// Data Member
protected:
    UINT m_idDefaultButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGENERAL_H__8883EB88_B79D_4AD4_B009_CC29F0205F37__INCLUDED_)
