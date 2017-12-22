
/*-------------------------------------------------------------------------
 * fedsrv\AGCSector.CPP
 * 
 * Implementation of CAGCSector
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include "AGCSector.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCSector

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCSector)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCSector::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCSector
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

