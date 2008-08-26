#pragma once

/////////////////////////////////////////////////////////////////////////////
// ToolHelp.h: Declaration of the CToolHelp class.
//

#include <..\TCLib\DynaLib.h>
#include <tlhelp32.h>


/////////////////////////////////////////////////////////////////////////////
//
class CToolHelp : public TCDynaLib
{
// Construction
public:
  CToolHelp() : TCDynaLib(TEXT("kernel32.dll")) {}

// Library Functions
public:
  TCDynaLib_Fn2(HANDLE, CreateToolhelp32Snapshot, DWORD, DWORD)
  TCDynaLib_Fn2(BOOL, Process32First,HANDLE, LPPROCESSENTRY32)
  TCDynaLib_Fn2(BOOL, Process32Next,HANDLE, LPPROCESSENTRY32)
  #if defined(UNICODE) || defined(_UNICODE)
    TCDynaLib_Fn3(DWORD, GetLongPathNameW, LPCTSTR, LPTSTR, DWORD)
    bool Exists_GetLongPathName() {return Exists_GetLongPathNameW();}
  #else
    TCDynaLib_Fn3(DWORD, GetLongPathNameA, LPCTSTR, LPTSTR, DWORD)
    bool Exists_GetLongPathName() {return Exists_GetLongPathNameA();}
  #endif

// Required Function Map
public:
  TCDynaLib_BEGIN_REQUIRED_MAP()
    TCDynaLib_REQUIRED_FUNCTION(CreateToolhelp32Snapshot)
    TCDynaLib_REQUIRED_FUNCTION(Process32First)
    TCDynaLib_REQUIRED_FUNCTION(Process32Next)
  TCDynaLib_END_REQUIRED_MAP()
};


