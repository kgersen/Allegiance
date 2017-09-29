//////////////////////////////////////////////////////////////////////////////
//
// PCH.H
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _PCH_H_
#define _PCH_H_

#define __MODULE__ "Utility"

//
// C headers
//

#include "stdio.h"
#include "string.h"
#include <stdarg.h>
#include <time.h>
#include <malloc.h>
// mdvalley: Another include to shut the ATL libraries up.
#include <stddef.h>

//
// Windows headers
//

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include <urlmon.h>

// BT - STEAM
#include "steam_gameserver.h"

//
// Wintrek headers
//
#include "guids.h"
#include "zlib.h"
#include "utility.h"
//#include "zauth.h" // KG removed


#endif
