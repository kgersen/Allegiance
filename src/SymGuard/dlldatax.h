#if !defined(__dlldatax_h__)
#define __dlldatax_h__

#if _MSC_VER >= 1000
  #pragma once
#endif // _MSC_VER >= 1000

#ifdef _MERGE_PROXYSTUB

extern "C" 
{
  BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason, 
    LPVOID lpReserved);
  STDAPI PrxDllCanUnloadNow(void);
  STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
  STDAPI PrxDllRegisterServer(void);
  STDAPI PrxDllUnregisterServer(void);
}

#endif

#endif // !defined(__dlldatax_h__)
