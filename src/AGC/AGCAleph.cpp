/////////////////////////////////////////////////////////////////////////////
// AGCAleph.cpp : Implementation of CAGCAleph
//

#include "pch.h"
#include "AGCAleph.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCAleph

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCAleph)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCAleph::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCAleph
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

