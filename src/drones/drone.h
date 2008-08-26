#ifndef DRONE_DEF_H
#define DRONE_DEF_H


class Goal;     // Defined in goal.h


/*-------------------------------------------------------------------------
 * Class: Drone
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is the main drone class.  It defines 99% of the behavior of the specific
 *    drone classes, with virtual functions so that each child class can have it's
 *    own little quirks
 */

class Drone 
{
public:
    Drone(IshipIGC* id, DroneType dt);                                                      // Constructor

    virtual ~Drone()                                                                        // Need this so for child classes
    { 
        ClearGoals();                       
    };

    virtual void Update(Time now, float dt);                                                // Update the controls for the drone

    virtual bool Message(Time now, CommandID cm, ImodelIGC*  pmodelObject);                 // Command the drone

    virtual void Status(void);                                                              // Chat goal and other important info

    DroneType GetDroneType(void)                                                            // Return the actual type of drone
    { 
        return dType; 
    };

    IshipIGC* GetShip()                                                                     // Each drone works on a ship
    { 
        return m_pShip; 
    };

    void SetShip(IshipIGC* ship);                                                           // Change the ship that the drone controls

    IstationIGC* GetStation()                                                               // Return any station affliations (Null until someone commands otherwise)
    { 
        return m_pStation; 
    };

    void SetStation(ImodelIGC* pStation)                                                    // The drone will look to this station first in time of need
    {
        assert(pStation && pStation->GetObjectType() == OT_station);
        m_pStation = (IstationIGC*) pStation;
    }

    // Goal operations
    virtual void SetGoal(CommandType cm, ImodelIGC* pTarget = NULL, bool reply = false);    // Set the current goal of the drone

    virtual void SetGoal(Goal* goal);       

    void BackupGoal(void);                                                                  // Allow intermediate goals with a stack architecture

    void RestoreGoal(void);

    Goal* GetGoal(void)                                                                     // Return the current goal
    { 
        return m_goal; 
    };

    virtual void DockEvent(void)                                                            // Some drones (miners for one) need to know when they dock
    {};

    void ToggleVerbose()                                                                    // Flip the state of verboseness
    { 
        bVerbose = !bVerbose; 
    };

    void Verbose(const char* format, ...);                                                  // Send a chat, but only if verbose is turned on
    
    float GetMoveSkill(void)                                                                // Return skill values
    { 
        return m_flMoveSkill; 
    };

    float GetShootSkill(void) 
    { 
        return m_flShootSkill; 
    };

    float GetBravery(void)    
    { 
        return m_flBravery; 
    };
    
    bool SetShootSkill(float shoot)                                                         // Set skill values
    {
        if (shoot <= 1.0f && shoot >= 0.0f) {
            m_flShootSkill = shoot;
            Verbose("Shooting skill now at %d %%", (int)(shoot*100));
            return true;
        }
        return false;
    };

    bool SetMoveSkill(float move)
    {
        if (move <= 1.0f && move >= 0.0f) {
            m_flMoveSkill = move;
            Verbose("Moving skill now at %d %%", (int)(move*100));
            return true;
        }
        return false;
    };

    bool SetBravery(float bravery) 
    { 
        if (bravery <= 1.0f && bravery >= 0.0f) {
            m_flBravery = bravery;
            Verbose("Bravery now at %d %%", (int)(bravery*100));
            return true;
        }
        return false;
    };

    IstationIGC* FindBestStation(bool fCheckEnemyStations);                                 // Analyze space for the closest/most friendly station

    void Die(void)                                                                          // Set self-destruct flag
    {
        m_fDie = true;
    };

    bool GetDisappear(void)                                                                 // Is this drone trying to disappear?
    {
        return m_fDisappear;
    };

    void Disappear(void)                                                                    // This drone is trying to disappear
    {
        m_fDisappear = true;
    };

    void AddGoal(Goal* goal)                                                                // Put a new goal on the end of the stack
    {
        m_goalMemory.PushEnd(goal);
    };

    void ClearGoals(void);                                                                  // Delete the current goal and the stack

    int GetIndex() {return m_index;}

    ImodelIGC * GetRepairStation()
    {
      return (IstationIGC*) FindTarget(m_pShip, c_ttFriendly | c_ttStation | c_ttNearest | c_ttAnyCluster,
                                       NULL, NULL, NULL, NULL, c_sabmRepair);
    }

    void SendChat(SoundID soundID, const char* format, ...);                                                    // Wrapper for DroneSendChat
protected:


    virtual void DetermineGoal(void);                                                       // No goal?  Then figure one out
    
    virtual Goal* GetNewDefaultGoal(void);                                                  // Get a default goal
    
