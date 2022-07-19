/////////////////////////////////////////////////////////////////////////////
// AdminPageThreadWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AllSrvUI.h"
#include "AdminPageThreadWnd.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAdminPageThreadWnd


/////////////////////////////////////////////////////////////////////////////
// Message Map

BEGIN_MESSAGE_MAP(CAdminPageThreadWnd, CWnd)
  ON_MESSAGE(wm_AdminPageMsg, OnAdminPageMsg)
  ON_MESSAGE(wm_AdminPageRun, OnAdminPageRun)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Construction

CAdminPageThreadWnd::CAdminPageThreadWnd()
{
  // Create an AGCEvent object
  VERIFY(SUCCEEDED(m_spEvent.CreateInstance(__uuidof(AGCEvent))));

  // Get the AGCEvent object's IPersistStream interface
  m_spEventPersist = m_spEvent;
  _ASSERTE(NULL != m_spEventPersist);
}

BOOL CAdminPageThreadWnd::Create()
{
  return CWnd::CreateEx(0, "static", NULL, WS_OVERLAPPED, 0, 0, 0, 0, NULL, NULL);
}

BOOL CAdminPageThreadWnd::Create(CString strShortcutPath)
{
  m_strShortcutPath = strShortcutPath;
  return Create();
}


/////////////////////////////////////////////////////////////////////////////
// Operations

HRESULT CAdminPageThreadWnd::PostAdminPageMsg(IAGCEvent* pEvent)
{
  return PostAdminPage(pEvent, wm_AdminPageMsg);
}

HRESULT CAdminPageThreadWnd::PostAdminPageRun(IAGCEvent* pEvent)
{
  return PostAdminPage(pEvent, wm_AdminPageRun);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT CAdminPageThreadWnd::PostAdminPage(IAGCEvent* pEvent, UINT message)
{
  // Persist the event object into a stream
  IStreamPtr spStm;
  HRESULT hr = CreateStreamOnHGlobal(NULL, true, &spStm);
  if (FAILED(hr))
    return hr;
  IPersistStreamPtr spPersist(pEvent);
  if (NULL == spPersist)
    return E_NOINTERFACE;
  if (FAILED(hr = spPersist->Save(spStm, false)))
    return hr;

  // Post the stream to the worker thread window
  PostMessage(message, reinterpret_cast<WPARAM>(spStm.Detach()), NULL);
  return S_OK;
}

CString CAdminPageThreadWnd::GetDate()
{
  DATE date;
  if (FAILED(m_spEvent->get_Time(&date)))
    return CString();

  // Format the date as a string
  SYSTEMTIME sysTime;
  VariantTimeToLocalTime(date, &sysTime);
  SystemTimeToVariantTime(&sysTime, &date);
  CComBSTR bstr;
  VarBstrFromDate(date, GetThreadLocale(), LOCALE_NOUSEROVERRIDE, &bstr);
  return CString(bstr.m_str,bstr.Length());
}

CString CAdminPageThreadWnd::GetSender()
{
  CComBSTR bstrSender;
  if (FAILED(m_spEvent->get_SubjectName(&bstrSender)))
    return CString();
  return CString(bstrSender.m_str,bstrSender.Length());
}

CString CAdminPageThreadWnd::GetMessage()
{
  CComVariant varMessage;
  if (FAILED(m_spEvent->get_Property(&CComVariant(L"Message"), &varMessage)))
    return CString();
  varMessage.ChangeType(VT_BSTR);
  return V_BSTR(&varMessage);
}

void CAdminPageThreadWnd::TranslateEvent(WPARAM wParam)
{
  // Reinterpret the message parameter
  IStreamPtr spStm(reinterpret_cast<IStream*>(wParam), false);

  // Rewind the specified stream
  LARGE_INTEGER li = {0};
  VERIFY(SUCCEEDED(spStm->Seek(li, STREAM_SEEK_SET, NULL)));

  // Persist the event from the specified stream
  VERIFY(SUCCEEDED(m_spEventPersist->Load(spStm)));
}

bool CAdminPageThreadWnd::VariantTimeToLocalTime(DATE date, SYSTEMTIME* psystime)
{
  // Convert the VARIANT time to a SYSTEMTIME structure
  SYSTEMTIME systimeUTC, systime;
  if (!VariantTimeToSystemTime(date, &systimeUTC))
    return false;

  // Convert the SYSTEMTIME from UTC to the local time zone
  if (!SystemTimeToTzSpecificLocalTime(NULL, &systimeUTC, &systime))
  {
    TIME_ZONE_INFORMATION tz;
    DWORD zone = GetTimeZoneInformation(&tz);
    if (zone != TIME_ZONE_ID_UNKNOWN)
    {
      LONG lBias = tz.Bias;
      if (zone == TIME_ZONE_ID_STANDARD)
        lBias += tz.StandardBias;
      else //TIME_ZONE_ID_DAYLIGHT:
        lBias += tz.DaylightBias;

      FILETIME ft;
      VERIFY(SystemTimeToFileTime(&systimeUTC, &ft));
      __int64 time64 = ((__int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
      __int64 bias = (__int64)lBias * (__int64)600000000;
      time64 -= bias;
      ft.dwHighDateTime = (DWORD)(time64 >> 32);
      ft.dwLowDateTime = (DWORD)time64;
      VERIFY(FileTimeToSystemTime(&ft,&systime));
    }
  }

  // Copy the SYSTEMTIME structure to the specified [out] parameter
  __try
  {
    CopyMemory(psystime, &systime, sizeof(*psystime));
    return true;
  }
  __except(1)
  {
    return false;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Message Handlers

LRESULT CAdminPageThreadWnd::OnAdminPageMsg(WPARAM wParam, LPARAM)
{
  // Translate the event
  TranslateEvent(wParam);

  // Format the string
  CString strText;
  strText.Format(TEXT("From:\t%s\nSent:\t%s\nTo:\tADMIN\n\n%s"), GetSender(),
    GetDate(), GetMessage());

  // Display the message box
  HWND hwnd = ::GetForegroundWindow();
  ::SetForegroundWindow(*this);
  MessageBox(strText, TEXT("Admin Alert"), MB_OK | MB_ICONWARNING);
  ::SetForegroundWindow(hwnd);
  return 0;
}

LRESULT CAdminPageThreadWnd::OnAdminPageRun(WPARAM wParam, LPARAM)
{
  // Translate the event
  TranslateEvent(wParam);

  // Format the string
  CString strText;
  strText.Format(TEXT("\"%s\" \"%s\" %s"), GetDate(), GetSender(), GetMessage());

  // Execute the shortcut
  SHELLEXECUTEINFO shei = {sizeof(shei)};
  shei.lpVerb = TEXT("open");
  shei.lpFile = m_strShortcutPath;
  shei.lpParameters = strText;
  ShellExecuteEx(&shei);
  return 0;
}

