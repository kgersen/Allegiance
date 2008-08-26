// TODO: Create the Session Information object
//var info = new ActiveXObject("TCObj.SessionInfo");
//info.ApplicationName = WScript.ScriptFullName;

// Connect to the Pig Server
var app = new ActiveXObject("Pigs.Session");
// TODO: app.SessionInfo = objInfo;

// Get the collection of pigs
var pigs = app.Pigs;

// Get an iterator on the collection of pigs
var itPigs = new Enumerator(pigs);

// Get the first pig in the iterator
var pig = itPigs.item();

// Get the first pig's ship
var ship = pig ? pig.Ship : null;

// Jump straight into the debugger
debugger;
