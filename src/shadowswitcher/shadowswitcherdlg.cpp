// ShadowSwitcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowSwitcher.h"
#include "ShadowSwitcherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
void assert(bool b) 
{
    if (!b) {
        DebugBreak();
    }
}

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#else
void assert(bool b) {}
#endif

/////////////////////////////////////////////////////////////////////////////
// CShadowSwitcherDlg dialog

CShadowSwitcherDlg::CShadowSwitcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShadowSwitcherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShadowSwitcherDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CShadowSwitcherDlg::~CShadowSwitcherDlg()
{
    ::RegCloseKey(m_hKey);
}

void CShadowSwitcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShadowSwitcherDlg)
	DDX_Control(pDX, IDC_Shadow, m_btnShadow);
	DDX_Control(pDX, IDC_Retail, m_btnRetail);
	DDX_Control(pDX, IDC_Delete, m_btnDelete);
	DDX_Control(pDX, IDC_Text, m_text);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShadowSwitcherDlg, CDialog)
	//{{AFX_MSG_MAP(CShadowSwitcherDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Delete, OnDelete)
	ON_BN_CLICKED(IDC_Retail, OnRetail)
	ON_BN_CLICKED(IDC_Shadow, OnShadow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadowSwitcherDlg message handlers

BOOL CShadowSwitcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    //
    // Get the reg key
    //

    HRESULT hr = 
        ::RegCreateKeyEx(
            HKEY_LOCAL_MACHINE, 
            "Software\\Microsoft\\Microsoft Games\\Allegiance\\1.0",
            0, 
            "", 
            REG_OPTION_NON_VOLATILE, 
            KEY_READ | KEY_WRITE, 
            NULL, 
            &m_hKey, 
            NULL
        );

    //
    // Get the exe and art paths
    //

    if (  
           FAILED(hr) 
        || (!GetRegistrySetting("EXE Path", m_achExePath, sizeof(m_achExePath)))
        || (!GetRegistrySetting( "ArtPath", m_achArtPath, sizeof(m_achArtPath)))
    ) {
        Error("Failure accessing Allegiance registry settings.");
    }

    //
    // Make sure the art path is in a valid location relative to the exe
    //

    if (!CheckArtPath()) {
        Error("The artpath must be a direct child of the Allegiance exe path for this utility to work.");
    }

    //
    // Figure out the paths for the shadow and retail directories
    //

    strcpy(m_achShadowPath, m_achExePath);
    strcpy(m_achShadowPath + strlen(m_achExePath), "Shadow");

    strcpy(m_achRetailPath, m_achExePath);
    strcpy(m_achRetailPath + strlen(m_achExePath), "Retail");

    //
    // Update the internal state
    //

    Update();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShadowSwitcherDlg::OnPaint() 
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
HCURSOR CShadowSwitcherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CShadowSwitcherDlg::DeleteDirectory(char* pch)
{
    CString strMatch = CString(pch) + "\\*.*";

    WIN32_FIND_DATA data;

    HANDLE hff = FindFirstFile(strMatch, &data);

    if (hff == INVALID_HANDLE_VALUE) {
        Error("Error reading directory contents");
    }

    while (true) {
        UpdateText((char*)LPCTSTR(CString("Deleting ") + CString(data.cFileName)));

        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (
                   (strcmp(data.cFileName, "." ) != 0)
                && (strcmp(data.cFileName, "..") != 0)
            ) {
                DeleteDirectory(
                    (char*)LPCTSTR(CString(pch) + CString("\\") + CString(data.cFileName))
                );
            }
        } else {
            CString strFile = CString(pch) + CString("\\") + CString(data.cFileName);
            if (
                   (!SetFileAttributes((char*)LPCTSTR(strFile), FILE_ATTRIBUTE_NORMAL))
                || (!DeleteFile((char*)LPCTSTR(strFile)))
            ) {
                Error("Error deleting file.");
            }
        }

        if (!FindNextFile(hff, &data)) {
            if (GetLastError() == ERROR_NO_MORE_FILES) {
                FindClose(hff);

                if (!RemoveDirectory(pch)) {
                    Error("Error removing directory");
                }

                return;
            }

            Error("Error deleting directory contents");
        }
    }
}

