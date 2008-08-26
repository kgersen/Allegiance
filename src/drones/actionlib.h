#ifndef ACTION_LIB_H_
#define ACTION_LIB_H_


/*-------------------------------------------------------------------------
 * Function: StopEverything
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Clear the ship controls
 */

extern bool StopEverything(IshipIGC* pShip, Time lastUpdate);


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
extern bool StrafeAttackTarget(IshipIGC*       pShip,
                                ImodelIGC*      pTarget,
                                Time            lastUpdate,
                                float           dt,
                                float           shootSkill,
                                float           moveSkill,
                                float           howClose);
*/

/*-------------------------------------------------------------------------
 * Function: StationaryAttackTarget
 *-------------------------------------------------------------------------
 * Purpose:
 *	  Don't thrust at all, just rotate your target into position and fire away
 */

extern bool StationaryAttackTarget(IshipIGC*		pShip,
								   Time				lastUpdate,
								   ImodelIGC*		pTarget,
								   float			dt,
                                   float            shootSkill,
                                   float            moveSkill,
								   bool				fCareful);


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

extern bool DoGotoAction(IshipIGC*      pShip,
                         Time           lastUpdate,
						 ImodelIGC*		pTarget,
                         Vector         position,
                         Vector         velocity,
                         float          radius,
                         float          dt,
                         float          skill,
                         bool           bThroughAlephs = false,
                         bool           orbit = false,
						 bool			dodgeBullets = true);

#endif