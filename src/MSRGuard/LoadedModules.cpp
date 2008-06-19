/////////////////////////////////////////////////////////////////////////////
// LoadedModules.cpp: Implementation of the CLoadedModules class.
//

#include "pch.h"
#include "LoadedModules.h"


/////////////////////////////////////////////////////////////////////////////
// CLoadedModules


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction


/////////////////////////////////////////////////////////////////////////////
// Operations


/////////////////////////////////////////////////////////////////////////////
//
void CLoadedModules::Add(void* pvImageBase, const ZString& strName)
{
  // Create a new CLoadedModule structure
  CLoadedModule module;
  module.m_strModuleName = strName;
  module.m_ModuleBase    = reinterpret_cast<ULONG>(pvImageBase);
  module.m_ModuleSize    = 0;

  // Add the object to the map
  m_Modules[module.m_ModuleBase] = module;
}


/////////////////////////////////////////////////////////////////////////////
//
void CLoadedModules::Remove(void* pvImageBase)
{
  // Remove the specified image from the map
  XLoadedModuleIt it = m_Modules.find(reinterpret_cast<ULONG>(pvImageBase));
  if (it != m_Modules.end())
    m_Modules.erase(it);
}


/////////////////////////////////////////////////////////////////////////////
//
CLoadedModuleIt CLoadedModules::findAddress(void* pvAddress)
{
	CLoadedModuleIt it; //--Imago
  const ULONG nAddress = reinterpret_cast<ULONG>(pvAddress);
  for (it = begin(); it != end(); ++it)
  {
    const CLoadedModule& m = it->second;
    if (m.m_ModuleBase < nAddress && nAddress < (m.m_ModuleBase + m.m_ModuleSize))
      break;
  }
  return it;
}


/////////////////////////////////////////////////////////////////////////////
//
void CLoadedModules::Dump()
{
  #if defined(_DEBUG)
    debugf("Loaded Modules By Address:\n");
    for (CLoadedModuleIt it = begin(); it != end(); ++it)
    {
      const CLoadedModule& m = it->second;
      debugf("    %08X-%08X %s\n", m.m_ModuleBase,
        m.m_ModuleBase + m.m_ModuleSize, (LPCTSTR)m.m_strModuleName);
    }
  #else // defined(_DEBUG)
    // Does nothing in non-debug builds
  #endif // defined(_DEBUG)
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
//
BOOL CALLBACK CLoadedModules::EnumLoadedModulesProc(PSTR ModuleName,
  ULONG ModuleBase, ULONG ModuleSize, PVOID UserContext)
{
  // Reinterpret the UserContext
  CLoadedModules* pThis = reinterpret_cast<CLoadedModules*>(UserContext);

  // Find the specified module if it's already in the map
  XLoadedModuleIt it = pThis->m_Modules.find(ModuleBase);
  if (it != pThis->m_Modules.end())
  {
    // Update the size field of the existing CLoadedModule structure
    CLoadedModule& module = it->second;
    module.m_ModuleSize = ModuleSize;
  }

  // Continue enumeration
  return true;
}


/////////////////////////////////////////////////////////////////////////////
//
void CLoadedModules::ResolveEmptyNames(HANDLE hProcess)
{
  // Shouldn't have to do anything under Win9x
  if (IsWin9x())
    return;

  // Loop through each entry
  for (XLoadedModuleIt it = m_Modules.begin(); it != m_Modules.end(); ++it)
  {
    CLoadedModule& m = it->second;
    if (m.m_strModuleName.IsEmpty())
    {
      // Use the module base address as the HMODULE
      HMODULE hModule = reinterpret_cast<HMODULE>(m.m_ModuleBase);

      // Get the module file name
      TCHAR szModule[_MAX_PATH * 2];
      if (::GetModuleFileNameEx(hProcess, hModule, szModule, sizeofArray(szModule)))
        m.m_strModuleName = szModule;
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
//
void CLoadedModules::ResolveUnknownSizes(HANDLE hProcessSym)
{
  // Loop through each entry
  for (XLoadedModuleIt it = m_Modules.begin(); it != m_Modules.end(); ++it)
  {
    // Determine if the current path name is fully-qualified
    CLoadedModule& m = it->second;
    TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
    _tsplitpath(m.m_strModuleName, szDrive, szDir, NULL, NULL);
    bool bFullyQualified = TEXT('\0') != szDrive[0] || TEXT('\0') != szDir[0];

    // If we don't have enough information about the module, get some more
    if (!m.m_ModuleSize || !bFullyQualified)
    {
      IMAGEHLP_MODULE mi = {sizeof(mi)};
      if (::SymGetModuleInfo(hProcessSym, m.m_ModuleBase, &mi))
      {
        m.m_ModuleSize = mi.ImageSize;
        if (!bFullyQualified)
          m.m_strModuleName = mi.ImageName;
      }
    }
  }
}

