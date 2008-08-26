#include "pch.h"


/*-------------------------------------------------------------------------
 * Function: Drone::Drone
 *-------------------------------------------------------------------------
 * Purpose:
 *      Does nothing accept initialize all of the drone variables
 */

Drone::Drone(IshipIGC* id, DroneType dt) :
    m_pShip(id), 
    m_goal(NULL), 
    m_goalMemory(0,0), 
    bVerbose(false), 
    dType(dt), 
    m_fDie(false), 
    m_fDisappear(false),
    m_flShootSkill(0.0f), 
    m_flMoveSkill(0.0f), 
    m_flBravery(0.0f), 
    m_pStation(NULL), 
    m_index(g_drones.GetIndex())
{}


/*-------------------------------------------------------------------------
 * Function: Drone::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *      Update the ship controls....
 *    Start by checking the instincts, then check the current goal.  If that goal is
 *    not done yet, then run that goal.  Otherwise try to find a goal for next time
 *    using DetermineGoal()
 */

void Drone::Update(Time now, float dt) 
{
    if (m_pShip->GetStation()) // undock immediately
    {
        m_pShip->SetStation(NULL);
    }
    
    assert (m_pShip->GetCluster() != NULL);

    Instincts();                        // Check some things EVERY round

    if (m_goal && m_goal->Done())
    {
        m_goal->DoneEvent();        // Mostly for done messages
        delete m_goal;
        m_goal = NULL;
    }

    if (!m_goal)
    {
        DetermineGoal();
    }

    if (m_goal)
    {
        m_goal->Update(now, dt);    // The goal is not done, update the controls using that goal
    }
    else
    {
        //No goal ... well, do nothing
        m_pShip->SetStateM(now, 0);
        ControlData cd;
        cd.Reset();
        m_pShip->SetControls(cd);
    }

    if (m_fDie == true)                    // This has to be the last thing in the update cycle
    {
        //Self-destruct ... but first nuke parts, fuel & ammo
        {
            PartLinkIGC*    ppl;
            while (ppl = m_pShip->GetParts()->first())  //intentional assignement
            {
                ppl->data()->Terminate();
            }
        }
        m_pShip->SetAmmo(0);
        m_pShip->SetFuel(0.0f);
        m_pShip->GetMission()->GetIgcSite()->KillShipEvent(now, m_pShip, NULL, 0.0f, m_pShip->GetPosition(), Vector::GetZero());
    }
}


/*-------------------------------------------------------------------------
 * Function: Drone::SetShip
 *-------------------------------------------------------------------------
 * Purpose:
 *      If I have to update the ship, I also have to update the goals which save the
 *    ship seperately
 */

void Drone::SetShip(IshipIGC* ship)
{
    assert(ship);
    m_pShip = ship;

    // Kind of a pain, but I have to make sure that the goals are using the correct ship, too
    if (m_goal)
        m_goal->m_pShip = ship;
    for (int i=m_goalMemory.GetCount(); i>0; i--)
    {
        m_goalMemory[i-1]->m_pShip = ship;
    }
}


/*-------------------------------------------------------------------------
 * Function: Drone::Message
 *-------------------------------------------------------------------------
 * Purpose:
 *      Most commands end up using SetGoal(command), but there are a few exceptions
 *    to that rule.  Catch those exceptions here.
 */

bool 
Drone::Message(Time now, CommandID cm, ImodelIGC*  pmodelObject) 
{
    if (!m_pShip->AcceptsCommandF(cm))                                //  Make sure that this drone can do this command
    {
        SendChat(droneCantDoThatSound, "I can't do that, you must have the wrong drone");
        return false;
    }

    const CommandData& cmdata = m_pShip->GetMission()->GetCommand(cm);

    if (pmodelObject)                                                // Make sure that the target is appropriate for that command
    {
        if (!cmdata.LegalObject(pmodelObject->GetObjectType()))
        {
            SendChat(droneCantDoThatSound, "I can't do that to %s", GetModelName(pmodelObject));
            return false;
        }
        if (pmodelObject == m_pShip) {
            SendChat(droneCantDoThatSound, "I can't do that to myself...");
            return false;
        }
    }
    else if (cmdata.otmLegal != 0)
    {
        SendChat(droneCantDoThatSound, "I can't do that to nothing");
        return false;
    }

    CommandType ct = (CommandType) cm;
    
    if (ct == c_ctNoCommand)                                        // Cut out the junk commands
    {
        SendChat(droneCantDoThatSound, "I don't understand...");
        return false;
    }
    else if (ct == c_ctStatus)                                        // Get the status
        Status();
    else if (ct == c_ctHome)                                        // Set the home station
    {
        if (pmodelObject && pmodelObject->GetObjectType() == OT_station)
            SetStation(pmodelObject);
        else SendChat(droneCantDoThatSound, "That's not a station!");
    }
    else if (ct == c_ctVerbose)                                        // Toggle verbose
    {
        ToggleVerbose();
        if (bVerbose)
            Status();
    }
    else if (ct == c_ctSDestruct)                                    // Self Destruct
    {
        Die();
    }
    else if (ct == c_ctPop)                                            // Pop a goal
    {
        if (m_goal) 
        {
            delete m_goal;
            m_goal = NULL;
        }
        DetermineGoal();
    }
    else                                                            // It is a command associated with a goal.
    {                                                                // Clear the current goals and do this order
        ClearGoals();
        SetGoal(ct, pmodelObject, true);
    }
    return true;
}


