#include "pch.h"

    

/*-------------------------------------------------------------------------
 * Function: Drones::Drones
 *-------------------------------------------------------------------------
 * Purpose:
 *      Start with a clean slate -- empty the drone list
 */
Drones::Drones() :
    m_pPilot(NULL)
{
    memset(m_pDroneList, 0, sizeof(Drone*) * MAX_DRONES);
}


/*-------------------------------------------------------------------------
 * Function: Drones::SetCore
 *-------------------------------------------------------------------------
 * Purpose:
 *      Set the drones core pointer, and initialize this core with all of the commands
 *    that the ships and drones can use.  This is the first place to add a command to the game.
 */
void Drones::SetCore(ImissionIGC* core)
{ 
    assert(core);

    // When I set the core, I need to register all of the possible drone commands with that core
    const short NONE        = 0;
    const short ALEPHS      = (1 << OT_warp);
    const short SHIPS       = (1 << OT_ship);
    const short PROJECTILES = (1 << OT_projectile);
    const short ASTEROIDS   = (1 << OT_asteroid);
    const short STATIONS    = (1 << OT_station);
    const short TREASURES   = (1 << OT_treasure);
    const short MISSILES    = (1 << OT_missile);

    core->AddCommand(c_ctDestroy,   "kill",         "cmdDestroy",   SHIPS | ASTEROIDS | STATIONS);
    core->AddCommand(c_ctMine,      "mine",         "cmdMine",      ASTEROIDS);
    core->AddCommand(c_ctUnload,    "unload",       "cmdUnload",    STATIONS);
    core->AddCommand(c_ctPatrol,    "patrol",       "cmdPatrol",    ALEPHS | ASTEROIDS | STATIONS);
    core->AddCommand(c_ctGoto,      "goto",         "cmdGoto",      ALEPHS | SHIPS | ASTEROIDS | STATIONS | TREASURES);
    core->AddCommand(c_ctPickup,    "pickup",       "cmdPickup",    TREASURES);
    core->AddCommand(c_ctDefend,    "defend",       "cmdDefend",    ALEPHS | SHIPS | ASTEROIDS | STATIONS | TREASURES);
    core->AddCommand(c_ctStop,      "stop",         "cmdStop",      NONE);
    core->AddCommand(c_ctDisable,   "disable",      "cmdDisable",   SHIPS | STATIONS);
    core->AddCommand(c_ctFollow,    "follow",       "cmdFollow",    SHIPS);
    core->AddCommand(c_ctScout,     "scout",        "cmdScout",     ALEPHS | SHIPS | ASTEROIDS | STATIONS | TREASURES);
    core->AddCommand(c_ctHome,      "home",         "cmdHome",      STATIONS);
    core->AddCommand(c_ctStatus,    "status",       "cmdStatus",    NONE);
    core->AddCommand(c_ctVerbose,   "verbose",      "",             NONE);
    core->AddCommand(c_ctSDestruct, "die",          "",             NONE);
    core->AddCommand(c_ctPop,       "cancel",       "cmdCancel",    NONE);
    core->AddCommand(c_ctRepair,    "repair",       "cmdRepair",    STATIONS);
    core->AddCommand(c_ctCapture,   "capture",      "cmdCapture",   STATIONS);
    core->AddCommand(c_ctFace,      "face",           "",           ALEPHS | SHIPS | ASTEROIDS | STATIONS | TREASURES);
    core->AddCommand(c_ctConstruct, "build",        "cmdBuild",     ASTEROIDS);
//  core->AddCommand(c_ctTest,      "test",         "",             NONE);
}


/*-------------------------------------------------------------------------
 * Function: Drones::Create
 *-------------------------------------------------------------------------
 * Purpose:
 *      Take all of the parameters that I need to make the IGC ship, and make a drone on
 *    top of that.
 */
