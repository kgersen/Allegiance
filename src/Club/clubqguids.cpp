/*-------------------------------------------------------------------------
  srvqguids.cpp
  
  Guids for srvqueries
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

#define SQLGUID(X) GUID X::s_guid = GUID_NULL;

/*
SQLGUID(CQLogonStats)
*/

