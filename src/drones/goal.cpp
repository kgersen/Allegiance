#include "pch.h"


/*-------------------------------------------------------------------------
 * Function: Goal::Report
 *-------------------------------------------------------------------------
 * Purpose:
 *    This is really just a wrapper on DroneSendChat.  Pretty simple.
 */

void Goal::Report(const char* format, ...) 
{
    char szChat[256]; // $CRC: sync this with client
    va_list vl;
    va_start(vl, format);
    int cbChat = wvsprintfA(szChat, format, vl);
    assert(cbChat < sizeof(szChat));
    va_end(vl);
    m_pShip->GetMission()->GetIgcSite()->DroneSendChat(m_pShip, szChat, CHAT_TEAM, m_pShip);
}


/*-------------------------------------------------------------------------
 * Function: IdleGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Sit there and dodge.
 */

void IdleGoal::Update(Time now, float dt) 
{
    assert(!OBJECT_IS_BAD(m_pShip));

    // Stay still by going to your current location.  This lets the drones dodge things (aka projectiles)
    // even while they aren't moving.
    DoGotoAction(m_pShip, now, NULL, m_pShip->GetPosition(), Vector::GetZero(), 0.0f, dt, m_pDrone->GetMoveSkill());
    
    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: KillAnythingGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    If there are any enemies in the sector, then attack them, but don't leave
 *    the sector doing it.
 */

void KillAnythingGoal::Update(Time now, float dt) 
{
    assert(!OBJECT_IS_BAD(m_pShip));

    // Find the closest enemy ship
    ImodelIGC* pAttackTarget = FindTarget(m_pShip, 
                                          c_ttEnemy | c_ttNeutral | c_ttShip | c_ttNearest);

    if (pAttackTarget)	    // Now we've decided whether there is anyone to worry about, so attack them.  
    {
		// Use SameSectorDestroy, so that we don't get pulled away
        Goal* newGoal = new SameSectorDestroyGoal(m_pDrone, pAttackTarget);
        m_pDrone->SetGoal(newGoal);
        newGoal->Update(now, dt);
        Goal::Update(now,dt);
        return;
    }
    else					// There is no-one around.  Just dodge, or repair if we are hurt
    {
        if (m_pShip->GetFraction() < 1.0f)	        // If I'm hurt, I should go repair...
        {
            IstationIGC*    pstation = (IstationIGC*)(m_pDrone->GetRepairStation());
            if (pstation)
            {
                Goal* newGoal = new MaybeRepairGoal(m_pDrone, pstation);
                m_pDrone->SetGoal(newGoal);
                newGoal->Update(now, dt);
            }
        }
        else									// Otherwise just dodge
            DoGotoAction(m_pShip, now, NULL, m_pShip->GetPosition(), Vector::GetZero(), 0.0f, dt, m_pDrone->GetMoveSkill());
    }

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: GotoGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Go to the desired target, and push a stop command when I get there, if
 *    desired
 */

void GotoGoal::Update(Time now, float dt) 
{
    assert (!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));

    if (m_pTarget->GetCluster() != m_pShip->GetCluster())					// Make sure the target is in this sector
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now,dt);
        return;
    }

    if (DoGotoAction(m_pShip,												// This handles just about everything else
                     now,
                     m_pTarget,
                     m_pTarget->GetPosition(),
                     m_pTarget->GetVelocity(),
                     distFromTarget,
                     dt,
                     m_pDrone->GetMoveSkill(),
                     (m_pTarget->GetObjectType() == OT_warp),
					 false,
					 fDodgeBullets)) 
	{
        // Then we are there
        if (fStopThere && !fGotThere)  // make sure I only do this once
            m_pDrone->SetGoal(new StayPutGoal(m_pDrone));
        fGotThere = true;
    }
    
    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: FollowGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Go just behind the target... Farther behind if the target is moving.
 *    This is a simple way of "flying in formation"
 */

