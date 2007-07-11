/*
**
**  File:	mission8.h
**
**  Author:  Adam Davison
**
**  Description:
**      Header file for the training library "Mission Nan" interface.
**
**  History:
*/

#ifndef _MISSION_NAN_H_
#define _MISSION_NAN_H_

#ifndef _TRAINING_MISSION_H_
#include "TrainingMission.h"
#endif

#ifndef _GOAL_H_
#include "Goal.h"
#endif


namespace Training
{

	class Mission8 : public TrainingMission
	{
        public:
            virtual /* void */              ~Mission8 (void);
            virtual int                     GetMissionID (void);
            virtual SectorID                GetStartSectorID (void);
            virtual bool                    RestoreShip (void);

        protected:
            virtual void                    CreateUniverse (void);
            virtual Condition*              CreateMission (void);

                    Goal*                   CreateGoal01 (void);
                    Goal*                   CreateGoal02 (void);
                    Goal*                   CreateGoal03 (void);
                    Goal*                   CreateGoal04 (void);
                    Goal*                   CreateGoal05 (void);
                    Goal*                   CreateGoal06 (void);
                    Goal*                   CreateGoal07 (void);
                    Goal*                   CreateGoal08 (void);
                    Goal*                   CreateGoal09 (void);
					ImodelIGC*              pShip;
					ImissionIGC*			pMission;
					ImodelIGC*				pStation;
					ShipID					bomberShipID;
					ShipID					nanShipID;
    };
}

#endif
