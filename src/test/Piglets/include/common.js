/*
 *
 * common.js
 *
 * Common routines useful in constructing a pig behavior script
 * Written by Matthew Lee
 *
 */

/////////////////////////////////////////////////////////////////////////////
// Returns the proper string based on the plurality of the numeric argument
// given.  the non-plural string is the second argument, the plural string is
// the third argument.
// example: nStationCount + " station" + plural(nStationCount, "", "s") + " found"
function plural(nCount, strNonPlural, strPlural)
{
  if (nCount == 1 || nCount == -1)
    return strNonPlural;
  else
    return strPlural;
}

/////////////////////////////////////////////////////////////////////////////
// Selects the best hull from a collection of hulls.  Scans the list, noting the
// index of the Fighter and the Interceptor.  If a Fighter was found, it returns
// that index.  Otherwise, the index of the Interceptor is returned.  If an
// Interceptor is not found, returns 0, which should be a scout or a recovery
// ship.
function SelectBestHull (hullCollection, strFirstChoice, strSecondChoice)
{
  var FirstHull = -1, SecondHull = -1, DefaultHull = -1;
  var e = new Enumerator (hullCollection)
  Host.Trace("Hull types:\n");

  // loop thru collection
  for (var i=0; !e.atEnd(); e.moveNext(), i++)
  {
    var hull = e.item();
    Host.Trace("" + i + ". " + hull.Name + "\n");

    // search for strFirstChoice in the name
    if (hull.Name.search(strFirstChoice) != -1)
      FirstHull = i;

    // search for strSecondChoice in the name
    if (hull.Name.search(strSecondChoice) != -1)
      SecondHull = i;		

    // search for strSecondChoice in the name
    if (hull.Name.search("Scout") != -1)
      DefaultHull = i;		
  }
  Host.Trace("First: " + FirstHull + " Second: " + SecondHull + "\n");

  // look for valid first choice index
  if (FirstHull != -1)
  {
    // first choice found, return index
    Host.Trace ("Selecting first choice.\n");
    return FirstHull;
  }
  else if (SecondHull != -1)
  {
    // second choice found, return index
    Host.Trace ("Selecting second choice.\n");
    return SecondHull;
  }
  else
  {
    // default found, return index
    Host.Trace ("Selecting default choice.\n");
    return DefaultHull;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Calculates the distance squared to an AGCShip or PigShip.
function Range2Ship(agcShip)
{
  return (Position.Subtract(agcShip.Position).LengthSquared);
}

/////////////////////////////////////////////////////////////////////////////
// This function starts a ripcord operation.
function DoRipCord()
{
  // stop the ship, stop all weapons
  AllStop();
  FireMissile(false);
  FireWeapon(false);

  // start ripcording
  Ship.RipCord(true);

  // go back to normal in 15 seconds - set a timer
  CreateTimer(15, "EndRipCord()", 1, "RipcordTimer");
}

/////////////////////////////////////////////////////////////////////////////
// This function ends a ripcord operation - it should not be called directly.
function EndRipCord()
{
  // stop ripcording
  Ship.RipCord(false);

  // kill ripcord timer
  Timer.Kill();
}


