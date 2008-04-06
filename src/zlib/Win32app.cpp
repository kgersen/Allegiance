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

// mmf added code for chat logging
// mmf 7/15 changed creation flag on chat file so other processes can read from it
// avalanche + mmf 03/22/07 (bugs 108 and 109) place chat logs in logs folder, use \r\n

HANDLE chat_logfile = NULL;
char logFileName[MAX_PATH + 21];

void InitializeLogchat()
{
	HKEY hKey;
	DWORD dwType;
	char szValue[20];
	DWORD cbValue = sizeof(szValue);
	bool bLogChat = false;

	if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
	{
		::RegQueryValueEx(hKey, "LogChat", NULL, &dwType, (unsigned char*)&szValue, &cbValue);		
		::RegCloseKey(hKey);
		bLogChat = (strcmp(szValue, "1") == 0);
	}

	if (bLogChat)
	{
	time_t longTime;
	time(&longTime);
	tm* t = new tm;
//	tm* t = localtime(&longTime);
	localtime_s(t, &longTime);

	// char logFileName[MAX_PATH + 21]; make this global so chat can open and close it
	// turns out this is not needed but leaving it here instead of moving it again
	GetModuleFileName(NULL, logFileName, MAX_PATH);
	char* p = strrchr(logFileName, '\\');
	if (!p)
		p = logFileName;
	else
		p++;

	strcpy(p, "logs\\");

	if (!CreateDirectory(logFileName, NULL))
	{
		if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			debugf("Unable to create chat log directory %s\n",logFileName);
		}
	}

	// mmf 1/17/08 fixed month
	sprintf(p+5, "chat_%02d-%02d-%02d-%02d%02d%02d.txt", (t->tm_year - 100), (t->tm_mon +1), t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	// mmf changed 3 param from 0 to FILE_SHARE_READ
	chat_logfile =
		CreateFile(
			logFileName,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH,
			NULL
		);
	delete t;
	}
	if (chat_logfile == NULL) debugf("Unable to create chat_logfile %s\n",logFileName);
}

void TerminateLogchat()
{
	if (chat_logfile) {
		CloseHandle(chat_logfile);
        chat_logfile = NULL;
    }
}

void logchat(const char* strText)
{
	// unravel this from debugf-ZDebugOutputImpl-DebugOutput
    const size_t size = 512;
    char         bfr[size];
	int length;

	time_t  longTime;
    time(&longTime);
	tm* t = new tm;
//    tm*             t = localtime(&longTime);
	localtime_s(t, &longTime);

	length = strlen(strText);

	// don't log if text is bigger than buffer, we don't want to log these long 'spam' chat's anyway
	if (chat_logfile && (length < 490)) {
		sprintf(bfr, "%02d/%02d/%02d %02d:%02d:%02d: %s\r\n",
            (t->tm_mon + 1), t->tm_mday, (t->tm_year - 100), t->tm_hour, t->tm_min, t->tm_sec, strText);
        DWORD nBytes;
        ::WriteFile(chat_logfile, bfr, strlen(bfr), &nBytes, NULL);
	}
	delete t;
}

// end mmf chat logging code


void ZDebugOutputImpl(const char *psz)
{
    if (g_papp)
        g_papp->DebugOutput(psz);
    else
        ::OutputDebugStringA(psz);
}
HANDLE g_logfile = NULL;

extern int g_outputdebugstring = 0;  // mmf temp change, control outputdebugstring call with reg key

void retailf(const char* format, ...)
{
    if (g_bOutput)
    {
#ifndef DREAMCAST        
        const size_t size = 2048; //Avalance: Changed to log longer messages. (From 512)
        char         bfr[size];

        va_list vl;
        va_start(vl, format);
        _vsnprintf_s(bfr, size, (size-1), format, vl); //Avalanche: Fix off by one error. 
        va_end(vl);

        ZDebugOutputImpl(bfr);
#else
        ZDebugOutputImpl(format);
#endif
    }
}

extern bool g_bOutput = true;

// mmf log to file on SRVLOG define as well as _DEBUG
#ifdef _DEBUG
#define SRVLOG
#endif

#ifdef SRVLOG // mmf changed this from _DEBUG

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
            const size_t size = 2048; //Avalanche: Changed to handle longer messages (from 512)
            char         bfr[size];

            va_list vl;
            va_start(vl, format);
            _vsnprintf_s(bfr, size, (size-1), format, vl); //Avalanche: Fix off by one error. 
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

		// mmf added this regkey check 
        if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
        {
            ::RegQueryValueEx(hKey, "OutputDebugString", NULL, &dwType, (unsigned char*)&szValue, &cbValue);
            ::RegCloseKey(hKey);

            g_outputdebugstring = (strcmp(szValue, "1") == 0);
        }

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
			tm* t = new tm;
//            tm*             t = localtime(&longTime);
			localtime_s(t, &longTime);

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
//            strcpy_s(p, _MAX_PATH + 16, months[t->tm_mon]);
//            sprintf_s(p + 3, _MAX_PATH + 13, "%02d%02d%02d%02d.txt",
//                    t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			strcpy(p, months[t->tm_mon]);
			sprintf(p+3, "%02d%02d%02d%02d.txt",
				t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
			delete t;
			// mmf this is NOT where the logfile AllSrv.txt is generated
			//     this is the client logfile and only for FZDebug build
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
#endif  // SRVLOG or _DEBUG

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
		// mmf for now tie this to a registry key
		if (g_outputdebugstring)
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
    srand(GetTickCount() + (int)time(NULL));

	// mmf why is this done?
    // shift the stack locals and the heap by a random amount.            
    char* pzSpacer = new char[4 * (int)random(21, 256)];
    pzSpacer[0] = *(char*)_alloca(4 * (int)random(1, 256));

    __try { 
        do {
            #ifdef _DEBUG
                InitializeDebugf();
            #endif

			InitializeLogchat();  // mmf

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

			TerminateLogchat(); // mmf

        } while (false);
    }  __except (g_papp->OnException(_exception_code(), (ExceptionData*)_exception_info())){
    }  
    delete pzSpacer;

    return 0;
}
