#if !defined(AFX_PROPZONELAUNCH_H__66168C57_E169_4979_AF70_B8D566B17D23__INCLUDED_)
#define AFX_PROPZONELAUNCH_H__66168C57_E169_4979_AF70_B8D566B17D23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropZoneLaunch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropZoneLaunch dialog

class CPropZoneLaunch : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropZoneLaunch)

// Construction
public:
	CPropZoneLaunch();
	~CPropZoneLaunch();

// Dialog Data
	//{{AFX_DATA(CPropZoneLaunch)
	enum { IDD = IDD_ZONELAUNCH };
	CString	m_strZoneCmdLine;
	CString	m_strZoneCurrentDir;
	CString	m_strZoneExeName;
	CString	m_strZoneFilePath;
	CString	m_strZoneGUID;
	CString	m_strZoneVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropZoneLaunch)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropZoneLaunch)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPZONELAUNCH_H__66168C57_E169_4979_AF70_B8D566B17D23__INCLUDED_)
