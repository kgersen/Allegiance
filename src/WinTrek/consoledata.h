/////////////////////////////////////////////////////////////////////////////
//
// TMemberSnapshotValue
//
/////////////////////////////////////////////////////////////////////////////

template<class ObjectClass, class StaticType>
class TMemberSnapshotValue : public TStaticValue<StaticType>, 
                             public EventTargetContainer<TMemberSnapshotValue<ObjectClass, StaticType> >
{
private:

    typedef StaticType (ObjectClass::*PFNMember)();

    TRef<ObjectClass> m_pobject;
    PFNMember m_pfn;

public:
    TMemberSnapshotValue(ObjectClass* pobject, 
            PFNMember pfn, IEventSource* peventSource, 
            const StaticType& value) :
        TStaticValue<StaticType>(value),
        m_pobject(pobject),
        m_pfn(pfn)
    {
        ZAssert(pobject != NULL);
        ZAssert(pfn != NULL);
		// mdvalley: Needs pointer and class.
        AddEventTarget(&TMemberSnapshotValue<ObjectClass,StaticType>::OnEvent, peventSource);
    }

    bool OnEvent()
    {
        StaticType value = (m_pobject->*m_pfn)();
        if (GetValueInternal() != value)
            {
            GetValueInternal() = value;
            Changed();
            }
        return true;
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// TMemberSnapshotValueFactory
//
/////////////////////////////////////////////////////////////////////////////

template<class ObjectClass, class StaticType>
class TMemberSnapshotValueFactory : public IFunction
{
private:
    typedef StaticType (ObjectClass::*PFNMember)();
    typedef TMemberSnapshotValue<ObjectClass, StaticType> MemberSnapshotValue;
    
    PFNMember m_pfn;
    StaticType m_value;

public:
    TMemberSnapshotValueFactory(PFNMember pfn, const StaticType& value) :
        m_pfn(pfn),
        m_value(value)
    {
    }

    TRef<IObject> Apply(ObjectStack& stack)
    {
        TRef<ObjectClass> pobject; CastTo(pobject,            (IObject*)stack.Pop());
        TRef<IEventSource> peventSource; CastTo(peventSource, (IObject*)stack.Pop());

        return (TStaticValue<StaticType>*) new MemberSnapshotValue(pobject, m_pfn, peventSource, m_value);
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// ModelWrapper
//
/////////////////////////////////////////////////////////////////////////////

class ModelWrapper : public TrekClientEventSink
{
protected:
    TRef<ImodelIGC>       m_pmodel;
    TRef<EventSourceImpl> m_peventSourceOnChange;
    
public:
    ModelWrapper() :
        m_peventSourceOnChange(new EventSourceImpl())
    {
    }
        
    ModelWrapper(ImodelIGC* pmodel) : 
        m_pmodel(pmodel),
        m_peventSourceOnChange(new EventSourceImpl())
    {
    }

    ImodelIGC* GetModel()
    {
        return m_pmodel;
    }

    void SetModel(ImodelIGC* pmodel)
    {
        if (m_pmodel == pmodel)
            return;
        m_pmodel = pmodel;
        m_peventSourceOnChange->Trigger();
    }

    void OnModelTerminated(ImodelIGC* pmodel)
    {
        if (pmodel == m_pmodel)
            SetModel(NULL);            
    }
    
    void OnTechTreeChanged(SideID sid)
    {
        if (m_pmodel && m_pmodel->GetSide() && m_pmodel->GetSide()->GetObjectID() == sid 
            && m_pmodel->GetObjectType() == OT_station)
        {
            // this might have been a station upgrade which effected my model
            m_peventSourceOnChange->Trigger();
        }
    }

    IEventSource* GetOnChangeEventSource()
    {
        return m_peventSourceOnChange;
    }
};


/////////////////////////////////////////////////////////////////////////////
//
// ModelData
//
/////////////////////////////////////////////////////////////////////////////

class ModelData : public ModelWrapper
{
private:
    IshipIGC* GetShip()
        { ZAssert(GetModelTypeInternal() == OT_ship);  return ((IshipIGC*)(ImodelIGC*)m_pmodel)->GetSourceShip(); }
    IstationIGC* GetStation()
        { ZAssert(GetModelTypeInternal() == OT_station);  return (IstationIGC*)(ImodelIGC*)m_pmodel; }
    IasteroidIGC* GetAsteroid()
        { ZAssert(GetModelTypeInternal() == OT_asteroid);  return (IasteroidIGC*)(ImodelIGC*)m_pmodel; }
    IprobeIGC* GetProbe()
        { ZAssert(GetModelTypeInternal() == OT_probe);  return (IprobeIGC*)(ImodelIGC*)m_pmodel; }
    ObjectType  GetModelTypeInternal();
    
public:    
    ModelData(){};
    ModelData(ImodelIGC* pmodel) : ModelWrapper(pmodel) {};

    ImodelIGC*  GetModel();

    float       GetModelType();
    ZString     GetModelTypeDesc();
    TRef<Image> GetModelTypeIcon();
    ZString     GetName();
    ZString     GetSectorName();
    ZString     GetSideName();
    Color       GetSideColor();
    TRef<Image> GetSideIcon();
    float       GetSpeed();
    float       GetMass();
    float       GetRange();
    float       GetPercentHitPoints();
    float       GetPercentShields();
    float       GetPercentEnergy();
    float       GetAmmo();
    float       GetFuel();
    float       GetOre();
    float       GetVectorLock();
    float       GetCloaking();
    float       GetSignature();
    float       GetNumObservers();
    float       GetRipcordTimeLeft();
    float       GetEndurance();
    float       IsCloaked();
    float       IsEjectPod();
    float       IsRipcording();
    bool        IsVisible();
    bool        IsNotNull();
	float       GetX(); //Andon
	float       GetY(); //Andon
	float       GetZ(); //Andon
	float		IDTarget(); //Andon
	float		GetMaxThrust(); //Andon
	float		GetMaxHitPoints(); //Andon
	float		GetNumHitPoints(); //Andon
	float		GetMaxShields(); //Andon
	float		GetNumShields(); //Andon
	float		GetMaxEnergy(); //Andon
	float		GetNumEnergy(); //Andon
	float		GetMaxAmmo(); //Andon
	float		GetNumAmmo(); //Andon
	float		GetMaxFuel(); //Andon
	float		GetNumFuel(); //Andon
	float		GetScanRange(); //Andon

    static void ExportAccessors(INameSpace* pns)
    {
		// mdvalley: many many pointers and class names now
        pns->AddMember("GetModelType",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetModelType, (float)OT_invalid));
        pns->AddMember("GetModelTypeDesc",  new TMemberSnapshotValueFactory<ModelData, ZString>(&ModelData::GetModelTypeDesc, ""));
//        pns->AddMember("GetModelTypeIcon",  new TMemberSnapshotValueFactory<ModelData, TRef<Image> >(GetModelTypeIcon));
        pns->AddMember("GetName",  new TMemberSnapshotValueFactory<ModelData, ZString>(&ModelData::GetName, ""));
        pns->AddMember("GetSectorName",  new TMemberSnapshotValueFactory<ModelData, ZString>(&ModelData::GetSectorName, ""));
        pns->AddMember("GetSideName",  new TMemberSnapshotValueFactory<ModelData, ZString>(&ModelData::GetSideName, ""));
        pns->AddMember("GetSideColor",  new TMemberSnapshotValueFactory<ModelData, Color>(&ModelData::GetSideColor, Color(255,255,255)));
//        pns->AddMember("GetSideIcon",  new TMemberSnapshotValueFactory<ModelData, TRef<Image> >(GetSideIcon));
		pns->AddMember("GetSpeed",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetSpeed, 0.0f));
        pns->AddMember("GetMass",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMass, 0.0f));
        pns->AddMember("GetRange",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRange, 0.0f));
        pns->AddMember("GetPercentHitPoints",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetPercentHitPoints, 0.0f));
        pns->AddMember("GetPercentShields",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetPercentShields, 0.0f));
        pns->AddMember("GetPercentEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetPercentEnergy, 0.0f));
        pns->AddMember("GetAmmo",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetAmmo, 0.0f));
        pns->AddMember("GetFuel",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetFuel, 0.0f));
        pns->AddMember("GetOre",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetOre, 0.0f));
        pns->AddMember("GetVectorLock",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetVectorLock, 0.0f));
        pns->AddMember("GetCloaking",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetCloaking, 0.0f));
        pns->AddMember("GetNumObservers",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumObservers, 0.0f));
        pns->AddMember("GetSignature",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetSignature, 0.0f));
        pns->AddMember("GetRipcordTimeLeft",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRipcordTimeLeft, 0.0f));
        pns->AddMember("GetEndurance",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetEndurance, 1.0f));
        pns->AddMember("IsCloaked",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsCloaked, 0.0f));
        pns->AddMember("IsEjectPod",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsEjectPod, 0.0f));
        pns->AddMember("IsRipcording",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsRipcording, 0.0f));
        pns->AddMember("IsVisible", new TMemberSnapshotValueFactory<ModelData, bool>(&ModelData::IsVisible, false));
        pns->AddMember("IsNotNull", new TMemberSnapshotValueFactory<ModelData, bool>(&ModelData::IsNotNull, false));
		pns->AddMember("GetX",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetX, 0.0f)); //Andon
        pns->AddMember("GetY",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetY, 0.0f)); //Andon
        pns->AddMember("GetZ",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetZ, 0.0f)); //Andon
		pns->AddMember("IDTarget",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IDTarget, 0.0f)); //Andon
		pns->AddMember("GetMaxThrust",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxThrust, 0.0f)); //Andon
		pns->AddMember("GetNumEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumEnergy, 0.0f)); //Andon
		pns->AddMember("GetMaxEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxEnergy, 0.0f)); //Andon
		pns->AddMember("GetNumHitPoints",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumHitPoints, 0.0f)); //Andon
		pns->AddMember("GetMaxHitPoints",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxHitPoints, 0.0f)); //Andon
		pns->AddMember("GetNumShields",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumShields, 0.0f)); //Andon
		pns->AddMember("GetMaxShields",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxShields, 0.0f)); //Andon
		pns->AddMember("GetNumAmmo",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumAmmo, 0.0f)); //Andon
		pns->AddMember("GetMaxAmmo",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxAmmo, 0.0f)); //Andon
		pns->AddMember("GetNumFuel",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumFuel, 0.0f)); //Andon
		pns->AddMember("GetMaxFuel",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxFuel, 0.0f)); //Andon
		pns->AddMember("GetScanRange",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetScanRange, 0.0f)); //Andon
    }

};

