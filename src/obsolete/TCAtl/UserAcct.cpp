/////////////////////////////////////////////////////////////////////////////
// UserAcct.cpp : Implementation of the TCUserAccount class.
//

#include "UserAcct.h"


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

HRESULT TCUserAccount::Init(LPCWSTR szUserName)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Delete any previous user name
  m_spszUserName = NULL;

  // Delete any previous SID
  m_spSIDPrincipal = NULL;

  // Get the SID and domain name of the specified user
  RETURN_FAILED(GetSID(szUserName, &m_spSIDPrincipal, &m_spszDomainName));

  // Get a pointer to just the user name (no domain)
  LPCWSTR pszWhack = wcschr(szUserName, L'\\');
  LPCWSTR pszUserOnly = pszWhack ? pszWhack + 1 : szUserName;

  // Save the user name
  int cchUserName = wcslen(pszUserOnly) + 1;
  m_spszUserName = (LPWSTR)CoTaskMemAlloc(cchUserName * sizeof(WCHAR));
  wcscpy(m_spszUserName, pszUserOnly);

  // Get the server information of the local machine
  TCNetApiPtr<SERVER_INFO_101*> si101;
  DWORD dw = NetServerGetInfo(NULL, 101, (LPBYTE*)&si101);
  if (NERR_Success != dw)
    return HRESULT_FROM_WIN32(dw);

  // Declare and initialize an LSA_OBJECT_ATTRIBUTES structure
  LSA_OBJECT_ATTRIBUTES oa = {sizeof(oa)};

  // Special processing when the local computer is a backup domain controller
  TCNetApiPtr<WCHAR*> domainController;
  if (si101->sv101_type & SV_TYPE_DOMAIN_BAKCTRL)
  {
    // Get the server name of the primary domain controller
    TCNetApiPtr<USER_MODALS_INFO_2*> umi2;
    if (0 == (dw = NetUserModalsGet(NULL, 2, (LPBYTE*)&umi2)))
    {
      // Get the domain name of the primary domain controller
      NetGetDCName(NULL, umi2->usrmod2_domain_name, (LPBYTE*)&domainController);

      // Create an LSA_UNICODE_STRING for the name of the PDC
      LSA_UNICODE_STRING lsaPDC;
      lsaPDC.Length        = (USHORT)((wcslen(domainController) * sizeof(WCHAR))-2);
      lsaPDC.MaximumLength = (USHORT)(lsaPDC.Length + sizeof(WCHAR));
      lsaPDC.Buffer        = &domainController[2];

      // Open the policy of the primary domain controller
      RETURN_FAILED(LsaOpenPolicy(&lsaPDC, &oa,
        POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &m_hPolicy));
    }
  }

  // Open the policy of the local computer if not a BDC or if anything failed
  if (domainController.IsNull())
  {
    RETURN_FAILED(LsaOpenPolicy(NULL, &oa,
      POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &m_hPolicy));
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

// Security ID (SID) Lookup
HRESULT TCUserAccount::GetSID(LPCWSTR szUserName, PSID* ppSID, LPWSTR* ppszDomain)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Initialize the [out] parameters
  if (ppSID)
    *ppSID = NULL;
  if (ppszDomain)
    *ppszDomain = NULL;

  // Skip past "\" or ".\", if the specified name begins with that
  if (L'\\' == szUserName[0])
    szUserName += 1;
  else if (L'.' == szUserName[0] && L'\\' == szUserName[1])
    szUserName += 2;

  // Get the needed size of the buffers
  SID_NAME_USE eUse;
  DWORD cbSID = 0, cchDomain = 0;
  if (!LookupAccountNameW(NULL, szUserName, NULL, &cbSID, NULL, &cchDomain, &eUse))
  {
    DWORD dwLastError = GetLastError();
    if (ERROR_INSUFFICIENT_BUFFER != dwLastError)
      return HRESULT_FROM_WIN32(dwLastError);
  }

  // Allocate domain name buffer on the task allocation, since we return it
  DWORD cbDomain = cchDomain * sizeof(WCHAR);
  TCCoTaskPtr<WCHAR*> spszDomain = (WCHAR*)CoTaskMemAlloc(cbDomain);
  if (spszDomain.IsNull())
    return E_OUTOFMEMORY;

  // Allocate the SID buffer on the task allocator, since we return it
  TCCoTaskPtr<PSID> spSID = (PSID)CoTaskMemAlloc(cbSID);
  if (spSID.IsNull())
    return E_OUTOFMEMORY;

  // Lookup the account name
  if (!LookupAccountNameW(NULL, szUserName, spSID, &cbSID, spszDomain,
    &cchDomain, &eUse))
      return HRESULT_FROM_WIN32(GetLastError());

  // Copy the SID to the [out] parameter
  if (ppSID)
    *ppSID = spSID.Detach();

  // Copy the domain string to the [out] parameter
  if (ppszDomain)
    *ppszDomain = spszDomain.Detach();

  // Indicate success
  return S_OK;
}