    bool Instincts(void);                                                                   // Always check these
    
    TRef<IshipIGC> m_pShip;         // Pointer to the ship I am controlling     
    TRef<IstationIGC> m_pStation;   // Favorite station
    Goal* m_goal;                   // Current goal

private:
    TVector<Goal*> m_goalMemory;    // Goal stack
    bool bVerbose;                  // Verbose flag         // todo:  change this to a verbose target (or NULL for no verbose)
    const DroneType dType;          // Keep track of what specific type of drone this is
    float m_flMoveSkill;            // Skill levels
    float m_flShootSkill;
    float m_flBravery;          
    bool  m_fDie;                   // Am I supposed to self-destruct next round?
    bool  m_fDisappear;             // Am I trying to disappear?
    int   m_index; // index in Drones class

    friend class MimicGoal;         // Needs to play with goal memory
    friend class ConstructGoal;
};


/*-------------------------------------------------------------------------
 * Function: OBJECT_IS_BAD
 *-------------------------------------------------------------------------
 * Purpose:
 *    I use this all over the place.  This checks that an object is alive and flying
 */

inline bool OBJECT_IS_BAD(ImodelIGC* x) { return (!x || x->GetCluster() == NULL); };


// Specific Drone Types:


/*-------------------------------------------------------------------------
 * Class: WingManDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *    These are the fighters
 */

class WingManDrone : public Drone
{
public:
    WingManDrone(IshipIGC* id) 
        : Drone(id, c_dtWingman)
    {};

protected:
    virtual Goal* GetNewDefaultGoal(void);
};

/*-------------------------------------------------------------------------
 * Class: ConstructionDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *    These build buildings and then die
 */

class ConstructionDrone : public Drone
{
public:
    ConstructionDrone(IshipIGC* id) 
    :
        Drone(id, c_dtConstruction)
    {
    }

    void Initialize(IstationTypeIGC* pstationtype);

    IstationTypeIGC*    GetBuildStationType(void)
    {
        return m_pstationtype;
    }

private:
    TRef<IstationTypeIGC>   m_pstationtype;
};


/*-------------------------------------------------------------------------
 * Class: AutoPilotDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *    Quite a bit like the fighters, but different default goals, etc.
 */

class AutoPilotDrone : public Drone
{
public:
    AutoPilotDrone(IshipIGC* id) 
        : Drone(id, c_dtAutoPilot)
    {};

protected:
    virtual Goal* GetNewDefaultGoal(void);
};


/*-------------------------------------------------------------------------
 * Class: PigPilotDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *    The main difference between this and the AutoPilotDrone is that this
 *    one is not restricted to a subset of verbs. Also, there can be
 *    multiple instances of this one in the g_Drones collection.
 */

class PigPilotDrone : public Drone
{
public:
    PigPilotDrone(IshipIGC* id) 
        : Drone(id, c_dtPigPilot)
    {};

protected:
    virtual Goal* GetNewDefaultGoal(void);
//    virtual void Update(Time now, float dt);
};


/*-------------------------------------------------------------------------
 * Class: BountyHunter
 *-------------------------------------------------------------------------
 * Purpose:
 *    These are mercenaries for hire.  They accept ONE kill command, and then
 *    execute it at all costs.  Once done, they ride off into the sunset (disappear)
 */
class BountyHunter : public Drone
{
public:
    BountyHunter(IshipIGC* id)
        : Drone(id, c_dtBountyHunter), m_pTarget(NULL)
    {};

    virtual void SetGoal(CommandType cm, ImodelIGC* pTarget = NULL, bool reply = false);
    virtual Goal* GetNewDefaultGoal(void);

    TRef<ImodelIGC> m_pTarget;          // This is their target, which is set only ONCE
};


/*-------------------------------------------------------------------------
 * Class: Pirate
 *-------------------------------------------------------------------------
 * Purpose:
 *    These guys come into the sector, pick a leader, and then as a group try to 
 *    kill all of the miners in the sector.  If their leader gets hurt too much, then
 *    they all run.
 */

class Pirate : public Drone
{
public:
    Pirate(IshipIGC* id) :
      Drone(id, c_dtPirate)
    {};

    Goal* GetNewDefaultGoal(void);
};


/*-------------------------------------------------------------------------
 * Class: DumbDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *    They can do anything, but their default command is to do nothing (and take no time doing it).
 *    They are a computationally free drone, meant for performance tests, and that's it.
 */

class DumbDrone : public Drone
{
public:
    DumbDrone(IshipIGC* id) 
        : Drone(id, c_dtDummy)
    {};

protected:
    virtual Goal* GetNewDefaultGoal(void) 
    { 
        return NULL; 
    }; 
};

#endif
