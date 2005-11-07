// ShadowSwitcherDlg.h : header file
//

#if !defined(AFX_SHADOWSWITCHERDLG_H__7054CA7E_8EBD_4F97_8B90_24FC1B610A29__INCLUDED_)
#define AFX_SHADOWSWITCHERDLG_H__7054CA7E_8EBD_4F97_8B90_24FC1B610A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShadowSwitcherDlg dialog

#define StringSize 1024

class CShadowSwitcherDlg : public CDialog
{
// Construction
public:
	bool CheckArtPath();
	CShadowSwitcherDlg(CWnd* pParent = NULL);	// standard constructor
    CShadowSwitcherDlg::~CShadowSwitcherDlg();

// Dialog Data
	//{{AFX_DATA(CShadowSwitcherDlg)
	enum { IDD = IDD_SHADOWSWITCHER_DIALOG };
	CButton	m_btnShadow;
	CButton	m_btnRetail;
	CButton	m_btnDelete;
	CStatic	m_text;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowSwitcherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CShadowSwitcherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDelete();
	afx_msg void OnRetail();
	afx_msg void OnShadow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HKEY m_hKey;
    char m_achExePath[StringSize];
    char m_achArtPath[StringSize];
    char m_achShadowPath[StringSize];
    char m_achRetailPath[StringSize];
    char m_achCfgSave[StringSize];
    char m_achCfgFile[StringSize];
    char m_achPID[StringSize];
    bool m_bShadow;
    bool m_bInstalled;

    void DeleteDirectory(char* pch);
    void CopyDirectory(char* pchFrom, char* pchTo);
	bool GetRegistrySetting(char* pszValue, char* pszResult, DWORD length);
    void SetRegistrySetting(char *pszValue, char *pszResult);
	void Error(char* psz);
    void Install();
	void Update();
    void UpdateText(char* ps);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWSWITCHERDLG_H__7054CA7E_8EBD_4F97_8B90_24FC1B610A29__INCLUDED_)
