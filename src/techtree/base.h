
#ifndef _BASE_H_
#define _BASE_H_

class CBase
{
    private:
        CRITICAL_SECTION mBaseCritSect;

    public:
        CBase();
        virtual ~CBase();

        VOID Lock();
        VOID Unlock();

		virtual VOID operator=(const CBase &Other);
} ;

typedef CBase * PBase;

#endif

