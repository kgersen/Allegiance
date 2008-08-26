/////////////////////////////////////////////////////////////////////////////
// pch.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma warning (disable:4786)  // disable 'identifier was truncated to '255'...' warning

#if _MSC_VER >= 1000
  #pragma once
#endif // _MSC_VER >= 1000

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0400
#endif

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM         // Enable DCOM features
#define __MODULE__ "TCDeploy"

// Win32 headers
#include <windows.h>        // Win32 declarations
#include <psapi.h>

// CRT headers
#include <crtdbg.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

// STL headers
#include <algorithm>
#include <map>
#include <set>
#include <vector>

// ATL headers
#include <atlbase.h>        // ATL base declarations
#include <..\TCAtl\ComModule.h>
class CDllModule : public TCComModule<CDllModule>
{
// Command-line Option Map
public:
  BEGIN_CMDLINE_OPTION_MAP()
    CMDLINE_OPTION_RegServer9x()
    CMDLINE_OPTION_RegServerNT()
    CMDLINE_OPTION_UnregServer()
    CMDLINE_OPTION_Help()
//    CMDLINE_OPTION_NoExit()
    CMDLINE_OPTION_ENTRY_EX("NoExit", NULL, OnNoExit, 0, 0, true, false, true, true)                                                        
    CMDLINE_OPTION_Exit()
    CMDLINE_OPTION_ENTRY_EX("Install", "Startup Group Installation",
      OnInstall, 0, 0, false, true, true, false)
  END_CMDLINE_OPTION_MAP()

// Operations
public:
  HRESULT OnNoExit(int argc, TCHAR* argv[], int* pnArgs);
  HRESULT OnExit(int argc, TCHAR* argv[], int* pnArgs);
  HRESULT OnInstall(int argc, TCHAR* argv[], int* pnArgs);

// Overrides
public:
  HRESULT GetAccessSecurity(PSECURITY_DESCRIPTOR* psd, DWORD* pcb);
  void    DisplaySyntax();
  LPCTSTR GetRegInfo(UINT& idResource, bool& bRegTypeLib);
};
extern CDllModule _Module;
#include <atlcom.h>         // ATL component object classes
#include <atlwin.h>         // ATL window classes

#include <TCLib.h>
#include <..\TCLib\AutoCriticalSection.h>
#include <..\TCLib\ObjectLock.h>
#include <..\TCLib\CoInit.h>
#include <..\TCLib\TCThread.h>
#include <..\TCLib\AutoHandle.h>
#include <..\TCLib\UtilImpl.h>
#include <TCAtl.h>
#include <..\TCAtl\PropertyClass.h>
#include <..\TCAtl\TCNullStreamImpl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\DualEventsCP.h>

#include <ZLib.h>
