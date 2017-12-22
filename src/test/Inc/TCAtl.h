#ifndef __TCAtl_h__
#define __TCAtl_h__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// TCAtl.h | Declarations and definitions of extensions to Microsoft's
// Active Template Library (ATL) framework.  These declarations and
// definitions are to be independent from the Microsoft Foundation Class
// (MFC) framework, but are also to be compatible with it.
//
// The files in this library are intended to be used in much the same way as
// the files in the ATL framework.  The TCAtl.h file should be included
// from a project's pch.h file (or the functional equivalent), while the
// TCAtl.cpp file should be included by ONLY ONE module in the project.
// Usually this will be the pch.cpp file (or the functional equivalent).
//

// Disable this warning since ATL may generate some very long symbolic names
#pragma warning(disable: 4786)

#if !defined(_INC_COMDEF)
  #include <comdef.h>
#endif // !defined(_INC_COMDEF)

#ifndef __ATLCTL_H__
  #include <atlctl.h>
#endif // !__ATLCTL_H__

#ifdef _AFX
  #ifndef __AFXOLE_H__
    #include <afxole.h>
  #endif // !__AFXOLE_H__
#endif // _AFX

#ifndef __TCLib_h__
  #include <TCLib.h>
#endif // !__TCLib_h__


/////////////////////////////////////////////////////////////////////////////
// Macros


/////////////////////////////////////////////////////////////////////////////
// IID Interpretation

LPCTSTR TCGetIIDName(REFIID riid);


/////////////////////////////////////////////////////////////////////////////
// SafeArray Streaming

HRESULT TCReadSafeArrayFromStream(IStream* pstm, CComVariant& var);
HRESULT TCWriteSafeArrayToStream(IStream* pstm, CComVariant& var);


/////////////////////////////////////////////////////////////////////////////
// Object Stream Size Determination

HRESULT TCGetPersistStreamSize(IUnknown* punk, ULARGE_INTEGER* pCbSize);


/////////////////////////////////////////////////////////////////////////////

#endif // __TCAtl_h__