// AppID Helpers
HRESULT TCUserAccount::ResolveAppID(LPWSTR pszAppID, HKEY* phKey, REGSAM samDesired)
{
  USES_CONVERSION;

  // Is the specified string a GUID string?
  GUID appid;
  HRESULT hr = CLSIDFromString(pszAppID, &appid);
  if (FAILED(hr))
  {
    // Is the specified string a ProgID?
    if (FAILED(hr = CLSIDFromProgID(pszAppID, &appid)))
      return hr;

    // Convert the ProgID's CLSID to a string
    _VERIFYE(StringFromGUID2(appid, pszAppID, 48));

    // Recursively call into ourself to resolve the CLSID string into an AppID
    return ResolveAppID(pszAppID, phKey, samDesired);
  }

  // Convert the AppID back to a string
  RETURN_FAILED(StringFromGUID2(appid, pszAppID, 48));

  // Format a string for the AppID's registry key
  TCHAR szKey[_MAX_PATH];
  _stprintf(szKey, TEXT("AppID\\%ls"), pszAppID);

  // Does the AppID exist in the registry?
  LONG lr;
  CRegKey key;
  if (ERROR_SUCCESS != (lr = key.Open(HKEY_CLASSES_ROOT, szKey, samDesired)))
  {
    // Format a string for the CLSID's registry key
    TCHAR szKey[_MAX_PATH];
    _stprintf(szKey, TEXT("CLSID\\%ls"), pszAppID);

    // Open the CLSID's registry key
    if (ERROR_SUCCESS != (lr = key.Open(HKEY_CLASSES_ROOT, szKey, samDesired)))
      return HRESULT_FROM_WIN32(lr);

    // Read the CLSID's AppID value
    TCHAR szAppID[48];
    DWORD cbData = sizeof(szAppID);
    if (ERROR_SUCCESS != (lr = RegQueryValueEx(key, TEXT("AppID"), 0, NULL,
      (BYTE*)szAppID, &cbData)))
        return HRESULT_FROM_WIN32(lr);

    // Copy the AppID String to the [in/out] parameter
    wcscpy(pszAppID, T2CW(szAppID));

    // Recursively call into ourself to resolve and verify the APPID string
    return ResolveAppID(pszAppID, phKey, samDesired);
  }

  // Detach the AppID key from the CRegKey object
  if (phKey)
    *phKey = key.Detach();

  // Indicate success
  return S_OK;
}


HRESULT TCUserAccount::HasRight(LPTSTR pszPrivilege) const
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Fail if Init has not been called successfully
  if (!m_hPolicy || m_spSIDPrincipal.IsNull())
    return E_UNEXPECTED;

  // Fail if the specified pointer is NULL
  if (!pszPrivilege)
    return E_POINTER;

  // Get the array of user rights
  PLSA_UNICODE_STRING pUserRights = NULL;
  ULONG cRights = 0;
  RETURN_FAILED(LsaEnumerateAccountRights(m_hPolicy, m_spSIDPrincipal,
    &pUserRights, &cRights));

  // Get a pointer to a UNICODE version of the specified privilege
  USES_CONVERSION;
  LPCWSTR pszWidePrivilege = T2CW(pszPrivilege);

  // Loop through the array of privileges
  for (ULONG i = 0; i < cRights; ++i)
    if (0 == _wcsicmp(pUserRights[i].Buffer, pszWidePrivilege))
      return S_OK;

  // Specified privilege wasnt' found
  return S_FALSE;
}