Drone * Drones::Create(ImissionIGC*     pMission,
                       DroneType        dtType,
                       const char*      name,
                       HullID           hullID,
                       IsideIGC*        side,
                       Mount            nParts,
                       const PartData*  parts,
                       float            shootSkill,
                       float            moveSkill,
                       float            bravery)
{
    // Do IGC initialization:
    const Mount maxParts = 10;
    assert (nParts < maxParts);
    char bfr[sizeof(DataShipIGC) + maxParts * sizeof(PartData)];
    DataShipIGC* ds = (DataShipIGC*)bfr;

    // Make sure we don't exceed our drone limit
    {
        int oldIndex = m_index;
        do
        {
            m_index = (m_index + 1) % MAX_DRONES;
        }
        while (m_pDroneList[m_index] && (m_index != oldIndex));

        if (m_pDroneList[m_index])
        {
            return NULL;
        }
    }

    ds->shipID = -(m_index + 2);
    ds->cmLegal = GetDroneCommands(dtType);            // Set the legal commands for the ship
    ds->hullID = hullID;
    ds->sideID = side->GetObjectID();
    ds->wingID = NA;
    ds->nKills = 0;
    ds->nDeaths = 0;

    // If it's not given a name, call it Drone#
    assert ((!name) || (strlen(name) < c_cbName - 4));
    strcpy(ds->name, name ? name : "Drone");
    _itoa(m_index, ds->name + strlen(ds->name), 10);

    ds->nParts = nParts;
    ds->partsOffset = sizeof(DataShipIGC);            // Set up the parts
    {
        PartData*   pd = (PartData*)&bfr[sizeof(DataShipIGC)];
        for (int i = 0; (i < nParts); i++)
            *(pd++) = parts[i];
    }

    IshipIGC* ship = (IshipIGC*)(pMission->CreateObject(Time::Now(),            // Make the IGC ship
                                OT_ship, bfr, sizeof(DataShipIGC) + nParts * sizeof(PartData)));

    if (ship)
    {
        // Create the drone on that ship
        switch (dtType) {
        case  c_dtMining:
            m_pDroneList[m_index] = new MiningDrone(ship);
            break;
        case c_dtTurret:
            m_pDroneList[m_index] = new TurretDrone(ship);
            break;
        case c_dtWingman:
            m_pDroneList[m_index] = new WingManDrone(ship);
            break;
        case c_dtBountyHunter:
            m_pDroneList[m_index] = new BountyHunter(ship);
            break;
        case c_dtPirate:
            m_pDroneList[m_index] = new Pirate(ship);
            break;
        case c_dtDummy:
            m_pDroneList[m_index] = new DumbDrone(ship);
            break;
        case c_dtConstruction:
            m_pDroneList[m_index] = new ConstructionDrone(ship);
            break;
        default:
            assert(0);
            break;
        }
        ship->SetPrivateData((DWORD)m_pDroneList[m_index]);
        ship->Release();

        // Set the default skills
        m_pDroneList[m_index]->SetShootSkill(shootSkill);
        m_pDroneList[m_index]->SetMoveSkill(moveSkill);
        m_pDroneList[m_index]->SetBravery(bravery);

        return m_pDroneList[m_index];
    }

    return NULL;
}


/*-------------------------------------------------------------------------
 * Function: Drones::MessageToDrone #1
 *-------------------------------------------------------------------------
 * Purpose:
 *      Parse the string for the drone name, and pass it on to the next msg function
 */
void Drones::MessageToDrone(Time now, IshipIGC* pSender, const char* msg, ImodelIGC* pmodelObject, bool fCheckPilot)
{
    assert (pSender);
    IsideIGC*   pside = pSender->GetSide();

    // Figure out which drone this message is for:
    int length = 0;
    Drone * pdRecip = NULL;

    for (int i=0; i<MAX_DRONES; i++) 
    {
        if (m_pDroneList[i] && (m_pDroneList[i]->GetShip()->GetSide() == pside)) 
        {
            length = strlen(m_pDroneList[i]->GetShip()->GetName());
            if ((_strnicmp(m_pDroneList[i]->GetShip()->GetName(), msg, length) == 0) && isspace(msg[length])) 
            {
                // Make sure that it is an exact match.
                pdRecip = m_pDroneList[i];
                msg += length + 1;
                break;
            }
        }
    }    

    if (!pdRecip && fCheckPilot && m_pPilot)
    {
        length = strlen(m_pPilot->GetShip()->GetName());
        if ((_strnicmp(m_pPilot->GetShip()->GetName(), msg, length) == 0) && isspace(msg[length]))
            msg += length + 1;
        pdRecip = m_pPilot;
    }

    if (pdRecip)
        MessageToDrone(now, pSender, pdRecip, msg, pmodelObject);
}


