#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigConfig.h : main header file for the PigConfig application
//


/////////////////////////////////////////////////////////////////////////////
// CPigConfigApp: See PigConfig.cpp for the implementation of this class.
//
class CPigConfigApp : public CWinApp
{
public:
    CPigConfigApp();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPigConfigApp)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation

    //{{AFX_MSG(CPigConfigApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////
