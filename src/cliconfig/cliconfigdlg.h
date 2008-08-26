// CliConfigDlg.h : header file
//

#if !defined(AFX_CliConfigDLG_H__A9B0D790_7FFA_11D2_BFE5_00A0C9C9CCA4__INCLUDED_)
#define AFX_CliConfigDLG_H__A9B0D790_7FFA_11D2_BFE5_00A0C9C9CCA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCliConfigDlg dialog

class CCliConfigDlg : public CDialog
{
// Construction
public:
	CCliConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCliConfigDlg)
	enum { IDD = IDD_CLICONFIG_DIALOG };
	CComboBox	m_comboServer;
	CString	m_strArtPath;
	BOOL	m_bCaptureFrameData;
	CString	m_strFrameDataFilename;
	BOOL	m_bLogToFile;
	CDate	m_dateArt;
	BOOL	m_bCockpit;
	BOOL	m_bEnvironment;
	BOOL	m_bMusic;
	BOOL	m_bPosters;
	BOOL	m_bRoundRadar;
	BOOL	m_bSoftwareHUD;
	CString	m_strZoneCommandLine;
	CString	m_strZoneCurrentDir;
	CString	m_strZoneExeName;
	CString	m_strZoneFilePath;
	CString	m_strZoneGUID;
	CString	m_strZoneVersion;
	CString	m_strZoneLobby;
	//}}AFX_DATA

    CString m_strRetailFilename;
    CString m_strTestFilename;
    CString m_strDebugFilename;
    CString m_strOtherFilename;
    CString m_strServer;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCliConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCliConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnCaptureframedata();
	afx_msg void OnDebugbuild();
	afx_msg void OnExefilenamebutton();
	afx_msg void OnArtpathbutton();
	afx_msg void OnRetailbuild();
	afx_msg void OnOtherbuild();
	afx_msg void OnFramefilebutton();
	afx_msg void OnTestbuild();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CliConfigDLG_H__A9B0D790_7FFA_11D2_BFE5_00A0C9C9CCA4__INCLUDED_)
