#pragma once

#define _WIN32_WINNT 0x0A00

#define __MODULE__ "AutoUpdate"

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>

#include <commctrl.h>


#include "zlib.h"
#include "Utility.h"
#include "zreg.h"
#include "AutoDownload.h"

