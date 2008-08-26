#ifndef GOAL_H_
#define GOAL_H_


/*-------------------------------------------------------------------------
 * Constant: HowClose
 *-------------------------------------------------------------------------
 * Purpose:
 *	  How close do I want the GotoGoal to put me to my target.
 *    Since I didn't want to have every call to GotoGoal have to specify the
 *    acceptable radius from the target, I overloaded the constructor, and let
 *    the default value be defined with this constant. 
 */

const float c_fHowClose = 40.0f;    //Leave a lot of slack so things won't bounce on autopilot


/*-------------------------------------------------------------------------
 * Constant: HowCloseFollow
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Probably don't want to follow something as close as I would want to goto
 *    it, so make that it's own variable
 */

const float c_fHowCloseFollow = 25.0f;



// Abstract Goal Types:

/*-------------------------------------------------------------------------
 * Abstract Class: Goal
 *-------------------------------------------------------------------------
 * Purpose:
 *    This class sets up the interface and framework for all of the goal classes
 *    and implements default behaviors so that the children don't have to redo code.
 */

class Goal 
{
public:
    Goal(Drone* pDrone, CommandType type) :						
      m_pDrone(pDrone),											
      m_pShip(pDrone->GetShip()),								
	  m_tCreated(Time::Now()), 
	  m_tLastUpdate(Time::Now()), 
	  m_ctType(type), 
	  DONE(true)
    {};

	virtual ~Goal()												// Need virtual destructor so that I can call Goal::~Goal for all children, too
    {};
    
    virtual void Update(Time now, float dt)						// Keep timing, and update CshipIGC targeting info
    { 
        m_tLastUpdate = now;
    };

    virtual bool Done(void)										// Never end
    { 
        return false; 
    };

    virtual void Status(void) = 0;								// Just an interface class

    virtual void DoneEvent(void)								// Default done message
    { 
        m_pDrone->Verbose("Goal Accomplished..."); 
    };

    CommandType GetType(void)									// Return the type of child this is
    {
        return m_ctType;
    };

    virtual ImodelIGC* GetTarget(void)							// Some goals do have targets, but assume that we don't
    {
        return NULL;
    };

protected:
    void Report(const char* format, ...);						// Call Gamesite::DroneSendChat()

    Drone*          m_pDrone;									// Keep track of the drone that is using this goal
    IshipIGC*       m_pShip;									// Can get this from the drone, but we use it all the time
    Time            m_tCreated;									// Keep some timing information
    Time            m_tLastUpdate;
    CommandType     m_ctType;									// Need to know a which child it is
    const bool      DONE;										// Just mapping true to the name DONE

    friend class Drone;											// Drone needs to get at this stuff
};

/*-------------------------------------------------------------------------
 * Abstract Class: TargetedGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    A majority of the goals need to keep a target.  Hold on to that TRef here,
 *    and handle the basic checks for whether that object is still valid, etc.
 */

class TargetedGoal : public Goal
{
public:
    TargetedGoal(Drone* pDrone, ImodelIGC* pTarget, CommandType type) :
      Goal(pDrone, type),										
	  m_pTarget(pTarget)
    {};
    
    virtual bool Done(void)										// Done if my object is no longer valid
    { 
        return (OBJECT_IS_BAD(m_pTarget)); 
    };
    
    virtual void DoneEvent(void)								// Default targeted goal done message
    {
        m_pDrone->Verbose("Target is no longer valid");
    };

    virtual ImodelIGC* GetTarget(void)							// Return the real target
    {
        return m_pTarget;
    };

protected:
    TRef<ImodelIGC> m_pTarget;									// Keep track of a target for the goal
};



// Actual Goal Types


/*-------------------------------------------------------------------------
 * Class: IdleGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Don't do anything but sit still and dodge.  This goal never ends.
 */

class IdleGoal : public Goal
{
public:
    IdleGoal (Drone* pDrone) :									
      Goal(pDrone, c_ctStop)
    {};
    
    virtual void Update(Time now, float dt);					

    virtual void Status(void)									
    { 
        Report("Awaiting orders"); 
    };
};


/*-------------------------------------------------------------------------
 * Class: StayPutGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Don't do anything but sit still and dodge.  This goal times out after
 *    the TIME_LIMIT.
 */

