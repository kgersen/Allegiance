/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	notcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "notcondition" interface.
**
**  History:
*/
#ifndef _NOT_CONDITION_H_
#define _NOT_CONDITION_H_

#ifndef _PREDICATE_H_
#include "Predicate.h"
#endif  //_PREDICATE_H_

namespace Training
{
    #define NotCondition(pCondition)  Predicate (pCondition, false)
}

#endif  //_NOT_CONDITION_H_
