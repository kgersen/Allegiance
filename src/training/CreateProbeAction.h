/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateProbeAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CreateProbeAction" interface.
**
**  History:
*/
#ifndef _CREATE_PROBE_ACTION_H_
#define _CREATE_PROBE_ACTION_H_

#ifndef _CREATE_OBJECT_ACTION_H_
#include "CreateObjectAction.h"
#endif// _CREATE_OBJECT_ACTION_H_

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CreateProbeAction : public CreateObjectAction
    {
        public:
                    /* void */          CreateProbeAction (ProbeID probeID, ExpendableTypeID typeID, const Vector& position, SideID sideID, SectorID sectorID);
            virtual /* void */          ~CreateProbeAction (void);
            virtual void                Execute (void);

        protected:
                    ProbeID             m_probeID;
                    ExpendableTypeID    m_typeID;
                    Vector              m_position;
                    SideID              m_sideID;
                    SectorID            m_sectorID;
                    DataProbeIGC        m_probeData;
    };

    //------------------------------------------------------------------------------
}

#endif  //_CREATE_PROBE_ACTION_H_
