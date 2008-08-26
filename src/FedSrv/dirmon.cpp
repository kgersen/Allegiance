
#include "pch.h"

char g_chFileChangeBuff[4<<10];

DirectoryMonitor::DirectoryMonitor()
    : m_nPollInterval(0), m_cFiles(0), m_pargFiles(NULL), m_hCompPort(NULL), m_hDir(NULL), m_pFileNamesBuffer(NULL),
      m_pNewestFileCached(NULL)
{
    m_szDir[0] = '\0';
    m_szDirWithSpec[0] = '\0';

    m_overlapped.Internal = 0;
    m_overlapped.InternalHigh = 0;
    m_overlapped.Offset = 0;
    m_overlapped.OffsetHigh = 0;
    m_overlapped.hEvent = NULL;
}

DirectoryMonitor::~DirectoryMonitor()
{
    if (m_hDir)
        CloseHandle(m_hDir);

    if (m_pFileNamesBuffer)
        delete[] m_pFileNamesBuffer;

    if (m_pargFiles)
        delete [] m_pargFiles;
}



/*-------------------------------------------------------------------------
 * FilterFile
 *-------------------------------------------------------------------------
 * Purpose:
 *    Consider filtering out the file.  Some files should never be sent.
 * 
 * Returns:
 *    true if file should be filtered.
 *
 * Remarks:
 *    The client also filters out some files.  The client needs it's own
 *    filter because the server doesn't know everything about the client, like
 *    what build flavor it is running.
 */
