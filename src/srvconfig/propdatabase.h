#if !defined(AFX_PROPDATABASE_H__0AA13A0A_96A4_41FA_82F8_C230D9EAF537__INCLUDED_)
#define AFX_PROPDATABASE_H__0AA13A0A_96A4_41FA_82F8_C230D9EAF537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropDatabase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropDatabase dialog

class CPropDatabase : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropDatabase)

// Construction
public:
    CPropDatabase();
    ~CPropDatabase();

// Dialog Data
    //{{AFX_DATA(CPropDatabase)
	enum { IDD = IDD_DATABASE };
	CEdit	m_editConnectionString2;
    CEdit m_editConnectionString;
    CString   m_strSQLPassword;
    CString   m_strSQLSrc;
    CString   m_strSQLUser;
    CString   m_strConnectionString;
	CString	m_strConnectionString2;
	int		m_nNotifyThreads;
	int		m_nSilentThreads;
	int		m_nNotifyThreads2;
	int		m_nSilentThreads2;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropDatabase)
    public:
    virtual void OnOK();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
  static int CALLBACK WordBreakProc(LPTSTR, int, int, int);

// Message Handlers
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropDatabase)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnconnectionstring();
	afx_msg void OnBtnconnectionstring2();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPDATABASE_H__0AA13A0A_96A4_41FA_82F8_C230D9EAF537__INCLUDED_)
