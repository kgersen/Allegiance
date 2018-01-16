// These values bridge COM Enums defined in PigsIDL into JS compatible enums that can be used for intellisense.


// Enums is the top level namespace. All Enums will go under this namespace. This will avoid collisions
// with global COM variables like "PigState" which has the same name as the enum. 
var Enums = function () { };


////////////////////////////////////////////////////////////////////////////////////////////////////
// PigState Enum

Enums.PigState = function () { };

Enums.PigState.PigState_NonExistant = PigState_NonExistant;
Enums.PigState.PigState_LoggingOn = PigState_LoggingOn;
Enums.PigState.PigState_LoggingOff = PigState_LoggingOff;
Enums.PigState.PigState_MissionList = PigState_MissionList;
Enums.PigState.PigState_CreatingMission = PigState_CreatingMission;
Enums.PigState.PigState_JoiningMission = PigState_JoiningMission;
Enums.PigState.PigState_QuittingMission = PigState_QuittingMission;
Enums.PigState.PigState_TeamList = PigState_TeamList;
Enums.PigState.PigState_JoiningTeam = PigState_JoiningTeam;
Enums.PigState.PigState_WaitingForMission = PigState_WaitingForMission;
Enums.PigState.PigState_Docked = PigState_Docked;
Enums.PigState.PigState_Launching = PigState_Launching;
Enums.PigState.PigState_Flying = PigState_Flying;
Enums.PigState.PigState_Terminated = PigState_Terminated;





