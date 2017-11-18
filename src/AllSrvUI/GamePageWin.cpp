/////////////////////////////////////////////////////////////////////////////
// GamePageWin.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "GamePageWin.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGamePageWin property page

IMPLEMENT_DYNAMIC(CGamePageWin, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CGamePageWin, CPropertyPage)
  //{{AFX_MSG_MAP(CGamePageWin)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CGamePageWin::CGamePageWin() :
  CPropertyPage(CGamePageWin::IDD)
{
  //{{AFX_DATA_INIT(CGamePageWin)
  //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CGamePageWin::OnOK() 
{
  // Perform default processing
  CPropertyPage::OnOK();
}

BOOL CGamePageWin::OnSetActive()
{
  // Perform default processing
  BOOL bResult = CPropertyPage::OnSetActive();

  // Return the result of the default processing
  return bResult;
}

void CGamePageWin::DoDataExchange(CDataExchange* pDX)
{
  // Perform default processing
  CPropertyPage::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CGamePageWin)
  //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CGamePageWin::OnInitDialog() 
{
  // Perform default processing
  CPropertyPage::OnInitDialog();

  // Return true to set the focus to the first tabstop control
  return true;
}


