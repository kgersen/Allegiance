
/*-------------------------------------------------------------------------
 * fedsrv\AdminShip.CPP
 * 
 * Implementation of CAdminShip.  
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminShip

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminShip)


/////////////////////////////////////////////////////////////////////////////
// Construction

CAdminShip::CAdminShip()
{
}

                                                               

/*-------------------------------------------------------------------------
 * get_Pilot()
 *-------------------------------------------------------------------------
 * Output:
 *   a pointer to an User that is piloting this ship
 */
STDMETHODIMP CAdminShip::get_User(IAdminUser** ppUser)
{
  CFSShip* pfsShip = (CFSShip*)GetIGC()->GetPrivateData(); // Igc2Host(GetIGC());

  if (pfsShip->IsPlayer())
  {
    CFSPlayer *pfsPlayer = pfsShip->GetPlayer();

    RETURN_FAILED (pfsPlayer->CAdminSponsor<CAdminUser>::Make(IID_IAdminUser, (void**)ppUser));

    pfsPlayer->CAdminSponsor<CAdminUser>::GetLimb()->Init(pfsPlayer);
  }
  else
  {
    *ppUser = NULL; 
    //  CLEAROUT(ppUser, (IAdminUser*)NULL);
  }

  return S_OK;
}
