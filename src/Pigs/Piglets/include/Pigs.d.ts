///**
//* ITCStrings - Collection of TCStrings
//* @typedef {string[]} ITCStrings 
//*/

///**
// * IPigBehaviorType
// * - InvokeCommands (Property) Returns some stuff.
// * - Test (Property) Returns some other stuff.
// * @typedef {object} IPigBehaviorType 
// * @property {ITCStrings} InvokeCommands
// * @property {number} Test 
// */

///**
//* Gets the object that describes this class of behavior.
//* @type {IPigBehaviorType}
//*/
//var BehaviorType;

enum PigState {
    PigState_NonExistant,
    PigState_LoggingOn,
    PigState_LoggingOff,
    PigState_MissionList,
    PigState_CreatingMission,
    PigState_JoiningMission,
    PigState_QuittingMission,
    PigState_TeamList,
    PigState_JoiningTeam,
    PigState_WaitingForMission,
    PigState_Docked,
    PigState_Launching,
    PigState_Flying,
    PigState_Terminated,
}



/**
 * Can be one of:
 * - PigState_NonExistant
 * - PigState_LoggingOn
 * - PigState_LoggingOff
 * - PigState_MissionList
 * - PigState_CreatingMission
 * - PigState_JoiningMission
 * - PigState_QuittingMission
 * - PigState_TeamList
 * - PigState_JoiningTeam
 * - PigState_WaitingForMission
 * - PigState_Docked
 * - PigState_Launching
 * - PigState_Flying
 * - PigState_Terminated
 */
declare var PigState: PigState;

   
