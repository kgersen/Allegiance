#ifndef _zlib_H_
#define _zlib_H_

//////////////////////////////////////////////////////////////////////////////
//
// zlib.H
//
//////////////////////////////////////////////////////////////////////////////

// warning C4355: 'this' : used in base member initializer list
#pragma warning(disable:4355)

#ifndef DREAMCAST
    #define USEASM
    #define FLOATASM
#endif

//
// Required CRT headers
//

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "malloc.h"

#ifdef DREAMCAST
#include "dreamcast.h"
#endif

//
// Required Win32 headers
//

#include "windows.h"
#include "wininet.h"

//
// DirectX Stuff
//

#define DIRECTDRAW_VERSION 0x0700
#define DIRECT3D_VERSION   0x0700

#include "ddraw.h"
#include "d3d.h"

//
// Library Headers
//

#include "zassert.h"
#include "alloc.h"
#include "mask.h"
#include "tref.h"
#include "cast.h"
#include "zmath.h"
#include "zadapt.h"

#include "tcompare.h"
#include "tarray.h"
#include "tvector.h"
#include "tstack.h"
#include "tlist.h"
#include "tmap.h"

#include "comobj.h"

//
// Types
//

#include "ztime.h"
#include "zstring.h"
#include "point.h"
#include "rect.h"
#include "vector.h"
#include "vertex.h"
#include "orientation.h"
#include "matrix.h"
#include "quaternion.h"
#include "color.h"


//
// Interfaces
//

#include "event.h"
#include "input.h"
#include "genericlist.h"
#include "FTPSession.h"

//
// Win32 Wrapper Classes
//

#include "base.h"
#include "window.h"
#include "win32app.h"
#include "VersionInfo.h"

//
// Parser
//

#include "token.h"

#endif
