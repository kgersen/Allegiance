#pragma once

/////////////////////////////////////////////////////////////////////////////
// MSRGuard.h : Defines the module-wide state
//


/////////////////////////////////////////////////////////////////////////////
// Forward Declaration

class IAppMode;


/////////////////////////////////////////////////////////////////////////////
// CMSRGuardModule
//
class CMSRGuardModule : public Win32App
{
// Construction
public:
  CMSRGuardModule();
  void Init();

// Attributes
public:
  // Option flags
  bool GetOptionValue(const ZString& strOptionFlag, ZString& strOut) const;
  bool GetOptionExists(const ZString& strOptionFlag) const;
  ZString GetOptionString(const ZString& strOptionFlag) const;
  ZString GetOptionString(const ZString& strOptionFlag, const ZString& strDefault) const;
  long GetOptionLong(const ZString& strOptionFlag) const;
  long GetOptionLong(const ZString& strOptionFlag, long nDefault) const;
  bool GetOptionBool(const ZString& strOptionFlag) const;
  bool GetOptionBool(const ZString& strOptionFlag, bool bDefault) const;
  bool GetConfigValue(const ZString& strConfigName, ZString& strOut) const;
  // Config settings
  bool GetConfigExists(const ZString& strConfigName) const;
  ZString GetConfigString(const ZString& strConfigName) const;
  ZString GetConfigString(const ZString& strConfigName, const ZString& strDefault) const;
  long GetConfigLong(const ZString& strConfigName) const;
  long GetConfigLong(const ZString& strConfigName, long nDefault) const;
  bool GetConfigBool(const ZString& strConfigName) const;
  bool GetConfigBool(const ZString& strConfigName, bool bDefault) const;
  // Other
  HINSTANCE GetResourceInstance() const;
  HINSTANCE GetModuleInstance() const;
  LPCTSTR GetTitle() const;
  int GetCountOfOptionsEaten() const;
  // Debugging control
  bool GetHitFirstBreakpoint() const;
  void SetHitFirstBreakpoint(bool bHit);

// Operations
public:
  int DoMain(int argc, TCHAR* argv[]);
  HRESULT HandleErrorV(HRESULT hr, UINT idFmt, va_list argptr);
  HRESULT HandleError(HRESULT hr, UINT idFmt, ...);
  HRESULT DisplayMessageV(UINT idFmt, va_list argptr);
  HRESULT DisplayMessage(UINT idFmt, ...);
  ZString GetErrorString(HRESULT hr);
  // Resource helpers
  HRESULT LoadString(ZString& strOut, UINT idRes);
  HRESULT FormatStringV(ZString& strOut, UINT idFmt, va_list argptr);
  HRESULT FormatString(ZString& strOut, UINT idFmt, ...);
  HRESULT FormatStringV(ZString& strOut, LPCTSTR pszFmt, va_list argptr);
  HRESULT FormatString(ZString& strOut, LPCTSTR pszFmt, ...);
  HRESULT LoadMINI(ZString& strOut);
  HRESULT LoadHTML(ZString& strOut);
  // Registry helpers
  static HKEY HKEYFromString(const ZString& strHKEY);

// Implementation
private:
  int ProcessCmdLineOption(int i, int argc, TCHAR* argv[]);
  void SetAppMode(IAppMode* pAppMode);
  HRESULT LoadConfig();

// Option Flag Handlers
private:
  int OnOptionHelp  (int i, int argc, TCHAR* argv[]);
  int OnOptionPID   (int i, int argc, TCHAR* argv[]);
  int OnOptionINI   (int i, int argc, TCHAR* argv[]);
  int OnOptionMINI  (int i, int argc, TCHAR* argv[]);
  int OnOptionHTML  (int i, int argc, TCHAR* argv[]);
  int OnOptionBind  (int i, int argc, TCHAR* argv[]);
  int OnOptionSilent(int i, int argc, TCHAR* argv[]);

// Types
private:
  typedef int (CMSRGuardModule::*XOptionFlagProc)(int, int, TCHAR*[]);
  struct XOptionFlag
  {
    LPCTSTR         m_szOptionFlag;
    int             m_cchSignificant;
    XOptionFlagProc m_pfn;
  };
  typedef const XOptionFlag* XOptionFlagIt;
  struct XLessCI : public std::binary_function<ZString, ZString, bool>
  {
    bool operator()(const ZString& x, const ZString& y) const
    {
      return 0 > _stricmp(x, y);
    }
  };
  typedef std::map<ZString, ZString, XLessCI> XOptionMap;
  typedef XOptionMap::const_iterator XOptionMapIt;
  typedef std::map<ZString, ZString, XLessCI> XConfigMap;
  typedef XConfigMap::const_iterator XConfigMapIt;

// Data Members
private:
  XOptionMap m_Options;
  XConfigMap m_Configs;
  HINSTANCE  m_hinst;
  HINSTANCE  m_hinstResource;
  int        m_cOptionsEaten;
  IAppMode*  m_pAppMode;
  bool       m_bHitFirstBreakpoint;

// Static Data
private:
  const static XOptionFlag s_OptionFlags[];
};


