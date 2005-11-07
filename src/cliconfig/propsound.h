#if !defined(AFX_PROPSOUND_H__80E15D62_F1B2_40BE_8ADE_B60D23FC8A1A__INCLUDED_)
#define AFX_PROPSOUND_H__80E15D62_F1B2_40BE_8ADE_B60D23FC8A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSound.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropSound dialog

class CPropSound : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropSound)

// Construction
public:
	CPropSound();
	~CPropSound();

// Dialog Data
	//{{AFX_DATA(CPropSound)
	enum { IDD = IDD_SOUND };
	BOOL	m_bHardwareSound;
	BOOL	m_bMusic;
	int		m_nMusicGain;
	int		m_nSoundFXGain;
	int		m_nVoiceOverGain;
	int		m_nSoundQuality;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropSound)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropSound)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSOUND_H__80E15D62_F1B2_40BE_8ADE_B60D23FC8A1A__INCLUDED_)
