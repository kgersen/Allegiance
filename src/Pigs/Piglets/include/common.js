//imagotrigger@gmail.com 10/14 originally from Matthew Lee, 1999 Microsoft


function DisplayStateTransition(eStatePrevious) {
    Trace("State changed from " + StateName(eStatePrevious) + " to " + PigStateName + "\n");
}

function NeedPickup() {
	if (HailedForRescue)
		return;
	var nearestFriend;
	var objShips = Ship.Sector.Ships;
	var iShip = FindNearestFriend(objShips);
	if (iShip != -1) nearestFriend = objShips(iShip);
	if (nearestFriend) {
		var range = Range2Ship(nearestFriend);
		var range2 = Range2Ship(MyGarrison);
		Trace("In NeedPickup()...ship "+range+" vs station "+range2+"\n");
		if (range < range2) {
			Ship.Team.SendChat("I need a pickup!",-1,1219); //voNeedPickupSound
			HailedForRescue = true;
		}
	}
}

function KillTimers() {
	if ("object" == typeof(Properties("UpdateTargetTimer")))
		Properties("UpdateTargetTimer").Kill();
	if ("object" == typeof(Properties("FindTargetTimer")))
		Properties("FindTargetTimer").Kill();
	if ("object" == typeof(Properties("RearmTimer")))
        Properties("RearmTimer").Kill();
    if ("object" == typeof (Properties("CheckFireMissileTimer")))
        Properties("CheckFireMissileTimer").Kill();
}


function KillAllTimers() {
    KillTimers();
    if ("object" == typeof (Properties("CheckFireMissileTimer")))
        Properties("CheckFireMissileTimer").Kill();
    KillTimer("onStateDockedTimeElapsedTimer");
}

function KillTimer(timerName) {
    if ("object" == typeof (Properties(timerName)))
        Properties(timerName).Kill();
}

function Delay(action) {
    KillTimer(action);
    Properties(action) = undefined;
    CreateTimer(0.1, action, 1, action);
}

function dump(arr,level) {
	var dumped_text = "";
	if(!level) level = 0;
	var level_padding = "";
	for(var j=0;j<level+1;j++) level_padding += "    ";
	if(typeof(arr) == 'object') {
		for(var item in arr) {
			var value = arr[item];
			if(typeof(value) == 'object') {
				dumped_text += level_padding + "'" + item + "' ...\n";
				dumped_text += dump(value,level+1);
			} else {
				dumped_text += level_padding + "'" + item + "' => \"" + value + "\"\n";
			}
		}
	} else {
		dumped_text = "===>"+arr+"<===("+typeof(arr)+")";
	}
	return dumped_text;
}

function plural(nCount, strNonPlural, strPlural) {
  if (nCount == 1 || nCount == -1)
    return strNonPlural;
  else
    return strPlural;
}

function SelectBestHull (hullCollection, strFirstChoice, strSecondChoice) {
  var FirstHull = -1, SecondHull = -1, DefaultHull = -1;
  var e = new Enumerator (hullCollection)
	for (var i=0; !e.atEnd(); e.moveNext(), i++) {
        var hull = e.item();
        
        if (hull.Name.search(strFirstChoice) != -1)
                FirstHull = i;

        if (hull.Name.search(strSecondChoice) != -1)
            SecondHull = i;		

        if (hull.Name.search("Scout") != -1)
            DefaultHull = i;		

    }
	if (FirstHull != -1) {
    return FirstHull;
	} else if (SecondHull != -1) {
    return SecondHull;
	} else {
    return DefaultHull;
  }
}

function Range2Ship(agcShip) {
	
	//Trace("My Position: " + Position + "\n");
	//Trace("agcShip.Position: " + agcShip.Position + "\n");
	
    return (Position.Subtract(agcShip.Position).Length);
}

function FindNearestEnemy(shipCollection) {
	var Nearest = -1, Dist = Number.MAX_VALUE;
	var e = new Enumerator (shipCollection);
	for (var i=0; !e.atEnd(); e.moveNext(), i++) {
		var ship = e.item();
		if (ship && ship.Team != Ship.Team && !ship.BaseHullType.HasCapability(4)) { //c_habmLifepod
			var range = Range2Ship(ship);
			if (range < Dist) {
				Dist = range;
				Nearest = i;
			}
		}
	}
	return Nearest;
}

function FindNearestFriend(shipCollection) {
	var Nearest = -1, Dist = Number.MAX_VALUE;
	var e = new Enumerator (shipCollection);
	for (var i=0; !e.atEnd(); e.moveNext(), i++) {
		var ship = e.item();
		if (ship && ship.Team == Ship.Team && !ship.BaseHullType.HasCapability(4)) { //c_habmLifepod
			var range = Range2Ship(ship);
			if (range < Dist) {
				Dist = range;
				Nearest = i;
			}
		}
	}
	return Nearest;
}

function FindNearestFriendlyStation(stationCollection) {
    var Nearest = -1, Dist = Number.MAX_VALUE;
    var e = new Enumerator(stationCollection);

    for (var i =0; !e.atEnd(); e.moveNext(), i++) {
        x = e.item();
        if (x.Team == Ship.Team) {
            var range = Range2Ship(x);
            if (range < Dist) {
                Dist = range;
                Nearest = i;
            }
        }
    }
    return Nearest
}
function FindNearestEnemyStation(stationCollection) {
    var Nearest = -1, Dist = Number.MAX_VALUE;
    var e = new Enumerator(stationCollection);

    for (var i = 0; !e.atEnd(); e.moveNext(), i++) {
        x = e.item();
        if (x.Team != Ship.Team) {
            var range = Range2Ship(x);
            if (range < Dist) {
                Dist = range;
                Nearest = i;
            }
        }
    }
    return Nearest
}

function IsTargetValid(shipCollection,objTarget) {
	var e = new Enumerator (shipCollection);
	for (; !e.atEnd(); e.moveNext()) {
		var ship = e.item();
		if (ship && ship.Team != Ship.Team && !ship.BaseHullType.HasCapability(4) && ship.ObjectID == objTarget.ObjectID)
			return true;
	}
	return false;
}

function DoRipCord() {
  AllStop();
  FireMissile(false);
  FireWeapon(false);

  Ship.RipCord(true);
  CreateTimer(15, "EndRipCord()", 1, "RipcordTimer");
}
function EndRipCord() {
  Ship.RipCord(false);
  Timer.Kill();
}

