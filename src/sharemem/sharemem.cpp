
#include "pch.h"

CPerfShare::CPerfShare()
{
    mhSharedMemory  = NULL;
    mhSharedMutex   = NULL;
    mpShareHeader   = NULL;
    mpShareInstance = NULL;
    mlLockCount     = 0;
}


CPerfShare::~CPerfShare()
{
    this->Terminate();
}


BOOL CPerfShare::Lock()
{
    BOOL fResult;
    LONG lTemp;

    fResult = FALSE;
    if (TRUE == this->Initialized())
    {
        lTemp = InterlockedIncrement(&mlLockCount);
        if (lTemp > 1)
            fResult = TRUE;
        else if (WAIT_FAILED != WaitForSingleObject(mhSharedMutex,
                                                    SHARE_MUTEX_TIMEOUT))
        {
            fResult = TRUE;
        }
    }
    
    return(fResult);
}


VOID CPerfShare::Unlock()
{
    LONG lTemp;

    if (TRUE == this->Initialized())
    {
        lTemp = InterlockedDecrement(&mlLockCount);
        if (0 == lTemp)
            ReleaseMutex(mhSharedMutex);
    }
}


BOOL CPerfShare::InitializeHeader()
{
    PSHARE_INSTANCE pInstance;
    DWORD dwIndex;

    //
    // Map the file.
    //
    mpShareHeader = (PSHARE_HEADER) MapViewOfFile(
                                        mhSharedMemory,     // Memory Handle
                                        FILE_MAP_WRITE,     // Permissions
                                        0,                  // Start Offset Hi
                                        0,                  // Start Offset Low
                                        0);                 // Map all of it
    if (NULL == mpShareHeader)
        return(FALSE);

    //
    // Initialize the queue.
    //
    mpShareHeader->dwNumberOfInstances = 0;
    mpShareHeader->dwInstanceHeader = DWORD_NULL;
    mpShareHeader->dwFreeHeader = 0;

    pInstance = (PSHARE_INSTANCE) (mpShareHeader + 1);
    for(dwIndex = 0; dwIndex < MAX_INSTANCES_OF_ALL_SHARES - 1; dwIndex++)
    {
        pInstance->dwNextInList = dwIndex + 1;
        pInstance++;
    }
    pInstance->dwNextInList = DWORD_NULL;

    return(TRUE);
}


BOOL CPerfShare::Initialize(BOOL bReadOnly)
{
    LONG lError;

    if (TRUE == this->Initialized())
        return(FALSE);

    // Create a NULL dacl to give "everyone" access
    SECURITY_DESCRIPTOR sd;
    SECURITY_ATTRIBUTES sa = {sizeof(sa), &sd, false};
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, true, NULL, FALSE);

    //
    // Create/access the shared memory queue.
    //
    mhSharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, // OS paging file
                                        &sa,                // Security
                                        PAGE_READWRITE,     // Permissions
                                        0,                  // Max Size High
                                        SHARE_SIZE,         // Max Size Low
                                        SHARE_FILE_NAME);   // Name of File

    lError = GetLastError();
    if (NULL == mhSharedMemory)
        return(FALSE);

    //
    // Create/access the shared mutex for syncronization.
    //
    mhSharedMutex = CreateMutex(&sa,                        // Security
                                FALSE,                      // Initial Owner
                                SHARE_MUTEX_NAME);          // Name of Mutex

    if (NULL == mhSharedMutex)
        return(FALSE);

    //
    // See if we need to initialize the global structure.
    //
    if (ERROR_ALREADY_EXISTS != lError)
    {
        if (FALSE == this->InitializeHeader())
            return(FALSE);
    } else
    {
        //
        // Map the file.
        //
        mpShareHeader = (PSHARE_HEADER) MapViewOfFile(
                                        mhSharedMemory,     // Memory Handl
                                        bReadOnly ?         // Permissions
                                        FILE_MAP_READ :
                                        FILE_MAP_WRITE,
                                        0,                  // Start Offset Hi
                                        0,                  // Start Offset Low
                                        0);                 // Map all of it
        if (NULL == mpShareHeader)
            return(FALSE);
    }

    return(TRUE);
}


VOID CPerfShare::Terminate()
{
    SHARE_INSTANCE * pTempInstance;

    while(NULL != mpShareInstance)
    {
        pTempInstance = mpShareInstance;
        mpShareInstance = mpShareInstance->pNextUsedByClient;
        this->FreeCounters(pTempInstance->Data);
    }
    if (mpShareHeader != NULL)
    {
        UnmapViewOfFile(mpShareHeader);
        mpShareHeader = NULL;
    }
    if (mhSharedMutex != NULL)
    {
        CloseHandle(mhSharedMutex);
        mhSharedMutex = NULL;
    }
    if (mhSharedMemory != NULL)
    {
        CloseHandle(mhSharedMemory);
        mhSharedMemory = NULL;
    }
}