/////////////////////////////////////////////////////////////////////////////
//
// PartWrapper
//
/////////////////////////////////////////////////////////////////////////////

class PartWrapper : public IObject
{
protected:
    TRef<IpartIGC>          m_ppart;
    TRef<EventSourceImpl>   m_peventSourceOnChange;

private:

public:
    PartWrapper() :
        m_peventSourceOnChange(new EventSourceImpl())
    {
    }
        
    PartWrapper(IpartIGC* ppart) : 
        m_ppart(ppart),
        m_peventSourceOnChange(new EventSourceImpl())
    {
    }

    IpartIGC* GetPart()
    {
        return m_ppart;
    }

    void SetPart(IpartIGC* ppart)
    {
        if (m_ppart == ppart)
            return;
        m_ppart = ppart;
        m_peventSourceOnChange->Trigger();
    }

    IEventSource* GetOnChangeEventSource()
    {
        return m_peventSourceOnChange;
    }

    ZString     GetPartName();
    float       GetCount();
    float       GetRange();
    float       GetDamage();
	float		GetRate();
    float       GetAfterburnerFuelConsumption();
    float       GetAfterburnerFuelLeft();
    float       GetAfterburnerTopSpeed();
	float       GetAfterburnerTimeLeft();
	float		GetMaxShieldStrength();
	float		GetShieldStrength();
    float       GetRegenRate();
	float		GetRechargeTime();
	float		GetCloakTimeLeft();
    float       IsEnergyDamage();
    float       IsShipKiller();
    float       IsStationKiller();
    float       IsAsteroidKiller();
    float       IsSelected();
    float       IsActive();
    float       IsOutOfAmmo();
    float       GetReadyState();
    float       GetMountedFraction();
    float       GetArmedFraction();

