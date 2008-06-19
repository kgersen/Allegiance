/*-------------------------------------------------------------------------
 * pch.h
 *
 * Headers for fedsrv
 *
 * Owner:
 *
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#ifndef _FEDSRV_PCH_
#define _FEDSRV_PCH_

#define _ATL_STATIC_REGISTRY

// browse info overflow warnings for STL objects
#pragma warning(disable : 4786)
#pragma warning(disable:4018)
#pragma warning(disable:4244)

// Standard C Includes
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
// Standard C++ Includes
#include <algorithm>
#include <map>
#include <set>
#include <vector>

// Windows Includes
#include <windows.h>
#include <commctrl.h>
#include <sqlext.h>
//#include <dplobby.h>
#include <mmsystem.h>

// ZLib Includes
#if !defined(ALLSRV_STANDALONE)
  #define __MODULE__ "AllSrv"
#else // !defined(ALLSRV_STANDALONE)
  #define __MODULE__ "AllSrv32"
#endif // !defined(ALLSRV_STANDALONE)
#include <guids.h>
#include <zlib.h>

#include "resource.h"
#include <utility.h>
#include <igc.h>

#include "..\AGC\GameParamData.h"

//#include <atlbase.h>
//extern CServiceModule _Module;
//#include <atlcom.h>

#include <ZoneAuthDecrypt.h>

// This also includes <atlbase.h> and <atlcom.h>
#include "AllSrvModule.h"

#include <TCLib.h>
#include <..\TCLib\AutoHandle.h>

#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\UserAcct.h>
#include <..\TCLib\TCThread.h>


#include "Zone.h"

#include <messagesAll.h>
#include <Messages.h>
#include <MessagesLS.h>

#include "SWMRG.h"
#include "BitArray.h"
#include "point.h"
//#include "srvdbg.h"

#include "sharemem.h"
//#include "config.h"
#include "counters.h"
#include "srvqueries.h" // and allegdb.h

#include "AllSrvModuleIDL.h"
#include "AdminUtil.h"
#include "fsside.h"
#include "fscluster.h"
#include "fsmission.h"
#include "fedsrvsql.h"
#include "zauth.h"
#include "fedsrv.h"
extern Global g;
#include "fsship.h"

#include "AdminGame.h"
#include "AdminGames.h"
#include "AdminSession.h"


#include "AdminUser.h"
#include "AdminUsers.h"
#include "AdminServer.h"
#include "AdminShip.h"

#include "dbgutil.h"


#include "regkey.h"
#include "asynchttp.h"

/////////////////////////////////////////////////////////////////////////////
// This is included very last since it conflicts with <atlwin.h>, included
// indirectly by <TCAtl.h>.
//
#include <windowsx.h> // For GlobalAllocPtr and GlobalFreePtr


/////////////////////////////////////////////////////////////////////////////

#endif // _FEDSRV_PCH_

