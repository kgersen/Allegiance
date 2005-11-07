
struct CadetMessage
{
    short cbMsg;
    short msgID;
};

#define ChatMessageID 1

struct ChatMessage : public CadetMessage
{
    char szMsg[256];
};

#define ShipUpdateMessageID 2

struct ShipUpdateMessage : public CadetMessage
{
	ShipUpdate shipUpdate;
};

#define NewShipInstanceMessageID 3

struct NewShipInstanceMessage : public CadetMessage
{
};

#define LogonMessageID 4

struct LogonMessage : public CadetMessage
{
};

#define FireMissileMessageID 5

struct FireMissileMessage : public CadetMessage
{
    Vector              position;
    Vector              velocity;
    Vector              forward;
    float               lock;
    Time                timeFired;
    ExpendableTypeID    missiletypeID;
    ShipID              launcherID;
    SectorID            clusterID;
    MissileID           missileID;
    ObjectType          targetType;
    ObjectID            targetID;
};

#define TerrainChangeMessageID 6

enum TerrainChange {tcInit= 0, tcStraight, tcTurn};

struct TerrainChangeMessage : public CadetMessage
{
    TerrainChange tc;
};

class CadetPlaySite : public IObject
{
public:
    virtual void OnAppMessage(CadetMessage* pmsg) {};
};

enum CadetMode {cmSinglePlayer = 0, cmHostSerial, cmJoinSerial, cmHostModem, cmJoinModemNum1, cmJoinModemNum2};

class CadetPlay : public IObject
{
public:
    virtual void SetCadetMode(CadetMode cm) = 0;
    virtual CadetMode GetCadetMode() = 0;
    virtual HRESULT HostSession() = 0;
    virtual HRESULT JoinSession(ImissionIGC* pmission) = 0;
    virtual HRESULT ReceiveMessages() = 0;
    virtual void SendMessage(CadetMessage* pmsg, bool fGuaranteed = true) = 0;
    virtual void HandleIGCMessage(ImissionIGC* pmission, CadetMessage* pmsg) = 0;
    virtual void SetCadetPlaySite(CadetPlaySite* psite) = 0;
    virtual void SendShipUpdateMessages(ImissionIGC* pmission) = 0;
    virtual void SendShipUpdateMessage(IshipIGC* pship) = 0;
    virtual void SendNewShipInstanceMessage(IshipIGC* pship) = 0;
    virtual void SendFireMissileMessage(DataMissileIGC* pdataMissile) = 0;
  
    static TRef<CadetPlay> Create();
};
