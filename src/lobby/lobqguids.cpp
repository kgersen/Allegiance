/*-------------------------------------------------------------------------
  LobQGuids.cpp
  
  Guids for LobQueries
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

#define SQLGUID(X) GUID X::s_guid = GUID_NULL;

//Imago removed - 7/5/08
//#ifdef USEAUTH
SQLGUID(CQLobbyLogon)
//#endif

