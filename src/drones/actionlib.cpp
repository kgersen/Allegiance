#include "pch.h"


// Utility functions:


/*-------------------------------------------------------------------------
 * Function: getDirection
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Get the button control mask to thrust in a given direction
 */

int  getDirection(const Vector&      dP, 
                  const Orientation& orientation)
{
    float   z = dP * orientation.GetForward();
    float   y = dP * orientation.GetUp();
    float   x = dP * orientation.GetRight();

    double  absX = fabs(x);
    double  absY = fabs(y);
    double  absZ = fabs(z);

    return (absX > absY)
           ? ((absX > absZ)
              ? (x >= 0.0f ? rightButtonIGC : leftButtonIGC)
              : (z >= 0.0f ? forwardButtonIGC : backwardButtonIGC))
           : ((absY > absZ)
              ? (y >= 0.0f ? upButtonIGC : downButtonIGC)
              : (z >= 0.0f ? forwardButtonIGC : backwardButtonIGC));
}


/*-------------------------------------------------------------------------
 * Enumeration: ShotStatus
 *-------------------------------------------------------------------------
 * Purpose:
 *	  How clean of a shot do we have??  
 * Notes:
 *    The values are ordered in order of importantance
 */

enum ShotStatus 
{               
    c_ssCleanShot       = 0,
    c_ssNeutralBehind   = 1,
    c_ssFriendlyBehind  = 2,
    c_ssBlocked         = 3
};


/*-------------------------------------------------------------------------
 * Function: checkModels
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Check the model list to find anything affecting the status of the shot
 *    that we would like to fire
 */

ShotStatus checkModels(IshipIGC* pShip, ImodelIGC* pTarget, const ModelListIGC* models, const Vector& direction, const float dist2, const float range2) 
{
    assert(models);
    ShotStatus ss = c_ssCleanShot;
    IsideIGC* mySide = pShip->GetSide();

	// Traverse the object list    
	for (ModelLinkIGC* mLink = models->first(); mLink != NULL; mLink = mLink->next()) 
	{
        ImodelIGC*  m = mLink->data();
        IsideIGC* side = m->GetSide();

        if ((m != pTarget) && (m != pShip) && ((side == NULL) || (side == mySide)))		// Then it's something that I don't want to hit
        {
            // Is it in my line of fire?
            Vector dp        = m->GetPosition() - pShip->GetPosition();
            float dot        = dp * direction;
            float dp2        = dp.LengthSquared();

            // Is it in front of me and in my weapons range??
            if (dot >= 0.0f && dp2 < range2)
            {
                assert((direction.LengthSquared() < 1.05f) && (direction.LengthSquared() > 0.95));

                Vector  closest = pShip->GetPosition() + direction * dot;
                Vector  offset = closest - m->GetPosition();
                float   offsetLength2 = offset.LengthSquared();
                float   r = m->GetRadius();
                if (offsetLength2 < (r * r))
                {
                    if (dp2 > dist2)
                        ss = (side == NULL) ? c_ssNeutralBehind : c_ssFriendlyBehind;
                    else {
                        return c_ssBlocked;
                    }
                }
            }
        }
    }

    return ss;
}


/*-------------------------------------------------------------------------
 * Function: getShotStatus
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Send the appropriate model lists into checkmodels to get the status of the
 *    shot that we would like to fire
 */