void FollowGoal::Update(Time now, float dt)
{
    assert (!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));			

    if (m_pTarget->GetCluster() != m_pShip->GetCluster())					// Make sure the target is in this sector
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));		
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }

    Vector pos = m_pTarget->GetPosition();		// Base everything off the pos and vel of the target
    Vector vel = m_pTarget->GetVelocity();

    pos -= m_pTarget->GetOrientation().GetForward() * (m_pTarget->GetRadius() + c_fHowCloseFollow);				// Fly in formation by flying behind
    pos -= vel / 10.0f;																							// Farther behind based on speed

    DoGotoAction(m_pShip, now, m_pTarget, pos, vel, m_pTarget->GetRadius(), dt, m_pDrone->GetMoveSkill());
    
    Goal::Update(now, dt);
};


/*-------------------------------------------------------------------------
 * Function: DestroyGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    To destroy the target.  If we don't have any weapons mounted, then try
 *    to ram the target.  If we do, then the current approach is to get into a 
 *    good position, and then just use the turret attack code.
 * Note:
 *    I was hoping to add 5 or 6 different attack styles here, and then have the
 *    drone choose between them based on the situation.  I didn't get to it.  :(
 */

void DestroyGoal::Update(Time now, float dt) {
    assert (!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));
    
    if (m_pTarget->GetCluster() != m_pShip->GetCluster())					// Make sure the target is in this sector
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }

    IweaponIGC* w = (IweaponIGC*)m_pShip->GetMountedPart(ET_Weapon, 0);		// Get the mounted weapon

    if (w) {
        IprojectileTypeIGC* pProjectile = w->GetProjectileType();
        float fRange = pProjectile->GetSpeed() * pProjectile->GetLifespan();	// Weapon range

        Vector vToTarg = m_pTarget->GetPosition() - m_pShip->GetPosition();

        // If we are within weapon range, and we aren't going to hit any friendlies then do turret attack
        if (fRange*fRange > vToTarg.LengthSquared())
        {
            if (StationaryAttackTarget(m_pShip, now, m_pTarget, dt, m_pDrone->GetShootSkill(), m_pDrone->GetMoveSkill(), true)) {
                SetState(c_Shooting);    
                Goal::Update(now, dt);
                return;
            }
        }
        else 
        {
            SetState(c_Goto);

			/*
            m_pDrone->SetGoal(new GotoGoal(m_pDrone, m_pTarget, fRange, false, false));
            m_pDrone->GetGoal()->Update(now, dt);
            Goal::Update(now, dt);
            return;
			*/

			// This is good in theory, but to pull it off, the ending condition of the Goto has to be the same as the fRange check above.
			// I fixed this case for the turretAttack relocation, but didn't get around to testing that fix here.
			// The advantage of doing this is that the mimic command will cause pirates and the like to fly in formation a little more often.

        }
    }
    else 
        SetState(c_Ramming);

    // Otherwise move towards target ( and ram it )
    DoGotoAction(m_pShip, now, m_pTarget, m_pTarget->GetPosition(), Vector::GetZero(), m_pTarget->GetRadius() + m_pShip->GetRadius() - c_fHowClose, dt, m_pDrone->GetMoveSkill(), false, false, false);

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: PatrolGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Find the closest enemy.   If there aren't any interesting enemies, then
 *    continue going to either the target or back to the starting position.
 */

void PatrolGoal::Update(Time now, float dt) {
    assert (!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));
    
    // Find the closest enemy
    ImodelIGC* pAttackTarget = FindTarget(m_pShip,
                                          c_ttEnemy | c_ttNeutral | c_ttShip | c_ttNearest);
    

    // If he is close enough, then attack him, otherwise continue patrolling
    if (pAttackTarget)
    {
        // Close enough = 1500.0??
        Vector vToAttackTarg = pAttackTarget->GetPosition() - m_pShip->GetPosition();

        // 1500 ^2 = 2250000
        if (vToAttackTarg.LengthSquared() < 2250000.0f)
        {
            m_pDrone->SetGoal(c_ctDestroy, pAttackTarget);
            m_pDrone->GetGoal()->Update(now, dt);
            Goal::Update(now, dt);
            return;
        }
    }

    if (m_bToSecondTarget)
    {
        if (DoGotoAction(m_pShip, now, NULL, m_vFavoriteSpot, Vector::GetZero(), 0.0f, dt, m_pDrone->GetMoveSkill())) 
        {
            // then we're there...
            m_bToSecondTarget = false;
            m_pDrone->Verbose("I got back to where I started.  Going back to %s", GetModelName(m_pTarget));
        }
    }
    else
    {
        if (DoGotoAction(m_pShip, now, m_pTarget, m_pTarget->GetPosition(), m_pTarget->GetVelocity(), c_fHowClose + m_pTarget->GetRadius() + m_pShip->GetRadius(), dt, m_pDrone->GetMoveSkill()))
        {
            // then we're there...
            m_bToSecondTarget = true;
            m_pDrone->Verbose("I got to %s, going back to where I started", GetModelName(m_pTarget));
        }
    }

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: PickupGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Just move to the center of the object.  The collision should pick it up
 */

