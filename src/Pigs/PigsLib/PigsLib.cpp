/////////////////////////////////////////////////////////////////////////////
// PigsLib.cpp | Implementation of DLL Exports.
//

#include "pch.h"
#include "SrcInc.h"
#include <PigsLib.h>
#include <TCLib.h>
#include <..\TCLib\WinApp.h>

#include "PigsIDL_i.c"
#include "PigsLib_i.c"


/////////////////////////////////////////////////////////////////////////////
// Note: Proxy/Stub Information
//    To merge the proxy/stub code into the object DLL, add the file 
//    dlldatax.c to the project.  Make sure precompiled headers 
//    are turned off for this file, and add _MERGE_PROXYSTUB to the 
//    defines for the project.  
//
//    If you are not running WinNT4.0 or Win95 with DCOM, then you
//    need to remove the following define from dlldatax.c
//    #define _WIN32_WINNT 0x0400
//
//    Further, if you are running MIDL without /Oicf switch, you also 
//    need to remove the following define from dlldatax.c.
//    #define USE_STUBLESS_PROXY
//
//    Modify the custom build rule for PigsLib.idl by adding the following 
//    files to the Outputs.
//      PigsLib_p.c
//      dlldata.c
//    To build a separate proxy/stub DLL, 
//    run nmake -f PigsLibps.mk in the project directory.


/////////////////////////////////////////////////////////////////////////////
// Global Initialization

#ifdef _MERGE_PROXYSTUB
  extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;  // ATL mandatory symbol
TCWinApp  g_app;


/////////////////////////////////////////////////////////////////////////////
// External Declarations

TC_OBJECT_EXTERN(CPigMissionParams)
// End: External Declarations


/////////////////////////////////////////////////////////////////////////////
// Object Map

BEGIN_OBJECT_MAP(ObjectMap)
  TC_OBJECT_ENTRY_STD(PigMissionParams)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  lpReserved;

  #ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
      return false;
  #endif

  if (DLL_PROCESS_ATTACH == dwReason)
  {
    TRACE_MODULE_INIT(hInstance, true);

    // Initialize ZLIB
    InitAllocs();

    // Report ASSERTS to Deugger
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

    // Initialize the ATL module
    _Module.Init(ObjectMap, hInstance);
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
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
  #ifdef _MERGE_PROXYSTUB
    if (S_OK != PrxDllCanUnloadNow())
      return S_FALSE;
  #endif

  return _Module.GetLockCount() ? S_FALSE : S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
  // Initialize the application, if needed
  g_papp->Initialize(ZString());

  #ifdef _MERGE_PROXYSTUB
    if (S_OK == PrxDllGetClassObject(rclsid, riid, ppv))
      return S_OK;
  #endif

  return _Module.GetClassObject(rclsid, riid, ppv);
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
  #ifdef _MERGE_PROXYSTUB
    RETURN_FAILED(PrxDllRegisterServer());
  #endif

  // registers object, typelib and all interfaces in typelib
  RETURN_FAILED(_Module.RegisterServer(TRUE));

  // Register the component category
  return RegisterComponentCategory(CATID_PigObjects,
    L"Allegiance Pig Server Objects");
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
  #ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
  #endif

  // Unregister the component category
  UnregisterComponentCategory(CATID_PigObjects);

  _Module.UnregisterServer();
  return S_OK;
}


