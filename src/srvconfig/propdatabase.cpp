// PropDatabase.cpp : implementation file
//

#include "stdafx.h"
#include "srvconfig.h"
#include "PropDatabase.h"
#include <atldbcli.h>

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPropDatabase property page

IMPLEMENT_DYNCREATE(CPropDatabase, CPropertyPage)

CPropDatabase::CPropDatabase() : CPropertyPage(CPropDatabase::IDD)
{
    //{{AFX_DATA_INIT(CPropDatabase)
    m_strSQLPassword = _T("");
    m_strSQLSrc = _T("");
    m_strSQLUser = _T("");
    m_strConnectionString = _T("");
	m_strConnectionString2 = _T("");
	m_nNotifyThreads = 3;
	m_nSilentThreads = 2;
	m_nNotifyThreads2 = 0;
	m_nSilentThreads2 = 0;
	//}}AFX_DATA_INIT
}

CPropDatabase::~CPropDatabase()
{
}

void CPropDatabase::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropDatabase)
    DDX_Control(pDX, IDC_CONNECTIONSTRING, m_editConnectionString);
    DDX_Text(pDX, IDC_CONNECTIONSTRING, m_strConnectionString);
	//}}AFX_DATA_MAP
    #if !defined(ALLSRV_STANDALONE)
      DDX_Text(pDX, IDC_SQLPASSWORD, m_strSQLPassword);
      DDX_Text(pDX, IDC_SQLSRC, m_strSQLSrc);
      DDX_Text(pDX, IDC_SQLUSER, m_strSQLUser);
  	  DDX_Control(pDX, IDC_CONNECTIONSTRING2, m_editConnectionString2);
      DDX_Text(pDX, IDC_CONNECTIONSTRING2, m_strConnectionString2);
	  DDX_Text(pDX, IDC_SQLNOTIFYTHREADS, m_nNotifyThreads);
	  DDX_Text(pDX, IDC_SQLSILENTTHREADS, m_nSilentThreads);
  	  DDX_Text(pDX, IDC_SQLNOTIFYTHREADS2, m_nNotifyThreads2);
	  DDX_Text(pDX, IDC_SQLSILENTTHREADS2, m_nSilentThreads2);
    #endif // !defined(ALLSRV_STANDALONE)
}


BEGIN_MESSAGE_MAP(CPropDatabase, CPropertyPage)
    //{{AFX_MSG_MAP(CPropDatabase)
    ON_BN_CLICKED(IDC_BTNCONNECTIONSTRING, OnBtnconnectionstring)
	ON_BN_CLICKED(IDC_BTNCONNECTIONSTRING2, OnBtnconnectionstring2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Implementation

int CALLBACK CPropDatabase::WordBreakProc(LPTSTR lpch, int ichCurrent,
  int cch, int code)
{
    switch (code)
    {
        case WB_ISDELIMITER:
        {
            LPCTSTR psz = lpch + ichCurrent - 1;
            return psz >= lpch && TEXT(';') == *psz;
        }
        case WB_LEFT:
        {
            LPCTSTR psz = lpch + ichCurrent - 2;
            while (psz >= lpch && TEXT(';') != *psz)
                --psz;
            return (psz < lpch) ? 0 : (psz + 1 - lpch);
        }
        case WB_RIGHT:
        {
            LPCTSTR pszEnd = lpch + cch;
            LPCTSTR psz = lpch + ichCurrent - 1;
            while (psz < pszEnd && TEXT(';') != *psz)
                ++psz;
            ++psz;
            return min(psz - lpch, cch);
        }
    }

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

BOOL CPropDatabase::OnInitDialog() 
{
    CoInitialize( NULL );
    CPropertyPage::OnInitDialog();

    // Set the word-break proc for the edit control
    m_editConnectionString.SendMessage(EM_SETWORDBREAKPROC, 0,
        (LPARAM)WordBreakProc);
    m_editConnectionString2.SendMessage(EM_SETWORDBREAKPROC, 0,
        (LPARAM)WordBreakProc);
    
    // get the stuff from the registry
    HKEY hKey;
    DWORD dwType;
    char  szValue[MAX_PATH];
#if !defined(ALLSRV_STANDALONE)
    DWORD dwValue;
#endif
    DWORD cbValue;
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, HKLM_FedSrv, 0, KEY_READ, &hKey))
    {
#if !defined(ALLSRV_STANDALONE)
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLUser", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strSQLUser = szValue;
        else
            m_strSQLUser = "sa";
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLPW", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strSQLPassword = szValue;
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLSrc", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strSQLSrc = szValue;
        else
            m_strSQLSrc = "federation";
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLConfig", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strConnectionString2 = szValue;
        dwValue=0;
        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLThreadsSilent", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nSilentThreads = dwValue;
        else
            m_nSilentThreads = 2;
        dwValue=0;
        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLThreadsNotify", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nNotifyThreads = dwValue;
        else
            m_nNotifyThreads = 3;
#endif
        szValue[0]='\0';
        cbValue = MAX_PATH;
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "ConnectionString", NULL, &dwType, (unsigned char*)szValue, &cbValue))
            m_strConnectionString = szValue;

        RegCloseKey(hKey);
    }