void PickupGoal::Update(Time now, float dt) {
    assert (!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));

    if (m_pTarget->GetCluster() != m_pShip->GetCluster())
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }

    if (DoGotoAction(m_pShip, now, m_pTarget, m_pTarget->GetPosition(), Vector::GetZero(), 0.0f, dt, m_pDrone->GetMoveSkill()))
    {
        // Got there, now what do I do???
    }

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: DefendGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    If there are any enemies that could endanger the defendee, then attack them.
 *    If not, then just orbit or follow the target (static or dynamic).
 */

void DefendGoal::Update(Time now, float dt) {
    assert (!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));

    if (m_pTarget->GetCluster() != m_pShip->GetCluster())					// Make sure the target is in the same cluster
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }

    // If we are here, then we aren't currently targeting anyone, check 
    // for the closest enemy and decided if they are close enough to worry about
    ImodelIGC* pAttackTarget = FindTarget(m_pShip, 
                                          c_ttEnemy | c_ttNeutral | c_ttShip | c_ttNearest,
                                          NULL, NULL,
                                          &(m_pTarget->GetPosition()));

    // "Close Enough" == ScanRange / 2  
    float distToWorryAbout = m_pShip->GetHullType()->GetScannerRange() / 2;
    
    if (pAttackTarget)			// Is the closest enemy within that range?
    {
        Vector dist = pAttackTarget->GetPosition() - m_pTarget->GetPosition();
        if (dist.LengthSquared() > (distToWorryAbout * distToWorryAbout))
            pAttackTarget = NULL;   // too far away
    }
    
    // Now we've decided whether there is anyone to worry about, so attack them.  
    if (pAttackTarget)
    {
        Goal* newGoal = new DefendingDestroyGoal(m_pDrone, pAttackTarget, m_pTarget, distToWorryAbout);
        m_pDrone->SetGoal(newGoal);
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }
    // Otherwise try to orbit/follow the defendee
    else    
    {
        if (m_pTarget->GetAttributes() & c_mtStatic)
        {
            DoGotoAction(m_pShip, 
                         now, 
                         m_pTarget, 
                         m_pTarget->GetPosition(), 
                         Vector::GetZero(), // use top orbit speed 
                         (m_pTarget->GetRadius() + m_pShip->GetRadius()) * 2.0f,            // as good a radius as any??
                         dt,
                         m_pDrone->GetMoveSkill(),
                         false,
                         true);             // orbit
        }
        else
        {
            DoGotoAction(m_pShip, 
                         now, 
                         m_pTarget, 
                         m_pTarget->GetPosition(), 
                         m_pTarget->GetVelocity(), 
                         c_fHowClose + m_pTarget->GetRadius() + m_pShip->GetRadius(),
                         dt, 
                         m_pDrone->GetMoveSkill());
        }
    }

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: RepairGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Find a station and get to it, so that my hull will be repaired. 
 *    Run away from enemies.
 */

