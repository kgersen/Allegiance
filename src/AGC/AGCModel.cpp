/////////////////////////////////////////////////////////////////////////////
// AGCModel.cpp : Implementation of the CAGCModel class.
//

#include "pch.h"
#include "AGCModel.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCModel

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCModel)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCModel::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCModel
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

