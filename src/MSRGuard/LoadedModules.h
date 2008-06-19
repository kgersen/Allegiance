#pragma once

/////////////////////////////////////////////////////////////////////////////
// LoadedModules.h: Declaration of the CLoadedModules class.
//


/////////////////////////////////////////////////////////////////////////////
// Types

struct CLoadedModule
{
  ZString m_strModuleName;
  ULONG   m_ModuleBase;
  ULONG   m_ModuleSize;
};

typedef std::map<ULONG, CLoadedModule>   CLoadedModuleMap;
typedef CLoadedModuleMap::const_iterator CLoadedModuleIt;


/////////////////////////////////////////////////////////////////////////////

class CLoadedModules
{
// Construction / Destruction
public:
//  CLoadedModules();
//  ~CLoadedModules();

// Attributes
public:
  CLoadedModuleIt begin();
  CLoadedModuleIt end();

// Operations
public:
  void Add(void* pvImageBase, const ZString& strName);
  void Remove(void* pvImageBase);
  CLoadedModuleIt find(void* pvImageBase);
  CLoadedModuleIt findAddress(void* pvAddress);
  void clear();
  bool Enumerate(HANDLE hProcess, HANDLE hProcessSym);
  void Dump();

// Implementation
private:
  static BOOL CALLBACK EnumLoadedModulesProc(PSTR ModuleName,
    ULONG ModuleBase, ULONG ModuleSize, PVOID UserContext);
  void ResolveEmptyNames(HANDLE hProcess);
  void ResolveUnknownSizes(HANDLE hProcessSym);

// Types
private:
  typedef CLoadedModuleMap::iterator XLoadedModuleIt;

// Data Members
private:
  CLoadedModuleMap m_Modules;
};



/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
//
inline CLoadedModuleIt CLoadedModules::begin()
{
  return m_Modules.begin();
}


/////////////////////////////////////////////////////////////////////////////
//
inline CLoadedModuleIt CLoadedModules::end()
{
  return m_Modules.end();
}


/////////////////////////////////////////////////////////////////////////////
// Operations


/////////////////////////////////////////////////////////////////////////////
//
inline CLoadedModuleIt CLoadedModules::find(void* pvImageBase)
{
  return m_Modules.find(reinterpret_cast<ULONG>(pvImageBase));
}


/////////////////////////////////////////////////////////////////////////////
//
inline void CLoadedModules::clear()
{
  // Clear the map
  m_Modules.clear();
}


/////////////////////////////////////////////////////////////////////////////
//
inline bool CLoadedModules::Enumerate(HANDLE hProcess, HANDLE hProcessSym)
{
  // Enumerate the modules --Imago fixed for VC9
  if (!::EnumerateLoadedModules(hProcess, (PENUMLOADED_MODULES_CALLBACK)EnumLoadedModulesProc, this))
    return false;
  ResolveEmptyNames(hProcess);
  ResolveUnknownSizes(hProcessSym);
  return true;
}