class StayPutGoal : public IdleGoal
{
public:
    StayPutGoal (Drone* pDrone) :								
      IdleGoal(pDrone), 
	  TIME_LIMIT(120.0f)
    {};
    
    virtual bool Done(void)										// Have we hit the time limit yet?
    {
        float length = m_tLastUpdate - m_tCreated;
        return (length >= TIME_LIMIT);
    };

    virtual void DoneEvent(void)								// If it ended, it was because it timed out
    { 
        m_pDrone->Verbose("I'm not going to just sit here forever"); 
    };

protected:
    const float TIME_LIMIT;										// How long will I just sit there?
};


/*-------------------------------------------------------------------------
 * Class: GotoGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Use the DoGotoAction to get within the specified radius of the target,
 *    and maybe stop when we get there.
 */

class GotoGoal : public TargetedGoal
{
public:
    GotoGoal (Drone* pDrone, ImodelIGC* pTarget, bool stopThere = false, bool dodgeBullets = true) :
      TargetedGoal(pDrone,pTarget, c_ctGoto),					
	  fGotThere(false), 
	  fStopThere(stopThere), 
	  fDodgeBullets(dodgeBullets)
    {
        distFromTarget = c_fHowClose + pTarget->GetRadius();
        m_dist2 = distFromTarget * distFromTarget;
    };

    GotoGoal (Drone* pDrone, ImodelIGC* pTarget, float radius, bool stopThere = false, bool dodgeBullets = true) :
      TargetedGoal(pDrone,pTarget, c_ctGoto),					
	  fGotThere(false), 
	  distFromTarget(radius - 1.0f),							// If we are given a radius, then adjust it to account for the
	  fStopThere(stopThere),									// weirdness of DoGotoAction (considers a goal done if the distance between the
	  fDodgeBullets(dodgeBullets)								// goal and the current pos is less that sqrt(1) )
    {
		m_dist2 = distFromTarget * distFromTarget;  
	};

    virtual void Update(Time now, float dt);

    virtual void Status() 
    { 
        Report("Going to %s", GetModelName(m_pTarget)); 
    };

    virtual bool Done()											// Either the object is bad, or we got there
    {
        //Hack time ... do a special case for going to a warp: you are there if you are in the same sector as its destination
        if (m_pTarget->GetObjectType() == OT_warp)
        {
            IwarpIGC*   pwarpDestination = ((IwarpIGC*)((ImodelIGC*)m_pTarget))->GetDestination();
            assert (pwarpDestination);
            return m_pShip->GetCluster() == pwarpDestination->GetCluster();
        } 
        return (fGotThere || TargetedGoal::Done()) ;
    };

    virtual void DoneEvent()									// Report a message appropriate to why we finished
    { 
        if (TargetedGoal::Done())
            TargetedGoal::DoneEvent();
        else
            m_pDrone->Verbose("I have reached %s", GetModelName(m_pTarget));
    };

private:
    float distFromTarget;										// Need to figure out the actual range that we want (but only need to do it once)
    bool fGotThere;												// Set this flag according to DoGotoAction
    bool fStopThere;											// Do we want to push a stop command when we get there?
	bool fDodgeBullets;											// Sometimes, we don't want to dodge bullets in the process
    float m_dist2;												// Only compute distance*distance one time
};

class   NewGotoGoal : public TargetedGoal
{
    public:
        NewGotoGoal (Drone*                 pDrone,
                     ImodelIGC*             pTarget, 
                     Waypoint::Objective    objective = Waypoint::c_oEnter,
                     float                  offset = c_fHowClose,
                     CommandType            ct = c_ctGoto)
        :
            TargetedGoal(pDrone, pTarget, ct),
            m_gotoplan(pDrone->GetShip())
        {
            m_gotoplan.Set(objective, pTarget, offset);
        };

        virtual void Update(Time now, float dt)
        {
            if (m_pTarget)
                m_gotoplan.Execute(now, dt, true);
        }

        virtual void Status() 
        { 
            Report("Going to %s", GetModelName(m_pTarget)); 
        };

        virtual bool Done()											// Either the object is bad, or we got there
        {
            return !m_pTarget; //m_gotoplan.GetMaskWaypoints() == 0;
        };

