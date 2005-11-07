#if !defined(AFX_SHOWCONFIGDLG_H__4CB7E91B_1398_4274_85D5_7123BBE09933__INCLUDED_)
#define AFX_SHOWCONFIGDLG_H__4CB7E91B_1398_4274_85D5_7123BBE09933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShowConfigDlg dialog

class CShowConfigDlg : public CDialog
{
// Construction
public:
	CShowConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShowConfigDlg)
	enum { IDD = IDD_SHOWCONFIG };
	CString	m_strContents;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShowConfigDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWCONFIGDLG_H__4CB7E91B_1398_4274_85D5_7123BBE09933__INCLUDED_)
