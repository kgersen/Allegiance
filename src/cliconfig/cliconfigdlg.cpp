// CliConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CliConfig.h"
#include "cdate.h"
#include "CliConfigDlg.h"
#include "regkey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCliConfigDlg dialog

CCliConfigDlg::CCliConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCliConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCliConfigDlg)
	m_strArtPath = _T("");
	m_bCaptureFrameData = FALSE;
	m_strFrameDataFilename = _T("");
	m_bLogToFile = FALSE;
	m_dateArt = CDate();
	m_bCockpit = FALSE;
	m_bEnvironment = FALSE;
	m_bMusic = FALSE;
	m_bPosters = FALSE;
	m_bRoundRadar = FALSE;
	m_bSoftwareHUD = FALSE;
	m_strZoneCommandLine = _T("");
	m_strZoneCurrentDir = _T("");
	m_strZoneExeName = _T("");
	m_strZoneFilePath = _T("");
	m_strZoneGUID = _T("");
	m_strZoneVersion = _T("");
	m_strZoneLobby = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCliConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCliConfigDlg)
	DDX_Control(pDX, IDC_SERVER, m_comboServer);
	DDX_Text(pDX, IDC_ARTPATH, m_strArtPath);
	DDX_Check(pDX, IDC_CAPTUREFRAMEDATA, m_bCaptureFrameData);
	DDX_Text(pDX, IDC_FRAMEFILENAME, m_strFrameDataFilename);
	DDX_Check(pDX, IDC_LOGTOFILE, m_bLogToFile);
	DDX_CDate(pDX, IDC_ARTDATE, m_dateArt);
	DDX_Check(pDX, IDC_COCKPIT, m_bCockpit);
	DDX_Check(pDX, IDC_ENVIRONMENT, m_bEnvironment);
	DDX_Check(pDX, IDC_MUSIC, m_bMusic);
	DDX_Check(pDX, IDC_POSTERS, m_bPosters);
	DDX_Check(pDX, IDC_ROUNDRADAR, m_bRoundRadar);
	DDX_Check(pDX, IDC_SOFTWAREHUD, m_bSoftwareHUD);
	DDX_Text(pDX, IDC_ZONECMDLINE, m_strZoneCommandLine);
	DDX_Text(pDX, IDC_ZONECURRENTDIR, m_strZoneCurrentDir);
	DDX_Text(pDX, IDC_ZONEEXENAME, m_strZoneExeName);
	DDX_Text(pDX, IDC_ZONEFILEPATH, m_strZoneFilePath);
	DDX_Text(pDX, IDC_ZONEGUID, m_strZoneGUID);
	DDX_Text(pDX, IDC_ZONEVERSION, m_strZoneVersion);
	DDX_Text(pDX, IDC_ZONELOBBY, m_strZoneLobby);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCliConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CCliConfigDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CAPTUREFRAMEDATA, OnCaptureframedata)
	ON_BN_CLICKED(IDC_DEBUGBUILD, OnDebugbuild)
	ON_BN_CLICKED(IDC_EXEFILENAMEBUTTON, OnExefilenamebutton)
	ON_BN_CLICKED(IDC_ARTPATHBUTTON, OnArtpathbutton)
	ON_BN_CLICKED(IDC_RETAILBUILD, OnRetailbuild)
	ON_BN_CLICKED(IDC_OTHERBUILD, OnOtherbuild)
	ON_BN_CLICKED(IDC_FRAMEFILEBUTTON, OnFramefilebutton)
	ON_BN_CLICKED(IDC_TESTBUILD, OnTestbuild)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCliConfigDlg message handlers