        virtual void DoneEvent()									// Report a message appropriate to why we finished
        { 
            if (TargetedGoal::Done())
                TargetedGoal::DoneEvent();
            else
                m_pDrone->Verbose("I have reached %s", GetModelName(m_pTarget));
        };

        void SetGotoPlan(Waypoint::Objective o, ImodelIGC * pmodelTarget, float offset = 0.0f)
        {
            m_pTarget = pmodelTarget;
            m_gotoplan.Set(o, pmodelTarget, offset);
        }

    protected:
        GotoPlan    m_gotoplan;
};


/*-------------------------------------------------------------------------
 * Class: GotoSameSectorGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    If we aren't in the same sector as the target, then we need to go through
 *    alephs until we are in the right sector.  FindPath should always return the 
 *    correct to go through next.
 */

class GotoSameSectorGoal : public GotoGoal
{
public:
    GotoSameSectorGoal (Drone *pDrone, ImodelIGC* pTarget, bool dodgeBullets = true) :			
      GotoGoal(pDrone, FindPath(pDrone->GetShip(), pTarget), 0.0f, false, dodgeBullets), 
      m_pFinalTarget(pTarget),
      m_pCurCluster(pDrone->GetShip()->GetCluster()), 
	  m_pTarCluster(pTarget->GetCluster())
    {};

    virtual void Update(Time now, float dt)														// If the clusters of either of the objects change,
    {																							// then recompute the FindPath
        assert (!OBJECT_IS_BAD(m_pTarget) && !OBJECT_IS_BAD(m_pFinalTarget));

		if ((m_pShip->GetCluster() != m_pCurCluster) || (m_pFinalTarget->GetCluster() != m_pTarCluster))
        {
            m_pCurCluster = m_pShip->GetCluster();
            m_pTarCluster = m_pFinalTarget->GetCluster();
            m_pTarget = FindPath(m_pShip, m_pFinalTarget);

            if (Done())
                return;
        }

        GotoGoal::Update(now, dt);																// Just goto the aleph
    };

    virtual bool Done()																			// If either of the objects is no longer valid,
    {																							// or we have succeeding in getting to the same sector
        return (TargetedGoal::Done() || OBJECT_IS_BAD(m_pFinalTarget) || (m_pShip->GetCluster() == m_pFinalTarget->GetCluster())); 
    };

    virtual void DoneEvent()																	// Announce that we got there
    { 
        if (TargetedGoal::Done())
            TargetedGoal::DoneEvent();
        else
            m_pDrone->Verbose("%s and I are both in %s", GetModelName(m_pFinalTarget), m_pTarCluster->GetName());
    };

private:
    TRef<ImodelIGC> m_pFinalTarget;			// This is the model we're trying to get to
    TRef<IclusterIGC> m_pCurCluster;		// This is the cluster we are currently in														
    TRef<IclusterIGC> m_pTarCluster;		// This is the cluster that we are trying to get to
};


/*-------------------------------------------------------------------------
 * Class: FollowGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Fly on the wing of the target object.  If the target is static, then this
 *    is just an un-ending goto command
 */

class FollowGoal : public TargetedGoal
{
public:
    FollowGoal (Drone* pDrone, ImodelIGC* pTarget) :					
      TargetedGoal(pDrone,pTarget, c_ctFollow)
    {};

    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("Following %s", GetModelName(m_pTarget)); 
    };
};


/*-------------------------------------------------------------------------
 * Class: DestroyGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Attack the target with all of your guns.  If you don't have any guns,
 *    then try to ram it to death
 */

class DestroyGoal : public TargetedGoal
{
public:
    DestroyGoal (Drone* pDrone, ImodelIGC* pTarget) :					
      TargetedGoal(pDrone,pTarget, c_ctDestroy), 
	  sCurrent(c_None)
    {};

    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("Attempting to destroy %s", GetModelName(m_pTarget)); 
    };

    virtual void DoneEvent(void)										// Somehow the object is no longer around
    { 
        m_pDrone->Verbose("%s has been destroyed", GetModelName(m_pTarget)); 
    };

