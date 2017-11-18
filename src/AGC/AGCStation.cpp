/////////////////////////////////////////////////////////////////////////////
// AGCStation.cpp : Implementation of CAGCStation
//

#include "pch.h"
#include "AGCStation.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCStation

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCStation)


/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo Interface Methods

STDMETHODIMP CAGCStation::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCStation
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

