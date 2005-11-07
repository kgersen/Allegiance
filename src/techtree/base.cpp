
//#include <windows.h>
#include "pch.h"
#include <base.h>

CBase::CBase()
{
    InitializeCriticalSection(&mBaseCritSect);
}


CBase::~CBase()
{
    DeleteCriticalSection(&mBaseCritSect);
}


VOID CBase::Lock()
{
    EnterCriticalSection(&mBaseCritSect);
}


VOID CBase::Unlock()
{
    LeaveCriticalSection(&mBaseCritSect);
}


VOID CBase::operator=(const CBase &Other)
{
}


