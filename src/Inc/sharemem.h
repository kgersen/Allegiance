//
//  File:   sharemem.h
//
//  Descrition:
//      This file contains structure information and a class definition
//      for managing shared memory between a test tool and a perfmon DLL.
//      Code for the class can be found in sharemem.cpp.  This file
//      does NOT need to be modified if customizing this code to your
//      personalized test tool.
//
//  History:
//      February 20, 1997       jfink       Created file.
//

#ifndef _SHAREMEM_H_
#define _SHAREMEM_H_

//
// An instance element in the shared memory.
//
#define MAX_BYTES_FOR_INSTANCE_COUNTERS     192
#define MAX_INSTANCE_NAME_LENGTH            32
#define DWORD_NULL                          ((DWORD) -1)

#define ALIGN_ON_QWORD(x) ((VOID *)( ((DWORD) x & 0x00000007) ? ( ((DWORD) x & 0xFFFFFFF8) + 8 ) : ( (DWORD) x ) ))

typedef struct _SHARE_INSTANCE
{
    WCHAR                       wszInstanceName[MAX_INSTANCE_NAME_LENGTH + 4];
    DWORD                       dwFirstCounter;
    BYTE                        Data[MAX_BYTES_FOR_INSTANCE_COUNTERS];
    DWORD                       dwDataSize;
    DWORD                       dwNextInList;
    struct _SHARE_INSTANCE *    pNextUsedByClient;
} SHARE_INSTANCE, *PSHARE_INSTANCE;


//
// The header information for the shared memory.
//
typedef struct _SHARE_HEADER
{
    DWORD               dwNumberOfInstances;
    DWORD               dwInstanceHeader;
    DWORD               dwFreeHeader;
} SHARE_HEADER, *PSHARE_HEADER;


//
// Constant definitions.
//
#define MAX_INSTANCES_OF_ALL_SHARES 1000
#define SHARE_SIZE                  (sizeof(SHARE_HEADER) +         \
                                     MAX_INSTANCES_OF_ALL_SHARES *  \
                                      sizeof(SHARE_INSTANCE))
#define FIRST_COUNTERA              "First Counter"
#define FIRST_COUNTERW              L"First Counter"
#define FIRST_COUNTER               TEXT("First Counter")

#define SHARE_FILE_NAME             TEXT("PerfDLLShareFile")
#define SHARE_MUTEX_NAME            TEXT("PerfDLLShareMutex")
#define SHARE_MUTEX_TIMEOUT         (1000L)


//
// The class for managing the shared memory.  Documentation follows.
//
class CPerfShare
{
    private:
        HANDLE          mhSharedMemory;
        HANDLE          mhSharedMutex;

        PSHARE_HEADER   mpShareHeader;
        PSHARE_INSTANCE mpShareInstance;

        LONG            mlLockCount;

        BOOL InitializeHeader();

        BOOL  GetFirstCounterValue( PCHAR szServiceName,
                                    PDWORD pdwFirst);
        BOOL  GetFirstCounterValue( PWCHAR wszServiceName,
                                    PDWORD pdwFirstCounter);
        BOOL  GetFirstCounterValueA(PCHAR  szServiceName,
                                    PDWORD pdwFirstCounter);
        BOOL  GetFirstCounterValueW(PWCHAR wszServiceName,
                                    PDWORD pdwFirstCounter);
        PVOID AllocateLocalCounters(DWORD dwDataSize);
        PVOID AllocateCounters(PWCHAR wszInstanceName, DWORD dwFirstCounter,
                                DWORD dwDataSize);

    public:
        CPerfShare();
        ~CPerfShare();

        BOOL Initialize(BOOL bReadOnly = FALSE);
        VOID Terminate();

        PVOID AllocateCounters(PWCHAR wszServiceName, PWCHAR wszInstanceName,
                                DWORD dwDataSize);
        PVOID AllocateCounters(PCHAR   szServiceName, PCHAR   szInstanceName,
                                DWORD dwDataSize);
        VOID  FreeCounters(PVOID pvCounters);

        BOOL Lock();
        VOID Unlock();
        PSHARE_HEADER GetShareHeader();
        BOOL Initialized();
} ;

typedef CPerfShare * PPerfShare;


//
//  Function definitions follow.
//

//
//  BOOL InitializeHeader()
//
//  Description:
//      Initialize the shared header.  This function is only called
//      by Initialize if the shared memory did not exist previously.
//
//  Returns:
//      TRUE on success.
//      FALSE otherwise (could not MapViewOfFile).
//

