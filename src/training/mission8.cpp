/*
**
**  File:	MissionNan.cpp
**
**  Author:  Adam Davison
**
**  Description:
**      Implementation of the nanite training mission
**
**  History:
*/
#include "pch.h"
#include "Mission8.h"
#include "Training.h"
#include "GoalList.h"
#include "ConditionalAction.h"
#include "GetShipIsDestroyedCondition.h"
#include "GetShipHasMountedCondition.h"
#include "FalseCondition.h"
#include "ElapsedTimeCondition.h"
#include "SetDisplayModeAction.h"
#include "CreateDroneAction.h"
#include "MessageAction.h"
#include "DoDamageAction.h"
#include "DoDamageAdjustableAction.h"
#include "SetCommandAction.h"
#include "notcondition.h"
#include "orcondition.h"
#include "CreateWaypointAction.h"
#include "SetControlConstraintsAction.h"
#include "SetControlsAction.h"
#include "GetSectorCondition.h"
#include "ShowPaneAction.h"
#include "SetDroneNanAction.h"
#include "SetStationDestroyedAction.h"
#include "TurnToAction.h"
#include "objectpointingatcondition.h"
#include "GetShipIsStoppedCondition.h"
#include "igc.h"

namespace Training
{
	Mission8::~Mission8(void)
	{
	}

	int Mission8::GetMissionID(void)
	{
		return c_TM_8_Nanite; // For now
	}

	SectorID Mission8::GetStartSectorID(void)
	{
		return 1051;
	}

	bool Mission8::RestoreShip(void)
	{
		Terminate();
		return false;
	}

	void Mission8::CreateUniverse(void)
	{
		LoadUniverse("training_nan", 3, 1051); // Rix Scout in sector 1051

		trekClient.fGroupFire = true;

		ImissionIGC*	pCoreU = trekClient.GetCore();
		ImodelIGC*		pStationU = pCoreU->GetModel(OT_station, 1051);
        IshipIGC*       pShipU = trekClient.GetShip();
		IshipIGC*		pCommanderU = pCoreU->GetShip(m_commanderID);
        pCommanderU->SetStation (static_cast<IstationIGC*> (pStationU));
        pCommanderU->SetCommand (c_cmdAccepted, NULL, c_cidDoNothing);
        pCommanderU->SetCommand (c_cmdCurrent, NULL, c_cidDoNothing);
        pCommanderU->SetAutopilot (false);

		IsideIGC* frside = pCoreU->GetSide(0);
		pShipU->SetSide(frside);
		pShipU->SetPosition(Vector(-1500.0f, 1500.0f, 0.0f));

        // take one of the guns off the ship
        IpartIGC*       pPart = pShipU->GetMountedPart (ET_Weapon, 0);
        if (pPart)
            pPart->Terminate ();

        // Stick gat on the front
        AddPartToShip (71, 0, 0);
		// Take out the default probe
		pPart = pShipU->GetMountedPart(ET_Dispenser, 0);

		if (pPart) {
			pPart->Terminate();
		}

		// Put in a prox 1
		AddPartToShip(59, 0, 0x7fff);

		// Load a cargo a little more appropriate for a nan
		for (int i = -1; i >= -c_maxCargo; i--) {
			pPart = pShipU->GetMountedPart(NA, i);
            if (pPart)
            {
				pPart->Terminate();
            }

			if (i == -1) {
				// Put one nan in cargo
				AddPartToShip (190, i, 0x7fff);
			} else {
				// Fill the rest with spare prox
				AddPartToShip (59, i, 0x7fff);
			}
		}

		// Load custom key bindings:
		if (!GetWindow()->GetInput()->LoadMap(INPUTMAP_FILE)) 
				GetWindow()->GetInput()->LoadMap(DEFAULTINPUTMAP_FILE);
		
	}

