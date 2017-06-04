

class CAdditionalAGCParamData  // this is extra game parameter data that the object needs but MissionParams do not provide
{
public:
  CAdditionalAGCParamData()
  {
    Reset();
  }

  TechTreeBitMask   m_ttbmAltered[c_cSidesMax];    // which bits have changed
  TechTreeBitMask   m_ttbmNewSetting[c_cSidesMax]; // the new bits
  char              m_szTeamName[c_cSidesMax][c_cbName];

  void Reset()
  {
     ZeroMemory(m_szTeamName, sizeof(m_szTeamName));

     for (int i = 0; i < c_cSidesMax; ++i)
     {
        m_ttbmAltered[i].ClearAll();
        m_ttbmNewSetting[i].ClearAll();
     }
  }
};

class CGameParamData : public MissionParams, public CAdditionalAGCParamData
{
public:

  void Reset()
  {
    CAdditionalAGCParamData::Reset();
     MissionParams::Reset();
  }
};