/*-------------------------------------------------------------------------
 * Function: Drones::MessageToDrone #2
 *-------------------------------------------------------------------------
 * Purpose:
 *      We know the ship id, still have to parse for the command
 */
void Drones::MessageToDrone(Time now, IshipIGC* pSender, Drone * pdRecip, const char* msg, ImodelIGC*  pmodelObject)
{
    assert (pSender);

    if (!pdRecip || OBJECT_IS_BAD(pdRecip->GetShip()))
    {
        if (!m_pPilot || m_pPilot != pdRecip)        // allow it to be to the autoPilot
            return;
    }

    if (pSender->GetSide() != pdRecip->GetShip()->GetSide())
        return;     //Nice try

    CommandID cm = c_ctNoCommand;

    // Then figure out which command the message is:
    for (CommandID id = 0; id < pSender->GetMission()->GetNcommands(); id++)
    {
        const CommandData& cmddata = pSender->GetMission()->GetCommand(id);
        if (cmddata.MatchCommand(msg) == CommandData::c_mComplete)
            cm = id;
    }
    
    // Hack in some cheats to set the drone levels    
    if (cm == c_ctNoCommand)
    {
        ZString zmsg(msg);

        ZString comm = zmsg.GetToken();
        ZString zvalue = zmsg.GetToken();
        
        if (!zvalue.IsEmpty())
        {
            float value = (float) atoi(zvalue) / 100.0f;
            if (!_stricmp(comm, "sskill"))
            {
                pdRecip->SetShootSkill(value);
                return;
            }
            else if (!_stricmp(comm, "mskill"))
            {
                pdRecip->SetMoveSkill(value);
                return;
            }
            else if (!_stricmp(comm, "bravery"))
            {
                pdRecip->SetBravery(value);
                return;
            }
        }
    }

    if (pdRecip != m_pPilot)
        MessageToDrone(now, pSender, pdRecip, cm, pmodelObject);
    else
        MessageToPilot(cm, pmodelObject);
}


/*-------------------------------------------------------------------------
 * Function: Drones::MessageToDrone #3
 *-------------------------------------------------------------------------
 * Purpose:
 *      We know everything we need to.  pass it on to the drone
 */
void Drones::MessageToDrone(Time now, IshipIGC* pSender, Drone * pdRecip, CommandID cm, ImodelIGC* pmodelObject)
{ 
    if (!pdRecip || OBJECT_IS_BAD(pdRecip->GetShip()))
        return;

    if (pSender->GetSide() != pdRecip->GetShip()->GetSide())
        return;     //Nice try

    pdRecip->Message(now, cm, pmodelObject);
}

/*-------------------------------------------------------------------------
 * Function: Drones::DockEvent
 *-------------------------------------------------------------------------
 * Purpose:
 *      Pass the dock event on to the intended drone
 */
void Drones::DockEvent(IshipIGC * pship)
{
    Drone * pdrone = (Drone *) pship->GetMission()->GetIgcSite()->GetDroneFromShip(pship);
    if (!pdrone)
        return;

    pdrone->DockEvent();
}


/*-------------------------------------------------------------------------
 * Function: Drones::GetDroneCommands
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return the command mask of supported commands for a drone type.  This
 *    is the only place where need to change which commands are accepted by which drones
 */
