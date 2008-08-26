#include "pch.h"


/*-------------------------------------------------------------------------
 * Function: MiningDrone::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *    When the mining drone needs to figure out what to do, he should either
 *    mine or unload depending on how full he is already
 */

Goal* MiningDrone::GetNewDefaultGoal(void)
{ 
    if (fMiningTime*2 < fMiningTimeLimit) 
    {
        ImodelIGC*  pmodel = FindTarget(m_pShip,
                               c_ttNeutral | c_ttAsteroid | c_ttNearest | c_ttAnyCluster,
                               NULL, NULL, NULL, NULL,
                               c_aabmMineHe3);

        if (pmodel)
        {
            Verbose("I don't have much Helium 3, I'd better go mine");
            return new MineGoal(this, pmodel);
        }
    }

    //Either no place to mine or mostly full ... try to unload if there's anything to unload
    if (fMiningTime != 0.0f)
    {
        IstationIGC*    pstation = (IstationIGC*)GetUnloadStation();
        if (pstation)
        {
            Verbose("OK, I'm at least half full, so I'll go unload");
            return new UnloadGoal(this, pstation);
        }
    }

    return NULL;
};


/*-------------------------------------------------------------------------
 * Function: UnloadGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    If we don't have a target station, then find one.  Otherwise just goto
 *    it.  We could make this use the goto function, probably.
 */

void UnloadGoal::Update(Time now, float dt)
{
    assert (m_pTarget);
    NewGotoGoal::Update(now, dt);
}


/*-------------------------------------------------------------------------
 * Function: MineGoal::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *    If we don't have a target asteroid, then find one.  Given a target, we
 *    want to move up right next to it, and start rotating (for lack of a better
 *    effect).  Also have to increment the MiningTime value
 *
 * Notes:
 *	  Right now, when the miners are actually at their target and mining, then they
 *	  are not dodging incoming projectiles.  Rob has expressed interest in something
 *    like this:   If the miner is getting injured (maybe save the current hitpoints
 *    when we start mining and compare to that??), then he should stop mining, send a
 *    chat message saying "I'm under attack, please help", and then just start orbitting
 *    the asteroid with a very small radius for defense.
 */

void MineGoal::Update(Time now, float dt) 
{
    assert (m_pTarget);
	if (m_pTarget->GetCluster() == m_pShip->GetCluster())
    {
        // Are we close enough to our target to mine it ... distance < 50.0f
        Vector  dp = m_pTarget->GetPosition() - m_pShip->GetPosition();
        float   distance2 = dp.LengthSquared();
        float   radius = m_pTarget->GetRadius() + m_pShip->GetRadius() + 60.0f;
        if (distance2 < radius * radius)
        {
            // We are close enough to mine ... do it.

            // Keep facing the target, and add a little roll for effect
            ControlData    controls;
            float   deltaAngle = turnToFace(dp, dt, m_pShip, &controls, m_pDrone->GetMoveSkill());

            controls.jsValues[c_axisRoll] = 1.0f;
            controls.jsValues[c_axisThrottle] = -1.0f;

		    // Zero the motion & weapon bits
            m_pShip->SetStateBits(now, weaponsMaskIGC | buttonsMaskIGC, miningMaskIGC); 
            m_pShip->SetControls(controls);

		    // Increment the amount of time that we have been mining appropriately
            MiningDrone* pDrone;
            CastTo(pDrone, m_pDrone);

            assert (m_pTarget->GetObjectType() == OT_asteroid);
            pDrone->fMiningTime += ((IasteroidIGC*)((ImodelIGC*)m_pTarget))->MineOre(dt * m_pShip->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaMiningRate));

            return;
        }
    }

    NewGotoGoal::Update(now, dt);
}



