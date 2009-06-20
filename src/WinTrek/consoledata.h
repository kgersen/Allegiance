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
	float		GetMassLimit(); //Andon: Added for GetMassLimit usage
	float		GetX(); //Andon: Added for GetX
	float		GetY(); //Andon: Added for GetY
	float		GetZ(); //Andon: Added for GetZ
    float       GetSpeed();
	float		GetType();//Andon: Added for GetType usage
	float		IsFriendly();//Andon: Added for IsFriendly usage
	float		GetMaxSpeed();//Andon: Added for GetMaxSpeed usage
	float		GetMaxThrust();//Andon: Added for GetMaxThrust usage
	float		GetScanRange();//Andon: Added for GetScanRange usage
	float		IsStationRisk();//Andon: Added for IsStationRisk usage
	float		IsStationCapture();//Andon: Added for IsStationCapture usage
	float		IsRescue();//Andon: Added for IsRescue usage
	float		IsLeadRelay();//Andon: Added for IsLeadRelay usage
	float		CanLandCarrier();//Andon: Added for CanLandCarrier usage
    float       GetMass();
    float       GetRange();
    float       GetPercentHitPoints();
	float		GetNumberHitPoints();//Andon: Added for GetNumberHitPoints usage
	float		GetMaxHitPoints();//Andon: Added for GetMaxHitPoints Usage
	float		GetArmorClass();//Andon: Added for GetArmorClass usage
    float       GetPercentShields();
	float		GetNumberShields();//Andon: Added for GetNumberShields Usage
	float		GetMaxShields();//Andon: Added for GetMaxShields Usage
	float		GetShieldClass();//Andon: Added for GetShieldClass usage
    float       GetPercentEnergy();
	float		GetNumberEnergy();//Andon: Added for GetNumberEnergy Usage
	float		GetMaxEnergy();//Andon: Added for GetMaxEnergy Usage
	float		GetEnergyRegen();//Andon: Added for GetEnergyRegen Usage
    float       GetAmmo();
	float		GetNumberAmmo();//Andon: Added for GetNumberAmmo Usage
	float		GetMaxAmmo();//Andon: Added for GetMaxAmmo Usage
    float       GetFuel();
	float		GetNumberFuel();//Andon: Added for GetNumberFuel Usage
	float		GetMaxFuel();//Andon: Added for GetMaxFuel Usage
    float       GetOre();
    float       GetVectorLock();
    float       GetCloaking();
    float       GetSignature();
    float       GetNumObservers();
    float       GetRipcordTimeLeft();
    float       GetRipcordEnergy();//Andon: Added for GetRipcordEnergy Usage
	float		GetRipcordTime();//Andon: Added for GetRipcordTime Usage
	float		CanRipcord();//Andon: Added for CanRipcord usage
	float		CanSmallRipcord();//Andon: Added for CanSmallRipcord usage
	float		IsRipcord();//Andon: Added for IsRipcord usage
	float		IsSmallRipcord();//Andon: Added for IsSmallRipcord usage
	float		GetECM();//Andon: Added for GetECM usage
	float		GetSideThrust();//Andon: Added for GetSidethrust usage
	float		GetRearThrust();//Andon: Added for GetRearthrust usage
    float       GetEndurance();
    float       IsCloaked();
    float       IsEjectPod();
    float       IsRipcording();
    bool        IsVisible();
    bool        IsNotNull();

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
		//pns->AddMember("GetMass",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMass, 0.0f)); // Imago removed (Andon)
        pns->AddMember("GetMassLimit",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMassLimit, 0.0f)); //Andon: Added for GetMassLimit usage
		//pns->AddMember("GetSpeed",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetSpeed, 0.0f)); // Imago removed (Andon)
        pns->AddMember("GetMass",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMass, 0.0f));
        pns->AddMember("GetX",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetX, 0.0f)); //Andon: Added for GetX
        pns->AddMember("GetY",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetY, 0.0f)); //Andon: Added for GetY
        pns->AddMember("GetZ",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetZ, 0.0f)); //Andon: Added for GetZ
        pns->AddMember("IsFriendly",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsFriendly, 0.0f));//Andon: Added for IsFriendly usage
        pns->AddMember("GetType",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetType, 0.0f));//Andon: Added for GetType usage
        pns->AddMember("GetSpeed",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetSpeed, 0.0f));
        pns->AddMember("GetMaxSpeed",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxSpeed, 0.0f));//Andon: Added for GetMaxSpeed Usage
        pns->AddMember("GetMaxThrust",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxThrust, 0.0f));//Andon: Added for GetMaxThrust usage
        pns->AddMember("GetSideThrust",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetSideThrust, 0.0f));//Andon: Added for GetSideThrust usage
        pns->AddMember("GetRearThrust",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRearThrust, 0.0f));//Andon: Added for GetRearThrust usage
        pns->AddMember("GetScanRange",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetScanRange, 0.0f));//Andon: Added for GetScanRange usage
        pns->AddMember("IsStationRisk",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsStationRisk, 0.0f));//Andon: Added for IsStationRisk usage
        pns->AddMember("IsStationCapture",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsStationCapture, 0.0f));//Andon: Added for IsStationCapture usage
        pns->AddMember("IsRescue",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsRescue, 0.0f));//Andon: Added for IsRescue usage
        pns->AddMember("IsLeadRelay",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsLeadRelay, 0.0f));//Andon: Added for IsLeadRelay Usage
        pns->AddMember("CanLandCarrier",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::CanLandCarrier, 0.0f));//Andon: Added for CanLandCarrier Usage
        pns->AddMember("GetRange",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRange, 0.0f));
        pns->AddMember("GetPercentHitPoints",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetPercentHitPoints, 0.0f));
        pns->AddMember("GetNumberHitPoints",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumberHitPoints, 0.0f));//Andon: Added for GetNumberHitPoints Usage
		pns->AddMember("GetMaxHitPoints",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxHitPoints, 0.0f));//Andon: Added for GetMaxHitPoints usage
		pns->AddMember("GetArmorClass",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetArmorClass, 0.0f));//Andon: Added for GetArmorClass usage
		pns->AddMember("GetPercentShields",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetPercentShields, 0.0f));
        pns->AddMember("GetNumberShields",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumberShields, 0.0f));//Andon: Added for GetNumberShields usage
		pns->AddMember("GetMaxShields",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxShields, 0.0f));//Andon: Added for GetMaxShields usage
		pns->AddMember("GetShieldClass",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetShieldClass, 0.0f));//Andon: Added for GetShieldClass usage
		pns->AddMember("GetPercentEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetPercentEnergy, 0.0f));
        pns->AddMember("GetNumberEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumberEnergy, 0.0f));//Andon: Added for GetNumberEnergy usage
        pns->AddMember("GetMaxEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxEnergy, 0.0f));//Andon: Added for GetMaxEnergy Usage
        pns->AddMember("GetEnergyRegen",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetEnergyRegen, 0.0f));//Andon: Added for GetEnergyRegen Usage
        pns->AddMember("GetAmmo",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetAmmo, 0.0f));
        pns->AddMember("GetNumberAmmo",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumberAmmo, 0.0f));//Andon: Added for GetNumberAmmo Usage
        pns->AddMember("GetMaxAmmo",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxAmmo, 0.0f));//Andon: Added for GetMaxAmmo Usage
        pns->AddMember("GetFuel",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetFuel, 0.0f));
        pns->AddMember("GetNumberFuel",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumberFuel, 0.0f));//Andon: Added for GetNumberFuel Usage
        pns->AddMember("GetMaxFuel",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetMaxFuel, 0.0f));//Andon: Added for GetMaxFuel Usage
        pns->AddMember("GetOre",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetOre, 0.0f));
        pns->AddMember("GetVectorLock",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetVectorLock, 0.0f));
        pns->AddMember("GetCloaking",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetCloaking, 0.0f));
        pns->AddMember("GetNumObservers",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetNumObservers, 0.0f));
        pns->AddMember("GetSignature",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetSignature, 0.0f));
        pns->AddMember("GetRipcordTimeLeft",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRipcordTimeLeft, 0.0f));
        pns->AddMember("GetRipcordEnergy",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRipcordEnergy, 0.0f));//Andon: Added for GetRipcordEnergy usage
        pns->AddMember("GetRipcordTime",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetRipcordTime, 0.0f));//Andon: Added for GetRipcordTime usage
        pns->AddMember("CanRipcord",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::CanRipcord, 0.0f));//Andon: Added for CanRipcord usage
        pns->AddMember("CanSmallRipcord",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::CanSmallRipcord, 0.0f));//Andon: Added for CanSmallRipcord usage
        pns->AddMember("IsRipcord",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsRipcord, 0.0f));//Andon: Added for IsRipcord usage
        pns->AddMember("IsSmallRipcord",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsSmallRipcord, 0.0f));//Andon: Added for IsSmallRipcord usage
        pns->AddMember("GetECM",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetECM, 0.0f));//Andon: Added for GetECM usage
        pns->AddMember("GetEndurance",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::GetEndurance, 1.0f));
        pns->AddMember("IsCloaked",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsCloaked, 0.0f));
        pns->AddMember("IsEjectPod",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsEjectPod, 0.0f));
        pns->AddMember("IsRipcording",  new TMemberSnapshotValueFactory<ModelData, float>(&ModelData::IsRipcording, 0.0f));
        pns->AddMember("IsVisible", new TMemberSnapshotValueFactory<ModelData, bool>(&ModelData::IsVisible, false));
        pns->AddMember("IsNotNull", new TMemberSnapshotValueFactory<ModelData, bool>(&ModelData::IsNotNull, false));
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



