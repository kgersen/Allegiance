//////////////////////////////////////////////////////////////////////////////
//
// Win32 Application
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _Win32App_h_
#define _Win32App_h_

class ExceptionData {
public:
    EXCEPTION_RECORD *ExceptionRecord;
    CONTEXT          *ContextRecord;
};

class Win32App : public IObject {
protected:
    __declspec(dllexport) Win32App();

public:
    __declspec(dllexport) virtual ~Win32App();

    static  void    Exit(int value);

    virtual HRESULT Initialize(const ZString& strCommandLine);
    virtual void    Terminate();
    virtual int     OnException(DWORD code, ExceptionData* pdata);

    virtual void    DebugOutput(const char *psz);
    virtual bool    OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule);
    virtual void    OnAssertBreak();
};

#endif