HRESULT TCUserAccount::SetRight(LPTSTR pszPrivilege)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Fail if Init has not been called successfully
  if (!m_hPolicy || m_spSIDPrincipal.IsNull())
    return E_UNEXPECTED;

  // Fail if the specified pointer is NULL
  if (!pszPrivilege)
    return E_POINTER;

  // Get a pointer to a UNICODE version of the specified privilege
  USES_CONVERSION;
  LPWSTR pszWidePrivilege = T2W(pszPrivilege);

  // Create an LSA_UNICODE_STRING for the specified privilege name
  LSA_UNICODE_STRING lsaString;
  lsaString.Length        = (USHORT)(wcslen(pszWidePrivilege) * sizeof(WCHAR));
  lsaString.MaximumLength = (USHORT)(lsaString.Length + sizeof(WCHAR));
  lsaString.Buffer        = pszWidePrivilege;

  // Attempt to add the specified privilege to the current user
  RETURN_FAILED(LsaAddAccountRights(m_hPolicy, m_spSIDPrincipal,
    &lsaString, 1));

  // Indicate success
  return S_OK;
}


HRESULT TCUserAccount::RemoveRight(LPTSTR pszPrivilege)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Fail if Init has not been called successfully
  if (!m_hPolicy || m_spSIDPrincipal.IsNull())
    return E_UNEXPECTED;

  // Fail if the specified pointer is NULL
  if (!pszPrivilege)
    return E_POINTER;

  // Get a pointer to a UNICODE version of the specified privilege
  USES_CONVERSION;
  LPWSTR pszWidePrivilege = T2W(pszPrivilege);

  // Create an LSA_UNICODE_STRING for the specified privilege name
  LSA_UNICODE_STRING lsaString;
  lsaString.Length        = (USHORT)(wcslen(pszWidePrivilege) * sizeof(WCHAR));
  lsaString.MaximumLength = (USHORT)(lsaString.Length + sizeof(WCHAR));
  lsaString.Buffer        = pszWidePrivilege;

  // Attempt to remove the specified privilege from the current user
  RETURN_FAILED(LsaRemoveAccountRights(m_hPolicy, m_spSIDPrincipal, false,
    &lsaString, 1));

  // Indicate success
  return S_OK;
}


HRESULT TCUserAccount::SetRunAsInteractiveUser(LPCTSTR szAppID)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Copy the specified AppID string to a non-const wide string
  USES_CONVERSION;
  UINT cchAppID = max(_tcslen(szAppID), 48) + 1;
  LPWSTR pszAppID = (LPWSTR)_alloca(cchAppID * sizeof(WCHAR));
  wcscpy(pszAppID, T2CW(szAppID));

  // Resolve the specified string to an AppID
  HKEY hkey = NULL;
  RETURN_FAILED(ResolveAppID(pszAppID, &hkey));
  CRegKey key;
  key.Attach(hkey);

  // Set "Interactive User" as the RunAs user for the AppID
  // mdvalley: it used to be SetStringValue, but that ain't in my ATL.
  LONG lr = key.SetValue(TEXT("RunAs"), TEXT("Interactive User"));
  if (lr)
    return HRESULT_FROM_WIN32(lr);

  // Indicate success
  return S_OK;
}


HRESULT TCUserAccount::SetRunAsUser(LPCTSTR szAppID, LPCTSTR szPassword)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Fail if Init has not been called successfully
  if (!m_hPolicy || m_spSIDPrincipal.IsNull())
    return E_UNEXPECTED;

  // Concatenate the user name onto the domain name
  USES_CONVERSION;
  UINT cch = wcslen(m_spszDomainName) + wcslen(m_spszUserName) + 2;
  LPTSTR pszUserName = (LPTSTR)_alloca(cch * sizeof(TCHAR));
  _tcscpy(pszUserName, W2CT(m_spszDomainName));
  _tcscat(pszUserName, TEXT("\\"));
  _tcscat(pszUserName, W2CT(m_spszUserName));

  // Delegate to the static method
  return SetRunAsUser(szAppID, pszUserName, szPassword);
}


