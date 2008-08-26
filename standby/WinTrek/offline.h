
#ifndef _OFFLINE_H_
#define _OFFLINE_H_

//VOID LoadDefaultMap(ImissionIGC * pCore, bool fSwarm = false, SideID = 2);
VOID LoadMDLMap(WinTrekClient * pClient, const ZString& strMap,
                    bool fSwarm = false, SideID = 2);

#endif

