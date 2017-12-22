/////////////////////////////////////////////////////////////////////////////
// AGCProbe.cpp : Implementation of the CAGCProbe class.
//

#include "pch.h"
#include "AGCProbe.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCProbe

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCProbe)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCProbe::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCProbe
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

