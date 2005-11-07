#include "pch.h"


////////////////////////////////////////////////////////////////////////////////
//
// ZFile
//
////////////////////////////////////////////////////////////////////////////////
ZFile::ZFile(const PathString& strPath, DWORD how) : 
    m_p(NULL)
{
    WCHAR wszPath[MAX_PATH];
    
    mbstowcs(wszPath, (LPCSTR)strPath, MAX_PATH);
    if (INVALID_HANDLE_VALUE == (m_handle = CreateFileForMapping( wszPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
        {
        ZString str = "Failed to open file: " + strPath + " (code = " + ZString((int)GetLastError()) + ")\r\n";
        debugf(str);
        }
    else
        {
        ZString str = "Opened file: " + strPath + "\r\n";
        debugf(str);
        }
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
     return m_handle != (HANDLE)-1;
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

void* ZFile::GetPointer()
{
    if (m_p != NULL) {
        return m_p;
    }

    m_hfileMapping = CreateFileMapping(m_handle, 0, PAGE_READONLY, 0, 0, NULL);
    if (m_hfileMapping == (HANDLE)-1) {
        ZError("Can't map file");
        return NULL;
    }

    m_p = MapViewOfFile(m_hfileMapping, FILE_MAP_READ, 0, 0, 0);

    return m_p;
}

/*
    void* m_p;
    char* m_pRead;
	DWORD m_dwSize;
	DWORD m_dwRead;

////////////////////////////////////////////////////////////////////////////////
//
// ZFile
//
////////////////////////////////////////////////////////////////////////////////

ZFile::ZFile(const PathString& strPath, DWORD how) : 
    m_p(NULL),
    m_pRead(NULL),
	m_dwSize(0),
	m_dwRead(0)
{
	HRSRC       hrsrc;
    HGLOBAL     hglobal;
    TCHAR       outbuf[256];

    ZString strFile = strPath;
    if (!strPath.GetExtension().IsEmpty())
        strFile = strFile.LeftOf(strPath.GetExtension().GetLength()+1) + strPath.GetExtension();

    WCHAR wszFile[MAX_PATH];
    mbstowcs(wszFile, (LPCSTR)strFile, MAX_PATH);

    // Handles obtained through FindResource and LoadResource do not have
    // to be freed or released.  This is done automatically.
	hrsrc = FindResource (GetModuleHandle(NULL), wszFile, RT_RCDATA);
    if (hrsrc == NULL) 
        {
        wsprintf (outbuf, TEXT("ERROR: Could not find resource \"%s\".\r\n"), wszFile);
        OutputDebugString (outbuf);
        return;
        }

    m_dwSize = SizeofResource(GetModuleHandle(NULL), hrsrc); 

    // Load the resource.
    wsprintf (outbuf, TEXT("Loading the resource \"%s\".\r\n"), wszFile);
    OutputDebugString (outbuf);
    hglobal = LoadResource (GetModuleHandle(NULL), hrsrc);
    if (hglobal == NULL) 
		{
        wsprintf (outbuf, TEXT("ERROR: Could not load the resource \"%s\".\r\n"), wszFile);
        OutputDebugString (outbuf);
    	return;
		}

    // Get pointer to raw data in resource.
    m_p = LockResource (hglobal);
    m_pRead = (char*)m_p;
}

ZFile::~ZFile()
{
}

bool ZFile::IsValid()
{
    return m_p != NULL;
}

DWORD ZFile::Read(void* p, DWORD length)
{
	DWORD cbActual = length < (m_dwSize - m_dwRead) ? length : (m_dwSize - m_dwRead);

    if (p)
		{
		memcpy(p, m_pRead, cbActual);
		m_dwRead += cbActual;
		m_pRead += cbActual;
		}

    return cbActual;
}

DWORD ZFile::Write(void* p, DWORD length)
{
    return 0;
}

bool ZFile::Write(const ZString& str)
{
    return 0;
}

bool  ZFile::WriteString(const ZString& str)
{
    return 0;
}

bool ZFile::Write(DWORD value)
{
    return 0;
}

bool ZFile::Write(int value)
{
    return 0;
}

bool ZFile::Write(float value)
{
    return 0;
}

int ZFile::GetLength()
{
    return (int)m_dwSize;
}

void* ZFile::GetPointer()
{
    return m_p;
}
*/
