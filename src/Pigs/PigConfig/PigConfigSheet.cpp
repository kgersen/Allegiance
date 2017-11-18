/////////////////////////////////////////////////////////////////////////////
// CPigConfigSheet.cpp : Implementation of the CPigConfigSheet class.
//

#include "pch.h"
#include "resource.h"
#include "PigConfigSheet.h"
#include "AboutDlg.h"


/////////////////////////////////////////////////////////////////////////////
// CPigConfigSheet dialog


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CPigConfigSheet, CPropertySheet)
  //{{AFX_MSG_MAP(CPigConfigSheet)
  ON_WM_SYSCOMMAND()
  ON_BN_CLICKED(IDOK, OnOK)
  ON_BN_CLICKED(IDCANCEL, OnCancel)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CPigConfigSheet::CPigConfigSheet(CWnd* pParentWnd, UINT iSelectPage)
  : CPropertySheet(IDS_TITLE, pParentWnd, iSelectPage),
  m_PageGeneral(this),
  m_PageEvents(this),
  m_PageServers(this)
{
}

bool CPigConfigSheet::Create()
{
  // Attempt to connect to a PigSrv on the local computer
  HRESULT hr = m_spSession.CreateInstance("Pigs");

  if (FAILED(hr))
  {
    CString strMsg;
    strMsg.Format(IDS_E_CANTCREATESESSION, hr, GetErrorText(hr));
    MessageBox(strMsg);
    return false;
  }

  // Add our property pages
  AddPage(&m_PageGeneral);
  AddPage(&m_PageServers);

  // Only add this one if AGC is properly registered
  if (m_PageEvents.IsDisplayable())
    AddPage(&m_PageEvents);

  // Declare the style of window to use
  static const DWORD dwStyle = WS_VISIBLE | WS_SYSMENU | WS_CAPTION |
    WS_POPUP | WS_MINIMIZEBOX | DS_MODALFRAME;

  // Perform default processing
  if (!CPropertySheet::Create(NULL, dwStyle))
  {
    DWORD dwLastError = ::GetLastError();
    CString strMsg;
    strMsg.Format(IDS_E_CANTCREATEWINDOW, dwLastError, GetErrorText(dwLastError));
    MessageBox(strMsg);
    return false;
  }

  // Change this flag
  m_nFlags &= WF_CONTINUEMODAL;

  // Indicate success
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CPigConfigSheet::InitSysMenu()
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

void CPigConfigSheet::InitIcon()
{
  // Load and set the large icon
  LPCTSTR pszRes = MAKEINTRESOURCE(IDR_MAINFRAME);
  HICON hIcon = (HICON)::LoadImage(AfxGetResourceHandle(), pszRes,
    IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON),
    LR_DEFAULTCOLOR);
  SetIcon(hIcon, true);

  // Load and set the small icon
  HICON hIconSmall = (HICON)::LoadImage(AfxGetResourceHandle(), pszRes, 
    IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON),
    ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
  SetIcon(hIconSmall, FALSE);
}

void CPigConfigSheet::InitButtons()
{
  // Caculate the height of 7 dialog units
  CRect rectUnits(0, 0, 0, 7);
  MapDialogRect(/*GetSafeHwnd(), */rectUnits);

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

void CPigConfigSheet::PostNcDestroy()
{
  delete this;
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CPigConfigSheet::OnInitDialog()
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


void CPigConfigSheet::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPigConfigSheet::OnOK()
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

void CPigConfigSheet::OnCancel()
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

