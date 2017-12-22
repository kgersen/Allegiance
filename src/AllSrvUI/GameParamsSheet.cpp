/////////////////////////////////////////////////////////////////////////////
// CGameParamsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "GameParamsSheet.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGameParamsSheet dialog


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CGameParamsSheet, CPropertySheet)
    //{{AFX_MSG_MAP(CGameParamsSheet)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CGameParamsSheet::CGameParamsSheet(CWnd* pParentWnd, UINT iSelectPage)
  : CPropertySheet(IDS_GAME_PARAMS, pParentWnd, iSelectPage)
{
  // Add our property pages
  AddPage(&m_PageWin);
  AddPage(&m_PageTeam);
  AddPage(&m_PageSpecifics);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CGameParamsSheet::InitButtons()
{
  // Calculate the height of 7 dialog units
  CRect rectUnits(0, 0, 0, 7);

  //mmf vc8 does not like two arguments, below was commented out, I uncommented it - Imago made them co-exist
#if _MSC_VER >= 1400
  MapDialogRect(rectUnits);
#else
  MapDialogRect(GetSafeHwnd(), rectUnits); 
#endif


  // Get the OK and cancel buttons
  CWnd* pwndOK     = GetDlgItem(IDOK);
  CWnd* pwndCancel = GetDlgItem(IDCANCEL);
  CWnd* pwndHelp   = GetDlgItem(IDHELP);

  // Get the window rectangles of the buttons
  CRect rectOK, rectCancel, rectHelp;
  pwndOK->GetWindowRect(rectOK);
  pwndCancel->GetWindowRect(rectCancel);
  pwndHelp->GetWindowRect(rectHelp);
  ScreenToClient(rectOK);
  ScreenToClient(rectCancel);
  ScreenToClient(rectHelp);

  // Compute the offset to position these buttons flush-right
  int cxOffset = rectHelp.right - rectCancel.right;

  // Move the buttons into place
  rectOK.OffsetRect(cxOffset, 0);
  rectCancel.OffsetRect(cxOffset, 0);
  pwndOK->MoveWindow(rectOK);
  pwndCancel->MoveWindow(rectCancel);

  // Hide the Apply and Help buttons
  GetDlgItem(ID_APPLY_NOW)->ShowWindow(SW_HIDE);
  GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);

  // Show the OK and Cancel buttons
  GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
  GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);

  // Enable the OK and Cancel buttons
  GetDlgItem(IDOK)->EnableWindow(true);
  GetDlgItem(IDCANCEL)->EnableWindow(true);
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CGameParamsSheet::OnInitDialog()
{
  // Perform default processing
  CPropertySheet::OnInitDialog();

  // Initialize the OK and Cancel buttons
  InitButtons();

  // Return true to set the focus to the first tabstop control
  return true;
}