HRESULT TCUserAccount::SetRunAsUser(LPCTSTR szAppID, LPCTSTR szUserName, LPCTSTR szPassword)
{
  // Not supported under Windows9x
  if (IsWin9x())
    return S_FALSE;

  // Copy the specified AppID string to a non-const wide string
  USES_CONVERSION;
  UINT cchAppID = max(_tcslen(szAppID), 48) + 1;
  LPWSTR pszAppID = (LPWSTR)_alloca(cchAppID * sizeof(WCHAR));
  wcscpy(pszAppID, T2CW(szAppID));

  // Resolve the specified string to an AppID
  HKEY hkey = NULL;
  RETURN_FAILED(ResolveAppID(pszAppID, &hkey));
  CRegKey key;
  key.Attach(hkey);

  // Ensure that we have a domain name
  LPCTSTR pszUserName = szUserName;
  LPCTSTR pszWhack = _tcschr(szUserName, TEXT('\\'));
  if (!pszWhack || pszWhack == szUserName ||
    (pszWhack == (szUserName + 1) && TEXT('.') == *szUserName))
  {
    // Get the domain name and user name
    TCCoTaskPtr<LPTSTR> spszDomainName;
    RETURN_FAILED(GetSID(szUserName, NULL, &spszDomainName));
    LPCTSTR pszUserOnly = pszWhack ? pszWhack + 1 : szUserName;

    // Concatenate the user name onto the domain name
    UINT cch = _tcslen(spszDomainName) + _tcslen(pszUserOnly) + 2;
    LPTSTR pszUserNameTemp = (LPTSTR)_alloca(cch * sizeof(TCHAR));
    _tcscpy(pszUserNameTemp, spszDomainName);
    _tcscat(pszUserNameTemp, TEXT("\\"));
    _tcscat(pszUserNameTemp, pszUserOnly);
    pszUserName = pszUserNameTemp;
  }

  // Open the local security policy
  TCLsaHandle           hPolicy;
  LSA_OBJECT_ATTRIBUTES oa = {sizeof(oa)};
  RETURN_FAILED(LsaOpenPolicy(NULL, &oa, POLICY_CREATE_SECRET, &hPolicy));

  // Format the key string
  WCHAR szKey[48] = L"SCM:";
  wcscat(szKey, pszAppID);

  // Create an LSA_UNICODE_STRING for the key name
  LSA_UNICODE_STRING lsaKeyString;
  lsaKeyString.Length        = (wcslen(szKey) + 1) * sizeof(WCHAR);
  lsaKeyString.MaximumLength = lsaKeyString.Length;
  lsaKeyString.Buffer        = szKey;

  // Copy the specified password string to a non-const wide string
  UINT cchPassword = _tcslen(szPassword);
  LPWSTR pszPassword = (LPWSTR)_alloca((cchPassword + 1) * sizeof(WCHAR));
  wcscpy(pszPassword, T2CW(szPassword));

  // Create an LSA_UNICODE_STRING for the password string
  LSA_UNICODE_STRING lsaPasswordString;
  lsaPasswordString.Length        = (cchPassword + 1) * sizeof(WCHAR);
  lsaPasswordString.MaximumLength = lsaPasswordString.Length;
  lsaPasswordString.Buffer        = pszPassword;

  // Store the specified password
  RETURN_FAILED(LsaStorePrivateData(hPolicy, &lsaKeyString,
    &lsaPasswordString));

  // Set the specified user name as the RunAs user for the AppID
  // mdvalley: Another former SetStringValue
  LONG lr = key.SetValue(TEXT("RunAs"), pszUserName);
  if (lr)
    return HRESULT_FROM_WIN32(lr);

  // Indicate success
  return S_OK;
}