BOOL CPerfShare::GetFirstCounterValue(PCHAR szServiceName, PDWORD pdwFirst)
{
    HKEY hRegKey;
    DWORD dwSize, dwType;
    LONG lStatus;

    lStatus = RegOpenKeyExA(HKEY_LOCAL_MACHINE,         // Hive
                            szServiceName,              // Subkey
                            0L,                         // Reserved
                            KEY_READ,                   // Access Rights
                            &hRegKey);                  // Hkey

    if (ERROR_SUCCESS != lStatus)
        return(FALSE);

    if (pdwFirst)
    {
        dwSize = sizeof(DWORD);
        lStatus = RegQueryValueExA(hRegKey,             // SubKey
                                    FIRST_COUNTERA,     // Value name
                                    0L,                 // Reserved
                                    &dwType,            // Value Type
                                    (LPBYTE) pdwFirst,  // Destination
                                    &dwSize);           // Size of buffer

        if ((ERROR_SUCCESS != lStatus) || (REG_DWORD != dwType))
        {
            RegCloseKey(hRegKey);
            return(FALSE);
        }
    }

    RegCloseKey(hRegKey);
    return(TRUE);
}


BOOL CPerfShare::GetFirstCounterValue(PWCHAR wszServiceName, PDWORD pdwFirst)
{
    HKEY hRegKey;
    DWORD dwSize, dwType;
    LONG lStatus;

    lStatus = RegOpenKeyExW(HKEY_LOCAL_MACHINE,         // Hive
                            wszServiceName,             // Subkey
                            0L,                         // Reserved
                            KEY_READ,                   // Access Rights
                            &hRegKey);                  // Hkey

    if (ERROR_SUCCESS != lStatus)
        return(FALSE);

    if (pdwFirst)
    {
        dwSize = sizeof(DWORD);
        lStatus = RegQueryValueExW(hRegKey,             // SubKey
                                    FIRST_COUNTERW,     // Value name
                                    0L,                 // Reserved
                                    &dwType,            // Value Type
                                    (LPBYTE) pdwFirst,  // Destination
                                    &dwSize);           // Size of buffer

        if ((ERROR_SUCCESS != lStatus) || (REG_DWORD != dwType))
        {
            RegCloseKey(hRegKey);
            return(FALSE);
        }
    }

    RegCloseKey(hRegKey);
    return(TRUE);
}


BOOL CPerfShare::GetFirstCounterValueA(PCHAR szServiceName, PDWORD pdwFirst)
{
    CHAR szSubKey[2048];

    sprintf(szSubKey, "System\\CurrentControlSet\\Services\\%s\\Performance",
        szServiceName);
    return(GetFirstCounterValue(szSubKey, pdwFirst));
}


BOOL CPerfShare::GetFirstCounterValueW(PWCHAR wszServiceName, PDWORD pdwFirst)
{
    WCHAR wszSubKey[2048];

    swprintf(wszSubKey, L"System\\CurrentControlSet\\Services\\%s\\Performance",
        wszServiceName);
    return(GetFirstCounterValue(wszSubKey, pdwFirst));
}


PVOID CPerfShare::AllocateLocalCounters(DWORD dwDataSize)
{
    SHARE_INSTANCE * pShareInstance;

    if (dwDataSize > MAX_BYTES_FOR_INSTANCE_COUNTERS)
        return(NULL);

    if (FALSE == this->Lock())
        return(NULL);

    pShareInstance = new SHARE_INSTANCE;
    if (NULL != pShareInstance)
    {
        *(pShareInstance->wszInstanceName) = L'\0';
        pShareInstance->dwFirstCounter = 0xFFFF;
        pShareInstance->dwDataSize = MAX_BYTES_FOR_INSTANCE_COUNTERS;
        pShareInstance->dwNextInList = DWORD_NULL;
        pShareInstance->pNextUsedByClient = mpShareInstance;
        mpShareInstance = pShareInstance;
    } else
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);

    this->Unlock();
    return(pShareInstance);
}