protected:
    enum State															// Maintian some state information for verbose comments	
    { 
        c_Shooting,														// Four different "modes"	
        c_Goto, 
        c_Ramming, 
        c_None 
    };

    virtual void SetState(State sNew)									// Call this all the time, and only use it if the state changed
    {
        if (sNew == sCurrent)
            return;
        sCurrent = sNew;
        
        switch (sNew) 
        {
        case c_Shooting:
            m_pDrone->Verbose("I'm in range, and opening fire on %s", GetModelName(m_pTarget));
            break;
        case c_Goto:
            m_pDrone->Verbose("%s is out of range, I'm going after it", GetModelName(m_pTarget));
            break;
        case c_Ramming:
            m_pDrone->Verbose("I don't have any weapons mounted, I'm trying to RAM %s", GetModelName(m_pTarget));
            break;
        }
    };

    State sCurrent;														// Remember the state
};


/*-------------------------------------------------------------------------
 * Class: KillAnythingGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Look for the closest enemy in the sector, and try to destroy it.  If there
 *    aren't any enemies, then just hang tight, and dodge.
 */

class KillAnythingGoal : public Goal
{
public:
    KillAnythingGoal(Drone *pDrone) :									
      Goal(pDrone, c_ctDestroy)
    {};

    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("Looking for enemies"); 
    };
};


/*-------------------------------------------------------------------------
 * Class: SameSectorDestroyGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Same as the DestroyGoal, except we don't actually leave the sector trying
 *    to kill this guy.
 */

class SameSectorDestroyGoal : public DestroyGoal
{
public:
    SameSectorDestroyGoal(Drone* pDrone, ImodelIGC* pTarget) :
      DestroyGoal(pDrone, pTarget)
    {};

    virtual bool Done(void)											// End if our clusters are not the same
    {
        return (DestroyGoal::Done() || m_pShip->GetCluster() != m_pTarget->GetCluster());
    };

    virtual void DoneEvent(void)									// Could be that we killed him, or he might have left the sector
    {
        if (DestroyGoal::Done())
            DestroyGoal::DoneEvent();
        else
            m_pDrone->Verbose("%s has left the sector", GetModelName(m_pTarget));
    };
};


/*-------------------------------------------------------------------------
 * Class: DisableGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Same as DestroyGoal, except we stop once we have taken out the target's shields
 */

class DisableGoal : public DestroyGoal
{
public:
	DisableGoal (Drone* pDrone, ImodelIGC* pTarget) :
	  DestroyGoal(pDrone, pTarget)
	{};

    virtual bool Done(void)									// Check if the shields have been destroyed
    {
        switch (m_pTarget->GetObjectType())						// Different ways to get the shields
        {
        case OT_ship:
        {
                IshieldIGC* shield = (IshieldIGC*) ((IshipIGC*)m_pTarget.operator->())->GetMountedPart(ET_Shield, 0);
                return ((shield == NULL) || (shield->GetFraction() < 0.0f));
        }
        case OT_station:
        {
                IstationIGC* pTargetStation = (IstationIGC*) m_pTarget.operator->();
                return ( pTargetStation->GetShieldFraction() < 0.0f);
        }
        default:
            return true;
        }
    };

    virtual void Status(void) 
    { 
        Report("Attempting to disable %s", GetModelName(m_pTarget)); 
    };

    virtual void DoneEvent(void)							// Could have died, but hopefully we disabled it
    { 
        if (OBJECT_IS_BAD(m_pTarget))
            m_pDrone->Verbose("%s was destroyed while I was attempting to disable it", GetModelName(m_pTarget));
        else 
            m_pDrone->Verbose("%s is disabled", GetModelName(m_pTarget));
    };
};


/*-------------------------------------------------------------------------
 * Class: DefendingDestroyGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Try to destroy this guy, but end if you get too far from your defend target, 
 *    or if there are better targets for you to try to get.
 */

class DefendingDestroyGoal : public DestroyGoal {
public:
    DefendingDestroyGoal (Drone* pDrone, ImodelIGC* pTarget, ImodelIGC* pDefendee, float range) :
      DestroyGoal(pDrone,pTarget), m_pDefendee(pDefendee), fRangeSquared(range * range)
    {};

