/*
**  File:	Mission10.cpp
**
**  Description:
**      Implementation of the tenth pseudo training mission "Free Flight", intended for dev use.
**      Cloned mission 6 without intro and with less constraints.
*/
#include    "pch.h"
#include    "Mission10.h"

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
    /* void */  Mission10::~Mission10(void)
    {
    }

    //------------------------------------------------------------------------------
    int         Mission10::GetMissionID(void)
    {
        return c_TM_10_Free_Flight;
    }

    //------------------------------------------------------------------------------
    SectorID    Mission10::GetStartSectorID(void)
    {
        //return 1030; //old map
        return 2080; //new map
    }

    //------------------------------------------------------------------------------
    bool        Mission10::RestoreShip(void)
    {
        // end the mission
        Terminate();

        // and return false so the engine stays in the override camera mode
        return false;
    }

    //------------------------------------------------------------------------------
    void        Mission10::CreateUniverse(void)
    {
        LoadUniverse("training_6", 488, 1030);    // 488 = Adv. IC fighter
        
        ImissionIGC*    pCore = trekClient.GetCore();

        //output map info
        const ClusterListIGC *clusters = pCore->GetClusters();
        for (ClusterLinkIGC* cLink = clusters->first(); cLink != NULL; cLink = cLink->next())
        {
            IclusterIGC* pCluster = cLink->data();
            debugf("Cluster %s - ObjectID: %d\n", pCluster->GetName(), pCluster->GetObjectID());
        }
        const StationListIGC *stations = pCore->GetStations();
        for (StationLinkIGC* cLink = stations->first(); cLink != NULL; cLink = cLink->next())
        {
            IstationIGC* pStation = cLink->data();
            debugf("Station %s - ObjectID: %d\n", pStation->GetName(), pStation->GetObjectID());
        }


        trekClient.fGroupFire = true;           // activate all the starting weapons

        // put the commander ship in the station
        ImodelIGC*      pStation = pCore->GetModel(OT_station, 1030);
        IshipIGC*       pCommander = pCore->GetShip(m_commanderID);
        pCommander->SetStation(static_cast<IstationIGC*> (pStation));
        pCommander->SetCommand(c_cmdAccepted, NULL, c_cidDoNothing);
        pCommander->SetCommand(c_cmdCurrent, NULL, c_cidDoNothing);
        pCommander->SetAutopilot(false);

        /*// Adjust loadout
        IshipIGC*       pShipU = trekClient.GetShip();
        // destroy mounted dumbfires
        IpartIGC*       pPart = pShipU->GetMountedPart(ET_Magazine, 0);
        if (pPart)
            pPart->Terminate();;
        // Mount Seeker 2s
        AddPartToShip(154, 0, 5);
        // Change missiles in cargo for Seeker 2s
        for (int i = -1; i >= -c_maxCargo; i--) {
            pPart = pShipU->GetMountedPart(NA, i);
            if (pPart->GetEquipmentType() == ET_Magazine)
            {
                pPart->Terminate();
                AddPartToShip(154, i, 5);
            }
        }
        trekClient.SaveLoadout(pShipU);
        //Add tech, so Seeker2s don't show as new treassure
        IshipIGC*       pShip = trekClient.GetShip();
        debugf("Pre DevelopmentTechs: %d\n", pShip->GetSide()->GetDevelopmentTechs());
        TechTreeBitMask ttbm;
        ttbm.ClearAll();
        ttbm |= trekClient.GetCore()->GetPartType(154)->GetEffectTechs(); //Seeker 2 tech
        pShip->GetSide()->ApplyDevelopmentTechs(ttbm);
        debugf("Post DevelopmentTechs: %d\n", pShip->GetSide()->GetDevelopmentTechs()); //same as before, but works*/
    }

    //------------------------------------------------------------------------------
    Condition*  Mission10::CreateMission(void)
    {
        debugf("Creating mission 10.\n");
        m_commandViewEnabled = true;

        GoalList*   pGoalList = new GoalList;

        ImissionIGC*        pMission = trekClient.GetCore();
        ImodelIGC*          pHomeStation = pMission->GetModel(OT_station, 2080);

        // create friendly miner
        {
            Goal*	pGoal = new Goal(new ElapsedTimeCondition(0.1f));

            ShipID          minerID = pMission->GenerateNewShipID();
            Vector          pos = Vector(0.0f, 2500.0f, 100.0f); //create forward to draw aggro

            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Miner 01", minerID, 438, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);
            pGoal->AddStartAction(pCreateDroneAction);

            pGoalList->AddGoal(pGoal);
        }

        // play the introductory audio and change camera
        {
            Goal*   pGoal = CreatePlaySoundGoal(salCommenceScanSound);
            pGoal->AddStartAction(new SetDisplayModeAction(TrekWindow::cmCockpit)); //this needs some delay prior, so stuff is initialized
            pGoalList->AddGoal(pGoal);
        }


        // create friendly miner
        {
            Goal*	pGoal = new Goal(new ElapsedTimeCondition(0.2f));

            ShipID          minerID = pMission->GenerateNewShipID();
            Vector          pos = pHomeStation->GetPosition();
            pos.x += -250.0f; //simulate undock
            pos.y += 200.0f;

            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Miner 02", minerID, 438, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);
            pGoal->AddStartAction(pCreateDroneAction);

            pGoalList->AddGoal(pGoal);
        }

        pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(1.0f)));

        // tm_6_02
        // Iron Coalition Intelligence reports inbound enemy craft. 
        // They're using an old code for their comms, so we can hear 
        // them. Watch your message stream to see what they're up to.
        {
            Goal*   pGoal = CreatePlaySoundGoal(tm_6_02Sound);
            pGoal->AddStartAction(new MessageAction("Watch the chat to see what the enemy craft are after."));
            pGoalList->AddGoal(pGoal);
        }

        // create friendly miner
        {
            Goal*	pGoal = new Goal(new ElapsedTimeCondition(0.2f));

            ShipID          minerID = pMission->GenerateNewShipID();
            Vector          pos = pHomeStation->GetPosition();
            pos.x += 250.0f; //simulate undock
            pos.y += -200.0f;

            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Miner 03", minerID, 438, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);
            pGoal->AddStartAction(pCreateDroneAction);

            pGoalList->AddGoal(pGoal);
        }
        // wait half  second
        pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(0.5f)));

        // tm_6_03
        // DEFEND THE MINERS! This simulation is over when you die.
        pGoalList->AddGoal(CreatePlaySoundGoal(tm_6_03Sound));

        // create enemy carrier
        {
            Goal*	pGoal = new Goal(new ElapsedTimeCondition(1.0f));

            ShipID          carrierID = pMission->GenerateNewShipID();
            Vector          pos = Vector(3560.0f, 4880.0f, 850.0f); //Near Neptune aleph

            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Enemy Carrier", carrierID, 325, 1, c_ptCarrier);
            pCreateDroneAction->SetCreatedLocation(2081, pos); //Mars sector 2081
            pGoal->AddStartAction(pCreateDroneAction);

            pGoalList->AddGoal(pGoal);
        }

        // wait one more second
        pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(1.0f)));

        //create enemy ids to reuse
        ShipID              enemyScoutID = pMission->GenerateNewShipID();
        ShipID              enemyFighterID = pMission->GenerateNewShipID();
        //spawn enemies
        {
            Goal*           pGoal = new Goal(new ElapsedTimeCondition(8.0f));

            CreateDroneAction*  pCreateEnemyScoutAction = new CreateDroneAction("Enemy Support", enemyScoutID, 310, 1, c_ptWingman); //310 == Bios Scout
            pCreateEnemyScoutAction->SetCreatedLocation(GetStartSectorID(), Vector(3800.0f, 4275.0f, 855.0f));
            CreateDroneAction* pCreateEnemyFighterAction = new CreateDroneAction("Enemy Fighter", enemyFighterID, 315, 1, c_ptWingman); //315 == Bios Fighter
            pCreateEnemyFighterAction->SetCreatedLocation(GetStartSectorID(), Vector(3800.0f, 4175.0f, 855.0f));

            pGoal->AddStartAction(pCreateEnemyScoutAction);
            pGoal->AddStartAction(pCreateEnemyFighterAction);

            pGoalList->AddGoal(pGoal);
        }

        // create enemy miners
        {
            Goal*	pGoal = new Goal(new ElapsedTimeCondition(1.0f));

            ShipID          minerID = pMission->GenerateNewShipID();
            Vector          pos = Vector(3080.0f, -440.0f, -700.0f); //near mid he-3

            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Enemy Miner 01", minerID, 338, 1, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation(2082, pos); //sector "Enemy"
            pGoal->AddStartAction(pCreateDroneAction);

            minerID = pMission->GenerateNewShipID();
            pos = Vector(-1760.0f, -3240.0f, -700.0f); //near lower left he-3
            pCreateDroneAction = new CreateDroneAction("Enemy Miner 02", minerID, 338, 1, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation(2082, pos); //sector "Enemy"
            pGoal->AddStartAction(pCreateDroneAction);

            pGoalList->AddGoal(pGoal);
        }
        // create friendly miner
        {
            Goal*	pGoal = new Goal(new ElapsedTimeCondition(1.0f));

            ShipID          minerID = pMission->GenerateNewShipID();
            Vector          pos = pHomeStation->GetPosition();
            pos.x += -250.0f; //simulate undock
            pos.y += 200.0f;

            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Miner 04", minerID, 437, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);
            pGoal->AddStartAction(pCreateDroneAction);

            pGoalList->AddGoal(pGoal);
        }

        //respawn the enmies if destroyed (or not yet created)
        {
            Goal*               pGoal = new Goal(new FalseCondition); //will never evaluate as true - wait for player to be dead

                                                                      //can't reuse old actions, or clean-up will fail
            CreateDroneAction*  pCreateEnemyScoutAction = new CreateDroneAction("Enemy Support", enemyScoutID, 310, 1, c_ptWingman); //310 == Bios Scout
            pCreateEnemyScoutAction->SetCreatedLocation(GetStartSectorID(), Vector(3800.0f, 4275.0f, 855.0f));
            CreateDroneAction* pCreateEnemyFighterAction = new CreateDroneAction("Enemy Fighter", enemyFighterID, 315, 1, c_ptWingman); //315 == Bios Fighter
            pCreateEnemyFighterAction->SetCreatedLocation(GetStartSectorID(), Vector(3800.0f, 4175.0f, 855.0f));

            Condition*          pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2(OT_ship, static_cast<ObjectID> (enemyScoutID));
            pGoal->AddConstraintCondition(new ConditionalAction(pShipIsDestroyedcondition, pCreateEnemyScoutAction));
            pShipIsDestroyedcondition = new GetShipIsDestroyedCondition2(OT_ship, static_cast<ObjectID> (enemyFighterID));
            pGoal->AddConstraintCondition(new ConditionalAction(pShipIsDestroyedcondition, pCreateEnemyFighterAction));

            pGoalList->AddGoal(pGoal);
        }

        return new Goal(pGoalList);

    }

    //------------------------------------------------------------------------------
}