
/*-------------------------------------------------------------------------
 * fedsrv\AdminAGCTeam.CPP
 * 
 * Implementation of CAGCTeam
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include "AGCTeam.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCTeam

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCTeam)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCTeam::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCTeam
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

