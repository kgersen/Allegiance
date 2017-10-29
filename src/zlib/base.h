//////////////////////////////////////////////////////////////////////////////
//
// Base
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _base_h_
#define _base_h_

#include <Windows.h>

#include "tref.h"
#include "zstring.h"

////////////////////////////////////////////////////////////////////////////////
//
// Helper Functions
//
////////////////////////////////////////////////////////////////////////////////

bool IsWindows9x();


////////////////////////////////////////////////////////////////////////////////
//
// Waitable Object
//
////////////////////////////////////////////////////////////////////////////////

class WaitableObject : public IObject {
protected:
    HANDLE m_handle;

public:
    WaitableObject(HANDLE handle = NULL) :
        m_handle(handle)
    {}

    virtual ~WaitableObject()
    {
        CloseHandle(m_handle);
    }

    DWORD Wait(DWORD dwTimeout = -1)
    {
        return WaitForSingleObject(m_handle, dwTimeout);
    }
};

////////////////////////////////////////////////////////////////////////////////
//
// File
//
////////////////////////////////////////////////////////////////////////////////

class ZFile : public WaitableObject {
private:
	PathString m_pathString; // BT - STEAM

protected:

    BYTE* m_p;
    HANDLE m_hfileMapping;

public:
	ZFile(); // BUILD_DX9: added for DX9 but can stay for DX7 as well

    ZFile(const PathString& strPath, DWORD how = OF_READ | OF_SHARE_DENY_WRITE);
    virtual ~ZFile();

    virtual bool  IsValid();
    virtual int   GetLength();
    virtual BYTE * GetPointer(bool bWrite = false, bool bCopyOnWrite = false);

    virtual DWORD Read(void* p, DWORD length);
    DWORD Write(void* p, DWORD length);

    bool  Write(DWORD value);
    bool  Write(int   value);
    bool  Write(float value);
    bool  Write(const ZString& str);
    bool  WriteString(const ZString& str);
    bool  WriteAlignedString(const ZString& str);
    bool  WritePad(int length);
//KGJV 32B - added Tell and Seek
    long  Tell();
    int   Seek(long offset, int origin);

    template<class Type>
    bool WriteStructure(const Type& value) 
    {
        return Write(&value, sizeof(Type));
    }

	// BT - STEAM
	ZString GetSha1Hash();
	static FILETIME GetMostRecentFileModificationTime(ZString &searchPath);
};

class ZWriteFile : public ZFile {
public:
    ZWriteFile(const PathString& strPath);
};

////////////////////////////////////////////////////////////////////////////////
//
// Event
//
////////////////////////////////////////////////////////////////////////////////

class Event : public WaitableObject {
public:
    Event() : WaitableObject(CreateEvent(NULL, FALSE, FALSE, NULL))
    {}

    void Set()   { SetEvent(m_handle); }
    void Reset() { ResetEvent(m_handle); }
    void Pulse() { PulseEvent(m_handle); }
};

////////////////////////////////////////////////////////////////////////////////
//
// Thread
//
///////////////////////////////////////////////////////////////////////////////

template<class ObjectType>
class Thread : public WaitableObject {
protected:
    typedef DWORD (ObjectType::*PFM)();

    DWORD m_tid;
    ObjectType* m_pobject;
    PFM   m_pfm;

    DWORD Start()
    {
        return (m_pobject->*m_pfm)();
    }

    static DWORD ThreadStarter(Thread* pthis)
    {
        return pthis->Start();
    }

public:
    Thread(ObjectType* pobject, PFM pfm, DWORD dwStackSize = 0) :
        m_pobject(pobject),
        m_pfm(pfm),
        WaitableObject(
            CreateThread(
                NULL,
                dwStackSize,
                (LPTHREAD_START_ROUTINE)ThreadStarter,
                this,
                CREATE_SUSPENDED,
                &m_tid
            )
        )
    {}

    void SetPriority(int priority) { SetThreadPriority(m_handle, priority); }
    void Resume() { ResumeThread(m_handle); }
    void Suspend() { SuspendThread(m_handle); }

    DWORD Run()
    {
        DWORD dw;
        Resume();
        Wait();
        GetExitCodeThread(m_handle, &dw);
        return dw;
    }
};

////////////////////////////////////////////////////////////////////////////////
//
// Critical Section
//
///////////////////////////////////////////////////////////////////////////////

class CriticalSection {
protected:
    CRITICAL_SECTION m_crit;

public:
    CriticalSection()
    {
        InitializeCriticalSection(&m_crit);
    }
    ~CriticalSection()
    {
        DeleteCriticalSection(&m_crit);
    }

    void Enter() { EnterCriticalSection(&m_crit); }
    void Leave() { LeaveCriticalSection(&m_crit); }
};

////////////////////////////////////////////////////////////////////////////////
//
// Single Enter Critical Section
//
///////////////////////////////////////////////////////////////////////////////

class SingleEnterCriticalSection {
protected:
    CriticalSection& m_crit;
    bool m_bInCS;

public:
    SingleEnterCriticalSection(CriticalSection& crit) :
        m_crit(crit),
        m_bInCS(false)
    {
    }

    void Enter()
    {
        if (!m_bInCS) {
            m_crit.Enter();
            m_bInCS = true;
        }
    }

    void Leave()
    {
        if (m_bInCS) {
            m_bInCS = false;
            m_crit.Leave();
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
//
// Critical Section Lock
//
///////////////////////////////////////////////////////////////////////////////

class CriticalSectionLock {
protected:
    CriticalSection& m_crit;

public:
    CriticalSectionLock(CriticalSection& crit) :
        m_crit(crit)
    {
        m_crit.Enter();
    }

    ~CriticalSectionLock()
    {
        m_crit.Leave();
    }
};

#endif
