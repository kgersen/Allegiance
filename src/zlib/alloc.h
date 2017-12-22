//////////////////////////////////////////////////////////////////////////////
//
// Debug allocation
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _alloc_H_
#define _alloc_H_

#define _DUMP_LEAKS_

// VS.Net 2003 port: problem with 'new' been redefined. disable this still workaround found 
#if _MSC_VER >= 1310
#define ZLIB_SKIP_NEW_REDEF
#endif
#if defined(_DEBUG) && !defined(ZLIB_SKIP_NEW_REDEF)
    #if defined(_DUMP_LEAKS_)

        #include "crtdbg.h"

        void* ZAlloc(unsigned int s, char* pszFile, int line);
        void ZFree(void* pv);

        inline void* __cdecl operator new(
           unsigned int s,
           char* pszFile,
           int line
        ) {
            return ZAlloc(s, pszFile, line);
        }

        inline void __cdecl operator delete(void* pv, bool b) { ZFree(pv); }

        #define _DebugNewDefined_
        #define new new(__FILE__, __LINE__)

        inline void InitAllocs()
        {
            int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
            _CrtSetDbgFlag(tmpFlag | _CRTDBG_LEAK_CHECK_DF/* | _CRTDBG_CHECK_ALWAYS_DF*/);
        }
    #else
        inline void InitAllocs() {}
    #endif
#else
    inline void InitAllocs() {}
#endif

#define ArrayCount(parray) (sizeof(parray) / sizeof(*parray))

//////////////////////////////////////////////////////////////////////////////
//
// Subclasses of TZeroFill are filled with zeros on construction
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
class TZeroFill : public Type {
protected:
    TZeroFill()
    {
        Reinitialize();
    }

public:
    void Reinitialize()
    {
        memset(this, 0, sizeof(Type));
    }
};

#endif
