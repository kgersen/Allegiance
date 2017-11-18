#ifndef __GamePageSpecifics_h__
#define __GamePageSpecifics_h__

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// GamePageSpecifics.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// Forward Declaration

class CGameParamsSheet;


/////////////////////////////////////////////////////////////////////////////
// CGamePageSpecifics dialog

class CGamePageSpecifics : public CPropertyPage
{
// Declarations
public:
  DECLARE_DYNAMIC(CGamePageSpecifics)
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CGamePageSpecifics();

// Dialog Data
  //{{AFX_DATA(CGamePageSpecifics)
  enum { IDD = IDD_GAME_SPECIFICS};
  //}}AFX_DATA

// Attributes
public:

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CGamePageSpecifics)
  public:
  virtual void OnOK();
  virtual BOOL OnSetActive();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CGamePageSpecifics)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

// Data Members
protected:
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////

#endif // !__GamePageSpecifics_h__
