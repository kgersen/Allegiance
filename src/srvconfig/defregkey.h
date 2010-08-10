#ifndef _DEFREGKEY_H
#define _DEFREGKEY_H

#ifndef __ATLBASE_H__
#include <atlbase.h>
#endif // __ATLBASE_H__

class CDefaultRegKey : public CRegKey
{
public:
    CDefaultRegKey() {}
    ~CDefaultRegKey();

	LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName, DWORD dwValueDefault);
	LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName, LPCTSTR lpszValueDefault);
	LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName, LPCTSTR lpszValueDefault);
};

inline CDefaultRegKey::~CDefaultRegKey()
{
    Close();
}

#endif // _DEFREGKEY_H
