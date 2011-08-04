// Dijkstra's algorithm for finding a single-source shortest path estimate to all other
// sectors.

// Implemented by Matthew Lee

// Source: Introduction to Algorithms, by Cormen, Leiserson, and Rivest
// Bretton has a copy in his office.

// Important Functions In This File
// --------------------------------
// InitPathGenerator()
// NextAlephInShortestPath(fromSector, toSector)

// the "predecessor" is the previous step in a path.  the final path is obtained by walking back
// the predecessor links from destination to source.
var Predecessor;

// estimate is the amount of steps from the source the node is.
var Estimate;

// Q is the list of sectors yet to be processed.  size of Q strictly decreases.
var Q;

// Init function, sets up the data structures
function InitPathGenerator()
{
  //S = new ActiveXObject("Scripting.Dictionary");
  Q = new ActiveXObject("Scripting.Dictionary");
  Predecessor = new ActiveXObject("Scripting.Dictionary");
  Estimate = new ActiveXObject("Scripting.Dictionary");
}

// returns the key in Q that has the minimum value associated with it.
// also known as "poor man's priority queue."
function MinimumItem()
{
  var minitem, min = 100000000;
  e = (new VBArray(Q.Keys())).toArray();
  for (key in e)
  {
    x = e[key];
    if (Estimate.Item(x) < min)
    {
      minitem = x;
      min = Estimate.Item(x);
    }
  }
  return minitem;
}

// debug function to dump out the contents of a Scripting.Dictionary object
function DumpDictionary(objDict, strTitle)
{
  Trace("****************** " + strTitle + " ************************\n");
  keyarray = (new VBArray(objDict.Keys())).toArray();
  for (key in keyarray)
  {
    i = objDict.Item(keyarray[key]);
    Trace("" + keyarray[key] + " : " + i + "\n");
  }
}

// returns the weight of a sector, based on the number of enemies in the sector.
// this was originally intended to make enemy-laden sectors less desirable for
// inclusion in a pig path, but it didn't work out.
function SectorWeight(agcSector)
{
  Trace("station count in " + agcSector.Name + " = " + agcSector.Stations.Count);
  if (agcSector.Stations.Count == 0)
  {
    Trace("\n");
    return 1;
  }
  var w = 1;
  e = new Enumerator(agcSector.Stations);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    if (x.Team != Ship.Team)
    {
      w += 100;
      Trace(" false");
    }
    else
    {
      Trace(" true");
    }
  }
  Trace(" w = " + w + "\n");
  return w;
}

// returns the number of enemy stations in a sector
function EnemiesPresent(agcSector)
{
  if (agcSector.Stations.Count == 0)
  {
    return false;
  }
  var w = false;
  e = new Enumerator(agcSector.Stations);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    if (x.Team != Ship.Team)
      w = true;
  }
  return w;
}

// this "relaxes" the node V by setting its predecessor to node U and changing its
// estimate to U's estimate plus one.
function Relax(uSector, vSector)
{
  weight = 1;
  if (Estimate.Item(vSector) > (Estimate.Item(uSector) + weight))
  {
    Estimate.Item(vSector) = Estimate.Item(uSector) + weight;
    Predecessor.Item(vSector) = uSector;
  }
}

// this is the function used in scout2.  it calculates the path to a destination sector (toSector)
// from the current sector (fromSector), and then returns the name of the aleph the scout needs to
// fly to to get to the next step along the path.
// example:
//
// sector map:  A -- B -- C -- D -- E
//
// scout is in sector B, wants to fly to E.
//
// NextAlephInShortestPath(B, E) == C
// NextAlephInShortestPath(C, E) == D
// NextAlephInShortestPath(D, E) == E
//
function NextAlephInShortestPath(fromSector, toSector)
{
  FindShortestPath(fromSector, toSector);
  return (NextAleph(fromSector, toSector));
}

// FindShortestPath is the main algorithm in this file.  It calculates the best
// path from fromSector to toSector and puts the results in the Predecessor and
// Estimate data structures.  Nothing is returned; the results of the path
// generation must be extracted from those data structures.
function FindShortestPath(fromSector, toSector)
{
  Predecessor.RemoveAll();
  Estimate.RemoveAll();
  Q.RemoveAll();

  e = new Enumerator(Game.Sectors);
  for (; !e.atEnd(); e.moveNext())
  {
    x = e.item();
    Predecessor.Add(x, "-");
    Estimate.Add(x, 1000000);
    Q.Add(x, "*");
  }

  Estimate.Item(fromSector) = 0;

  while (Q.Count > 0)
  {
    u = MinimumItem();
    Q.Remove(u);
    alephlist = u.Alephs;
    if (alephlist.Count < 1)
      continue;
    e = new Enumerator(alephlist);
    for (; !e.atEnd(); e.moveNext())
    {
      x = e.item();
      v = x.Destination.Sector;
      Relax(u, v);
    }
  }
}

// debug function to walk through a finished path
function ReportPath(fromSector, toSector)
{
  var s = "";
  var p = toSector;
  Trace("***************** path generator *********************\n");
  while (p != fromSector)
  {
    Trace("step: " + p.Name + "\n");
    p = Predecessor.Item(p);
  }
  DumpDictionary(Estimate, "Estimates");
}

// finds the next aleph in an already calculated path from fromSector to toSector.
function NextAleph(fromSector, toSector)
{
  var result;
  var p = toSector;
  while (p != fromSector)
  {
    result = p;
    p = Predecessor.Item(p);
  }
  return result;
}

