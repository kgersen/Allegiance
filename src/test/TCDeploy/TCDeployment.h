/////////////////////////////////////////////////////////////////////////////
// TCDeployment.h: Definition of the TCDeployment class
//

#ifndef __TCDeployment_h__
#define __TCDeployment_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#include "TCDeploy.h"
#include "resource.h"       // main symbols

#include <objsafe.h>


/////////////////////////////////////////////////////////////////////////////
// TCDeployment
//
class TCDeployment : 
	public IDispatchImpl<ITCDeployment, &IID_ITCDeployment, &LIBID_TCDeployLib>, 
	public ISupportErrorInfo,
  public ITCDeploymentClass,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<TCDeployment,&CLSID_TCDeployment>
{
// Declarations
public:
  DECLARE_CLASSFACTORY_SINGLETON(TCDeployment)
  DECLARE_REGISTRY_RESOURCEID(IDR_TCDeployment)

// Category Map
public:
  BEGIN_CATEGORY_MAP(TCDeployment)
    IMPLEMENTED_CATEGORY(CATID_TCDeploy)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(TCDeployment)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(ITCDeployment)
	  COM_INTERFACE_ENTRY(ITCDeploymentQuit)
	  COM_INTERFACE_ENTRY_FUNC(IID_ITCDeploymentClass, 0, QI_TCDeploymentClass)
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Implementation
public:
  static HRESULT CreateTheProcess(BSTR bstrCmdLine, BSTR bstrDirectory,
    BSTR bstrStdOut, BSTR bstrStdErr, BSTR bstrStdIn, ShowCmd eShowCmd,
    PROCESS_INFORMATION* pProcessInfo);
protected:
  static HRESULT WINAPI QI_TCDeploymentClass(void* pv, REFIID riid,
    LPVOID* ppv, DWORD dw);
  static HRESULT RegUnregServer(BSTR bstrModule, BSTR* pbstrResult,
    LPSTR pszExport);

// ITCDeploymentQuit Interface Methods
public:
  STDMETHODIMP Quit();

// ITCDeploymentClass Interface Methods
public:
  STDMETHODIMP put_NoExit(VARIANT_BOOL bNoExit);
  STDMETHODIMP get_NoExit(VARIANT_BOOL* pbNoExit);

// ISupportsErrorInfo Interface Methods
public:
	STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// ITCDeployment Interface Methods
public:
  STDMETHODIMP CreateObject(BSTR bstrProgID, BSTR bstrComputer,
    IUnknown** ppUnk);
  STDMETHODIMP ObjectReference(IUnknown* pUnk, BSTR* pbstrObjectReference);
  STDMETHODIMP Sleep(long nDurationMS);
  STDMETHODIMP ExecuteAndWait(BSTR bstrCmdLine, BSTR bstrDirectory,
    BSTR bstrStdOut, BSTR bstrStdErr, BSTR bstrStdIn, ShowCmd eShowCmd,
    long* pnExitCode);
  STDMETHODIMP Execute(BSTR bstrCmdLine, BSTR bstrDirectory,
    BSTR bstrStdOut, BSTR bstrStdErr, BSTR bstrStdIn, ShowCmd eShowCmd,
    ITCDeploymentProcess** ppProcess);
  STDMETHODIMP RegServer(BSTR bstrModule, BSTR* pbstrResult);
  STDMETHODIMP UnregServer(BSTR bstrModule, BSTR* pbstrResult);
  STDMETHODIMP get_Version(ITCDeploymentVersion** ppVersion);
  STDMETHODIMP GetObject(BSTR bstrMoniker, VARIANT_BOOL bAllowUI,
    long nTimeoutMS, IUnknown** ppUnk);
  STDMETHODIMP WriteStreamToFile(BSTR bstrFileName, VARIANT* pvarStream,
    VARIANT_BOOL bOverwrite);
  STDMETHODIMP GetProcessID(BSTR bstrName, long* pdwProcessID);

// Types
protected:
  class XWaitThreadArgs;
  typedef TCObjectLock<TCAutoCriticalSection> XLockStatic;

// Data Members
protected:
  static TCAutoCriticalSection m_csCurDir;
  static TCAutoCriticalSection m_csStatic;
  static bool                  m_bNoExit;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCDeployment_h__
