// ShadowSwitcher.h : main header file for the SHADOWSWITCHER application
//

#if !defined(AFX_SHADOWSWITCHER_H__8717C759_9ABA_4B6B_9847_554D56E92967__INCLUDED_)
#define AFX_SHADOWSWITCHER_H__8717C759_9ABA_4B6B_9847_554D56E92967__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CShadowSwitcherApp:
// See ShadowSwitcher.cpp for the implementation of this class
//

class CShadowSwitcherApp : public CWinApp
{
public:
	CShadowSwitcherApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowSwitcherApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CShadowSwitcherApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWSWITCHER_H__8717C759_9ABA_4B6B_9847_554D56E92967__INCLUDED_)
