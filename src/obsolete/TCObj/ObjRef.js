// Create the utility object
var objUtil = new ActiveXObject("TCUtil");

// Create an object, using the JScript operator new
var objStrings = new ActiveXObject("TCObj.Strings");

// Add some strings to the collection
objStrings.Add("One");
objStrings.Add("Two");
objStrings.Add("Three");

// Create an ObjRef string for the object
var strObjRef = objUtil.ObjectReference(objStrings);

// Display the string
WScript.Echo(strObjRef);

// Get the object from it's reference string
var objStringsFromObjRef = WScript.GetObject(strObjRef);

// Display the strings in the collection
var str = "";
for (i = 0; i < objStringsFromObjRef.Count; ++i)
  str += "objStringsFromObjRef(i) = " + objStringsFromObjRef(i) + "\n";
WScript.Echo(str);

