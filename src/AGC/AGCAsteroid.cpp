/////////////////////////////////////////////////////////////////////////////
// AGCAsteroid.cpp : Implementation of CAGCAsteroid
//

#include "pch.h"
#include "AGCAsteroid.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCAsteroid

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCAsteroid)


/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo Interface Methods

STDMETHODIMP CAGCAsteroid::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCAsteroid
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