bool DirectoryMonitor::FilterFile(WIN32_FIND_DATA & finddata)
{
    //
    // skip directory listings and hidden files
    //
    if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        return true;
        
    if (finddata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
        return true;

    char * szFileName = finddata.cFileName;

    unsigned cLen;

    // we check the first X chars because compressed files have an _ at the
    // end of their filenames
    cLen = 11;
    assert(strlen("Artwork.ini") == cLen);

    if (_strnicmp(szFileName, "Artwork.ini", cLen) == 0)
      return true;

    return false; // false == do not filter
}



/*-------------------------------------------------------------------------
 * SetupMonitor
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get names and time stamps of all files, and setup change notification
 * 
 * Side Effects:
 *    Notification setup, where we can poll for any changes since last poll,
 *      which is done on WM_TIMER
 */
BOOL DirectoryMonitor::SetupMonitor(LPCSTR szDir, int nPollInterval)
{
  m_nPollInterval = nPollInterval;
  // take snapshot of files
  WIN32_FIND_DATA finddata;
  HANDLE hsearchFiles = 0;

  // clean up from any previous times
  m_cFiles = 0;

  int cbValue = strlen(szDir);
  lstrcpy(m_szDir, szDir);
  if (cbValue < 1 || '\\' != szDir[cbValue - 1]) // needs to end in "\"
  {  
    m_szDir[cbValue++] = '\\';
    m_szDir[cbValue] = '\0';
  }

  memcpy(m_szDirWithSpec, m_szDir, cbValue+1);
  m_szDirWithSpec[cbValue++] = '*';
  m_szDirWithSpec[cbValue] = '\0';

  // count the files in the file path
  int cFiles = 0;
  int cFileNameBufferBytes = 0;
  hsearchFiles = FindFirstFile(m_szDirWithSpec, &finddata);
  if (INVALID_HANDLE_VALUE == hsearchFiles)
  {
      return false;
  }
  while (INVALID_HANDLE_VALUE != hsearchFiles)
  {
    if (!FilterFile(finddata))
    {
        // include this file
        cFiles++;
        cFileNameBufferBytes += strlen(finddata.cFileName) + 1;
    }

    if (!FindNextFile(hsearchFiles, &finddata))
    {
      FindClose(hsearchFiles);
      hsearchFiles = INVALID_HANDLE_VALUE;
    }
  }
  
  m_cFiles    = cFiles;

  if (m_pargFiles)
    delete [] m_pargFiles;

  m_pargFiles = new FileChangeInfo[cFiles];

  if (m_pFileNamesBuffer)
    delete[] m_pFileNamesBuffer;

  m_pFileNamesBuffer = new char[cFileNameBufferBytes];
  
  char* pNextSlot = m_pFileNamesBuffer;
  char* pEnd = m_pFileNamesBuffer + cFileNameBufferBytes;

  // now do it again, this time storing the stuff
  hsearchFiles = FindFirstFile(m_szDirWithSpec, &finddata); // search handle
  for(int i=0; (i < m_cFiles) && (INVALID_HANDLE_VALUE != hsearchFiles);)
  {
     // skip directory listings and hidden files
    if (!FilterFile(finddata)) 
    {
        int cbFileName = lstrlen(finddata.cFileName) + 1;

        m_pargFiles[i].szFileName = pNextSlot; 
        pNextSlot += cbFileName;
        if(pNextSlot > pEnd)
        {
            _AGCModule.TriggerEvent(NULL, AllsrvEventID_ArtChangedInInit, "", -1, -1, -1, 0);
            return false;
        }
        CopyMemory(m_pargFiles[i].szFileName, finddata.cFileName, cbFileName);
        m_pargFiles[i].ftLastWriteTime = finddata.ftLastWriteTime;
        m_pargFiles[i].cFileSize = finddata.nFileSizeLow; // note: we assume the FileSize High is zero
        i++;
    }
    if (!FindNextFile(hsearchFiles, &finddata))
    {
      FindClose(hsearchFiles);
      hsearchFiles = INVALID_HANDLE_VALUE;
    }
  }
  FindClose(hsearchFiles);

  if (m_hDir)
    CloseHandle(m_hDir);

  // setup change notification
  m_hDir = CreateFile( m_szDir,
                          FILE_LIST_DIRECTORY,
                          FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                          NULL,
                          OPEN_EXISTING,
                          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
                          NULL);
  m_hCompPort = CreateIoCompletionPort(m_hDir, NULL, NULL, 0);

  ZAssert(m_hDir != INVALID_HANDLE_VALUE);
  DWORD dwBytesReturned = 0;

  if (IsWinNT())
  {
    ReadDirectoryChangesW(m_hDir,
                          g_chFileChangeBuff,
                          sizeof(g_chFileChangeBuff),
                          TRUE,
                          FILE_NOTIFY_CHANGE_LAST_WRITE,
                          &dwBytesReturned,
                          &m_overlapped,
                          NULL);
  }
  return true;
}

/*-------------------------------------------------------------------------
 * CheckForFileChanges
 *-------------------------------------------------------------------------
 * Purpose:
 *    See if any files have changed, and if so, update our files list
 */
void DirectoryMonitor::CheckForFileChanges()
{
  // Check for new files
  DWORD dwBytesTransferred;
  LPOVERLAPPED pOverlapped = &m_overlapped;
  DWORD dwCompletionKey;
  static char szPrevFileName[c_cbFileName]; // don't process changes twice--bug in ReadDirectoryChangesW
  if (GetQueuedCompletionStatus(m_hCompPort, &dwBytesTransferred, &dwCompletionKey, &pOverlapped, 0))
  {

    DWORD cbOffset;
    PFILE_NOTIFY_INFORMATION pfni = (PFILE_NOTIFY_INFORMATION) g_chFileChangeBuff;

    do
    {
      // convert the string from Unicode to OEM
      char szFileName[c_cbFileName];
      WideCharToMultiByte(CP_OEMCP, 0, pfni->FileName, -1, szFileName, 
          sizeof(szFileName), NULL, NULL);
      cbOffset = pfni->NextEntryOffset;

      if(lstrcmp(szPrevFileName, szFileName)) // skip if same as last file
      {
        if (pfni->Action == FILE_ACTION_MODIFIED)
        {
          lstrcpy(szPrevFileName, szFileName); // save this file name for next time
        
          // Check for known files--one could argue that we need to handle additions...
          int iFile = 0;
          for (iFile = 0; iFile < m_cFiles; iFile++)
          {
            if (!lstrcmpi(m_pargFiles[iFile].szFileName, szFileName)) // equality check
            {
              char szFullPath[128];
              lstrcpy(szFullPath, m_szDir);
              lstrcat(szFullPath, "\\");
              lstrcat(szFullPath, szFileName);
          
              HANDLE hFile = CreateFile(szFullPath, GENERIC_READ, FILE_SHARE_READ,
                                NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);

              BY_HANDLE_FILE_INFORMATION FileInfo;
              GetFileInformationByHandle(hFile, &FileInfo);
          
              m_pargFiles[iFile].ftLastWriteTime = FileInfo.ftLastWriteTime;
              m_pargFiles[iFile].cFileSize = FileInfo.nFileSizeLow; // note: we are assume the FileSize High is zero

              CloseHandle(hFile);
            }
          }
        }
      }
      else // files matched
        szPrevFileName[0] = 0;
      
      pfni = (PFILE_NOTIFY_INFORMATION)((LPBYTE) pfni + cbOffset);

    } while( cbOffset );

    if (IsWinNT())
    {
      ReadDirectoryChangesW(m_hDir,
                            g_chFileChangeBuff,
                            sizeof(g_chFileChangeBuff),
                            TRUE,
                            FILE_NOTIFY_CHANGE_LAST_WRITE,
                            &cbOffset, // recycling the variable--don't care about it anymore
                            &m_overlapped,
                            NULL);
    }
  }
}

#if 0
    // TODO:  periodically check for changed files every m_nPollingInterval milliseconds
    CheckForChangedFiles();
#endif
    
// nType    one of: edmNewerThan=1, edmEqualTo=2, edmOlderThan=4
// ft       the filetime compared against each file based on nType
// pfci     the function that will be called with each file
// pData    data needed by the callback function to understand the caller's context
bool DirectoryMonitor::IterateFiles(int nType, FILETIME* pft, LPIFFCIPROC FileChangeCallback, void* pData)
{
  bool bAnyMatches = false;

  m_pNewestFileCached = NULL;

  for (int iFile = 0; iFile < m_cFiles; iFile++)
  {
    DWORD dwHighFile = m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime;
    DWORD dwLowFile = m_pargFiles[iFile].ftLastWriteTime.dwLowDateTime;
    BOOL bMatch = false;
    switch(nType)
    {
    case edmNewerThan:
        if (dwHighFile > pft->dwHighDateTime || (dwHighFile == pft->dwHighDateTime && dwLowFile > pft->dwLowDateTime))
            bMatch = true;
        break;
    case edmEqualTo:
        if (dwHighFile == pft->dwHighDateTime && dwLowFile == pft->dwLowDateTime)
            bMatch = true;
        break;
    case edmOlderThan:
        if (dwHighFile < pft->dwHighDateTime || (dwHighFile == pft->dwHighDateTime && dwLowFile < pft->dwLowDateTime))
            bMatch = true;
        break;
    }
    if (bMatch)
    {
        bAnyMatches = true;
        if (!FileChangeCallback(&m_pargFiles[iFile], pData)) // make callback...
            return false; // if any errors, skip art download by returning "no matches"
    }
  }
  return bAnyMatches;
}

FileChangeInfo* DirectoryMonitor::GetNewestFile()
{
  if (m_pNewestFileCached == NULL)
  {
      DWORD dwHighFile = (DWORD)0;
      DWORD dwLowFile = (DWORD)0;
      for (int iFile = 0; iFile < m_cFiles; iFile++)
      {
        if (
            m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime > dwHighFile || 
            (
             m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime == dwHighFile && 
             m_pargFiles[iFile].ftLastWriteTime.dwLowDateTime > dwLowFile
            )
           )
        {
            dwHighFile = m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime;
            dwLowFile = m_pargFiles[iFile].ftLastWriteTime.dwLowDateTime;  
            m_pNewestFileCached = &m_pargFiles[iFile];
        }
      }
  }

  return m_pNewestFileCached;
}

FileChangeInfo* DirectoryMonitor::GetOldestFile() 
{
  assert(0); // note: if this function ever gets used, 
             //       it should use a cached value like GetNewestFile() does with m_pNewestFileCached
  /*
  DWORD dwHighFile = (DWORD)-1;
  DWORD dwLowFile = (DWORD)-1;
  FileChangeInfo* pFile = NULL;
  for (int iFile = 0; iFile < m_cFiles; iFile++)
  {
    if (
        m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime < dwHighFile || 
        (
         m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime == dwHighFile && 
         m_pargFiles[iFile].ftLastWriteTime.dwLowDateTime < dwLowFile 
        )
       )
    {
        dwHighFile = m_pargFiles[iFile].ftLastWriteTime.dwHighDateTime;
        dwLowFile = m_pargFiles[iFile].ftLastWriteTime.dwLowDateTime;  
        pFile = &m_pargFiles[iFile];
    }
  }
  return pFile*/
  return 0; // allow to compile
}
