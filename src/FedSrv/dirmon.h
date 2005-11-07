/*-------------------------------------------------------------------------
 * DirMon.h
 * 
 * The definitions, etc. for a class that helps monitor a directory for 
 * file changes.
 * 
 * Author: MarkSn
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifdef _DIRMON_H
#error dirmon.h included twice
#endif
#define _DIRMON_H

// ** Stuff for monitoring the directory
struct FileChangeInfo
{
  char *   szFileName;
  FILETIME ftLastWriteTime;
  unsigned long cFileSize;
};

typedef bool (*LPIFFCIPROC) (FileChangeInfo*, void*);

class DirectoryMonitor
{
private:
    HANDLE          m_hDir;
    HANDLE          m_hCompPort;
    OVERLAPPED      m_overlapped;
    FILETIME        m_ftLastWriteTime;
    char *          m_pFileNamesBuffer; // allocation of memory for filenames in FileChangeInfo
    FileChangeInfo* m_pNewestFileCached;

private:
    bool            FilterFile(WIN32_FIND_DATA & finddata);

protected:
    char            m_szDir[MAX_PATH];          // directory where files to be watched are located
    char            m_szDirWithSpec[MAX_PATH];  // same as m_szDir, but with file spec on the end
    int             m_cFiles;                   // number of file info structs in m_pargFiles
    FileChangeInfo* m_pargFiles;                // array of m_cFiles FileChangeInfo structs
    int             m_nPollInterval;            // number of ms to wait between checks for changes

public:
    enum { edmNewerThan=1, edmEqualTo=2, edmOlderThan=4 };

    DirectoryMonitor();
    virtual ~DirectoryMonitor();

    BOOL SetupMonitor(LPCSTR szDir, int nPollingInterval);
    void CheckForFileChanges();
    bool IterateFiles(int nType, FILETIME* pft, LPIFFCIPROC FileChangeCallback, void* pData);
    FileChangeInfo* GetNewestFile();
    FileChangeInfo* GetOldestFile();
};

