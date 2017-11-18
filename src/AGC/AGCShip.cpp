/////////////////////////////////////////////////////////////////////////////
// AGCShip.cpp : Implementation of CAGCShip
//

#include "pch.h"
#include "AGCShip.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCShip

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCShip)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCShip::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCShip
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


