#pragma once

#ifndef __ClubSite_h__
#define __ClubSite_h__

/////////////////////////////////////////////////////////////////////////////
// AllClubSite.h
//

class ATL_NO_VTABLE IClubSite
{
public:
  virtual int LogEvent(WORD wType, LPCTSTR pFormat, ...) = 0;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__AllClubAppSite_h__
