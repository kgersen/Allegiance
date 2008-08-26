/////////////////////////////////////////////////////////////////////////////
// CSrvConfigSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SrvConfig.h"
#include "SrvConfigSheet.h"

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
// CSrvConfigSheet dialog


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CSrvConfigSheet, CPropertySheet)
    //{{AFX_MSG_MAP(CSrvConfigSheet)
    ON_WM_SYSCOMMAND()
    ON_BN_CLICKED(IDOK, OnOK)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CSrvConfigSheet::CSrvConfigSheet(CWnd* pParentWnd, UINT iSelectPage)
    : CPropertySheet(IDS_TITLE, pParentWnd, iSelectPage)
{
    // Add our property pages
    AddPage(&m_pp1);
    #if !defined(ALLSRV_STANDALONE)
      AddPage(&m_pp2);
    #endif // !defined(ALLSRV_STANDALONE)
    AddPage(&m_pp3);

    // Only add this one if AGC is properly registered
    if (m_pp4.IsDisplayable())
        AddPage(&m_pp4);
}

BOOL CSrvConfigSheet::Create()
{
  // Declare the style of window to use
  static const DWORD dwStyle = WS_VISIBLE | WS_SYSMENU | WS_CAPTION |
    WS_POPUP | WS_MINIMIZEBOX | DS_MODALFRAME;

  // Perform default processing
  if (!CPropertySheet::Create(NULL, dwStyle))
    return false;

  // Change this flag
  m_nFlags &= WF_CONTINUEMODAL;

  // Indicate success
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CSrvConfigSheet::InitSysMenu()
{
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
}

void CSrvConfigSheet::InitIcon()
{
    // Load and set the large icon
    LPCTSTR pszRes = MAKEINTRESOURCE(IDR_MAINFRAME);
    HICON hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), pszRes,
        IMAGE_ICON, ::GetSystemMetrics(SM_CXICON),
        ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hIcon, true);

    // Load and set the small icon
    HICON hIconSmall = (HICON)::LoadImage(AfxGetResourceHandle(), pszRes, 
        IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON),
        ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    SetIcon(hIconSmall, false);
}

void CSrvConfigSheet::InitButtons()
{
    // Caculate the height of 7 dialog units
    CRect rectUnits(0, 0, 0, 7);
    MapDialogRect(GetSafeHwnd(), rectUnits);

    // Get the OK and cancel buttons
    CWnd* pwndOK     = GetDlgItem(IDOK);
    CWnd* pwndCancel = GetDlgItem(IDCANCEL);
    CWnd* pwndHelp   = GetDlgItem(IDHELP);

    // Get the window rectangles of the buttons
    CRect rectOK, rectCancel, rectHelp;
    pwndOK->GetWindowRect(rectOK);
    pwndCancel->GetWindowRect(rectCancel);
    pwndHelp->GetWindowRect(rectHelp);
    ScreenToClient(rectOK);
    ScreenToClient(rectCancel);
    ScreenToClient(rectHelp);

    // Compute the offset to position these buttons flush-right
    int cxOffset = rectHelp.right - rectCancel.right;

    // Move the buttons into place
    rectOK.OffsetRect(cxOffset, 0);
    rectCancel.OffsetRect(cxOffset, 0);
    pwndOK->MoveWindow(rectOK);
    pwndCancel->MoveWindow(rectCancel);

    // Compute the height offset of the client area
    int cyClient = rectCancel.Height() + rectUnits.Height();

    // Compute our new window rectangle
    CRect rectWnd;
    GetWindowRect(rectWnd);
    rectWnd.bottom += cyClient;

    // Resize our window
    MoveWindow(rectWnd);

    // Show the OK and Cancel buttons
    GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
    GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
    
    // Enable the OK and Cancel buttons
    GetDlgItem(IDOK)->EnableWindow(true);
    GetDlgItem(IDCANCEL)->EnableWindow(true);
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CSrvConfigSheet::PostNcDestroy()
{
  delete this;
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CSrvConfigSheet::OnInitDialog()
{
    // Perform default processing
    CPropertySheet::OnInitDialog();

    // Add "About..." menu item to system menu.
    InitSysMenu();

    // Set the icon for this dialog.
    InitIcon();

    // Initialize the OK and Cancel buttons
    InitButtons();

    // Return true to set the focus to the first tabstop control
    return true;
}


void CSrvConfigSheet::OnSysCommand(UINT nID, LPARAM lParam)
{
    // Handle the About... menu item
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
        return;
    }

    // Perform default processing
    CPropertySheet::OnSysCommand(nID, lParam);
}

void CSrvConfigSheet::OnOK()
{
  // Delegate to each property page
  for (int i = 0; i < GetPageCount(); ++i)
  {
    CPropertyPage* pPage = GetPage(i);
    if (pPage->GetSafeHwnd())
      pPage->OnOK();
  }

  // Destroy the window
  DestroyWindow();
}

void CSrvConfigSheet::OnCancel()
{
  // Delegate to each property page
  for (int i = 0; i < GetPageCount(); ++i)
  {
    CPropertyPage* pPage = GetPage(i);
    if (pPage->GetSafeHwnd())
      pPage->OnCancel();
  }

  // Destroy the window
  DestroyWindow();
}