void CShadowSwitcherDlg::CopyDirectory(char* pchFrom, char* pchTo)
{
    if (!CreateDirectory(pchTo, NULL)) {
        Error("Unable to create directory.");
    }

    CString strMatch = CString(pchFrom) + "\\*.*";

    WIN32_FIND_DATA data;

    HANDLE hff = FindFirstFile(strMatch, &data);

    if (hff == INVALID_HANDLE_VALUE) {
        Error("Error reading directory contents");
    }

    while (true) {
        UpdateText((char*)LPCTSTR(CString("Copying ") + CString(data.cFileName)));

        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (
                   (strcmp(data.cFileName, "." ) != 0)
                && (strcmp(data.cFileName, "..") != 0)
            ) {
                CopyDirectory(
                    (char*)LPCTSTR(CString(pchFrom) + CString("\\") + CString(data.cFileName)), 
                    (char*)LPCTSTR(CString(pchTo  ) + CString("\\") + CString(data.cFileName))
                );
            }
        } else {
            if (!
                CopyFile(
                    (char*)LPCTSTR(CString(pchFrom) + CString("\\") + CString(data.cFileName)), 
                    (char*)LPCTSTR(CString(pchTo  ) + CString("\\") + CString(data.cFileName)),
                    true
                )
            ) {
                Error("Error copying file.");
            }
        }

        if (!FindNextFile(hff, &data)) {
            if (GetLastError() == ERROR_NO_MORE_FILES) {
                FindClose(hff);
                return;
            }

            Error("Error reading directory contents");
        }
    }
}

char g_achShadow[]    = ". Betaworld is in use.";
char g_achWasShadow[] = ". Betaworld has been used.";

bool EndMatches(char* psz, char* pszLast)
{
    int length     = strlen(psz);
    int lengthLast = strlen(pszLast);

    if (length < lengthLast) {
        return false;
    }

    int delta      = length - lengthLast;

    for (int index = 0; index < lengthLast; index++) {
        if (pszLast[index] != psz[index + delta]) {
            return false;
        }
    }

    return true;
}


void CShadowSwitcherDlg::Install() 
{
    if (!m_bInstalled) {
        m_bInstalled = true;
        CopyDirectory(m_achExePath, m_achShadowPath);

        if (!EndMatches(m_achPID, g_achWasShadow)) {
            CString strPID = m_achPID;
            CString newPID = strPID + CString(g_achWasShadow);
            SetRegistrySetting("PID", (char*)LPCTSTR(newPID));
        }

        SetRegistrySetting("SaveCfgFile", "None");

        Update();
    }
}

void CShadowSwitcherDlg::OnDelete() 
{
    assert(m_bInstalled);

    if (m_bShadow) {
        //
        // Switch back to retail
        //

        OnRetail();
    }

    assert(!m_bShadow);

    //
    // Delete the shadow path
    //

    DeleteDirectory(m_achShadowPath);
    ::RegDeleteValue(m_hKey, "SaveCfgFile");

    //
    // Update
    //

    Update();
}

#define ShadowCfgString "http://Allegiance.zone.com/allegianceshadow.cfg"

void CShadowSwitcherDlg::OnShadow() 
{
    //
    // Copy if needed
    //

    Install();

    //
    //
    //

    assert(!m_bShadow);
    m_bShadow = true;

    //
    // Rename the directories
    // 

    if (!MoveFile(m_achExePath, m_achRetailPath)) {
        Error("Error renaming the retail directory");
    }

    if (!MoveFile(m_achShadowPath, m_achExePath)) {
        Error("Error renaming the shadow directory");
    }

    //
    // Change the registry
    //

    SetRegistrySetting("SaveCfgFile", m_achCfgFile);
    SetRegistrySetting("CfgFile", ShadowCfgString);

    CString strPID = m_achPID;
    CString newPID = strPID.Left(strPID.GetLength() - (sizeof(g_achWasShadow) - 1)) + CString(g_achShadow);

    SetRegistrySetting("PID", (char*)LPCTSTR(newPID));

    //
    // Update
    //

    Update();
}

