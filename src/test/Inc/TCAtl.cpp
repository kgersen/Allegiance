#ifndef __TCAtl_cpp__
#define __TCAtl_cpp__

#pragma once


/////////////////////////////////////////////////////////////////////////////
// TCAtl.cpp | Implementation of the classes declared in TCAtl.h

#include "TCAtl.h"
#include <map>
#include <string>
#include <..\TCLib\AutoCriticalSection.h>
#include <..\TCLib\ObjectLock.h>
#include "..\TCAtl\TCNullStreamImpl.h"


/////////////////////////////////////////////////////////////////////////////
// IID Interpretation

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the human-friendly name of a specified interface
// identifier (IID).
//
// Converts the specified IID reference to a string representation. The
// string is either the human-friendly name of the interface, as stored in
// the registry, or the standard GUID string representation if the interface
// name cannot be resolved from the registry.
//
// Once an IID is resolved to a string, it is added to a static map so that
// subsequent conversions can resolve the same IID very quickly. An STL
// *std::map* is used for this, along with an TCAutoCriticalSection to
// synchronize multi-threaded access to the map.
//
// Parameters:
//   riid - A constant reference to the IID to be converted to a string
// representation.
//
// Return Value: An LPCTSTR (const TCHAR*) that points to the string
// representation of the specified /riid/.
LPCTSTR TCGetIIDName(REFIID riid)
{
  typedef std::basic_string<TCHAR> tstring;
  typedef std::map<IID, tstring> IIDMap;
  static IIDMap map;
  static TCAutoCriticalSection cs;
  TCObjectLock<TCAutoCriticalSection> lock(&cs);

  // Look for the specified IID in the map
  IIDMap::iterator it = map.find(riid);
  if (map.end() != it)
    return (*it).second.c_str();

  // Convert the specified IID into a string representation
  WCHAR szIID[48];
  StringFromGUID2(riid, szIID, sizeofArray(szIID));
  USES_CONVERSION;
  LPCTSTR pszIID = OLE2CT(szIID);

  // Attempt to find it in the interfaces section
  TCHAR szName[_MAX_PATH] = TEXT("");
  DWORD dw = sizeof(szName);
  CRegKey key;
  LONG lr = key.Open(HKEY_CLASSES_ROOT, TEXT("Interface"));
  if (ERROR_SUCCESS == lr && ERROR_SUCCESS == (lr = key.Open(key, pszIID)))
	  // mdvalley: no such thing as QueryStringValue!
    if (ERROR_SUCCESS == key.QueryValue(NULL, szName, &dw))
      pszIID = szName;

  // Add the IID and it's string representation to the map
  tstring str(pszIID);
  map.insert(IIDMap::value_type(riid, str));

  // Return the string
  return str.c_str();
}


/////////////////////////////////////////////////////////////////////////////
// SafeArray Streaming

