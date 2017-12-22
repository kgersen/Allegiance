
#include "pch.h"



/*-------------------------------------------------------------------------
 * CFSSide::CFSSide()
 *-------------------------------------------------------------------------
 */
CFSSide::CFSSide(TRef<IsideIGC> pSide, CFSMission * pfsmission) :
  m_pIsideIGC(pSide),
  m_pfsmission(pfsmission)
{
  m_pgrpSide = g.fm.CreateGroup(ZString(pSide->GetName()));
  m_pIsideIGC->SetPrivateData((DWORD)this); // link to IGC side
}


/*-------------------------------------------------------------------------
 * CFSSide::~CFSSide()
 *-------------------------------------------------------------------------
 */
CFSSide::~CFSSide()
{
  g.fm.DeleteGroup(m_pgrpSide);
  m_pIsideIGC->SetPrivateData((DWORD)NULL);  // unlink from IGC Side
}


/*-------------------------------------------------------------------------
 * CFSSide::IsInvited()
 *-------------------------------------------------------------------------
 */
bool CFSSide::IsInvited(CFSPlayer * pfsPlayer)
{
  if(m_pfsmission->IsSquadGame())
  {
    for (std::vector<ZString>::iterator i(m_Invitations.begin()); i != m_Invitations.end(); ++i)
    {
      //
      // Iterate through each squad that the player is on
      //
      int nNumSquads = pfsPlayer->GetSquadMembershipList()->n();
      SquadMembershipLink* pSquadLink = pfsPlayer->GetSquadMembershipList()->first();
      for (int iSquad = 0; iSquad < nNumSquads; ++iSquad, pSquadLink = pSquadLink->next())
      {
        SquadMembership* psquadmembership = (pSquadLink->data());

        if (_stricmp(psquadmembership->GetName(), PCC(static_cast<ZString>(*i))) == 0)
          return true;
      }
    }
  }
  else
  {
    for (std::vector<ZString>::iterator i(m_Invitations.begin()); i != m_Invitations.end(); ++i)
    {
      if (_stricmp(PCC(static_cast<ZString>(*i)), pfsPlayer->GetName()) == 0)
        return true;
    }
  }
  return false;
}

/*-------------------------------------------------------------------------
 * CFSSide::AddInvitation()
 *-------------------------------------------------------------------------
 */
void CFSSide::AddInvitation(const char * szSubjectName)
{
  if (m_pfsmission->IsSquadGame())
  {
    if(m_Invitations.size() > 0)
      m_Invitations.clear();

    m_pfsmission->SetSideName(m_pIsideIGC->GetObjectID(), szSubjectName);
  }
  m_Invitations.push_back(ZString(szSubjectName));
}

/*-------------------------------------------------------------------------
 * CFSSide::GetInvitedSquadName()
 *-------------------------------------------------------------------------
 */
const char*  CFSSide::GetInvitedSquadName()
{
  if (m_Invitations.size() == 0)
    return NULL;
  return *(m_Invitations.begin());
}
