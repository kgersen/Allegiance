#ifndef __IActiveScriptSiteImpl_h__
#define __IActiveScriptSiteImpl_h__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// IActiveScriptSiteImpl.h : Declaration of the IActiveScriptSiteImpl class.
//

#include <AGC.h>
#include <activscp.h>
#include <TCLib.h>
#include <..\TCLib\ExcepInfo.h>
#include <..\TCLib\ObjectLock.h>
#include <..\TCLib\tstring.h>


/////////////////////////////////////////////////////////////////////////////
// IActiveScriptSiteImpl
//
template <class T>
class IActiveScriptSiteImpl : public IActiveScriptSite
{
// Types
protected:
  typedef TCObjectLock<T> XTLock;
  typedef IActiveScriptSiteImpl<T> IActiveScriptSiteImplBase;

// Attributes
public:
  void SetScriptFileName(tstring strFileName)
  {
    XTLock lock(static_cast<T*>(this));
    m_strScriptFileName = strFileName;

    // Use the file name as the short name
    TCHAR szName[_MAX_FNAME];
    _tsplitpath(strFileName.c_str(), NULL, NULL, szName, NULL);
    m_strScriptShortName = szName;
  }
  tstring GetScriptFileName()
  {
    XTLock lock(static_cast<T*>(this));
    return m_strScriptFileName;
  }
  tstring GetScriptShortName()
  {
    XTLock lock(static_cast<T*>(this));
    return m_strScriptShortName;
  }

// IActiveScriptSite Interface Methods
public:
  STDMETHODIMP GetLCID(LCID* plcid)
  {
    CLEAROUT(plcid, GetThreadLocale());
    return S_OK;
  }

  STDMETHODIMP GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask,
    IUnknown** ppunkItem, ITypeInfo** ppti)
  {
    return TYPE_E_ELEMENTNOTFOUND;
  }

  STDMETHODIMP GetDocVersionString(BSTR* pbstrVersion)
  {
    return E_NOTIMPL;
  }

  STDMETHODIMP OnScriptTerminate(const VARIANT* pvarResult,
    const EXCEPINFO* pei)
  {
    #if 0 // #ifdef _DEBUG
      _variant_t var(pvarResult);
      var.ChangeType(VT_BSTR);
      _TRACE2("IActiveScriptSiteImpl::OnScriptTerminate(%ls): \"%ls\"\n",
        V_BSTR(&var), pei ? pei->bstrDescription : L"");
    #endif // _DEBUG
    return S_OK;
  }

  STDMETHODIMP OnStateChange(SCRIPTSTATE ssScriptState)
  {
    #if 0 // #ifdef _DEBUG
      LPCSTR pszState;
      switch (ssScriptState)
      {
        case SCRIPTSTATE_UNINITIALIZED: pszState = "Uninitialized"; break;
        case SCRIPTSTATE_STARTED      : pszState = "Started"      ; break;
        case SCRIPTSTATE_CONNECTED    : pszState = "Connected"    ; break;
        case SCRIPTSTATE_DISCONNECTED : pszState = "Disconnected" ; break;
        case SCRIPTSTATE_CLOSED       : pszState = "Closed"       ; break;
        case SCRIPTSTATE_INITIALIZED  : pszState = "Initialized"  ; break;
        default                       : pszState = "(Unknown)"    ;
      }
      _TRACE1("IActiveScriptSiteImpl::OnStateChange(%hs)\n", pszState);
    #endif // _DEBUG
    return S_OK;
  }

  STDMETHODIMP OnScriptError(IActiveScriptError* pScriptError)
  {
    // Trigger an event, if anyone is listening
    HAGCLISTENERS hListeners = GetAGCGlobal()->EventListeners(
      PigEventID_ScriptError, -1, -1, -1);
    if (hListeners)
    {
      // Get the exception info
      TCEXCEPINFO ei;
      _SVERIFYE(pScriptError->GetExceptionInfo(&ei));

      // Get the line and column of the error
      DWORD dwCtx;
      ULONG ulLine = 0;
      LONG  ich = 0;
      pScriptError->GetSourcePosition(&dwCtx, &ulLine, &ich);

      // Get the text of the source line
      CComBSTR  bstrLine;
      pScriptError->GetSourceLineText(&bstrLine);

      // Trigger an event
      XTLock lock(static_cast<T*>(this));
      USES_CONVERSION;
      _AGCModule.TriggerEvent(hListeners, PigEventID_ScriptError,
        GetScriptShortName().c_str(), -1, -1, -1, 5,
        "FileName"      , VT_LPWSTR, T2CW(GetScriptFileName().c_str()),
        "Line"          , VT_I4    , ulLine,
        "Column"        , VT_I4    , ich,
        "Description"   , VT_BSTR  , ei.bstrDescription,
        "SourceLineText", VT_BSTR  , (BSTR)bstrLine);
    }

    // Indicate success
    return S_OK;
  }

  STDMETHODIMP OnEnterScript()
  {
    //_TRACE0("IActiveScriptSiteImpl::OnEnterScript()\n");
    return S_OK;
  }

  STDMETHODIMP OnLeaveScript()
  {
    //_TRACE0("IActiveScriptSiteImpl::OnLeaveScript()\n");
    return S_OK;
  }

// Data Members
protected:
  tstring m_strScriptFileName;
  tstring m_strScriptShortName;
};


/////////////////////////////////////////////////////////////////////////////

#endif __IActiveScriptSiteImpl_h__