	Condition* Mission8::CreateMission(void)
	{
        // create the goal and the goal list
        GoalList*                       pGoalList = new GoalList;
        Goal*                           pGoal = new Goal (pGoalList);

        pShip = static_cast<ImodelIGC*> (trekClient.GetShip ());
		pMission = trekClient.GetCore();
		pStation = pMission->GetModel (OT_station, 1051);

        // add the subgoals

        pGoalList->AddGoal (CreateGoal01 ());
        pGoalList->AddGoal (CreateGoal02 ());
		pGoalList->AddGoal (CreateGoal03 ());        
        //pGoalList->AddGoal (CreateGoal04 ());
        //pGoalList->AddGoal (CreateGoal05 ());
        //pGoalList->AddGoal (CreateGoal06 ());
        //pGoalList->AddGoal (CreateGoal07 ());
        //pGoalList->AddGoal (CreateGoal08 ());
        //pGoalList->AddGoal (CreateGoal09 ());

        // return the goal
        return pGoal;
	}

	    //------------------------------------------------------------------------------
    Goal*       Mission8::CreateGoal01 (void)
    {
        GoalList*   pGoalList = new GoalList;

        // play the introductory audio
        {
            Goal*                           pGoal = CreatePlaySoundGoal (salCommenceScanSound);
            SetControlConstraintsAction*    pSetControlConstraintsAction = new SetControlConstraintsAction;
            //pSetControlConstraintsAction->DisableInputAction (0xffffffff);
            pSetControlConstraintsAction->ScaleInputControl (c_axisYaw, 0.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisPitch, 0.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisRoll, 0.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisThrottle, 0.0f);
            pSetControlConstraintsAction->DisableInputAction (0xffffffff);
			pGoal->AddStartAction (pSetControlConstraintsAction);
			
            pGoalList->AddGoal (pGoal);
        }

		pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(3.0f)));

        {
            Goal*   pGoal = new Goal (new ElapsedTimeCondition (1.0f));
            pGoal->AddStartAction (new SetDisplayModeAction (TrekWindow::cmCockpit));
            pGoalList->AddGoal (pGoal);
        }

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(7.0f)); 
			pGoal->AddStartAction(new MessageAction("Welcome Cadet.  All your instructions will come in the chat window, just like in a real game!"));
			pGoal->AddStartAction(new MessageAction("Read the Chat!", 951));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition (14.0f)); 
			pGoal->AddStartAction(new MessageAction("I will be training you how to use the Nanite Repair System.  First you must equip it.  If you press F4, you'll notice I have placed one in your ships cargo.  In a real game, you'll be able to mount a 'nan' or place one in cargo in the Ship Loudout screen before you launch."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal (new GetShipHasMountedCondition(trekClient.GetShip(), 190 ) ); 
			pGoal->AddStartAction(new MessageAction("You need to swap it with the Gat Gun currently mounted.  Pressing Ctrl+[Slot number] will swap the item currently in that slot with the next weapon of the same type in cargo.  So press Ctrl+1 now to mount the Nanite Repair System in slot 1."));
			pGoalList->AddGoal(pGoal);

		}
		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition (7.0f)); 
			pGoal->AddStartAction(new MessageAction("Good.  To use your nan, simply shoot it at a friendly ship or station just like you would shoot your gat gun at an enemy ship."));
			pGoalList->AddGoal(pGoal);
		}

		
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (3.0f));
			trekClient.GetSide()->GetStations()->first()->data()->ReceiveDamage(c_dmgidNoWarn, 12500.0f, Time::Now(), Vector (-1.0f, 0.0f, 0.0f), Vector (1.0f, 0.0f, 0.0f), 0 );
			pGoalList->AddGoal(pGoal);
		}
		

		{
            ObjectPointingAtCondition*  pObjectPointingAtCondition = new ObjectPointingAtCondition (static_cast<ImodelIGC*> (trekClient.GetShip ()), static_cast<ImodelIGC*> (trekClient.GetSide()->GetStations()->first()->data()));
			Goal*                       pGoal = new Goal (pObjectPointingAtCondition);
			pObjectPointingAtCondition->SetMinimumAngle (1.0f);
            TurnToAction*               pTurnToAction = new TurnToAction (static_cast<ImodelIGC*> (trekClient.GetShip ()), static_cast<ImodelIGC*> (trekClient.GetSide()->GetStations()->first()->data()));
            ConditionalAction*          pConditionalAction = new ConditionalAction (new TrueCondition(), pTurnToAction);
            pGoal->AddConstraintCondition (pConditionalAction);
            pGoalList->AddGoal (pGoal);
		}

        // wait half second, and reset the controls
        {
            Goal*   pGoal = new Goal (new ElapsedTimeCondition (0.05f));
            SetControlsAction*          pSetControlsAction = new SetControlsAction;
            pGoal->AddStartAction (pSetControlsAction);
            pGoalList->AddGoal (pGoal);
        }

		{	
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (8.0f));
			pGoal->AddStartAction (new MessageAction("Take a look at our outpost.  It has been badly damaged from a recent attack.  I want you to repair it."));
			pGoalList->AddGoal(pGoal);
		}

		{	
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (14.0f));
			pGoal->AddStartAction (new MessageAction("Here is an important tip to quickly repair a station.  All of your energy will drain when you try to nan the station.  Your energy recharges relatively slow and nanning with your energy gone takes a very, very long time."));
			pGoalList->AddGoal(pGoal);
		}

		{	
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (12.0f));
			pGoal->AddStartAction (new MessageAction("However, entering a station by docking in the green door and then launching again will restore your energy as well as ammo, armor, and cargo.")); 
			pGoalList->AddGoal(pGoal);
		}

		{	
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (12.0f));
			pGoal->AddStartAction (new MessageAction("While you nan, sidethrust towards our outpost's entrance.  When you run out of energy, enter the green door to relaunch with full energy and keep on nanning until you have fully repaired the hull.")); 
			pGoalList->AddGoal(pGoal);
		}

		{
			Goal*	pGoal = new Goal(new NotCondition(new GetShipIsDamagedCondition(OT_station, static_cast<ImodelIGC*>(trekClient.GetSide()->GetStations()->first()->data())->GetObjectID())));
			pGoal->AddStartAction (new MessageAction("I am enabling your controls now.  Repair the station.", 961)); 
			SetControlConstraintsAction*    pSetControlConstraintsAction = new SetControlConstraintsAction;
            pSetControlConstraintsAction->ScaleInputControl (c_axisYaw, 1.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisPitch, 1.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisRoll, 1.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisThrottle, 1.0f);
            pSetControlConstraintsAction->EnableInputAction (0xffffffff);
            pGoal->AddStartAction (pSetControlConstraintsAction);
			pGoalList->AddGoal(pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(6.0f));
			pGoal->AddStartAction(new MessageAction("Nice work.", 960));
			pGoalList->AddGoal (pGoal);
		}
		
        return new Goal (pGoalList);
    }

    //------------------------------------------------------------------------------
    Goal*       Mission8::CreateGoal02 (void)
    {
        GoalList*   pGoalList = new GoalList;
		ShipID      minerID = pMission->GenerateNewShipID ();
		ShipID		atID = pMission->GenerateNewShipID();
		ShipID		atID2 = pMission->GenerateNewShipID();
		ShipID		defID = pMission->GenerateNewShipID();
		ShipID		defID2 = pMission->GenerateNewShipID();
		ShipID		defID3 = pMission->GenerateNewShipID();

		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition(0.5f));

            // create friendly miner
            Vector  pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
            pos.x += random(-150.0f, 150.0f);
            pos.y += random(-150.0f, 150.0f);
            pos.z += random(-150.0f, 150.0f);
            
            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction ("Miner 01", minerID, 436, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), pos);
			pGoal->AddStartAction(pCreateDroneAction);

			pGoalList->AddGoal (pGoal);			
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(14.0f));
			pGoal->AddStartAction(new MessageAction("One of our miners has left base to mine a Helium-3 asteroid.  Maintaining a good economy and hurting your opponents is very important in Allegiance so they will want to attack our miner when they find it."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(14.0f));
			pGoal->AddStartAction(new MessageAction("Nanning miners to keep them alive is a critical task that you will will need to do in almost every game.  Head over to our miner now and stick close to it so you'll be ready when the enemy tries to kill it."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(7.0f));
			pGoal->AddStartAction(new MessageAction("You'll need to keep it alive long enough for some of your teammates to launch from base and destroy the attackers."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(24.0f));

			Vector pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
			pos.x = -2500.0f;
			pos.y = 2500.0f;
			pos.z = 0.0f;
			
			CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("Enemy Fighter", atID, 315, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(atID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(atID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));

			pos.x = -2200.0f;
			pCreateDroneAction = new CreateDroneAction("Enemy Fighter", atID2, 315, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(atID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(atID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));
			pGoal->AddStartAction(new MessageAction("Looks like we've got company.  Nan the miner as soon as its hull starts taking damage!  Don't let it die!"));
			
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(0.5f));

			Vector  pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
            pos.x += random(-150.0f, 150.0f);
            pos.y += random(-150.0f, 150.0f);
            pos.z += random(-150.0f, 150.0f);

			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("TheBored", defID, 315, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(2.5f));

			Vector  pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
            pos.x += random(-150.0f, 150.0f);
            pos.y += random(-150.0f, 150.0f);
            pos.z += random(-150.0f, 150.0f);

			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("AEM", defID2, 315, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(2.0f));

			Vector  pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
            pos.x += random(-150.0f, 150.0f);
            pos.y += random(-150.0f, 150.0f);
            pos.z += random(-150.0f, 150.0f);

			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("lawson", defID3, 315, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID2), 0.0f)));
			pGoal->AddStartAction(new MessageAction("Defend miners.", 962));

			MessageAction*                  pMinerDeadMessage =  new MessageAction("Our miners are dead!", 963);
			Condition*                      pAttackerDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID), 0.0f));
			Condition*                      pAttackerDeadCondition2 = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID), 0.0f));
			Condition*                      pAttackerDeadCondition3 = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID), 0.0f));
			Condition*                      pAttackerDeadCondition4 = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID), 0.0f));
			Condition*                      pAttackerDeadCondition5 = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID), 0.0f));
			Condition*                      pAttackerDeadCondition6 = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID), 0.0f));
			SetCommandAction*				pKillOtherAttackerAction = new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID2), c_cidAttack);
			SetCommandAction*				pKillOtherAttackerAction2 = new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID2), c_cidAttack);
			SetCommandAction*				pKillOtherAttackerAction3 = new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID2), c_cidAttack);
			SetCommandAction*				pKillOtherAttackerAction4 = new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID2), c_cidAttack);
			SetCommandAction*				pKillOtherAttackerAction5 = new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID2), c_cidAttack);
			SetCommandAction*				pKillOtherAttackerAction6 = new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID2), c_cidAttack);
			Condition*                      pMinerDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f));
			ConditionalAction*              pConditionalAction = new ConditionalAction (pAttackerDeadCondition, pKillOtherAttackerAction, false, true);
			ConditionalAction*              pConditionalAction2 = new ConditionalAction (pAttackerDeadCondition2, pKillOtherAttackerAction2, false, true);
			ConditionalAction*              pConditionalAction3 = new ConditionalAction (pAttackerDeadCondition3, pKillOtherAttackerAction3, false, true);
			ConditionalAction*              pConditionalAction4 = new ConditionalAction (pAttackerDeadCondition4, pKillOtherAttackerAction4, false, true);
			ConditionalAction*              pConditionalAction5 = new ConditionalAction (pAttackerDeadCondition5, pKillOtherAttackerAction5, false, true);
			ConditionalAction*              pConditionalAction6 = new ConditionalAction (pAttackerDeadCondition6, pKillOtherAttackerAction6, false, true);
			ConditionalAction*              pMinerDeadConditionalAction = new ConditionalAction (pMinerDeadCondition, pMinerDeadMessage, false, true);
			pGoal->AddConstraintCondition (pConditionalAction);
			pGoal->AddConstraintCondition (pConditionalAction2);
			pGoal->AddConstraintCondition (pConditionalAction3);
			pGoal->AddConstraintCondition (pConditionalAction4);
			pGoal->AddConstraintCondition (pConditionalAction5);
			pGoal->AddConstraintCondition (pConditionalAction6);
			pGoal->AddConstraintCondition (pMinerDeadConditionalAction);

			pGoalList->AddGoal (pGoal);
		}
		
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(12.0f));
			Condition*                      pMinerAliveCondition = new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f);
			Condition*                      pMinerDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f));
			pGoal->AddConstraintCondition ( new ConditionalAction( pMinerAliveCondition, new MessageAction("Yee-haaaaaw!  Good job Cadet.", 964), false, true));
			pGoal->AddConstraintCondition ( new ConditionalAction( pMinerDeadCondition, new MessageAction("Losing that miner hurts...but we will continue on with the rest of training."), false, true));
			pGoalList->AddGoal (pGoal);
		}


        return new Goal (pGoalList);
    }

    //------------------------------------------------------------------------------
    Goal*       Mission8::CreateGoal03 (void)
    {

		GoalList*   pGoalList = new GoalList;
		Goal*       pGoal = new Goal (pGoalList);
		ShipID		bomberShipID = pMission->GenerateNewShipID();
		ShipID		nanShipID = pMission->GenerateNewShipID();
		BuoyID		alephBuoyID1 = pMission->GenerateNewBuoyID();

		{
			Vector  pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
			pos.x += random(-150.0f, 150.0f);
            pos.y += random(-150.0f, 150.0f);
            pos.z += random(-150.0f, 150.0f);
			
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("Commander", bomberShipID, 604, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);

			pGoal->AddStartAction(pCreateDroneAction);
		}

		{
			Goal* pGoal = new Goal(new ElapsedTimeCondition (3.0f));
			pGoal->AddStartAction(new CreateWaypointAction(alephBuoyID1, Vector(-2500.0f, 2500.0f, 0.0f), 1051));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal* pGoal = new Goal(new ElapsedTimeCondition (2.0f));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(10.0f));
			pGoal->AddStartAction(new MessageAction("Now that we have temporarily halted their attack, we are going to make a bomb run on their outpost in Tathlum.  Meet me in front of the aleph."));
			pGoal->AddStartAction(new SetCommandAction(trekClient.GetShipID(), c_cmdQueued, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal* pGoal = new Goal(new ElapsedTimeCondition (3.0f));
			Vector  pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
			pos.x += random(-150.0f, 150.0f);
            pos.y += random(-150.0f, 150.0f);
            pos.z += random(-150.0f, 150.0f);
			
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("bahdohday", nanShipID, 3, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);

			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetDroneNanAction(nanShipID));

			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_buoy, alephBuoyID1, c_cidGoto));

			pGoalList->AddGoal(pGoal);
		}


        MessageAction*                  pBomberDeadMessage =  new MessageAction("Our bomber has died.", 959);
		MessageAction*                  pStationDeadMessage =  new MessageAction("Enemy outpost destroyed.", 952);
		SetStationDestroyedAction*		pStationDestroyedAction = new SetStationDestroyedAction(pStation);
        Condition*                      pStationDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f));
		Condition*                      pStationDeadCondition2 = new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f));
        Condition*                      pBomberDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f));
		ConditionalAction*              pConditionalAction = new ConditionalAction (pStationDeadCondition, pStationDeadMessage, false, true);
        ConditionalAction*              pConditionalAction2 = new ConditionalAction (pBomberDeadCondition, pBomberDeadMessage, false, true);
		ConditionalAction*              pConditionalAction3 = new ConditionalAction (pStationDeadCondition2, pStationDestroyedAction, false, true);
        pGoal->AddConstraintCondition (pConditionalAction);
        pGoal->AddConstraintCondition (pConditionalAction2);
		pGoal->AddConstraintCondition (pConditionalAction3);
		
		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition(8.0f));
			pGoal->AddStartAction(new MessageAction("bahdohday is also in a scout equipped with a nan and will come with you to provide a little help."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new GetShipIsStoppedCondition(bomberShipID));
			pGoal->AddStartAction(new MessageAction("The Nanite Repair System only has a range of 400m so always stay close to the bomber."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(11.0f));
			pGoal->AddStartAction(new MessageAction("Wait for signal.", 955));
			pGoal->AddStartAction(new MessageAction("Be ready to follow me into the aleph to attack their base in Tathlum when I say go."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(5.0f));
			pGoal->AddStartAction(new MessageAction("Everybody ready?", 956));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(8.0f));
			pGoal->AddStartAction(new MessageAction("They will try to stop us from destroying their base, so make sure you stick close and be ready to nan me when my hull starts taking damage."));
			pGoalList->AddGoal (pGoal);
		}


		{
			BuoyID alephBuoyID = pMission->GenerateNewBuoyID();
			Goal*	pGoal = new Goal (new GetSectorCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 1052));
			pGoal->AddStartAction(new MessageAction("Go Go Go!", 957));
			pGoal->AddStartAction(new CreateWaypointAction(alephBuoyID, Vector(0.0f, 0.0f, 0.0f), 1052));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, OT_buoy, alephBuoyID, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, OT_buoy, alephBuoyID, c_cidGoto));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(8.0f));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, pStation, c_cidAttack)); 
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, pStation, c_cidAttack));

			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));
			pGoalList->AddGoal(pGoal);
		}
		
		{
			const StationListIGC* list = pMission->GetStations();
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (1.5f));
			Vector pos(0.0f, -300.0f, 400.0f);
			Vector pos2(0.0f, 100.0f, 300.0f);
			Vector pos3(0.0f, 0.0f, -400.0f);

			ShipID defID = pMission->GenerateNewShipID();
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("Enemy Fighter", defID, 315, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));

			ShipID defID2 = pMission->GenerateNewShipID();
			pCreateDroneAction = new CreateDroneAction("Enemy Fighter", defID2, 315, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos2);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));

			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));

  		    pGoalList->AddGoal(pGoal);
			
			pGoal = new Goal(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID)));
			pGoal->AddStartAction(new MessageAction("Here come a couple defenders, keep the bomber alive long enough to blow the base!"));
			pGoal->AddStartAction(new MessageAction("Defend bombers.", 958));
			pGoalList->AddGoal(pGoal);

			pGoal = new Goal (new ElapsedTimeCondition(1.5f));
			pGoalList->AddGoal(pGoal);

			pGoal = new Goal (new ElapsedTimeCondition(1.0f));
			ShipID defID3 = pMission->GenerateNewShipID();
			pCreateDroneAction = new CreateDroneAction("Enemy Fighter", defID3, 315, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos3);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new  MessageAction("Stay on target.", 954));
			//pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			//pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			//pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			//pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));		
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
			
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(trekClient.GetShip()->GetObjectID()), c_cidAttack), false, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(trekClient.GetShip()->GetObjectID()), c_cidAttack), false, true ));


			pGoalList->AddGoal(pGoal);


			pGoal = new Goal(new OrCondition(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)),
					new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f))));
			pGoalList->AddGoal(pGoal);
			

		//SoundID         soundID = static_cast<SoundID> (GetNumber ("voObjectiveCompleteSoundId"));
		
			pGoal = new Goal(new ElapsedTimeCondition(3.0f));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoalList->AddGoal (pGoal);
		}
		
        return pGoal;
    }
}