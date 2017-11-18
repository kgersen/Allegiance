/////////////////////////////////////////////////////////////////////////////
// AdminSessionHelper.cpp | Implementation of the CAdminSessionHelper class.
//

#include "pch.h"
#include "AdminSessionHelper.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminSessionHelper

TC_OBJECT_EXTERN_IMPL(CAdminSessionHelper)


#define szAllSrvRunning TEXT("{5B5ED2A6-E7C0-11D2-8B4B-00C04F681633}_Running")
#define szAllSrvRunningGlobal (TEXT("Global\\") szAllSrvRunning)


/////////////////////////////////////////////////////////////////////////////
// IAdminSessionHelper Interface Methods

STDMETHODIMP CAdminSessionHelper::get_IsAllSrvRunning(VARIANT_BOOL* pbIsRunning)
{
  // Open the mutex using the global name first
  HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, false, szAllSrvRunningGlobal);
  if (!hMutex)
    hMutex = ::OpenMutex(SYNCHRONIZE, false, szAllSrvRunning);
  bool bRunning = NULL != hMutex;
  if (bRunning)
    ::CloseHandle(hMutex);
  CLEAROUT(pbIsRunning, VARBOOL(bRunning));
  return S_OK;
}