void CShadowSwitcherDlg::OnRetail() 
{
    assert(m_bInstalled);
    assert(m_bShadow);
    m_bShadow = false;

    //
    // Rename the directories
    // 

    if (!MoveFile(m_achExePath, m_achShadowPath)) {
        Error("Error renaming the retail directory");
    }

    if (!MoveFile(m_achRetailPath, m_achExePath)) {
        Error("Error renaming the shadow directory");
    }

    //
    // Change the registry
    //

    SetRegistrySetting("SaveCfgFile", "None");

    if (strcmp(m_achCfgSave, "None") == 0) {
        ::RegDeleteValue(m_hKey, "CfgFile");
    } else {
        SetRegistrySetting("CfgFile", m_achCfgSave);
    }

    CString strPID = m_achPID;
    CString newPID = strPID.Left(strPID.GetLength() - (sizeof(g_achShadow) - 1)) + CString(g_achWasShadow);

    SetRegistrySetting("PID", (char*)LPCTSTR(newPID));

    //
    // Update
    //

    Update();
}

void CShadowSwitcherDlg::UpdateText(char* psz)
{
    m_text.SetWindowText(psz);
    m_text.UpdateWindow();
}

void CShadowSwitcherDlg::Update()
{
    bool bExists = GetRegistrySetting("CfgFile", m_achCfgFile, sizeof(m_achCfgFile));

    m_bShadow = 
           bExists 
        && (0 == strcmp(ShadowCfgString, m_achCfgFile));

    if (!bExists) {
        strcpy(m_achCfgFile, "None");
    }

    m_bInstalled = GetRegistrySetting("SaveCfgFile", m_achCfgSave, sizeof(m_achCfgSave));
    GetRegistrySetting("PID", m_achPID, sizeof(m_achPID));

    if (m_bShadow) {
        m_text.SetWindowText("You are currently using the Allegiance shadow servers.");

        m_btnShadow.EnableWindow(false);
        m_btnRetail.EnableWindow(true);
        m_btnDelete.EnableWindow(true);
    } else {
        m_text.SetWindowText("You are currently using the Allegiance retail servers.");

        m_btnShadow.EnableWindow(true);
        m_btnRetail.EnableWindow(false);

        m_btnDelete.EnableWindow(m_bInstalled);
    }
}

void CShadowSwitcherDlg::Error(char *psz)
{
    DWORD error = GetLastError();
    MessageBox(psz, "Error", MB_ICONERROR | MB_OK);

    //
    // Better way to exit?
    //

    _exit(0);
}

bool CShadowSwitcherDlg::GetRegistrySetting(char *pszValue, char *pszResult, DWORD length)
{
    DWORD dwSize = length;
    DWORD dwType = REG_SZ;

    return 
           ::RegQueryValueEx(m_hKey, pszValue, NULL, &dwType, (BYTE*)pszResult, &dwSize)
        == ERROR_SUCCESS;
}

void CShadowSwitcherDlg::SetRegistrySetting(char *pszValue, char *psz)
{
    int length = strlen(psz) + 1;

    long result =
        RegSetValueEx(
            m_hKey,
            pszValue,
            NULL,
            REG_SZ,
            (const unsigned char*)psz,
            length
        );

    if (result != ERROR_SUCCESS) {
        Error("Couldn't set registry key");
    }
}

bool CShadowSwitcherDlg::CheckArtPath()
{
    int length = strlen(m_achExePath);
    for (int index = 0; index < length; index++) {
        if (m_achExePath[index] != m_achArtPath[index]) {
            return false;
        }
    }

    if (0 != strcmp(m_achArtPath + length, "\\Artwork")) {
        return false;
    }

    return true;
}
