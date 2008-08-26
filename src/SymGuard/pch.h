/////////////////////////////////////////////////////////////////////////////
// pch.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once


/////////////////////////////////////////////////////////////////////////////
// Standard C/C++ Includes

#pragma warning(disable:4786)

#include <crtdbg.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <wchar.h>
#include <map>


/////////////////////////////////////////////////////////////////////////////
// Win32 Includes

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM         // Enable DCOM features
#include <windows.h>        // Win32 declarations
#include <tchar.h>
#include <dbghelp.h>
#include <objsafe.h>
#include <comdef.h>


/////////////////////////////////////////////////////////////////////////////
// ATL Includes

#include <atlbase.h>        // ATL base declarations
extern CComModule _Module;
#include <atlcom.h>         // ATL component object classes


/////////////////////////////////////////////////////////////////////////////
// ZLib Includes

#define __MODULE__ "SymGuard"
#define ZLIB_SKIP_NEW_REDEF
#include <zlib.h>
#include <zreg.h>
#include <FixDelete.h>


/////////////////////////////////////////////////////////////////////////////
// TCLib Includes

#include <TCLib.h>
#include <..\TCLib\AutoHandle.h>
#include <..\TCLib\AutoCriticalSection.h>
#include <..\TCLib\ObjectLock.h>
#include <..\TCLib\RelPath.h>
//#include <..\TCLib\CoInit.h>
//#include <..\TCLib\TCThread.h>


/////////////////////////////////////////////////////////////////////////////
// TCAtl Includes

#include <TCAtl.h>
#include <..\TCAtl\TCNullStreamImpl.h>
#include <..\TCAtl\ObjectMap.h>

