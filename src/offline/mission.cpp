#include "pch.h"


//////////////////////////////////////////////////////////////////////////////
//
// IGCObjectFactory
//
//////////////////////////////////////////////////////////////////////////////
#if 0
typedef TRef<IObject> TRefIObject; //hack for VC5

template<class ClassType>
class TIGCObjectFactory : public IFunction
{
private:
    typedef TRef<IObject> (ClassType::*PFNMember)(ObjectStack& stack);
    
    TRef<ClassType> m_pobj;
    PFNMember m_pfn;
    
public:
    TIGCObjectFactory(ClassType* pobj, PFNMember pfn) :
        m_pobj(pobj),
        m_pfn(pfn)
    {
    }

    TRefIObject Apply(ObjectStack& stack)
    {
        return (m_pobj->*m_pfn)(stack);
    }
};



//////////////////////////////////////////////////////////////////////////////
//
// SphericalRegion
//
//////////////////////////////////////////////////////////////////////////////

class SphericalRegionImpl : public SphericalRegion
{
private:
    TRef<ImodelIGC> m_pmodelCenter;
    float           m_radiusSquared;
    
public:

    SphericalRegionImpl(ImodelIGC* pmodel, float radius) :
        m_pmodelCenter(pmodel),
        m_radiusSquared(radius * radius)
    {
    }

    bool InRegion(ImodelIGC* pmodel)
    {
        return ((pmodel->GetPosition() 
                    - m_pmodelCenter->GetPosition()).LengthSquared()
                <= m_radiusSquared);
    }

};


TRef<SphericalRegion> SphericalRegion::Create(ImodelIGC* pmodel, float radius)
{
    return new SphericalRegionImpl(pmodel, radius);
}

//////////////////////////////////////////////////////////////////////////////
//
// RegionAdviseEntry
//
//////////////////////////////////////////////////////////////////////////////

class RegionAdviseEntry : public IObject
{
private:
    TRef<Region> m_pregion;
    TMap<TRef<ImodelIGC>, bool> m_mapInRegion;
    TList<TRef<ImodelIGC> > m_lstUnadvise;

public:

    RegionAdviseEntry(Region* pregion) :
        m_pregion(pregion)
    {
    }

    void Advise(ImodelIGC* pmodel)
    {
        m_mapInRegion.Set(pmodel, m_pregion->InRegion(pmodel));
        m_lstUnadvise.Remove(pmodel);
    }

    void Unadvise(ImodelIGC* pmodel)
    {
        m_lstUnadvise.PushEnd(pmodel);
    }

    void UpdateUnadvise()
    {
        TList<TRef<ImodelIGC> >::Iterator iter(m_lstUnadvise);
        while (!iter.End())
            {
            m_mapInRegion.Remove(iter.Value());
            iter.Next();
            }
        m_lstUnadvise.SetEmpty();          
    }
    
    void Update(MissionManager* pmanager)
    {
        TMap<TRef<ImodelIGC>, bool>::Iterator iter(m_mapInRegion);
        while (!iter.End())
            {
            bool bInRegion = iter.Value();
            TRef<ImodelIGC> pmodel = iter.Key();

            if (bInRegion != m_pregion->InRegion(pmodel) &&
                    !m_lstUnadvise.Find(pmodel))
                {
                if (bInRegion)
                    pmanager->FireOnExitedRegion(pmodel, m_pregion);
                else
                    pmanager->FireOnEnteredRegion(pmodel, m_pregion);
                m_mapInRegion.Set(pmodel, !bInRegion);
                }
                 
            iter.Next();
            }
        UpdateUnadvise();
    }

};

//////////////////////////////////////////////////////////////////////////////
//
// MissionManager
//
//////////////////////////////////////////////////////////////////////////////

class MissionManagerImpl : public MissionManager
{
private:
    TRef<MissionContext> m_pmissionContext;

    TList<TRef<MissionEventHandler> > m_lstEventHandlers;
    TMap<TRef<Region>, TRef<RegionAdviseEntry> > m_mapRegionAdviseEntries;
    TRef<MissionStage> m_pmissionStage;

    TRef<INameSpace> m_pnsClusters;
    TRef<INameSpace> m_pnsTechTree;

    short m_clusterIDNext;
    short m_asteroidIDNext;
    short m_stationTypeIDNext;
    short m_hullIDNext;
    short m_partTypeIDNext;
    short m_shipIDNext;
    short m_projectileTypeIDNext;
    short m_expendableTypeIDNext;

public:

    MissionManagerImpl(MissionContext* pmissionContext) :
        m_pmissionContext(pmissionContext),
        m_clusterIDNext(0),
        m_asteroidIDNext(0),
        m_hullIDNext(0),
        m_stationTypeIDNext(0),
        m_partTypeIDNext(0),
        m_projectileTypeIDNext(0),
        m_expendableTypeIDNext(0),
        m_shipIDNext(0)
    {
    }

    TRef<IhullTypeIGC> GetHullType(const ZString& strHullType)
    {
        TRef<IhullTypeIGC> phullType;
        if (m_pnsTechTree)
            phullType = (IhullTypeIGC*)m_pnsTechTree->FindMember(strHullType);
        return phullType;            
    }