//
//  BOOL GetFirstCounterValue(PWCHAR wszServiceName, PDWORD pdwFirstCounter)
//
//  Description:
//      Given the service name, open the registry and find that service's
//      first counter value.
//
//  Inputs:
//      wszServiceName  [IN]     - The name of the service.
//      pdwFirstCounter [MODIFY] - A pointer to a DWORD to store the value.
//
//  Returns:
//      TRUE on success.
//      FALSE otherwise (value not in registry).
//

//
//  BOOL Initialize(BOOL bReadOnly = FALSE)
//
//  Description:
//      Initialize the shared memory segment and the mutex used to
//      provide process syncronization.
//
//  Inputs:
//      bReadOnly [IN] - If TRUE, the memory is opened in readonly mode.
//                       If FALSE, the memory can be written to.
//
//  Returns:
//      TRUE on success.
//      FALSE otherwise (failure to acquire shared memory or mutex).
//
//  Remarks:
//      The bReadOnly defaults to FALSE because the tools using this
//      code need to be able to write to this memory.  The perfmon DLL
//      does not need to and can Initialize with a value of TRUE.  The
//      first class instance to have initialized called is always opened
//      with the value FALSE because it has to initialize the shared
//      memory segment.
//

//
//  VOID Terminate()
//
//  Description:
//      Clean up the mess made by this class.  UnMap the shared memory,
//      close handles, etc.
//

//
//  PVOID AllocateCounters(PWCHAR wszServiceName, PWCHAR wszInstanceName)
//
//  Description:
//      Allocate a shared memory "element" for this tool.  The tool should
//      provide an instance name so that perfmon can list this name in
//      its list.
//
//  Inputs:
//      wszInstanceName [IN] - The wide character name for the instance.
//
//  Returns:
//      A pointer to the TOOL_COUNTERS structure defined by the tool
//      developer on success.
//      NULL on failure (no free elements left in the free list).
//
//  Remarks:
//      This function will return a pointer to the already existing
//      counter structure if AllocateCounters has been called previously
//      and DeallocateCounters has not been called.
//

//
//  PVOID AllocateCounters(PCHAR szServiceName, PCHAR szInstanceName)
//
//  Description:
//      Allocate a shared memory "element" for this tool.  The tool should
//      provide an instance name so that perfmon can list this name in
//      its list.
//
//  Inputs:
//      szInstanceName [IN] - The ASCII character name for the instance.
//
//  Remarks:
//      This function will return a pointer to the already existing
//      counter structure if AllocateCounters has been called previously
//      and DeallocateCounters has not been called.
//

//
//  VOID FreeCounters(PVOID pvCounters)
//
//  Description:
//      Deallocate the shared memory "element" for this tool.
//
//  Inputs:
//      pvCounters [IN] - Pointer to a previously allocated counter block.
//
//  Remarks:
//      If AllocateCounters has not been successfully called,
//      this function will do nothing.
//

//
//  BOOL Lock()
//
//  Description:
//      Lock the shared memory list header so that no other class instance
//      can modify it.
//
//  Returns:
//      TRUE on success.
//      FALSE otherwise (failure to acquire mutex, class not initialized).
//
//  Remarks:
//      This function does not need to be called by the tool developer.
//      The only reason it is public is because the perfmon DLL needs
//      to lock the list when it gathers performance data.
//
//      If the Lock() function is called multiple times by the same class
//      instance, an internal counter is incremented.  This guarantees
//      that a single class instance can not cause deadlock for the
//      entire system.  The shared memory segment will not be unlocked
//      until the Unlock() method is called an equal number of times.
//

//
//  VOID Unlock()
//
//  Description:
//      Unlock the shared memory list header so that other processes and
//      threads can modify it.
//
//  Remarks:
//      See the Lock() method.
//

//
//  PSHARE_LIST GetShareHeader()
//
//  Description:
//      Return a pointer to the shared memory list header.
//
//  Returns:
//      A pointer to the header on success.
//      NULL otherwise (class instance not initialized).
//
//  Remarks:
//      This function does not need to be called by the tool developer.
//      The only reason it is public is because the perfmon DLL needs
//      to lock the list when it gathers performance data.
//

//
//  BOOL Initialized()
//
//  Description:
//      Return a boolean indicating if the class instance has been
//      initialized successfully.
//
//  Returns:
//      TRUE on success.
//      FALSE otherwise.
//

#endif

