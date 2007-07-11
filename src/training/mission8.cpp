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
		if (!GetWindow()->GetInput()->LoadMap(INPUTMAP_FILE)) {
			// Defaults, do I need this line?
			GetWindow()->GetInput()->LoadMap(DEFAULTINPUTMAP_FILE);
		}
	}

	Condition* Mission8::CreateMission(void)
	{
        // create the goal and the goal list
        GoalList*                       pGoalList = new GoalList;
        Goal*                           pGoal = new Goal (pGoalList);

        // put a constraint that will point the back in if it's more than 7500m from the base
        pShip = static_cast<ImodelIGC*> (trekClient.GetShip ());
		pMission = trekClient.GetCore();
		pStation = pMission->GetModel (OT_station, 1051);
		
		bomberShipID = pMission->GenerateNewShipID();

		Vector pos = pStation->GetPosition();
		pos.x = -2500.0f;
		pos.y = 2500.0f;
		pos.z = 0.0f;
		
		CreateDroneAction* pCreateDroneAction = new CreateDroneAction("Commander", bomberShipID, 604, 0, c_ptWingman);
		pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), pos);

		Goal* AddBomber = new Goal(new ElapsedTimeCondition (0.1f));
		AddBomber->AddStartAction(pCreateDroneAction);
		pGoalList->AddGoal(AddBomber);

		pos.x = -2200.0f;
		nanShipID = pMission->GenerateNewShipID();
		CreateDroneAction* pCreateDroneAction2 = new CreateDroneAction("bahdohday", nanShipID, 3, 0, c_ptWingman);
		pCreateDroneAction2->SetCreatedLocation(GetStartSectorID(), pos);

		Goal* AddNan = new Goal(new ElapsedTimeCondition (0.1f));
		AddBomber->AddStartAction(pCreateDroneAction2);
		pGoalList->AddGoal(AddNan);

        MessageAction*                  pBomberDeadMessage =  new MessageAction("The bomber has died.", 959);
		MessageAction*                  pStationDeadMessage =  new MessageAction("Enemy Outpost Destroyed.", 952);
		SetStationDestroyedAction*		pStationDestroyedAction = new SetStationDestroyedAction(pStation);
        Condition*                      pStationDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f));
		Condition*                      pStationDeadCondition2 = new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f));
        Condition*                      pBomberDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f));
		ConditionalAction*              pConditionalAction = new ConditionalAction (pStationDeadCondition, pStationDeadMessage, false, true);
        ConditionalAction*              pConditionalAction2 = new ConditionalAction (pBomberDeadCondition, pBomberDeadMessage, true, true);
		ConditionalAction*              pConditionalAction3 = new ConditionalAction (pStationDeadCondition2, pStationDestroyedAction, false, true);
        pGoal->AddConstraintCondition (pConditionalAction);
        pGoal->AddConstraintCondition (pConditionalAction2);
		pGoal->AddConstraintCondition (pConditionalAction3);

        // add the subgoals
        pGoalList->AddGoal (CreateGoal01 ());
        pGoalList->AddGoal (CreateGoal02 ());
        //pGoalList->AddGoal (CreateGoal03 ());
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
		/*
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(7.0f));
			pGoal->AddStartAction(new MessageAction("Welcome Cadet to a training simulation designed to teach you the basics of using the Nanite Repair System."));
			pGoalList->AddGoal (pGoal);
		}
		*/
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(7.0f));
			pGoal->AddStartAction(new MessageAction("Welcome Cadet.  All your instructions will come in the chat window, just like in a real game!"));
			pGoal->AddStartAction(new MessageAction("Read the Chat!", 951));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition (9.0f));
			pGoal->AddStartAction(new MessageAction("I'm in a damaged Ballista, a Rixian Unity bomber.  To test out the Nanite Repair System you will repair my ship."));
			//pGoal->AddStartAction(new ShowPaneAction(ofInventory));
			// Do 900 damage, bomber has med shld 1 (300hp) + hull (800hp)
			// so 900 should leave it with no shield and 1/4 hull
			pGoal->AddStartAction(new DoDamageAdjustableAction (static_cast<ObjectID>(bomberShipID), 900.0f));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal (new GetShipHasMountedCondition(trekClient.GetShip(), 190 ) ); 
			pGoal->AddStartAction(new MessageAction("Before you can use it you must equip it.  Swap it with the Gat Gun currently mounted.  Pressing F4 will display your inventory (cargo).  Pressing Ctrl+[Slot number] will swap the item currently in that slot with the next weapon of the same type in cargo.  So press Ctrl+1 now to mount the Nanite Repair System in slot 1."));
			pGoalList->AddGoal(pGoal);

		}
		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition (7.0f));
			pGoal->AddStartAction(new MessageAction("Good.  To use your nan, simply shoot it at a friendly ship or station just like you would shoot your gat gun at an enemy ship.  I'm enabling your controls now."));
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
			Goal*	pGoal = new Goal (new ElapsedTimeCondition (2.0f));
			pGoal->AddStartAction(new SetCommandAction(trekClient.GetShip(), c_cmdQueued, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
			pGoal->AddStartAction(new SetDroneNanAction(nanShipID));
			pGoal->AddStartAction(new MessageAction("Need Repairs.", 953));
			//pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
			//pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
			pGoalList->AddGoal(pGoal);
		}

		{
			Goal*	pGoal = new Goal(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID))));
			pGoal->AddStartAction(new MessageAction("The Nanite Repair System only has a range of 400m so stay close to the bomber.  Fully repair my ship's hull to continue with training."));
			pGoalList->AddGoal(pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(5.0f));
			pGoal->AddStartAction(new MessageAction("Nice work.", 960));
			pGoalList->AddGoal (pGoal);
		}

        return new Goal (pGoalList);
    }

    //------------------------------------------------------------------------------
    Goal*       Mission8::CreateGoal02 (void)
    {
        GoalList*   pGoalList = new GoalList;
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(11.0f));
			pGoal->AddStartAction(new MessageAction("Wait for signal.", 955));
			pGoal->AddStartAction(new MessageAction("Be ready to follow me into the aleph to attack the base on the other side when I say to go.  bahdohday is also in a Scout equipped with a nan and will come with you to provide a little help."));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(2.0f));
			pGoal->AddStartAction(new MessageAction("Everybody ready?", 956));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(8.0f));
			pGoal->AddStartAction(new MessageAction("There may be defenders, so make sure you stick close and be ready to nan me when my hull starts taking damage."));
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
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("TheBored", defID, 315, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));

			ShipID defID2 = pMission->GenerateNewShipID();
			pCreateDroneAction = new CreateDroneAction("AEM", defID2, 315, 1, c_ptWingman);
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
			pCreateDroneAction = new CreateDroneAction("lawson", defID3, 315, 1, c_ptWingman);
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

        return new Goal (pGoalList);
    }
}