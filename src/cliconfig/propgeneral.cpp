/////////////////////////////////////////////////////////////////////////////
// PropGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "cliconfig.h"
#include "cdate.h"
#include "PropGeneral.h"
#include "ShowConfigDlg.h"
#include "regkey.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropGeneral property page

IMPLEMENT_DYNCREATE(CPropGeneral, CPropertyPage)

CPropGeneral::CPropGeneral() : CPropertyPage(CPropGeneral::IDD)
{
    CString strValue;
    HKEY hKey;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectPlay\\Applications\\Allegiance", 0, KEY_READ, &hKey)
        && SUCCEEDED(LoadRegString(hKey, "CurrentDirectory", strValue)))
        strValue += "\\artwork";
    else
    {
        char szBuf[MAX_PATH];
        ::GetModuleFileName(NULL, szBuf, sizeof(szBuf));
        for (char* sz=szBuf+strlen(szBuf); (sz > szBuf) && (*sz != '\\'); sz--);
        *sz = '\0';
        strcat(szBuf, "\\artwork");
        strValue = szBuf;
    }

    //{{AFX_DATA_INIT(CPropGeneral)
    m_dateArt = CDate(99,01,01,00,00,00);
    m_strArtPath = strValue;
    m_bCaptureFrameData = FALSE;
    m_bCockpit = TRUE;
    m_bEnvironment = TRUE;
    m_strFrameFileName = _T("");
    m_bLogToFile = FALSE;
    m_bPosters = TRUE;
    m_bRoundRadar = FALSE;
    m_bSoftwareHUD = FALSE;
	m_bBiDiLighting = TRUE;
	m_bCenterHUD = TRUE;
	m_bChatHistoryHUD = TRUE;
	m_bDebris = TRUE;
	m_bLensFlare = TRUE;
	m_bLinearResponse = TRUE;
	m_bStars = TRUE;
	m_bStrobes = TRUE;
	m_bTargetHUD = TRUE;
	m_bTrails = TRUE;
	//}}AFX_DATA_INIT
//    m_strZoneLobby = _T("");
}

CPropGeneral::~CPropGeneral()
{
}

void CPropGeneral::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropGeneral)
	DDX_Control(pDX, IDC_CONFIG, m_comboConfig);
    DDX_CDate(pDX, IDC_ARTDATE, m_dateArt);
    DDX_Text(pDX, IDC_ARTPATH, m_strArtPath);
    DDX_Check(pDX, IDC_CAPTUREFRAMEDATA, m_bCaptureFrameData);
    DDX_Check(pDX, IDC_COCKPIT, m_bCockpit);
    DDX_Check(pDX, IDC_ENVIRONMENT, m_bEnvironment);
    DDX_Text(pDX, IDC_FRAMEFILENAME, m_strFrameFileName);
    DDX_Check(pDX, IDC_LOGTOFILE, m_bLogToFile);
    DDX_Check(pDX, IDC_POSTERS, m_bPosters);
    DDX_Check(pDX, IDC_ROUNDRADAR, m_bRoundRadar);
    DDX_Check(pDX, IDC_SOFTWAREHUD, m_bSoftwareHUD);
	DDX_Check(pDX, IDC_BIDILIGHTING, m_bBiDiLighting);
	DDX_Check(pDX, IDC_CENTERHUD, m_bCenterHUD);
	DDX_Check(pDX, IDC_CHATHISTORYHUD, m_bChatHistoryHUD);
	DDX_Check(pDX, IDC_DEBRIS, m_bDebris);
	DDX_Check(pDX, IDC_LENSFLARE, m_bLensFlare);
	DDX_Check(pDX, IDC_LINEARRESPONSE, m_bLinearResponse);
	DDX_Check(pDX, IDC_STARS, m_bStars);
	DDX_Check(pDX, IDC_STROBES, m_bStrobes);
	DDX_Check(pDX, IDC_TARGETHUD, m_bTargetHUD);
	DDX_Check(pDX, IDC_TRAILS, m_bTrails);
	//}}AFX_DATA_MAP
//    DDX_Text(pDX, IDC_ZONELOBBY, m_strZoneLobby);
}


