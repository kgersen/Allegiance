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
#include "GetShipHasCargoCondition.h"
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
#include "PlaySoundAction.h"
#include "SetCargoNanAction.h"
#include "ResetAction.h"
#include "ResetShipAction.h"
//#include "igc.h"

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

	/*
	bool Mission8::RestoreShip(void)
	{
		Terminate();
		return false;
	}
	*/

	void Mission8::CreateUniverse(void)
	{
		LoadUniverse("training_nan", 685, 1051); // Adv Rix Scout in sector 1051; 3 (old) vs 685 difference: scan range reduced from 50000 to 3000 

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
        AddPartToShip (33, 0, 0);
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
		// BT - 9/17 - Added a training input map so that the keys match up to the instructor's requests.
		//if (!GetWindow()->GetInput()->LoadMap(TRAINING_INPUTMAP_FILE)) // BT - 10/17 - Taking this back out, default input map is now back to the original values.
			if (!GetWindow()->GetInput()->LoadMap(INPUTMAP_FILE))
				GetWindow()->GetInput()->LoadMap(DEFAULTINPUTMAP_FILE);
		
	}

	Condition* Mission8::CreateMission(void)
	{
        m_commandViewEnabled = true;
        // create the goal and the goal list
        GoalList*                       pGoalList = new GoalList;
        Goal*                           pGoal = new Goal (pGoalList);

        pShip = static_cast<ImodelIGC*> (trekClient.GetShip ());
		pMission = trekClient.GetCore();
		//pStation = pMission->GetModel (OT_station, 1051);
        homeRedDoor = pMission->GetModel(OT_station, 1052)->GetPosition();
        homeRedDoor.x += 50.0f;
        homeRedDoor.y += -200.0f;
        homeRedDoor.z += -20.0f;

		pGoal->AddConstraintCondition(new ConditionalAction( new GetShipHasCargoCondition(trekClient.GetShip(), ET_Magazine), new SetCargoNanAction(static_cast<ObjectID>(trekClient.GetShip()->GetObjectID()))));

        // add the subgoals

        pGoalList->AddGoal (CreateGoal01 ());
        pGoalList->AddGoal (CreateGoal02 ());
		pGoalList->AddGoal (CreateGoal03 ());        

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
            pSetControlConstraintsAction->ScaleInputControl (c_axisYaw, 0.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisPitch, 0.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisRoll, 0.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisThrottle, 0.0f);
            pSetControlConstraintsAction->DisableInputAction (0xffffffff);
			pGoal->AddStartAction (pSetControlConstraintsAction);
			
            pGoalList->AddGoal (pGoal);
        }

		//pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(3.0f)));

        {
            Goal*   pGoal = new Goal (new ElapsedTimeCondition (1.0f));
            pGoal->AddStartAction (new SetDisplayModeAction (TrekWindow::cmCockpit));
            pGoalList->AddGoal (pGoal);
        }

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_01Sound));
        {
            Goal*	pGoal = CreatePlaySoundGoal(tm_8_02Sound);
            pGoal->AddSkipGoalCondition(new GetShipHasMountedCondition(trekClient.GetShip(), 190));
            pGoal->AddStartAction(new MessageAction("F4 toggles the inventory. You will want it to be open."));
            pGoalList->AddGoal(pGoal);
        }
		//pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_03Sound));

        {
            Goal*	pGoal = CreatePlaySoundGoal(tm_8_03Sound);
            pGoal->AddSkipGoalCondition(new GetShipHasMountedCondition(trekClient.GetShip(), 190));
            pGoalList->AddGoal(pGoal);
        }

		{
			Goal*	pGoal = new Goal (new GetShipHasMountedCondition(trekClient.GetShip(), 190 ) ); 
			pGoal->AddStartAction (new MessageAction("Swap the Nanite Repair System into slot 1 by pressing Ctrl+1.")); 
			pGoalList->AddGoal(pGoal);
		}

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_04Sound));
		
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (3.0f));
			trekClient.GetSide()->GetStations()->first()->data()->ReceiveDamage(c_dmgidNoWarn, 12500.0f, Time::Now(), Vector (-1.0f, 0.0f, 0.0f), Vector (1.0f, 0.0f, 0.0f), 0 );
			pGoalList->AddGoal(pGoal);
		}
		
		/* Removed until turning bug is fixed
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
		*/

		{	
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (8.0f));
			pGoal->AddStartAction (new PlaySoundAction (tm_8_05Sound));
            SetControlConstraintsAction*    pSetControlConstraintsAction = new SetControlConstraintsAction;
			pSetControlConstraintsAction->ScaleInputControl (c_axisYaw, 1.0f);
            pSetControlConstraintsAction->ScaleInputControl (c_axisPitch, 1.0f);
            pSetControlConstraintsAction->ScaleInputControl(c_axisRoll, 1.0f);
            pSetControlConstraintsAction->ScaleInputControl(c_axisThrottle, 1.0f);
            pSetControlConstraintsAction->EnableInputAction(0xffffffff);
			pGoal->AddStartAction (pSetControlConstraintsAction);
			pGoalList->AddGoal(pGoal);
		}

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_06Sound));
		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_07Sound));
		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_08Sound));
		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_09Sound));

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (0.5f));;
			pGoal->AddStartAction (new MessageAction("Repair the station.", voRepairStationSound)); 
			pGoalList->AddGoal(pGoal);
		}

		{
			Goal*	pGoal = new Goal(new NotCondition(new GetShipIsDamagedCondition(OT_station, static_cast<ImodelIGC*>(trekClient.GetSide()->GetStations()->first()->data())->GetObjectID())));
			pGoalList->AddGoal(pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(6.0f));
			pGoal->AddStartAction(new MessageAction("Nice work.", voNiceWorkSound));
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
		/*  Code used when only testing Goal02
		{
            Goal*                           pGoal = CreatePlaySoundGoal (salCommenceScanSound);
            pGoalList->AddGoal (pGoal);
        }

		pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(3.0f)));

        {
            Goal*   pGoal = new Goal (new ElapsedTimeCondition (1.0f));
            pGoal->AddStartAction (new SetDisplayModeAction (TrekWindow::cmCockpit));
            pGoalList->AddGoal (pGoal);
        }
		*/

		{
			Goal*	pGoal = new Goal (new ElapsedTimeCondition(0.5f));

            // create friendly miner
            CreateDroneAction*  pCreateDroneAction = new CreateDroneAction ("Miner 01", minerID, 437, 0, c_ptMiner);
            pCreateDroneAction->SetCreatedLocation (GetStartSectorID (), homeRedDoor);
			pGoal->AddStartAction(pCreateDroneAction);

			pGoalList->AddGoal (pGoal);			
		}

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_10Sound));
		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_11Sound));
		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_12Sound));

		pGoalList->AddGoal (new Goal(new ElapsedTimeCondition(16.0f)));

        //Create enemy stealth fighters
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(1.5f));

			Vector pos = trekClient.GetSide()->GetStations()->first()->data()->GetPosition ();
			pos.x = -2500.0f;
			pos.y = 2500.0f;
			pos.z = 0.0f;
			
			CreateDroneAction*  pCreateDroneAction = new CreateDroneAction("solap", atID, 386, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(atID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(atID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));

			pos.x = -2200.0f;
			pCreateDroneAction = new CreateDroneAction("toast", atID2, 386, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(atID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(atID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(minerID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

        //Create defenders
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(1.0f));

			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("TheBored", defID, 615, 0, c_ptWingman);
            pCreateDroneAction->SetCreatedLocation(1051, homeRedDoor + Vector(20.0f, 0.0f, 10.0f));
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(0.5f));

			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("AEM", defID2, 615, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, homeRedDoor+ Vector(-20.0f, 0.0f, 10.0f));
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(1.0f));

			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("lawson", defID3, 615, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1051, homeRedDoor+Vector(0.0f, 0.0f, -10.0f));
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(atID), c_cidAttack));

			pGoalList->AddGoal (pGoal);
		}

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_13Sound));
		pGoalList->AddGoal (new Goal(new ElapsedTimeCondition(2.0f)));

		{
			Goal*	pGoal = new Goal(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(atID2), 0.0f)));
			pGoal->AddStartAction(new MessageAction("Defend miners.", voDefendMinerSound));

			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f)), new SetCommandAction(atID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(minerID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f)), new SetCommandAction(atID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(minerID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f)), new SetCommandAction(atID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(minerID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f)), new SetCommandAction(atID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(minerID), c_cidDoNothing), true, true ));
			
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
			ConditionalAction*              pConditionalAction = new ConditionalAction (pAttackerDeadCondition, pKillOtherAttackerAction, false, true);
			ConditionalAction*              pConditionalAction2 = new ConditionalAction (pAttackerDeadCondition2, pKillOtherAttackerAction2, false, true);
			ConditionalAction*              pConditionalAction3 = new ConditionalAction (pAttackerDeadCondition3, pKillOtherAttackerAction3, false, true);
			ConditionalAction*              pConditionalAction4 = new ConditionalAction (pAttackerDeadCondition4, pKillOtherAttackerAction4, false, true);
			ConditionalAction*              pConditionalAction5 = new ConditionalAction (pAttackerDeadCondition5, pKillOtherAttackerAction5, false, true);
			ConditionalAction*              pConditionalAction6 = new ConditionalAction (pAttackerDeadCondition6, pKillOtherAttackerAction6, false, true);
			pGoal->AddConstraintCondition (pConditionalAction);
			pGoal->AddConstraintCondition (pConditionalAction2);
			pGoal->AddConstraintCondition (pConditionalAction3);
			pGoal->AddConstraintCondition (pConditionalAction4);
			pGoal->AddConstraintCondition (pConditionalAction5);
			pGoal->AddConstraintCondition (pConditionalAction6);

			pGoalList->AddGoal (pGoal);
		}
		
		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(4.0f));
			Condition*                      pMinerAliveCondition = new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f);
			Condition*                      pMinerDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f));
			pGoal->AddConstraintCondition ( new ConditionalAction( pMinerAliveCondition, new MessageAction("Yee-haaaaaw!", voYeeHaSound), false, true));
			pGoal->AddConstraintCondition ( new ConditionalAction( pMinerDeadCondition, new MessageAction("Our miners are dead!", voMinersAreDeadSound), false, true));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(8.0f));
			Condition*                      pMinerAliveCondition = new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f);
			Condition*                      pMinerDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(minerID), 0.0f));
			pGoal->AddConstraintCondition ( new ConditionalAction( pMinerAliveCondition, new PlaySoundAction (tm_8_14Sound), false, true));
			pGoal->AddConstraintCondition ( new ConditionalAction( pMinerDeadCondition, new PlaySoundAction (tm_8_15Sound), false, true));
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
        ImodelIGC*	pEnemyStation = pMission->GetModel(OT_station, 1051);
        pEnemyStation->SetSideVisibility(pShip->GetSide(), true);


		/*  Code used when only testing Goal03
		{
            Goal*                           pGoal = CreatePlaySoundGoal (salCommenceScanSound);
            pGoalList->AddGoal (pGoal);
        }

		pGoalList->AddGoal(new Goal(new ElapsedTimeCondition(3.0f)));

        {
            Goal*   pGoal = new Goal (new ElapsedTimeCondition (1.0f));
            pGoal->AddStartAction (new SetDisplayModeAction (TrekWindow::cmCockpit));
            pGoalList->AddGoal (pGoal);
        }
		*/

		{
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("Commander", bomberShipID, 604, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), homeRedDoor);

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
			pGoal->AddStartAction (new SetDisplayModeAction (TrekWindow::cmCockpit));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(10.0f));
			pGoal->AddStartAction(new PlaySoundAction (tm_8_16Sound));
			pGoal->AddStartAction(new MessageAction("Meet me in front of the aleph."));
			pGoal->AddStartAction(new SetCommandAction(trekClient.GetShipID(), c_cmdQueued, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal* pGoal = new Goal(new ElapsedTimeCondition (3.0f));
			
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("bahdohday", nanShipID, 685, 0, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(GetStartSectorID(), homeRedDoor);

			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetDroneNanAction(nanShipID));

			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_buoy, alephBuoyID1, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_buoy, alephBuoyID1, c_cidGoto));

			pGoalList->AddGoal(pGoal);
		}


        MessageAction*                  pBomberDeadMessage =  new MessageAction("Our bomber has been destroyed.", voArghSound);
		MessageAction*                  pStationDeadMessage =  new MessageAction("Enemy outpost destroyed.", 901);
		SetStationDestroyedAction*		pStationDestroyedAction = new SetStationDestroyedAction(pEnemyStation);
        Condition*                      pStationDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f));
		Condition*                      pStationDeadCondition2 = new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f));
        Condition*                      pBomberDeadCondition = new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f));
		ConditionalAction*              pConditionalAction = new ConditionalAction (pStationDeadCondition, pStationDeadMessage, false, true);
        ConditionalAction*              pConditionalAction2 = new ConditionalAction (pBomberDeadCondition, pBomberDeadMessage, false, true);
		ConditionalAction*              pConditionalAction3 = new ConditionalAction (pStationDeadCondition2, pStationDestroyedAction, false, true);
        pGoal->AddConstraintCondition (pConditionalAction);
        pGoal->AddConstraintCondition (pConditionalAction2);
		pGoal->AddConstraintCondition (pConditionalAction3);
		
		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_17Sound));

		{
			Goal*	pGoal = new Goal(new GetShipIsStoppedCondition(bomberShipID));
			pGoal->AddStartAction(new PlaySoundAction (tm_8_18Sound));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(4.0f));
			pGoal->AddStartAction(new MessageAction("Wait for signal.", voWaitForSignalSound));
			pGoalList->AddGoal (pGoal);
		}

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_19Sound));

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(5.0f));
			pGoal->AddStartAction(new MessageAction("Everybody ready?", voEveryoneReadySound));
			pGoalList->AddGoal (pGoal);
		}

		pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_20Sound));

		{
			BuoyID alephBuoyID = pMission->GenerateNewBuoyID();
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(4.0f));
			pGoal->AddStartAction(new MessageAction("Go Go Go!", voGoGoGoSound));
			pGoal->AddStartAction(new CreateWaypointAction(alephBuoyID, Vector(0.0f, 0.0f, 0.0f), 1052));

			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, OT_buoy, alephBuoyID, c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, OT_buoy, alephBuoyID, c_cidGoto));

			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));

			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal (new GetSectorCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 1052));
            pGoal->AddConstraintCondition(CreateTooLongCondition(40.0f, tm_2_11rSound));
			pGoalList->AddGoal (pGoal);
		}

		{
			Goal*	pGoal = new Goal(new ElapsedTimeCondition(2.0f));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdCurrent, pEnemyStation, c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(bomberShipID, c_cmdAccepted, pEnemyStation, c_cidAttack));
			pGoalList->AddGoal(pGoal);
		}
		
		{
			const StationListIGC* list = pMission->GetStations();
			Goal*	pGoal = new Goal(new ElapsedTimeCondition (1.5f));
			Vector pos(0.0f, -300.0f, 400.0f);
			Vector pos2(0.0f, 100.0f, 300.0f);
			Vector pos3(0.0f, 0.0f, -400.0f);

			ShipID defID = pMission->GenerateNewShipID();
			CreateDroneAction* pCreateDroneAction = new CreateDroneAction("Tkela", defID, 385, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));

			ShipID defID2 = pMission->GenerateNewShipID();
			pCreateDroneAction = new CreateDroneAction("ribski", defID2, 314, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos2);
			pGoal->AddStartAction(pCreateDroneAction);
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));

			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));
			pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidGoto));

  		    pGoalList->AddGoal(pGoal);

			pGoal = new Goal (new ElapsedTimeCondition(1.5f));
			pGoalList->AddGoal(pGoal);

			pGoal = new Goal (new ElapsedTimeCondition(2.0f));
			ShipID defID3 = pMission->GenerateNewShipID();
			pCreateDroneAction = new CreateDroneAction("dontmess", defID3, 314, 1, c_ptWingman);
			pCreateDroneAction->SetCreatedLocation(1052, pos3);
			pGoal->AddStartAction(pCreateDroneAction);

			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));
			pGoal->AddStartAction(new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack));		

			pGoalList->AddGoal(pGoal);

			//static_cast<ObjectID>(trekClient.GetShip()->GetObjectID())
			//defID and defID3 attacking bomber, defID2 attacking bahdohday
			
			pGoalList->AddGoal (CreatePlaySoundGoal (tm_8_21Sound));
			pGoalList->AddGoal (new Goal(new ElapsedTimeCondition(4.0f)));

			pGoal = new Goal(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID)));
				pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
				pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
				
				pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
				pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));

				pGoal->AddStartAction(new MessageAction("Defend bombers.", voDefendBomberSound));
			pGoalList->AddGoal(pGoal);

			//defID and defID3 attacking bomber, defID2 attacking bahdohday or bomber
			pGoal = new Goal(new OrCondition(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)),
					new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f))));

			    pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(trekClient.GetShip()->GetObjectID()), c_cidAttack));
				pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(trekClient.GetShip()->GetObjectID()), c_cidAttack));	

				//pGoal->AddStartAction(new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));
				//pGoal->AddStartAction(new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(nanShipID), c_cidAttack));	

				pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
				pGoal->AddStartAction(new SetCommandAction(nanShipID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidRepair));
				
				pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
				pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
				
				//pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));
				//pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(nanShipID), 0.0f)), new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidAttack), false, true ));

				pGoal->AddStartAction(new  MessageAction("Stay on target.", voStayOnTargetSound));
			pGoalList->AddGoal(pGoal);
		
			pGoal = new Goal(new ElapsedTimeCondition(5.0f));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_station, 1051, 0.0f)), new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));

			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f)), new SetCommandAction(defID, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f)), new SetCommandAction(defID, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f)), new SetCommandAction(defID2, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f)), new SetCommandAction(defID2, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f)), new SetCommandAction(defID3, c_cmdAccepted, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			pGoal->AddConstraintCondition( new ConditionalAction(new NotCondition(new GetShipIsDamagedCondition(OT_ship, static_cast<ObjectID>(bomberShipID), 0.0f)), new SetCommandAction(defID3, c_cmdCurrent, OT_ship, static_cast<ObjectID>(bomberShipID), c_cidDoNothing), true, true ));
			
			pGoalList->AddGoal (pGoal);
		}
		
        return pGoal;
    }
}