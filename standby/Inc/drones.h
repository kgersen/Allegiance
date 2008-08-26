#ifndef DRONES_H
#define DRONES_H

// There are only a handfull of things that the rest of the game
// needs to know about.  Everything else should be completely 
// contained in the drones directory.


const int MAX_DRONES = 100;        // this is the maximum number of drones for the entire game
                                // the number was arbitrary, but the idea is that we don't want too
                                // many drones taking away from player cycles


class Drone;    // class declaration, but not definition.  See drone.h

//KGJV - missing decl
typedef WORD DroneType;
typedef WORD CommandMask;

/*-------------------------------------------------------------------------
 * Class: Drones
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is the drones wrapper class.  This class should make knowledge of
 *    of the actual Drone class completely unnecessary outside of the drones
 *    directory.
 */
class Drones {
    public:
        
        Drones();                                               // Constructor
        
        ~Drones()                                               // Destructor
        { 
            Purge(); 
        };

        //Leave this around so the drones can set the default command
        //(NYI this really should be done via the database (server) or messages (client))
        void SetCore(ImissionIGC* pmission);

        /*
        ImissionIGC * GetCore()                                 // Want to access the trekcore, too
        { 
            return m_pTrekCore; 
        };
        */

        Drone* Create( ImissionIGC*     pMission,               // Create a new drone (and it's accompanying ship)
                       DroneType        dtType,                 // what type of drone
                       const char*      name,                   // name, or NULL = default name
                       HullID           hullID,                 // Which hull to use
                       IsideIGC*        side,                   // what side is it on
                       Mount            nParts,                 // Part data
                       const PartData*  parts,
                       float            shootSkill = 1.0f,      // Unless specified, give the drones all their skills
                       float            moveSkill  = 1.0f,        
                       float            bravery    = 1.0f);


        // Message to Drones: these functions are wrappers of each other.
        // It should be passed as much info as possible
       
       /*
        void MessageToDrone(Time now,                           // Just support old calling method, really we want to have the sender field
                            const char* msg, 
                            ImodelIGC*  pmodelObject) 
        { 
            MessageToDrone(now, NULL, msg, pmodelObject); 
        };
        */

        void MessageToDrone(Time now,                           // When we know the sender, but not the recipient or command
                            IshipIGC* pSender,
                            const char* msg, 
                            ImodelIGC*  pmodelObject,
                            bool fCheckPilot = false);

        void MessageToDrone(Time now,                           // We know the sender and recipient, bot not which command
                            IshipIGC* pSender,
                            Drone * pdRecip, 
                            const char* msg, 
                            ImodelIGC*  pmodelObject);

        void MessageToDrone(Time now,                           // We know the sender, recipient, and command.... Just pass it on to the drone
                            IshipIGC* pSender,
                            Drone * pdRecip, 
                            CommandID cm,
                            ImodelIGC* pmodelObject);    

        void DockEvent(IshipIGC * pship);                       // Let the drone know that it has docked

        CommandMask GetDroneCommands(DroneType);                // GetDroneCommands returns mask of CommandIDs that are supported by each
                                                                // drone.  If no drone matches pDroneShip, I return -1.

        void Update(Time  lastUpdate,                           // This is in the main game loop.  Update all of the drone controls.
                    Time  now);
        

        // we also want to have a special drone for the auto pilot.  This will only get used on the client.

        void CreatePilot(IshipIGC* ship);                       // Create an autoPilot drone using the existing ship

        void MessageToPilot(CommandID cm, ImodelIGC* pmodelObject);    // Send the message right to the pilot
        
        void UpdatePilot(Time lastUpdate, Time now, float dt);  // This is not in the main drone update, because it will normally be the 
                                                                // only one running on the client, and it won't happen on the server
        
        void ResetPilot(void);                                  // Clear the pilots goals
        
        IshipIGC* GetPilotShip(void);                           // Get the pilot's ships
        
        void SetPilotShip(IshipIGC* ship);                      // Set the pilot to use a different ship

        // end pilot stuff


        // begin Pig pilot stuff

        Drone* CreatePigPilot(IshipIGC* ship);                  // Create a pig autoPilot drone using the existing ship

        // end Pig pilot stuff


        bool DeleteShip(IshipIGC* ship);                        // Clean up the drone using this ship

        void Purge(ImissionIGC* pMission = NULL);               // Clean up all of the drones (for the specified mission)


        // Here are a couple of specialized functions, necessary to keep drone.h hidden.
        bool DisappearingDrone(IshipIGC * pship);               // When a special drone (bounty hunter, pirate) wants to go into in aleph,
                                                                // and never come out the other side
        int  GetIndex() {return m_index;} // so drones can keep track of their own index and help out on deletion

        void InitializeConstructor(Drone*           pdrone,
                                   IstationTypeIGC* pstationtype);

    private:
        Drone*          m_pDroneList[MAX_DRONES];                   // This is the actual list of drone pointers
        //ImissionIGC*    m_pTrekCore;                                // Pointer to the core
        Drone*          m_pPilot;                                   // keep the autoPilot seperately
        int             m_index;                                    // next slot to try

    friend class Pirate;                                        // Pirate needs to find other pirates, and miners without searching all ships
};


/*-------------------------------------------------------------------------
 * Global: g_drones
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is the ONLY instance of the Drones class, but it needs to be
 *      accessable by EVERYONE
 */
extern Drones g_drones;


/*-------------------------------------------------------------------------
 * Include: drone.h
 *-------------------------------------------------------------------------
 * Purpose:
 *      If all goes well, and the server really becomes independent of the Drone class,
 *    then we won't need this include.  Until it does, though, we need to include the 
 *    Information about the Drone class.
 */
#include "..\drones\drone.h"

#endif
#endif