#ifndef __TCDeploymentVersion_h__
#define __TCDeploymentVersion_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// TCDeploymentVersion.h: Declaration of the TCDeploymentVersion class.
//

#include "TCDeploy.h"
#include "resource.h"
#include <..\TCAtl\VersionInfoImpl.h>
#include <objsafe.h>


/////////////////////////////////////////////////////////////////////////////
// TCDeploymentVersion
//
class TCDeploymentVersion :
  public TCVersionInfoImpl<TCDeploymentVersion, ITCDeploymentVersion, &LIBID_TCDeployLib>, 
  public CComCoClass<TCDeploymentVersion, &CLSID_TCDeploymentVersion>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCDeploymentVersion)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(TCDeploymentVersion)
    IMPLEMENTED_CATEGORY(CATID_TCDeploy)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(TCDeploymentVersion)
    COM_INTERFACE_ENTRY(ITCDeploymentVersion)
    COM_INTERFACE_ENTRIES_TCVersionInfoImpl()
  END_COM_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCDeploymentVersion_h__
