/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	elapsedtimecondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "elapsedtimecondition" interface.
**
**  History:
*/
#ifndef _ELAPSED_TIME_CONDITION_H_
#define _ELAPSED_TIME_CONDITION_H_

#ifndef _PERIODIC_CONDITION_H_
#include "PeriodicCondition.h"
#endif  //_PERIODIC_CONDITION_H_

#ifndef _N_TIMES_CONDITION_H_
#include "NTimesCondition.h"
#endif  //_N_TIMES_CONDITION_H_

#ifndef _TRUE_CONDITION_H_
#include "TrueCondition.h"
#endif  //_TRUE_CONDITION_H_

namespace Training
{
    #define ElapsedTimeCondition(fTime)  NTimesCondition (new PeriodicCondition (new TrueCondition, fTime), 2, true)
}

#endif  //_ELAPSED_TIME_CONDITION_H_