BOOL CCliConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // initialize the server MRU list
    for (int i=0;i<16;i++)
    {
        CString strLook, strFind;
        strLook.Format("MRUServer%02d", i);
        strFind = AfxGetApp()->GetProfileString("MRUServer", strLook, "");
        if (strFind!="")
            m_comboServer.AddString(strFind);
    }

    // fill the server list with some server names if it doesn't have any yet
    if (m_comboServer.GetCount()==0)
    {
        m_comboServer.AddString("oblivion0");
        m_comboServer.AddString("oblivion1");
        m_comboServer.AddString("oblivion2");
        m_comboServer.AddString("oblivion3");
        m_comboServer.AddString("oblivion4");
        m_comboServer.AddString("oblivion5");
    }

    // in case the registry keys aren't present, set the values of the fields
    // with default values that can't be set during construction
    FILETIME ft;
    ft.dwLowDateTime = 0;
    ft.dwHighDateTime = 0;

    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    char  szValue[MAX_PATH];
    DWORD dwValue;
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
    {
        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "InternetLobbyServer", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strServer = szValue;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "ZoneClubLobbyServer", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strZoneLobby = szValue;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "ArtPath", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strArtPath = szValue;

        cbValue = sizeof(dwValue); dwValue = 0;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "UpdateLow", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            ft.dwLowDateTime = dwValue;
        cbValue = sizeof(dwValue); dwValue = 0;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "UpdateHigh", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            ft.dwHighDateTime = dwValue;
            //ATL CTime will assert if we go with default -Imago
            if (ft.dwHighDateTime && ft.dwLowDateTime)
        		m_dateArt = CDate(ft);

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "LogFrameData", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bCaptureFrameData = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "LogFrameDataPath", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strFrameDataFilename = szValue;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Cockpit", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bCockpit = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Environment", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bEnvironment = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Music", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bMusic = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "Posters", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bPosters = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "RoundRadar", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bRoundRadar = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "StyleHUD", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bSoftwareHUD = (dwValue==1) ? 1 : 0;

        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "LogToFile", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_bLogToFile = (dwValue==1) ? 1 : 0;

        RegCloseKey(hKey);
    }
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\DirectPlay\\Applications\\Allegiance", 0, KEY_READ, &hKey))
    {
        cbValue = MAX_PATH; szValue[0] = '\0';
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "CommandLine", NULL, &dwType, (unsigned char*)&szValue, &cbValue))
            m_strZoneCommandLine = szValue;

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

    // select the correct server from the MRU list
    if (m_strServer.GetLength()>0)
    {
        if (m_comboServer.FindStringExact(1, m_strServer)==-1)
            m_comboServer.AddString(m_strServer);
        m_comboServer.SelectString(-1, m_strServer);
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
            strAppPath += "\\allegiance.exe";
    	RegCloseKey(hKey);
    }
    CString strAppPathTry = m_strZoneFilePath + "\\" + m_strZoneExeName;
    if (strAppPath.CompareNoCase(strAppPathTry)!=0)
        MessageBox("App Path and Zone Exe Filename are not in-sync.  Check values and press OK to correct the problem.", "Registry Problem Detected", MB_OK);

    // now that all the fields have the right values in them, put the values
    // in the controls on the dialog

	SetDlgItemText(IDC_SERVER, m_strServer);
	SetDlgItemText(IDC_ZONELOBBY, m_strZoneLobby);
	SetDlgItemText(IDC_ARTPATH, m_strArtPath);
    CString str;
    m_dateArt.GetParseDate(str);
    SetDlgItemText(IDC_ARTDATE, str);
	CheckDlgButton(IDC_CAPTUREFRAMEDATA, m_bCaptureFrameData);
	SetDlgItemText(IDC_FRAMEFILENAME, m_strFrameDataFilename);
    CheckDlgButton(IDC_LOGTOFILE, m_bLogToFile);
    CheckDlgButton(IDC_COCKPIT, m_bCockpit);
	CheckDlgButton(IDC_ENVIRONMENT, m_bEnvironment);
	CheckDlgButton(IDC_MUSIC, m_bMusic);
	CheckDlgButton(IDC_POSTERS, m_bPosters);
	CheckDlgButton(IDC_ROUNDRADAR, m_bRoundRadar);
	CheckDlgButton(IDC_SOFTWAREHUD, m_bSoftwareHUD);
	SetDlgItemText(IDC_ZONECMDLINE, m_strZoneCommandLine);
	SetDlgItemText(IDC_ZONECURRENTDIR, m_strZoneCurrentDir);
	SetDlgItemText(IDC_ZONEEXENAME, m_strZoneExeName);
	SetDlgItemText(IDC_ZONEFILEPATH, m_strZoneFilePath);
	SetDlgItemText(IDC_ZONEGUID, m_strZoneGUID);
	SetDlgItemText(IDC_ZONEVERSION, m_strZoneVersion);

    // set the enable state of the controls based on their new values
    GetDlgItem(IDC_FRAMEFILENAME)->EnableWindow(m_bCaptureFrameData);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCliConfigDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCliConfigDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCliConfigDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCliConfigDlg::OnOK()
{
    HKEY hKey;
    DWORD dw;
    if (ERROR_SUCCESS == ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dw))
    {
        CString str;
        DWORD dwValueOne = 1;
        DWORD dwValueZero = 0;

        // save the server setting
        m_comboServer.GetWindowText(str);
        if (str.CompareNoCase(m_strServer)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "InternetLobbyServer", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strServer = str;

        // save the servers in the MRU list--just don't want to add one twice
        if (m_comboServer.FindStringExact(1, m_strServer)==-1)
            m_comboServer.AddString(m_strServer);
        for (int i=0;i<16 && i<m_comboServer.GetCount();i++)
        {
            CString strEntry;
            strEntry.Format("MRUServer%02d", i);
            m_comboServer.GetLBText(i, m_strServer);
            ::RegSetValueEx(hKey, strEntry, NULL, REG_SZ, (const unsigned char*)(const char*)m_strServer, m_strServer.GetLength());
        }

        GetDlgItemText(IDC_ZONELOBBY, str);
        if (str.CompareNoCase(m_strZoneLobby)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "ZoneClubLobbyServer", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneLobby = str;

        GetDlgItemText(IDC_ARTPATH, str);
        if (str.CompareNoCase(m_strArtPath)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "ArtPath", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strArtPath = str;

        GetDlgItemText(IDC_ARTDATE, str);
        CDate dateNew;
        dateNew.SetDate(str);
        if (m_dateArt!=dateNew)
        {
            FILETIME ft;
            dateNew.GetFiletime(&ft);
            ::RegSetValueEx(hKey, "UpdateLow", NULL, REG_DWORD, (const unsigned char*)&ft.dwLowDateTime, sizeof(ft.dwLowDateTime));
            ::RegSetValueEx(hKey, "UpdateHigh", NULL, REG_DWORD, (const unsigned char*)&ft.dwHighDateTime, sizeof(ft.dwHighDateTime));
        }

        if (m_bCaptureFrameData!=(BOOL)IsDlgButtonChecked(IDC_CAPTUREFRAMEDATA))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "LogFrameData", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_CAPTUREFRAMEDATA) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bCaptureFrameData = IsDlgButtonChecked(IDC_CAPTUREFRAMEDATA);

        GetDlgItemText(IDC_FRAMEFILENAME, str);
        if (str.CompareNoCase(m_strFrameDataFilename)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "LogFrameDataPath", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strFrameDataFilename = str;

        if (m_bCockpit!=(BOOL)IsDlgButtonChecked(IDC_COCKPIT))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "Cockpit", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_COCKPIT) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bCockpit = IsDlgButtonChecked(IDC_COCKPIT);

        if (m_bEnvironment!=(BOOL)IsDlgButtonChecked(IDC_ENVIRONMENT))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "Environment", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_ENVIRONMENT) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bEnvironment = IsDlgButtonChecked(IDC_ENVIRONMENT);

        if (m_bMusic!=(BOOL)IsDlgButtonChecked(IDC_MUSIC))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "Music", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_MUSIC) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bMusic = IsDlgButtonChecked(IDC_MUSIC);

        if (m_bPosters!=(BOOL)IsDlgButtonChecked(IDC_POSTERS))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "Posters", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_POSTERS) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bPosters = IsDlgButtonChecked(IDC_POSTERS);

        if (m_bRoundRadar!=(BOOL)IsDlgButtonChecked(IDC_ROUNDRADAR))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "RoundRadar", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_ROUNDRADAR) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bRoundRadar = IsDlgButtonChecked(IDC_ROUNDRADAR);

        if (m_bSoftwareHUD!=(BOOL)IsDlgButtonChecked(IDC_SOFTWAREHUD))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "StyleHUD", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_SOFTWAREHUD) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bSoftwareHUD = IsDlgButtonChecked(IDC_SOFTWAREHUD);

        if (m_bLogToFile!=(BOOL)IsDlgButtonChecked(IDC_LOGTOFILE))
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "LogToFile", NULL, REG_DWORD,
                (const unsigned char*)(IsDlgButtonChecked(IDC_LOGTOFILE) ? &dwValueOne : &dwValueZero), sizeof(DWORD)))
                m_bLogToFile = IsDlgButtonChecked(IDC_LOGTOFILE);

        ::RegCloseKey(hKey);
    }

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

        GetDlgItemText(IDC_ZONEEXENAME, str);
        if (str.CompareNoCase(m_strZoneExeName)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "File", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneExeName = str;

        GetDlgItemText(IDC_ZONECMDLINE, str);
        if (str.CompareNoCase(m_strZoneCommandLine)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "CommandLine", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneCommandLine = str;

        GetDlgItemText(IDC_ZONECURRENTDIR, str);
        if (str.CompareNoCase(m_strZoneCurrentDir)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "CurrentDirectory", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneCurrentDir = str;

        GetDlgItemText(IDC_ZONEFILEPATH, str);
        if (str.CompareNoCase(m_strZoneFilePath)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "Path", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneFilePath = str;

        GetDlgItemText(IDC_ZONEGUID, str);
        if (str.CompareNoCase(m_strZoneGUID)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "GUID", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneGUID = str;

        GetDlgItemText(IDC_ZONEVERSION, str);
        if (str.CompareNoCase(m_strZoneVersion)!=0)
            if (ERROR_SUCCESS == ::RegSetValueEx(hKey, "Version", NULL, REG_SZ, (const unsigned char*)(const char*)str, str.GetLength()))
                m_strZoneVersion = str;

        RegCloseKey(hKey);
    }

    // we don't want the window to exit, since this is really an apply button
	CDialog::OnOK();
}

void CCliConfigDlg::OnCaptureframedata()
{
    GetDlgItem(IDC_FRAMEFILENAME)->EnableWindow(IsDlgButtonChecked(IDC_CAPTUREFRAMEDATA));
}

void CCliConfigDlg::OnRetailbuild()
{
    SetDlgItemText(IDC_EXEFILENAME, m_strRetailFilename);
    //GetDlgItem(IDC_EXEFILENAME)->EnableWindow(FALSE);
}

void CCliConfigDlg::OnTestbuild()
{
    SetDlgItemText(IDC_EXEFILENAME, m_strTestFilename);
    //GetDlgItem(IDC_EXEFILENAME)->EnableWindow(FALSE);
}

void CCliConfigDlg::OnDebugbuild()
{
    SetDlgItemText(IDC_EXEFILENAME, m_strDebugFilename);
    //GetDlgItem(IDC_EXEFILENAME)->EnableWindow(FALSE);
}

void CCliConfigDlg::OnOtherbuild()
{
    SetDlgItemText(IDC_EXEFILENAME, m_strOtherFilename);
    //GetDlgItem(IDC_EXEFILENAME)->EnableWindow(TRUE);
}

void CCliConfigDlg::OnExefilenamebutton()
{
    //OPENFILENAME ofn;
    //GetOpenFileName(&ofn);
}

void CCliConfigDlg::OnArtpathbutton()
{
	// TODO: Add your control notification handler code here

}

void CCliConfigDlg::OnFramefilebutton()
{
	// TODO: Add your control notification handler code here

}

