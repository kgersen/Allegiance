#ifndef MINING_H_
#define MINING_H_

/*-------------------------------------------------------------------------
 * Constant: Helium Three Rate
 *-------------------------------------------------------------------------
 * Purpose:
 *    This the amount of helium three that a mining drone can get from an
 *    asteroid per second.  This should problably be moved into the database
 *    at some point.
 */

const float c_flHeliumThreeRate = 80.0f;


/*-------------------------------------------------------------------------
 * Class: MiningDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *    Mining drones are another special instance of the Drone class.  They 
 *    need to keep track of how much helium three they have, until IGC supports
 *    helium three parts
 */

class MiningDrone : public Drone
{
public:
  MiningDrone(IshipIGC* id)        // Constructor
    : Drone(id, c_dtMining),      
          fMiningTimeLimit(120.0f),      // This is the number of seconds that the miner can mine before he is full
          fMiningTime(0.0f)          // Start with no helium three
  {};

    virtual void DockEvent(void)      // Add the appropriate amount of money to the team, and reset my mining time
                      // The amount of helium three on a ship should really be in IGC, not here.
                      // This was a good temporary setup
    {
        if (fMiningTime != 0.0f)
        {
            m_pShip->GetMission()->GetIgcSite()->PaydayEvent(m_pShip->GetSide(), fMiningTime * c_flHeliumThreeRate);
            fMiningTime = 0.0f;
        }
    };

    ImodelIGC * GetUnloadStation()
    {
        IstationIGC * pstation = GetStation();

        if (OBJECT_IS_BAD(pstation) || !pstation->GetStationType()->HasCapability(c_sabmUnload))
        {
            return FindTarget(m_pShip,
                                   c_ttFriendly | c_ttStation | c_ttNearest | c_ttAnyCluster,
                                   NULL, NULL, NULL, NULL,
                                   c_sabmUnload);
        }
        else
            return pstation;
    }

protected:
  float fMiningTimeLimit;          // How long to mine before having to unload
  float fMiningTime;            // How long have I been mining since the last unload

    virtual Goal* GetNewDefaultGoal(void);  // Mining/Unloading is the default

  friend class MineGoal;          // These guys need to know about my miningTime and Limit
    friend class UnloadGoal;
};


/*-------------------------------------------------------------------------
 * Class: UnloadGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is a mining drone specific goal.  It is satisfied when the mining drone
 *    is "empty" (mining time = 0).  It uses the Drone::FindBestStation to find a station
 *    if one is not specified
 */

class UnloadGoal : public NewGotoGoal
{
public:
  UnloadGoal (MiningDrone* pDrone, IstationIGC* pStation) :        // Constructor
      NewGotoGoal(pDrone, pStation)              
  {};

  virtual void Update(Time now, float dt);                // Run the main loop

    virtual bool Done()                            // Done if mining time = 0
    {
        if (OBJECT_IS_BAD(m_pTarget) ||
            (m_pTarget->GetObjectType() != OT_station) ||
            !((IstationIGC*)(ImodelIGC*)m_pTarget)->GetStationType()->HasCapability(c_sabmUnload))
        {
            return true;
        }

        MiningDrone* pDrone;
        CastTo(pDrone, m_pDrone);
        return (pDrone->fMiningTime <= 0.0f);
    };

    virtual void DoneEvent()                        // Announce completion
    {
        m_pDrone->Verbose("Helium 3 unloaded");
    };

    virtual void Status()                          // Tell about target station, and how much mining we've done
    {
        if (m_pTarget)
            Report("I'm unloading at %s", m_pTarget->GetName());
        else
            Report("Looking for a station for unloading");

        MiningDrone* pDrone;
        CastTo(pDrone, m_pDrone);
        Report("I have mined for %d seconds", (int) pDrone->fMiningTime);  // todo: add HOW MUCH HELIUM 3
    };
};


/*-------------------------------------------------------------------------
 * Class: MineGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is a mining drone specific goal.  The miner takes a target asteroid,
 *    or else finds the closest one, and mines until it reaches it's limit.
 */

class MineGoal : public NewGotoGoal
{
public:
  MineGoal (MiningDrone* pDrone, ImodelIGC* pTarget) :          // Constructor
    NewGotoGoal(pDrone, pTarget, Waypoint::c_oGoto)              
  {};

  virtual void Update(Time now, float dt);

    virtual bool Done()                           // Done when we have reached our mining limit
    {
	    if ((OBJECT_IS_BAD(m_pTarget) ||
            (m_pTarget->GetObjectType() != OT_asteroid)) ||
            !((IasteroidIGC*)((ImodelIGC*)m_pTarget))->HasCapability(c_aabmMineHe3))
	    {
            return true;
        }
     
        MiningDrone* pDrone;
        CastTo(pDrone, m_pDrone);

        return (pDrone->fMiningTime >= pDrone->fMiningTimeLimit); 
    };

    virtual void DoneEvent()                      // We are going to want to keep mining, but first we have to unload
    {
    };

    virtual void Status()                         // Info about what we're mining, and our progress so far
    {
        if (m_pTarget) 
            Report("I'm mining %s", GetModelName(m_pTarget));
        else 
            Report("I'm about to start mining");

        MiningDrone* pDrone;
        CastTo(pDrone, m_pDrone);
        Report("I at %d%% of capacity.", int(100.0f * pDrone->fMiningTime / pDrone->fMiningTimeLimit));  // todo : comment on how much helium 3
    };
};


#endif
