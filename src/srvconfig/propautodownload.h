#if !defined(AFX_PROPAUTODOWNLOAD_H__73DF7E09_8EF9_4980_AFC9_25D51EA68E5E__INCLUDED_)
#define AFX_PROPAUTODOWNLOAD_H__73DF7E09_8EF9_4980_AFC9_25D51EA68E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropAutoDownload.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropAutoDownload dialog

class CPropAutoDownload : public CPropertyPage
{
    DECLARE_DYNCREATE(CPropAutoDownload)

// Construction
public:
    CPropAutoDownload();
    ~CPropAutoDownload();

// Dialog Data
    //{{AFX_DATA(CPropAutoDownload)
	enum { IDD = IDD_AUTODOWNLOAD };
    CString   m_strFtpPassword;
    CString   m_strFtpServer;
    CString   m_strFtpUserID;
    BOOL    m_bWatchArtDir;
	CString	m_strFtpRoot;
	//}}AFX_DATA


// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPropAutoDownload)
    public:
    virtual void OnOK();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPropAutoDownload)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPAUTODOWNLOAD_H__73DF7E09_8EF9_4980_AFC9_25D51EA68E5E__INCLUDED_)