/*-------------------------------------------------------------------------
 * Function: Drone::SendChat
 *-------------------------------------------------------------------------
 * Purpose:
 *      A wrapper on the GameSite::DroneSendChat function
 */

void 
Drone::SendChat(SoundID soundID, const char* format, ...) 
{
    char szChat[256]; // $CRC: sync this with client
    va_list vl;
    va_start(vl, format);
    int cbChat = wvsprintfA(szChat, format, vl);
    assert(cbChat < sizeof(szChat));
    va_end(vl);
    m_pShip->GetMission()->GetIgcSite()->DroneSendChat(m_pShip, szChat, CHAT_TEAM, m_pShip, soundID);
}


/*-------------------------------------------------------------------------
 * Function: Drone::Verbose
 *-------------------------------------------------------------------------
 * Purpose:
 *      A wrapper on the m_pShip->GetMission()->GetIgcSite()->DroneSendChat function, but only send the chat
 *    if verbose mode is on.
 */

void Drone::Verbose(const char* format, ...) 
{
    if (!bVerbose)
        return;

    char szChat[256]; // $CRC: sync this with client
    va_list vl;
    va_start(vl, format);
    int cbChat = wvsprintfA(szChat, format, vl);
    assert(cbChat < sizeof(szChat));
    va_end(vl);
    m_pShip->GetMission()->GetIgcSite()->DroneSendChat(m_pShip, szChat, CHAT_TEAM, m_pShip);
}


/*-------------------------------------------------------------------------
 * Function: Drone::Status
 *-------------------------------------------------------------------------
 * Purpose:
 *      Send status information over the chat.  Goal information and drone info, too
 */

void Drone::Status (void) 
{
    if (m_goal)
        m_goal->Status();
    else 
        SendChat(NA, "No goal information available");

    Verbose("I am in %s", m_pShip->GetCluster()->GetName());
    Verbose("Shooting skill: %d%%  Moving Skill %d%%  Bravery: %d%%", (int)(m_flShootSkill*100), (int)(m_flMoveSkill*100), (int)(m_flBravery*100));
}


/*-------------------------------------------------------------------------
 * Function: Drone::DetermineGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Given the current goal, the goal stack, and the default goal, come up with a goal
 *    for the drone
 */

void Drone::DetermineGoal(void) 
{
    if (m_goal)                            // Already have a goal
        return;
   
    while (m_goalMemory.GetCount())        // Have a goal in memory, do that
    {
        RestoreGoal();
        assert(m_goal);
        if (!m_goal->Done())
            return;
    }

    SetGoal(GetNewDefaultGoal());        // No goals anywhere, use the default.
}


/*-------------------------------------------------------------------------
 * Function: Drone::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      This is meant to re-declared in the child classes.  Default goal for a default
 *    drone is to sit their and be dodge.
 */

Goal* Drone::GetNewDefaultGoal(void)
{ 
    Verbose("I'll just stay here and wait for an order");
    return new IdleGoal(this); 
}


/*-------------------------------------------------------------------------
 * Function: Drone::SetGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Given the commandtype and target, backup the current goal and set m_goal 
 *    appropriately. Also send a confirmation message.
 */

