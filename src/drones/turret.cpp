#include "pch.h"


/*-------------------------------------------------------------------------
 * Function: TurretDrone::SetGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Turrets accept the same commands as other drones, but those commands have
 *    different meanings.
 */

void TurretDrone::SetGoal(CommandType cm, ImodelIGC* pTarget, bool reply) 
{
    BackupGoal();		// still need to backup the current goal, just like Drone::SetGoal
    
    switch(cm)			// depends on the type of command
	{
    default:			// anything we don't understand can complete to the basic turret attack
    case c_ctDestroy:	
        {
            m_goal = new TurretAttackGoal(this, pTarget);
			if (reply)
			{
				if (OBJECT_IS_BAD(pTarget))
					SendChat(NA, "I'll shoot at the closest enemy");
				else
					SendChat(NA, "Giving priority to %s", GetModelName(pTarget));
			}
        }
        break;
    case c_ctGoto:		// goto this target, making sure that I am in the moving state
        {
		    if (pTarget)
            {
                PrepareToMove();
	            m_goal = new GotoGoal(this, pTarget);
				if (reply)
	                SendChat(NA, "Going to %s", GetModelName(pTarget));
            }
        }
        break;
    case c_ctDefend:	// Do turret attack regardless, and relocate first (if necessary)
        {
            m_goal = new TurretAttackGoal(this, NULL);

			if (pTarget)
            {
				if (pTarget->GetAttributes() & c_mtStatic != 0)		// don't want to have to move to moving targets... too vulnerable
				{
					float radius = pTarget->GetRadius() * 2;	// we want to go to somewhere on a radius twice as big as the object's radius

					if ((pTarget->GetPosition() - m_pShip->GetPosition()).LengthSquared() > radius * radius)
					{
						PrepareToMove();						// have to pack up and move, first
						Drone::SetGoal(new GotoGoal(this, pTarget, radius));
						if (reply)
							SendChat(NA, "Defending %s", GetModelName(pTarget));
					}
				}
				else if (reply)
					SendChat(NA, "I can't really defend a moving target, I'll stay here and shoot at enemies");
            }
			else if (reply)
				SendChat(NA, "Just defending myself");
        }
        break;
    case c_ctStop:												// Get ready to dodge, and most importantly STOP shooting
        {
            PrepareToMove();
            m_goal = new StayPutGoal(this);
            if (reply)
				SendChat(NA, "Just dodging for a while");
        }
        break;
    case c_ctFollow:											// Move towards my target until told to stop
        {
		    if (pTarget)
            {
                PrepareToMove();
	            m_goal = new FollowGoal(this, pTarget);
                if (reply)
					SendChat(NA, "Following %s", GetModelName(pTarget));
            }
        }
        break;
	case c_ctFace:
		{
			if (pTarget)
			{
				m_goal = new FaceGoal(this, pTarget);
				if (reply)
					SendChat(NA, "I'll sit here and face %s", GetModelName(pTarget));
			}
		}
		break;
    }
}


/*-------------------------------------------------------------------------
 * Function: TurretDrone::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *	  The default command for the turrets is to deploy and do the turret attack,
 *    which means start shooting at enemies.
 */

Goal* TurretDrone::GetNewDefaultGoal(void)
{ 
    Verbose("Doing the turrent attack");
    return new TurretAttackGoal(this, NULL); 
}


/*-------------------------------------------------------------------------
 * Function: TurretDrone::PrepareToMove
 *-------------------------------------------------------------------------
 * Purpose:
 *	  The turrets have two basic states...  When they are moving, they can't
 *    have shields, and they can't fire.
 */

void TurretDrone::PrepareToMove(void) 
{
    if (moving)							// We want to be able to call this as much as we
        return;							// need to without screwing up shield load time, etc.
    
    Verbose("Preparing to Move");

    //No shields allowed when moving: find and unmount our shield
    IpartIGC* shield = m_pShip->GetMountedPart(ET_Shield, 0);
    if (shield != NULL)
    {
        shield->SetMountID(Time::Now(), NA);
    }

// No more animations??
/*	
    if (m_pShip->GetAnimation() != c_animstateIdle)
        m_pShip->SetAnimation(-c_animstateIdle2Special,
                              c_animstateIdle);
*/

    moving = TRUE;
}


/*-------------------------------------------------------------------------
 * Function: TurretDrone::PrepareToFire
 *-------------------------------------------------------------------------
 * Purpose:
 *    This function gets the ship deployed.  If the ship was moving, then we have
 *    to reinable our guns and our shields.
 */

