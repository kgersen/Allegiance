#include "zmath.h"

#include <crtdbg.h>

#include "zassert.h"

//////////////////////////////////////////////////////////////////////////////
//
// Allocation
//
//////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
    #undef new
    #undef delete

    void* ZAlloc(
       unsigned int s,
       char* pszFile,
       int line
    ) {
        void* pv = ::operator new(s, _CLIENT_BLOCK, pszFile, line);
        return pv;
    }

    typedef struct _CrtMemBlockHeader{
        struct _CrtMemBlockHeader * pBlockHeaderNext;                        
        struct _CrtMemBlockHeader * pBlockHeaderPrev;                        
        char *                      szFileName;                              
        int                         nLine;                                   
        size_t                      nDataSize;                               
        int                         nBlockUse;                               
        long                        lRequest;                                
        unsigned char               gap[4];                 
    } _CrtMemBlockHeader;                                               
                                                                             
    void ZFree(void* pv) 
    {
        if (pv != NULL) {
            _CrtMemBlockHeader* pHead = (((_CrtMemBlockHeader *)pv)-1);     
            _free_dbg(pv, pHead->nBlockUse);                                
        }
    }

#endif

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

DWORD CountBits(DWORD dw)
{
    int count = 0;

    while (dw != 0) {
        if (dw & 1) {
            count++;
        }

        dw = dw >> 1;
    }

    return count;
}

DWORD GetShift(DWORD dw)
{
    if (dw == 0) {
        return -1;
    }

    DWORD index = 0;

    while ((dw & 1) == 0) {
        index++;
        dw = dw >> 1;
    }

    return index;
}

DWORD NextPowerOf2(DWORD x)
{
    DWORD p = 1;

    while (p < x) {
        p *= 2;
        ZAssert(p != 0);
    }

    return p;
}

//////////////////////////////////////////////////////////////////////////////
//
// Constants
//
//////////////////////////////////////////////////////////////////////////////

float g_0      =   0.0f;
float g_1      =   1.0f;
float g_0_5    =   0.5f;
float g_255    = 255.0f;
float g_Inv255 = 1.0f / 255.0f;