    static void ExportAccessors(INameSpace* pns)
    {
		// mdvalley: like earlier, pointers and class names
        pns->AddMember("GetPartName",  new TMemberSnapshotValueFactory<PartWrapper, ZString>(&PartWrapper::GetPartName, ""));
        pns->AddMember("GetCount",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetCount, 1.0f));
        pns->AddMember("GetWeaponRange",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetRange, 0.0f));
        pns->AddMember("GetDamage",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetDamage, 0.0f));
		pns->AddMember("GetRate",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetRate, 0.0f));
        pns->AddMember("GetAfterburnerFuelConsumption",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetAfterburnerFuelConsumption, 0.0f));
        pns->AddMember("GetAfterburnerFuelLeft",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetAfterburnerFuelLeft, 0.0f));
        pns->AddMember("GetAfterburnerTopSpeed",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetAfterburnerTopSpeed, 0.0f));
		pns->AddMember("GetAfterburnerTimeLeft",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetAfterburnerTimeLeft, 0.0f));
        pns->AddMember("GetMaxShieldStrength",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetMaxShieldStrength, 0.0f));
		pns->AddMember("GetShieldStrength",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetShieldStrength, 0.0f));
		pns->AddMember("GetRegenRate",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetRegenRate, 0.0f));
        pns->AddMember("GetRechargeTime",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetRechargeTime, 0.0f));
		pns->AddMember("GetCloakTimeLeft",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetCloakTimeLeft, 0.0f));
		pns->AddMember("IsEnergyDamage",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsEnergyDamage, 0.0f));
        pns->AddMember("IsSelected",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsSelected, 0.0f));
        pns->AddMember("IsShipKiller",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsShipKiller, 0.0f));
        pns->AddMember("IsStationKiller",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsStationKiller, 0.0f));
        pns->AddMember("IsAsteroidKiller",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsAsteroidKiller, 0.0f));
        pns->AddMember("IsActive",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsActive, 0.0f));
        pns->AddMember("IsOutOfAmmo",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::IsOutOfAmmo, 0.0f));
        pns->AddMember("GetReadyState",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetReadyState, 0.0f));
        pns->AddMember("GetMountedFraction",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetMountedFraction, 0.0f));
        pns->AddMember("GetArmedFraction",  new TMemberSnapshotValueFactory<PartWrapper, float>(&PartWrapper::GetArmedFraction, 0.0f));
    }

};




