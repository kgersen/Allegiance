/*
**
**  File:	DoDamageAdjustableAction.cpp
**
**  Author:  Adam Davison
**
**  Description:
**      Implementation of the training library "DoDamageAdjustableAction" interface.
**
**  History:
*/
#include    "pch.h"
#include    "DoDamageAdjustableAction.h"

namespace Training
{
	/* void */  DoDamageAdjustableAction::DoDamageAdjustableAction (ShipID ship, float amount) :
    m_shipID (ship),
	m_amount (amount)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  DoDamageAdjustableAction::~DoDamageAdjustableAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        DoDamageAdjustableAction::Execute (void)
    {
		IshipIGC*	pShip = trekClient.GetCore()->GetShip(m_shipID);

		pShip->ReceiveDamage (c_dmgidCollision, m_amount, Time::Now (), Vector (-1.0f, 0.0f, 0.0f), Vector (1.0f, 0.0f, 0.0f), 0);

		pShip->SetStateBits(weaponsMaskIGC | buttonsMaskIGC, allWeaponsIGC);
		ControlData controls;
		controls.Reset();
		pShip->SetControls(controls);
    }

    //------------------------------------------------------------------------------
}
