/////////////////////////////////////////////////////////////////////////////
// RelPath.cpp : Imlementation of relative pathname functions.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "RelPath.h"
#include "StrManip.h"


/////////////////////////////////////////////////////////////////////////////
// Description: Creates a relative path name, if possible.
//
// Parameters:
//   bBeginWithDot - Prepends the relative path with the a period and
// backslash. While possibly rendundant, this may be useful for some contexts
// of relative pathnames. If a relative path cannot be resolved, this
// parameter is ignored.
//
bool TCMakeRelativePath(LPCTSTR pszPath, LPCTSTR pszFrom, LPTSTR pszDest,
  int cchMaxDest, bool bBeginWithDot)
{
  // Ensure that input paths are absolute
  TCHAR szPathCopy[_MAX_PATH], szFromCopy[_MAX_PATH];
  if (!_tfullpath(szPathCopy, pszPath, sizeofArray(szPathCopy)))
    return false;
  if (!_tfullpath(szFromCopy, pszFrom, sizeofArray(szFromCopy)))
    return false;

  // Replace (in-place) forward slashes with backslashes in input paths
  TCReplaceText(szPathCopy, TEXT("/"), TEXT("\\"), szPathCopy, -1);
  TCReplaceText(szFromCopy, TEXT("/"), TEXT("\\"), szFromCopy, -1);

  // The From path MUST be a directory, not a filename, so append backslash
  if (TEXT('\\') != szFromCopy[_tcslen(szFromCopy) - 1])
    _tcscat(szFromCopy, TEXT("\\"));

  // Use COM monikers to create a relative path
  HRESULT hr;
  IBindCtxPtr pbc;
  IMonikerPtr pmkFrom, pmkPath, pmkRelative;
  LPOLESTR pszRelative = NULL;
  if (SUCCEEDED(hr = CreateFileMoniker(_bstr_t(szFromCopy), &pmkFrom))
    && SUCCEEDED(hr = CreateFileMoniker(_bstr_t(szPathCopy), &pmkPath))
    && S_OK == (hr = pmkFrom->RelativePathTo(pmkPath, &pmkRelative))
    && SUCCEEDED(hr = CreateBindCtx(0, &pbc))
    && SUCCEEDED(hr = pmkRelative->GetDisplayName(pbc, NULL, &pszRelative)))
  {
    if (bBeginWithDot)
    {
      TC_tcscpyn(pszDest, TEXT(".\\"), cchMaxDest);
      cchMaxDest -= 2;
      pszDest += 2;
    }
    TC_tcscpyn(pszDest, _bstr_t(pszRelative), cchMaxDest);
    CoTaskMemFree(pszRelative);
    return true;
  }

  // Could not make a relative path, copy the absolute path
  TC_tcscpyn(pszDest, szPathCopy, cchMaxDest);
  return false;
}

