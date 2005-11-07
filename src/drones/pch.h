// pch.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#define __MODULE__ "WinTrek"

#pragma warning(disable:4786)

//
// C Headers
//

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

//
// WinTrek headers
//

#include "zlib.h"
#include "utility.h"
#include "igc.h"





//
// Drone headers
//

//#define DRONES_DEBUG			// uncommenting this will enable control spews for all of the drone ships... It's a lot of info

#include "drones.h"			
#include "drone.h"
#include "goal.h"
#include "mining.h"
#include "turret.h"
#include "actionlib.h"
