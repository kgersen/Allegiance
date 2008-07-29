//////////////////////////////////////////////////////////////////////////////
//
// PCH.H
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _PCH_H_
#define _PCH_H_

// __MODULE__ is used by ZAssert
#define __MODULE__ "SoundEngine"

//////////////////////////////////////////////////////////////////////////////
//
// Disable some warnings
//
//////////////////////////////////////////////////////////////////////////////

// 'this' : used in base member initializer list
//#pragma warning(disable : 4355)

// identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786)


//////////////////////////////////////////////////////////////////////////////
//
// The include files
//
//////////////////////////////////////////////////////////////////////////////
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>
 
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <dsound.h>

#include <mmreg.h>	// mdvalley: Required for waveformatextensible
#include <ks.h>

// mdvalley: additional includes for the vorbis codec
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
// include for timing information
#include <sys/types.h>
#include <sys/timeb.h>

#include "zlib.h"
//#include "fixdelete.h"

#endif
