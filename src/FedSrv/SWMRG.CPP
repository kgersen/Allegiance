/*-------------------------------------------------------------------------
 * src\fedsrv\SWMRG.CPP
 * 
 * Single Writer, Multiple Reader w/ Guard
 *
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/


/*
#include <windows.h>
#include <string.h>
#include <tchar.h>
#include "SWMRG.H"		       // The header file
*/

#include "pch.h"

/*-------------------------------------------------------------------------
 * ConstructObjName
 *-------------------------------------------------------------------------
 * Purpose:
 *    Just construct a name for the kernel object
 * 
 * Returns:
 *    the munged name
 */
static LPCTSTR ConstructObjName (LPCTSTR lpszPrefix, LPCTSTR lpszSuffix, 
	                               LPTSTR lpszFullName, size_t cbFullName, bool * pfOk) 
{
	*pfOk = true;	// Assume success.

	if (lpszSuffix == NULL)
		return(NULL);

	if ((_tcslen(lpszPrefix) + _tcslen(lpszSuffix)) >= cbFullName) 
	{
		// If the strings will overflow the buffer,
		// indicate an error.
		*pfOk = false;
		return NULL;
	}

	_tcscpy(lpszFullName, lpszPrefix);
	_tcscat(lpszFullName, lpszSuffix);
	return(lpszFullName);
}


/*-------------------------------------------------------------------------
 * FSWMRGInitialize
 *-------------------------------------------------------------------------
 * Purpose:
 *    Initializes a SWMRG structure. This structure must be 
 *    initialized before any writer or reader threads attempt
 *    to wait on it.
 *    The structure must be allocated by the application and 
 *    the structure's address is passed as the first parameter.
 *    The lpszName parameter is the name of the object. Pass
 *    NULL if you do not want to share the object.
 * 
 * Returns:
 *    success
 */
bool FSWMRGInitialize(PSWMRG pSWMRG, LPCTSTR lpszName) 
{
	TCHAR szFullObjName[100];
	LPCTSTR lpszObjName;
	bool fOk;

	// Initialize all data members to NULL so that we can
	// accurately check whether an error has occured.
	pSWMRG->hMutexNoWriter  = NULL;
	pSWMRG->hEventNoReaders = NULL;
	pSWMRG->hSemNumReaders  = NULL;

	// This mutex guards access to the other objects
	// managed by this data structure and also indicates 
	// whether there any writer threads are writing.
	// Initially no thread owns the mutex.
	lpszObjName = ConstructObjName(__TEXT("SWMRGMutexNoWriter"), lpszName, 
	                               szFullObjName, ARRAY_SIZE(szFullObjName), &fOk);
	if (fOk)		
		pSWMRG->hMutexNoWriter = CreateMutex(NULL, FALSE, lpszObjName);

	// Create the manual-reset event that is signalled when 
	// no reader threads are reading.
	// Initially no reader threads are reading.
	lpszObjName = ConstructObjName(__TEXT("SWMRGEventNoReaders"), lpszName,
		                             szFullObjName, ARRAY_SIZE(szFullObjName), &fOk);
	if (fOk)
		pSWMRG->hEventNoReaders = CreateEvent(NULL, TRUE, TRUE, lpszObjName);

	// Initialize the variable that indicates the number of 
	// reader threads that are reading.
	// Initially no reader threads are reading.
	lpszObjName = ConstructObjName(__TEXT("SWMRGSemNumReaders"), lpszName,
		                             szFullObjName, ARRAY_SIZE(szFullObjName), &fOk);
	if (fOk)
		pSWMRG->hSemNumReaders = CreateSemaphore(NULL, 0, 0x7FFFFFFF, lpszObjName);


	if (NULL == pSWMRG->hMutexNoWriter || 
	    NULL == pSWMRG->hEventNoReaders || 
	    NULL == pSWMRG->hSemNumReaders) 
	{
		// If a synchronization object could not be created,
		// destroy any created objects and return failure.
		SWMRGDelete(pSWMRG);
		fOk = false;
	} 
	else 
	{
		fOk = true;
	}

	// Return TRUE upon success, FALSE upon failure.
	return(fOk);
}


/*-------------------------------------------------------------------------
 * SWMRGDelete
 *-------------------------------------------------------------------------
 * Purpose:
 *    Deletes the system resources associated with a SWMRG 
 *    structure. The structure must be deleted only when
 *    no writer or reader threads in the calling process
 *    will wait on it.
 */
