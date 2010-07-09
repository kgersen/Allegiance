#ifndef _minidump_h_
#define _minidump_h_

//////////////////////////////////////////////////////////////////////////////
//
// minidump / 7z wrapper - Imago 7/10
//
//////////////////////////////////////////////////////////////////////////////

class FileTimeEquals {
public:
    bool operator () (const WIN32_FIND_DATA pdata1, const WIN32_FIND_DATA pdata2)
    {
         return ((CompareFileTime(&pdata1.ftLastWriteTime, &pdata2.ftLastWriteTime)) == 0) ? 1 : 0;
    }
};

class FileTimeCompare {
public:
    bool operator () (const WIN32_FIND_DATA pdata1, const WIN32_FIND_DATA pdata2)
    {
        return  ((CompareFileTime(&pdata1.ftLastWriteTime, &pdata2.ftLastWriteTime)) == 1) ? 0 : 1;
    }
};

typedef TList<WIN32_FIND_DATA, FileTimeEquals, FileTimeCompare> FileList;

FileList FindDumps();

int Create7z(const char * szFile, const char * sz7z);

void DeleteDumps(bool bAll = true);

ZString GetAppDir();

#endif