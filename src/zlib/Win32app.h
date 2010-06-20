//////////////////////////////////////////////////////////////////////////////
//
// Win32 Application
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _Win32App_h_
#define _Win32App_h_


//Imago removed, now known as EXCEPTION_POINTERS 6/10
/*
class ExceptionData {
public:
    EXCEPTION_RECORD *ExceptionRecord;
    CONTEXT          *ContextRecord;
};
*/ 

class Win32App : public IObject {
protected:
    __declspec(dllexport) Win32App();

public:
    __declspec(dllexport) virtual ~Win32App();

    static  void    Exit(int value);

    virtual HRESULT Initialize(const ZString& strCommandLine);
    virtual void    Terminate();
    virtual int     OnException(DWORD code, EXCEPTION_POINTERS* pdata);
	LONG __stdcall  ExceptionHandler(EXCEPTION_POINTERS* pep); //imago 6/10
    virtual void    DebugOutput(const char *psz);
    virtual bool    OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule);
    virtual void    OnAssertBreak();

	// KGJV - added for DX9 behavior
	virtual bool    IsBuildDX9();

	//imago 6/10
	bool EnforceFilter( bool bEnforce );
	bool WriteMemory( BYTE* pTarget, const BYTE* pSource, DWORD Size );

};

#endif
