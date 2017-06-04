/////////////////////////////////////////////////////////////////////////////
// InvokeArgs.cpp | Implementation of the TCInvokeArgs class.

#include "pch.h"
#include <TCLib.h>
#include "InvokeArgs.h"


/////////////////////////////////////////////////////////////////////////////  
// TCInvokeArgs


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Deletes the dispatch arguments structure, if one was created.
//
// See Also: TCInvokeArgs::SetDispParams
TCInvokeArgs::~TCInvokeArgs()
{
  if (rgvarg)
    Empty();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Invoke method.
//
// This method calls *Invoke* on the specified *IDispatch* pointer,
// specifying m_dispid as the dispatch ID, and passing
// the arguments specified in a prior call to SetDispParams.
//
// Parameters:
//   pdisp - The *IDispatch* pointer on which to call *Invoke*.
//
// Return Value: The HRESULT of the *IDispatch::Invoke* call is returned.
//
// See Also: TCInvokeArgs::SetDispParams, TCInvokeArgs::m_dispid
HRESULT TCInvokeArgs::Invoke(IDispatch* pdisp, VARIANT* pvarResult)
{
  HRESULT hr = S_OK;
  if (NULL != pdisp)
  {
    __try
    {
      // Invoke the dispatch method
      hr = pdisp->Invoke(m_dispid, IID_NULL, LOCALE_USER_DEFAULT,
        DISPATCH_METHOD, this, pvarResult, NULL, NULL);
    }
    __except(1)
    {
      hr = E_UNEXPECTED;
    }
  }

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the dispatch ID and dispatch parameters for subsequent
// *Invoke* calls.
//
// This method saves the specified /dispid/ and creates an array of
// *VARIANTARG* structures suitable for use with the
// *IDispatch::Invoke* method.
//
// For each pair of arguments in the specified variable argument list,
// /argptr/, a *VARIANTARG* is created and initialized. The *VARIANTARG* is
// then assigned the variant type and variant value specified in the argument
// pair. So, for example, the following would set three dispatch parameters,
// of types *VT_BOOL*, *VT_I4*, and *VT_BSTR* to *VARIANT_TRUE*, 137, and
// "Hello", respectively:
//
//      CComBSTR bstrText("Hello");
//      pCall->SetDispParams(DISPID_HELLO, 3, VT_BOOL, VARIANT_TRUE,
//        VT_I4, 137, VT_BSTR, bstrText);
//
// Note: Notice that although 3 is specified as the /cArgs/ parameter, the
// variable argument list contains six actual parameters. This is because for
// each argument specified in /cArgs/, an argument /pair/ is implied, since
// each dispatch argument has a variant type and value.
//
// Parameters:
//   dispid - The dispatch ID used in subsequent *Invoke* calls.
//   cArgs - The number of dispatch arguments specified in the variable
// argument list.
//   argptr - The standardized representation of a variable argument list.
//
// See Also: TCInvokeArgs::SetDispParams, TCInvokeArgs::Invoke
void TCInvokeArgs::SetDispParamsV(DISPID dispid, UINT nArgs, va_list argptr)
{
  // Reinitialize
  if (rgvarg)
    Empty();

  // Save the specified dispatch id
  m_dispid = dispid;

  // Return immediately if 0 arguments specified
  if (!nArgs)
    return;

  // Create the VARIANTARG array with the specified number of arguments
  rgvarg = new VARIANTARG[cArgs = nArgs];

  // Loop thru each argument and initialize the VARIANTARG array element
  while (nArgs--)
  {
    VariantInit(&rgvarg[nArgs]);
    VARTYPE vt = va_arg(argptr, VARTYPE);
    switch (V_VT(&rgvarg[nArgs]) = vt)
    {
      case VT_BOOL:
        V_BOOL(&rgvarg[nArgs]) = va_arg(argptr, VARIANT_BOOL);
        break;
      case VT_I1:
        V_I1(&rgvarg[nArgs]) = va_arg(argptr, CHAR);
        break;
      case VT_I2:
        V_I2(&rgvarg[nArgs]) = va_arg(argptr, SHORT);
        break;
      case VT_I4:
        V_I4(&rgvarg[nArgs]) = va_arg(argptr, LONG);
        break;
      case VT_UI1:
        V_UI1(&rgvarg[nArgs]) = va_arg(argptr, BYTE);
        break;
      case VT_UI2:
        V_UI2(&rgvarg[nArgs]) = va_arg(argptr, USHORT);
        break;
      case VT_ERROR:
        V_ERROR(&rgvarg[nArgs]) = va_arg(argptr, SCODE);
        break;
      case VT_R4:
      {
        // ... pushes a float argument onto the stack as a double
        DOUBLE dblTemp = va_arg(argptr, DOUBLE);
        V_R4(&rgvarg[nArgs]) = (float)dblTemp;
        break;
      }
      case VT_R8:
        V_R8(&rgvarg[nArgs]) = va_arg(argptr, DOUBLE);
        break;
      case VT_DECIMAL:
        V_DECIMAL(&rgvarg[nArgs]) = va_arg(argptr, DECIMAL);
        break;
      case VT_CY:
        V_CY(&rgvarg[nArgs]) = va_arg(argptr, CY);
        break;
      case VT_DATE:
        V_DATE(&rgvarg[nArgs]) = va_arg(argptr, DATE);
        break;
      case VT_BSTR:
        V_BSTR(&rgvarg[nArgs]) = va_arg(argptr, BSTR);
        break;
      case VT_UNKNOWN:
        V_UNKNOWN(&rgvarg[nArgs]) = va_arg(argptr, IUnknown*);
        break;
      case VT_DISPATCH:
        V_DISPATCH(&rgvarg[nArgs]) = va_arg(argptr, IDispatch*);
        break;
      case VT_VARIANT:
        V_VARIANTREF(&rgvarg[nArgs]) = va_arg(argptr, VARIANT*);
        break;
      default:
        if (vt & VT_BYREF)
        {
          V_BYREF(&rgvarg[nArgs]) = va_arg(argptr, void*);
          break;
        }
        _TRACE0("TCInvokeArgs::SetDispParamsV(): ");
        _TRACE2("Specified VARTYPE %hu (0x%04X) is unsupported\n", vt, vt);
        _ASSERT(false);
    }
  }
} 

