#include "pch.h"
#include "regkey.h"

//////////////////////////////////////////////////////////////////////////////
//
// Some assertion functions
//
//////////////////////////////////////////////////////////////////////////////

Win32App *g_papp;

//////////////////////////////////////////////////////////////////////////////
//
// Some assertion functions
//
//////////////////////////////////////////////////////////////////////////////
void ZAssertImpl(bool bSucceeded, const char* psz, const char* pszFile, int line, const char* pszModule)
{
    if (!bSucceeded) {
        //
        // Just in case this was a Win32 error get the last error
        //

        DWORD dwError = GetLastError();

        if (!g_papp) {
            __asm int 3; // (debug break)
        } else if (g_papp->OnAssert(psz, pszFile, line, pszModule)) {
            g_papp->OnAssertBreak();
        }
    }
}

void ZDebugOutputImpl(const char *psz)
{
    if (g_papp)
        g_papp->DebugOutput(psz);
    else
        ::OutputDebugStringA(psz);
}
HANDLE g_logfile = NULL;

void retailf(const char* format, ...)
{
    if (g_bOutput)
    {
#ifndef DREAMCAST        
        const size_t size = 256;
        char         bfr[size];

        va_list vl;
        va_start(vl, format);
        _vsnprintf(bfr, size, format, vl);
        va_end(vl);

        ZDebugOutputImpl(bfr);
#else
        ZDebugOutputImpl(format);
#endif
    }
}

extern bool g_bOutput = true;

#ifdef _DEBUG

    void ZWarningImpl(bool bSucceeded, const char* psz, const char* pszFile, int line, const char* pszModule)
    {
        if (!bSucceeded) {
            debugf("%s(%d) : ShouldBe failed: '%s'\n", pszFile, line, psz);
        }
    }

    bool ZFailedImpl(HRESULT hr, const char* pszFile, int line, const char* pszModule)
    {
        bool bFailed = FAILED(hr);
        ZAssertImpl(!bFailed, "Function Failed", pszFile, line, pszModule);
        return bFailed;
    }

    bool ZSucceededImpl(HRESULT hr, const char* pszFile, int line, const char* pszModule)
    {
        bool bSucceeded = SUCCEEDED(hr);
        ZAssertImpl(bSucceeded, "Function Failed", pszFile, line, pszModule);
        return bSucceeded;
    }


    #ifdef _TRACE
        bool    g_bEnableTrace = false;
        ZString g_strSpaces;
        int g_indent = 0;
        int g_line   = 0;

        void SetStrSpaces()
        {
            g_strSpaces = 
                  " "
                + ZString((float)g_indent, 2, 0) 
                + ZString(' ', g_indent * 2 + 1);
        }

        void ZTraceImpl(const char* pcc)
        {
            if (g_bEnableTrace) {
                ZDebugOutput(ZString((float)g_line, 4, 0) + g_strSpaces + ZString(pcc) + "\n");
            }
            g_line++;
        }

        void ZEnterImpl(const char* pcc) 
        {
            ZTraceImpl("enter " + ZString(pcc));
            g_indent += 1;
            SetStrSpaces();
        }

        void ZExitImpl(const char* pcc)
        {
            g_indent -= 1;
            SetStrSpaces();
            ZTraceImpl("exit   " + ZString(pcc));
        }

        void ZStartTraceImpl(const char* pcc)
        {
            g_indent = 0;
            g_line   = 0;
            SetStrSpaces();
            ZTraceImpl(pcc);
        }
    #endif

    void debugf(const char* format, ...)
    {
        if (g_bOutput)
        {
#ifndef DREAMCAST        
            const size_t size = 256;
            char         bfr[size];

            va_list vl;
            va_start(vl, format);
            _vsnprintf(bfr, size, format, vl);
            va_end(vl);

            ZDebugOutputImpl(bfr);
#else
            ZDebugOutputImpl(format);
#endif
        }
    }

    void InitializeDebugf()
    {
#ifndef DREAMCAST        
        HKEY hKey;
        DWORD dwType;
        char  szValue[20];
        DWORD cbValue = sizeof(szValue);
        bool  bLogToFile = false;

        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
        {
            ::RegQueryValueEx(hKey, "LogToFile", NULL, &dwType, (unsigned char*)&szValue, &cbValue);
            ::RegCloseKey(hKey);

            bLogToFile = (strcmp(szValue, "1") == 0);
        }

        if (bLogToFile)
        {
            time_t  longTime;
            time(&longTime);
            tm*             t = localtime(&longTime);

            char    logFileName[MAX_PATH + 16];
            GetModuleFileName(NULL, logFileName, MAX_PATH);
            char*   p = strrchr(logFileName, '\\');
            if (!p)
                p = logFileName;
            else
                p++;

            const char* months[] = {"jan", "feb", "mar", "apr",
                                    "may", "jun", "jul", "aug",
                                    "sep", "oct", "nov", "dec"};
            strcpy(p, months[t->tm_mon]);
            sprintf(p + 3, "%02d%02d%02d%02d.txt",
                    t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

            g_logfile = 
                CreateFile(
                    logFileName, 
                    GENERIC_WRITE, 
                    0,
                    NULL, 
                    OPEN_ALWAYS,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
                    NULL
                );
        }
#endif
    }

    void TerminateDebugf()
    {
#ifndef DREAMCAST        
        if (g_logfile) {
            CloseHandle(g_logfile);
            g_logfile = NULL;
        }
#endif
    }
#endif

//////////////////////////////////////////////////////////////////////////////
//
// Win32 Application
//
//////////////////////////////////////////////////////////////////////////////

Win32App::Win32App()
{
    g_papp = this;
}

Win32App::~Win32App() 
{
}

HRESULT Win32App::Initialize(const ZString& strCommandLine)
{
    return S_OK;
}

void Win32App::Terminate()
{
}

void Win32App::Exit(int value)
{
    _CrtSetDbgFlag(0);
    _exit(value);
}

int Win32App::OnException(DWORD code, ExceptionData* pdata)
{
	return EXCEPTION_CONTINUE_SEARCH;
}


#ifdef MemoryOutput
    TList<ZString> g_listOutput;
#endif

void Win32App::DebugOutput(const char *psz)
{
    #ifdef MemoryOutput
        g_listOutput.PushFront(ZString(psz));

        if (g_listOutput.GetCount() > 100) {
            g_listOutput.PopEnd();
        }
    #else
        ::OutputDebugStringA(psz);

        if (g_logfile) {
            DWORD nBytes;
            ::WriteFile(g_logfile, psz, strlen(psz), &nBytes, NULL);
        }
    #endif
}

bool Win32App::OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule)
{
    ZDebugOutput(
          ZString("assertion failed: '")
        + psz
        + "' ("
        + pszFile
        + ":"
        + ZString(line)
        + ")\n"
    );

    //return _CrtDbgReport(_CRT_ASSERT, pszFile, line, pszModule, psz) == 1;
    return true;
}