    virtual bool Done(void) 
    {
        if (DestroyGoal::Done())                // The normal destroy ending conditions still apply
            return true;

        if (!OBJECT_IS_BAD(m_pDefendee)) 
        {                                       // If my defendee dies, I probably want to go ahead
                                                // and kill the guy

                                                // Otherwise, make sure I'm not too far from home
            Vector vFromHome = m_pShip->GetPosition() - m_pDefendee->GetPosition();
            return (vFromHome.LengthSquared() > fRangeSquared);
        }

        // todo: add a check for other possible (more worthy targets here)
        return false;
    };

    virtual void DoneEvent(void) 
    { 
        if (OBJECT_IS_BAD(m_pDefendee))
			m_pDrone->Verbose("I have failed");
		else
			m_pDrone->Verbose("%s is no longer a threat to %s", GetModelName(m_pTarget), GetModelName(m_pDefendee)); 
    };

protected:
    const TRef<ImodelIGC> m_pDefendee;			// Need to remember who I am defending
    const float  fRangeSquared;					// How far can I get from him? (only compute the square once)
};


/*-------------------------------------------------------------------------
 * Class: PatrolGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Fly back and forth from the starting position and the target, attacking
 *    any enemies that I find on the way
 */

class PatrolGoal : public TargetedGoal
{
public:
    PatrolGoal(Drone* pDrone, ImodelIGC* pTarget, Vector vSweetSpot) : 
      TargetedGoal(pDrone, pTarget, c_ctPatrol), 
	  m_vFavoriteSpot(vSweetSpot), 
	  m_bToSecondTarget(false)
    {};

    PatrolGoal(Drone* pDrone, ImodelIGC* pTarget) : 
      TargetedGoal(pDrone, pTarget, c_ctPatrol), 
	  m_vFavoriteSpot(pDrone->GetShip()->GetPosition()), 
	  m_bToSecondTarget(false)
    {};
    
    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("I'm patrolling"); 
    };

protected:
    Vector m_vFavoriteSpot;		// This is the starting point (one of two targets), the other target is m_pTarget
    bool m_bToSecondTarget;		// Remember which way I am going right now
};


/*-------------------------------------------------------------------------
 * Class: PickupGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Go pickup this target.  Duh.  We know that we are done because that object would
 *    not exist any more.
 */

class PickupGoal : public TargetedGoal
{
public:
    PickupGoal (Drone* pDrone, ImodelIGC* pTarget) :
      TargetedGoal(pDrone,pTarget, c_ctPickup)
    {};

    virtual void Update(Time now, float dt);

	virtual void DoneEvent(void)
	{
		m_pDrone->Verbose("Got it");
	};

    virtual void Status(void) 
    { 
        Report("Attempting to pick up %s", GetModelName(m_pTarget)); 
    };
};


/*-------------------------------------------------------------------------
 * Class: RepairGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get to some station to repair myself.  Goal satisfied when my hull is 
 *    at full strength.  This goal goes with the instincts.
 */

class RepairGoal : public NewGotoGoal
{
public:
    RepairGoal (Drone* pDrone, ImodelIGC* pStation) :
      NewGotoGoal(pDrone,
                  pStation,
                  Waypoint::c_oEnter, 0.0f, c_ctRepair)
    {};

    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("Heading to %s to repair myself", GetModelName(m_pTarget)); 
    };

    virtual bool Done(void)		// My hull has all of it's hitpoints.
    { 
        return (m_pShip->GetFraction() >= 1.0f) &&
               (GetAmmoState(m_pShip) != c_asEmpty); 
    };

	virtual void DoneEvent(void)
	{
		m_pDrone->Verbose("All better");
	};
};


/*-------------------------------------------------------------------------
 * Class: MaybeRepairGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Repair, but end if there are any enemies in the sector.  Repairing just
 *    seems like a good thing to do when nobody is around.  Right now, this just
 *    gets called from KillAnythingGoal.
 */

class MaybeRepairGoal : public RepairGoal
{
public:
    MaybeRepairGoal(Drone *pDrone, IstationIGC* pstation) :
      RepairGoal(pDrone, pstation)
    {};

    virtual bool Done(void)		// If we can find any enemy ships, then we're done
    {
        return (RepairGoal::Done() || FindTarget(m_pShip, 
                                                 c_ttEnemy | c_ttShip));
    };
};


