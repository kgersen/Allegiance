#ifndef __SymGuardVersion_h__
#define __SymGuardVersion_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// SymGuardVersion.h: Declaration of the CSymGuardVersion class.
//

#include <SymGuard.h>
#include "resource.h"
#include <..\TCAtl\VersionInfoImpl.h>


/////////////////////////////////////////////////////////////////////////////
// SymGuardVersion
//
class CSymGuardVersion :
  public TCVersionInfoImpl<CSymGuardVersion, ISymGuardVersion, &LIBID_SymGuardLib>,
  public CComCoClass<CSymGuardVersion, &CLSID_SymGuardVersion>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_SymGuardVersion)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CSymGuardVersion)
    IMPLEMENTED_CATEGORY(CATID_SymGuard)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CSymGuardVersion)
    COM_INTERFACE_ENTRY(ISymGuardVersion)
    COM_INTERFACE_ENTRIES_TCVersionInfoImpl()
  END_COM_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__SymGuardVersion_h__
