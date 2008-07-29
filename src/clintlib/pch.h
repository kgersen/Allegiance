#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
//#include <dplobby.h>
#include <tchar.h>

//
// Headers for fed code.
//
#define __MODULE__  "clintlib"

#include "guids.h"
#include "zlib.h"
#include "utility.h"
#include "igc.h"
#include "Zone.h"
#include "messagesAll.h"
#include "messages.h"
#include "messagesLC.h"
#include "ClubMessages.h" // KG changed location
#include "AutoDownload.h"

#ifdef USEAUTH
#include "zauth.h"
#endif
#include "clintlib.h"

// removed engine dep since color codes are in zlib now
//#include "engine.h"
