/////////////////////////////////////////////////////////////////////////////
// DlgAbout.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "DlgAbout.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CDlgAbout, CDialog)
  //{{AFX_MSG_MAP(CDlgAbout)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CDlgAbout::CDlgAbout(CWnd* pParent)
  : CDialog(CDlgAbout::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgAbout)
  m_strDescription = _T("");
  m_strVersion = _T("");
  m_strProductID = _T("");
  m_strCopyright = _T("");
  m_strLegalWarning = _T("");
  //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
  // Perform default processing
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CDlgAbout)
  DDX_Text(pDX, IDC_DESCRIPTION, m_strDescription);
  DDX_Text(pDX, IDC_VERSION, m_strVersion);
  DDX_Text(pDX, IDC_PRODUCT_ID, m_strProductID);
  DDX_Text(pDX, IDC_COPYRIGHT, m_strCopyright);
  DDX_Text(pDX, IDC_LEGAL_WARNING, m_strLegalWarning);
  //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CDlgAbout::OnInitDialog() 
{
  // Perform default processing
  CDialog::OnInitDialog();

  // Read the description, version and copyright from VersionInfo resource
  ZVersionInfo vi;
  m_strDescription = vi.GetFileDescription();
  m_strVersion     = vi.GetFileVersionString();
  m_strCopyright   = vi.GetLegalCopyright();

  // Read the Product ID from the registry
  CRegKey key;
  if (ERROR_SUCCESS == key.Open(HKEY_LOCAL_MACHINE, HKLM_AllSrvUI, KEY_READ))
    LoadRegString(key, TEXT("PID"), m_strProductID);
  if (m_strProductID.IsEmpty())
    m_strProductID.LoadString(IDS_PRODUCT_ID_NONE);

  // Read the Legal Warning from a string resource
  m_strLegalWarning.LoadString(IDS_LEGAL_WARNING);

  // Update fields from data members
  UpdateData(false);

  // Return true to set the focus to the first tabstop control
  return true;
}

