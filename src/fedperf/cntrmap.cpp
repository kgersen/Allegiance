
#include "pch.h"


//
// Global list of objects.
//
PERF_OBJECT_MAP gPerfObject[MAX_PERF_OBJECTS];
DWORD gdwNumberOfPerfObjects = 0;
DWORD dwIndex;

void AddObject(const wchar_t * szObject, DWORD dwCounters, BOOL * rgIsPerSecond)
{
    DWORD i;
    PPERF_OBJECT_MAP pMap;
    pMap = &gPerfObject[gdwNumberOfPerfObjects++];
    wcscpy(pMap->mszProgramName, szObject);
    pMap->mPerfId = dwIndex;
    pMap->mdwCounters = dwCounters;
    dwIndex += 2;

    pMap->mCounter = (PPERF_COUNTER_MAP) HeapAlloc(GetProcessHeap(), 0,
                                                    sizeof(PERF_COUNTER_MAP) * 
                                                    pMap->mdwCounters);

    for (i = 0; i < dwCounters; i++)
    {
        pMap->mCounter[i].mPerfId = dwIndex;
        pMap->mCounter[i].mdwCounterType = rgIsPerSecond[i] ? PERF_COUNTER_COUNTER : PERF_COUNTER_RAWCOUNT;
        dwIndex += 2;
    }
}


BOOL LoadPerfObjects()
{

    dwIndex = 0;

    // ********** AllSrv ***********
    BOOL rgfPerSecondServer[] = {0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    AddObject(L"AllSrv", sizeof(rgfPerSecondServer) / sizeof(rgfPerSecondServer[0]), rgfPerSecondServer);

    // ********** AllLobby ***********
    BOOL rgfPerSecondLobby[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    AddObject(L"AllLobby", sizeof(rgfPerSecondLobby) / sizeof(rgfPerSecondLobby[0]), rgfPerSecondLobby);

    // ********** AllLobbyPerServer ***********
    BOOL rgfPerSecondLobbyPerServer[] = {0, 0, 0};
    AddObject(L"AllLobbyPerServer", 3, rgfPerSecondLobbyPerServer);

    return(TRUE);
}