PVOID CPerfShare::AllocateCounters(PWCHAR wszInstanceName,
                                    DWORD dwFirstCounter,
                                    DWORD dwDataSize)
{
    SHARE_INSTANCE * pShareInstance;

    if (dwDataSize > MAX_BYTES_FOR_INSTANCE_COUNTERS)
        return(NULL);

    if (FALSE == this->Lock())
        return(NULL);

    if (DWORD_NULL == mpShareHeader->dwFreeHeader)
    {
        this->Unlock();
        return(NULL);
    }

    //
    // Pop the first element off the free list.
    //
    pShareInstance = (PSHARE_INSTANCE) (mpShareHeader + 1);
    pShareInstance += mpShareHeader->dwFreeHeader;
    mpShareHeader->dwFreeHeader = pShareInstance->dwNextInList;

    //
    // Push the element onto the use list.
    //
    pShareInstance->dwNextInList = mpShareHeader->dwInstanceHeader;
    mpShareHeader->dwInstanceHeader = (pShareInstance -
                                    (PSHARE_INSTANCE) (mpShareHeader + 1));
    mpShareHeader->dwNumberOfInstances++;

    //
    // Initialize the instance.
    //
    ZeroMemory(pShareInstance->Data, MAX_BYTES_FOR_INSTANCE_COUNTERS);
    pShareInstance->wszInstanceName[MAX_INSTANCE_NAME_LENGTH] = 0;
    wcsncpy(pShareInstance->wszInstanceName,
            wszInstanceName,
            MAX_INSTANCE_NAME_LENGTH);
    pShareInstance->dwFirstCounter = dwFirstCounter;
    pShareInstance->dwDataSize = dwDataSize;

    //
    // Throw the instance onto our local in use list.
    //
    pShareInstance->pNextUsedByClient = mpShareInstance;
    mpShareInstance = pShareInstance;

    this->Unlock();
    return(mpShareInstance->Data);
}


PVOID CPerfShare::AllocateCounters(PWCHAR wszServiceName,
                                    PWCHAR wszInstanceName,
                                    DWORD dwDataSize)
{
    DWORD dwFirstCounter;
    PVOID pTemp;

    //
    // We need some kind of tracking ID in the DLL to differentiate
    // between different objects.  We require the tool to give us
    // the service name, and we'll use the FirstCounter Id as our
    // unique value.  Since this could fail, we do it first so that
    // we know things later are ok.
    //
    if (FALSE == this->GetFirstCounterValueW(wszServiceName, &dwFirstCounter))
        return(this->AllocateLocalCounters(dwDataSize));

    pTemp = this->AllocateCounters(wszInstanceName,
                                    dwFirstCounter,
                                    dwDataSize);
    return(pTemp);
}


PVOID CPerfShare::AllocateCounters(PCHAR szServiceName,
                                    PCHAR szInstanceName,
                                    DWORD dwDataSize)
{
    PVOID pTemp;
    WCHAR wszInstanceName[MAX_INSTANCE_NAME_LENGTH + 4];
    DWORD dwFirstCounter;
    INT i;

    if (FALSE == this->GetFirstCounterValueA(szServiceName, &dwFirstCounter))
        return(this->AllocateLocalCounters(dwDataSize));

    i = MultiByteToWideChar(
                            CP_ACP,             // Source code page
                            0,                  // Flags
                            szInstanceName,     // Source
                            -1,                 // NULL terminated
                            wszInstanceName,    // Destination
                            MAX_INSTANCE_NAME_LENGTH + 1);

    if (0 == i)
    {
        return(NULL);
    }

    pTemp = this->AllocateCounters(wszInstanceName,
                                    dwFirstCounter,
                                    dwDataSize);
    return(pTemp);
}


VOID CPerfShare::FreeCounters(PVOID pvCounters)
{
    PSHARE_INSTANCE pInstance, pLoop, pPrev;
    DWORD *pdwLoop, dwFind;


    if (FALSE == this->Lock())
        return;

    //
    // Find the element in our list.
    //
    pPrev = NULL;
    for(pLoop = mpShareInstance; NULL != pLoop;
        pLoop = pLoop->pNextUsedByClient)
    {
        if (pLoop->Data == pvCounters)
            break;
        pPrev = pLoop;
    }

    if (NULL == pLoop)
    {
        this->Unlock();
        return;
    }

    //
    // Now remove it from our internal list.
    //
    if (NULL == pPrev)
        mpShareInstance = pLoop->pNextUsedByClient;
    else
        pPrev->pNextUsedByClient = pLoop->pNextUsedByClient;
    pLoop->pNextUsedByClient = NULL;

    //
    // Find the counter element on the use list and remove it.
    //
    pInstance = (PSHARE_INSTANCE) (mpShareHeader + 1);
    dwFind = (pLoop - pInstance);

    for(pdwLoop = &(mpShareHeader->dwInstanceHeader); *pdwLoop != DWORD_NULL;
        pdwLoop = &(pInstance[*pdwLoop].dwNextInList))
    {
        if (*pdwLoop == dwFind)
            break;
    }

    //
    // Make sure we really found it.
    //
    if (DWORD_NULL == *pdwLoop)
    {
        this->Unlock();
        return;
    }

    *pdwLoop = pInstance[dwFind].dwNextInList;
    mpShareHeader->dwNumberOfInstances--;

    //
    // Add the node to the top of the free list.
    //
    pInstance[dwFind].dwNextInList = mpShareHeader->dwFreeHeader;
    mpShareHeader->dwFreeHeader = dwFind;

    this->Unlock();
}


PSHARE_HEADER CPerfShare::GetShareHeader()
{
    return(mpShareHeader);
}


BOOL CPerfShare::Initialized()
{
    return(mpShareHeader != NULL);
}



