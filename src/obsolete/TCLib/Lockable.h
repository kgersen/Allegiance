/////////////////////////////////////////////////////////////////////////////
// Lockable.h: Declaration of the TCLockable class.
//

#ifndef __Lockable_h__
#define __Lockable_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#include "AutoCriticalSection.h"
#include "ObjectLock.h"


/////////////////////////////////////////////////////////////////////////////

class TCLockable
{
// Construction
public:
  TCLockable() {}

// Operations
public:
  void Lock();
  void Unlock();

// Types
protected:
  typedef TCObjectLock<TCLockable> XLock;

// Data Members
protected:
  TCAutoCriticalSection m_cs;
};


/////////////////////////////////////////////////////////////////////////////
// Operations

inline void TCLockable::Lock()
{
  m_cs.Lock();
}

inline void TCLockable::Unlock()
{
  m_cs.Unlock();
}



/////////////////////////////////////////////////////////////////////////////

#endif // !__Lockable_h__
