#pragma once

/////////////////////////////////////////////////////////////////////////////
// ExcepInfo.h


/////////////////////////////////////////////////////////////////////////////

struct TCEXCEPINFO : public tagEXCEPINFO
{
// Construction / Destruction
public:
  TCEXCEPINFO();
  ~TCEXCEPINFO();

// Operations
public:
  static void Release(EXCEPINFO* pei);
  static HRESULT CreateErrorInfo(EXCEPINFO* pei, IErrorInfo** ppei);
  static HRESULT SetErrorInfo(EXCEPINFO* pei);
  void Release();
  HRESULT CreateErrorInfo(IErrorInfo** ppei);
  HRESULT SetErrorInfo();
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

inline TCEXCEPINFO::TCEXCEPINFO()
{
  ZeroMemory(this, sizeof(*this));
}

inline TCEXCEPINFO::~TCEXCEPINFO()
{
  Release();
}


/////////////////////////////////////////////////////////////////////////////
// Operations

inline void TCEXCEPINFO::Release()
{
  // Delegate to the static method
  Release(this);
}

inline HRESULT TCEXCEPINFO::CreateErrorInfo(IErrorInfo** ppei)
{
  // Delegate to the static method
  return CreateErrorInfo(this, ppei);
}

inline HRESULT TCEXCEPINFO::SetErrorInfo()
{
  // Delegate to the static method
  return SetErrorInfo(this);
}


/////////////////////////////////////////////////////////////////////////////