void Drone::SetGoal(CommandType cm, ImodelIGC* pTarget, bool reply) 
{
    BackupGoal();

    assert(cm != c_ctNoCommand);
    
    switch(cm) 
    {
    case c_ctDestroy:
        {
            if (pTarget) {
                m_goal = new DestroyGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "I'm going to try to destroy %s", GetModelName(pTarget));
            }
            else {
                m_goal = new KillAnythingGoal(this);
                if (reply)
                    SendChat(NA, "I'm going to try to destroy any enemies that I see");
            }
        }
        break;
    case c_ctMine:
        {
            MiningDrone* pDrone = NULL;
            CastTo(pDrone, this);

            if ((pTarget->GetObjectType() == OT_asteroid) &&
                ((IasteroidIGC*)pTarget)->HasCapability(c_aabmMineHe3))
            {
                m_goal = new MineGoal(pDrone, pTarget);
                if (reply)
                {
                    SendChat(droneInTransitSound, "I'm going to mine %s", GetModelName(pTarget));
                }
            }
            else
            {
                SendChat(droneCantDoThatSound, "I can't do that");
            }
        }
        break;
    case c_ctUnload:
        {
            MiningDrone* pDrone = NULL;
            CastTo(pDrone, this);
            IstationIGC* pStation;
            CastTo(pStation, pTarget);

            if (pStation)
            {
                m_goal = new UnloadGoal(pDrone, pStation);

                if (reply)
                {
                    if (pStation)
                        SendChat(NA, "I'm going to unload at %s", GetModelName(pStation));
                    else
                        SendChat(NA, "I'm going to go unload");
                }
            }
        }
        break;
    case c_ctPatrol:
        {
            if (pTarget) {
                m_goal = new PatrolGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "I'm going to patrol between where I am now, and %s", GetModelName(pTarget));
            }
        }
        break;
    case c_ctGoto:
        {
            if (pTarget)
            {
                //enter starbases by default, goto everything else
                m_goal = new NewGotoGoal(this, pTarget, (pTarget->GetObjectType() == OT_station) ||
                                                        (pTarget->GetObjectType() == OT_warp)
                                                        ? Waypoint::c_oEnter
                                                        : Waypoint::c_oGoto);
                //m_goal = new GotoGoal(this, pTarget, true);
                if (reply)
                    SendChat(NA, "I'm going to %s", GetModelName(pTarget));
            }
        }
        break;
    case c_ctPickup:
        {
            if (pTarget) {
                m_goal = new PickupGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "I'm going to pick up %s", GetModelName(pTarget));
            }
        }
        break;
    case c_ctDefend:
        {
            if (pTarget) {
                m_goal = new DefendGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "I'll defend %s to the end", GetModelName(pTarget));
            }
        }
        break;
    case c_ctStop:
        {
            m_goal = new StayPutGoal(this);
            if (reply)
                SendChat(NA, "Just hangin'");
        }
        break;
    case c_ctDisable:
        {
            if (pTarget) {
                m_goal = new DisableGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "I'm going to attempt to disable %s", GetModelName(pTarget));
            }
        }
        break;
    case c_ctFollow:
        {
            if (pTarget) {
                m_goal = new FollowGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "Following %s", GetModelName(pTarget));
            }
        }
        break;
    case c_ctScout:
        {
            m_goal = new ScoutGoal(this, pTarget);
            if (reply)
                SendChat(NA, "I'm scouting %s", (pTarget) ? GetModelName(pTarget) : m_pShip->GetCluster()->GetName());
        }
        break;
    case c_ctRepair:
        {
            if (pTarget)
            {
                m_goal = new RepairGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "I'm repairing");
            }
        }
        break;
    case c_ctCapture:
        {
            if (pTarget) {
                m_goal = new CaptureGoal(this, pTarget);
                if (reply)
                    SendChat(NA, "Attempting to capture %s", GetModelName(pTarget));
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
    case c_ctConstruct:
        {
            ConstructionDrone*  pcd = NULL;
            CastTo(pcd, this);
            IstationTypeIGC*    pst = pcd->GetBuildStationType();

            if ((pTarget->GetObjectType() == OT_asteroid) &&
                ((IasteroidIGC*)pTarget)->HasCapability(pst->GetBuildAABM()))
            {
                m_goal = new ConstructGoal(this,
                                           pst,
                                           (IasteroidIGC*)pTarget);
                if (reply)
                {
                    SendChat(droneInTransitSound, "Building %s at %s", pst->GetName(), GetModelName(pTarget));
                }
            }
            else
            {
                SendChat(droneCantDoThatSound, "I can't do that");
            }
        }
        break;

    case c_ctTest:
        {
            m_goal = new TestGoal(this);
            bVerbose = true;    
            if (reply)
                SendChat(NA, "Initializing test run");
        }
        break;
    }
}