CommandMask Drones::GetDroneCommands(DroneType dt) 
{
    CommandMask commands = 0;
    
    const CommandMask STANDARD = (1 << c_ctStop) | (1 << c_ctHome) | (1 << c_ctStatus) | (1 << c_ctVerbose) | 
                                 (1 << c_ctSDestruct) | (1 << c_ctTest) | (1 << c_ctPop) | (1 << c_ctRepair) |
                                 (1 << c_ctFace);
    
    switch (dt) 
    {
    case c_dtMining:
        commands = (STANDARD | (1 << c_ctMine) | (1 << c_ctUnload) | (1 << c_ctDestroy) | (1 << c_ctGoto) | (1 << c_ctFollow) | (1 << c_ctScout));
        break;
    case c_dtTurret:
        commands = (STANDARD | (1 << c_ctDestroy) | (1 << c_ctGoto) | (1 << c_ctDefend) | (1 << c_ctFollow));
        break;
    case c_dtWingman:
        commands = (STANDARD | (1 << c_ctDestroy) | (1 << c_ctPatrol) | (1 << c_ctGoto) | (1 << c_ctPickup) | 
                               (1 << c_ctDefend) | (1 << c_ctDisable) | (1 << c_ctFollow) | (1 << c_ctScout) | (1 << c_ctCapture));
        break;
    case c_dtBountyHunter:
        commands = (1 << c_ctStatus) | (1 << c_ctVerbose) | (1 << c_ctDestroy) | (1 << c_ctSDestruct);
        break;
    case c_dtPirate:
        commands = (1 << c_ctVerbose) | (1 << c_ctSDestruct);
        break;
    case c_dtDummy:
        commands = (STANDARD | (1 << c_ctDestroy) | (1 << c_ctPatrol) | (1 << c_ctGoto) | (1 << c_ctPickup) | 
                               (1 << c_ctDefend) | (1 << c_ctFollow) | (1 << c_ctScout));
        break;
    case c_dtAutoPilot:
        commands = (1 << c_ctGoto);
        break;

    case c_dtConstruction:
        commands = STANDARD | (1 << c_ctConstruct) | (1 << c_ctGoto) | (1 << c_ctFollow);
        break;

    case c_dtPigPilot:
        // Allow everything but mining, unloading, and building
        commands = ~((1 << c_ctMine) | (1 << c_ctUnload) | (1 << c_ctConstruct));
        break;

    default:
        break;
    }

    return commands;
}


/*-------------------------------------------------------------------------
 * Function: Drones::Update
 *-------------------------------------------------------------------------
 * Purpose:
 *      This is the main game loop.  Update the state of all of the drones
 */
void Drones::Update(Time  lastUpdate, 
                    Time  now)
{
    for (int i=0; i<MAX_DRONES; i++) 
    {
        if (m_pDroneList[i])
            m_pDroneList[i]->Update(now, now - lastUpdate);
    }
}


/*-------------------------------------------------------------------------
 * Function: Drones::CreatePilot
 *-------------------------------------------------------------------------
 * Purpose:
 *      Need to attach the autoPilot to an existing ship
 */
void Drones::CreatePilot(IshipIGC* ship) 
{
    if(!ship)
        return;

    if (m_pPilot)
        delete m_pPilot;

    m_pPilot = new AutoPilotDrone(ship);        // Make the autoPilot drone

    m_pPilot->SetShootSkill(1.0f);                // Set default skill levels
    m_pPilot->SetMoveSkill(1.0f);
    m_pPilot->SetBravery(0.5f);

    m_pPilot->ToggleVerbose();                    // Start the pilot as verbose

    ship->SetPrivateData((DWORD)m_pPilot);
}


/*-------------------------------------------------------------------------
 * Function: Drones::MessageToPilot
 *-------------------------------------------------------------------------
 * Purpose:
 *      Pass the message on to the autoPilot drone
 */
void Drones::MessageToPilot(CommandID cm, ImodelIGC* pmodelObject)
{
    assert(m_pPilot);
    if (!m_pPilot->GetShip()->AcceptsCommandF(cm))
        cm = c_ctNoCommand;
    m_pPilot->Message(Time::Now(), (CommandType) cm, pmodelObject);
}

/*-------------------------------------------------------------------------
 * Function: Drones::UpdatePilot
 *-------------------------------------------------------------------------
 * Purpose:
 *      Update the controls of the autoPilot drone
 */
void Drones::UpdatePilot(Time lastUpdate, Time now, float dt)
{
    assert(m_pPilot);
    m_pPilot->Update(now, dt); 
}

