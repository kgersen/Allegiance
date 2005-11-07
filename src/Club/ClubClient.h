/*-------------------------------------------------------------------------
  client.h
  
  Per client stuff
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _CLIENT_H_
#define _CLIENT_H_



class CFLClient : public IObject
{
public:
  CFLClient(CFMConnection * pcnxn) :
  m_dwID(c_dwID),
  m_pcnxn(pcnxn)
  {
    assert(pcnxn);
    m_pcnxn->SetPrivateData((DWORD) this); // set up two-way link between connection and this
  }
  ~CFLClient()
  {
    m_pcnxn->SetPrivateData(0); // disconnect two-way link between connection and this
  }
  bool IsValidThisPtr() // this
  {
    bool fValid = this && (c_dwID == m_dwID);
    assert (fValid); // this should only be false if either a bug in the client, or hacked client
    return fValid;
  }
  static CFLClient * FromConnection(CFMConnection & cnxn) 
  {
    return (CFLClient *)cnxn.GetPrivateData();
  }
  CFMConnection * GetConnection()
  {
    return m_pcnxn;
  }

  int GetZoneID() 
  { 
    return m_nMemberID; 
  }

  void SetZoneID(int idZone) 
  { 
    m_nMemberID = idZone; 
  }

  bool GetClubMember()
  {
    return m_fClubMember;
  }

  void SetClubMember(bool fMember)
  {
    m_fClubMember = fMember;
  }

private:
  static const DWORD c_dwID;
  char            m_szName[c_cbNameDB];
  CFMConnection * m_pcnxn;
  DWORD           m_dwID;
  int             m_nMemberID;
  bool            m_fClubMember : 1; // if they're not a club member, they can still come in, but not do stuff
};

#endif

