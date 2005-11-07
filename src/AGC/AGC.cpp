/////////////////////////////////////////////////////////////////////////////
// AGC.cpp : Implementation of DLL Exports.
//
// Note: Proxy/Stub Information
//      To merge the proxy/stub code into the object DLL, add the file 
//      dlldatax.c to the project.  Make sure precompiled headers 
//      are turned off for this file, and add _MERGE_PROXYSTUB to the 
//      defines for the project.  
//
//      If you are not running WinNT4.0 or Win95 with DCOM, then you
//      need to remove the following define from dlldatax.c
//      #define _WIN32_WINNT 0x0400
//
//      Further, if you are running MIDL without /Oicf switch, you also 
//      need to remove the following define from dlldatax.c.
//      #define USE_STUBLESS_PROXY
//
//      Modify the custom build rule for AGC.idl by adding the following 
//      files to the Outputs.
//          AGC_p.c
//          dlldata.c
//      To build a separate proxy/stub DLL, 
//      run nmake -f AGCps.mk in the project directory.

#include "pch.h"
#include "dlldatax.h"
#include "AGC.h"
#include "AGCWinApp.h"
#include "AGCGlobal.h"


/////////////////////////////////////////////////////////////////////////////
// External Declarations

TC_OBJECT_EXTERN(CAGCGlobal)
TC_OBJECT_EXTERN(CAGCVector)
TC_OBJECT_EXTERN(CAGCOrientation)
TC_OBJECT_EXTERN(CAGCEvent)
TC_OBJECT_EXTERN(CAGCCommand)
TC_OBJECT_EXTERN(CAGCVersionInfo)
TC_OBJECT_EXTERN(CAGCDBParams)
TC_OBJECT_EXTERN(CAGCEventLogger)
TC_OBJECT_EXTERN(CAGCEventIDRange)
TC_OBJECT_EXTERN(CAGCEventIDRanges)
TC_OBJECT_EXTERN(CAGCGameParameters)
TC_OBJECT_EXTERN(CTCStrings)
TC_OBJECT_EXTERN(CTCMarshalByValue)
TC_OBJECT_EXTERN(CTCNullStream)
//TC_OBJECT_EXTERN(CTCPropBagOnRegKey)
TC_OBJECT_EXTERN(CTCUtility)
TC_OBJECT_EXTERN(CAdminSessionHelper)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCHullType)
//TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCHullTypes)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCGame)
//TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCGames)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCShip)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCShips)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCStation)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCStations)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCAleph)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCAlephs)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCSector)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCSectors)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCTeam)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCTeams)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCAsteroid)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCAsteroids)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCModel)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCModels)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCProbe)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAGCProbes)


/////////////////////////////////////////////////////////////////////////////
// Global Declarations

#ifdef _MERGE_PROXYSTUB
  extern "C" HINSTANCE hProxyDll;
#endif

CAGCModule   _AGCModule;
CComModule   _Module;
CAGCWinApp  g_app;


/////////////////////////////////////////////////////////////////////////////
// Object Map

BEGIN_OBJECT_MAP(ObjectMap)
  TC_OBJECT_ENTRY_STD(AGCGlobal)          // <-- Should always be first entry
  TC_OBJECT_ENTRY_STD(AGCVector)
  TC_OBJECT_ENTRY_STD(AGCOrientation)
  TC_OBJECT_ENTRY_STD(AGCEvent)
  TC_OBJECT_ENTRY_STD(AGCCommand)
  TC_OBJECT_ENTRY_STD(AGCVersionInfo)
  TC_OBJECT_ENTRY_STD(AGCDBParams)
  TC_OBJECT_ENTRY_STD(AGCEventLogger)
  TC_OBJECT_ENTRY_STD(AGCEventIDRange)
  TC_OBJECT_ENTRY_STD(AGCEventIDRanges)
  TC_OBJECT_ENTRY_STD(AGCGameParameters)
  TC_OBJECT_ENTRY_STD(TCStrings)
  TC_OBJECT_ENTRY_STD(TCMarshalByValue)
  TC_OBJECT_ENTRY_STD(TCNullStream)
//  TC_OBJECT_ENTRY_STD(TCPropBagOnRegKey)
  TC_OBJECT_ENTRY_STD(TCUtility)
  TC_OBJECT_ENTRY_STD(AdminSessionHelper)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCHullType)
//  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCHullTypes)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCGame)
//  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCGames)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCShip)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCShips)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCStation)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCStations)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCAleph)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCAlephs)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCSector)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCSectors)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCTeam)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCTeams)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCAsteroid)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCAsteroids)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCModel)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCModels)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCProbe)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AGCProbes)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
  #ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
      return false;
  #endif

  if (DLL_PROCESS_ATTACH == dwReason)
  {
    TRACE_MODULE_INIT(hInstance, true);
    _Module.Init(ObjectMap, hInstance, &LIBID_AGCLib);
		DisableThreadLibraryCalls(hInstance);
  }
  else if (DLL_PROCESS_DETACH == dwReason)
  {
    _Module.Term();
    TRACE_MODULE_INIT(hInstance, false);
  }
  return true;    // ok
}


/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by COM

STDAPI DllCanUnloadNow(void)
{
  #ifdef _MERGE_PROXYSTUB
    if (PrxDllCanUnloadNow() != S_OK)
      return S_FALSE;
  #endif
  return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  #ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
      return S_OK;
  #endif
  return _Module.GetClassObject(rclsid, riid, ppv);
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
  HRESULT hRes;
  #ifdef _MERGE_PROXYSTUB
    hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
      return hRes;
  #endif

  // Create an instance of the component category manager
  CComPtr<ICatRegister> spCatReg;
  hRes = spCatReg.CoCreateInstance(CLSID_StdComponentCategoriesMgr);
  ZSucceeded(hRes);
  if (SUCCEEDED(hRes))
  {
    // Determine the LCID for US English
    const LANGID langid = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
    const LCID lcid = MAKELCID(langid, SORT_DEFAULT);

    // Register the component category
    CATEGORYINFO catinfo;
    catinfo.catid = CATID_AGC;
    catinfo.lcid = lcid;
    wcscpy(catinfo.szDescription, L"Allegiance Active Game Core");
    hRes = spCatReg->RegisterCategories(1, &catinfo);
    ZSucceeded(hRes);
  }

  // registers object, typelib and all interfaces in typelib
  return _Module.RegisterServer(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
  #ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
  #endif

  // Create the component category manager
  CComPtr<ICatRegister> spCatReg;
  HRESULT hRes = spCatReg.CoCreateInstance(CLSID_StdComponentCategoriesMgr);
  ZSucceeded(hRes);
  if (SUCCEEDED(hRes))
  {
    // Unregister our component category
    CATID catid = CATID_AGC;
    spCatReg->UnRegisterCategories(1, &catid);
    spCatReg = NULL;
  }

  return _Module.UnregisterServer(TRUE);
}


STDAPI DllRegisterAGC()
{
  return DllRegisterServer();
}


STDAPI DllUnregisterAGC()
{
  return DllUnregisterServer();
}

