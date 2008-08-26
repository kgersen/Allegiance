#ifndef _MISSION_H_
#define _MISSION_H_

#if 0
//////////////////////////////////////////////////////////////////////////////
//
// MissionContext
//
//////////////////////////////////////////////////////////////////////////////

class MissionContext : public IObject
{
public:
    virtual TRef<IshipIGC> GetShip() = 0;
    virtual ImissionIGC* GetMissionIGC() = 0;
    virtual TRef<Modeler> GetModeler() = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// Regions
//
//////////////////////////////////////////////////////////////////////////////

class Region : public IObject
{
public:
    virtual bool InRegion(ImodelIGC* pmodel) = 0;
};

class PlanarRegion : public Region
{
public:
            
};

class SphericalRegion : public Region
{
public:
    static TRef<SphericalRegion> Create(ImodelIGC* pmodel, float radius);
};



//////////////////////////////////////////////////////////////////////////////
//
// Path
//
//////////////////////////////////////////////////////////////////////////////

class PathSegment : public IObject
{
public:
};

class Path : public IObject
{
public:
};


//////////////////////////////////////////////////////////////////////////////
//
// MissionStage
//
//////////////////////////////////////////////////////////////////////////////

class MissionStage: public IObject
{
public:
};



//////////////////////////////////////////////////////////////////////////////
//
// MissionEventHandler
//
//////////////////////////////////////////////////////////////////////////////


class MissionEventHandler : public IObject
{
public:
    virtual bool OnModelTerminated(ImodelIGC* model){return false;}
    virtual bool OnModelKilled(ImodelIGC* model){return false;}
    virtual bool OnModelDamaged(ImodelIGC* model){return false;}
    virtual bool OnDocked(IshipIGC* pship, IstationIGC* pstation){return false;}
    virtual bool OnSwitchSector(IshipIGC* pship, IclusterIGC* pclusterOld, IclusterIGC* pclusterNew){return false;}
    virtual bool OnHitTreasure(IshipIGC* pship, ItreasureIGC* ptreasure){return false;}
    virtual bool OnEnteredRegion(ImodelIGC* pmodel, Region* pregion){return false;}
    virtual bool OnExitedRegion(ImodelIGC* pmodel, Region* pregion){return false;}
    virtual bool OnDivergedPath(IshipIGC* pship, Path* ppath, float distance, float angle){return false;}
    virtual bool OnSwitchedMissionStage(MissionStage* pstageNew, MissionStage* pstageOld){return false;}
};

//////////////////////////////////////////////////////////////////////////////
//
// MissionManager
//
//////////////////////////////////////////////////////////////////////////////


enum DestroyDroneMethod
{
    DestroyDroneExplode,
    DestroyDroneWarp,
    DestroyDroneOffScreen,
    DestroyDroneFlank,
    DestroyDroneDock
};

class MissionManager : public IObject
{
public:
    virtual void Update(Time time, float dtime) = 0;

    // stuff for loading, creating, and accessing items

    virtual void CreateDefaults() = 0;
    virtual void LoadClusters(const ZString& strMdlFile) = 0;
    virtual void LoadTechTree(const ZString& strMdlFile) = 0;
    /*
    virtual Drone* CreateDrone(DroneType dt, const ZString& strLoadout, SideID sideID, const ZString& strName,
                    const Vector& pos, IclusterIGC* pcluster) = 0;
    */
    virtual TRef<IshipIGC> CreateShip(const ZString& strLoadout, SideID sideID, const ZString& strName) = 0;
    virtual TRef<Region> GetRegion(const ZString& strRegion) = 0;
//    virtual void CreatePowerup(const ZString& strPartType) = 0;
//    virtual void DestroyDrone(Drone* pdrone, DestroyDroneMethod method) = 0;

    // Events and mission flow
    
    virtual void SetMissionStage(MissionStage* pstageNew) = 0;
    virtual void AdviseRegion(Region* pregion, ImodelIGC* pmodel) = 0;
    virtual void UnadviseRegion(Region* pregion, ImodelIGC* pmodel) = 0;
    virtual void AddMissionEventHandler(MissionEventHandler* phandler) = 0;
    virtual void RemoveMissionEventHandler(MissionEventHandler* phandler) = 0;

    // Triggering of events
    
    virtual void FireOnModelTerminated(ImodelIGC* model) = 0;
    virtual void FireOnModelKilled(ImodelIGC* model) = 0;
    virtual void FireOnModelDamaged(ImodelIGC* model) = 0;
    virtual void FireOnDocked(IshipIGC* pship, IstationIGC* pstation) = 0;
    virtual void FireOnSwitchSector(IshipIGC* pship, IclusterIGC* pclusterOld, IclusterIGC* pclusterNew) = 0;
    virtual void FireOnHitTreasure(IshipIGC* pship, ItreasureIGC* ptreasure) = 0;
    virtual void FireOnEnteredRegion(ImodelIGC* pmodel, Region* pregion) = 0;
    virtual void FireOnExitedRegion(ImodelIGC* pmodel, Region* pregion) = 0;
    virtual void FireOnDivergedPath(IshipIGC* pship, Path* ppath, float distance, float angle) = 0;
    virtual void FireOnSwitchedMissionStage(MissionStage* pstageNew, MissionStage* pstageOld) = 0;


    static TRef<MissionManager> Create(MissionContext* pcontext);

};


#endif
#endif