ShotStatus getShotStatus(IshipIGC* pShip, ImodelIGC* pTarget, const Vector& direction, float range)
{
    Vector vShipToTarget = pTarget->GetPosition() - pShip->GetPosition();
    float dist2 = vShipToTarget.LengthSquared();
    float flRange2 = range * range;
    ShotStatus sStatus = c_ssCleanShot;

    IclusterIGC* cluster = pTarget->GetCluster();
    if (cluster)									
    {
		// Try to check in order of likeliness (and size of the list)
		
		// Check the planets
        ShotStatus ss = checkModels(pShip, pTarget, reinterpret_cast<const ModelListIGC*>(cluster->GetAsteroids()), direction, dist2, flRange2);
        if (ss == c_ssBlocked)  return c_ssBlocked;
        if (ss > sStatus)  sStatus = ss;

		// Check the stations
        ss = checkModels(pShip, pTarget, reinterpret_cast<const ModelListIGC*>(cluster->GetStations()), direction, dist2, flRange2);
        if (ss == c_ssBlocked)  return c_ssBlocked;
        if (ss > sStatus)  sStatus = ss;

		// Check the warps
        ss = checkModels(pShip, pTarget, reinterpret_cast<const ModelListIGC*>(cluster->GetWarps()), direction, dist2, flRange2);
        if (ss == c_ssBlocked)  return c_ssBlocked;
        if (ss > sStatus)  sStatus = ss;

		// Check the ships
        ss = checkModels(pShip, pTarget, reinterpret_cast<const ModelListIGC*>(cluster->GetShips()), direction, dist2, flRange2);
        if (ss > sStatus)  sStatus = ss;
    }
    return sStatus;
}







// External functions:



/*-------------------------------------------------------------------------
 * Function: StopEverything
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Clear the ship controls
 */

bool StopEverything(IshipIGC* pShip, Time lastUpdate) 
{
    assert(pShip);

    pShip->SetStateBits(lastUpdate, weaponsMaskIGC | buttonsMaskIGC, 0); // stop all firing

    ControlData controls;
    controls.Reset();

    pShip->SetControls(controls);       // zero the motion bits
    return true;
}


/*-------------------------------------------------------------------------
 * Function: StrafeAttackTarget
 *-------------------------------------------------------------------------
 * Purpose:
 *    To fly straight at the target, guns blazing, and then pull up at the 
 *    last second.
 * Notes:
 *	  This never quite panned out. You can look at bug 1191 for information.  
 *    Basically, using the goto function and trying to create a nice path 
 *    for the drones to come in and then pull up was next to impossible.  
 *    They would either get too much momentum towards their target, and then 
 *    not be able to pull up (especially for stations).  First, I was hitting 
 *    this in the DoGotoAction:   
 *		if ((offsetLength2 < (r * r)) && (offsetLength2 > 0.1f))  
 *
 *    In otherwords, since we couldn't divide by zero, if we were perfectly 
 *    on path with an obstacle, we would just hit it.  That meant that I could 
 *    not just GOTO our target and let the dodge code do it's thing.  So I 
 *    played with making a nice arc in at the target, and then up.  Once again, 
 *    I either got a lot of good shots in, and then couldn't pull up, or I 
 *    would only get one or two shots in if I was pulling up in enough time.  
 *    I think doing it right would mean NOT using the DoGotoAction function, 
 *    and that was more than I felt I should do in the last week.  Too dangerous.
 */