    int ReadPartData(IhullTypeIGC* phullType, PartData* ppartData, IObjectList* ppartsList)
    {
        Mount rgMounts[ET_MAX - 1];
        memset(rgMounts, 0, sizeof(Mount)*(ET_MAX-1));
        int nParts = 0;
        
        ppartsList->GetFirst();
        while (NULL != ppartsList->GetCurrent())
            {
            TRef<IpartTypeIGC> ppartType = (IpartTypeIGC*)ppartsList->GetCurrent();

            EquipmentType et = ppartType->GetEquipmentType();
    
            Mount mountETMax = 1;
            if (ET_Weapon == et)
                mountETMax = phullType->GetMaxWeapons();

            nParts++;
            rgMounts[et-1]++;
            ppartData->partID = ppartType->GetObjectID();
            ppartData->mountID = rgMounts[et-1] > mountETMax ? NA : rgMounts[et-1]-1;
            ppartData->amount = 0;
            ppartData++;
            
            ppartsList->GetNext();
            }
        return nParts;
    }

    /*
    Drone* CreateDrone(DroneType dt, const ZString& strLoadout, SideID sideID, const ZString& strName,
                    const Vector& pos, IclusterIGC* pcluster)
    {
        ZAssert(m_pnsTechTree);

        TRef<IObjectList> pparamList = (IObjectList*)m_pnsTechTree->FindMember(strLoadout);

        // hulltype
        TRef<IhullTypeIGC> phullType = (IhullTypeIGC*)pparamList->GetFirst();
        HullID hullID = phullType->GetObjectID();

        // parts
        TRef<IObjectList> ppartsList =  (IObjectList*)pparamList->GetNext();
        PartData partData[10];
        int cParts = ReadPartData(phullType, partData, ppartsList);

        Drone* pdrone = g_drones.Create(
                        m_pmissionContext->GetMissionIGC(), 
                        dt,
                        strName,
                        hullID,
                        m_pmissionContext->GetMissionIGC()->GetSide(sideID),
                        cParts, 
                        partData);

        pdrone->GetShip()->SetPosition(pos);
        pdrone->GetShip()->SetCluster(pcluster);
        
        return pdrone;
    }
    */

    TRef<IshipIGC> CreateShip(const ZString& strLoadout, SideID sideID, const ZString& strName)
    {
        ZAssert(m_pnsTechTree);

        char buf[4096];
        DataShipIGC* pdata = (DataShipIGC*)buf;
        PartData* ppartData = (PartData*)(buf + sizeof(DataShipIGC));

        TRef<IObjectList> pparamList = (IObjectList*)m_pnsTechTree->FindMember(strLoadout);
        TRef<IhullTypeIGC> phullType = (IhullTypeIGC*)pparamList->GetFirst();
        
        pdata->hullID = phullType->GetObjectID();
        pdata->shipID = m_shipIDNext++;
        pdata->sideID = 0;
        pdata->wingID = 0;
        pdata->nKills = 0;
        pdata->nDeaths = 0;
        strcpy(pdata->name, strName);
        pdata->partsOffset = sizeof(DataShipIGC);

        TRef<IObjectList> ppartsList =  (IObjectList*)pparamList->GetNext();
        pdata->nParts = ReadPartData(phullType, ppartData, ppartsList);

        IshipIGC* pship = (IshipIGC*)m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(), OT_ship, pdata, sizeof(DataShipIGC) + sizeof(PartData)*pdata->nParts);
        pship->Release();
        
