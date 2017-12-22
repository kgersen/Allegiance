
/*-------------------------------------------------------------------------
 * fedsrv\AGCAGCGameParameters.CPP
 * 
 * Implementation of CAGCGameParameters
 *
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *
 * There are two types of Game Parameters objects.  The first is the
 * kind that is attached to an existing game mission.  You get only do
 * gets on this kind of object.  The other is the unattached kind.  This
 * kind is used to create a new mission.  You can to gets or sets on this
 * kind.
 *
 * If CAGCGameParameters::m_pfsMission is NULL, then this object is "unattached"
 *
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include "AGCGameParameters.h"
#include "MessageVersion.h" // For MSGVER - KGJV: Changed to not depend on DirectPlay include
//ALLOC_MSG_LIST;       // Since <messages.h> includes an extern to it


/////////////////////////////////////////////////////////////////////////////
// CAGCGameParameters

TC_OBJECT_EXTERN_IMPL(CAGCGameParameters)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CAGCGameParameters::CAGCGameParameters()
    : m_timestart(0)
{
};

CAGCGameParameters::~CAGCGameParameters() 
{ 
};


/////////////////////////////////////////////////////////////////////////////
// IPersist Interface Methods

STDMETHODIMP CAGCGameParameters::GetClassID(CLSID* pClassID)
{
  CLEAROUT(pClassID, GetObjectCLSID());
  return S_OK;                  
}



/////////////////////////////////////////////////////////////////////////////
// IPersistStreamInit Interface Methods

STDMETHODIMP CAGCGameParameters::IsDirty()
{
  // Always dirty since we don't maintain a dirty flag
  return S_OK;
}

STDMETHODIMP CAGCGameParameters::Load(IStream* pStm)
{
  // Read the version indicator (old struct size will never == c_cHasVersion)
  int n;
  RETURN_FAILED(pStm->Read(&n, sizeof(n), NULL));
  if (c_cHasVersion != n)
    return E_UNEXPECTED;

  // Read the version number (of AGCGameParameters)
  RETURN_FAILED(pStm->Read(&n, sizeof(n), NULL));
  if (c_cCurrentVersion != n)
    return E_UNEXPECTED;

  // Read the protocol version number (of Allegiance messaging)
  RETURN_FAILED(pStm->Read(&n, sizeof(n), NULL));
  if (MSGVER != n)
    return E_UNEXPECTED;

  // Read the size of the following structure
  RETURN_FAILED(pStm->Read(&n, sizeof(n), NULL));
  if (n != sizeof(m_mp))
    return E_UNEXPECTED;

  // Thread-sync the data members
  XLock lock(this);

  // Read the data structure(s)
  RETURN_FAILED(pStm->Read(&m_mp, sizeof(m_mp), NULL));

  // Read the game start time
  RETURN_FAILED(pStm->Read(&m_timestart, sizeof(m_timestart), NULL));

  // Read the game story text
  RETURN_FAILED(m_bstrStoryText.ReadFromStream(pStm));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCGameParameters::Save(IStream* pStm, BOOL fClearDirty)
{
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // I M P O R T A N T !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //
  // NOTE: If this stream format is ever changed, PLEASE update
  //   c_cCurrentVersion in the header file!
  // And, don't forget to update the corresponding Load method above.
  //
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  XLock lock(this);

  // Write the version indicator (old struct size will never == c_cHasVersion)
  int n = c_cHasVersion;
  RETURN_FAILED(pStm->Write(&n, sizeof(n), NULL));

  // Write the version number (of AGCGameParameters)
  n = c_cCurrentVersion;
  RETURN_FAILED(pStm->Write(&n, sizeof(n), NULL));

  // Write the protocol version number (of Allegiance messaging)
  n = MSGVER;
  RETURN_FAILED(pStm->Write(&n, sizeof(n), NULL));

  // Write the size of the following structure
  n = sizeof(m_mp);
  RETURN_FAILED(pStm->Write(&n, sizeof(n), NULL));
  RETURN_FAILED(pStm->Write(&m_mp, sizeof(m_mp), NULL));

  // Write the game start time
  DATE theTime = 0;
  if (m_mp.timeStart.clock())
  {
    time_t aclock;
    time(&aclock);
    Time timeNow = Time::Now();
    aclock = aclock - (timeNow.clock()-m_mp.timeStart.clock())/1000;
    struct tm *newtime;
    newtime = localtime(&aclock);

    SYSTEMTIME st;
    st.wYear         = newtime->tm_year+1900;
    st.wMonth        = newtime->tm_mon+1;
    st.wDayOfWeek    = newtime->tm_wday;
    st.wDay          = newtime->tm_mday;
    st.wHour         = newtime->tm_hour;
    st.wMinute       = newtime->tm_min;
    st.wSecond       = newtime->tm_sec;
    st.wMilliseconds = 0;

    SystemTimeToVariantTime(&st, &theTime);
    //debugf("Returning start time of %d/%d/%d, %d:%d:%d\n", st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);
  }
  else
  {
    //debugf("Returning start time of zero.\n");
  }
  RETURN_FAILED(pStm->Write(&theTime, sizeof(theTime), NULL));

  // Write the game story text
  RETURN_FAILED(m_bstrStoryText.WriteToStream(pStm));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCGameParameters::GetSizeMax(ULARGE_INTEGER* pCbSize)
{
  XLock lock(this);
  return TCGetPersistStreamSize(GetUnknown(), pCbSize);
}

STDMETHODIMP CAGCGameParameters::InitNew( void)
{
  // Initialize the GameParameters object
  m_mp.Reset();
  m_bstrStoryText.Empty();

  // Indicate success
  return S_OK;
}


STDMETHODIMP CAGCGameParameters::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IAGCGameParameters
  };
  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
  {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return ERROR;
}


STDMETHODIMP CAGCGameParameters::Validate()
{
  // Validate the parameters
  XLock lock(this);
  const char * szInvalid = m_mp.Invalid();
  if (szInvalid) 
    return Error(szInvalid);

  return S_OK;
}                          


/*-------------------------------------------------------------------------
 * put_MinPlayers()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_MinPlayers(BYTE Val)
{
  XLock lock(this);
  m_mp.nMinPlayersPerTeam = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_MinPlayers()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_MinPlayers(BYTE* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, (BYTE)m_mp.nMinPlayersPerTeam);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_MaxPlayers()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_MaxPlayers(BYTE Val)
{
  XLock lock(this);
  m_mp.nMaxPlayersPerTeam = Val;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_MaxPlayers()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_MaxPlayers(BYTE* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, (BYTE)m_mp.nMaxPlayersPerTeam);
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_GameName()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_GameName(BSTR* pVal)
{
  XLock lock(this);
  CComBSTR bstrTemp(m_mp.strGameName);
  CLEAROUT(pVal, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GameName()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_GameName(BSTR Val)
{
  XLock lock(this);
  if (BSTRLen(Val))
  {
    USES_CONVERSION;
    lstrcpynA(m_mp.strGameName, OLE2CA(Val), sizeof(m_mp.strGameName));
  }
  else
  {
    ZeroMemory(m_mp.strGameName, sizeof(m_mp.strGameName));
  }
  
  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_EjectPods()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_EjectPods(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bEjectPods));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_EjectPods()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_EjectPods(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bEjectPods = !!Val;
  return S_OK;
}




/*-------------------------------------------------------------------------
 * get_AllowPrivateTeams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowPrivateTeams(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowPrivateTeams));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowPrivateTeams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowPrivateTeams(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowPrivateTeams = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PowerUps()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_PowerUps(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bPowerUps));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_PowerUps()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_PowerUps(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bPowerUps = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_AllowJoiners()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowJoiners(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowJoiners));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowJoiners()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowJoiners(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowJoiners = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_Stations()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_Stations(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bStations));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_Stations()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_Stations(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bStations = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_ScoresCount()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_ScoresCount(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bScoresCount));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_ScoresCount()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_ScoresCount(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bScoresCount = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_Drones()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_Drones(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bDrones));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_Drones()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_Drones(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bDrones = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_IsGoalConquest()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_IsGoalConquest(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.IsConquestGame()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_IsGoalConquest()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_IsGoalConquest(VARIANT_BOOL Val)
{
  XLock lock(this);

  if (!Val)
    m_mp.iGoalConquestPercentage = 0;
  else 
  {
    // set to 100 only if needed--don't override another "true" value of iGoalConquestPercentage with 100
    if (m_mp.iGoalConquestPercentage == 0)
      m_mp.iGoalConquestPercentage = 100;
  }

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_IsGoalArtifacts()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_IsGoalArtifacts(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.IsArtifactsGame()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_IsGoalArtifacts()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_IsGoalArtifacts(VARIANT_BOOL Val)
{
  XLock lock(this);

  if (!Val)
    m_mp.nGoalArtifactsCount = 0;
  else 
  {
    // set to 10 only if needed--don't override another "true" value of nGoalArtifactsCount with 10
    if (m_mp.nGoalArtifactsCount == 0)
      m_mp.nGoalArtifactsCount = 10;
  }
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_IsGoalTeamMoney()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_IsGoalTeamMoney(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.IsProsperityGame()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GoalTeamMoney()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_IsGoalTeamMoney(VARIANT_BOOL Val)
{
  XLock lock(this);

  if (!Val)
    m_mp.fGoalTeamMoney = 0;
  else 
  {
    // set to 1.0 only if needed--don't override another "true" value of fGoalTeamMoney with 1.0
    if (m_mp.fGoalTeamMoney == 0)
      m_mp.fGoalTeamMoney = 1.0f;
  }

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_GoalTeamKills()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_IsGoalTeamKills(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.IsDeathMatchGame()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GoalTeamKills()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_IsGoalTeamKills(VARIANT_BOOL Val)
{
  XLock lock(this);

  if (!Val)
    m_mp.nGoalTeamKills = 0;
  else 
  {
    // set to 10 only if needed--don't override another "true" value of nGoalTeamKills with 10
    if (m_mp.nGoalTeamKills == 0)
      m_mp.nGoalTeamKills = 10;
  }

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_Resources()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_Resources(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.iResources));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_Resources()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_Resources(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.iResources = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_ResourceAmountsVisible()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_ResourceAmountsVisible(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bResourceAmountsVisible));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_ResourceAmountsVisible()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_ResourceAmountsVisible(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bResourceAmountsVisible = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_RandomWormholes()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_RandomWormholes(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bRandomWormholes));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_RandomWormholes()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_RandomWormholes(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bRandomWormholes = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NoTeams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NoTeams(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bNoTeams));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NoTeams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NoTeams(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bNoTeams = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_ShowHomeSector()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_ShowHomeSector(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bShowHomeSector));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_ShowHomeSector()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_ShowHomeSector(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bShowHomeSector = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_rgCivID()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_CivIDs(short element, short* pVal)
{
  // Bounds check
  if (0 > element || element >= sizeofArray(m_mp.rgCivID))
    return E_INVALIDARG;

  XLock lock(this);
  CLEAROUT(pVal, (short)m_mp.rgCivID[element]);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_rgCivID()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_CivIDs(short element, short Val)
{
  // Bounds check
  if (0 > element || element >= sizeofArray(m_mp.rgCivID))
    return E_INVALIDARG;

  XLock lock(this);
  m_mp.rgCivID[element] = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_GoalTeamMoney()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_GoalTeamMoney(AGCMoney* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, (AGCMoney)m_mp.fGoalTeamMoney);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GoalTeamMoney()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_GoalTeamMoney(AGCMoney Val)
{
  XLock lock(this);
  m_mp.fGoalTeamMoney = static_cast<float>(Val);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_TsiPlayerRegenerate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_TsiPlayerRegenerate(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.tsiPlayerRegenerate);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TsiPlayerRegenerate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_TsiPlayerRegenerate(short Val)
{
  XLock lock(this);
  m_mp.tsiPlayerRegenerate = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_TsiNeutralRegenerate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_TsiNeutralRegenerate(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.tsiNeutralRegenerate);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TsiNeutralRegenerate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_TsiNeutralRegenerate(short Val)
{
  XLock lock(this);
  m_mp.tsiNeutralRegenerate = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_TsiPlayerStart()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_TsiPlayerStart(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.tsiPlayerStart);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TsiPlayerStart()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_TsiPlayerStart(short Val)
{
  XLock lock(this);
  m_mp.tsiPlayerStart = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_TsiNeutralStart()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_TsiNeutralStart(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.tsiNeutralStart);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TsiNeutralStart()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_TsiNeutralStart(short Val)
{
  XLock lock(this);
  m_mp.tsiNeutralStart = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_StartingMoney()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_StartingMoney(float* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.fStartingMoney);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_StartingMoney()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_StartingMoney(float Val)
{
  XLock lock(this);
  m_mp.fStartingMoney = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_Lives()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_Lives(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iLives);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_Lives()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_Lives(short Val)
{
  XLock lock(this);
  m_mp.iLives = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_GoalTeamKills()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_GoalTeamKills(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nGoalTeamKills);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GoalTeamKills()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_GoalTeamKills(short Val)
{
  XLock lock(this);
  m_mp.nGoalTeamKills = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_MapType()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_MapType(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.mmMapType);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_MapType()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_MapType(short Val)
{
  XLock lock(this);
  m_mp.mmMapType = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_MapSize()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_MapSize(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iMapSize);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_MapSize()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_MapSize(short Val)
{
  XLock lock(this);
  m_mp.iMapSize = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_RandomEncounters()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_RandomEncounters(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iRandomEncounters);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_RandomEncounters()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_RandomEncounters(short Val)
{
  XLock lock(this);
  m_mp.iRandomEncounters = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NeutralSectors()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NeutralSectors(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bNeutralSectors));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NeutralSectors()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NeutralSectors(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bNeutralSectors = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_AlephPositioning()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AlephPositioning(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iAlephPositioning);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AlephPositioning()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AlephPositioning(short Val)
{
  XLock lock(this);
  m_mp.iAlephPositioning = Val;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_AlephsPerSector()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AlephsPerSector(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iAlephsPerSector);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AlephsPerSector()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AlephsPerSector(short Val)
{
  XLock lock(this);
  m_mp.iAlephsPerSector = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_Teams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_Teams(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nTeams);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_Teams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_Teams(short Val)
{
  XLock lock(this);

  if (Val > c_cSidesMax) 
    Val = c_cSidesMax;
  else
  if (Val < 2) // Hardcoded minimum set in missionigc.cpp
    Val = 2;

  m_mp.nTeams = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_MinRank()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_MinRank(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iMinRank);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_MinRank()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_MinRank(short Val)
{
  XLock lock(this);
  m_mp.iMinRank = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_MaxRank()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_MaxRank(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iMaxRank);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_MaxRank()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_MaxRank(short Val)
{
  XLock lock(this);
  m_mp.iMaxRank = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PlayerSectorAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_PlayerSectorAsteroids(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nPlayerSectorAsteroids);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_PlayerSectorAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_PlayerSectorAsteroids(short Val)
{
  XLock lock(this);
  m_mp.nPlayerSectorAsteroids = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NeutralSectorAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NeutralSectorAsteroids(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nNeutralSectorAsteroids);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NeutralSectorAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NeutralSectorAsteroids(short Val)
{
  XLock lock(this);
  m_mp.nNeutralSectorAsteroids = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PlayerSectorTreasures()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_PlayerSectorTreasures(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nPlayerSectorTreasures);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_PlayerSectorTreasures()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_PlayerSectorTreasures(short Val)
{
  XLock lock(this);
  m_mp.nPlayerSectorTreasures = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NeutralSectorTreasures()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NeutralSectorTreasures(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nNeutralSectorTreasures);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NeutralSectorTreasures()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NeutralSectorTreasures(short Val)
{
  XLock lock(this);
  m_mp.nNeutralSectorTreasures = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PlayerSectorTreasureRate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_PlayerSectorTreasureRate(FLOAT* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nPlayerSectorTreasureRate);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_PlayerSectorTreasureRate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_PlayerSectorTreasureRate(FLOAT Val)
{
  XLock lock(this);
  m_mp.nPlayerSectorTreasureRate = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NeutralSectorTreasureRate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NeutralSectorTreasureRate(FLOAT* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nNeutralSectorTreasureRate);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NeutralSectorTreasureRate()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NeutralSectorTreasureRate(FLOAT Val)
{
  XLock lock(this);
  m_mp.nNeutralSectorTreasureRate = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PlayerSectorMineableAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_PlayerSectorMineableAsteroids(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nPlayerSectorMineableAsteroids);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_PlayerSectorMineableAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_PlayerSectorMineableAsteroids(short Val)
{
  XLock lock(this);
  m_mp.nPlayerSectorMineableAsteroids = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NeutralSectorMineableAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NeutralSectorMineableAsteroids(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nNeutralSectorMineableAsteroids);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NeutralSectorMineableAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NeutralSectorMineableAsteroids(short Val)
{
  XLock lock(this);
  m_mp.nNeutralSectorMineableAsteroids = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PlayerSectorSpecialAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_PlayerSectorSpecialAsteroids(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nPlayerSectorSpecialAsteroids);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_PlayerSectorSpecialAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_PlayerSectorSpecialAsteroids(short Val)
{
  XLock lock(this);
  m_mp.nPlayerSectorSpecialAsteroids = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_NeutralSectorSpecialAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_NeutralSectorSpecialAsteroids(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nNeutralSectorSpecialAsteroids);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_NeutralSectorSpecialAsteroids()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_NeutralSectorSpecialAsteroids(short Val)
{
  XLock lock(this);
  m_mp.nNeutralSectorSpecialAsteroids = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_IGCStaticFile()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_IGCStaticFile(BSTR *pVal)
{
  XLock lock(this);
  CComBSTR bstrTemp(m_mp.szIGCStaticFile);
  CLEAROUT(pVal, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_IGCStaticFile()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_IGCStaticFile(BSTR Val)
{
  XLock lock(this);
  if (BSTRLen(Val))
  {
    USES_CONVERSION;
    lstrcpynA(m_mp.szIGCStaticFile, OLE2CA(Val), sizeof(m_mp.szIGCStaticFile));
  }
  else
  {
    ZeroMemory(m_mp.szIGCStaticFile, sizeof(m_mp.szIGCStaticFile));
  }
  
  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_GamePassword()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_GamePassword(BSTR *pVal)
{
  XLock lock(this);
  CComBSTR bstrTemp(m_mp.strGamePassword);
  CLEAROUT(pVal, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GamePassword()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_GamePassword(BSTR Val)
{
  XLock lock(this);
  if (BSTRLen(Val))
  {
    USES_CONVERSION;
    lstrcpynA(m_mp.strGamePassword, OLE2CA(Val), sizeof(m_mp.strGamePassword));
  }
  else
  {
    ZeroMemory(m_mp.strGamePassword, sizeof(m_mp.strGamePassword));
  }
  
  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_InvulnerableStations()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_InvulnerableStations(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bInvulnerableStations));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_InvulnerableStations()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_InvulnerableStations(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bInvulnerableStations = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_ShowMap()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_ShowMap(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bShowMap));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_ShowMap()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_ShowMap(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bShowMap = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_AllowDevelopments()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_AllowDevelopments(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowDevelopments));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowDevelopments()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_AllowDevelopments(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowDevelopments = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_AllowDefections()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_AllowDefections(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowDefections));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowDefections()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_AllowDefections(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowDefections = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_SquadGame()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_SquadGame(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bSquadGame));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_SquadGame()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_SquadGame(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bSquadGame = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_AllowFriendlyFire()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_AllowFriendlyFire(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowFriendlyFire));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowFriendlyFire()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_AllowFriendlyFire(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowFriendlyFire = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_IGCcoreVersion()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_IGCcoreVersion(long *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, long(m_mp.verIGCcore));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_get_GameLength()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_GameLength(float *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.dtGameLength);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GameLength()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_GameLength(float Val)
{
  XLock lock(this);
  m_mp.dtGameLength = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_He3Density()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_He3Density(float *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.fHe3Density);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_He3Density()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_He3Density(float Val)
{
  XLock lock(this);
  m_mp.fHe3Density = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_KillPenalty()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_KillPenalty(AGCMoney *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, AGCMoney(m_mp.m_killPenalty));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_KillPenalty()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_KillPenalty(AGCMoney Val)
{
  XLock lock(this);
  m_mp.m_killPenalty = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_KillReward()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_KillReward(AGCMoney *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, AGCMoney(m_mp.m_killReward));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_KillReward()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_KillReward(AGCMoney Val)
{
  XLock lock(this);
  m_mp.m_killReward = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_EjectPenalty()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_EjectPenalty(AGCMoney *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, AGCMoney(m_mp.m_ejectPenalty));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_EjectPenalty()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_EjectPenalty(AGCMoney Val)
{
  XLock lock(this);
  m_mp.m_ejectPenalty = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_EjectReward()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_EjectReward(AGCMoney *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, AGCMoney(m_mp.m_ejectReward));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_EjectReward()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_EjectReward(AGCMoney Val)
{
  XLock lock(this);
  m_mp.m_ejectReward = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_TimeStart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_TimeStart(double *pVal)
{
    XLock lock(this);
    CLEAROUT(pVal, double(m_timestart));
    return S_OK;
}


/*-------------------------------------------------------------------------
 * get_TimeStartClock()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_TimeStartClock(long *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, long(m_mp.timeStart.clock()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_GoalArtifactsCount()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_GoalArtifactsCount(short *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nGoalArtifactsCount);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GoalArtifactsCount()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_GoalArtifactsCount(short Val)
{
  XLock lock(this);
  m_mp.nGoalArtifactsCount = Val;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_AutoRestart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_AutoRestart(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAutoRestart));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AutoRestart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_AutoRestart(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bAutoRestart = !!newVal;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_DefaultCountdown()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_DefaultCountdown(float *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.fStartCountdown);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_DefaultCountdown()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_DefaultCountdown(float newVal)
{
  XLock lock(this);
  m_mp.fStartCountdown = newVal;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_InitialMinersPerTeam()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_InitialMinersPerTeam(unsigned char *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, (unsigned char)(m_mp.nInitialMinersPerTeam));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_InitialMinersPerTeam()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_InitialMinersPerTeam(unsigned char newVal)
{
  XLock lock(this);
  m_mp.nInitialMinersPerTeam = newVal;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_MaxDronesPerTeam()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_MaxDronesPerTeam(unsigned char *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, (unsigned char)(m_mp.nMaxDronesPerTeam));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_MaxDronesPerTeam()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_MaxDronesPerTeam(unsigned char newVal)
{
  XLock lock(this);
  m_mp.nMaxDronesPerTeam = newVal;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_CustomMap()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_CustomMap(BSTR* pVal)
{
  XLock lock(this);
  CComBSTR bstrTemp(m_mp.szCustomMapFile);
  CLEAROUT(pVal, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_CustomMap()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_CustomMap(BSTR Val)
{
  XLock lock(this);
  if (BSTRLen(Val))
  {
    USES_CONVERSION;
    lstrcpynA(m_mp.szCustomMapFile, OLE2CA(Val), sizeof(m_mp.szCustomMapFile));
  }
  else
  {
    ZeroMemory(m_mp.szCustomMapFile, sizeof(m_mp.szCustomMapFile));
  }
  
  // Indicate success
  return S_OK;
}

/*-------------------------------------------------------------------------
 * get_RestartCountdown()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_RestartCountdown(float *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.fRestartCountdown);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_RestartCountdown()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_RestartCountdown(float newVal)
{
  XLock lock(this);
  m_mp.fRestartCountdown = newVal;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TotalMaxPlayers()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_TotalMaxPlayers(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nTotalMaxPlayersPerGame);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TotalMaxPlayers()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_TotalMaxPlayers(short newVal)
{
  XLock lock(this);
  m_mp.nTotalMaxPlayersPerGame = newVal;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_LockTeamSettings()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_LockTeamSettings(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bLockTeamSettings));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_LockTeamSettings()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_LockTeamSettings(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bLockTeamSettings = !!newVal;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * put_InvitationList()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_InvitationListID(int* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nInvitationListID);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_InvitationList()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_InvitationListID(int newVal)
{
  XLock lock(this);
  m_mp.nInvitationListID = newVal;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * put_IsSquadGame()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_IsSquadGame(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bSquadGame));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_IsSquadGame()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_IsSquadGame(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bSquadGame = !!newVal;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * put_LockGameOpen()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_LockGameOpen(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bLockGameOpen));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_LockGameOpen()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_LockGameOpen(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bLockGameOpen = !!newVal;
  return S_OK;
}




/*-------------------------------------------------------------------------
 * get_TeamName()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_TeamName(short iTeam, BSTR* pVal)
{
  XLock lock(this);

  // Ensure that the specified index is in range
  if ((unsigned short) iTeam >= c_cSidesMax)
    return E_INVALIDARG;

  CComBSTR bstrTemp(m_mp.m_szTeamName[iTeam]);
  CLEAROUT(pVal, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_TeamName()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_TeamName(short iTeam, BSTR Val)
{
  XLock lock(this);

  // Ensure that the specified index is in range
  if ((unsigned short) iTeam >= c_cSidesMax)
    return E_INVALIDARG;

  // Get the character count of the specified string
  UINT cchVal = BSTRLen(Val);
  if (cchVal)
  {
    // Ensure that the specified string will fit into the buffer
    if (cchVal >= sizeof(m_mp.m_szTeamName[0]))
      return E_INVALIDARG;

    USES_CONVERSION;
    lstrcpynA(m_mp.m_szTeamName[iTeam], OLE2CA(Val), sizeof(m_mp.m_szTeamName[0]));
  }
  else
  {
    ZeroMemory(m_mp.m_szTeamName[iTeam], sizeof(m_mp.m_szTeamName[0]));
  }
  
  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_IsTechBitOverridden()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_IsTechBitOverridden(short iTeam, short iBitID, VARIANT_BOOL* pVal)
{
  XLock lock(this);
  if ((unsigned short)iBitID >= c_ttbMax || (unsigned short) iTeam >= c_cSidesMax)
    return E_INVALIDARG;
  CLEAROUT(pVal, VARBOOL(m_mp.m_ttbmAltered[iTeam].GetBit(iBitID)));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_OverriddenTechBit()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_OverriddenTechBit(short iTeam, short iBitID, VARIANT_BOOL* pVal)
{
  XLock lock(this);
  if ((unsigned short)iBitID >= c_ttbMax || (unsigned short) iTeam >= c_cSidesMax)
    return E_INVALIDARG;
  CLEAROUT(pVal, VARBOOL(m_mp.m_ttbmNewSetting[iTeam].GetBit(iBitID)));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_OverriddenTechBit()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_OverriddenTechBit(short iTeam, short iBitID, VARIANT_BOOL newVal)
{
  XLock lock(this);
  if ((unsigned short)iBitID >= c_ttbMax || (unsigned short) iTeam >= c_cSidesMax)
    return E_INVALIDARG;
  m_mp.m_ttbmAltered[iTeam].SetBit(iBitID);
  if(newVal)
    m_mp.m_ttbmNewSetting[iTeam].SetBit(iBitID);
  else
    m_mp.m_ttbmNewSetting[iTeam].ClearBit(iBitID);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_SetOverriddenTechBitRange()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_SetOverriddenTechBitRange(short iTeam, short iBitID_First, short iBitID_Last, VARIANT_BOOL newVal)
{
  XLock lock(this);
  if ((unsigned short)iBitID_First >= c_ttbMax || (unsigned short)iBitID_Last >= c_ttbMax || (unsigned short) iTeam >= c_cSidesMax)
    return E_INVALIDARG;
 
  if(newVal)
  {
    for (int i = iBitID_First; i <= iBitID_Last; ++i)
    {
      m_mp.m_ttbmAltered[iTeam].SetBit(i);
      m_mp.m_ttbmNewSetting[iTeam].SetBit(i);
    }
  }
  else
  {
    for (int i = iBitID_First; i <= iBitID_Last; ++i)
    {
      m_mp.m_ttbmAltered[iTeam].SetBit(i);
      m_mp.m_ttbmNewSetting[iTeam].ClearBit(i);
    }
  }
  return S_OK;
}




/*-------------------------------------------------------------------------
 * put_IsGoalFlags()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_IsGoalFlags(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.IsFlagsGame()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_IsGoalFlags()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_IsGoalFlags(VARIANT_BOOL newVal)
{
  XLock lock(this);
  if (!newVal)
    m_mp.nGoalFlagsCount = 0;
  else 
  {
    // set to 10 only if needed--don't override another "true" value of nGoalFlagsCount with 10
    if (m_mp.nGoalFlagsCount == 0)
      m_mp.nGoalFlagsCount = 10;
  }
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_GoalFlagsCount()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_GoalFlagsCount(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.nGoalFlagsCount);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_GoalFlagsCount()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_GoalFlagsCount(short Val)
{
  XLock lock(this);
  m_mp.nGoalFlagsCount = Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_StoryText()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_StoryText(BSTR* pbstrStory)
{
  XLock lock(this);
  CComBSTR bstr(m_bstrStoryText);
  CLEAROUT(pbstrStory, (BSTR)bstr);
  bstr.Detach();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_StoryText()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_StoryText(BSTR bstrStory)
{
  XLock lock(this);
  m_bstrStoryText = bstrStory;
  return S_OK;
}




/*-------------------------------------------------------------------------
 * get_AllowEmptyTeams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowEmptyTeams(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowEmptyTeams));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowEmptyTeams()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowEmptyTeams(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowEmptyTeams = !!Val;
  return S_OK;
}




/*-------------------------------------------------------------------------
 * get_AutoStart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_AutoStart(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAutoStart));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AutoStart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_AutoStart(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bAutoStart = !!newVal;
  return S_OK;
}




/*-------------------------------------------------------------------------
 * get_AllowRestart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_AllowRestart(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowRestart));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowRestart()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_AllowRestart(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bAllowRestart = !!newVal;
  return S_OK;
}

/*-------------------------------------------------------------------------
 * get_Experimental() // mmf 10/07 Experimental game type
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::get_Experimental(VARIANT_BOOL *pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bExperimental));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_Experimental() // mmf 10/07 Experimental game type
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAGCGameParameters::put_Experimental(VARIANT_BOOL newVal)
{
  XLock lock(this);
  m_mp.bExperimental = !!newVal;
  return S_OK;
}

/*-------------------------------------------------------------------------
 * get_AllowShipyardPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowShipyardPath(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowShipyardPath));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowShipyardPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowShipyardPath(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowShipyardPath = !!Val;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_AllowSupremacyPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowSupremacyPath(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowSupremacyPath));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowSupremacyPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowSupremacyPath(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowSupremacyPath = !!Val;
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_AllowTacticalPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowTacticalPath(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowTacticalPath));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowTacticalPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowTacticalPath(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowTacticalPath = !!Val;
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_AllowExpansionPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAGCGameParameters property.
 * 
 */
STDMETHODIMP CAGCGameParameters::get_AllowExpansionPath(VARIANT_BOOL* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, VARBOOL(m_mp.bAllowExpansionPath));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * put_AllowExpansionPath()
 *-------------------------------------------------------------------------
 * Purpose:
 *    If this CAGCGameParameters is unattached to an existing mission,
 *    then set this property.  Otherwise return an error.
 * 
 */
STDMETHODIMP CAGCGameParameters::put_AllowExpansionPath(VARIANT_BOOL Val)
{
  XLock lock(this);
  m_mp.bAllowExpansionPath = !!Val;
  return S_OK;
}


STDMETHODIMP CAGCGameParameters::get_MaxImbalance(short* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_mp.iMaxImbalance);
  return S_OK;
}

STDMETHODIMP CAGCGameParameters::put_MaxImbalance(short Val)
{
  XLock lock(this);
  m_mp.iMaxImbalance = Val;
  return S_OK;
}

