/////////////////////////////////////////////////////////////////////////////
// TCDeploymentProcess.h: Definition of the TCDeploymentProcess class
//

#ifndef __TCDeploymentProcess_h__
#define __TCDeploymentProcess_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#include "TCDeploy.h"
#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// TCDeploymentProcess
//
class TCDeploymentProcess : 
	public IDispatchImpl<ITCDeploymentProcess, &IID_ITCDeploymentProcess, &LIBID_TCDeployLib>, 
	public CComObjectRoot,
	public CComCoClass<TCDeploymentProcess,&CLSID_TCDeploymentProcess>
{
// Declarations
public:
  //DECLARE_NOT_AGGREGATABLE(TCDeploymentProcess) 
  DECLARE_REGISTRY_RESOURCEID(IDR_TCDeploymentProcess)

// Category Map
public:
  BEGIN_CATEGORY_MAP(TCDeploymentProcess)
    IMPLEMENTED_CATEGORY(CATID_TCDeploy)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(TCDeploymentProcess)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(ITCDeploymentProcess)
  END_COM_MAP()

// Construction
public:
	TCDeploymentProcess() {}
  void Init(HANDLE hProcess)
  {
    assert(hProcess);
    m_shProcess = hProcess;
  }

// Attributes
public:
  HANDLE GetHandle() const
  {
    return m_shProcess;
  }

// ITCDeploymentProcess Interface Methods
public:
  STDMETHODIMP get_IsCompleted(VARIANT_BOOL* pbCompleted);
  STDMETHODIMP get_ExitCode(long* pnExitCode);

// Data Members
protected:
  TCHandle m_shProcess;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCDeploymentProcess_h__
