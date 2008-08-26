#if !defined(AFX_PROPGENERAL_H__01F71EEA_CF3C_405D_A0FF_7A2264432848__INCLUDED_)
#define AFX_PROPGENERAL_H__01F71EEA_CF3C_405D_A0FF_7A2264432848__INCLUDED_

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// PropGeneral.h : header file
//

#include "cdate.h"


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
	CComboBox	m_comboConfig;
    CDate m_dateArt;
    CString m_strArtPath;
    BOOL  m_bCaptureFrameData;
    BOOL  m_bCockpit;
    BOOL  m_bEnvironment;
    CString m_strFrameFileName;
    BOOL  m_bLogToFile;
    BOOL  m_bPosters;
    BOOL  m_bRoundRadar;
    BOOL  m_bSoftwareHUD;
	BOOL	m_bBiDiLighting;
	BOOL	m_bCenterHUD;
	BOOL	m_bChatHistoryHUD;
	BOOL	m_bDebris;
	BOOL	m_bLensFlare;
	BOOL	m_bLinearResponse;
	BOOL	m_bStars;
	BOOL	m_bStrobes;
	BOOL	m_bTargetHUD;
	BOOL	m_bTrails;
	//}}AFX_DATA
//    CString m_strZoneLobby;


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
	afx_msg void OnViewbtn();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGENERAL_H__01F71EEA_CF3C_405D_A0FF_7A2264432848__INCLUDED_)