void Win32App::OnAssertBreak()
{
    #ifdef MemoryOutput
        ZString str;

        TList<ZString>::Iterator iter(g_listOutput);

        while (!iter.End()) {
            str = iter.Value() + str;
            iter.Next();
        }
    #endif
    
    //
    // Cause an exception
    //

    (*(int*)0) = 0;
}

//////////////////////////////////////////////////////////////////////////////
//
// Win Main
//
//////////////////////////////////////////////////////////////////////////////

__declspec(dllexport) int WINAPI Win32Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    HRESULT hr;

    // seed the random number generator with the current time
    // (GetTickCount may be semi-predictable on server startup, so we add the 
    // clock time to shake things up a bit)
    srand(GetTickCount() + time(NULL));

    // shift the stack locals and the heap by a random amount.            
    char* pzSpacer = new char[4 * (int)random(21, 256)];
    pzSpacer[0] = *(char*)_alloca(4 * (int)random(1, 256));

    __try {
        do {
            #ifdef _DEBUG
                InitializeDebugf();
            #endif

            BreakOnError(hr = Window::StaticInitialize());
            BreakOnError(hr = g_papp->Initialize(lpszCmdLine));

            //
            // Win32App::Initialize() return S_FALSE if this is a command line app and
            // we shouldn't run the message loop
            //

            if (SUCCEEDED(hr) && S_FALSE != hr) {
                Window::MessageLoop();
            }

            g_papp->Terminate();
            Window::StaticTerminate();

            #ifdef _DEBUG
                TerminateDebugf();
            #endif

        } while (false);
    } __except (g_papp->OnException(_exception_code(), (ExceptionData*)_exception_info())){
    }

    delete pzSpacer;

    return 0;
}
