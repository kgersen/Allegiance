/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	Mission5.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "Mission5" interface.
**
**  History:
*/
#ifndef _MISSION_FOUR_H_
#define _MISSION_FOUR_H_

#ifndef _TRAINING_MISSION_H_
#include "TrainingMission.h"
#endif  //_TRAINING_MISSION_H_

#ifndef _GOAL_H_
#include "Goal.h"
#endif  //_GOAL_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Mission5 : public TrainingMission
    {
        public:
            virtual /* void */              ~Mission5 (void);
            virtual int                     GetMissionID (void);
            virtual SectorID                GetStartSectorID (void);
            virtual void                    SetupShipAndCamera (void);
            virtual bool                    RecordKeyPress (TrekKey key);

        protected:
                    ShipID                  m_enemyScoutID;
                    ShipID                  m_miner1ID;
                    ShipID                  m_miner2ID;
                    ShipID                  m_miner3ID;
                    ShipID                  m_miner4ID;
                    ShipID                  m_scoutID;
                    ShipID                  m_builder1ID;
                    ShipID                  m_builder2ID;
                    ShipID                  m_builder3ID;

        protected:
            virtual void                    CreateUniverse (void);
            virtual Condition*              CreateMission (void);

                    Goal*                   CreateGoal01 (void);
                    Goal*                   CreateGoal02 (void);
                    Goal*                   CreateGoal03 (void);
                    Goal*                   CreateGoal04 (void);
                    Goal*                   CreateGoal05 (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_MISSION_FOUR_H_
