/*-------------------------------------------------------------------------
  pch.h

  Precompiled header for AllClub

  Owner:

  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _ALLCLUB_PCH_
#define _ALLCLUB_PCH_

#define ALLCLUB_MODULE 1
#pragma warning(disable:4244)

#include <windows.h>
#include <stdlib.h>
#include <memory.h>
#include <sqlext.h>
#include <dplobby.h>

// This also includes <atlbase.h> and <atlcom.h>
#include "AllClubModule.h"

#define __MODULE__ "AllClub"
#include <zlib.h>
#include <utility.h>

#include <TCLib.h>
#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\UserAcct.h>
#include <..\TCLib\TCThread.h>

#include "..\fedsrv\fedsrvsql.h"

#include <guids.h>
#include "resource.h"
#include "zone.h"
#include "messagecore.h"
#include "ClubMessages.h"
#include "sharemem.h"
#include "ClubCounters.h"
#include "zauth.h"

//$ ASYNCCLUB
// #include "clubqueries2.h"

#include "regkey.h" // KGJV added

#include "ClubApp.h"
#include "ClubClient.h"
#include "StaticData.h" //- obsolete


/////////////////////////////////////////////////////////////////////////////
// This is included very last since it conflicts with <atlwin.h>, included
// indirectly by <TCAtl.h>.
//
#include <windowsx.h> // For GlobalAllocPtr and GlobalFreePtr


/////////////////////////////////////////////////////////////////////////////

#endif