void SWMRGDelete(PSWMRG pSWMRG) 
{

	// Destroy any synchronization objects that were 
	// successfully created.
	if (pSWMRG->hMutexNoWriter)
		CloseHandle(pSWMRG->hMutexNoWriter);

	if (pSWMRG->hEventNoReaders)
		CloseHandle(pSWMRG->hEventNoReaders);

	if (pSWMRG->hSemNumReaders)
		CloseHandle(pSWMRG->hSemNumReaders);
}


/*-------------------------------------------------------------------------
 * SWMRGWaitToWrite
 *-------------------------------------------------------------------------
 * Purpose:
 *    A writer thread calls this function to know when 
 *    it can successfully write to the shared data.
 * 
 * Returns:
 *    WaitForMultipleObjects()
 */
DWORD SWMRGWaitToWrite(PSWMRG pSWMRG, DWORD dwTimeout) 
{
	DWORD dw; 
	HANDLE rgHandles[2];

   // We can write if the following are true:
   // 1. The mutex guard is available and
   //    no other threads are writing.
   // 2. No threads are reading.
	rgHandles[0] = pSWMRG->hMutexNoWriter;
	rgHandles[1] = pSWMRG->hEventNoReaders;
   dw = WaitForMultipleObjects(2, rgHandles, TRUE, dwTimeout);

	if (dw != WAIT_TIMEOUT) 
	{
		// This thread can write to the shared data.

		// Because a writer thread is writing, the mutex should not
		// not be released. This stops other writers and readers.
	}

	return(dw);
}


/*-------------------------------------------------------------------------
 * SWMRGDoneWriting
 *-------------------------------------------------------------------------
 * Purpose:
 *    A writer thread calls this function to let other threads
 *    know that it no longer needs to write to the shared data.
 */
void SWMRGDoneWriting(PSWMRG pSWMRG) 
{
   // Presumably, a writer thread calling this function has
   // successfully called WaitToWrite. This means that we
   // do not have to wait on any synchronization objects 
   // here because the writer already owns the mutex.

	// Allow other writer/reader threads to use
	// the SWMRG synchronization object.
	ReleaseMutex(pSWMRG->hMutexNoWriter);
}


/*-------------------------------------------------------------------------
 * SWMRGWaitToRead
 *-------------------------------------------------------------------------
 * Purpose:
 *    A reader thread calls this function to know when 
 *    it can successfully read the shared data.
 * 
 * Returns:
 *    WaitForSingleObject()
 */
DWORD SWMRGWaitToRead(PSWMRG pSWMRG, DWORD dwTimeout) 
{
	DWORD dw; 
	LONG lPreviousCount;

   // We can read if the mutex guard is available
   // and no threads are writing.
   dw = WaitForSingleObject(pSWMRG->hMutexNoWriter, dwTimeout);

	if (dw != WAIT_TIMEOUT) 
	{
		// This thread can read from the shared data.

		// Increment the number of reader threads.
		ReleaseSemaphore(pSWMRG->hSemNumReaders, 1, &lPreviousCount);
		if (lPreviousCount == 0) 
		{
			// If this is the first reader thread, 
			// set our event to reflect this.
			ResetEvent(pSWMRG->hEventNoReaders);
		}

		// Allow other writer/reader threads to use
		// the SWMRG synchronization object.
		ReleaseMutex(pSWMRG->hMutexNoWriter);
	}

	return(dw);
}


/*-------------------------------------------------------------------------
 * SWMRGDoneReading
 *-------------------------------------------------------------------------
 * Purpose:
 *    A reader thread calls this function to let other threads
 *    know when it no longer needs to read the shared data.
 */
void SWMRGDoneReading(PSWMRG pSWMRG) 
{
	bool fLastReader;

	HANDLE rgHandles[2];

   // We can stop reading if the mutex guard is available,
	// but when we stop reading we must also decrement the
	// number of reader threads.
	rgHandles[0] = pSWMRG->hMutexNoWriter;
	rgHandles[1] = pSWMRG->hSemNumReaders;
    WaitForMultipleObjects(2, rgHandles, TRUE, INFINITE);

	fLastReader = WAIT_TIMEOUT == WaitForSingleObject(pSWMRG->hSemNumReaders, 0);

	if (fLastReader) 
	{
		// If this is the last reader thread, 
		// set our event to reflect this.
		SetEvent(pSWMRG->hEventNoReaders);
	} 
	else 
	{
		// If this is NOT the last reader thread, we successfully
		// waited on the semaphore. We must release the semaphore
		// so that the count accurately reflects the number
		// of reader threads.
		ReleaseSemaphore(pSWMRG->hSemNumReaders, 1, NULL);
	}

	// Allow other writer/reader threads to use
	// the SWMRG synchronization object.
	ReleaseMutex(pSWMRG->hMutexNoWriter);
}

