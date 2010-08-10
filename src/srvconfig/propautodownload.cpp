// PropAutoDownload.cpp : implementation file
//

#include "stdafx.h"
#include "srvconfig.h"
#include "PropAutoDownload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropAutoDownload property page

IMPLEMENT_DYNCREATE(CPropAutoDownload, CPropertyPage)

CPropAutoDownload::CPropAutoDownload() : CPropertyPage(CPropAutoDownload::IDD)
{
    //{{AFX_DATA_INIT(CPropAutoDownload)
    m_strFtpPassword = _T("");
    m_strFtpServer = _T("");
    m_strFtpUserID = _T("");
    m_bWatchArtDir = FALSE;
	m_strFtpRoot = _T("");
	//}}AFX_DATA_INIT
}

CPropAutoDownload::~CPropAutoDownload()
{
}

void CPropAutoDownload::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropAutoDownload)
    DDX_Text(pDX, IDC_FTPPASSWORD, m_strFtpPassword);
    DDX_Text(pDX, IDC_FTPSERVER, m_strFtpServer);
    DDX_Text(pDX, IDC_FTPUSERID, m_strFtpUserID);
    DDX_Check(pDX, IDC_WATCHARTDIR, m_bWatchArtDir);
	DDX_Text(pDX, IDC_FTPROOT, m_strFtpRoot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropAutoDownload, CPropertyPage)
    //{{AFX_MSG_MAP(CPropAutoDownload)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropAutoDownload message handlers

BOOL CPropAutoDownload::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();
    
    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    char  szValue[MAX_PATH];
    DWORD dwValue;
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, HKLM_AllLobby, 0, KEY_READ, &hKey))
    {
        dwValue=0;
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "AutoUpdateActive", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bWatchArtDir = (dwValue==1) ? TRUE : FALSE;
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "FTPArtServer", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strFtpServer = szValue;
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "FTPArtRoot", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strFtpRoot = szValue;
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "FTPAccount", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strFtpUserID = szValue;
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "FTPPW", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strFtpPassword = szValue;

        RegCloseKey(hKey);
    }

    UpdateData(false);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropAutoDownload::OnOK() 
{
    UpdateData(true);

    CDefaultRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_LOCAL_MACHINE, HKLM_AllLobby))
    {
        key.SetValue(m_bWatchArtDir ? 1 : 0, "AutoUpdateActive",    0   );
        key.SetValue(m_strFtpServer,         "FtpArtServer",        ""  );
        key.SetValue(m_strFtpRoot,           "FtpArtRoot",          ""  );
        key.SetValue(m_strFtpUserID,         "FtpAccount",          ""  );
        key.SetValue(m_strFtpPassword,       "FtpPW",               ""  );
    }

    CPropertyPage::OnOK();
}