BEGIN_MESSAGE_MAP(CPropGeneral, CPropertyPage)
    //{{AFX_MSG_MAP(CPropGeneral)
	ON_BN_CLICKED(IDC_VIEWBTN, OnViewbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


static int AddIfNotThere(CComboBox& combo, LPCTSTR pszString)
{
    int nIndex = combo.FindStringExact(-1, pszString);
    if (CB_ERR == nIndex)
        nIndex = combo.AddString(pszString);
    return nIndex;
}


/////////////////////////////////////////////////////////////////////////////
// CPropGeneral message handlers

BOOL CPropGeneral::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();

    // initialize the server MRU list
    for (int i=0;i<16;i++)
    {
        CString strLook, strFind;
        strLook.Format("MRUConfig%02d", i);
        strFind = AfxGetApp()->GetProfileString("MRUConfig", strLook, "");
        if (strFind!="")
            m_comboConfig.AddString(strFind);
    }

    // fill the server list with some server names if it doesn't have any yet
    AddIfNotThere(m_comboConfig, "http://allegiance/internal.cfg");
    AddIfNotThere(m_comboConfig, "http://allegiance/public.cfg");
    AddIfNotThere(m_comboConfig, "http://207.46.173.22/allegiance.cfg");

    // in case the registry keys aren't present, set the values of the fields
    // with default values that can't be set during construction
    FILETIME ft;
    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;

    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    DWORD dwValue;
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
    {
        CString strValue;
        if (SUCCEEDED(LoadRegString(hKey, "CfgFile", strValue)))
        {
            if (m_comboConfig.FindStringExact(1, strValue) == CB_ERR)
                m_comboConfig.AddString(strValue);
            m_comboConfig.SelectString(-1, strValue);
        }

//        if (SUCCEEDED(LoadRegString(hKey, "ZoneClubLobbyServer", strValue)))
//            m_strZoneLobby = strValue;

        if (SUCCEEDED(LoadRegString(hKey, "ArtPath", strValue)))
            m_strArtPath = strValue;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "UpdateLow", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            ft.dwLowDateTime = dwValue;
        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "UpdateHigh", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            ft.dwHighDateTime = dwValue;
        m_dateArt = CDate(ft);

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "LogFrameData", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bCaptureFrameData = (dwValue==1) ? 1 : 0;

        if (SUCCEEDED(LoadRegString(hKey, "LogFrameDataPath", strValue)))
            m_strFrameFileName = strValue;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Cockpit", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bCockpit = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Environment", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bEnvironment = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Posters", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bPosters = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "RoundRadar", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bRoundRadar = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "StyleHUD", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bSoftwareHUD = (dwValue==1) ? 1 : 0;

        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "LogToFile", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bLogToFile = (dwValue==1) ? 1 : 0;
        
        RegCloseKey(hKey);
    }

    UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropGeneral::OnOK() 
{
    UpdateData(true);

    HKEY hKey;
    DWORD dw;
    if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw))
    {
        DWORD dwValueOne = 1;
        DWORD dwValueZero = 0;

        // save the server setting
        CString strConfig;
        m_comboConfig.GetWindowText(strConfig);
        ::RegSetValueEx(hKey, "CfgFile", NULL, REG_SZ, (const unsigned char*)(const char*)strConfig, strConfig.GetLength());

        // save the configs in the MRU list--just don't want to add one twice
        if (m_comboConfig.FindStringExact(1, strConfig)==-1)
            m_comboConfig.AddString(strConfig);
        for (int i=0;i<16 && i<m_comboConfig.GetCount();i++)
        {
            CString strEntry;
            strEntry.Format("MRUConfig%02d", i);
            m_comboConfig.GetLBText(i, strConfig);
            ::RegSetValueEx(hKey, strEntry, NULL, REG_SZ, (const unsigned char*)(const char*)strConfig, strConfig.GetLength());
        }

//        ::RegSetValueEx(hKey, "ZoneClubLobbyServer", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneLobby, m_strZoneLobby.GetLength());
        ::RegSetValueEx(hKey, "ArtPath", NULL, REG_SZ, (const unsigned char*)(const char*)m_strArtPath, m_strArtPath.GetLength());

        FILETIME ft;
        m_dateArt.GetFiletime(&ft);
        ::RegSetValueEx(hKey, "UpdateLow", NULL, REG_DWORD, (const unsigned char*)&ft.dwLowDateTime, sizeof(ft.dwLowDateTime));
        ::RegSetValueEx(hKey, "UpdateHigh", NULL, REG_DWORD, (const unsigned char*)&ft.dwHighDateTime, sizeof(ft.dwHighDateTime));

        ::RegSetValueEx(hKey, "LogFrameData", NULL, REG_DWORD, 
            (const unsigned char*)(m_bCaptureFrameData ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "LogFrameDataPath", NULL, REG_SZ, (const unsigned char*)(const char*)m_strFrameFileName, m_strFrameFileName.GetLength());

        ::RegSetValueEx(hKey, "Cockpit", NULL, REG_DWORD, 
                (const unsigned char*)(m_bCockpit ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "Environment", NULL, REG_DWORD, 
                (const unsigned char*)(m_bEnvironment ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "Posters", NULL, REG_DWORD, 
                (const unsigned char*)(m_bPosters ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "RoundRadar", NULL, REG_DWORD, 
                (const unsigned char*)(m_bRoundRadar ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "StyleHUD", NULL, REG_DWORD, 
                (const unsigned char*)(m_bSoftwareHUD ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegSetValueEx(hKey, "LogToFile", NULL, REG_DWORD, 
                (const unsigned char*)(m_bLogToFile ? &dwValueOne : &dwValueZero), sizeof(DWORD));

        ::RegCloseKey(hKey);
    }

    CPropertyPage::OnOK();
}

void CPropGeneral::OnViewbtn() 
{
    CString strURL;
    GetDlgItemText(IDC_CONFIG, strURL);
    // Ensure that we have an absolute URL
    USES_CONVERSION;
    if (S_FALSE == IsValidURL(NULL, T2CW(strURL), 0))
    {
        MessageBox("The value specified is not a valid URL.", "Unable to Display", MB_OK);
        return;
    }

    // Open the specified script file
    IStream* spstm;
    HRESULT hr = URLOpenBlockingStream(NULL, strURL, &spstm, 0, NULL);        
    if (FAILED(hr))          
    {
        MessageBox("Unable to open the specified config file, or it doesn't exist.", "Unable to Display", MB_OK);
        return;
    }

    // Get the size of the stream
    const LARGE_INTEGER nPosZero = {0, 0};
    ULARGE_INTEGER cbFileLarge;
    spstm->Seek(nPosZero, STREAM_SEEK_END, &cbFileLarge);
    //Assert(0 == cbFileLarge.HighPart);
    DWORD cbFile = cbFileLarge.LowPart;
    spstm->Seek(nPosZero, STREAM_SEEK_SET, NULL);

    CShowConfigDlg dlg;
    LPTSTR szBuffer = dlg.m_strContents.GetBufferSetLength(cbFile);

    hr = spstm->Read(szBuffer, cbFile, NULL);

    dlg.DoModal();
}
