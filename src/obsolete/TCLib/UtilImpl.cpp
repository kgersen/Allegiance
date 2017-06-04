/////////////////////////////////////////////////////////////////////////////
// UtilImpl.cpp : Implementation of the TCUtilImpl class.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "AutoHandle.h"
#include "UtilImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Function Prototypes

// Hasn't made it into the Visual C++ objbase.h yet, but the binaries have it
// under Win98, NT4:SP4, and DCOM95.
WINOLEAPI CreateObjrefMoniker(LPUNKNOWN pUnk, LPMONIKER* ppMk);


/////////////////////////////////////////////////////////////////////////////
// TCUtilImpl


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

STDMETHODIMP TCUtilImpl::CreateObject(BSTR bstrProgID, BSTR bstrComputer,
  IUnknown** ppUnk)
{
  // Initialize the [out] parameter
  CLEAROUT(ppUnk, (IUnknown*)NULL);

  // Convert the specified ProgID to a CLSID
  CLSID clsid;
  if (!BSTRLen(bstrProgID))
    return E_INVALIDARG;
  RETURN_FAILED(CLSIDFromProgID(bstrProgID, &clsid));

  // Initialize the COSERVERINFO and MULTI_QI structures
  COSERVERINFO csi   = {0, bstrComputer, NULL, 0};
  MULTI_QI     mqi[] =
  {
    {&IID_IUnknown                 , NULL, S_OK},
    {&IID_IDispatch                , NULL, S_OK},
    {&IID_IConnectionPointContainer, NULL, S_OK},
  };
  const static ULONG cMQI = sizeofArray(mqi);

  // Determine if the specified computer name is definitely local
  bool bLocalForSure = true;
  if (BSTRLen(bstrComputer))
  {
    TCHAR szLocal[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD cchLocal = sizeofArray(szLocal);
    GetComputerName(szLocal, &cchLocal);
    USES_CONVERSION;
    bLocalForSure = !_tcsicmp(szLocal, OLE2CT(bstrComputer));
  }
  DWORD dwClsCtx = bLocalForSure ? CLSCTX_SERVER : CLSCTX_REMOTE_SERVER;

  // Attempt to create the instance
  HRESULT hr = CoCreateInstanceEx(clsid, NULL, dwClsCtx, &csi, cMQI, mqi);
  if (FAILED(hr))
  {
    _TRACE_BEGIN
      _TRACE_PART1("TCCreateObject failed: hr=0x%08X", hr);
      _TRACE_PART1(", dwClsCtx=%hs", (CLSCTX_SERVER == dwClsCtx) ?
        "CLSCTX_SERVER" : "CLSCTX_REMOTE_SERVER");
      _TRACE_PART1(", ProgID=\"%ls\"", bstrProgID);
      _TRACE_PART1(", Computer= \"%ls\"\n", bstrComputer ?
        bstrComputer : L"");
    _TRACE_END
    return hr;
  }

  // Copy the IUnknown interface pointer to the [out] parameter
  *ppUnk = mqi[0].pItf;

  // Release each interface not being returned
  for (ULONG i = 1; i < cMQI; ++i)
    if (mqi[i].pItf)
      mqi[i].pItf->Release();

  // Return the result of the QI for IUnknown
  return mqi[0].hr;
}


STDMETHODIMP TCUtilImpl::ObjectReference(IUnknown* pUnk,
  BSTR* pbstrObjectReference)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstrObjectReference, (BSTR)NULL);

  // Create the object reference moniker
  IMonikerPtr spMoniker;
  RETURN_FAILED(CreateObjrefMoniker(pUnk, &spMoniker));

  // Create a binding context
  IBindCtxPtr spbc;
  RETURN_FAILED(CreateBindCtx(0, &spbc));

  // Get the displayable name of the moniker
  TCCoTaskPtr<LPOLESTR> spszDisplayName;
  RETURN_FAILED(spMoniker->GetDisplayName(spbc, NULL, &spszDisplayName));

  // Create a BSTR from the display name
  *pbstrObjectReference = SysAllocString(spszDisplayName);

  // Indicate success or failure
  return *pbstrObjectReference ? S_OK : E_OUTOFMEMORY;
}


STDMETHODIMP TCUtilImpl::Sleep(long nDurationMS)
{
  // Copied from WSH
  const long lTimerGranularity = 1000;
  double StartTickCount;
  double CurTickCount;
  double EndTickCount;
  DWORD CurWaitTime;
  MSG msg;

  if (nDurationMS < 0)
    return E_INVALIDARG;

  StartTickCount = ::GetTickCount();
  EndTickCount = StartTickCount + nDurationMS;
  CurTickCount = StartTickCount;

  // WARNING: The tick count wraps around every 49 days. We do NOT
  // want to have an unreproducible bug that causes the task to hang
  // for 49 days!  That's why we're doing this in doubles, not
  // integers.

  while(CurTickCount < EndTickCount)
  {
    CurWaitTime = (DWORD)(EndTickCount - CurTickCount);
    if (CurWaitTime > lTimerGranularity)
      CurWaitTime = lTimerGranularity;

    // WARNING:  We call ::MsgWaitForMultipleObjects here instead of
    // ::Sleep for a good reason -- COM will have created windows 
    // message loops to handle Asynchronous Procedure Calls -- like
    // events thrown by connection points that we may have advised.
    // We can't just put the process to sleep -- all the messages
    // will just queue up, which could be very bad if this sleeps for
    // a long time.

    DWORD dwWait = ::MsgWaitForMultipleObjects(0, NULL, TRUE, CurWaitTime, QS_ALLINPUT);

    // See if we woke up because of a message:
    if (WAIT_TIMEOUT != dwWait && ::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      ::TranslateMessage(&msg);
      ::DispatchMessage(&msg);
    }

    CurTickCount = ::GetTickCount();
    if (CurTickCount < StartTickCount)
    {
      // The tick count has recently wrapped around.
      // Note that we can assume that it can only wrap around
      // once, as this function can only run at most 25 days.
      // (The argument is a signed value.)
      CurTickCount += (DWORD)(0xFFFFFFFF);
    }
  }

  return NOERROR;
}


STDMETHODIMP TCUtilImpl::GetObject(BSTR bstrMoniker, VARIANT_BOOL bAllowUI,
  long nTimeoutMS, IUnknown** ppUnk)
{
  // Create the BIND_OPTS structure
  BIND_OPTS bo;
  bo.cbStruct = sizeof(bo);
  bo.grfFlags = bAllowUI ? BIND_MAYBOTHERUSER : 0;
  bo.grfMode  = STGM_READWRITE;
  if (-1 == nTimeoutMS)
    bo.dwTickCountDeadline = 0;
  else
  {
    bo.dwTickCountDeadline = GetTickCount() + nTimeoutMS;
    if (0 == bo.dwTickCountDeadline)
      ++bo.dwTickCountDeadline;
  }

  // Use CoGetObject to parse and bind the moniker
  return CoGetObject(bstrMoniker, &bo, IID_IUnknown, (void**)ppUnk);  
}

