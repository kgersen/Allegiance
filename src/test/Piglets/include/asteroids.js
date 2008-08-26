/*
 *
 * Asteroid tracking code for scout2
 * by Matthew Lee
 *
 */

// asteroid variables
var UAsteroids;
var SiAsteroids;
var HeAsteroids;
var CAsteroids;

// InitAsteroidLists sets up the four asteroid lists.
function InitAsteroidLists()
{
  UAsteroids = new ActiveXObject("Scripting.Dictionary");
  SiAsteroids = new ActiveXObject("Scripting.Dictionary");
  HeAsteroids = new ActiveXObject("Scripting.Dictionary");
  CAsteroids = new ActiveXObject("Scripting.Dictionary");
}

// CheckOreAsteroid checks if an asteroid is a special asteroid, and adds it to one of its lists
// if necessary, as well as reporting status messages.
function CheckOreAsteroid(objAsteroid)
{
  if (objAsteroid.Name.charAt(0) == 'a')
    return;

  if (UAsteroids.Exists(objAsteroid) || SiAsteroids.Exists(objAsteroid) || CAsteroids.Exists(objAsteroid) || HeAsteroids.Exists(objAsteroid))
    return;

  if (objAsteroid.Name.charAt(0) == 'U')
  {
    StatusMessage("Uranium asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    UAsteroids.Add(objAsteroid, Ship.Name);
  } 
  else if (objAsteroid.Name.charAt(0) == 'S')
  {
    StatusMessage("Silicon asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    SiAsteroids.Add(objAsteroid, Ship.Name);
  } 
  else if (objAsteroid.Name.charAt(0) == 'H')
  {
    StatusMessage("Helium3 asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    HeAsteroids.Add(objAsteroid, Ship.Name);
  } 
  else if (objAsteroid.Name.charAt(0) == 'C')
  {
    StatusMessage("Carbon asteroid " + objAsteroid.Name + " found in " + objAsteroid.Sector.Name);
    CAsteroids.Add(objAsteroid, Ship.Name);
  }
}

// SectorAsteroidCount returns a string that is a report of the asteroids of a specific type.
// The parameter strTitle must be "Uranium", "Silicon", "Carbon", or "Helium3".
function SectorAsteroidCount(strTitle)
{
  var asteroidChar = 'a';
  var strResult = "";
  var asteroidCount = 0;
  var totalCount = 0;
  var strComma = "";

  switch (strTitle)
  {
  case "Uranium":
    asteroidChar = 'U';
    break;
  case "Silicon":
    asteroidChar = 'S';
    break;
  case "Carbon":
    asteroidChar = 'C';
    break;
  case "Helium3":
    asteroidChar = 'H';
    break;
  default:
    return "Invalid parameter in SectorAsteroidCount.";
  }

  e = new Enumerator (Game.Sectors);
  while (!e.atEnd())
  {
    asteroidCount = 0;
    x = e.item();
    if (x.Asteroids.Count == 0)
    {
      e.moveNext();
      continue;
    }

    f = new Enumerator (x.Asteroids);
    for (; !f.atEnd(); f.moveNext())
    {
      y = f.item();
      if (y.Name.charAt(0) == asteroidChar)
      {
        asteroidCount++;
        totalCount++;
      }
    }
    if (asteroidCount > 0)
    {
      strResult += strComma + x.Name;
      strComma = ", ";
    }
    if (asteroidCount > 1)
      strResult += " (x" + asteroidCount + ")";
    e.moveNext();
  }

  var s = "";
  if (totalCount > 0)
    s = strTitle + " asteroids found in " + strResult;
  else
    s = "No " + strTitle + " asteroids found.";
  return s;
}

// UpdateAsteroidLists updates all 4 asteroid lists with data from the game state.
function UpdateAsteroidLists()
{
  SectorAsteroidListUpdate('U');
  SectorAsteroidListUpdate('S');
  SectorAsteroidListUpdate('C');
  SectorAsteroidListUpdate('H');
}

// SectorAsteroidListUpdate updates a specific asteroid list from the data in the game.
// the input parameter asteroidChar must be 'U', 'S', 'C', or 'H'.
function SectorAsteroidListUpdate(asteroidChar)
{
  var refList;

  switch (asteroidChar)
  {
  case 'U':
    refList = UAsteroids;
    break;
  case 'S':
    refList = SiAsteroids;
    break;
  case 'C':
    refList = CAsteroids;
    break;
  case 'H':
    refList = HeAsteroids;
    break;
  }

  refList.RemoveAll();

  e = new Enumerator (Game.Sectors);
  while (!e.atEnd())
  {
    x = e.item();
    if (x.Asteroids.Count == 0)
    {
      e.moveNext();
      continue;
    }

    f = new Enumerator (x.Asteroids);
    for (; !f.atEnd(); f.moveNext())
    {
      y = f.item();
      if (y.Name.charAt(0) == asteroidChar)
      {
        refList.Add(y, Ship.Name);
      }
    }
    e.moveNext();
  }
}

// ScanAsteroidsForOres iterates thru the asteroids in a specified sector and
// calls CheckOreAsteroid on each one.
function ScanAsteroidsForOres(agcSector)
{
  // get collection of asteroids in the current sector
  var asteroidList = agcSector.Asteroids;
  var dist;
  var maxasteroid = new Array();

  // iterate through asteroid collection
  e = new Enumerator (asteroidList);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    CheckOreAsteroid(x);
  }

}