#if !defined(ALLSRV_STANDALONE)
    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, HKLM_AllLobby, 0, KEY_READ, &hKey))
    {
        dwValue=0;
        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLThreadsSilent", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nSilentThreads2 = dwValue;
        else
            m_nSilentThreads2 = 1;
        dwValue=0;
        cbValue = sizeof(dwValue);
        if (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "SQLThreadsNotify", NULL, &dwType, (unsigned char*)&dwValue, &cbValue))
            m_nNotifyThreads2 = dwValue;
        else
            m_nNotifyThreads2 = 5;

        RegCloseKey(hKey);
    }
#endif

    UpdateData(false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropDatabase::OnOK() 
{
    UpdateData(true);

    CDefaultRegKey key;
    if (ERROR_SUCCESS == key.Create(HKEY_LOCAL_MACHINE, HKLM_FedSrv))
    {
#if !defined(ALLSRV_STANDALONE)
        key.SetValue(m_strSQLUser,            "SQLUser",          "sa" );
        key.SetValue(m_strSQLPassword,        "SQLPW",            ""   );
        key.SetValue(m_strSQLSrc,             "SQLSrc",           "federation"   );
        key.SetValue(m_strConnectionString2,  "SQLConfig",        ""   );
        key.SetValue(m_nSilentThreads,        "SQLThreadsSilent", 2    );
        key.SetValue(m_nNotifyThreads,        "SQLThreadsNotify", 3    );
#endif
        key.SetValue(m_strConnectionString,   "ConnectionString", ""   );
    }
    
#if !defined(ALLSRV_STANDALONE)
    CDefaultRegKey keyLobby;
    if (ERROR_SUCCESS == keyLobby.Create(HKEY_LOCAL_MACHINE, HKLM_AllLobby))
    {
        keyLobby.SetValue(m_strConnectionString2,   "SQLConfig",        "");
        keyLobby.SetValue(m_nSilentThreads2,        "SQLThreadsSilent", 1 );
        keyLobby.SetValue(m_nNotifyThreads2,        "SQLThreadsNotify", 5 );
    }
#endif
    
    CPropertyPage::OnOK();
}

void CPropDatabase::OnBtnconnectionstring() 
{
#if 1
    CDataSource m_DataSource;
    HRESULT hr;
    m_DataSource.Close();
    hr = m_DataSource.Open();
    if ( FAILED( hr ) )
    {
        m_DataSource.Close();
        if ( REGDB_E_CLASSNOTREG == hr )
        {
            // Microsoft Data Link component not found.
            MessageBox( _T("Microsoft Data Link component not found "
                           "(Program Files\\Common Files\\system\\ole db\\MSDASC.DLL)."
                           "This sample requires the Data Link component.") );
        }
        return;
    }
    BSTR bstrTemp = NULL;
    if ( FAILED( m_DataSource.GetInitializationString( (BSTR*) &bstrTemp, TRUE ) ) )
    {
        m_DataSource.Close();
        return;
    }
    m_strConnectionString = bstrTemp;
    SysFreeString(bstrTemp);
    UpdateData(false);
#else
    CComPtr<IDBPromptInitialize> spPrompt;
    HRESULT hr = spPrompt.CoCreateInstance(CLSID_DataLinks);
    if (FAILED(hr))
    {
        // Microsoft Data Link component not found.
        MessageBox( _T("Microsoft Data Link component not found "
                       "(Program Files\\Common Files\\system\\ole db\\MSDASC.DLL)."
                       "This sample requires the Data Link component.") );
        return;
    }

    // Open the data source specified by current string, if any
    USES_CONVERSION;
    CDataSource m_DataSource;
    m_DataSource.OpenFromInitializationString(T2COLE(m_strConnectionString));

    // Prompt the user for a data source
    IUnknown* pUnk = m_DataSource.m_spInit;
    pUnk->AddRef();
    hr = spPrompt->PromptDataSource(NULL, GetSafeHwnd(),
        DBPROMPTOPTIONS_WIZARDSHEET, 0, NULL, NULL, IID_IUnknown, &pUnk);
    if (SUCCEEDED(hr))
    {
        m_DataSource.m_spInit = CComQIPtr<IDBInitialize>(pUnk);
        pUnk->Release();

        CComBSTR bstrTemp;
        m_DataSource.GetInitializationString(&bstrTemp, true);
        m_strConnectionString = bstrTemp;
        UpdateData(false);
    }
#endif
}

void CPropDatabase::OnBtnconnectionstring2() 
{
#if 1
    CDataSource m_DataSource;
    HRESULT hr;
    m_DataSource.Close();
    hr = m_DataSource.Open();
    if ( FAILED( hr ) )
    {
        m_DataSource.Close();
        if ( REGDB_E_CLASSNOTREG == hr )
        {
            // Microsoft Data Link component not found.
            MessageBox( _T("Microsoft Data Link component not found "
                           "(Program Files\\Common Files\\system\\ole db\\MSDASC.DLL)."
                           "This sample requires the Data Link component.") );
        }
        return;
    }
    BSTR bstrTemp = NULL;
    if ( FAILED( m_DataSource.GetInitializationString( (BSTR*) &bstrTemp, TRUE ) ) )
    {
        m_DataSource.Close();
        return;
    }
    m_strConnectionString2 = bstrTemp;
    SysFreeString(bstrTemp);
    UpdateData(false);
#else
    CComPtr<IDBPromptInitialize> spPrompt;
    HRESULT hr = spPrompt.CoCreateInstance(CLSID_DataLinks);
    if (FAILED(hr))
    {
        // Microsoft Data Link component not found.
        MessageBox( _T("Microsoft Data Link component not found "
                       "(Program Files\\Common Files\\system\\ole db\\MSDASC.DLL)."
                       "This sample requires the Data Link component.") );
        return;
    }

    // Open the data source specified by current string, if any
    USES_CONVERSION;
    CDataSource m_DataSource;
    m_DataSource.OpenFromInitializationString(T2COLE(m_strConnectionString2));

    // Prompt the user for a data source
    IUnknown* pUnk = m_DataSource.m_spInit;
    pUnk->AddRef();
    hr = spPrompt->PromptDataSource(NULL, GetSafeHwnd(),
        DBPROMPTOPTIONS_WIZARDSHEET, 0, NULL, NULL, IID_IUnknown, &pUnk);
    if (SUCCEEDED(hr))
    {
        m_DataSource.m_spInit = CComQIPtr<IDBInitialize>(pUnk);
        pUnk->Release();

        CComBSTR bstrTemp;
        m_DataSource.GetInitializationString(&bstrTemp, true);
        m_strConnectionString2 = bstrTemp;
        UpdateData(false);
    }
#endif
}
