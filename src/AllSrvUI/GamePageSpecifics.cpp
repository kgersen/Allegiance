/////////////////////////////////////////////////////////////////////////////
// GamePageSpecifics.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "GamePageSpecifics.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGamePageSpecifics property page

IMPLEMENT_DYNAMIC(CGamePageSpecifics, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CGamePageSpecifics, CPropertyPage)
  //{{AFX_MSG_MAP(CGamePageSpecifics)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CGamePageSpecifics::CGamePageSpecifics() :
  CPropertyPage(CGamePageSpecifics::IDD)
{
  //{{AFX_DATA_INIT(CGamePageSpecifics)
  //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CGamePageSpecifics::OnOK() 
{
  // Perform default processing
  CPropertyPage::OnOK();
}

BOOL CGamePageSpecifics::OnSetActive()
{
  // Perform default processing
  BOOL bResult = CPropertyPage::OnSetActive();

  // Return the result of the default processing
  return bResult;
}

void CGamePageSpecifics::DoDataExchange(CDataExchange* pDX)
{
  // Perform default processing
  CPropertyPage::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CGamePageSpecifics)
  //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CGamePageSpecifics::OnInitDialog() 
{
  // Perform default processing
  CPropertyPage::OnInitDialog();

  // Return true to set the focus to the first tabstop control
  return true;
}