/*-------------------------------------------------------------------------
 * Function: Drone::SetGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Don't even have to switch on the command type, just backup and set the 
 *    goal
 */

void Drone::SetGoal(Goal* goal)
{ 
    if (goal) {
        BackupGoal();
        m_goal = goal;
    }
}


/*-------------------------------------------------------------------------
 * Function: Drone::SetGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Take the current goal, and move it to the top of the memory stack.
 */

void Drone::BackupGoal(void) 
{
    if (!m_goal)
        return;

    m_goalMemory.PushEnd(m_goal);
    m_goal = NULL;
} 


/*-------------------------------------------------------------------------
 * Function: Drone::RestoreGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Take the top of the stack, and put it as the current goal
 */

void Drone::RestoreGoal(void) 
{
    int goalNum = m_goalMemory.GetCount();

    if (goalNum) {
        if (m_goal)
            delete m_goal;
    
        goalNum--;  // the index equals the number of goals - 1
        m_goal = m_goalMemory[goalNum];
        m_goalMemory.Remove(goalNum);
    }
    else
    {
        Verbose("Tried to pop with no goals in memory");
    }
}


/*-------------------------------------------------------------------------
 * Function: Drone::ClearGoals
 *-------------------------------------------------------------------------
 * Purpose:
 *      Clear the current goal and the entire goal stack.  Start fresh.
 */

void Drone::ClearGoals(void) 
{
    int index = m_goalMemory.GetCount();

    while (index--) 
    {
        if (m_goalMemory[index])
            delete m_goalMemory[index];
        m_goalMemory.Remove(index);
    }

    if (m_goal)
        delete m_goal;
    m_goal = NULL;
}


/*-------------------------------------------------------------------------
 * Function: Drone::Instincts
 *-------------------------------------------------------------------------
 * Purpose:
 *      There are certain things that I always want to check, regardless of my
 *    goal.  Put all of those in here.
 */

bool Drone::Instincts(void) 
{
    if ((m_pShip->GetFraction() <= (1.0f - m_flBravery)) || (GetAmmoState(m_pShip) == c_asEmpty))          // Should I run away?
    {
        if ((dType == c_dtPirate) || (dType == c_dtBountyHunter))
        {
            if (!GetDisappear())
            {
                SendChat(NA, "It's just not worth it, I'm outta here");
                SetGoal(new DisappearGoal(this));
                return true;
            }
        }
        else
        {
            if (!m_goal || m_goal->GetType() != c_ctRepair)
            {
                // Check my entire goal history for a repair command, because I still want to be able to have sub-goals
                bool anyRepairGoals = false;

                for (int i = m_goalMemory.GetCount(); i>0; i--)
                {
                    if (m_goalMemory[i-1]->GetType() == c_ctRepair)
                    {
                        anyRepairGoals = true;
                        break;
                    }
                }
                        
                if (!anyRepairGoals)
                {
                    IstationIGC*    pstation = (IstationIGC*)(GetRepairStation());
                    if (pstation)
                    {
                        SendChat(droneTooMuchDamageSound, "Forget this.  I have to go get repaired");
                        SetGoal(new RepairGoal(this, pstation));
                        return true;
                    }
                }
            }
        }
    }

    // When we get DamageTracking for the drones...  Check to see if there is anyone that is just REALLY kicking our butts, REALLY being
    // defined in part by the bravery, and then decide to attack that person.
    return false;
}


/*-------------------------------------------------------------------------
 * Function: Drone::FindBestStation
 *-------------------------------------------------------------------------
 * Purpose:
 *      When the drone needs to dock, this function will return the station which
 *    he should dock at.  It gives priority to any station that it has been 
 *    explicitly told.
 */

IstationIGC* Drone::FindBestStation(bool fCheckEnemyStations) 
{
    IstationIGC* pHomeStation = m_pStation;

    // Go to my assigned station if it is in the cluster
    if (pHomeStation && m_pShip->GetCluster() == pHomeStation->GetCluster())
        return pHomeStation;
    
    // Find the closest friendly station in the cluster
    IstationIGC* pStation = (IstationIGC*) FindTarget(m_pShip,
                                                      c_ttFriendly | c_ttStation | c_ttNearest | c_ttAnyCluster);

    return pStation;    
}


/*-------------------------------------------------------------------------
 * Function: WingManDrone::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return a new default command for the drone
 */

