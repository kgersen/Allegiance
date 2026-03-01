// BT - mutex rollback 9/24

#ifndef __ZAutoCriticalSection_h__
#define __ZAutoCriticalSection_h__

#include "zlib_platform.h"
#include <mutex>

/////////////////////////////////////////////////////////////////////////////
// ZAutoCriticalSection.h | Declaration of the ZAutoCriticalSection class.
//


/////////////////////////////////////////////////////////////////////////////
// ZAutoCriticalSection provides methods for obtaining and releasing
// ownership of a Win32 critical section object. ZAutoCriticalSection
// automatically initializes critical section objects in the constructor, and
// automatically deletes the critical section in the destructor.
class ZAutoCriticalSection
{
public:
  ZAutoCriticalSection() {}
  ~ZAutoCriticalSection() {}

  void Lock() { m_mutex.lock(); }
  void Unlock() { m_mutex.unlock(); }

protected:
  std::recursive_mutex m_mutex;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__ZAutoCriticalSection_h__

