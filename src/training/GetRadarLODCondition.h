/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getradarlodcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getradarlodcondition" interface.
**
**  History:
*/
#ifndef _GET_RADAR_LOD_CONDITION_H_
#define _GET_RADAR_LOD_CONDITION_H_

#ifndef _CONDITION_H_
#include "Condition.h"
#endif  //_CONDITION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetRadarLODCondition : public Condition
    {
        public:
                    /* void */          GetRadarLODCondition (RadarImage::RadarLOD expectedRadarLOD);
            virtual /* void */          ~GetRadarLODCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    RadarImage::RadarLOD    m_expectedRadarLOD;
    };

    //------------------------------------------------------------------------------
}

#endif  //_GET_RADAR_LOD_CONDITION_H_
