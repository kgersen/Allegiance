// PropZoneLaunch.cpp : implementation file
//

#include "stdafx.h"
#include "cliconfig.h"
#include "PropZoneLaunch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropZoneLaunch property page

IMPLEMENT_DYNCREATE(CPropZoneLaunch, CPropertyPage)

CPropZoneLaunch::CPropZoneLaunch() : CPropertyPage(CPropZoneLaunch::IDD)
{
	//{{AFX_DATA_INIT(CPropZoneLaunch)
	m_strZoneCmdLine = _T("");
	m_strZoneCurrentDir = _T("");
	m_strZoneExeName = _T("");
	m_strZoneFilePath = _T("");
	m_strZoneGUID = _T("");
	m_strZoneVersion = _T("");
	//}}AFX_DATA_INIT
}

CPropZoneLaunch::~CPropZoneLaunch()
{
}

void CPropZoneLaunch::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropZoneLaunch)
	DDX_Text(pDX, IDC_ZONECMDLINE, m_strZoneCmdLine);
	DDX_Text(pDX, IDC_ZONECURRENTDIR, m_strZoneCurrentDir);
	DDX_Text(pDX, IDC_ZONEEXENAME, m_strZoneExeName);
	DDX_Text(pDX, IDC_ZONEFILEPATH, m_strZoneFilePath);
	DDX_Text(pDX, IDC_ZONEGUID, m_strZoneGUID);
	DDX_Text(pDX, IDC_ZONEVERSION, m_strZoneVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropZoneLaunch, CPropertyPage)
	//{{AFX_MSG_MAP(CPropZoneLaunch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropZoneLaunch message handlers

BOOL CPropZoneLaunch::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
    // in case the registry keys aren't present, set the values of the fields
    // with default values that can't be set during construction
    FILETIME ft;
    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;

    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    char  szValue[MAX_PATH];
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectPlay\\Applications\\Allegiance", 0, KEY_READ, &hKey))
    {
        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "CommandLine", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strZoneCmdLine = szValue;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "CurrentDirectory", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strZoneCurrentDir = szValue;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "File", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strZoneExeName = szValue;
        
        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Guid", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strZoneGUID = szValue;
        
        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Path", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strZoneFilePath = szValue;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Version", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strZoneVersion = szValue;

        RegCloseKey(hKey);
    }

    // get the path of the exe being used
    CString strAppPath = "";
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\allegiance.exe", 0, KEY_READ, &hKey))
    {
        char szValue[MAX_PATH];
        DWORD cbValue = sizeof(szValue);
        DWORD dwType;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Path", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            strAppPath = szValue;
            //strAppPath += "\\allegiance.exe";
    	RegCloseKey(hKey);
    }
    CString strAppPathTry = m_strZoneFilePath; // + "\\" + m_strZoneExeName;
    if (strAppPath.CompareNoCase(strAppPathTry)!=0)
        MessageBox("App Path and Zone Exe Filename are not in-sync.  Check values and press OK to correct the problem.", "Registry Problem Detected", MB_OK);

    UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropZoneLaunch::OnOK() 
{
    UpdateData(true);

    HKEY hKey;
    DWORD dw;

    // save the server setting
    if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\allegiance.exe", 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw))
    {
        CString str;
        str = m_strZoneFilePath; // + "\\" + m_strZoneExeName;
        ::RegSetValueEx(hKey, "Path", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength());
        RegCloseKey(hKey);
    }
    if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectPlay\\Applications\\Allegiance", 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw))
    {
        CString str;

        ::RegSetValueEx(hKey, "File", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneExeName, m_strZoneExeName.GetLength());
        ::RegSetValueEx(hKey, "CommandLine", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneCmdLine, m_strZoneCmdLine.GetLength());
        ::RegSetValueEx(hKey, "CurrentDirectory", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneCurrentDir, m_strZoneCurrentDir.GetLength());
        ::RegSetValueEx(hKey, "Path", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneFilePath, m_strZoneFilePath.GetLength());
        ::RegSetValueEx(hKey, "GUID", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneGUID, m_strZoneGUID.GetLength());
        ::RegSetValueEx(hKey, "Version", NULL, REG_SZ, (const unsigned char*)(const char*)m_strZoneVersion, m_strZoneVersion.GetLength());

        RegCloseKey(hKey);
    }
    
	CPropertyPage::OnOK();
}