/*
bool StrafeAttackTarget(IshipIGC*       pShip,
                        ImodelIGC*      pTarget,
                        Time            lastUpdate,
                        float           dt,
                        float           shootSkill,
                        float           moveSkill,
                        float           howClose)
{
    int buttonsM = 0;

    Vector direction = pTarget->GetPosition() - pShip->GetPosition();

    IweaponIGC* w = (IweaponIGC*)pShip->GetMountedPart(ET_Weapon, 0);
    if (w && !OBJECT_IS_BAD(pTarget)) 
	{
        IprojectileTypeIGC* pt			= w->GetProjectileType();
        float				lifespan	= pt->GetLifespan();
        Vector				direction;
		Vector				myPosition	= pShip->GetPosition();
        assert (pt);

        float   tToHit = solveForLead(pShip, pTarget, w, &direction, shootSkill);

		
		Vector destination = (tToHit < lifespan) 
								? (direction + pTarget->GetPosition()) 
								: pTarget->GetPosition();

		destination += (pShip->GetOrientation().GetUp() * pShip->GetRadius() * 5.0f) / ((destination - myPosition).Length());
		
//		destination += (CrossProduct(pShip->GetOrientation().GetRight(), destination).Normalize() * pShip->GetRadius() * 5.0f) / ((destination - myPosition).Length());

		Vector destVel = (destination - myPosition).Normalize() * pShip->GetEffectiveTopSpeed();

        // go straight for the ship, let the dodge code take care of any problems.
        DoGotoAction(pShip, 
                     lastUpdate, 
                     NULL, 
                     destination,
//                   destVel,
					 Vector::GetZero(),
                     0.0f, 
                     dt, 
                     moveSkill, false, false, false);

        ControlData controls;
        controls.Reset();

        //just need the angle...  todo: replace this with only the necessary calcs, then delete all controls in this func.
        float   deltaAngle = turnToFace((tToHit < lifespan) ? direction : (pTarget->GetPosition() - pShip->GetPosition()),
                                            dt, pShip, &controls, moveSkill);

        //shoot when appropriate
        if ((tToHit <= lifespan) && (deltaAngle < pi / 48.0f))
            buttonsM = allWeaponsIGC;

        if (buttonsM) {
            // if I am about to hit anything I don't want to hit,
            // then return false
        
            ShotStatus status = getShotStatus(pShip, pTarget, (tToHit < lifespan) ? direction : (pTarget->GetPosition() - pShip->GetPosition()), lifespan * pt->GetSpeed());

            if ((status == c_ssBlocked) ||
                (status == c_ssFriendlyBehind && (tToHit > shootSkill + 0.1)))       // there is something behind him, do I risk it?
            {
                // not going to try it
                buttonsM = 0;
            }
        }
    }
    pShip->SetStateBits(lastUpdate, weaponsMaskIGC | buttonsMaskIGC, buttonsM); 
    // ship controls will be set in the DoGotoAction function
    return (buttonsM != 0) ? true : false;
}
*/


/*-------------------------------------------------------------------------
 * Function: StationaryAttackTarget
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Don't thrust at all, just rotate your target into position and fire away
 */

bool StationaryAttackTarget(IshipIGC*       pShip,
                            Time            lastUpdate,
                            ImodelIGC*      pTarget,
                            float           dt,
                            float           shootSkill,
                            float           moveSkill,
                            bool            fCareful)        // fCareful: make sure you don't hit friendlies
{
    ControlData controls;
    controls.Reset();

    int buttonsM = 0;

    assert(!OBJECT_IS_BAD(pShip));

	// Make sure I have a weapon
    IweaponIGC* w = (IweaponIGC*)pShip->GetMountedPart(ET_Weapon, 0);

    if (w && !OBJECT_IS_BAD(pTarget)) 
	{
        IprojectileTypeIGC* pt = w->GetProjectileType();
        assert (pt);
        float   lifespan = pt->GetLifespan();

        Vector  direction;
        float   tToHit = solveForLead(pShip, pTarget, w, &direction, shootSkill);

        // Pivot to shoot the enemy
        float   deltaAngle = turnToFace((tToHit < lifespan) ? direction : (pTarget->GetPosition() - pShip->GetPosition()),
                                            dt, pShip, &controls, moveSkill);

        // Shoot when appropriate
        if ((tToHit <= lifespan) && (deltaAngle < pi / 32.0f))
            buttonsM = allWeaponsIGC;

        if (fCareful && buttonsM) {
            // If I am about to hit anything I don't want to hit, then don't fire
            
            ShotStatus status = getShotStatus(pShip, pTarget, direction, lifespan * pt->GetSpeed());

            if ((status == c_ssBlocked) || 
                (status == c_ssFriendlyBehind && (tToHit > shootSkill + 0.1)))       // There is something behind him, do I risk it?
            {
                // Not going to try it
                buttonsM = 0;
            }
        }
    }

    pShip->SetStateBits(lastUpdate, weaponsMaskIGC | buttonsMaskIGC, buttonsM); 
    pShip->SetControls(controls);
    return (buttonsM != 0) ? true : false;
}
                

/*-------------------------------------------------------------------------
 * Function: DoGotoAction
 *-------------------------------------------------------------------------
 * Purpose:
 *	  This is the most used function in all of the drone code.  It takes the
 *    ship, the target position, and a slew of other parameters, and does three
 *    things:
 *    1) Dodges immediate impacts
 *    2) Evaluates a path around huge static obstacles
 *    3) Pulls into the desired radius of the target position, or orbits 
 */