        return pship;
    }

    void LoadClusters(const ZString& strMdlFile)
    {
        TRef<INameSpace> pns =
            m_pmissionContext->GetModeler()->CreateNameSpace(
                        "mission",
                        m_pmissionContext->GetModeler()->GetNameSpace("model"));

        pns->AddMember("CreateCluster",   new TIGCObjectFactory<MissionManagerImpl>(this, CreateCluster));
        pns->AddMember("CreateAsteroid",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateAsteroid));
        pns->AddMember("CreateSphericalRegion",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateSphericalRegion));
        //pns->AddMember("CreateDrone",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateDrone));
        pns->AddMember("CreateStation",   new TIGCObjectFactory<MissionManagerImpl>(this, CreateStation));

        m_pnsClusters =
            m_pmissionContext->GetModeler()->GetNameSpace(strMdlFile);
    }

    void LoadTechTree(const ZString& strMdlFile)
    {
        TRef<INameSpace> pns =
            m_pmissionContext->GetModeler()->CreateNameSpace(
                        "mission",
                        m_pmissionContext->GetModeler()->GetNameSpace("model"));

        pns->AddMember("CreateStationType",   new TIGCObjectFactory<MissionManagerImpl>(this, CreateStationType));
        pns->AddMember("CreateHullType",   new TIGCObjectFactory<MissionManagerImpl>(this, CreateHullType));
        pns->AddMember("CreateAfterburnerType",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateAfterburnerType));
        pns->AddMember("CreatePackType",  new TIGCObjectFactory<MissionManagerImpl>(this, CreatePackType));
        pns->AddMember("CreateProjectileType",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateProjectileType));
        pns->AddMember("CreateWeaponType",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateWeaponType));
        pns->AddMember("CreateMissileType",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateMissileType));
        pns->AddMember("CreateMissileLauncherType",  new TIGCObjectFactory<MissionManagerImpl>(this, CreateMissileLauncherType));

        m_pnsTechTree =
            m_pmissionContext->GetModeler()->GetNameSpace(strMdlFile);
    }

    void ReadBuyableData(DataBuyableIGC* pdata, ObjectStack& stack)
    {
        pdata->price = (Money)GetNumber((IObject*)stack.Pop());
        strcpy(pdata->modelName, GetString((IObject*)stack.Pop()));
        pdata->iconName[0] = '\0';
        strcpy(pdata->name, GetString((IObject*)stack.Pop()));
        strcpy(pdata->description, GetString((IObject*)stack.Pop()));

        TRef<IObjectList> plistRequired; CastTo(plistRequired, (IObject*)stack.Pop());
        plistRequired->GetFirst();
        while (NULL != plistRequired->GetCurrent())
            {
            pdata->ttbmRequired.SetBit((int)GetNumber((IObject*)plistRequired->GetCurrent()));
            plistRequired->GetNext();
            }

        TRef<IObjectList> plistEffects; CastTo(plistEffects, (IObject*)stack.Pop());
        plistEffects->GetFirst();
        while (NULL != plistEffects->GetCurrent())
            {
            pdata->ttbmEffects.SetBit((int)GetNumber((IObject*)plistEffects->GetCurrent()));
            plistEffects->GetNext();
            }
    }

    void ReadPartTypeData(DataPartTypeIGC* pdata, EquipmentType et, ObjectStack& stack)
    {
        ReadBuyableData(pdata, stack);
        pdata->partID = m_partTypeIDNext++;
        pdata->successorPartID = NA;
        pdata->equipmentType = et;
        pdata->mass = GetNumber((IObject*)stack.Pop());
        pdata->signature = GetNumber((IObject*)stack.Pop());
        pdata->treasureChance = (short)GetNumber((IObject*)stack.Pop());
        pdata->partSize = (PartSize)GetNumber((IObject*)stack.Pop());
    }

    void ReadDataObject(DataObjectIGC* pdata, ObjectStack& stack)
    {
        pdata->color.r = pdata->color.g = pdata->color.b = pdata->color.a = 0; //GetColor((IObject*)stack.Pop());
        pdata->radius = GetNumber((IObject*)stack.Pop());
        pdata->rotation = GetNumber((IObject*)stack.Pop());
        strcpy(pdata->modelName, GetString((IObject*)stack.Pop()));
        strcpy(pdata->textureName, GetString((IObject*)stack.Pop()));
    }

    void ReadLauncherDef(LauncherDef* pdata, ObjectStack& stack)
    {
        ReadBuyableData(pdata, stack);
        pdata->signature = GetNumber((IObject*)stack.Pop());
        pdata->mass = GetNumber((IObject*)stack.Pop());
        pdata->partSize = (PartSize)GetNumber((IObject*)stack.Pop());
    }
    
    void ReadDataExpendableType(DataExpendableTypeIGC* pdata, ObjectStack& stack)
    {
        ReadDataObject(pdata, stack);
        pdata->expendabletypeID = m_expendableTypeIDNext++;
        pdata->loadTime = GetNumber((IObject*)stack.Pop());
        pdata->lifespan = GetNumber((IObject*)stack.Pop());
        pdata->signature = GetNumber((IObject*)stack.Pop());
        pdata->hitPoints = (HitPoints)GetNumber((IObject*)stack.Pop());
        ReadLauncherDef(&(pdata->launcherDef), stack);
    }

    TRef<IObject> CreateAfterburnerType(ObjectStack& stack)
    {
        DataAfterburnerTypeIGC afterburner;

        ReadPartTypeData(&afterburner, ET_Afterburner, stack);
        afterburner.fuelConsumption = GetNumber((IObject*)stack.Pop());
        afterburner.maxThrust = GetNumber((IObject*)stack.Pop());
        afterburner.onRate = GetNumber((IObject*)stack.Pop());
        afterburner.offRate = GetNumber((IObject*)stack.Pop());
        afterburner.thrustMultiplier = GetNumber((IObject*)stack.Pop());
        afterburner.interiorSound = (SoundID)GetNumber((IObject*)stack.Pop());
        afterburner.exteriorSound = (SoundID)GetNumber((IObject*)stack.Pop());

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(
                        Time::Now(), OT_partType,
                        &afterburner, sizeof(afterburner));
        pobject->Release();
        return pobject;
    }

    TRef<IObject> CreateMissileType(ObjectStack& stack)
    {
        DataMissileTypeIGC  missile;

        ReadDataExpendableType(&missile, stack);
        missile.acceleration = GetNumber((IObject*)stack.Pop());
        missile.turnRate = GetNumber((IObject*)stack.Pop());
        missile.initialSpeed = GetNumber((IObject*)stack.Pop());
        missile.armTime = GetNumber((IObject*)stack.Pop());
        missile.lockTime = GetNumber((IObject*)stack.Pop());
        missile.readyTime = GetNumber((IObject*)stack.Pop());
        missile.dispersion = GetNumber((IObject*)stack.Pop());
        missile.lockAngle = GetNumber((IObject*)stack.Pop());
        missile.power = GetNumber((IObject*)stack.Pop());
        missile.blastPower = GetNumber((IObject*)stack.Pop());
        missile.blastRadius = GetNumber((IObject*)stack.Pop());
        missile.width = GetNumber((IObject*)stack.Pop());
        missile.damageType = (DamageTypeMask)GetNumber((IObject*)stack.Pop());
        missile.bDirectional = GetBoolean((IObject*)stack.Pop());
        missile.launchSound = (SoundID)GetNumber((IObject*)stack.Pop());
        missile.ambientSound = (SoundID)GetNumber((IObject*)stack.Pop());

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_missileType, &missile, sizeof(missile));
        pobject->Release();
        return pobject;
    }

    TRef<IObject> CreateMissileLauncherType(ObjectStack& stack)
    {
        DataLauncherTypeIGC launcher;

        launcher.partID = m_partTypeIDNext++;
        launcher.successorPartID = NA;

        TRef<ImissileTypeIGC> pmissileType = (ImissileTypeIGC*)(IObject*)stack.Pop();
        launcher.expendabletypeID = pmissileType->GetObjectID();

        launcher.amount = (short)GetNumber((IObject*)stack.Pop());
        launcher.treasureChance = (short)GetNumber((IObject*)stack.Pop());

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_partType, &launcher, sizeof(launcher));
        pobject->Release();
        return pobject;
    }


    TRef<IObject> CreatePackType(ObjectStack& stack)
    {
        DataPackTypeIGC pack;
        ReadPartTypeData(&pack, ET_Pack, stack);
        pack.amount = (short)GetNumber((IObject*)stack.Pop());
        pack.packType = (PackType)GetNumber((IObject*)stack.Pop());
        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(
                        Time::Now(), OT_partType, &pack, sizeof(pack));
        pobject->Release();
        return pobject;
    }


    TRef<IObject> CreateProjectileType(ObjectStack& stack)
    {
        DataProjectileTypeIGC projectile;
        projectile.projectileTypeID = m_projectileTypeIDNext++;
        ReadDataObject(&projectile, stack);
        projectile.power = GetNumber((IObject*)stack.Pop());
        projectile.blastPower = GetNumber((IObject*)stack.Pop());
        projectile.blastRadius = GetNumber((IObject*)stack.Pop());
        projectile.speed = GetNumber((IObject*)stack.Pop());
        projectile.lifespan = GetNumber((IObject*)stack.Pop());
        projectile.damageType = (DamageTypeMask)GetNumber((IObject*)stack.Pop());
        projectile.absoluteF = GetBoolean((IObject*)stack.Pop());
        projectile.bDirectional = GetBoolean((IObject*)stack.Pop());
        projectile.width = GetNumber((IObject*)stack.Pop());
        projectile.ambientSound = (SoundID)GetNumber((IObject*)stack.Pop());

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_projectileType, &projectile, sizeof(projectile));
        pobject->Release();
        return pobject;
    }

    TRef<IObject> CreateStationType(ObjectStack& stack)
    {
        DataStationTypeIGC  st;
        memset(&st, 0, sizeof(st));
        st.stationTypeID = m_stationTypeIDNext++;
        st.maxArmorHitPoints = 5000.0f;
        st.maxShieldHitPoints = 250.0f;
        st.armorRegeneration = 10.0f;
        st.shieldRegeneration = 5.0f;
        strcpy(st.modelName, "ss27");
        strcpy(st.iconName, "stationbmp");
        strcpy(st.name, "Mining Platform");
        strcpy(st.description, "Mining Platform");
        st.price = 1000;
        st.timeToBuild = 100;
        st.textureName[0] = '\0';
        st.radius = 125;
        st.animIdle.start = 0.0f;
        st.animIdle.stop = 40.0f;
        st.ttbmRequired.ClearAll();
        st.ttbmEffects.ClearAll();
        st.ttbmLocal.ClearAll();
        st.signature = 2.5f;
        st.scannerRange = 2000.0f;
        st.aabmBuild = (c_aabmSpecial << 0); 
        st.successorStationTypeID = NA;
        st.sabmCapabilities = c_sabmUnload | c_sabmStart | c_sabmRestart | c_sabmLand | c_sabmRepair | c_sabmLoadoutMenu | c_sabmRipcord | c_sabmReload;
        st.interiorSound = 380;
        st.exteriorSound = 420;
        st.interiorAlertSound = 460;

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_stationType, &st, sizeof(st));
        pobject->Release();
        return pobject;

    }

    TRef<IObject> CreateStation(ObjectStack& stack)
    {
        DataStationIGC ds;

        // get the clusterid
        TRef<IclusterIGC> pcluster; CastTo(pcluster, (IObject*)stack.Pop());
        ds.clusterID = pcluster->GetObjectID();

       // get the station type
        TRef<IstationTypeIGC> pstationType; CastTo(pstationType, (IObject*)stack.Pop());
        ds.stationTypeID = pstationType->GetObjectID();
        strcpy(ds.name, pstationType->GetName());

        ds.position = GetVector((IObject*)stack.Pop());
        ds.forward = Vector::RandomDirection();
        static const Vector xAxis(1.0, 0.0, 0.0);
        ds.up = CrossProduct(ds.forward, xAxis);
        if (ds.up.LengthSquared() < 0.1f)
            {
            static const Vector zAxis(0.0, 0.0, 1.0);
            ds.up = CrossProduct(ds.forward, zAxis);
            }
        assert (ds.up.LengthSquared() > 0.1f);
        ds.up.SetNormalize();
        ds.rotation.axis(ds.forward);
        ds.rotation.angle(0.05f + random(0.0f, 0.05f));

        ds.sideID = (SideID)GetNumber((IObject*)stack.Pop());
        ds.stationID = m_pmissionContext->GetMissionIGC()->GenerateNewStationID();

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_station, &ds, sizeof(ds));
        pobject->Release();
        return pobject;
    }

    TRef<IObject> CreateHullType(ObjectStack& stack)
    {
        BYTE rgbData[4096];
        DataHullTypeIGC* pHullType = (DataHullTypeIGC*)rgbData;
        memset(pHullType, 0, sizeof(DataHullTypeIGC));

        pHullType->hullID = m_hullIDNext++;
        pHullType->successorHullID = NA;

        ReadBuyableData(pHullType, stack);

        pHullType->mass = GetNumber((IObject*)stack.Pop());
        pHullType->signature = GetNumber((IObject*)stack.Pop());
        pHullType->speed = GetNumber((IObject*)stack.Pop());
        for (int i = 0; i < 3; i++)
            pHullType->maxTurnRates[i] = GetNumber((IObject*)stack.Pop());    //yaw, pitch, roll
        for (i = 0; i < 3; i++)
            pHullType->turnTorques[i] = GetNumber((IObject*)stack.Pop());      //yaw, pitch, roll
        pHullType->thrust = GetNumber((IObject*)stack.Pop());
        pHullType->sideMultiplier = GetNumber((IObject*)stack.Pop());
        pHullType->backMultiplier = GetNumber((IObject*)stack.Pop());
        pHullType->scannerRange = GetNumber((IObject*)stack.Pop());
        pHullType->maxEnergy = GetNumber((IObject*)stack.Pop());
        pHullType->rechargeRate = GetNumber((IObject*)stack.Pop());
        pHullType->maxWeapons = (Mount)GetNumber((IObject*)stack.Pop());
        pHullType->hitPoints = (HitPoints)GetNumber((IObject*)stack.Pop());
        memset(pHullType->psEquipment, 0, sizeof(pHullType->psEquipment));
        pHullType->length = (short)GetNumber((IObject*)stack.Pop());
        pHullType->habmCapabilities = (HullAbilityBitMask)GetNumber((IObject*)stack.Pop());
        strcpy(pHullType->textureName, GetString((IObject*)stack.Pop()));

        pHullType->interiorSound = (SoundID)GetNumber((IObject*)stack.Pop());
        pHullType->exteriorSound = (SoundID)GetNumber((IObject*)stack.Pop());
        pHullType->mainThrusterInteriorSound = (SoundID)GetNumber((IObject*)stack.Pop());
        pHullType->mainThrusterExteriorSound = (SoundID)GetNumber((IObject*)stack.Pop());
        pHullType->manuveringThrusterInteriorSound = (SoundID)GetNumber((IObject*)stack.Pop());
        pHullType->manuveringThrusterExteriorSound = (SoundID)GetNumber((IObject*)stack.Pop());

        AnimationData* panimationData = pHullType->animations;
        TRef<IObjectList> plistAnimationData; CastTo(plistAnimationData, (IObject*)stack.Pop());
        plistAnimationData->GetFirst();
        while (NULL != plistAnimationData->GetCurrent())
            {
            TRef<IObjectPair> pparams; CastTo(pparams, (IObject*)plistAnimationData->GetCurrent());
            panimationData->start = GetNumber((IObject*)pparams->GetFirst());
            panimationData->stop = GetNumber((IObject*)pparams->GetSecond());
            panimationData++;
            plistAnimationData->GetNext();
            }

        pHullType->hardpointOffset = sizeof(DataHullTypeIGC);
        HardpointData* pHardPoint = (HardpointData *) (pHullType + 1);
        int cHardpoints = 0;
        TRef<IObjectList> plistHardPoints; CastTo(plistHardPoints, (IObject*)stack.Pop());
        plistHardPoints->GetFirst();
        while (NULL != plistHardPoints->GetCurrent())
            {
            TRef<IObjectPair> pparams; CastTo(pparams, (IObject*)plistHardPoints->GetCurrent());
            strcpy(pHardPoint->frameName, GetString((IObject*)pparams->GetFirst()));

            //NYI needs to be correctly initialized
            pHardPoint->bFixed = true;
            pHardPoint->minDot = (char)GetNumber((IObject*)pparams->GetSecond()) == 'n' ? 0.5f : 0.0f;

            cHardpoints++;
            pHardPoint++;
            plistHardPoints->GetNext();
            }
        

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_hullType, rgbData, sizeof(DataHullTypeIGC) + sizeof(HardpointData)*cHardpoints);
        pobject->Release();
        return pobject;
    }

    TRef<IObject> CreateWeaponType(ObjectStack& stack)
    {
        DataWeaponTypeIGC weapon;
        ReadPartTypeData(&weapon, ET_Weapon, stack);
        weapon.dtimeReady = GetNumber((IObject*)stack.Pop());
        weapon.dtimeBurst = GetNumber((IObject*)stack.Pop());
        weapon.energyPerShot = GetNumber((IObject*)stack.Pop());
        weapon.dispersion = GetNumber((IObject*)stack.Pop());
        weapon.cAmmoPerShot = (short)GetNumber((IObject*)stack.Pop());
        TRef<IprojectileTypeIGC> pprojectileType = (IprojectileTypeIGC*)(IObject*)stack.Pop();
        weapon.projectileTypeID = pprojectileType->GetObjectID();
        weapon.activateSound = (short)GetNumber((IObject*)stack.Pop());
        weapon.singleShotSound = (short)GetNumber((IObject*)stack.Pop());
        weapon.burstSound = (short)GetNumber((IObject*)stack.Pop());
        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_partType, &weapon, sizeof(weapon));
        pobject->Release();
        return pobject;
    }


    TRef<IObject> CreateCluster(ObjectStack& stack)
    {
        DataClusterIGC data;
        memset(&data, 0, sizeof(data));

        // assign a new cluster id
        data.clusterID = m_clusterIDNext++;

        // name, etc
        strcpy(data.name, GetString((IObject*)stack.Pop()));
        strcpy(data.posterName, GetString((IObject*)stack.Pop()));
        
         // lighting
        data.lightDirection = Vector::RandomDirection(); //GetVector((IObject*)stack.Pop());
        data.lightColor = 0xffffffff; //D3DRGBA(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());


        // positioning
        data.screenX    = GetNumber((IObject*)stack.Pop());
        data.screenY    = GetNumber((IObject*)stack.Pop());

        // properties
        data.starSeed = rand();
        data.nDebris = randomInt(250, 750);
        data.nStars = randomInt(250, 750);
        data.activeF = false;

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_cluster, &data, sizeof(data));
        pobject->Release();
        return pobject;
    }

    TRef<IObject> CreateSphericalRegion(ObjectStack& stack)
    {
        TRef<ImodelIGC> pmodel; CastTo(pmodel, (IObject*)stack.Pop());
        float radius = GetNumber((IObject*)stack.Pop());
        return (Region*)SphericalRegion::Create(pmodel, radius);
    }

    TRef<Region> GetRegion(const ZString& strRegion)
    {
        ZAssert(m_pnsClusters);
        return (Region*)m_pnsClusters->FindMember(strRegion);
    }

    /*
    TRef<IObject> CreateDrone(ObjectStack& stack)
    {    
        // cluster
        TRef<IclusterIGC> pcluster; CastTo(pcluster, (IObject*)stack.Pop());
        // position
        Vector pos = GetVector((IObject*)stack.Pop());
        // name
        ZString strName = GetString((IObject*)stack.Pop());
        //dronetype
        DroneType dt = (DroneType)(WORD)GetNumber((IObject*)stack.Pop());
        //side
        SideID sideID = (SideID)GetNumber((IObject*)stack.Pop());

        // loadout
        TRef<IObjectList> pparamList; CastTo(pparamList, (IObject*)stack.Pop());
        // hulltype
        TRef<IhullTypeIGC> phullType = (IhullTypeIGC*)pparamList->GetFirst();
        HullID hullID = phullType->GetObjectID();
        // parts
        TRef<IObjectList> ppartsList =  (IObjectList*)pparamList->GetNext();
        PartData partData[10];
        int cParts = ReadPartData(phullType, partData, ppartsList);

        Drone* pdrone = g_drones.Create(
                        m_pmissionContext->GetMissionIGC(), 
                        dt,
                        strName,
                        hullID,
                        m_pmissionContext->GetMissionIGC()->GetSide(sideID),
                        cParts, 
                        partData);
        pdrone->GetShip()->SetPosition(pos);
        pdrone->GetShip()->SetCluster(pcluster);
        
        return pdrone->GetShip();
    }
    */


    TRef<IObject> CreateAsteroid(ObjectStack& stack)
    {
        DataAsteroidIGC data;
        memset(&data, 0, sizeof(data));

        // get the clusterid
        TRef<IclusterIGC> pcluster; CastTo(pcluster, (IObject*)stack.Pop());
        data.clusterID = pcluster->GetObjectID();

        // assign a new asteroid id
        data.asteroidDef.asteroidID = m_asteroidIDNext++;

        // name, texture, model
        strcpy(data.name, GetString((IObject*)stack.Pop()));
        strcpy(data.asteroidDef.modelName, GetString((IObject*)stack.Pop()));
        strcpy(data.asteroidDef.textureName, GetString((IObject*)stack.Pop()));
        
        // properties
        data.mass                   = GetNumber((IObject*)stack.Pop());
        data.asteroidDef.radius     = (short)GetNumber((IObject*)stack.Pop());
        data.asteroidDef.hitpoints  = (short)GetNumber((IObject*)stack.Pop());
        data.asteroidDef.ore        = GetNumber((IObject*)stack.Pop());

        // positioning
        data.position = GetVector((IObject*)stack.Pop());
        data.forward = Vector::RandomDirection();
        static const Vector xAxis(1.0, 0.0, 0.0);
        data.up = CrossProduct(data.forward, xAxis);
        if (data.up.LengthSquared() < 0.1f)
            {
            static const Vector zAxis(0.0, 0.0, 1.0);
            data.up = CrossProduct(data.forward, zAxis);
            }
        assert (data.up.LengthSquared() > 0.1f);
        data.up.SetNormalize();
        data.rotation.axis(data.forward);
        data.rotation.angle(0.05f + random(0.0f, 0.05f));

/*      data.position   = GetVector((IObject*)stack.Pop());
        data.up         = GetVector((IObject*)stack.Pop());
        data.forward    = GetVector((IObject*)stack.Pop());
        TRef<IObjectPair> ppair; CastTo(ppair, (IObject*)stack.Pop());
        data.rotation   = Rotation(
                            GetVector(ppair->GetFirst()),
                            GetNumber(ppair->GetSecond()));
*/

        // capabilites
        TRef<IObjectList> plist; CastTo(plist, (IObject*)stack.Pop());
        plist->GetFirst();
        while (NULL != plist->GetCurrent())
            {
            data.asteroidDef.aabmCapabilities |= (short)GetNumber((IObject*)plist->GetCurrent());
            plist->GetNext();
            }

        IObject* pobject = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(),
                        OT_asteroid, &data, sizeof(data));
        pobject->Release();
        return pobject;
    }

    void CreateDefaults()
    {
        IObject* pobj;

        //
        // Default Hull Type (EVA pod).
        //
        DataHullTypeIGC     hullType;
        memset(&hullType, 0, sizeof(hullType));

        hullType.price = 0;
        strcpy(hullType.modelName, "utl22");
        strcpy(hullType.iconName, "fighterbmp");
        strcpy(hullType.name, "EVA Pod");
        strcpy(hullType.description, "Last hope home");
        hullType.ttbmRequired.ClearAll();
        hullType.ttbmEffects.ClearAll();

        hullType.mass = 2;
        hullType.signature = 50;
        hullType.speed = 20;
        hullType.maxTurnRates[0] = hullType.maxTurnRates[1] = pi / 2.0f;
        hullType.maxTurnRates[2] = 4.0f * pi / 9.0f;
        hullType.turnTorques[0] = hullType.turnTorques[1] = 3.0f * pi / 2.0f;
            hullType.turnTorques[2] = 4.0f * pi / 3.0f;
        hullType.thrust = hullType.mass * 10;
        hullType.sideMultiplier = 1.0f;
        hullType.backMultiplier = 1.0f;
        hullType.scannerRange = 1000;
        hullType.maxEnergy = 0;
        hullType.rechargeRate = 0;
        hullType.hullID = m_hullIDNext++;
        hullType.maxWeapons = 0;
        hullType.hitPoints = 50;
        memset(hullType.psEquipment, 0, sizeof(hullType.psEquipment));

        hullType.hardpointOffset = sizeof(hullType);
        hullType.length = 4;
        hullType.habmCapabilities = 29239;
        hullType.interiorSound = 140;
        hullType.exteriorSound = 180;
        hullType.mainThrusterInteriorSound = 220;
        hullType.mainThrusterExteriorSound = 260;
        hullType.manuveringThrusterInteriorSound = 300;
        hullType.manuveringThrusterExteriorSound = 340;

        pobj = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(), OT_hullType, &hullType, sizeof(hullType));
        assert (pobj);
        pobj->Release();

        // Default Station Type
        DataStationTypeIGC  st;
        st.stationTypeID = 0;
        st.maxArmorHitPoints = 500.0f;
        st.maxShieldHitPoints = 250.0f;
        st.armorRegeneration = 1.0f;
        st.shieldRegeneration = 5.0f;
        strcpy(st.modelName, "acs05");
        strcpy(st.iconName, "stationbmp");
        strcpy(st.name, "Station");
        strcpy(st.description, "Station");
        st.price = 1000;
        st.timeToBuild = 100;
        st.textureName[0] = '\0';
        st.radius = 125;
        st.animIdle.start = 0.0f;
        st.animIdle.stop = 40.0f;
        st.ttbmRequired.ClearAll();
        st.ttbmEffects.ClearAll();
        st.ttbmLocal.ClearAll();
        st.signature = 2.5f;
        st.scannerRange = 2000.0f;
        st.aabmBuild = (c_aabmSpecial << 0); 
        st.successorStationTypeID = NA;
        st.sabmCapabilities = c_sabmUnload | c_sabmStart | c_sabmRestart | c_sabmLand | c_sabmRepair | c_sabmLoadoutMenu | c_sabmRipcord | c_sabmReload;
        st.interiorSound = 380;
        st.exteriorSound = 420;
        st.interiorAlertSound = 460;
        pobj = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(), OT_stationType, &st, sizeof(st));
        assert (pobj);
        pobj->Release();

        /*
        // Construction Drone
        DataDroneTypeIGC   d;
        d.timeToBuild = 30;
        d.droneTypeID = 0;
        d.hullTypeID = hullType.hullID;
        d.droneType = c_dtConstruction;
        d.moveSkill = d.shootSkill = 1.0f;
        d.bravery = 0.5f;
        d.nParts = 0;
        d.price = 500;
        strcpy(d.modelName, "utl19");
        d.iconName[0] = '\0';
        strcpy(d.name, "Construction");
        strcpy(d.description, "Construction drone");
        d.ttbmRequired.ClearAll();
        d.ttbmEffects.ClearAll();
        pobj = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(), OT_droneType, &d, sizeof(d));
        assert (pobj);
        pobj->Release();
        */

        // Create the 3 civilizations
        static const char* civNames[c_cSidesMax] =
                            {"Rix", "Iron League", "Belorian"};

        DataCivilizationIGC c;
        c.lifepod = hullType.hullID;
        c.constructionDroneTypeID = 0;
        c.ttbmBaseTechs.ClearAll();
        c.gasBaseAttributes.Initialize();

        for (CivID cid = 0; (cid < 3); cid++)
            {
            strcpy(c.name, "Iron league");
            c.initialStationTypeID = 0;
            c.civilizationID = cid;
            pobj = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(), OT_civilization, &c, sizeof(c));
            assert (pobj);
            pobj->Release();
            }

        // Create the 3 teams

        static const float sideColors[3][3] =
                                    { {1.0f, 0.5f, 0.0f}, //orange
                                      {0.0f, 1.0f, 0.67f},//teal
                                      {0.0f, 0.0f, 1.0f}}; //blue

        static const char* sideNames[c_cSidesMax] =
                            {"Orange", "Teal", "Blue"};

        for (SideID sid = 0; (sid < 3); sid++)
            {
            IcivilizationIGC* pcivilization = m_pmissionContext->GetMissionIGC()->GetCivilization(sid);

            DataSideIGC ds;
            ds.sideID = sid;
            ds.gasAttributes.Initialize();
            ds.civilizationID = sid;
            ds.color.SetRGBA(sideColors[sid][0],
                             sideColors[sid][1],
                             sideColors[sid][2]);
            strcpy(ds.name, sideNames[sid]);
            ds.gas = pcivilization->GetBaseAttributes();
            ds.ttbmDevelopmentTechs = pcivilization->GetBaseTechs();
            ds.squadID = NA;
            ds.nKills = ds.nDeaths = ds.nBaseKills = ds.nBaseCaptures = ds.nFlags = ds.nArtifacts = 0;
            pobj = m_pmissionContext->GetMissionIGC()->CreateObject(Time::Now(), OT_side, &ds, sizeof(ds));
            assert (pobj);
            pobj->Release();
            }
    }
    

    void SetMissionStage(MissionStage* pstageNew)
    {
        FireOnSwitchedMissionStage(pstageNew, m_pmissionStage);
        m_pmissionStage = pstageNew;
    }
    
    void AdviseRegion(Region* pregion, ImodelIGC* pmodel)
    {
        TRef<RegionAdviseEntry> pentry;
        if (!m_mapRegionAdviseEntries.Find(pregion, pentry))
            {
            pentry = new RegionAdviseEntry(pregion);
            m_mapRegionAdviseEntries.Set(pregion, pentry);
            }
        pentry->Advise(pmodel);
        // todo: make sure not in delete list
    }


    void UnadviseRegion(Region* pregion, ImodelIGC* pmodel)
    {
        TRef<RegionAdviseEntry> pentry;
        ZVerify(m_mapRegionAdviseEntries.Find(pregion, pentry));
        pentry->Unadvise(pmodel);
        // todo: mark this entry to be deleted
    }
    
    void AddMissionEventHandler(MissionEventHandler* phandler)
    {
        m_lstEventHandlers.PushEnd(phandler);
    }

    void RemoveMissionEventHandler(MissionEventHandler* phandler)
    {
        m_lstEventHandlers.Replace(phandler, NULL);
    }
 
    void Update(Time time, float dtime)
    {
        // check region collisions... this doesn't handle jumps all the way through a region
        TMap<TRef<Region>, TRef<RegionAdviseEntry> >::Iterator iterEntries(m_mapRegionAdviseEntries);
        while (!iterEntries.End())
            {
            TRef<RegionAdviseEntry> pentry = iterEntries.Value();
            pentry->Update(this);           
            iterEntries.Next();
            }
    }

    void RemoveDeadHandlers()
    {
        // remove all of the NULL elements from the list
        while (m_lstEventHandlers.Remove(NULL)) 
            {
            }
    }

    #define ForEachHandler(fnc) \
        TList<TRef<MissionEventHandler> >::Iterator iter(m_lstEventHandlers); \
        while (!iter.End()) \
            { \
            if (iter.Value() != NULL) \
                if (!iter.Value()->fnc) \
                    RemoveMissionEventHandler(iter.Value()); \
            iter.Next(); \
            } \
        RemoveDeadHandlers(); \
        


    void FireOnModelTerminated(ImodelIGC* pmodel)
    {
        ForEachHandler( OnModelTerminated(pmodel) )
    }

    void FireOnModelKilled(ImodelIGC* pmodel)
    {
        ForEachHandler( OnModelKilled(pmodel) )
    }


    void FireOnModelDamaged(ImodelIGC* pmodel)
    {
        ForEachHandler( OnModelDamaged(pmodel) )
    }


    void FireOnDocked(IshipIGC* pship, IstationIGC* pstation)
    {
        ForEachHandler( OnDocked(pship, pstation) )
    }


    void FireOnSwitchSector(IshipIGC* pship, IclusterIGC* pclusterOld, IclusterIGC* pclusterNew)
    {
        ForEachHandler( OnSwitchSector(pship, pclusterOld, pclusterNew) )
    }


    void FireOnHitTreasure(IshipIGC* pship, ItreasureIGC* ptreasure)
    {
        ForEachHandler( OnHitTreasure(pship, ptreasure) )
    }


    void FireOnEnteredRegion(ImodelIGC* pmodel, Region* pregion)
    {
        ForEachHandler( OnEnteredRegion(pmodel, pregion) )
    }


    void FireOnExitedRegion(ImodelIGC* pmodel, Region* pregion)
    {
        ForEachHandler( OnExitedRegion(pmodel, pregion) )
    }


    void FireOnDivergedPath(IshipIGC* pship, Path* ppath, float distance, float angle)
    {
        ForEachHandler( OnDivergedPath(pship, ppath, distance, angle) )
    }


    void FireOnSwitchedMissionStage(MissionStage* pstageNew, MissionStage* pstageOld)
    {
        ForEachHandler( OnSwitchedMissionStage(pstageNew, pstageOld) )
    }


};


TRef<MissionManager> MissionManager::Create(MissionContext* pcontext)
{
    return new MissionManagerImpl(pcontext);
}


#endif