/////////////////////////////////////////////////////////////////////////////
// Global Instance
//
extern CMSRGuardModule g;


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetOptionValue(const ZString& strOptionFlag,
  ZString& strOut) const
{
  XOptionMapIt it = m_Options.find(strOptionFlag);
  if (m_Options.end() == it)
    return false;
  strOut = it->second;
  return true;
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetOptionExists(const ZString& strOptionFlag)
  const
{
  return m_Options.end() != m_Options.find(strOptionFlag);
}


/////////////////////////////////////////////////////////////////////////////
//
inline ZString CMSRGuardModule::GetOptionString(
  const ZString& strOptionFlag) const
{
  return GetOptionString(strOptionFlag, ZString());
}


/////////////////////////////////////////////////////////////////////////////
//
inline ZString CMSRGuardModule::GetOptionString(const ZString& strOptionFlag,
  const ZString& strDefault) const
{
  ZString str(strDefault);
  GetOptionValue(strOptionFlag, str);
  return str;
}


/////////////////////////////////////////////////////////////////////////////
//
inline long CMSRGuardModule::GetOptionLong(
  const ZString& strOptionFlag) const
{
  return GetOptionLong(strOptionFlag, 0L);
}


/////////////////////////////////////////////////////////////////////////////
//
inline long CMSRGuardModule::GetOptionLong(const ZString& strOptionFlag,
  long nDefault) const
{
  ZString str;
  return GetOptionValue(strOptionFlag, str) ? _ttol(str) : nDefault;
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetOptionBool(
  const ZString& strOptionFlag) const
{
  return GetOptionBool(strOptionFlag, false);
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetOptionBool(const ZString& strOptionFlag,
  bool bDefault) const
{
  return !!GetOptionLong(strOptionFlag, static_cast<long>(bDefault));
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetConfigValue(const ZString& strConfigName,
  ZString& strOut) const
{
  XConfigMapIt it = m_Configs.find(strConfigName);
  if (m_Configs.end() == it)
    return false;
  strOut = it->second;
  return true;
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetConfigExists(const ZString& strConfigName)
  const
{
  return m_Configs.end() != m_Configs.find(strConfigName);
}


/////////////////////////////////////////////////////////////////////////////
//
inline ZString CMSRGuardModule::GetConfigString(
  const ZString& strConfigName) const
{
  return GetConfigString(strConfigName, ZString());
}


/////////////////////////////////////////////////////////////////////////////
//
inline ZString CMSRGuardModule::GetConfigString(const ZString& strConfigName,
  const ZString& strDefault) const
{
  ZString str(strDefault);
  GetConfigValue(strConfigName, str);
  return str;
}


/////////////////////////////////////////////////////////////////////////////
//
inline long CMSRGuardModule::GetConfigLong(
  const ZString& strConfigName) const
{
  return GetConfigLong(strConfigName, 0L);
}


/////////////////////////////////////////////////////////////////////////////
//
inline long CMSRGuardModule::GetConfigLong(const ZString& strConfigName,
  long nDefault) const
{
  ZString str;
  return GetConfigValue(strConfigName, str) ? _ttol(str) : nDefault;
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetConfigBool(
  const ZString& strConfigName) const
{
  return GetConfigBool(strConfigName, false);
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetConfigBool(const ZString& strConfigName,
  bool bDefault) const
{
  return !!GetConfigLong(strConfigName, static_cast<long>(bDefault));
}


/////////////////////////////////////////////////////////////////////////////
//
inline HINSTANCE CMSRGuardModule::GetResourceInstance() const
{
  return m_hinstResource;
}


/////////////////////////////////////////////////////////////////////////////
//
inline HINSTANCE CMSRGuardModule::GetModuleInstance() const
{
  return m_hinst;
}


/////////////////////////////////////////////////////////////////////////////
//
inline LPCTSTR CMSRGuardModule::GetTitle() const
{
  return GetConfigString(TEXT("Title"));
}


/////////////////////////////////////////////////////////////////////////////
//
inline int CMSRGuardModule::GetCountOfOptionsEaten() const
{
  return m_cOptionsEaten;
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CMSRGuardModule::GetHitFirstBreakpoint() const
{
  return m_bHitFirstBreakpoint;
}


/////////////////////////////////////////////////////////////////////////////
//
inline void CMSRGuardModule::SetHitFirstBreakpoint(bool bHit)
{
  m_bHitFirstBreakpoint = bHit;
}

