
class CFSMission;
class CFSPlayer;

class CFSSide
{
public:

  CFSSide(TRef<IsideIGC> pSide, CFSMission * pfsmission);
  virtual ~CFSSide();

  CFSMission *        GetMission() {return m_pfsmission;}
  const IsideIGC *    GetSideIGC() {return m_pIsideIGC;}
  CFMGroup *          GetGroup() {return m_pgrpSide;}
  static CFSSide *    FromIGC(IsideIGC * pside) 
  { 
    return pside ? (CFSSide *) pside->GetPrivateData() : NULL;
  }

  bool         IsInvited(CFSPlayer * pfsPlayer);
  void         AddInvitation(const char * szPlayerName);
  const char*  GetInvitedSquadName();

private:

  IsideIGC *          m_pIsideIGC;
  CFMGroup *          m_pgrpSide;
  CFSMission *        m_pfsmission;

  std::vector<ZString> m_Invitations; // who is invited to play on this team?
};

