/////////////////////////////////////////////////////////////////////////////
// VersionInfo.cpp : Implementation of the ZVersionInfo class.
//

#include "VersionInfo.h"

/////////////////////////////////////////////////////////////////////////////
// ZVersionInfo


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

ZVersionInfo::ZVersionInfo(bool) :
  m_pVerInfo(nullptr),
  m_cbVerInfo(0),
  m_pFixed(nullptr),
  m_wLangID(-1),
  m_wCodePage(-1)
{
}

ZVersionInfo::ZVersionInfo(LPCTSTR szModule) :
  m_pVerInfo(nullptr),
  m_cbVerInfo(0),
  m_pFixed(nullptr),
  m_wLangID(-1),
  m_wCodePage(-1)
{
  HRESULT hr = Load(szModule);
  assert(SUCCEEDED(hr));
}

ZVersionInfo::ZVersionInfo(HINSTANCE hinstance) :
  m_pVerInfo(nullptr),
  m_cbVerInfo(0),
  m_pFixed(nullptr),
  m_wLangID(-1),
  m_wCodePage(-1)
{
  HRESULT hr = Load(hinstance);
  assert(SUCCEEDED(hr));
}

ZVersionInfo::~ZVersionInfo()
{
  // Ensure that there is no previous version information block allocated
  Unload();
}

bool ZVersionInfo::Load(LPCTSTR szModule)
{
  // Check for a nullptr or empty string
  if (!szModule || TEXT('\0') == *szModule)
    return Load(HINSTANCE(nullptr));

  // Ensure that there is no previous version information block allocated
  Unload();
  assert(!m_cbVerInfo);
  assert(!m_pVerInfo);
  assert(!m_pFixed);

  // Get the size of the version information of the specified module
  uint8_t* pVerInfo = nullptr;
  uint32_t cbVerInfo, dummy;
  cbVerInfo = GetFileVersionInfoSize(const_cast<LPTSTR>(szModule), LPDWORD(&dummy));
  if (cbVerInfo)
  {
    // Allocate space to hold the version information
    pVerInfo = new uint8_t[cbVerInfo];
    if (!pVerInfo)
    {
      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
      return false;
    }
  }
  else
    cbVerInfo = sizeof(*m_pFixed);

  // Attempt to load the version information block
  if (!GetFileVersionInfo(const_cast<LPTSTR>(szModule), 0, cbVerInfo, pVerInfo))
  {
    uint32_t dwLastError = GetLastError();
    delete [] pVerInfo;
    SetLastError(dwLastError ? dwLastError : ERROR_NO_MORE_ITEMS);
    return false;
  }

  // Attempt to get a pointer to the fixed version information
  VS_FIXEDFILEINFO* pFixed = nullptr;
  UINT cbFixed = 0;
  if (!VerQueryValue(pVerInfo, TEXT("\\"), (void**)&pFixed, &cbFixed) || cbFixed != sizeof(*pFixed))
  {
    uint32_t dwLastError = GetLastError();
    delete [] pVerInfo;
    SetLastError(dwLastError);
    return false;
  }

  // Save the size of the version info and the allocated pointers
  m_cbVerInfo = cbVerInfo;
  m_pVerInfo  = pVerInfo;
  m_pFixed    = pFixed;

  // Indicate success
  SetLastError(0);
  return true;
}

bool ZVersionInfo::Load(HINSTANCE hinstance)
{
  // Get the module name of the specified instance
  TCHAR szModule[_MAX_PATH];
  if (!GetModuleFileName(hinstance, szModule, sizeofArray(szModule)))
    return false;

  // Pass on to the string overload of this function
  return Load(szModule);
}

bool ZVersionInfo::Load(const void* pvVerInfo, UINT cbVerInfo)
{
  // Ensure that a nullptr was not passed
  if (!pvVerInfo)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  // Ensure that there is no previous version information block allocated
  Unload();

  // Allocate space to hold the version information
  uint8_t* pVerInfo = new uint8_t[cbVerInfo];
  if (!pVerInfo)
  {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    return false;
  }

  // Copy the specified block
  CopyMemory(pVerInfo, pvVerInfo, cbVerInfo);

  // Attempt to get a pointer to the fixed version information
  VS_FIXEDFILEINFO* pFixed = nullptr;
  UINT cbFixed = 0;
  if (!VerQueryValue(pVerInfo, TEXT("\\"), (void**)&pFixed, &cbFixed) || cbFixed != sizeof(*pFixed))
  {
    uint32_t dwLastError = GetLastError();
    delete [] pVerInfo;
    SetLastError(dwLastError ? dwLastError : ERROR_NO_MORE_ITEMS);
    return false;
  }

  // Save the size of the version info and the allocated and fixed pointers
  m_cbVerInfo = cbVerInfo;
  m_pVerInfo  = pVerInfo;
  m_pFixed    = pFixed;

  // Indicate success
  SetLastError(0);
  return true;
}

void ZVersionInfo::Unload()
{
  // Deallocate the version information block
  if (m_pVerInfo)
    delete m_pVerInfo;

  m_pVerInfo  = nullptr;
  m_cbVerInfo = 0;
  m_pFixed    = nullptr;
}


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// String Values

ZString ZVersionInfo::GetStringValue(LPCTSTR pszKey, bool* pbExists) const
{
  // Initialize the [out] parameter
  if (pbExists)
    *pbExists = false;

  // Ensure that we have a non-nullptr key
  if (!pszKey)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return ZString();
  }

  // Determine if a LANGID was specified
  LANGID wLangID = LANGID((LANGID(-1) == GetLanguageID()) ?
    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) : GetLanguageID());

  // Determine if a code page was specified
  uint16_t rgwCodePages[] =
  {
    1200,  // Unicode
    1252,  // Windows Multilingual
    0000,  // Neutral
  };
  int cCodePages = sizeofArray(rgwCodePages);
  if (uint16_t(-1) != m_wCodePage)
  {
    rgwCodePages[0] = m_wCodePage;
    cCodePages = 1;
  }

  // Format the base sub-block string
  TCHAR szBase[32];
  _stprintf_s(szBase, 32, TEXT("\\StringFileInfo\\%04X"), wLangID);

  // Loop thru each code page
  for (int iCP = 0; iCP < cCodePages; ++iCP)
  {
    // Format a sub-block string
    TCHAR szSubBlock[_MAX_PATH * 2];
    _stprintf_s(szSubBlock, _MAX_PATH * 2, TEXT("%s%04X\\%s"), szBase, rgwCodePages[iCP], pszKey);

    // Query the value
    UINT cbValue = 0;
    LPCTSTR pszValue = nullptr;
    if (m_pVerInfo != nullptr && VerQueryValue(m_pVerInfo, szSubBlock, (void**)&pszValue, &cbValue))
    {
      // Indicate that the key exists
      if (pbExists)
        *pbExists = true;

      // Indicate success
      SetLastError(0);
      return ZString(pszValue);
    }
  }

  // Indicate success
  SetLastError(0);
  return ZString();  
}

