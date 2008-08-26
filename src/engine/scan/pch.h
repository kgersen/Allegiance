//////////////////////////////////////////////////////////////////////////////
//
// PCH.H
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _PCH_H_
#define _PCH_H_

// __MODULE__ is used by ZAssert
#define __MODULE__ "Stars"

// conversion from 'float' to 'int', possible loss of data
//#pragma warning(disable : 4244)

// 'this' : used in base member initializer list
#pragma warning(disable : 4355)

// identifier was truncated to '255' characters in the debug information
#pragma warning(disable : 4786)

#include "windows.h"
#include "zmath.h"
#include "mylib.h"
#include "scan.h"

#endif
