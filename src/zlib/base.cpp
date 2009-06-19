#include "pch.h"

////////////////////////////////////////////////////////////////////////////////
//
// Helper Functions
//
////////////////////////////////////////////////////////////////////////////////

bool IsWindows9x()
{
    static bool bChecked = false;
    static bool bIs9x;

    if (!bChecked)
    {
        OSVERSIONINFO osversioninfo;

        osversioninfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        ZVerify(GetVersionEx(&osversioninfo));

        bIs9x = osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS;
        bChecked = true;
    }

    return bIs9x;
}


////////////////////////////////////////////////////////////////////////////////
//
// ZFile
//
////////////////////////////////////////////////////////////////////////////////

// BUILD_DX9: added for DX9 but can stay for DX7 as well
ZFile::ZFile( )
{
	m_p = NULL;
	m_handle = INVALID_HANDLE_VALUE;
}


ZFile::ZFile(const PathString& strPath, DWORD how) : 
    m_p(NULL)
{
    OFSTRUCT rob;
    m_handle = (HANDLE)OpenFile(strPath, &rob, how);
}

ZFile::~ZFile()
{
    if (m_p) {
        UnmapViewOfFile(m_p);
        CloseHandle(m_hfileMapping);
    }
}

bool ZFile::IsValid()
{
     return m_handle != (HANDLE)HFILE_ERROR;
}

DWORD ZFile::Read(void* p, DWORD length)
{
    DWORD cbActual;

    ReadFile(m_handle, p, length, (LPDWORD)&cbActual, NULL);
    return cbActual;
}

DWORD ZFile::Write(void* p, DWORD length)
{
    DWORD cbActual;

    WriteFile(m_handle, p, length, (LPDWORD)&cbActual, NULL);

    ZAssert(cbActual == length);

    return cbActual;
}

bool ZFile::Write(const ZString& str)
{
    return (Write((void*)(PCC)str, str.GetLength()) != 0);
}

bool  ZFile::WriteString(const ZString& str)
{
    return (Write((void*)(PCC)str, str.GetLength() + 1) != 0);
}

bool  ZFile::WriteAlignedString(const ZString& str)
{
    int length = str.GetLength() + 1;

    if (Write((void*)(PCC)str, str.GetLength() + 1) != 0) {
        return WritePad(length);
    }

    return false;
}

bool ZFile::WritePad(int length)
{
    static BYTE zeros[] = { 0, 0, 0 };
    int pad = 4 - (length & 3);

    if (pad != 4) {
        return (Write(zeros, pad) != 0);
    }

    return true;
}

bool ZFile::Write(DWORD value)
{
    return (Write(&value, 4) != 0);
}

bool ZFile::Write(int value)
{
    return (Write(&value, 4) != 0);
}

bool ZFile::Write(float value)
{
    return (Write(&value, 4) != 0);
}

int ZFile::GetLength()
{
    return GetFileSize(m_handle, NULL);
}

BYTE* ZFile::GetPointer(bool bWrite, bool bCopyOnWrite)
{
    ZAssert(!bWrite || !bCopyOnWrite);

    if (m_p == NULL) {
        m_hfileMapping = 
            CreateFileMapping(
                m_handle,
                0,
                bWrite
                    ? PAGE_READWRITE
                    : PAGE_READONLY,
                0,
                0,
                NULL
            );

        ZAssert(m_hfileMapping != NULL);

        m_p = 
            (BYTE*)MapViewOfFile(
                m_hfileMapping, 
                bWrite
                    ? FILE_MAP_WRITE
                    : (bCopyOnWrite
                        ? FILE_MAP_COPY
                        : FILE_MAP_READ), 
                0, 
                0, 
                0
            );

        ZAssert(m_p != NULL);
    }

    return m_p;
}

ZWriteFile::ZWriteFile(const PathString& strPath) :
    ZFile(strPath, OF_CREATE | OF_WRITE | OF_SHARE_EXCLUSIVE)
{
}

// KGJV 32B - added Tell and Seek
long   ZFile::Tell()
{
    DWORD dwPtr = SetFilePointer(m_handle,0,NULL,FILE_CURRENT);
    if (dwPtr != INVALID_SET_FILE_POINTER)
        return (long) dwPtr;
    else
        return -1;
}
int   ZFile::Seek(long offset, int origin)
{
    DWORD dwPtr = SetFilePointer(m_handle,offset,NULL,origin);
    return (dwPtr != INVALID_SET_FILE_POINTER);
}
