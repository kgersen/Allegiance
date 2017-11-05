/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	Mission6.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the sixth training mission
**
**  History:
*/
#include    "pch.h"
#include    "Mission6.h"

#include    "Training.h"
#include    "GoalList.h"
#include    "ConditionalAction.h"
#include    "GetShipIsDestroyedCondition.h"
#include    "FalseCondition.h"
#include    "ElapsedTimeCondition.h"
#include    "SetDisplayModeAction.h"
#include    "CreateDroneAction.h"
#include    "MessageAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  Mission6::~Mission6 (void)
    {
    }

    //------------------------------------------------------------------------------
    int         Mission6::GetMissionID (void)
    {
        return c_TM_6_Practice_Arena;
    }

    //------------------------------------------------------------------------------
    SectorID    Mission6::GetStartSectorID (void)
    {
        return 1030;
    }

    //------------------------------------------------------------------------------
    bool        Mission6::RestoreShip (void)
    {
        // end the mission
        Terminate ();

        // and return false so the engine stays in the override camera mode
        return false;
    }

    //------------------------------------------------------------------------------
    void        Mission6::CreateUniverse (void)
    {
        LoadUniverse ("training_3", 488, 1030);    // a fighter 3

        // activate all the starting weapons
        trekClient.fGroupFire = true;

        // put the commander ship in the station
        ImissionIGC*    pCore = trekClient.GetCore();
        ImodelIGC*      pStation = pCore->GetModel (OT_station, 1030);
        IshipIGC*       pCommander = pCore->GetShip (m_commanderID);
        pCommander->SetStation (static_cast<IstationIGC*> (pStation));
        pCommander->SetCommand (c_cmdAccepted, NULL, c_cidDoNothing);
        pCommander->SetCommand (c_cmdCurrent, NULL, c_cidDoNothing);
        pCommander->SetAutopilot (false);
    }

    //------------------------------------------------------------------------------
    Condition*  Mission6::CreateMission (void)
    {
        GoalList*   pGoalList = new GoalList;

        // play the introductory audio
        pGoalList->AddGoal (CreatePlaySoundGoal (salCommenceScanSound));

        // wait half  second
        pGoalList->AddGoal (new Goal (new ElapsedTimeCondition (0.5f)));

		// tm_6_01
		// Okay, Cadet, here's your Advanced Fighter. There's a base 
		// outpost here in case you need to replenish yourself.
        pGoalList->AddGoal (CreatePlaySoundGoal (tm_6_01Sound));

        // wait two more seconds in cockpit
        {
	        Goal*   pGoal = new Goal(new ElapsedTimeCondition(2.0f));
	        pGoal->AddStartAction(new SetDisplayModeAction(TrekWindow::cmCockpit));
	        pGoalList->AddGoal(pGoal);
        }

		// tm_6_02
		// Iron Coalition Intelligence reports inbound enemy craft. 
		// They're using an old code for their comms, so we can hear 
		// them. Watch your message stream to see what they're up to.
        {
            Goal*   pGoal = CreatePlaySoundGoal (tm_6_02Sound);
            pGoal->AddStartAction (new MessageAction ("Watch the chat to see what the enemy craft are after."));
            pGoalList->AddGoal (pGoal);
        }

        // wait half  second
        pGoalList->AddGoal (new Goal (new ElapsedTimeCondition (0.5f)));

		// tm_6_03
		// DEFEND THE MINERS! This simulation is over when you die.
        pGoalList->AddGoal (CreatePlaySoundGoal (tm_6_03Sound));

        // wait two more seconds
        pGoalList->AddGoal (new Goal (new ElapsedTimeCondition (2.0f)));

        // need this
        ImissionIGC*        pMission = trekClient.GetCore();
        ImodelIGC*          pStation = pMission->GetModel (OT_station, 1030);

        // wait for player to be dead
        {
            Goal*               pGoal = new Goal (new FalseCondition);

            // create enemy ships
            ShipID              enemyShipID = pMission->GenerateNewShipID ();
            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction ("Enemy Support", enemyShipID, 310, 1, c_ptWingman);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), Vector (3800.0f, 4275.0f, 855.0f));
            Condition*          pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2 (OT_ship, static_cast<ObjectID> (enemyShipID));
            pGoal->AddConstraintCondition (new ConditionalAction (pShipIsDestroyedcondition, pCreateDroneAction));

            enemyShipID = pMission->GenerateNewShipID ();
            pCreateDroneAction = new CreateDroneAction ("Enemy Fighter", enemyShipID, 315, 1, c_ptWingman);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), Vector (3800.0f, 4175.0f, 855.0f));
            pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2 (OT_ship, static_cast<ObjectID> (enemyShipID));
            pGoal->AddConstraintCondition (new ConditionalAction (pShipIsDestroyedcondition, pCreateDroneAction));

            pGoalList->AddGoal (pGoal);
        }

        // build the goal that we'll return
        {
            Goal*   pGoal = new Goal (pGoalList);

            // create friendly miners
            Vector              pos = pStation->GetPosition ();
            pos.x += random(-1000.0f, 1000.0f);
            pos.y += random(-1000.0f, 1000.0f);
            pos.z += random(-1000.0f, 1000.0f);

            ShipID              minerShipID = pMission->GenerateNewShipID ();
            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction ("Miner 01", minerShipID, 436, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), pos);
            Condition*          pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2 (OT_ship, static_cast<ObjectID> (minerShipID));
            pGoal->AddConstraintCondition (new ConditionalAction (pShipIsDestroyedcondition, pCreateDroneAction));

            pos = pStation->GetPosition ();
            pos.x += random(-1000.0f, 1000.0f);
            pos.y += random(-1000.0f, 1000.0f);
            pos.z += random(-1000.0f, 1000.0f);

            minerShipID = pMission->GenerateNewShipID ();
            pCreateDroneAction = new CreateDroneAction ("Miner 02", minerShipID, 436, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), pos);
            pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2 (OT_ship, static_cast<ObjectID> (minerShipID));
            pGoal->AddConstraintCondition (new ConditionalAction (pShipIsDestroyedcondition, pCreateDroneAction));

            pos = pStation->GetPosition ();
            pos.x += random(-1000.0f, 1000.0f);
            pos.y += random(-1000.0f, 1000.0f);
            pos.z += random(-1000.0f, 1000.0f);

            minerShipID = pMission->GenerateNewShipID ();
            pCreateDroneAction = new CreateDroneAction ("Miner 03", minerShipID, 436, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), pos);
            pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2 (OT_ship, static_cast<ObjectID> (minerShipID));
            pGoal->AddConstraintCondition (new ConditionalAction (pShipIsDestroyedcondition, pCreateDroneAction));

            pos = pStation->GetPosition ();
            pos.x += random(-1000.0f, 1000.0f);
            pos.y += random(-1000.0f, 1000.0f);
            pos.z += random(-1000.0f, 1000.0f);

            minerShipID = pMission->GenerateNewShipID ();
            pCreateDroneAction = new CreateDroneAction ("Miner 04", minerShipID, 436, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), pos);
            pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2 (OT_ship, static_cast<ObjectID> (minerShipID));
            pGoal->AddConstraintCondition (new ConditionalAction (pShipIsDestroyedcondition, pCreateDroneAction));

            return pGoal;
        }
    }

    //------------------------------------------------------------------------------
}


