#ifndef TURRET_H_
#define TURRET_H_


/*-------------------------------------------------------------------------
 * Class: TurretDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Turrets are gun towers that are dangerous when deployed, but vulnerable
 *    while moving.  When they do move, they CANNOT shoot, and they HAVE NO SHIELDS.
 */

class TurretDrone : public Drone
{
public:
    TurretDrone(IshipIGC* id) :											// Constructor, start deployed
        Drone(id, c_dtTurret),	// drone init 
		moving(true)				// set true so that we can prepare to fire
    { 
        PrepareToFire(); 
    };

    void PrepareToMove(void);											// Change to vulnerable moving state

    void PrepareToFire(void);											// Deploy and prepare to attack

protected:
    virtual void SetGoal(CommandType x, ImodelIGC* pTarget = NULL, bool reply = false);
																		// Redefine Drone::SetGoal(CommandType, ImodelIGC*, bool)
    Goal* GetNewDefaultGoal(void);										// Define the turret's default goal

private:
    bool moving;														// True = vulnerable, not shooting, False = no moving
};



/*-------------------------------------------------------------------------
 * Class: TurretAttackGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *	  The turret attack consists of three basic elements:
 *		1) if we are not given any targets, then stay put and shoot at the closest
 *		   enemy.
 *	    2) if we are given a static object as a target, and it is out of our range,
 *		   then redeploy so that we are in range, and start shooting at it.
 *		3) if we are given a dynamic object as a target, and it is out of range, 
 *		   then don't move, just keep doing #1.  Whenever that target is in range,
 *		   though, we will try to shoot at it.
 */

class TurretAttackGoal : public TargetedGoal
{
public:
    TurretAttackGoal (Drone* pDrone, ImodelIGC* pTarget) :				
      TargetedGoal(pDrone, pTarget, c_ctDestroy),						
	  m_pLastTarget(NULL)
    {};

    virtual void Update(Time now, float dt);							

    virtual void Status()												
    { 
        if (m_pTarget)
		{
			if (m_pLastTarget == m_pTarget)
	            Report("Attempting to destroy %s", GetModelName(m_pTarget));
			else
				Report("%s is out of range...", GetModelName(m_pTarget));
		}

		if (m_pLastTarget)
            Report("I'm currently attacking %s", GetModelName(m_pLastTarget));
        else
            Report("I'm on the lookout for enemy ships"); 
    };

    virtual bool Done(void)												
    { 
        return false;													// Don't ever have to stop, this what turrets do.
    };

private:
    TRef<ImodelIGC> m_pLastTarget;										// This is really just for status info, remember the last thing I was shooting at
};

#endif