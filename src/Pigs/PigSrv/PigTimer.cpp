/////////////////////////////////////////////////////////////////////////////
// PigTimer.cpp : Implementation of the CPigTimer class.

#include "pch.h"
#include <TCLib.h>
#include <..\TCAtl\ObjectMap.h>
#include "PigEventOwner.h"
#include "PigTimer.h"


/////////////////////////////////////////////////////////////////////////////
// CPigTimer

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CPigTimer)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPigTimer::CPigTimer() :
  m_nIntervalMS(0),
  m_nRepetitionCount(0),
  m_dwExpireNext(0)
{
}

HRESULT CPigTimer::FinalConstruct()
{
  // Indicate success
  return S_OK;
}

void CPigTimer::FinalRelease()
{
}

HRESULT CPigTimer::Init(CPigEventOwner* pOwner, float fInterval,
  BSTR bstrExpr, long nRepetitions, BSTR bstrName)
{
  XLock lock(this);

  // Perform default processing
  IPigEventImplBase::Init(pOwner);

  // Generate a timer name, if not specified
  CComBSTR bstrGeneratedName;
  if (!BSTRLen(bstrName))
  {
    CComBSTR bstrFmt;
    _VERIFYE(bstrFmt.LoadString(IDS_FMT_TIMER_NAME));
    OLECHAR szName[_MAX_PATH];
    swprintf(szName, bstrFmt, pOwner->GetNextEventNumber());
    bstrGeneratedName = szName;
    bstrName = bstrGeneratedName;
  }

  // Save the parameters
  RETURN_FAILED(put_Name(bstrName));
  RETURN_FAILED(put_ExpirationExpression(bstrExpr));
  RETURN_FAILED(put_Interval(fInterval));
  RETURN_FAILED(put_RepetitionCount(nRepetitions));

  // Calculate the next time that the timer should tick
  m_dwExpireNext = GetTickCount() + m_nIntervalMS;

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

bool CPigTimer::IsExpired()
{
  // Determine if this timer interval has expired
  XLock lock(this);
  return m_nRepetitionCount && (m_dwExpireNext < GetTickCount());
}

bool CPigTimer::Pulse(bool bExpired)
{
  // Do nothing if the event has not expired
  if (!bExpired)
    return false;

  // Calculate the next time that the timer should tick
  XLock lock(this);
  if (m_nIntervalMS)
    m_dwExpireNext += ((GetTickCount() - m_dwExpireNext + m_nIntervalMS - 1)
      / m_nIntervalMS) * m_nIntervalMS;

  // Decrement the repetition count (unless it's negative)
  //   Note: returning true here causes the CPigEventOwner to terminate us
  return (m_nRepetitionCount > 0) && (0 == --m_nRepetitionCount);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPigTimer::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPigTimer,
  };
  for (int i = 0; i < sizeofArray(arr); ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IPigTimer Interface Methods

STDMETHODIMP CPigTimer::put_RepetitionCount(long nRepetitionCount)
{
  // Validate the nRepetitionCount parameter
  if (0 == nRepetitionCount || nRepetitionCount < -1)
    return E_INVALIDARG;

  // Save the parameter
  XLock lock(this);
  m_nRepetitionCount = nRepetitionCount;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CPigTimer::get_RepetitionCount(long* pnRepetitionCount)
{
  XLock lock(this);
  CLEAROUT(pnRepetitionCount, m_nRepetitionCount)
  return S_OK;
}

STDMETHODIMP CPigTimer::put_Interval(float fInterval)
{
  XLock lock(this);
  m_nIntervalMS = fInterval * 1000.f;
  m_dwExpireNext = GetTickCount() + m_nIntervalMS;
  return S_OK;
}

STDMETHODIMP CPigTimer::get_Interval(float* pfInterval)
{
  CLEAROUT(pfInterval, float(m_nIntervalMS) / 1000.f);
  return S_OK;
}

