/*-------------------------------------------------------------------------
  StaticData.cpp

  Contains data from database that never changes, like civ ids.

  Owner: a-marcu

  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

//IStaticData * g_pStaticData = NULL;



class CStaticDataImpl :
    public IStaticData
{

public:

    CStaticDataImpl()
    {
        // TODO: make this read from database!!
        m_cCivs = 3;

        m_pCivs = new CivID[m_cCivs];

        // TODO: make this read from database!!
        m_pCivs[0] = 18;
        m_pCivs[1] = 25;
        m_pCivs[2] = 35;
    }

    ~CStaticDataImpl()
    {
        delete[] m_pCivs;
    }

    virtual int GetCivCount() // returns total number of civs
    {
        return m_cCivs;
    }

    virtual CivID GetCivID(int iCiv)
    {
        return m_pCivs[iCiv];
    }

private:

    int         m_cCivs;
    CivID *     m_pCivs;

};


IStaticData * CreateStaticData()
{
    return new CStaticDataImpl;
}
