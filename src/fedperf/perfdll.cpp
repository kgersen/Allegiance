
#include "pch.h"

#define MAX(a, b)           (((a) > (b)) ? (a) : (b))

//
// Global variables.
//
LONG glOpenCount = 0;
CPerfShare gPerfShare;
PSHARE_HEADER gpHeader = NULL;
BOOL gbInitialized = FALSE;


BOOL HandlerRoutine(DWORD dwCtrlType)
{
    //
    // This function is called in case of a CTRL-C or other
    // interrupt that will kill this process.
    //
    gPerfShare.Terminate();
    return(FALSE);
}


VOID InitializePerfObject(PPERF_OBJECT_MAP pPerfObject)
{
    HKEY hRegKey;
    LONG lStatus;
    DWORD dwFirstCounter, dwType, dwSize;
    WCHAR szRegKey[2048];

    //
    // Counter information is stored in HKLM\CCS\Services\<Name>\<SubKey>.
    //
    swprintf(szRegKey, L"System\\CurrentControlSet\\Services\\%s\\Performance",
        pPerfObject->mszProgramName);
    lStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,          // Hive
                            szRegKey,                   // SubKey
                            0L,                         // Reserved
                            KEY_READ,                   // Access Rights
                            &hRegKey);                  // Hkey

    if (lStatus != ERROR_SUCCESS)
    {
        pPerfObject->mdwCounters = 0;
        return;
    }

    //
    // Get the first counter offset.
    //
    dwSize = sizeof(DWORD);
    lStatus = RegQueryValueEx(hRegKey,                  // SubKey
                            FIRST_COUNTER,              // Value name
                            0L,                         // Reserved
                            &dwType,                    // Value type
                            (LPBYTE) &dwFirstCounter,   // Destination
                            &dwSize);                   // Size of buffer
    if ((lStatus != ERROR_SUCCESS) || (dwType != REG_DWORD))
    {
        pPerfObject->mdwCounters = 0;
        RegCloseKey(hRegKey);
        return;
    }
    RegCloseKey(hRegKey);

    //
    // Modify the global structure to hold the absolute offsets.
    //
    pPerfObject->mPerfId += dwFirstCounter;
    pPerfObject->mdwFirstCounter = dwFirstCounter;
    for(dwType = 0; dwType < pPerfObject->mdwCounters; dwType++)
        pPerfObject->mCounter[dwType].mPerfId += dwFirstCounter;

}


VOID CleanGlobalCounters()
{
    DWORD dwIndex;

	for(dwIndex = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
		HeapFree(GetProcessHeap(), 0, gPerfObject[dwIndex].mCounter);
}


DWORD _stdcall InitializeThread(LPVOID lpParam)
{
    DWORD dwIndex;

    //
    // Catch CTRL-Cs and other interrupts.
    //
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) HandlerRoutine, TRUE))
    {
        return(ERROR_GEN_FAILURE);
    }

    //
    // Load the object information from disk.
    //
    if (!LoadPerfObjects())
    {
        return(ERROR_GEN_FAILURE);
    }

    if (!gPerfShare.Initialize())
    {
        CleanGlobalCounters();
        return(ERROR_GEN_FAILURE);
    }

    if (NULL == (gpHeader = gPerfShare.GetShareHeader()))
    {
        gPerfShare.Terminate();
        CleanGlobalCounters();
        return(ERROR_GEN_FAILURE);
    }

    for(dwIndex = 0; dwIndex < gdwNumberOfPerfObjects; dwIndex++)
        InitializePerfObject(&gPerfObject[dwIndex]);

    gbInitialized = TRUE;
    return(ERROR_SUCCESS);
}


extern "C" DWORD APIENTRY DwInitialize(LPWSTR lpDeviceName)
{
    HANDLE hThread;
    DWORD dwThreadId;

    //
    // Only initialize the first time we get called.
    //
    if (0 < glOpenCount++)
        return(ERROR_SUCCESS);

    if (!(hThread = CreateThread(NULL,
                                0,
                                InitializeThread,
                                NULL,
                                0,
                                &dwThreadId)))
    {
        glOpenCount--;
        return(ERROR_GEN_FAILURE);
    }
    return(ERROR_SUCCESS);

}


extern "C" DWORD APIENTRY DwCleanup(void)
{
    if ((0 == --glOpenCount) && gbInitialized)
    {
        gPerfShare.Terminate();
        CleanGlobalCounters();
        gbInitialized = FALSE;
    }

    return(ERROR_SUCCESS);
}


