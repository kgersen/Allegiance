/////////////////////////////////////////////////////////////////////////////
// GamePageTeam.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "GamePageTeam.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGamePageTeam property page

IMPLEMENT_DYNAMIC(CGamePageTeam, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CGamePageTeam, CPropertyPage)
  //{{AFX_MSG_MAP(CGamePageTeam)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CGamePageTeam::CGamePageTeam() :
  CPropertyPage(CGamePageTeam::IDD)
{
  //{{AFX_DATA_INIT(CGamePageTeam)
	m_eTeamCount = -1;
	m_eMaxTeamInbalance = -1;
	m_eMinTeamPlayers = -1;
	m_eMaxTeamPlayers = -1;
	m_eSkillLevels = -1;
	m_eTeamLives = -1;
	m_eStartingMoney = -1;
	m_eTotalMoney = -1;
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CGamePageTeam::OnOK() 
{
  // Perform default processing
  CPropertyPage::OnOK();
}

BOOL CGamePageTeam::OnSetActive()
{
  // Perform default processing
  BOOL bResult = CPropertyPage::OnSetActive();

  // Return the result of the default processing
  return bResult;
}

void CGamePageTeam::DoDataExchange(CDataExchange* pDX)
{
  // Perform default processing
  CPropertyPage::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CGamePageTeam)
	DDX_CBIndex(pDX, IDC_TEAM_COUNT, m_eTeamCount);
	DDX_CBIndex(pDX, IDC_MAX_TEAM_INBALANCE, m_eMaxTeamInbalance);
	DDX_CBIndex(pDX, IDC_MIN_TEAM_PLAYERS, m_eMinTeamPlayers);
	DDX_CBIndex(pDX, IDC_MAX_TEAM_PLAYERS, m_eMaxTeamPlayers);
	DDX_CBIndex(pDX, IDC_SKILL_LEVEL, m_eSkillLevels);
	DDX_CBIndex(pDX, IDC_TEAM_LIVES, m_eTeamLives);
	DDX_CBIndex(pDX, IDC_STARTING_MONEY, m_eStartingMoney);
	DDX_CBIndex(pDX, IDC_TOTAL_MONEY, m_eTotalMoney);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CGamePageTeam::OnInitDialog() 
{
  // Perform default processing
  CPropertyPage::OnInitDialog();

  // Return true to set the focus to the first tabstop control
  return true;
}


