/*
**
**  File:	setstationdestroyedaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setstationdestroyedaction" interface.
**
**  History:
*/
#ifndef _SET_STATION_DESTROYED_ACTION_H_
#define _SET_STATION_DESTROYED_ACTION_H_

#ifndef _ACTION_H_
#include "Action.h"
#endif  //_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetStationDestroyedAction : public Action
    {
        public:
                    /* void */          SetStationDestroyedAction (ImodelIGC* theStation);
            virtual /* void */          ~SetStationDestroyedAction (void);
            virtual void                Execute (void);

        protected:
                    ImodelIGC* pStation;
    };

    //------------------------------------------------------------------------------
}

#endif  //_SET_STATION_DESTROYED_ACTION_H_