Goal* WingManDrone::GetNewDefaultGoal(void)
{
    Verbose("Watching for enemy ships..");
    return new KillAnythingGoal(this); 
}


/*-------------------------------------------------------------------------
 * Function: AutoPilotDrone::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return a new default command for the drone
 */

Goal* AutoPilotDrone::GetNewDefaultGoal(void)
{
    return new IdleGoal(this); 
}


/*-------------------------------------------------------------------------
 * Function: PigPilotDrone::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return a new default command for the drone
 */

Goal* PigPilotDrone::GetNewDefaultGoal(void)
{
    return new IdleGoal(this); 
}

#if 0
void PigPilotDrone::Update(Time now, float dt) 
{
    // Do nothing until undocked
    if (m_pShip->GetStation())
        return;
}
#endif

/*-------------------------------------------------------------------------
 * Function: BountyHunter::SetGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Can only take one target, once.  Assert that here.
 */

void BountyHunter::SetGoal(CommandType cm, ImodelIGC* pTarget, bool reply)
{
    if (cm == c_ctDestroy && pTarget && pTarget->GetCluster())
    {
        if (m_pTarget)
        {
            if (reply)
                SendChat(NA, "I already have my target");
            return;
        }
        else 
        {
            ClearGoals();
            if (reply)
                SendChat(NA, "As you wish, %s will be terminated", GetModelName(m_pTarget));
            m_pTarget = pTarget;
        }
    }
    
    Drone::SetGoal(cm, pTarget);
}


/*-------------------------------------------------------------------------
 * Function: BountyHunter::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return a new default command for the drone
 */

Goal* BountyHunter::GetNewDefaultGoal(void)
{
    if (m_pTarget)                                                        // Already got my target
    {
        if (m_pTarget->GetCluster())                                    // He's not dead
        {
            Verbose("Attempting to kill %s", GetModelName(m_pTarget));
            return new DestroyGoal(this, m_pTarget);
        }
        else                                                            // He is dead
        {
            Verbose("My work here is done.  %s has been reduced to his component atoms", GetModelName(m_pTarget));
            return new DisappearGoal(this);
        }
    }
    else                                                                // Still waiting for a target
    {
        Verbose("Waiting for my kill command");
        return new IdleGoal(this);
    }
}


/*-------------------------------------------------------------------------
 * Function: Pirate::GetNewDefaultGoal
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return a new default command for the drone
 */

Goal* Pirate::GetNewDefaultGoal(void)
{
    // Look for a leader pirate in this sector
    // (aka, a pirate that is not currently just trying to follow someone else)
    // If I find one, then fly on his wing (follow)
    // If not, then I get to be the leader, and I should look for miners that I can take out

    // If I was already leaving, then I should still leave
    if (GetDisappear())
    {
        Verbose("Disappearing");
        return new DisappearGoal(this);
    }

    {
        Drone* leader = NULL;

        for (int i = 0; i < MAX_DRONES; i++)
        {
            if (leader = g_drones.m_pDroneList[i])  // This is an intentional assignment
            {
                if (leader != this && leader->GetDroneType() == c_dtPirate && leader->GetShip()->GetCluster() == m_pShip->GetCluster())
                {   // Then it is another Pirate in this sector, now check his goal
                    if (leader->GetGoal() && leader->GetGoal()->GetType() != c_ctMimic)
                    {   // Then we have our leader!
                        Verbose("Following %s, he's my hero", GetModelName(leader->GetShip()));
                        return new MimicGoal(this, leader);
                    }
                }
            }
        }
    }

    // If I get here, then that means I get to be the leader!  Let's find some trouble to get into...
    {
        Drone* target = NULL;
   
        for (int i = 0; i < MAX_DRONES; i++)
        {
            if (target = g_drones.m_pDroneList[i])
            {
                if (target != this && target->GetDroneType() == c_dtMining && target->GetShip()->GetCluster() == m_pShip->GetCluster())
                {   // Then it is a Miner in this sector... that's good enough.  Let's go get him
                    Verbose("I'm off to kill the miners");
                    return new DestroyGoal(this, target->GetShip());
                }
            }
        }
    }
    
    // If we get here, then there is really no damage that I can/should do in this sector... Disappear.
    Verbose("Disappearing");
    return new DisappearGoal(this);
}

void ConstructionDrone::Initialize(IstationTypeIGC* pstationtype)
{
    m_pstationtype = pstationtype;

    SetGoal(new ConstructGoal(this, pstationtype, NULL));
}
