/////////////////////////////////////////////////////////////////////////////
// DlgChatPreferences.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "PageChat.h"
#include "DlgChatPreferences.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

#if defined(_UNICODE) || defined(UNICODE)
  #define IShellLinkPtr IShellLinkWPtr
#else // defined(_UNICODE) || defined(UNICODE)
  #define IShellLinkPtr IShellLinkAPtr
#endif // defined(_UNICODE) || defined(UNICODE)


/////////////////////////////////////////////////////////////////////////////
// CDlgChatPreferences dialog


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CDlgChatPreferences, CDialog)
  //{{AFX_MSG_MAP(CDlgChatPreferences)
	ON_BN_CLICKED(IDC_ENABLE_CHAT_LIST, OnEnableChatList)
	ON_BN_CLICKED(IDC_UNLIMITED_CHAT_HISTORY, OnUnlimitedChatHistory)
	ON_BN_CLICKED(IDC_LIMIT_CHAT_HISTORY, OnLimitChatHistory)
	ON_BN_CLICKED(IDC_ADMIN_RUN, OnAdminRun)
	ON_BN_CLICKED(IDC_ADMIN_RUN_PROPERTIES, OnAdminRunProperties)
	ON_BN_CLICKED(IDC_ADMIN_RUN_TEST, OnAdminRunTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CDlgChatPreferences::CDlgChatPreferences(CPageChat* pPage)
  : CDialog(CDlgChatPreferences::IDD, pPage),
  m_pPage(pPage)
{
  //{{AFX_DATA_INIT(CDlgChatPreferences)
	m_bAdminChatList = FALSE;
  m_bAdminMessageBox = FALSE;
  m_bAdminRun = FALSE;
	m_bEnableChatList = FALSE;
  m_eChatListLimit = -1;
  m_dwChatListLimit = 0;
	//}}AFX_DATA_INIT

  // Get the module filepath
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(NULL, szModule, sizeofArray(szModule));
  TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
  _tsplitpath(szModule, szDrive, szDir, NULL, NULL);
  _tmakepath(szModule, szDrive, szDir, TEXT("AdminAlert"), TEXT(".lnk"));
  m_strShortcutPath = szModule;
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CDlgChatPreferences::DoDataExchange(CDataExchange* pDX)
{
  // Perform default processing
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CDlgChatPreferences)
  DDX_Control(pDX, IDC_ADMIN_RUN_PROPERTIES, m_btnAdminRunProperties);
	DDX_Control(pDX, IDC_UNLIMITED_CHAT_HISTORY, m_btnUnlimitedChat);
	DDX_Control(pDX, IDC_LIMIT_CHAT_HISTORY, m_btnLimitChat);
	DDX_Control(pDX, IDC_CHAT_HISTORY_LIMIT, m_editChatListLimit);
	DDX_Control(pDX, IDC_LIMIT_POST_CAPTION, m_staticChatListLimit);
	DDX_Check(pDX, IDC_ADMIN_CHAT_LIST, m_bAdminChatList);
  DDX_Check(pDX, IDC_ADMIN_MSGBOX, m_bAdminMessageBox);
  DDX_Check(pDX, IDC_ADMIN_RUN, m_bAdminRun);
	DDX_Check(pDX, IDC_ENABLE_CHAT_LIST, m_bEnableChatList);
  DDX_Radio(pDX, IDC_UNLIMITED_CHAT_HISTORY, m_eChatListLimit);
  DDX_Text(pDX, IDC_CHAT_HISTORY_LIMIT, m_dwChatListLimit);
	//}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CDlgChatPreferences::LoadFromRegistry()
{
  // Open/create the registry key
  CRegKey key;
  key.Create(HKEY_LOCAL_MACHINE, HKLM_AllSrvUI);

  // mdvalley: all QueryDWORDValue(text, dwbool);
  // AdminAlertChatList
  DWORD dwBool = true;
  key.QueryValue(dwBool, TEXT("AdminAlertChatList"));
  m_bAdminChatList = !!dwBool;

  // AdminAlertMessageBox
  dwBool = true;
  key.QueryValue(dwBool, TEXT("AdminAlertMessageBox"));
  m_bAdminMessageBox = !!dwBool;

  // AdminAlertRun
  dwBool = false;
  if (ShortcutExists())
    key.QueryValue(dwBool, TEXT("AdminAlertRun"));
  m_bAdminRun = !!dwBool;

  // ChatList
  dwBool = true;
  key.QueryValue(dwBool, TEXT("ChatList"));
  m_bEnableChatList = !!dwBool;

  // ChatListLimit
  key.QueryValue(m_dwChatListLimit, TEXT("ChatListLimit"));
}

void CDlgChatPreferences::SaveToRegistry()
{
  // Open/create the registry key
  CRegKey key;
  key.Create(HKEY_LOCAL_MACHINE, HKLM_AllSrvUI);

  // mdvalley: All SetDWORDValue(text, m_b(
  // AdminAlertChatList
  key.SetValue(m_bAdminChatList, TEXT("AdminAlertChatList"));

  // AdminAlertMessageBox
  key.SetValue(m_bAdminMessageBox, TEXT("AdminAlertMessageBox"));

  // AdminAlertRun
  key.SetValue(m_bAdminRun, TEXT("AdminAlertRun"));

  // ChatList
  key.SetValue(m_bEnableChatList, TEXT("ChatList"));

  // ChatListLimit
  key.SetValue(m_dwChatListLimit, TEXT("ChatListLimit"));
}

bool CDlgChatPreferences::ShortcutExists()
{
  // Open the file for read-only access
  UINT uModeOld = ::SetErrorMode(SEM_NOOPENFILEERRORBOX);
  TCHandle shFile = ::CreateFile(m_strShortcutPath, GENERIC_READ,
    FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  ::SetErrorMode(uModeOld);
  return (INVALID_HANDLE_VALUE == shFile) ? (shFile.Detach(), false) : true;
}

void CDlgChatPreferences::UpdateUI()
{
	m_btnUnlimitedChat.EnableWindow(m_bEnableChatList);
  m_btnLimitChat.EnableWindow(m_bEnableChatList);
	m_editChatListLimit.EnableWindow(m_bEnableChatList && m_eChatListLimit);
  m_staticChatListLimit.EnableWindow(m_bEnableChatList && m_eChatListLimit);
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CDlgChatPreferences::OnInitDialog() 
{
  // Perform default processing
  CDialog::OnInitDialog();

  // Select the ChatListLimit radio button
  m_eChatListLimit = (0 == m_dwChatListLimit) ? 0 : 1;

  // Update the UI
  UpdateUI();

  // Update fields from data members
  UpdateData(false);

  // Return true to set the focus to the first tabstop control
  return true;
}

void CDlgChatPreferences::OnOK() 
{
  // Perform default processing
  CDialog::OnOK();

  // Set m_dwChatListLimit appropriately
  if (0 == m_eChatListLimit)
    m_dwChatListLimit = 0;

  // Can't run a shortcut if it doesn't exist
  if (!ShortcutExists())
    m_bAdminRun = false;

  // Save values to the registry
  SaveToRegistry();
}

void CDlgChatPreferences::OnEnableChatList() 
{
  // Update data members from fields
  UpdateData();

  // Update the UI
  UpdateUI();
}

void CDlgChatPreferences::OnUnlimitedChatHistory() 
{
  // Update data members from fields
  UpdateData();

  // Update the UI
  UpdateUI();
}

void CDlgChatPreferences::OnLimitChatHistory() 
{
  // Update data members from fields
  UpdateData();

  // Update the UI
  UpdateUI();
}

void CDlgChatPreferences::OnAdminRun() 
{
  // Create the shortcut and set properties if it does not exist
  if (!ShortcutExists())
    OnAdminRunProperties();
}

void CDlgChatPreferences::OnAdminRunProperties() 
{
    // <NKM>
    // remocve for now

//#if 0
  // Create the shortcut if it does not exist
  if (!ShortcutExists())
  {
    IShellLinkPtr spShellLink;
    HRESULT hr = spShellLink.CreateInstance(CLSID_ShellLink);
    if (SUCCEEDED(hr))
    {
      USES_CONVERSION;
      IPersistFilePtr spPersistFile(spShellLink);
      if (NULL != spPersistFile)
        hr = spPersistFile->Save(T2COLE(m_strShortcutPath), true);
    }
  }

  // Display the shortcut properties
  SHELLEXECUTEINFO shei = {sizeof(shei)};
  shei.fMask  = SEE_MASK_NOCLOSEPROCESS;
  shei.lpVerb = TEXT("properties");
  shei.lpFile = m_strShortcutPath;
  shei.nShow  = SW_SHOWNORMAL;
  bool bExecuted = !!::ShellExecuteEx(&shei);
//#endif
}

void CDlgChatPreferences::OnAdminRunTest() 
{
  IAGCEventCreatePtr spEventCreate;
  HRESULT hr = spEventCreate.CreateInstance(__uuidof(AGCEvent));
  if (SUCCEEDED(hr))
  {
    spEventCreate->Init();
    spEventCreate->put_SubjectName(CComBSTR(L"HelplessNewbie"));
    spEventCreate->put_ID(EventID_AdminPage);
    spEventCreate->AddProperty(CComBSTR(L"Message"),
      &CComVariant(L"Test message from Chat Preferences."));
    spEventCreate->AddProperty(CComBSTR(L"Target"),
      &CComVariant(L"ADMIN"));
    spEventCreate->AddProperty(CComBSTR(L"GameID"),
      &CComVariant(L"TEST"));

    // Save the current AdminAlert options
	  bool bAdminChatList   = !!m_bAdminChatList;
    bool bAdminMessageBox = !!m_bAdminMessageBox;
    bool bAdminRun        = !!m_bAdminRun;

    // Update the current values to test
    UpdateData();

    // Fire the test event
    IAGCEventPtr spEvent(spEventCreate);
    m_pPage->OnEvent(spEvent);

    // Restore the previous AdminAlert options
	  m_bAdminChatList   = bAdminChatList;
    m_bAdminMessageBox = bAdminMessageBox;
    m_bAdminRun        = bAdminRun;
  }
}

