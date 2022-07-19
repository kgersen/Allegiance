#ifndef __TCNullStream_h__
#define __TCNullStream_h__

/////////////////////////////////////////////////////////////////////////////
// TCNullStream.h | Declaration of the CTCNullStream, which implements the
// CLSID_TCNullStream component object.

#include "resource.h"       // main symbols
#include "SrcInc.h"
#include <TCObj.h>

#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// CTCNullStream

class ATL_NO_VTABLE CTCNullStream :
  public CTCNullStreamImpl,
  public CComCoClass<CTCNullStream, &CLSID_TCNullStream>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCNullStream)

// Category Map
public:
  BEGIN_CATEGORY_MAP(CTCNullStream)
    IMPLEMENTED_CATEGORY(CATID_TCObj)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CTCNullStream)
   COM_INTERFACE_ENTRY(IUnknown)
   COM_INTERFACE_ENTRY_CHAIN(CTCNullStreamImpl)
  END_COM_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCNullStream_h__

