/////////////////////////////////////////////////////////////////////////////
// PageGeneral.cpp : Implementation of the CPageGeneral class.
//

#include "pch.h"
#include "PageGeneral.h"
#include "PigConfigSheet.h"


/////////////////////////////////////////////////////////////////////////////
// CPageGeneral property page


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CPageGeneral, CPropertyPage)
  //{{AFX_MSG_MAP(CPageGeneral)
	ON_BN_CLICKED(IDC_LIMIT_PIGS, OnLimitPigs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CPageGeneral::CPageGeneral(CPigConfigSheet* pSheet) :
  CPropertyPage(CPageGeneral::IDD),
  m_pSheet(pSheet)
{
  //{{AFX_DATA_INIT(CPageGeneral)
  m_strArtPath = _T("");
  m_strPigletDir = _T("");
  m_bLimitPigs = FALSE;
  m_strVersion = _T("");
  m_strFlavor = _T("");
  m_cMaxPigs = 0;
  //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CPageGeneral::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPageGeneral)
  DDX_Control(pDX, IDC_MAX_PIGS_SPIN, m_spinMaxPigs);
  DDX_Control(pDX, IDC_MAX_PIGS, m_editMaxPigs);
  DDX_Control(pDX, IDC_MAX_PIGS_CAPTION, m_staticMaxPigs);
  DDX_Text(pDX, IDC_ART_PATH, m_strArtPath);
  DDX_Text(pDX, IDC_PIGLET_DIR, m_strPigletDir);
  DDX_Check(pDX, IDC_LIMIT_PIGS, m_bLimitPigs);
  DDX_Text(pDX, IDC_PIG_SRV_VERSION, m_strVersion);
  DDX_Text(pDX, IDC_PIG_SRV_FLAVOR, m_strFlavor);
  DDX_Text(pDX, IDC_MAX_PIGS, m_cMaxPigs);
  //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CPageGeneral::UpdateUI()
{
  // Update values from controls
  UpdateData(true);

  // Enable/disable the MaxPigs controls
  m_staticMaxPigs.EnableWindow(m_bLimitPigs);
    m_editMaxPigs.EnableWindow(m_bLimitPigs);
    m_spinMaxPigs.EnableWindow(m_bLimitPigs);
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CPageGeneral::OnInitDialog() 
{
  // Perform default processing
  CPropertyPage::OnInitDialog();

  // Initialize the spinner control
  m_spinMaxPigs.SetRange(0, 200);

  // Get values from Pigs.Session object
  IPigSessionPtr spSession(GetSheet()->GetSession());
  CComBSTR bstr;
  // ArtPath
  spSession->get_ArtPath(&bstr);
  m_strArtPath = bstr;
  // ScriptDir
  spSession->get_ScriptDir(&bstr);
  m_strPigletDir = bstr;
  // MaxPigs
  spSession->get_MaxPigs(&m_cMaxPigs);
  // Version
  IAGCVersionInfoPtr spVersion;
  spSession->get_Version(&spVersion);
  // Version.ProductVersionString
  spVersion->get_ProductVersionString(&bstr);
  m_strVersion = bstr;
  // Version.Value("BuildFlavor")
  spVersion->get_Value(CComBSTR(L"BuildFlavor"), &bstr);
  m_strFlavor = bstr;

  // Initialize the LimitPigs flag by comparing to -1
  m_bLimitPigs = (-1 != m_cMaxPigs);

  // Update controls from values
  UpdateData(false);

  // Update the UI to match the state of the current values
  UpdateUI();

  // Return true to set the focus to the first tabstop control
  return true;
}

void CPageGeneral::OnOK() 
{
  // Update values from controls
  UpdateData(true);

  // Save values to the Pigs.Session object
  IPigSessionPtr spSession(GetSheet()->GetSession());
  // ArtPath
  spSession->put_ArtPath(CComBSTR(m_strArtPath));
  // ScriptDir
  spSession->put_ScriptDir(CComBSTR(m_strPigletDir));
  // MaxPigs
  spSession->put_MaxPigs(m_bLimitPigs ? m_cMaxPigs : -1);

  // Perform default processing
  CPropertyPage::OnOK();
}

void CPageGeneral::OnLimitPigs() 
{
  UpdateUI();
}

