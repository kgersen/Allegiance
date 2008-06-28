// DefRegKey.cpp : implementation file
//

#include "stdafx.h"

LONG CDefaultRegKey::SetValue(DWORD dwValue, LPCTSTR lpszValueName, DWORD dwValueDefault)
{
    if (dwValue == dwValueDefault)
        return CRegKey::DeleteValue(lpszValueName);
    else
        return CRegKey::SetValue(dwValue, lpszValueName);
}

LONG CDefaultRegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName, LPCTSTR lpszValueDefault)
{
    if (lstrcmpi(lpszValue, lpszValueDefault))
        return CRegKey::SetValue(lpszValue, lpszValueName);
    else
        return CRegKey::DeleteValue(lpszValueName);
}

LONG CDefaultRegKey::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName, LPCTSTR lpszValueDefault)
{
    if (0 == lstrcmpi(lpszValue, lpszValueDefault))
        return CRegKey::DeleteValue(lpszValueName);
    else
        return CRegKey::SetKeyValue(lpszKeyName, lpszValue, lpszValueName);
}

