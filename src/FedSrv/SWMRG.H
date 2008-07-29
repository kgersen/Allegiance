/*-------------------------------------------------------------------------
 * src\fedsrv\SWMRG.H
 * 
 * Single Writer, Multiple Reader w/ Guard
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


#define ARRAY_SIZE(Array) \
   (sizeof(Array) / sizeof((Array)[0]))

// Create a BEGINTHREADEX macro that calls the C run-time's
// _beginthreadex function. The C run-time library doesn't
// want to have any reliance on Win32 data types such as
// HANDLE. This means that a Win32 programmer needs to cast
// the return value to a HANDLE. This is terribly inconvenient,
// so I have created this macro to perform the casting.
typedef unsigned (__stdcall *PTHREAD_START) (void *);

#define BEGINTHREADEX(lpsa, cbStack, lpStartAddr, \
   lpvThreadParm, fdwCreate, lpIDThread)          \
      ((HANDLE)_beginthreadex(                    \
         (void *) (lpsa),                         \
         (unsigned) (cbStack),                    \
         (PTHREAD_START) (lpStartAddr),           \
         (void *) (lpvThreadParm),                \
         (unsigned) (fdwCreate),                  \
         (unsigned *) (lpIDThread)))

// The single-writer/multiple-reader guard 
// compound synchronization object
struct SWMRG
{
   // This mutex guards access to the other objects
   // managed by this data structure and also indicates 
   // whether any writer threads are writing.
   HANDLE hMutexNoWriter;

   // This manual-reset event is signaled when
   // no reader threads are reading.
   HANDLE hEventNoReaders;

   // This semaphore is used simply as a counter that is
   // accessible between multiple processes. It is NOT
   // used for thread synchronization.
   // The count is the number of reader threads reading.
   HANDLE hSemNumReaders;

};

typedef SWMRG * PSWMRG;


// Initializes a SWMRG structure. This structure must be 
// initialized before any writer or reader threads attempt
// to wait on it.
// The structure must be allocated by the application and 
// the structure's address is passed as the first parameter.
// The lpszName parameter is the name of the object. Pass
// NULL if you do not want to share the object.
bool FSWMRGInitialize (PSWMRG pSWMRG, LPCTSTR lpszName);


// Deletes the system resources associated with a SWMRG 
// structure. The structure must be deleted only when
// no writer or reader threads in the calling process
// will wait on it.
void  SWMRGDelete (PSWMRG pSWMRG);


// A writer thread calls this function to know when 
// it can successfully write to the shared data.
DWORD SWMRGWaitToWrite (PSWMRG pSWMRG, DWORD dwTimeout);


// A writer thread calls this function to let other threads
// know that it no longer needs to write to the shared data.
void  SWMRGDoneWriting (PSWMRG pSWMRG);


// A reader thread calls this function to know when 
// it can successfully read the shared data.
DWORD SWMRGWaitToRead  (PSWMRG pSWMRG, DWORD dwTimeout);


// A reader thread calls this function to let other threads
// know when it no longer needs to read the shared data.
void  SWMRGDoneReading (PSWMRG pSWMRG);


