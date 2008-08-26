
#ifndef _CNTRMAP_H_
#define _CNTRMAP_H_

//
// Global defines for the PERF_OBJECT_MAP structure.
//

#define MAX_PERF_OBJECTS            20
#define MAX_SERVICE_NAME_LENGTH     256
#define PERF_OBJ_FILE               L"perfdll.ini"

typedef unsigned long PERFMON_ID;

typedef struct _PERF_COUNTER_MAP
{
    PERFMON_ID  mPerfId;
    DWORD       mdwCounterType;
} PERF_COUNTER_MAP, *PPERF_COUNTER_MAP;


typedef struct _PERF_OBJECT_MAP
{
    TCHAR               mszProgramName[MAX_SERVICE_NAME_LENGTH];

    //
    // mPerfId should be defined in counters.h
    //
    PERFMON_ID          mPerfId;

    //
    // mdwCounters is number of counters for this object.
    //
    DWORD               mdwCounters;
    DWORD               mdwFirstCounter;

    //
    // The counters for this object (max defined above).
    //
    PERF_COUNTER_MAP *  mCounter;
} PERF_OBJECT_MAP, *PPERF_OBJECT_MAP;


typedef struct _PERF_OBJECT_TYPE_MAP
{
    PERF_OBJECT_TYPE        mObject;
    PERF_COUNTER_DEFINITION mCounter[1];
} PERF_OBJECT_TYPE_MAP, *PPERF_OBJECT_TYPE_MAP;


extern PERF_OBJECT_MAP gPerfObject[MAX_PERF_OBJECTS];
extern DWORD gdwNumberOfPerfObjects;

//
// Interface to this module.
//
BOOL LoadPerfObjects();

#endif

