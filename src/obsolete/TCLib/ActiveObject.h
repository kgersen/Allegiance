#pragma once

#ifndef __ActiveObject_h__
#define __ActiveObject_h__

/////////////////////////////////////////////////////////////////////////////
// ActiveObject.h : Declaration and implementation of the
// MyRegisterActiveObject function, which provides the functionality of the
// OLE RegisterActiveObject API, but without its security context limitations.
//
// This was taken from Microsoft KB article ID: Q156673
//



/////////////////////////////////////////////////////////////////////////////

/*
 * MyRegisterActiveObject
 *
 * Purpose:
 *  Provides the functionality of the OLE API RegisterActiveObject. It registers an 
 *  object in the Running Object Table (ROT). In can handle the ROTFLAGS_ALLOWANYCLIENT flag,
 *  unlike RegisterActiveObject. 
 *
 * Parameters:
 *    punk        [in]  IUnknown of active object
 *    rclsid      [in]  CLSID of active object
 *    dwFlags     [in]  Specifies whether the ROT's reference to punk is weak or strong. 
 *                      This value must be either zero, indicating a weak reference 
 *                      or ROTFLAGS_REGISTRATIONKEEPSALIVE, indicating a strong reference.
 *                      If the caller is a server that is a service or an EXE server
 *                      configured with the RunAs registry value, it can also use the
 *                      ROTFLAGS_ALLOWANYCLIENT flag to allow clients in other security
 *                      contexts to bind to the active object.
 *    pdwRegister [out] Returns a 32-bit value that can be used to identify 
 *                      this ROT entry in subsequent calls to RevokeActiveObject.
 *                      If an error occurs, *pdwRegister is set to zero.
 *
 */
inline HRESULT MyRegisterActiveObject(IUnknown* punk, REFCLSID rclsid,
  DWORD dwFlags, unsigned long* pdwRegister)
{
        HRESULT hr;
        LPOLESTR psz = NULL;
        IMoniker* pmk = NULL;
        IRunningObjectTable* prot = NULL;

        hr = StringFromCLSID(rclsid, &psz);
        if (FAILED(hr))
                goto cleanup;

        hr = CreateItemMoniker(OLESTR("!"), psz, &pmk);
        if (FAILED(hr))
                goto cleanup;

        hr = GetRunningObjectTable(0, &prot);
        if (FAILED(hr))
                goto cleanup;

        hr = prot->Register(dwFlags, punk, pmk, pdwRegister);
        if (FAILED(hr))
        {
#ifdef _DEBUG
            // If IROT::Register fails with CO_E_WRONG_SERVER_IDENTITY, it
            // means that you running the service in a different user identity than
            // what the registy shows for the service. For example, this service is
            // registered to run in LocalSystem. If this is run for debugging in the
            // the interactive user's identity, CoRegisterClassObject will fail in this
            // manner. OLE does this to enforce security. While debugging in the 
            // interactive user's identity, remove the following named value from the
            // this services' entry in the registry to prevent this error:
            // [HKEY_CLASSES_ROOT\APPID\{0bf52b15-8cab-11cf-8572-00aa00c006cf}]
            // "LocalService"="HelloOleServerService"
            // Then Add the following registry entry:
            // [HKEY_CLASSES_ROOT\APPID\{0bf52b15-8cab-11cf-8572-00aa00c006cf}]
            // "RunAs"="Interactive User"
            // Remember to restore the registry when the debugging is completed.
            // 
            TCHAR ach[100];
            wsprintf(ach, TEXT("IROT::Register failed. HRESULT = %lx. See comments in source code"), hr);
            MessageBox(NULL, ach, TEXT("Hello Service."), 
                 MB_OK|MB_SERVICE_NOTIFICATION);
#endif     
            goto cleanup;
        }             

cleanup:
        if (psz) CoTaskMemFree(psz);
        if (pmk) pmk->Release();
        if (prot) prot->Release();
        return hr;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__ActiveObject_h__
