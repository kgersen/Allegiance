#ifndef __NetApi_h__
#define __NetApi_h__

/////////////////////////////////////////////////////////////////////////////

#include <lmerr.h>
#include <lmcons.h>
#include <lmapibuf.h>
#include <lmaccess.h>
#include <lmserver.h>
#include <ntsecapi.h>
#include <..\TCLib\AutoHandle.h>


/////////////////////////////////////////////////////////////////////////////
// {secret}
struct TCLsaHandleClose
{
  bool operator()(LSA_HANDLE h) {return LsaClose(h) >= 0;}
};

typedef TCAutoHandle<LSA_HANDLE, TCLsaHandleClose> TCLsaHandle;


/////////////////////////////////////////////////////////////////////////////
// {secret}
struct TCNetApiPtrClose
{
  bool operator()(void* h) {return NERR_Success == NetApiBufferFree(h);}
};

/////////////////////////////////////////////////////////////////////////////
// Declares a TCAutoHandle that encapsulates a WinNT NetApiBuffer type and
// will release the buffer using the WinNT NetApiBufferFree API.
// 
// See Also: TCAutoHandle, DECLARE_TCPtr
//
DECLARE_TCPtr(TCNetApiPtr, TCNetApiPtrClose)


/////////////////////////////////////////////////////////////////////////////

#endif // !__NetApi_h__
