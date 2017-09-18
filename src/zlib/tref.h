#ifndef _tref_H_
#define _tref_H_

// BT - 9/17 - Hunting down mystery fedsrv crashes.
#include "StackTracer.h"
//#include <mutex>

//////////////////////////////////////////////////////////////////////////////
//
// Base class for reference counted objects
//
//////////////////////////////////////////////////////////////////////////////

class IObjectSingle {
private:
	DWORD m_count;
	CRITICAL_SECTION m_criticalSection;

protected:
	typedef IObjectSingle QIType;


	void Internal_Release()
	{
		--m_count;
	}

public:
	IObjectSingle() :
		m_count(0)
	{
		InitializeCriticalSectionAndSpinCount(&m_criticalSection, 0x80000400);
	}

	virtual ~IObjectSingle()
	{
	}


	DWORD GetCount() const
	{
		return m_count;
	}

	virtual bool IsValid()
	{
		return true;
	}


#ifdef _DEBUG
	virtual
#endif
		DWORD __stdcall AddRef()
	{
		return ++m_count;
	}

#ifdef _DEBUG
	virtual
#endif
		DWORD __stdcall Release()
	{

		//if (--m_count == 0) {
		//	delete this;
		//	return 0;
		//}

		//return m_count; 


		int returnValue = 0;

		__try 
		{
			// BT - 9/17 - Making this operation thread safe... it may be the source of the fedsrv crashes.
			EnterCriticalSection(&m_criticalSection);

			if (--m_count == 0) {
				delete this;
				return 0;
			}

			LeaveCriticalSection(&m_criticalSection);

			return m_count;
		}
		__except (StackTracer::ExceptionFilter(GetExceptionInformation()))
		{
			StackTracer::OutputStackTraceToDebugF();
		}

		return returnValue;
	}
};

//////////////////////////////////////////////////////////////////////////////
//
// Object Interface
//
//////////////////////////////////////////////////////////////////////////////

class IObject : virtual public IObjectSingle {
public:
};

//////////////////////////////////////////////////////////////////////////////
//
// Reference to a reference counted object
//
//////////////////////////////////////////////////////////////////////////////

template <class Type>
class TRef {
private:
    Type* m_pt;

public:
    //
    // constructors
    //

    TRef() :
        m_pt(NULL)
    {
    }

    TRef(const Type* pt)
    {
        m_pt = (Type*)pt;
        if (m_pt)
            m_pt->AddRef();
    }

    TRef(const TRef& tref) :
        m_pt(tref.m_pt)
    {
        if (m_pt)
            m_pt->AddRef();
    }

	template< class Other > TRef( const TRef<Other>& oref ) : 
		m_pt( static_cast<Other*>(oref) ) 
	{ 
		if (m_pt) 
			m_pt->AddRef(); 
	} 

    
    //
    // assignment
    //

    TRef* Pointer()
    {
        return this;
    }

    Type** operator&()
    {
        if (m_pt) {
            m_pt->Release();
            m_pt = NULL;
        }
        return &m_pt;
    }

    TRef& operator=(const TRef& tref)
    {
        Type* ptOld = m_pt;
        m_pt = tref.m_pt;

        if (m_pt)
            m_pt->AddRef();

        if (ptOld)
            ptOld->Release();

        return *this;
    }
/*
    TRef& operator=(const Type* ptNew)
    {
        Type* ptOld = m_pt;
        m_pt = (Type*)ptNew;

        if (m_pt)
            m_pt->AddRef();

        if (ptOld)
            ptOld->Release();

        return *this;
    }
*/
    //
    // destructor
    //

    ~TRef(void)
    {
        if (m_pt)
            m_pt->Release();
    }

    //
    // members
    //

    operator Type*(void) const { return m_pt; }
    Type& operator*(void) const { return *m_pt; }
    Type* operator->(void) const { return m_pt; }

    friend bool operator==(const TRef& t1, Type* pt)
    {
        return t1.m_pt == pt;
    }

    friend bool operator!=(const TRef& t1, Type* pt)
    {
        return t1.m_pt != pt;
    }


    // define comparison operators for use in associative containers
    inline friend bool operator<(const TRef& t1, Type* pt) { return t1.m_pt < pt; };
    inline friend bool operator>(const TRef& t1, Type* pt) { return t1.m_pt > pt; };
    inline friend bool operator<=(const TRef& t1, Type* pt) { return t1.m_pt <= pt; };
    inline friend bool operator>=(const TRef& t1, Type* pt) { return t1.m_pt >= pt; };
};

#endif