/*-------------------------------------------------------------------------
 * Class: CaptureGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Make the target station have the same side as you.  When it is an enemy
 *    station, that means disabling it, and then docking to claim it.
 */

class CaptureGoal : public NewGotoGoal
{
public:
    CaptureGoal (Drone *pDrone, ImodelIGC* pStation) :
      NewGotoGoal(pDrone, pStation)
    {};

    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("Attempting to capture %s", GetModelName(m_pTarget)); 
    };

    virtual bool Done(void)			// Done if the station dies, or if it is on our side.
    { 
        return (TargetedGoal::Done() || m_pTarget->GetSide() == m_pShip->GetSide()); 
    };

	virtual void DoneEvent(void)
	{
		if (TargetedGoal::Done())
			TargetedGoal::DoneEvent();
		else
			m_pDrone->Verbose("%s is on our side", GetModelName(m_pTarget));
	};
};


/*-------------------------------------------------------------------------
 * Class: DefendGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    If the target is static, then I'll orbit it.  Dynamic objects, I just follow.
 *    If any enemies come too close to my defendee, then I try to destroy them.
 */

class DefendGoal : public TargetedGoal
{
public:
    DefendGoal (Drone* pDrone, ImodelIGC* pTarget) :
      TargetedGoal(pDrone,pTarget, c_ctDefend), 
	  fDone(false)
    {
        if (OBJECT_IS_BAD(m_pTarget))
            fDone = true;
    };

    virtual void Update(Time now, float dt);

    virtual bool Done(void)
    {
        if (fDone)		// special message if initial target is not valid
        {
            m_pDrone->Verbose("Defend target is not valid.");
            return true;
        }

        if (OBJECT_IS_BAD(m_pTarget)) 					// if we were defending something, and it died, 
		{												// then we could try to defend other stations in our sector
            m_pTarget = FindTarget(m_pShip,
                                   c_ttFriendly | c_ttStation | c_ttNearest);

            if (!m_pTarget) 
			{
                return true;
            }
            m_pDrone->Verbose("I have failed, now defending %s", GetModelName(m_pTarget));
        }
        return false;
    };

    virtual void Status(void) 
    { 
        Report("Defending %s", GetModelName(m_pTarget)); 
    };

private:
    bool    fDone;      // in case of bad initial targets
};


/*-------------------------------------------------------------------------
 * Class: ScoutGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    If we are given a target, then start spiraling out from there.  If not, spiral
 *    around 0,0,0.  Spiral out and then back in (radii based on Inner and Outer Limit vars)
 *    Report any enemies in scan range (report each on once).  Run away from enemies.
 */

class ScoutGoal : public TargetedGoal
{
public:
    ScoutGoal (Drone* pDrone, ImodelIGC* pTarget) :
      TargetedGoal(pDrone,pTarget, c_ctScout),
      m_vmReportedEnemies(0,0), 
	  flRadIncreasePerCycle(2.0f), 
	  sCurrent(c_None),
	  c_flOuterLimit(5000.0f), 
	  c_flInnerLimit(100.0f), 
	  radius(100.0f)
    {
		if (!OBJECT_IS_BAD(pTarget)) 
			flRadIncreasePerCycle = 0.0f;
	};

    ~ScoutGoal()							// Clear our reported enemies memory
    {
        for (int i = m_vmReportedEnemies.GetCount(); i > 0; i--)
        {
            m_vmReportedEnemies[i-1]->Release();
        }
    }

    virtual void Update(Time now, float dt);

	virtual bool Done(void)					// Never stop, unless told to
	{ 
		return false; 
	};
    
	virtual void Status(void)
    { 
		if (m_pTarget)
	        Report("I'm scouting around %s", GetModelName(m_pTarget)); 
		else
			Report("I'm scouting %s", m_pShip->GetCluster()->GetName());

        State sOld = sCurrent;  // print out state message, too
        SetState(c_None);
        SetState(sOld);
    };

protected:
    bool Announce(IshipIGC* pObject)		// Announce a ship.  If we already announced it, then just return
    {
        // check to see if we have already reported it
        if (m_vmReportedEnemies.Find(pObject) != -1)
            return false;

        // if not, announce it
        Report("Reporting: I found %s in %s", pObject->GetName(), m_pShip->GetCluster()->GetName());

        // add to the list of things we have reported
        pObject->AddRef();
        m_vmReportedEnemies.PushEnd(pObject);
        return true;
    };

private:
    float radius;												// Current orbital radius
    TVector<IshipIGC*, DefaultEquals> m_vmReportedEnemies;		// Reported enemy list
    float flRadIncreasePerCycle;								// Amount that radius changes per cycle
	const float c_flOuterLimit;									// Outer radius size
	const float c_flInnerLimit;									// Inner radius size

