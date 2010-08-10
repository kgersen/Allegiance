/////////////////////////////////////////////////////////////////////////////
// PropGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "srvconfig.h"
#include "PropGeneral.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropGeneral property page

IMPLEMENT_DYNCREATE(CPropGeneral, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CPropGeneral, CPropertyPage)
    //{{AFX_MSG_MAP(CPropGeneral)
    ON_BN_CLICKED(IDC_UPDATES_PER_SECOND_DEFAULT, OnUpdatesPerSecondDefault)
    ON_EN_CHANGE(IDC_UPDATESPERSECOND, OnChangeUpdatesPerSecond)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPropGeneral::CPropGeneral() :
  CPropertyPage(CPropGeneral::IDD),
    m_idDefaultButton(0)
{
    //{{AFX_DATA_INIT(CPropGeneral)
    m_strFileArtDir = _T("");
    m_strLobbyServer = _T("");
    m_bTimeout = FALSE;
    m_bWantInt3 = FALSE;
    m_nUpdatesPerSecond = c_nUpdatesPerSecondDefault;
	m_bProtocol = FALSE;
	//}}AFX_DATA_INIT
}

CPropGeneral::~CPropGeneral()
{
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CPropGeneral::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropGeneral)
    DDX_Control(pDX, IDC_UPDATES_PER_SECOND_DEFAULT, m_btnUpdatesPerSecondDefault);
    DDX_Control(pDX, IDC_UPDATES_PER_SECOND_SPIN, m_spinUpdatesPerSecond);
    DDX_Text(pDX, IDC_FILEARTDIR, m_strFileArtDir);
    DDX_Text(pDX, IDC_LOBBYSERVER, m_strLobbyServer);
    DDX_Check(pDX, IDC_TIMEOUT, m_bTimeout);
    DDX_Check(pDX, IDC_WANTINT3, m_bWantInt3);
    DDX_Text(pDX, IDC_UPDATESPERSECOND, m_nUpdatesPerSecond);
    DDV_MinMaxInt(pDX, m_nUpdatesPerSecond, c_nUpdatesPerSecondMin, c_nUpdatesPerSecondMax);
	DDX_Check(pDX, IDC_PROTOCOL, m_bProtocol);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CPropGeneral::OnInitDialog() 
{
    // Perform default processing
    CPropertyPage::OnInitDialog();

    // Initialize the spin control
    m_spinUpdatesPerSecond.SetRange(c_nUpdatesPerSecondMin,
        c_nUpdatesPerSecondMax);

    // Save the ID of the current default pushbutton
    m_idDefaultButton = LOWORD(GetDefID());

    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    char  szValue[MAX_PATH];
    DWORD dwValue;
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, HKLM_FedSrv, 0, KEY_READ, &hKey))
    {
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "LobbyServer", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strLobbyServer = szValue;
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "ArtPath", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strFileArtDir = szValue;
        dwValue=0;
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "fWantInt3", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bWantInt3 = (dwValue==1) ? TRUE : FALSE;
        else
            m_bWantInt3 = TRUE;
        dwValue=0;
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "fTimeout", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bTimeout = (dwValue==1) ? TRUE : FALSE;
        else
            m_bTimeout = TRUE;
        dwValue=0;
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "nUpdatesPerSecond", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nUpdatesPerSecond = dwValue;
        else
            m_nUpdatesPerSecond = 10;
        dwValue=0;
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "fProtocol", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bProtocol = (dwValue==1) ? TRUE : FALSE;
        else
            m_bProtocol = TRUE;

        RegCloseKey(hKey);
    }

    // Update controls from values
    UpdateData(false);

    // Initialize button state
    OnChangeUpdatesPerSecond();

    // Return true to set the focus to the first tabstop control
    return true;
}

void CPropGeneral::OnOK() 
{
    // Update values from controls
    UpdateData(true);

    CDefaultRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_LOCAL_MACHINE, HKLM_FedSrv))
    {
#if !defined(ALLSRV_STANDALONE)
        key.SetValue(m_strLobbyServer,      "LobbyServer",       "" );
#endif
        key.SetValue(m_strFileArtDir,       "ArtPath",           "" );
        key.SetValue(m_bWantInt3 ? 1 : 0,   "fWantInt3",         1  );
        key.SetValue(m_bTimeout ? 1 : 0,    "fTimeout",          1  );
        key.SetValue(m_bProtocol ? 1 : 0,   "fProtocol",         1  );
        key.SetValue(m_nUpdatesPerSecond,   "nUpdatesPerSecond", 10 );
    }

    CPropertyPage::OnOK();
}

void CPropGeneral::OnUpdatesPerSecondDefault() 
{
    // Set to the default value
    m_nUpdatesPerSecond = c_nUpdatesPerSecondDefault;

    // Update controls from values
    UpdateData(false);

    // Initialize button state
    OnChangeUpdatesPerSecond();
}

void CPropGeneral::OnChangeUpdatesPerSecond() 
{
    // Update values from controls
    if (m_btnUpdatesPerSecondDefault.GetSafeHwnd())
    {
        UpdateData(true);

        // Enable/disable the Restore Default button
        bool bEnable = c_nUpdatesPerSecondDefault != m_nUpdatesPerSecond;
        if (!bEnable && GetFocus() == &m_btnUpdatesPerSecondDefault)
        {
            GetDlgItem(IDC_UPDATESPERSECOND)->SetFocus();
            SetDefID(m_idDefaultButton);
        }
        m_btnUpdatesPerSecondDefault.EnableWindow(bEnable);
    }
}