bool DoGotoAction(IshipIGC*      pShip,
                  Time           lastUpdate,
                  ImodelIGC*     pTarget,
                  Vector         position,
                  Vector         velocity,
                  float          radius,
                  float          dt,
                  float          skill,
                  bool           bThroughWarps,
                  bool           orbit,
				  bool			 dodgeBullets)
{
    if (OBJECT_IS_BAD(pShip))
        return false;

    if (pTarget && pTarget->GetCluster() != pShip->GetCluster())	// Make sure that we are in the right cluster
    {
        if (!bThroughWarps)
        {
           StopEverything(pShip, lastUpdate); 
           return false;
        }

        IwarpIGC* gotoWarp = FindPath(pShip, pTarget);
        pTarget = (ImodelIGC*) gotoWarp;
        position = gotoWarp->GetPosition();
        velocity = Vector::GetZero();
        radius = 0.0f;
    }

    const Vector&       myPosition = pShip->GetPosition();
    const Vector&       myVelocity = pShip->GetVelocity();
    const Orientation&  myOrientation = pShip->GetOrientation();
    float               myRadius = pShip->GetRadius();

    bool fCheckTargetForCollision;
    if (pTarget)
    {
        //The only time we do not check for a collision vs. the target is when it is a warp we want
        //to go through
        switch (pTarget->GetObjectType())
        {
            case OT_warp:
            {
                fCheckTargetForCollision = !bThroughWarps;
            }
            break;
            case OT_station:
            {
                fCheckTargetForCollision = false;
            }
            break;
            default:
                fCheckTargetForCollision = true;
        }

        //However far we want to be from the target, make it the distance from our bounding sphere
        radius += myRadius;
    }
    else
    {
        //Sorta weird: no target implies we want to get to a position exactly, so worry about "hitting" the point
        fCheckTargetForCollision = true;
    }

    Vector              path = position - myPosition;
    float               distance2 = path.LengthSquared();

    float               maxSpeed = pShip->GetHullType()->GetMaxSpeed();
    float               acceleration = pShip->GetHullType()->GetThrust() / pShip->GetMass();

    float               speed = myVelocity.Length();
    float               timeToDodge = (speed / (acceleration * pShip->GetHullType()->GetBackMultiplier()));		//Worst case time to stop
    float               distOffOrbit = 0.0f;

    //This behavior as 3 parts: avoid hitting stuff, get close to my goal,
    //and approach my goal without hitting it.
    bool    fDiverted = false;

    //See if we are about to hit something (anything) or if there is something blocking out path to the goal
    ImodelIGC*  modelCollide = NULL;
    float   tCollide = (timeToDodge > 5.0f) ? timeToDodge : 5.0f;
    Vector  directionCollide;

    {
        IclusterIGC*    cluster = pShip->GetCluster();
        assert (cluster);

        const ModelListIGC*   models = cluster->GetModels();
        assert (models);

            // There are cases that we DO want to check the target for collisions, like when out destination is in
            // respect to the target, but not the center of the target, and when we are going to a warp but not through the warp

        for (ModelLinkIGC* l = models->first(); (l != NULL); l = l->next())
        {
            ImodelIGC* m = l->data();
            ObjectType type = m->GetObjectType();

            if (m != pShip && (m != pTarget || fCheckTargetForCollision))
            {
                // Also make sure that we never dodge our own projectiles:
                if (type == OT_projectile && 
					( (((IprojectileIGC*)m)->GetLauncher() == pShip) || !dodgeBullets ))
                        continue;
                if (type == OT_missile && ((ImissileIGC*)m)->GetLauncher() == pShip)
                        continue;

                float   r = m->GetRadius() + myRadius + 20.0f;
                Vector  dp = m->GetPosition() - myPosition;
                float   d2 = dp.LengthSquared();

                float   t = (d2 > r * r)
                            ? solveForImpact(dp,
                                             m->GetVelocity()*skill - myVelocity,
                                             0.0f,
                                             r,
                                             &directionCollide)
                            : 0.0f;


                assert (t >= 0.0f);

                // Add some adjustments for skill level
                float adj = ((type == OT_projectile) || (type == OT_missile)) ? skill : 1.0f;

                if (t < tCollide*adj)
                {
                    modelCollide = m;
                    tCollide = t;
                }

                if ((type == OT_asteroid) || (type == OT_station) || (type == OT_ship) || (type == OT_warp))
                {
                    if (orbit)
                    {		// Do we really want to do avoid this thing????
							// Don't do this unless we are so far away from our target that we are basically moving right towards it.

                    }
                    else
                    {
                        // A big non-moving thing ... is it in the way?
                        float   dot = dp * path;
                        if ((dot >= 0.0f) && (d2 < distance2)) 
                        {
                            // Well ... it's not behind us (looking the way we want to go), and it is closer than where we'd like to go.
                            Vector  closest = myPosition + path * (dot / distance2);
                            Vector  offset = closest - m->GetPosition();
                            float   offsetLength2 = offset.LengthSquared();
                            if ((offsetLength2 < (r * r)) && (offsetLength2 > 0.1f))
                            {
                                position = m->GetPosition() + offset * (r / (float)sqrt(offsetLength2));
                                velocity = Vector::GetZero();
                                path = position - myPosition;
                                distance2 = path.LengthSquared();
                                radius = 0.0f;								//We want to go to this point exactly.
                                orbit = false;

                                fDiverted = true;
                            }
                        }
                    }
                }
            }
        }
    }

    bool fAreWeThereYet = false;
    ControlData controls;
    controls.jsValues[c_axisYaw] = controls.jsValues[c_axisPitch] = controls.jsValues[c_axisRoll] = 0.0f;
    int stateM = 0;

    if (modelCollide)
    {
        // We are going to collide: find our relative positions at the time of collision
        // and generate as much of an away vector as possible
        const Vector&   hisVelocity = modelCollide->GetVelocity();
        Vector  directionAway = myPosition - modelCollide->GetPosition() +
                                tCollide * (myVelocity - hisVelocity);

        // Dodging along his velocity vector doesn't do much good, so ...
        float   lhv2 = hisVelocity.LengthSquared();
        if (lhv2 > 1.0f)    // but, if he's moving slow, it doesn't matter
        {
            float   lda2 = directionAway.LengthSquared();
            assert (lda2 != 0.0f);  // should be, at least, the sum of the radii

            // Remove the component of his velocity from directionAway
            float   dp = hisVelocity * directionAway;
            if (dp * dp > 0.99f * lhv2 * lda2)
            {
                // 99% (or more) of the away vector is parallel to his velocity vector ...
                // pick an arbitrary axis that is not parallel with his velocity
                Vector  arbitrary(1.0f, 0.0f, 0.0f);
                float   dot = arbitrary * hisVelocity;

                if (dot * dot > 0.99f * lhv2)
                {
                    // Try the y axis as an arbitrary axis
                    arbitrary.x = 0.0f;
                    arbitrary.y = 1.0f;
                }

                // The direction in which to dodge is perpendicular to both his velocity and the arbitrary vector
                directionAway = CrossProduct(arbitrary, hisVelocity);
            }
            else
            {
                // The direction vector has enough of a perpendicular component to the
                // velocity vector to make this worthwhile
                // Subtract out the component of directionAway that is parallel to hisVelocity
                directionAway -= hisVelocity * (dp / lhv2);
            }
        }

        // Thrust along the away vector
        controls.jsValues[c_axisThrottle] = 1.0f;
        stateM = getDirection(directionAway, myOrientation);
    }
    else
    {
        Vector desiredVelocity;
        if (orbit) 
        {
            assert(path.LengthSquared() > 0.0f);

			// Make sure that our radius is acceptable
			if (pTarget && radius < pTarget->GetRadius() + pShip->GetRadius()) 
                radius = pTarget->GetRadius() + pShip->GetRadius();

			// Get the tangent vector
            desiredVelocity = CrossProduct(path, pShip->GetVelocity());
            if (desiredVelocity.LengthSquared() <= 0.01f)
            {
                desiredVelocity = CrossProduct(path, pShip->GetOrientation().GetForward());
                if (desiredVelocity.LengthSquared() <= 0.01f)
                    desiredVelocity = pShip->GetOrientation().GetRight();
                else
                    desiredVelocity = CrossProduct(desiredVelocity, path);
            }
            else
                desiredVelocity = CrossProduct(desiredVelocity, path);


            // Adjust the tangent vector to smoothly approach the orbit if we are not currently in the orbit
			float distance = (float)sqrt(distance2);
            distOffOrbit = distance - radius;
            float topOrbitSpeed = (distOffOrbit > radius)
                                  ? maxSpeed
                                  : ((distOffOrbit < 0) 
                                     ? maxSpeed/2
                                     : (maxSpeed * distance / (2 * radius)));
            float vel2= velocity.LengthSquared();

            desiredVelocity.SetNormalize();
            if (vel2<= 0.0f || vel2> topOrbitSpeed*topOrbitSpeed)
                desiredVelocity *= topOrbitSpeed;
            else
                desiredVelocity *= ((float)sqrt(vel2));

			// Spiral towards the perfect orbit
            Vector spiral = path.Normalize() * distOffOrbit;
            desiredVelocity += spiral;
        }
        else
        {
            // We do not have to worry about hitting anything real soon, so concentrate on following our target
            Vector  direction;
            float   t = solveForImpact(path,
                                       velocity,
                                       maxSpeed,
                                       radius,
                                       &direction);

            // t is the time required to get to the desired distance from our target.
            // If we are inside out manuevering envelope, match the targets velocity vector
            // otherwise, blend in a combination of the shortest path and the velocity vector
            desiredVelocity = velocity;
            if ((t <= timeToDodge) && fCheckTargetForCollision && (!fDiverted))
            {
                //the distance traveled is proportional to the square of the elapsed time,
                //so pick a velocity based on the square of the time ratio
                //float   f = t / timeToDodge;
                //desiredVelocity += direction * (speed * f * f);
            }
            else if (t <= 1.0e10f)
                desiredVelocity += direction * maxSpeed;
            else
            {
                // Were not able to find any impact solution,
                // so if we are too close (a reason not to find a solution, move apart)
                // if we are too far, move closer
                float   distance = (float)sqrt(distance2);
                if (distance < radius)
                    desiredVelocity -= path * (maxSpeed * (radius - distance) / (radius * distance));
                else if (distance > 2.0f * radius)
                    desiredVelocity += path * (maxSpeed / distance);
            }
        }

        float   ldv2 = desiredVelocity.LengthSquared();
        if (ldv2 > 1.0f)
        {
            float deltaAngle = turnToFace(desiredVelocity,
                                          dt, pShip, &controls, skill);

            if (deltaAngle < pi * 0.25f)
            {
                // We are facing along (or close to) our desired velocity vector, set the throttle
                // appropriately
                controls.jsValues[c_axisThrottle] = (ldv2 < maxSpeed * maxSpeed)
                                                    ? 2.0f * ((float)sqrt(ldv2) / maxSpeed - 0.5f)
                                                    : 1.0f;
            }
            else
            {
                // We are not facing along (or close to) our desired velocity vector,
                // use side thrust to get there
                controls.jsValues[c_axisThrottle] = 1.0f;   //use all of the throttle

                stateM = getDirection(desiredVelocity - myVelocity, myOrientation);
            }
        }
        else
        {
            controls.jsValues[c_axisThrottle] = -1.0f;
            fAreWeThereYet = !fDiverted;
        }
    }

    pShip->SetStateBits(lastUpdate, buttonsMaskIGC | weaponsMaskIGC, stateM);    // Force all weapon to non-shooting
    pShip->SetControls(controls);

    if (orbit)
        return (distOffOrbit > -5.0f && distOffOrbit < 5.0f); 
    
    return fAreWeThereYet;
}