    enum State													// Three two states
    { 
        c_Orbitting, 
        c_Running, 
        c_None 
    };

    virtual void SetState(State sNew)
    {
        if (sNew == sCurrent)
            return;
        sCurrent = sNew;
        
        switch (sNew) 
        {
        case c_Orbitting:
            if (m_pTarget)
				m_pDrone->Verbose("I'm orbitting %s, currently with radius %d", GetModelName(m_pTarget), (int) radius);
			else 
				m_pDrone->Verbose("I'm scouting %s", m_pShip->GetCluster()->GetName());
            break;
        case c_Running:
            m_pDrone->Verbose("I'm attempting to stay away from enemy ships");
            break;
        }
    };

    State sCurrent;												// Current state
};


/*-------------------------------------------------------------------------
 * Class: DisappearGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    Some of the special characters want to go through an aleph and never come out the
 *    other side.  This is that disappearing trick.
 * Notes:
 *    Rob thinks that it is really bad and incredibly annoying to be chasing a guy, 
 *    almost killing him, into an aleph.  Then when you follow him through, he's not
 *    on the other side.  I think that is cool, but he hates it.  He was thinking that
 *    maybe we could only make them "disappear" if they are not being targetted at all.
 */

class DisappearGoal : public GotoGoal
{
public:
    DisappearGoal(Drone* pDrone) :		// Find the closest aleph
      GotoGoal(pDrone, FindTarget(pDrone->GetShip(), c_ttNeutral | c_ttWarp | c_ttNearest), 0.0f)
    {
        pDrone->Disappear();	// Set flag in the drone class
        if (!m_pTarget)			
            pDrone->Die();		// If I couldn't find an aleph, then just self destruct
    };

    virtual void Status(void)
    {
		if (m_pDrone->GetDroneType() == c_dtBountyHunter)
		{
			BountyHunter* pBounty;
			CastTo(pBounty, m_pDrone);

			// Bounty hunter is ONLY doing this if his mission is accomplished.  He repairs at base instead of running away
	        Report("Mission Accomplished.  %s is no more", GetModelName(pBounty->m_pTarget));
		}
		else		// Right now this is just a pirate
		{
			if (m_pShip->GetFraction() <= (1.0f - m_pDrone->GetBravery()))	// Either my leader chickened out, or I did
			{
				Report("Pirate guild retreating");
			}
			else	// There's no more miners to kill 
			{
				Report("Arggh..Jimlad, There's no bounty to be had, you scurvy nave.");
			}
		}
    };
};


/*-------------------------------------------------------------------------
 * Class: MimicGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is only for the pirates right now.  Basically, it is a hack on the 
 *    normal drone structure.  The drone's goal is always to mimic, but the 
 *    mimic goal keeps a goal of it's own, and calls that update every time.
 *    Basically, he follows the target when the target is moving, and attacks 
 *    what his "leader" or target is attacking, etc.
 */

class MimicGoal: public TargetedGoal
{
public:
    MimicGoal(Drone *pDrone, Drone* pTarget) :
      TargetedGoal(pDrone, pTarget->GetShip(), c_ctMimic), 
	  m_pdTarget(pTarget), 
	  m_pGoal(NULL)
    {};

    virtual void Update(Time now, float dt);
    
    virtual void Status(void)
    {
        Report("Following %s, he's our leader", GetModelName(m_pTarget));
    };

    virtual void DoneEvent(void)
    {
        m_pDrone->Verbose("%s is gone, time for a new leader", GetModelName(m_pTarget));
    };

protected:

