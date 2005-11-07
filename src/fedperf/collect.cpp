
#include "pch.h"

extern CPerfShare gPerfShare;
extern PSHARE_HEADER gpHeader;
extern BOOL gbInitialized;



BOOL HandleNonCases(LPWSTR lpRequestType,
                    LPVOID *lppData,
                    LPDWORD lpcbTotalBytes,
                    LPDWORD lpcObjectTypes,
                    PBOOL   bMask)
{
    DWORD dwQueryType, dwIndex;
    BOOL  bFound;

    //
    // Make sure we initialized ok.
    //
    if ((NULL == gpHeader) || !gbInitialized)
    {
        *lpcbTotalBytes = (DWORD) 0;
        *lpcObjectTypes = (DWORD) 0;
        return(TRUE);
    }

    //
    // See if this is a non-NT data request.
    //
    dwQueryType = GetQueryType(lpRequestType);
    if (QUERY_FOREIGN == dwQueryType)
    {
        *lpcbTotalBytes = (DWORD) 0;
        *lpcObjectTypes = (DWORD) 0;
        return(TRUE);
    }

    //
    // Make sure we're requesting one of our objects.
    //
    ZeroMemory(bMask, sizeof(BOOL) * MAX_PERF_OBJECTS);
    bFound = FALSE;
    if (QUERY_ITEMS == dwQueryType)
    {
        for(dwIndex = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
        {
            if ((IsNumberInUnicodeList(gPerfObject[dwIndex].mPerfId,
                lpRequestType)) && (gPerfObject[dwIndex].mdwCounters > 0))
            {
                bMask[dwIndex] = TRUE;
                bFound = TRUE;
            }
        }

        if (!bFound)
        {
            *lpcbTotalBytes = (DWORD) 0;
            *lpcObjectTypes = (DWORD) 0;
            return(TRUE);
        }
    } else
    {
        for(dwIndex = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
            bMask[dwIndex] = TRUE;
    }

    return(FALSE);
}


DWORD ComputeSpaceNeeded(PBOOL bMask)
{
    DWORD dwIndex, dwLoop, dwTemp, dwSpaceNeeded[MAX_PERF_OBJECTS];
    PSHARE_INSTANCE pInstance;

    //
    // Go through all the instances and see how much they're taking
    // up.  We keep totals for each object type.
    //
    ZeroMemory(dwSpaceNeeded, sizeof(DWORD) * MAX_PERF_OBJECTS);
    pInstance = (PSHARE_INSTANCE) (gpHeader + 1);

    for(dwLoop = gpHeader->dwInstanceHeader; dwLoop != DWORD_NULL;
        dwLoop = pInstance[dwLoop].dwNextInList)
    {
        for(dwIndex = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
        {
            if (gPerfObject[dwIndex].mdwFirstCounter ==
                pInstance[dwLoop].dwFirstCounter)
            {
                if (0 < (dwTemp = wcslen(pInstance[dwLoop].wszInstanceName)))
                    dwTemp = (dwTemp + 1) * sizeof(WCHAR);
                dwSpaceNeeded[dwIndex] += (sizeof(PERF_INSTANCE_DEFINITION) +
                                            DWORD_MULTIPLE(dwTemp) +
                                            sizeof(PERF_COUNTER_BLOCK) +
                                            pInstance[dwLoop].dwDataSize);
                                            
                break;
            }
        }
    }

    //
    // We have running totals for each object.  Now tack on the object
    // and counter specific data.
    //
    for(dwIndex = dwTemp = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
    {
        // if (dwSpaceNeeded[dwIndex] > 0)
        if (bMask[dwIndex])
        {
            dwTemp += (dwSpaceNeeded[dwIndex] +
                        sizeof(PERF_OBJECT_TYPE) +
                        sizeof(PERF_COUNTER_DEFINITION) *
                        gPerfObject[dwIndex].mdwCounters);
			if (!dwSpaceNeeded[dwIndex])
			{
				dwTemp += sizeof(PERF_INSTANCE_DEFINITION) +
						sizeof(PERF_COUNTER_BLOCK) +
						MAX_BYTES_FOR_INSTANCE_COUNTERS;
			}
        }
        // else
        //    bMask[dwIndex] = FALSE;
    }

    return(dwTemp);
}


VOID BuildPerfmonObjectBase(PPERF_OBJECT_MAP pObject,
                            PPERF_OBJECT_TYPE_MAP pMap)
{
    pMap->mObject.DefinitionLength = sizeof(PERF_OBJECT_TYPE) +
                                    sizeof(PERF_COUNTER_DEFINITION) *
                                    pObject->mdwCounters;
    pMap->mObject.HeaderLength = sizeof(PERF_OBJECT_TYPE);
    pMap->mObject.ObjectNameTitleIndex = pObject->mPerfId;
    pMap->mObject.ObjectNameTitle = 0;
    pMap->mObject.ObjectHelpTitleIndex = pObject->mPerfId + 1;
    pMap->mObject.ObjectHelpTitle = 0;
    pMap->mObject.DetailLevel = PERF_DETAIL_NOVICE;
    pMap->mObject.NumCounters = pObject->mdwCounters;
    pMap->mObject.NumInstances = 0;
    pMap->mObject.CodePage = 0;                                 // Unicode
    ZeroMemory(&(pMap->mObject.PerfTime), sizeof(LARGE_INTEGER));
    ZeroMemory(&(pMap->mObject.PerfFreq), sizeof(LARGE_INTEGER));
}


VOID BuildPerfmonObjectCounters(PPERF_OBJECT_MAP pObject,
                                PPERF_OBJECT_TYPE_MAP pMap)
{
    DWORD dwIndex, dwTotal;

    dwTotal = sizeof(PERF_COUNTER_BLOCK);
    for(dwIndex = 0; dwIndex < pObject->mdwCounters; dwIndex++)
    {
        pMap->mCounter[dwIndex].ByteLength = sizeof(PERF_COUNTER_DEFINITION);
        pMap->mCounter[dwIndex].CounterNameTitleIndex =
            pObject->mCounter[dwIndex].mPerfId;
        pMap->mCounter[dwIndex].CounterNameTitle = 0;
        pMap->mCounter[dwIndex].CounterHelpTitleIndex =
            pObject->mCounter[dwIndex].mPerfId + 1;
        pMap->mCounter[dwIndex].CounterHelpTitle = 0;
        pMap->mCounter[dwIndex].DefaultScale = 0;       // 1
        pMap->mCounter[dwIndex].DetailLevel = PERF_DETAIL_NOVICE;
        pMap->mCounter[dwIndex].CounterType =
            pObject->mCounter[dwIndex].mdwCounterType;

        //
        // We need to determine the counter size.
        //
        switch(pObject->mCounter[dwIndex].mdwCounterType &
                    PERF_SIZE_VARIABLE_LEN)
        {
            case PERF_SIZE_DWORD:
                pMap->mCounter[dwIndex].CounterSize = sizeof(DWORD);
                break;
            case PERF_SIZE_LARGE:
                pMap->mCounter[dwIndex].CounterSize = sizeof(LARGE_INTEGER);
                break;
            default:
                pMap->mCounter[dwIndex].CounterSize = 0;
                break;
        }
        pMap->mCounter[dwIndex].CounterOffset = dwTotal;

        dwTotal += pMap->mCounter[dwIndex].CounterSize;
    }

}


VOID BuildPerfmonObjectInstanceStructure(PERF_INSTANCE_DEFINITION *pInstance,
                                            PWCHAR wszInstanceName)
{
    DWORD dwLength;

    if (0 < (dwLength = wcslen(wszInstanceName) * sizeof(WCHAR)))
        dwLength += sizeof(WCHAR);
    pInstance->ByteLength = sizeof(PERF_INSTANCE_DEFINITION) +
                                    DWORD_MULTIPLE(dwLength);
    pInstance->ParentObjectTitleIndex = 0;
    pInstance->ParentObjectInstance = 0;
    pInstance->UniqueID = (DWORD) PERF_NO_UNIQUE_ID;
    pInstance->NameOffset = sizeof(PERF_INSTANCE_DEFINITION);
    pInstance->NameLength = dwLength;
    if (dwLength > 0)
        wcscpy((PWCHAR) (pInstance + 1), wszInstanceName);
}


DWORD BuildPerfmonObjectInstances(PPERF_OBJECT_MAP pObject,
                                    PPERF_OBJECT_TYPE_MAP pMap,
                                    PBYTE lpData)
{
    PERF_INSTANCE_DEFINITION *pInstance;
    PERF_COUNTER_BLOCK *pBlock;
    PSHARE_INSTANCE pList;
    DWORD dwLoop;

    //
    // Go through all the instances and copy our instances over.
    //
    pInstance = (PERF_INSTANCE_DEFINITION *) lpData;
    pList = (PSHARE_INSTANCE) (gpHeader + 1);
    for(dwLoop = gpHeader->dwInstanceHeader; dwLoop != DWORD_NULL;
        dwLoop = pList[dwLoop].dwNextInList)
    {
        if (pObject->mdwFirstCounter == pList[dwLoop].dwFirstCounter)
        {
            //
            // Add the instance structure.
            //
            BuildPerfmonObjectInstanceStructure(pInstance,
                                                pList[dwLoop].wszInstanceName);

            //
            // Add the counter block.
            //
            pBlock = (PERF_COUNTER_BLOCK *) ((PBYTE) pInstance +
                                            pInstance->ByteLength);
            pBlock->ByteLength = sizeof(PERF_COUNTER_BLOCK) +
                                 pList[dwLoop].dwDataSize;

            //
            // Add the data.
            //
            CopyMemory(pBlock + 1,
                        pList[dwLoop].Data,
                        pList[dwLoop].dwDataSize);
            pMap->mObject.NumInstances++;
            pInstance = (PERF_INSTANCE_DEFINITION *) ((PBYTE) pInstance +
                                                    pInstance->ByteLength +
                                                    pBlock->ByteLength);
        }
    }

    //
    // If there are no instances, we need to include an empty instance.
    //
    if (0 == pMap->mObject.NumInstances)
    {
        BuildPerfmonObjectInstanceStructure(pInstance, L"");
        pBlock = (PERF_COUNTER_BLOCK *) ((PBYTE) pInstance +
                                        pInstance->ByteLength);
        pBlock->ByteLength = sizeof(PERF_COUNTER_BLOCK) +
                             MAX_BYTES_FOR_INSTANCE_COUNTERS;
        ZeroMemory(pBlock + 1, MAX_BYTES_FOR_INSTANCE_COUNTERS);
        pInstance = (PERF_INSTANCE_DEFINITION *) ((PBYTE) pInstance +
                                                    pInstance->ByteLength +
                                                    pBlock->ByteLength);
    }
    return((PBYTE) pInstance - lpData);
}


DWORD BuildPerfmonObject(PPERF_OBJECT_MAP pObject, LPBYTE lpData)
{
    PPERF_OBJECT_TYPE_MAP pMap;
    
    //
    // Set up the PERF_OBJECT_TYPE.
    //
    pMap = (PPERF_OBJECT_TYPE_MAP) lpData;
    BuildPerfmonObjectBase(pObject, pMap);
    BuildPerfmonObjectCounters(pObject, pMap);
    lpData += pMap->mObject.DefinitionLength;

    //
    // Add the instances and their counters.
    //
    lpData += BuildPerfmonObjectInstances(pObject, pMap, lpData);
    pMap->mObject.TotalByteLength = (DWORD) (lpData - (PBYTE) pMap);

    return(pMap->mObject.TotalByteLength);
}


DWORD BuildPerfmonBuffer(PBOOL bMask, LPBYTE *lppData, DWORD dwSpaceNeeded)
{
    DWORD dwIndex, dwCount;

    for(dwIndex = dwCount = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
    {
        if (bMask[dwIndex])
        {
            *lppData += BuildPerfmonObject(&gPerfObject[dwIndex], *lppData);
            dwCount++;
        }
    }

    return(dwCount);
}


extern "C" DWORD APIENTRY DwCollectData(LPWSTR lpRequestType,
                                        LPVOID *lppData,
                                        LPDWORD lpcbTotalBytes,
                                        LPDWORD lpcObjectTypes)
{
    DWORD dwSpaceNeeded;
    BOOL  bMask[MAX_PERF_OBJECTS];
    PBYTE pStart;

    if (HandleNonCases(lpRequestType,
                        lppData,
                        lpcbTotalBytes,
                        lpcObjectTypes,
                        bMask))
    {
        return(ERROR_SUCCESS);
    }


    //
    // We have one or more objects that we need to report.
    //

    //
    // Lock the shared memory.
    //
    if (!gPerfShare.Lock())
    {
        *lpcbTotalBytes = (DWORD) 0;
        *lpcObjectTypes = (DWORD) 0;
        return(ERROR_SUCCESS);
    }

    //
    // Figure out how much space we need.
    //
    dwSpaceNeeded = ComputeSpaceNeeded(bMask);
    if (*lpcbTotalBytes < dwSpaceNeeded)
    {
        gPerfShare.Unlock();
        *lpcbTotalBytes = (DWORD) 0;
        *lpcObjectTypes = (DWORD) 0;
        return(ERROR_MORE_DATA);
    }

    //
    // Fill the buffer with the objects and their data.
    //
    pStart = (PBYTE) *lppData;
    *lpcObjectTypes = BuildPerfmonBuffer(bMask,
                                        (LPBYTE *) lppData,
                                        dwSpaceNeeded);
    *lpcbTotalBytes = ((PBYTE) *lppData) - pStart;

    //
    // We've finished, let's return.
    //
    gPerfShare.Unlock();
    return(ERROR_SUCCESS);
}


