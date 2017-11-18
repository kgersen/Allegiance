/////////////////////////////////////////////////////////////////////////////
// PigMissionParams.cpp : Implementation of the CPigMissionParams class.

#include "pch.h"
#include <TCLib.h>
#include "PigMissionParams.h"


/////////////////////////////////////////////////////////////////////////////
// Begin: Allegiance Initialization                                        //
                                                                           //
HKEY g_dwArtHive = HKEY_LOCAL_MACHINE;                                     //
char g_szArtReg[] =                                                        //
  "SOFTWARE\\Classes\\AppID\\{F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}";      //
                                                                           //
                                                                           //
// End: Allegiance Initialization                                          //
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CPigMissionParams

TC_OBJECT_EXTERN_IMPL(CPigMissionParams)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPigMissionParams::CPigMissionParams()
{
}

HRESULT CPigMissionParams::FinalConstruct()
{
  // Initialize the data
  InitNew();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPigMissionParams::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPigMissionParams,
  };
  for (int i = 0; i < sizeofArray(arr); ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IPigMissionParams Interface Methods

STDMETHODIMP CPigMissionParams::Validate()
{
  XLock lock(this);
  if (2 > m_mp.nTeams || m_mp.nTeams > 6)
    return Error(IDS_E_TEAMCOUNT, IID_IPigMissionParams);
  if (1 > m_mp.nMaxPlayersPerTeam || m_mp.nMaxPlayersPerTeam > 45)
    return Error(IDS_E_MINTEAMPLAYERS, IID_IPigMissionParams);
  if (1 > m_mp.nMinPlayersPerTeam || m_mp.nMinPlayersPerTeam > 30)
    return Error(IDS_E_MAXTEAMPLAYERS, IID_IPigMissionParams);
  if (m_mp.nMinPlayersPerTeam > m_mp.nMaxPlayersPerTeam)
    return Error(IDS_E_MINMAXREVERSED, IID_IPigMissionParams);
  return S_OK;
}

STDMETHODIMP CPigMissionParams::put_TeamCount(short nTeamCount)
{
  return TCComPropertyPut(this, m_mp.nTeams, nTeamCount, dispid_TeamCount);
}

STDMETHODIMP CPigMissionParams::get_TeamCount(short* pnTeamCount)
{
  return TCComPropertyGet(this, pnTeamCount, m_mp.nTeams);
}

STDMETHODIMP CPigMissionParams::put_MaxTeamPlayers(short nMaxTeamPlayers)
{
  return TCComPropertyPut(this, m_mp.nMaxPlayersPerTeam, nMaxTeamPlayers, dispid_TeamCount);
}

STDMETHODIMP CPigMissionParams::get_MaxTeamPlayers(short* pnMaxTeamPlayers)
{
  return TCComPropertyGet(this, pnMaxTeamPlayers, m_mp.nMaxPlayersPerTeam);
}

STDMETHODIMP CPigMissionParams::put_MinTeamPlayers(short nMinTeamPlayers)
{
  return TCComPropertyPut(this, m_mp.nMinPlayersPerTeam, nMinTeamPlayers, dispid_TeamCount);
}

STDMETHODIMP CPigMissionParams::get_MinTeamPlayers(short* pnMinTeamPlayers)
{
  return TCComPropertyGet(this, pnMinTeamPlayers, m_mp.nMinPlayersPerTeam);
}

STDMETHODIMP CPigMissionParams::put_MapType(PigMapType eMapType)
{
  return TCComPropertyPut(this, m_mp.mmMapType, eMapType, dispid_TeamCount);
}

STDMETHODIMP CPigMissionParams::get_MapType(PigMapType* peMapType)
{
  return TCComPropertyGet(this, peMapType, (PigMapType&)m_mp.mmMapType);
}


/////////////////////////////////////////////////////////////////////////////
// IPigMissionParamsPrivate Interface Methods

STDMETHODIMP CPigMissionParams::GetData(IStream** ppstm)
{
  // Initialize the [out] parameter
  CLEAROUT(ppstm, (IStream*)NULL);

  // Create a stream on global
  IStreamPtr spstm;
  RETURN_FAILED(CreateStreamOnHGlobal(NULL, true, &spstm));

  // Write the size of the data structure to the stream
  UINT cb = sizeof(m_mp);
  RETURN_FAILED(spstm->Write(&cb, sizeof(cb), NULL));

  // Write the data structure to the stream
  RETURN_FAILED(spstm->Write(&m_mp, cb, NULL));

  // Detach the interface to the [out] parameter
  *ppstm = spstm.Detach();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistStreamInit Interface Methods

STDMETHODIMP CPigMissionParams::InitNew()
{
  XLock lock(this);
  ZeroMemory(&m_mp, sizeof(m_mp));

  // Reset the structure
  m_mp.Reset();

  // Initialize the Game name
  strcpy(m_mp.strGameName, "Pig Mission");

  // Perform default processing
  return TCPersistStreamInitImplBase::InitNew();
}


