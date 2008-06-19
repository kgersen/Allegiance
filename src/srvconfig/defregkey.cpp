// DefRegKey.cpp : implementation file
// Imago updated for new ATL

#include "stdafx.h"

LONG CDefaultRegKey::SetValue(DWORD dwValue, LPCTSTR lpszValueName, DWORD dwValueDefault)
{
    if (dwValue == dwValueDefault)
        return CRegKey::DeleteValue(lpszValueName);
    else
        return CRegKey::SetDWORDValue(lpszValueName, dwValue);
}

LONG CDefaultRegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName, LPCTSTR lpszValueDefault)
{
    if (lstrcmpi(lpszValue, lpszValueDefault))
        return CRegKey::SetStringValue(lpszValueName, lpszValue);
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

