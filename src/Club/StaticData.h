/*-------------------------------------------------------------------------
  StaticData.h
  
  Contains data from database that never changes, like civ ids.
  
  Owner: a-marcu
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


class IStaticData
{

public:
    virtual ~IStaticData() {} 

    virtual int GetCivCount() = 0; // returns total number of civs

    virtual CivID GetCivID(int iCiv) = 0; // iCiv must be less than GetCivCount()
};


IStaticData * CreateStaticData();

extern IStaticData * g_pStaticData;