void RepairGoal::Update(Time now, float dt) {
    assert(m_pShip);

    if (OBJECT_IS_BAD(m_pTarget))
    {
        m_pTarget =  m_pDrone->GetRepairStation();
        // todo: remove this line when I am completely confident in FindBestStation and FindBestTarget:
        // CURTC: Or what happens if they can't find a station to repair at?
        if (m_pTarget)
            SetGotoPlan(Waypoint::c_oEnter, m_pTarget, c_fHowClose);
    }
    
    NewGotoGoal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: CaptureGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Disable the station's shields, and then dock with it to take it over
 */

void CaptureGoal::Update(Time now, float dt) {
    assert(!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));

    if (m_pTarget->GetCluster() != m_pShip->GetCluster())				// Make sure that we're in the right cluster
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }

    IstationIGC* pStation;
    CastTo(pStation, m_pTarget);

	// Check the shields
    if ((pStation->GetSide() != m_pShip->GetSide()) &&
        (pStation->GetShieldFraction() >= 0.0f))
    {
        m_pDrone->SetGoal(new DisableGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt); // skip NewGotoGoal::Update
        return;
    }

    //DoGotoAction(m_pShip, now, m_pTarget, m_pTarget->GetPosition(), Vector::GetZero(), 0.0f, dt, m_pDrone->GetMoveSkill(), false, false, false);
    NewGotoGoal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: ScoutGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    If we have a target, then do a spiral around it (by orbitting with an
 *    incrementally larger radius).  If not, spiral around the center of the cluster (0,0,0).
 *    Announce all enemies that are in our scan range.  Run away from enemies
 */

void ScoutGoal::Update(Time now, float dt) 
{
    assert (!OBJECT_IS_BAD(m_pShip));
		
	if (OBJECT_IS_BAD(m_pTarget))
		m_pTarget = NULL;

    if (m_pTarget && m_pTarget->GetCluster() != m_pShip->GetCluster())			// Make sure that we are in the right cluster
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }
	
	const ShipListIGC* ships = m_pShip->GetCluster()->GetShips();
	ShipLinkIGC* sLink = ships->first();
	IsideIGC* mySide = m_pShip->GetSide();
	Vector myPosition = m_pShip->GetPosition();
	float  myRadius = m_pShip->GetRadius();

	IshipIGC * pEnemy = NULL;
	float flClosest = 0.0f;
	
	// Traverse the ship list
	while (sLink != NULL)
	{
		IshipIGC* pship = sLink->data();
		
		// Put in side visibility stuff here
		if (pship->GetSide() != mySide)
		{		
			Announce(pship);		// Announce everyone.

			IweaponIGC* w = (IweaponIGC*)pship->GetMountedPart(ET_Weapon, 0);      // Would we really know this?  Is this unfair?

			if (w)					// If we are in weapons range, then RUN.
			{
				IprojectileTypeIGC* pProjectile = w->GetProjectileType();
				float fRange = pProjectile->GetSpeed() * w->GetLifespan();

				float distFromShots = (pship->GetPosition() - myPosition).Length() - fRange - myRadius;
				if (distFromShots < flClosest)
				{
					flClosest = distFromShots;
					pEnemy = pship;
				}
			}
		}
		sLink = sLink->next();
	};

	// If we found an enemy that we are in it's weapon range, move in the opposite direction
	if (pEnemy) 
	{
        SetState(c_Running);
        DoGotoAction(m_pShip, 
                     now, 
                     NULL, 
                     (2* myPosition) - pEnemy->GetPosition(),     // opposite direction of enemy
                     Vector::GetZero(), 
                     0.0f,
                     dt,
                     m_pDrone->GetMoveSkill(),
                     false,     // don't run through alephs
					 false,		// don't orbit
					 false);	// don't dodge bullets (or we'll never get away!)
        Goal::Update(now, dt);
        return;
    }

    // If no enemies, then orbit and increment radius
    SetState(c_Orbitting);

    if (DoGotoAction(m_pShip, 
                        now, 
                        m_pTarget, 
                        (m_pTarget) ? m_pTarget->GetPosition() : Vector::GetZero(), 
                        Vector::GetZero(), 
                        radius, 
                        dt,
                        m_pDrone->GetMoveSkill(), 
                        false, 
                        true))
    {
        radius += flRadIncreasePerCycle;   // only increment if we are close to the desired radius.
		if (radius > c_flOuterLimit || radius < c_flInnerLimit)
			flRadIncreasePerCycle = -flRadIncreasePerCycle;
    }

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: MimicGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    Keep a local goal that is always based on the goal of the guy we are
 *    mimicing.  If he is attacking, or disappearing, then do that.  Otherwise,
 *    just follow the guy (which means try to fly in formation).
 */