/*-------------------------------------------------------------------------
 * Function: Drones::ResetPilot
 *-------------------------------------------------------------------------
 * Purpose:
 *      Clear the goals of the autoPilot drone
 */
void Drones::ResetPilot(void)
{
    assert(m_pPilot);
    m_pPilot->ClearGoals();
}


/*-------------------------------------------------------------------------
 * Function: Drones::GetPilotShip
 *-------------------------------------------------------------------------
 * Purpose:
 *      Return the ship pointer that the autoPilot is using
 */
IshipIGC* Drones::GetPilotShip(void) 
{ 
    return (m_pPilot) ? m_pPilot->GetShip() : NULL; 
}


/*-------------------------------------------------------------------------
 * Function: Drones::GetPilotShip
 *-------------------------------------------------------------------------
 * Purpose:
 *      Set the ship pointer that the autoPilot is using
 */
void Drones::SetPilotShip(IshipIGC* ship) 
{ 
    if (m_pPilot && ship) 
    {
        m_pPilot->SetShip(ship); 
        ship->SetPrivateData((DWORD)m_pPilot);
    }
}


/*-------------------------------------------------------------------------
 * Function: Drones::CreatePigPilot
 *-------------------------------------------------------------------------
 * Purpose:
 *      Create a pig autoPilot drone using the existing ship.
 */
Drone* Drones::CreatePigPilot(IshipIGC* ship)
{
  if (!ship)
    return NULL;

  Drone* pPilot = new PigPilotDrone(ship);  // Make the PigPilot drone
  ship->SetPrivateData((DWORD)pPilot);

  pPilot->SetShootSkill(1.0f);              // Set default skill levels
  pPilot->SetMoveSkill(1.0f);
  pPilot->SetBravery(0.5f);

  pPilot->ToggleVerbose();                  // Start the pilot as verbose

  return pPilot;
}


/*-------------------------------------------------------------------------
 * Function: Drones::DeleteShip
 *-------------------------------------------------------------------------
 * Purpose:
 *      Delete the drone based on this ship
 */
bool Drones::DeleteShip(IshipIGC* ship) 
{
    assert(ship);
    Drone * pdrone = (Drone *) ship->GetMission()->GetIgcSite()->GetDroneFromShip(ship);
    
    if (pdrone)
    {
        if (pdrone == m_pPilot)
        {
            delete m_pPilot;
            m_pPilot = NULL;
        }
        else
        {
            m_pDroneList[pdrone->GetIndex()] = NULL;
            delete pdrone;
        }
        return true;
    }
    return false;
}


/*-------------------------------------------------------------------------
 * Function: Drones::Purge
 *-------------------------------------------------------------------------
 * Purpose:
 *      Delete all of the drones
 */
void Drones::Purge(ImissionIGC* pMission) 
{
    for (int i=0; i<MAX_DRONES; i++) 
    {
        if (m_pDroneList[i] &&
            (!pMission || pMission == m_pDroneList[i]->GetShip()->GetMission()))
        {
            delete m_pDroneList[i];
            m_pDroneList[i] = NULL;
        }
    }
    if (m_pPilot)
    {
        assert(!pMission || pMission == m_pPilot->GetShip()->GetMission());
        delete m_pPilot;
        m_pPilot = NULL;
    }
}


/*-------------------------------------------------------------------------
 * Function: Drones::DisappearingDrone
 *-------------------------------------------------------------------------
 * Purpose:
 *      Check if this drone is trying to disappear
 */
bool Drones::DisappearingDrone(IshipIGC * pship)
{
    Drone * pdrone = (Drone *) (pship->GetMission()->GetIgcSite()->GetDroneFromShip(pship));
    return (pdrone && pdrone->GetDisappear());
}

void    Drones::InitializeConstructor(Drone*           pdrone,
                                      IstationTypeIGC* pstationtype)
{
    assert (pdrone->GetDroneType() == c_dtConstruction);
    ConstructionDrone*  pcd = (ConstructionDrone*)pdrone;

    pcd->Initialize(pstationtype);
}
