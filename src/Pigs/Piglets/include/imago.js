//imagotrigger@gmail.com 10/14

// Globals
var MyGarrison;
var MyShip;
var EnemyGarrison;
var CurrentTarget;
var IsTargetClose = false;
var ReachedEnemy = false;
var GameController = false;
var HailedForRescue = false;
var RoundCount = 0;
var LastKill = (new Date).getTime();
var LastDeath = (new Date).getTime();
var LastDamage = (new Date).getTime();
var AboutToDie = false;

// genesis
function OnStateNonExistant(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	if (PigState_Terminated == eStatePrevious) {
		try { Logon(); }
		catch (e) {
			Trace("Logon failed: " + e.description + "\n");
			Shutdown();
		}
	}
}

// step 2...
function OnStateMissionList(eStatePrevious) {
	
	DisplayStateTransition(eStatePrevious);
	
	try {
		JoinMission(GameName);
		HasAlreadyJoinedAGame = true;
		return;
	} catch (e) {
		if (e.description != "No missions exist on the server." && e.description != "Specified game not found or has no positions available") {
			Trace("JoinMission failed: " + e.description + "\n");
			Shutdown();
		}
	}

	Trace("No JoinMission worked, so CreateMission("+ServerName+","+ServerAddr+");...\n");
	var objParams = new ActiveXObject("Pigs.MissionParams");
	
	Trace("objParams Created\n");
	Trace("GameName: " + GameName + "\n");
	
	objParams.TeamCount = 2;
	
	Trace("TeamCount set\n");
	
	objParams.minteamplayers = 1;
	
	Trace("minteamplayers set\n");
	
	objParams.maxteamplayers = 100;
	
	//Trace("maxteamplayers set to: " + objParams.maxteamplayers + "\n");
	
	objParams.gamename = GameName;
	
	Trace("gamename set\n");
	
	objParams.corename = "pcore014";
	objParams.maptype = PigMapType_Brawl;
	objParams.teamkills = KillGoal;
	objParams.killbonus = 0; // 0 = no kb    2 = c_stdkb
	objParams.defections = true;
	objParams.miners = 0;
	objParams.developments = false;
	objParams.conquest = 0;
	objParams.flags = 0;
	objParams.artifacts = 0;
	objParams.pods = false;
	objParams.experimental = true;
	try 
	{ 
		objParams.Validate();
		
		Trace("Here!\n");
		Trace("Attempting to start: " + GameName + "\n");
		Trace("PORK!\n");
		
		CreateMission(ServerName,ServerAddr,objParams); 
	}
	catch(e) {
		Trace("CreateMission("+ServerName+"...) failed: " + e.description + "\n");
		Shutdown();
	}
	GameController = true;
	AutoStartGame(objParams);
	RoundCount++;
}





// step 3...
function OnStateTeamList(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	if (PigState_JoiningTeam != eStatePrevious) {
		Trace("Attempting to JoinTeam\n");
        JoinTeam(CivSelection, "Oinkers");
	}
}

// step 4...
function OnStateWaitingForMission(eStatePrevious) {
    gameRunning = false;
    KillAllTimers();
    DisplayStateTransition(eStatePrevious);

    if (PigState_Flying == eStatePrevious) {
        Game.SendChat("My skills: " + (ShootSkill * 100) + "%");
    }

    if (IsMissionOwner()) {
        Game.SendChat("Starting in 45", 1301);
        CreateTimer(45, "StartGameTimer()", -1, "StartGameTimer");
    }
}
// step 4.1 (if mission owner)
function StartGameTimer() {
    Trace("killed timer, Attempting to StartGame\n");
    if (IsMissionOwner()) { //|| it.item().Ships.Count < MissionParams.MinTeamPlayers) {
        Game.SendChat("Launching...", 1301);
        StartGame();
    }
    Timer.Kill(); // kill at the end so that we try to StartGame again if it failed before
}

var gameRunning = false;
// step 5...
var latest_OnStateDocked_eStatePrevious = PigState_WaitingForMission;
function OnStateDocked(eStatePrevious) {
    latest_OnStateDocked_eStatePrevious = eStatePrevious;
	DisplayStateTransition(eStatePrevious);
    KillTimers();
    if (gameRunning == true) {
        CreateTimer(2.0, "onStateDockedTimeElapsed(true)", -1, "onStateDockedTimeElapsedTimer");
    } else {
        CreateTimer(16.0, "onStateDockedTimeElapsed(true)", -1, "onStateDockedTimeElapsedTimer");
    }
}

function onStateDockedTimeElapsed(safe) {
    gameRunning = true;
    eStatePrevious = latest_OnStateDocked_eStatePrevious;
    Timer.Kill();
    IsTargetClose = false;
    ReachedEnemy = false;
    AboutToDie = false;
    HailedForRescue = false;

    buyShipSetSkillsLaunch(safe);
}

function buyShipSetSkillsLaunch(safe) {
    if (safe) {

        var objHullTypes = HullTypes;

        var fRand = Random() % 4;
        Trace("Random : " + fRand + " \n");
        if (fRand >= 3) ShipSelection = "Scout";
        else if (fRand >= 2) ShipSelection = "Adv Stl Fighter";
        else if (fRand >= 1) ShipSelection = "Interceptor";
        else if (fRand >= 0) ShipSelection = "Adv Fighter";

        var realShipSelection = ShipSelection;
        if (Money && Money >= 500) {
            Trace("Money... " + Money + " \n");
            realShipSelection = "Bomber";
        }
        Trace("Requesting ship : " + realShipSelection + "\n");
        var iHull = SelectBestHull(objHullTypes, realShipSelection, "Fighter");
        Trace("Buying ship : " + objHullTypes(iHull).Name + "\n");
        Ship.BuyHull(objHullTypes(iHull));
        Trace("Launching into space...\n");
        SetSkills(ShootSkill, TurnSkill, GotoSkill);
        Launch();
    }
}

function OnMissionStarted() {
    Trace("OnMissionStarted()\n");
    gameRunning = true;
    buyShipSetSkillsLaunch(true);
}

// the end...
function OnStateLoggingOff(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	Shutdown();
}

//
// nonsense below!
//
function OnAlephHit(objAleph) {
	Trace("ooh i touched an aleph? wtf is this for\n");
}

function OnSectorChange(objSectorOld, objSectorNew) {
	Trace("ooh i changed sectors\n");
}

function OnActivate(objDeactivated) {
	Trace("Piglet Activated, previous was " + (objDeactivated ? objDeactivated.BehaviorType.Name : "(none)") + "\n");
}

function OnDeactivate(objActivated) {
	Trace("Piglet Dectivated, next is " + (objActivated ? objActivated.BehaviorType.Name : "(none)") + "\n");
}

function OnStateLoggingOn(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateCreatingMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	//NYI timeout timer!
}

function OnStateJoiningMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
	//NYI timeout timer!
}

function OnStateQuittingMission(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateJoiningTeam(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateLaunching(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}

function OnStateTeminated(eStatePrevious) {
	DisplayStateTransition(eStatePrevious);
}