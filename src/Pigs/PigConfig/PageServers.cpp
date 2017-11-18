/////////////////////////////////////////////////////////////////////////////
// PageServers.cpp : Implementation of the CPageServers class.
//

#include "pch.h"
#include "PageServers.h"
#include "PigConfigSheet.h"


/////////////////////////////////////////////////////////////////////////////
// CPageServers property page


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CPageServers, CPropertyPage)
  //{{AFX_MSG_MAP(CPageServers)
  ON_BN_CLICKED(IDC_SAME_AS_LOBBY, OnSameAsLobby)
  ON_BN_CLICKED(IDC_MY_COMPUTER, OnMyComputer)
  ON_BN_CLICKED(IDC_THIS_COMPUTER, OnThisComputer)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CPageServers::CPageServers(CPigConfigSheet* pSheet) :
  CPropertyPage(CPageServers::IDD),
  m_pSheet(pSheet)
{
  //{{AFX_DATA_INIT(CPageServers)
  m_strLobbyServer = _T("");
  m_bZoneLobby = FALSE;
  m_strAccountServer = _T("");
  m_nAccountServer = -1;
  //}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CPageServers::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPageServers)
  DDX_Control(pDX, IDC_ACCOUNT_SERVER, m_editAccountServer);
  DDX_Text(pDX, IDC_LOBBYSERVER, m_strLobbyServer);
  DDX_Check(pDX, IDC_ZONELOBBY, m_bZoneLobby);
  DDX_Text(pDX, IDC_ACCOUNT_SERVER, m_strAccountServer);
  DDX_Radio(pDX, IDC_SAME_AS_LOBBY, m_nAccountServer);
  //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CPageServers::UpdateUI()
{
  // Update values from controls
  UpdateData(true);

  // Enable/disable the account server control
  bool bEnable;
  switch (m_nAccountServer)
  {
    case 0:
    case 1:
      bEnable = false;
      break;
    case 2:
      bEnable = true;
      break;
  }
  m_editAccountServer.EnableWindow(bEnable);
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CPageServers::OnInitDialog() 
{
  // Perform default processing
  CPropertyPage::OnInitDialog();

  // Get values from Pigs.Session object
  IPigSessionPtr spSession(GetSheet()->GetSession());
  CComBSTR bstr;
  // LobbyServer
  spSession->get_MissionServer(&bstr);
  m_strLobbyServer = bstr;
  // AccountServer
  spSession->get_AccountServer(&bstr);
  if (!bstr.Length())
    m_nAccountServer = 0;
  else if (1 == bstr.Length() && OLESTR('.') == bstr[0])
    m_nAccountServer = 1;
  else
    m_nAccountServer = 2;

  // Update controls from values
  UpdateData(false);

  // Update the UI to match the state of the current values
  UpdateUI();

  // Return true to set the focus to the first tabstop control
  return true;
}

void CPageServers::OnOK() 
{
  // Update values from controls
  UpdateData(true);

  // Save values to the Pigs.Session object
  IPigSessionPtr spSession(GetSheet()->GetSession());
  // LobbyServer
  spSession->put_MissionServer(CComBSTR(m_strLobbyServer));
  // AccountServer
  CComBSTR bstrAccountServer;
  switch (m_nAccountServer)
  {
    case 0:  bstrAccountServer.Empty();              break;
    case 1:  bstrAccountServer = OLESTR(".");        break;
    case 2:  bstrAccountServer = m_strAccountServer; break;
  }
  spSession->put_AccountServer(bstrAccountServer);

  // Perform default processing
  CPropertyPage::OnOK();
}


void CPageServers::OnSameAsLobby() 
{
  // TODO: Add your control notification handler code here
  
}

void CPageServers::OnMyComputer() 
{
  // TODO: Add your control notification handler code here
  
}

void CPageServers::OnThisComputer() 
{
  // TODO: Add your control notification handler code here
  
}