void MimicGoal::Update(Time now, float dt)
{
    assert(!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));

    if (m_pdTarget->GetDisappear())
        m_pDrone->Disappear();

    Goal * toMimic = m_pdTarget->GetGoal();

    SetGoal(toMimic->GetType(), toMimic->GetTarget());

    if (m_pGoal && !m_pGoal->Done())
        m_pGoal->Update(now, dt);

    Goal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Class: FaceGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    We just want to turn to face our target.  Meant primarily for automated
 *    client frame rate testing.
 */

void FaceGoal::Update(Time now, float dt)
{
	assert(!OBJECT_IS_BAD(m_pShip) && !OBJECT_IS_BAD(m_pTarget));

	Vector path = m_pTarget->GetPosition() - m_pShip->GetPosition();

	ControlData controls;
	controls.Reset();
	
	turnToFace(path, dt, m_pShip, &controls, m_pDrone->GetMoveSkill());

    m_pShip->SetStateBits(now, weaponsMaskIGC | buttonsMaskIGC, 0); 
    m_pShip->SetControls(controls);

	Goal::Update(now, dt);
}

/*-------------------------------------------------------------------------
 * Function: TestGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    This never really got used, but it seemed cool.  Run through all of the
 *    accepted commands for this drone, and go until they're done.  The switch 
 *    statement was to set up appropriate situations for each command.
 * Notes:
 *    The problem is that some of the goals are never-ending.  Have to add a
 *    time-out for those.
 */

void TestGoal::Update(Time now, float dt) 
{
    while (!m_pShip->AcceptsCommandF(cidCurrent))
        cidCurrent++;

    if (!Done())
    {
        const CommandData& cmddata = m_pShip->GetMission()->GetCommand(cidCurrent);
        if (strlen(cmddata.szImage)) 
        {
            ImodelIGC * pTarget = NULL;

            switch(cidCurrent)  // Set up appropriate targets, etc.
            {
            case c_ctDestroy:
                // Make an enemy target to fire at
                break;
            }

            m_pDrone->Verbose("Now Testing %s", cmddata.szVerb);
            m_pDrone->SetGoal((CommandType) cidCurrent, pTarget);
        }
    }

    cidCurrent++;
    Goal::Update(now, dt);
}

void ConstructGoal::Update(Time now, float dt)
{
    //Do we have a good target?
    assert ((m_pTarget == NULL) || (m_pTarget->GetObjectType() == OT_asteroid));

    assert (m_pstationtype);
    AsteroidAbilityBitMask aabm = m_pstationtype->GetBuildAABM();

    IasteroidIGC*   pasteroid;
    CastTo(pasteroid, m_pTarget);
	if (OBJECT_IS_BAD(m_pTarget) || !pasteroid->HasCapability(aabm))
	{
        m_pTarget = (aabm != 0)
                    ? FindTarget(m_pShip,
                                 c_ttNeutral | c_ttAsteroid | c_ttNearest,
                                 NULL, NULL, NULL, NULL,
                                 aabm)
                    : NULL;

        if (m_pTarget)
        {
            m_pDrone->SendChat(droneInTransitSound, "Building %s at %s", m_pstationtype->GetName(), GetModelName(m_pTarget));
        }
        else
        {
            //Can't find anything: complain but do nothing else
            m_pDrone->SendChat(droneAintGonnaWorkSound, "No place to build %s", m_pstationtype->GetName());

            //Pop the goal stack for the drone
            m_pDrone->m_goal = NULL;
            m_pDrone->DetermineGoal();
            Drone*  pdrone = m_pDrone;

            delete this;

            return;
        }
    }

	if (m_pTarget->GetCluster() != m_pShip->GetCluster())
    {
        m_pDrone->SetGoal(new GotoSameSectorGoal(m_pDrone, m_pTarget));
        m_pDrone->GetGoal()->Update(now, dt);
        Goal::Update(now, dt);
        return;
    }

    m_doneF = DoGotoAction(m_pShip, now, m_pTarget,
                           m_pTarget->GetPosition(), Vector::GetZero(),
                           m_pTarget->GetRadius() + m_pShip->GetRadius(), dt, m_pDrone->GetMoveSkill());

    Goal::Update(now, dt);
}