    void SetGoal(CommandType ctype, ImodelIGC* pTarget)
    {
        if (m_pGoal && m_pGoal->GetType() == ctype && m_pGoal->GetTarget() == pTarget)		// We're already doing it, done
            return;

        if (ctype == c_ctNoCommand || ctype == c_ctMimic)									// Don't want to mimic a mimic (that would be bad)
            return;

        if (m_pGoal)																		// It's new, so clear the old goal
        {
            delete m_pGoal;
            m_pGoal = NULL;
        }

        if (ctype != c_ctDestroy)															// Follow him, unless he is killing something
        {
            ctype = c_ctFollow;
            pTarget = m_pTarget;
        }

        m_pDrone->SetGoal(ctype, pTarget);													// Play the goal games
        m_pGoal = m_pDrone->GetGoal();
        m_pDrone->m_goal = NULL;
        m_pDrone->RestoreGoal();
    };
    
    Drone * m_pdTarget;					// The drone that you are mimicing
    Goal *  m_pGoal;					// The goal you are running to mimic
};


/*-------------------------------------------------------------------------
 * Class: FaceGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    We just want to turn to face our target.  Meant primarily for automated
 *    client frame rate testing.
 */

class FaceGoal : public TargetedGoal
{
public:
	FaceGoal(Drone* pDrone, ImodelIGC* pTarget) :
	  TargetedGoal(pDrone, pTarget, c_ctFace)
	{};

	virtual void Update(Time now, float dt);

	virtual bool Done(void)					// Can't face something in a different cluster
	{
		return (!m_pTarget || m_pTarget->GetCluster() != m_pShip->GetCluster());
	};

	virtual void Status(void)
	{
		Report("Facing %s", GetModelName(m_pTarget));
	};
};

/*-------------------------------------------------------------------------
 * Class: ConstructGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    We just want to turn to face our target.  Meant primarily for automated
 *    client frame rate testing.
 */

class ConstructGoal : public TargetedGoal
{
public:
	ConstructGoal(Drone*            pDrone,
                  IstationTypeIGC*  pstationtype,
                  IasteroidIGC*     pasteroid)
    :
        TargetedGoal(pDrone, pasteroid, c_ctConstruct),
        m_pstationtype(pstationtype),
        m_doneF(false)
	{
    };

	virtual void Update(Time now, float dt);

	virtual bool Done(void)					// Can't face something in a different cluster
	{
        return m_doneF && !OBJECT_IS_BAD(m_pTarget);
	};

    virtual void DoneEvent(void)
    {
        assert (!OBJECT_IS_BAD(m_pTarget));        
        m_pDrone->SendChat(m_pstationtype->GetCompletionSound(), "%s completed.", m_pstationtype->GetName());
        m_pDrone->GetShip()->GetMission()->GetIgcSite()->BuildStation((IasteroidIGC*)((ImodelIGC*)m_pTarget),
                                                                      m_pDrone->GetShip()->GetSide(),
                                                                      m_pstationtype,
                                                                      m_tLastUpdate);

        //Quietly kill the ship (after nuking its parts to prevent treasure from being created)
        {
            PartLinkIGC*    plink;
            while (plink = m_pDrone->GetShip()->GetParts()->first())  //Not ==
                plink->data()->Terminate();
        }
        m_pDrone->Die();
    };

	virtual void Status(void)
	{
		Report("Building %s", m_pstationtype->GetName());
	};

private:
    TRef<IstationTypeIGC>   m_pstationtype;

    bool                    m_doneF;
};


/*-------------------------------------------------------------------------
 * Class: TestGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    This was meant to just cycle through all of the goals possible for that drone,
 *    but it never got implemented completely.  :(  Good in theory.
 */

class TestGoal : public Goal
{
public:
    TestGoal(Drone* pDrone) : 
      Goal(pDrone, c_ctTest), 
	  cidCurrent(0)
    {};

    virtual void Update(Time now, float dt);

    virtual void Status(void) 
    { 
        Report("Testing"); 
    };

    virtual bool Done(void)				// We tested all of the goals
    { 
        return (cidCurrent >= m_pShip->GetMission()->GetNcommands()); 
    };

    virtual void DoneEvent(void) 
    { 
        m_pDrone->Verbose("Done with test"); 
    };

private:
    CommandID cidCurrent;				// Which goal are we on right now
};


#endif