/////////////////////////////////////////////////////////////////////////////
// Description: Reads a *SAFEARRAY* from an *IStream* as a *CComVariant*.
//
// The ATL *CComVariant* class supports the reading and writing of itself to
// an *IStream* interface pointer for most [oleautomation] variant types.
// However, it does *not* support reading and writing of any of those variant
// types in a *SAFEARRAY*. This function, along with the complementary
// TCWriteSafeArrayToStream function, fills in that gap.
//
// Currently, only 1-dimensional arrays are supported.
// Note: The next variant type in the stream does not need to be a
// *SAFEARRAY*. If the next variant type read from the stream is not a
// *SAFEARRAY*, the function simply returns *S_FALSE*. This allows the
// function to be called first, prior to performing any default processing,
// such as that supplied by *CComVariant::ReadFromStream*.
//
// Parameters:
//   pstm - An *IStream* interface pointer from which the *SAFEARRAY* is to
// be read.
//   var - A reference to a *CComVariant* object in which the *SAFEARRAY* is
// to be created.
//
// Return Value: *S_FALSE* if the next variant in the stream is not a
// *SAFEARRAY*, *S_OK* if the *SAFEARRAY* was successfully read from the
// stream. Otherwise, a standard *HRESULT* indicating the error that
// occurred.
//
// See Also: TCWriteSafeArrayToStream
HRESULT TCReadSafeArrayFromStream(IStream* pstm, CComVariant& var)
{
  // Read the next VARTYPE from the stream
  VARTYPE vtRead;
  RETURN_FAILED(pstm->Read(&vtRead, sizeof(vtRead), NULL));

  // Return S_FALSE if the VARTYPE is not an array
  if (!(vtRead & VT_ARRAY))
    return S_FALSE;

  // Get the VARTYPE of the SAFEARRAY elements
  VARTYPE vt = vtRead & ~VT_ARRAY;

  // Read the number of dimensions in the SAFEARRAY
  UINT nDim = 0;
  RETURN_FAILED(pstm->Read(&nDim, sizeof(nDim), NULL));

  // Only 1-dimensional arrays are currently supported
  if (1 != nDim)
    return E_UNEXPECTED;

  // Allocate an array of SAFEARRAYBOUND structures on the stack
  UINT nByteSize = nDim * sizeof(SAFEARRAYBOUND);
  SAFEARRAYBOUND* psab = (SAFEARRAYBOUND*)_alloca(nByteSize);
  if (!psab)
    return E_OUTOFMEMORY;
  ZeroMemory(psab, nByteSize);

  // Read the element count and lower bound of each dimension
  for (UINT iDim = 0; iDim < nDim; ++iDim)
  {
    SAFEARRAYBOUND& sab = psab[iDim];
    RETURN_FAILED(pstm->Read(&sab.cElements, sizeof(sab.cElements), NULL));
    RETURN_FAILED(pstm->Read(&sab.lLbound, sizeof(sab.lLbound), NULL));
  }

  // Allocate the SAFEARRAY
  SAFEARRAY* psa = SafeArrayCreate(vt, nDim, psab);
  if (!psa)
    return E_OUTOFMEMORY;

  // Lock the SAFEARRAY for this scope
  RETURN_FAILED(SafeArrayLock(psa));
  TCSafeArrayLock lock(psa);

  // Get the byte size of each element
  UINT cbElement = SafeArrayGetElemsize(psa);

  // Get the number of elements in the 1st (and only) dimension
  long nElements = psa->rgsabound[0].cElements;

  // Read each element of the 1st (and only) dimension
  for (long i = 0; i < nElements; ++i)
  {
    // Read the CComVariant from the stream
    CComVariant varTemp;
    RETURN_FAILED(varTemp.ReadFromStream(pstm));
    
    // Ensure that the CComVariant is of the correct type
    if (vt != V_VT(&varTemp))
      return E_UNEXPECTED;

    // Get a pointer to the next element
    void* pvElement = NULL;
    RETURN_FAILED(SafeArrayPtrOfIndex(psa, &i, &pvElement));

    // Copy the CComVariant to the SAFEARRAY element
    CopyMemory(pvElement, &V_BYREF(&varTemp), cbElement);

    // Detach the VARIANT since the SAFEARRAY now owns it
    V_VT(&varTemp) = VT_EMPTY;
  }

  // Attach the SAFEARRAY to the specified CComVariant
  V_VT(&var) = vtRead;
  V_ARRAY(&var) = psa;

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Writes a *SAFEARRAY* in a *CComVariant* to an *IStream*.
//
// The ATL *CComVariant* class supports the reading and writing of itself to
// an *IStream* interface pointer for most [oleautomation] variant types.
// However, it does *not* support reading and writing of any of those variant
// types in a *SAFEARRAY*. This function, along with the complementary
// TCReadSafeArrayFromStream function, fills in that gap.
//
// Currently, only 1-dimensional arrays are supported.
//
// Note: The *CComVariant* reference specified by the /var/ parameters does
// not need to contain a *SAFEARRAY* variant type. If the variant type is not
// a *SAFEARRAY*, the function simply returns *S_FALSE*. This allows the
// function to be called first, prior to performing any default processing,
// such as that supplied by *CComVariant::WriteToStream*.
//
// Parameters:
//   pstm - An *IStream* interface pointer into which the *SAFEARRAY* is to
// be written.
//   var - A reference to a *CComVariant* object which may contain a
// *SAFEARRAY* variant type.
//
// Return Value: *S_FALSE* if the specified variant is not a *SAFEARRAY*,
// *S_OK* if the *SAFEARRAY* was successfully written to the stream.
// Otherwise, a standard *HRESULT* indicating the error that occurred.
//
// See Also: TCReadSafeArrayFromStream
HRESULT TCWriteSafeArrayToStream(IStream* pstm, CComVariant& var)
{
  // Write the VARTYPE of the VARIANT
  VARTYPE vt = V_VT(&var);
  RETURN_FAILED(pstm->Write(&vt, sizeof(vt), NULL));

  // Return S_FALSE if the VARTYPE is not an array
  if (!(vt & VT_ARRAY))
    return S_FALSE;

  // Get the VARTYPE of the SAFEARRAY elements
  vt &= ~VT_ARRAY;

  // Get the SAFEARRAY pointer of the VARIANT
  SAFEARRAY* psa = V_ARRAY(&var);

  // Lock the SAFEARRAY for this scope
  RETURN_FAILED(SafeArrayLock(psa));
  TCSafeArrayLock lock(psa);

  // Get the number of dimensions in the SAFEARRAY
  UINT nDim = SafeArrayGetDim(psa);

  // Only 1-dimensional arrays are currently supported
  if (1 != nDim)
    return E_INVALIDARG;

  // Write the number of dimensions in the SAFEARRAY
  RETURN_FAILED(pstm->Write(&nDim, sizeof(nDim), NULL));

  // Write the element count and lower bound of each dimension
  for (UINT iDim = 0; iDim < nDim; ++iDim)
  {
    SAFEARRAYBOUND& sab = psa->rgsabound[iDim];
    RETURN_FAILED(pstm->Write(&sab.cElements, sizeof(sab.cElements), NULL));
    RETURN_FAILED(pstm->Write(&sab.lLbound, sizeof(sab.lLbound), NULL));
  }

  // Get the byte size of each element
  UINT cbElement = SafeArrayGetElemsize(psa);

  // Get the number of elements in the 1st (and only) dimension
  long nElements = psa->rgsabound[0].cElements;

  // Write each element of the 1st (and only) dimension
  for (long i = 0; i < nElements; ++i)
  {
    // Get a pointer to the next element
    void* pvElement = NULL;
    RETURN_FAILED(SafeArrayPtrOfIndex(psa, &i, &pvElement));

    // Create a VARIANT around the element
    CComVariant varTemp;
    V_VT(&varTemp) = vt;
    CopyMemory(&V_BYREF(&varTemp), pvElement, cbElement);

    // Write the CComVariant to the stream
    HRESULT hr = varTemp.WriteToStream(pstm);

    // Detach the VARIANT since the SAFEARRAY still owns it
    V_VT(&varTemp) = VT_EMPTY;

    // Return if the Write operation failed
    RETURN_FAILED(hr);
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Object Stream Size Determination

/////////////////////////////////////////////////////////////////////////////
// Description: Computes the size of stream necessary to persist an object
// that supports *IPersistStream* or *IPersistStreamInit*.
//
// Call this function to compute the size of stream necessary to persist an
// object that supports *IPersistStream* or *IPersistStreamInit*. The size
// is computed by actually persisting the object to an TCNullStream object,
// which does nothing more than accumulate the size of all data written to
// it.
//
// Parameters:
//   punk - [in] The object for which the persistence size is to be computed.
// This object must support either the *IPersistStream* or
// *IPersistStreamInit* interface.
//   pCbSize - [out] A pointer to a ULARGE_INTEGER structure where the
// computed size will be returned.
//
// Return Value: If failure occurs in either the *IPersistStream::Save* or
// the *IStream::Seek* methods, that failure code is returned. Otherwise, one
// of the following values:
//
//   S_OK - The function succeeded
//   E_NOINTERFACE - The specified object does not support
// *IPersistStream* or *IPersistStreamInit*.
//   E_POINTER - The specified [out] parameter, /pCbSize/, is an invalid
// memory address, or the specified interface pointer, /punk/, is not valid.
//
// See Also: TCNullStream, CTCNullStream, TCPersistStreamInitImpl
HRESULT TCGetPersistStreamSize(IUnknown* punk, ULARGE_INTEGER* pCbSize)
{
  // Initialize the [out] parameter
  TCZeroMemory(pCbSize);

  // Get the IPersistStream(Init) interface of the specified object
  IPersistStreamPtr sppstm;
  HRESULT hr = TCSafeQI(punk, &sppstm);
  if (E_NOINTERFACE == hr)
    hr = punk->QueryInterface(IID_IPersistStreamInit, (void**)&sppstm);
  RETURN_FAILED(hr);

  // Create a null stream
  TCNullStreamImpl stm;

  // 'Persist' the specified object to the null stream
  RETURN_FAILED(sppstm->Save(&stm, FALSE));

  // Get the 'size' of the null stream
  LARGE_INTEGER dlibMove = {0, 0};
  return stm.Seek(dlibMove, STREAM_SEEK_END, pCbSize);
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCAtl_cpp__