void TurretDrone::PrepareToFire(void) 
{
    if (!moving)						// Call this anytime, and it only runs if we have to make the transition
        return;

    Verbose("Preparing to Fire");

	// Force a delay of 15 seconds before shooting.
    IweaponIGC* w = (IweaponIGC*)(m_pShip->GetMountedPart(ET_Weapon, 0));
    /*
    if (w)
        w->SetNextFire(Time::Now() + 15.0f);    
    */

    // Mount our shield
    IshieldIGC* shield = (IshieldIGC*)(m_pShip->GetMountedPart(ET_Shield, 0));
    if (shield == NULL)
	{
        for (PartLinkIGC*   l = m_pShip->GetParts()->first(); (l != NULL); l = l->next())
        {
            IpartIGC*       p = l->data();
            IpartTypeIGC*   pt = p->GetPartType();

            if (pt->GetEquipmentType() == ET_Shield)
            {
                HRESULT hr = p->SetMountID(Time::Now(), 0);
                if (SUCCEEDED(hr))
                    break;
            }
        }
    }

// No more animations??
/*
    if (m_pShip->GetAnimation() != c_animstateSpecial)
        m_pShip->SetAnimation(c_animstateIdle2Special,
                             c_animstateSpecial);
*/
    moving = FALSE;
}


/*-------------------------------------------------------------------------
 * Function: TurretAttackGoal::Update
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

void TurretAttackGoal::Update(Time now, float dt) 
{
    assert(m_pShip);
    
    if (OBJECT_IS_BAD(m_pTarget))	// Don't want to be trying for anything that is dead
        m_pTarget = NULL;

    TurretDrone* pDrone;
    CastTo(pDrone, m_pDrone);		// Need this so that we can call the PrepareTo* functions

    ImodelIGC* pTarget = NULL;		// This will be the target that we are going to fire on


    // Make sure that we have a weapon
	IweaponIGC* w = (IweaponIGC*)m_pShip->GetMountedPart(ET_Weapon, 0);
    if (w)
    {
        IprojectileTypeIGC* pProjectile = w->GetProjectileType();				// Figure out the range of our weapon, that is the 
        float dTarget = pProjectile->GetSpeed() * w->GetLifespan();	// maximum distance that we want to worry about

        // If we were given a target, and that target is in range, then fire on it.
        if (m_pTarget && ((m_pTarget->GetPosition() - m_pShip->GetPosition()).LengthSquared() < dTarget*dTarget) && m_pShip->CanSee(m_pTarget))
		{
            pTarget = m_pTarget;
		}
        /*
		else if (m_pTarget && (m_pTarget->GetAttributes() & c_mtStatic != 0)) // If that target is not in range, then move to it only if it is a static object
		{
			pDrone->PrepareToMove();
			m_pDrone->Verbose("Redeploying to attack %s", GetModelName(m_pTarget));
			m_pDrone->SetGoal(new GotoGoal(m_pDrone, m_pTarget, dTarget - m_pShip->GetRadius()));
			m_pDrone->GetGoal()->Update(now, dt);
			Goal::Update(now, dt);
			return;
		}
        */
        else 
        {
			// Start search of objects in the cluster to find the most appropriate targets
            IclusterIGC* cluster = m_pShip->GetCluster();
            if (cluster)				// Make sure that we have a cluster
            {
                const ModelListIGC*   models = cluster->GetModels();
                assert (models);

				// Traverse the entire model list:
                for (ModelLinkIGC* mLink = models->first(); mLink != NULL; mLink = mLink->next())
                {
                    ImodelIGC*  m = mLink->data();
                    ObjectType type = m->GetObjectType();

                    if ((m->GetSide() != m_pShip->GetSide()) && ((type == OT_ship) || (type == OT_station) || (type == OT_missile)) && m_pShip->CanSee(m))
                    {
                        // then it's something to shoot
                        Vector  dp = m->GetPosition() - m_pShip->GetPosition();
                        float   distanceTarget = dp.Length();
        
                        //Add in a fudge factor to reflect the angle off bow:
                        //Something dead infront is a better pTarget that something
                        //180 degrees off the bow
                        float cosOffFront = m_pShip->GetOrientation().CosForward2(dp);
                        float d = distanceTarget - 150.0f * cosOffFront;

                        if (d < dTarget)	// is this the best target so far?
                        {
                            pTarget = m;
                            dTarget = d;
                        }
                    }
                }
            }
        }
    }
    
	if (m_pLastTarget != pTarget)				// Update the last target for our book-keeping
	{
        m_pLastTarget = pTarget;				
        if (pTarget)
            m_pDrone->Verbose("Attacking %s", GetModelName(pTarget));
        else
            m_pDrone->Verbose("No more enemy ships in range");
    }

    pDrone->PrepareToFire();					// Make darn sure that we are in firing mode before we pull the trigger

    if (pTarget) 
	{
		// Turn and shoot this target
        StationaryAttackTarget(m_pShip, now, pTarget, dt, m_pDrone->GetShootSkill(), m_pDrone->GetMoveSkill(), true);
    }
    else 
	{
		// Make sure that we clear our controls
        StopEverything(m_pShip, now);	
	}

    Goal::Update(now, dt);
}