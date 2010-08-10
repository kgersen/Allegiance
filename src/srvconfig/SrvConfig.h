#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#if !defined(AFX_SrvConfig_H__A9B0D78E_7FFA_11D2_BFE5_00A0C9C9CCA4__INCLUDED_)
#define AFX_SrvConfig_H__A9B0D78E_7FFA_11D2_BFE5_00A0C9C9CCA4__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// SrvConfig.h : main header file for the SrvConfig application
//

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "..\inc\regkey.h"


/////////////////////////////////////////////////////////////////////////////
// CSrvConfigApp:
// See SrvConfig.cpp for the implementation of this class
//

class CSrvConfigApp : public CWinApp
{
public:
    CSrvConfigApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSrvConfigApp)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CSrvConfigApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_SrvConfig_H__A9B0D78E_7FFA_11D2_BFE5_00A0C9C9CCA4__INCLUDED_)
