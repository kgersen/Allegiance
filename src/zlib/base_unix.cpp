#include "base.h"
#include "zassert.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

////////////////////////////////////////////////////////////////////////////////
//
// Helper Functions
//
////////////////////////////////////////////////////////////////////////////////

class ZFilePrivate
{
public:
    ~ZFilePrivate()
    {
        if (m_fd != -1)
            close(m_fd);
    }
public:
    int m_fd = -1;
    PathString m_pathString;
    uint8_t* m_p = nullptr;
    size_t m_mappedSize = 0;
};

////////////////////////////////////////////////////////////////////////////////
//
// ZFile
//
////////////////////////////////////////////////////////////////////////////////

ZFile::ZFile() : 
    d(new ZFilePrivate)
{
}

ZFile::ZFile(const PathString& strPath, uint32_t how) : 
    d(new ZFilePrivate)
{
    d->m_pathString = strPath;
    int flags = 0;
    
    if ((how & OF_WRITE) == OF_WRITE)
        flags = O_RDWR;
    else
        flags = O_RDONLY;

    if ((how & OF_CREATE) == OF_CREATE)
        flags |= O_CREAT | O_TRUNC;

    d->m_fd = open(strPath, flags, 0664);
}

ZFile::~ZFile()
{
    if (d->m_p) {
        munmap(d->m_p, d->m_mappedSize);
    }
}

bool ZFile::IsValid()
{
     return d->m_fd != -1;
}

uint32_t ZFile::Read(void* p, uint32_t length)
{
    ssize_t result = read(d->m_fd, p, length);
    return (result < 0) ? 0 : (uint32_t)result;
}

uint32_t ZFile::Write(void* p, uint32_t length)
{
    ssize_t result = write(d->m_fd, p, length);
    if (result < 0) return 0;
    ZAssert((uint32_t)result == length);
    return (uint32_t)result;
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
    static uint8_t zeros[] = { 0, 0, 0 };
    int pad = 4 - (length & 3);

    if (pad != 4) {
        return (Write(zeros, pad) != 0);
    }

    return true;
}

bool ZFile::Write(uint32_t value)
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
    struct stat st;
    if (fstat(d->m_fd, &st) == 0)
        return st.st_size;
    return 0;
}

uint8_t* ZFile::GetPointer(bool bWrite, bool bCopyOnWrite)
{
    ZAssert(!bWrite || !bCopyOnWrite);

    if (d->m_p == nullptr) {
        int length = GetLength();
        if (length == 0) return nullptr;
        
        int prot = PROT_READ;
        if (bWrite) prot |= PROT_WRITE;
        if (bCopyOnWrite) prot |= PROT_WRITE; // MAP_PRIVATE handles copy-on-write
        
        int flags = bCopyOnWrite ? MAP_PRIVATE : MAP_SHARED;
        
        d->m_p = (uint8_t*)mmap(NULL, length, prot, flags, d->m_fd, 0);
        
        if (d->m_p == MAP_FAILED) {
            d->m_p = nullptr;
            ZAssert(false);
        } else {
            d->m_mappedSize = length;
        }
    }

    return d->m_p;
}

void ZFile::SetPointer(uint8_t * ptr)
{
    d->m_p = ptr;
}

ZWriteFile::ZWriteFile(const PathString& strPath) :
    ZFile(strPath, OF_CREATE | OF_WRITE)
{
}

long   ZFile::Tell()
{
    return lseek(d->m_fd, 0, SEEK_CUR);
}

int   ZFile::Seek(long offset, int origin)
{
    int unix_origin;
    switch(origin) {
        case 0: unix_origin = SEEK_SET; break; // FILE_BEGIN
        case 1: unix_origin = SEEK_CUR; break; // FILE_CURRENT
        case 2: unix_origin = SEEK_END; break; // FILE_END
        default: unix_origin = SEEK_SET; break;
    }
    return lseek(d->m_fd, offset, unix_origin) != -1;
}

ZString ZFile::GetSha1Hash()
{
    // TODO: Implement SHA1 for Linux or use a library
    return "0";
}

FILETIME ZFile::GetMostRecentFileModificationTime(ZString &searchPath)
{
    // TODO: Implement for Linux
    FILETIME ft;
    ft.dwHighDateTime = 0;
    ft.dwLowDateTime = 0;
    return ft;
}
