#ifndef _zlib_platform_h_
#define _zlib_platform_h_

#ifndef _WIN32
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cstdio>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>

inline uint32_t timeGetTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

inline uint32_t GetTickCount() { return timeGetTime(); }

#include <alloca.h>
#define _alloca alloca

#define _CrtSetDbgFlag(x)

// Basic types
typedef uint32_t            DWORD;
typedef DWORD*              PDWORD;
typedef DWORD*              LPDWORD;
typedef uint8_t             BYTE;
typedef uint16_t            WORD;
typedef uint16_t            LANGID;
typedef int32_t             BOOL;

#define MAX_PATH            260
typedef float               FLOAT;
typedef char                TCHAR;
typedef const char*         LPCTSTR;
typedef char*               LPTSTR;
typedef const char*         LPCSTR;
typedef char*               LPSTR;
typedef void*               LPVOID;
typedef const void*         LPCVOID;
typedef uint32_t            UINT;
typedef int64_t             INT64;
typedef int32_t             LONG;
typedef unsigned long       ULONG;
typedef uint64_t            ULONGLONG;
typedef uint16_t            ATOM;
typedef void*               HBRUSH;
typedef void*               PVOID;
typedef uint64_t            UINT64;
typedef int64_t             LONGLONG;
typedef uintptr_t           WPARAM;
typedef uintptr_t           LPARAM;
typedef intptr_t            LRESULT;
typedef intptr_t            LONG_PTR;
typedef uintptr_t           ULONG_PTR;
typedef uint32_t            UINT_PTR;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG  HighPart;
    };
    struct {
        DWORD LowPart;
        LONG  HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

#define TRUE  1
#define FALSE 0

// HRESULT
typedef int32_t             HRESULT;
#define S_OK                ((HRESULT)0L)
#define S_FALSE             ((HRESULT)1L)
#define E_FAIL              ((HRESULT)0x80004005L)
#define E_INVALIDARG        ((HRESULT)0x80070057L)
#define E_OUTOFMEMORY       ((HRESULT)0x8007000EL)
#define E_NOTIMPL           ((HRESULT)0x80004001L)

#define SUCCEEDED(hr)       (((HRESULT)(hr)) >= 0)
#define FAILED(hr)          (((HRESULT)(hr)) < 0)

// Basic structs
typedef struct tagRECT {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} RECT, *PRECT, *LPRECT;
typedef const RECT* LPCRECT;

typedef struct tagPOINT {
    int32_t x;
    int32_t y;
} POINT, *PPOINT, *LPPOINT;
typedef const POINT* LPCPOINT;

typedef uint32_t            COLORREF;
#define RGB(r,g,b)          ((COLORREF)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))

#define __forceinline       inline __attribute__((always_inline))
#define __stdcall
#define __declspec(x)

#define _strdup             strdup
#define strcpy_s(d,sz,s)    strcpy(d,s)
#define strcat_s(d,sz,s)    strcat(d,s)
#define sprintf_s(d,sz,f,...) snprintf(d,sz,f,##__VA_ARGS__)
#define _vsnprintf_s(d,sz,c,f,a) vsnprintf(d,c,f,a)
#define _vsnprintf          vsnprintf

inline char* _itoa_s(int value, char* str, size_t size, int radix) {
    const char* format = (radix == 10) ? "%d" : ((radix == 16) ? "%x" : "%d");
    snprintf(str, size, format, value);
    return str;
}

#include <pthread.h>
#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)

inline DWORD TlsAlloc() {
    pthread_key_t key;
    if (pthread_key_create(&key, NULL) == 0)
        return (DWORD)key;
    return TLS_OUT_OF_INDEXES;
}

inline BOOL TlsFree(DWORD dwTlsIndex) {
    return pthread_key_delete((pthread_key_t)dwTlsIndex) == 0;
}

inline LPVOID TlsGetValue(DWORD dwTlsIndex) {
    return pthread_getspecific((pthread_key_t)dwTlsIndex);
}

inline BOOL TlsSetValue(DWORD dwTlsIndex, LPVOID lpTlsValue) {
    return pthread_setspecific((pthread_key_t)dwTlsIndex, lpTlsValue) == 0;
}

typedef char*               BSTR;

// Handles
typedef void*               HANDLE;
typedef void*               HWND;
typedef void*               HINSTANCE;
typedef void*               HMODULE;
typedef void*               HDC;
typedef void*               HMENU;
typedef void*               HCURSOR;
typedef void*               HICON;
typedef void*               HBITMAP;
typedef void*               HFONT;
typedef void*               HRGN;

#define CALLBACK
#define WINAPI

typedef struct tagMSG {
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;
} MSG, *PMSG, *LPMSG;

#define PM_REMOVE           0x0001
inline BOOL PeekMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg) { return FALSE; }
inline BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax) { return FALSE; }
inline LRESULT DispatchMessage(const MSG* lpMsg) { return 0; }
inline BOOL TranslateMessage(const MSG* lpMsg) { return FALSE; }
inline void PostQuitMessage(int nExitCode) {}
inline LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }
typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
inline LRESULT CallWindowProc(WNDPROC lpPrevWndFunc, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }

#define HWND_TOP            ((HWND)0)
#define HWND_BOTTOM         ((HWND)1)
#define HWND_TOPMOST        ((HWND)-1)
#define HWND_NOTOPMOST      ((HWND)-2)

#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002
#define SWP_NOZORDER        0x0004
#define SWP_FRAMECHANGED    0x0020
#define SWP_SHOWWINDOW      0x0040

inline BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) { return TRUE; }
inline BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint) { return TRUE; }
inline BOOL DestroyWindow(HWND hWnd) { return TRUE; }

typedef struct tagPAINTSTRUCT {
    HDC  hdc;
    BOOL fErase;
    RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *LPPAINTSTRUCT;

inline HDC BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint) { return nullptr; }
inline BOOL EndPaint(HWND hWnd, const PAINTSTRUCT* lpPaint) { return TRUE; }

#define WM_CREATE           0x0001
#define WM_CLOSE            0x0010
#define WM_DESTROY          0x0002
#define WM_QUIT             0x0012
#define WM_SETFONT          0x0030
#define WM_PAINT            0x000F
#define WM_SETCURSOR        0x0020
#define WM_MOUSEMOVE        0x0200
#define WM_LBUTTONDOWN      0x0201
#define WM_LBUTTONUP        0x0202
#define WM_LBUTTONDBLCLK    0x0203
#define WM_RBUTTONDOWN      0x0204
#define WM_RBUTTONUP        0x0205
#define WM_RBUTTONDBLCLK    0x0206
#define WM_MBUTTONDOWN      0x0207
#define WM_MBUTTONUP        0x0208
#define WM_MBUTTONDBLCLK    0x0209
#define WM_MOUSEWHEEL       0x020A
#define WM_MOUSEHOVER       0x02A1
#define WM_MOUSELEAVE       0x02A3
#define WM_NCMOUSEHOVER     0x02A0
#define WM_NCMOUSELEAVE     0x02A2
#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101
#define WM_CHAR             0x0102
#define WM_SYSKEYDOWN       0x0104
#define WM_SYSKEYUP         0x0105
#define WM_SYSCHAR          0x0106
#define WM_ACTIVATE         0x0006
#define WM_ACTIVATEAPP      0x001C
#define WM_SETFOCUS         0x0007
#define WM_KILLFOCUS        0x0008
#define WM_COMMAND          0x0111
#define WM_SYSCOMMAND       0x0112
#define WM_TIMER            0x0113
#define WM_HSCROLL          0x0114
#define WM_VSCROLL          0x0115
#define WM_SIZE             0x0005
#define WM_MOVE             0x0003
#define WM_WINDOWPOSCHANGING 0x0046
#define WM_ERASEBKGND       0x0014
#define WM_GETMINMAXINFO    0x0024
#define WM_ENTERSIZEMOVE    0x0231
#define WM_EXITSIZEMOVE     0x0232
#define WM_NCMOUSEMOVE      0x00A0
#define WM_NCLBUTTONDOWN    0x00A1
#define WM_NCRBUTTONDOWN    0x00A4

#define WM_XBUTTONDOWN      0x020B
#define WM_XBUTTONUP        0x020C

#define HTCLIENT            1

#define GWL_STYLE           (-16)
#define GWL_EXSTYLE         (-20)
#define GWL_WNDPROC         (-4)
#define GWLx_WNDPROC        GWL_WNDPROC

#define WS_VISIBLE          0x10000000L
#define WS_CHILD            0x40000000L
#define WS_POPUP            0x80000000L
#define WS_CAPTION          0x00C00000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_BORDER           0x00800000L
#define WS_TABSTOP          0x00010000L
#define WS_OVERLAPPED       0x00000000L
#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

#define WS_EX_TOPMOST       0x00000008L
#define WS_EX_APPWINDOW     0x00040000L
#define WS_EX_TOOLWINDOW    0x00000080L
#define WS_EX_TRANSPARENT   0x00000020L

#define CW_USEDEFAULT       ((int)0x80000000)

#define BS_PUSHBUTTON       0x00000000L
#define BS_CHECKBOX         0x00000002L
#define BS_RADIOBUTTON      0x00000004L
#define BS_OWNERDRAW        0x0000000BL

#define ES_LEFT             0x0000L

inline LRESULT GetWindowLong(HWND hWnd, int nIndex) { return 0; }
inline LRESULT SetWindowLong(HWND hWnd, int nIndex, LONG dwNewLong) { return 0; }
inline LONG_PTR GetWindowLongPtr(HWND hWnd, int nIndex) { return 0; }
inline LONG_PTR SetWindowLongPtr(HWND hWnd, int nIndex, LONG_PTR dwNewLong) { return 0; }
inline BOOL GetClientRect(HWND hWnd, LPRECT lpRect) { if(lpRect) memset(lpRect, 0, sizeof(RECT)); return TRUE; }
inline BOOL GetWindowRect(HWND hWnd, LPRECT lpRect) { if(lpRect) memset(lpRect, 0, sizeof(RECT)); return TRUE; }
inline BOOL GetCursorPos(LPPOINT lpPoint) { if(lpPoint) { lpPoint->x = 0; lpPoint->y = 0; } return TRUE; }
inline BOOL SetCursorPos(int X, int Y) { return TRUE; }
inline BOOL ScreenToClient(HWND hWnd, LPPOINT lpPoint) { return TRUE; }
inline int GetWindowTextLength(HWND hWnd) { return 0; }
inline int GetWindowText(HWND hWnd, LPTSTR lpString, int nMaxCount) { return 0; }
inline BOOL ShowCursor(BOOL bShow) { return TRUE; }
inline HDC GetDC(HWND hWnd) { return nullptr; }
inline int ReleaseDC(HWND hWnd, HDC hDC) { return 0; }
inline HWND SetCapture(HWND hWnd) { return nullptr; }
inline BOOL ReleaseCapture() { return TRUE; }
inline HWND WindowFromPoint(POINT Point) { return nullptr; }
inline HWND GetParent(HWND hWnd) { return nullptr; }
inline BOOL ClientToScreen(HWND hWnd, LPPOINT lpPoint) { return TRUE; }
inline HWND SetFocus(HWND hWnd) { return nullptr; }
inline BOOL SetForegroundWindow(HWND hWnd) { return TRUE; }
inline BOOL InvalidateRgn(HWND hWnd, HRGN hRgn, BOOL bErase) { return TRUE; }
inline BOOL RedrawWindow(HWND hWnd, const RECT* lprcUpdate, HRGN hrgnUpdate, UINT flags) { return TRUE; }
inline int MessageBox(HWND hWnd, const char* lpText, const char* lpCaption, UINT uType) { return 0; }
inline HMENU GetSystemMenu(HWND hWnd, BOOL bRevert) { return nullptr; }
inline BOOL DrawMenuBar(HWND hWnd) { return TRUE; }
inline BOOL SetWindowText(HWND hWnd, const char* lpString) { return TRUE; }
inline BOOL ShowWindow(HWND hWnd, int nCmdShow) { return TRUE; }
inline int SetScrollPos(HWND hWnd, int nBar, int nPos, BOOL bRedraw) { return 0; }

typedef struct tagSCROLLINFO {
  UINT cbSize;
  UINT fMask;
  int  nMin;
  int  nMax;
  UINT nPage;
  int  nPos;
  int  nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;

inline BOOL SetScrollInfo(HWND hWnd, int nBar, const SCROLLINFO* lpsi, BOOL bRedraw) { return TRUE; }

#define RDW_INVALIDATE          0x0001
#define RDW_INTERNALPAINT       0x0002
#define RDW_ERASE               0x0004
#define RDW_VALIDATE            0x0008
#define RDW_NOINTERNALPAINT     0x0010
#define RDW_NOERASE             0x0020
#define RDW_NOCHILDREN          0x0040
#define RDW_ALLCHILDREN         0x0080
#define RDW_UPDATENOW           0x0100
#define RDW_ERASENOW            0x0200
#define RDW_FRAME               0x0400
#define RDW_NOFRAME             0x0800

#define MB_OK                       0x00000000L
#define MB_ICONERROR                0x00000010L

#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_SHOW             5

#define SB_HORZ             0
#define SB_VERT             1
#define SB_CTL              2
#define SB_BOTH             3

inline BOOL PostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return TRUE; }
inline LRESULT SendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }
inline LRESULT SendDlgItemMessage(HWND hDlg, int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam) { return 0; }
inline BOOL TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, LPCRECT prcRect) { return TRUE; }
inline HANDLE GetCurrentProcess() { return (HANDLE)(intptr_t)-1; }
inline DWORD GetCurrentProcessId() { return (DWORD)getpid(); }

typedef enum _MINIDUMP_TYPE {
    MiniDumpNormal = 0x00000000,
    MiniDumpWithDataSegs = 0x00000001,
    MiniDumpWithHandleData = 0x00000004,
    MiniDumpWithThreadInfo = 0x00001000,
    MiniDumpWithUnloadedModules = 0x00000020,
    MiniDumpWithProcessThreadData = 0x00000100,
} MINIDUMP_TYPE;

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
    DWORD ThreadId;
    PVOID ExceptionPointers;
    BOOL  ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

inline BOOL MiniDumpWriteDump(HANDLE hProcess, DWORD ProcessId, HANDLE hFile, MINIDUMP_TYPE DumpType, PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, void* UserStreamParam, void* CallbackParam) { return TRUE; }

inline HINSTANCE GetModuleHandle(const char* lpModuleName) { return nullptr; }
inline HCURSOR LoadCursor(HINSTANCE hInstance, const char* lpCursorName) { return nullptr; }
#define IDC_ARROW           ((const char*)32512)

typedef struct tagWINDOWPOS {
    HWND hwnd;
    HWND hwndInsertAfter;
    int  x;
    int  y;
    int  cx;
    int  cy;
    UINT flags;
} WINDOWPOS, *PWINDOWPOS, *LPWINDOWPOS;

inline BOOL AdjustWindowRect(LPRECT lpRect, DWORD dwStyle, BOOL bMenu) { return TRUE; }
inline HWND CreateWindowEx(DWORD dwExStyle, const char* lpClassName, const char* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) { return (HWND)1; }


typedef struct tagWNDCLASS {
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    LPCSTR      lpszMenuName;
    LPCSTR      lpszClassName;
} WNDCLASS, *PWNDCLASS, *LPWNDCLASS;

#define PAGE_READONLY          0x02
#define PAGE_READWRITE         0x04
#define PAGE_EXECUTE_READ      0x20
#define PAGE_EXECUTE_READWRITE 0x40

typedef size_t SIZE_T;

inline BOOL VirtualProtect(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect, PDWORD lpflOldProtect) { return TRUE; }

inline ATOM RegisterClass(const WNDCLASS* lpWndClass) { return (ATOM)1; }
inline void* GetProcAddress(HINSTANCE hModule, const char* lpProcName) { return nullptr; }
inline DWORD GetModuleFileName(HMODULE hModule, LPSTR lpFilename, DWORD nSize) { return 0; }
#define GetModuleFileNameA  GetModuleFileName

inline DWORD GetCurrentDirectory(DWORD nBufferLength, char* lpBuffer) {
    if (lpBuffer == nullptr) return MAX_PATH;
    if (getcwd(lpBuffer, nBufferLength)) return strlen(lpBuffer);
    return 0;
}
#define GetCurrentDirectoryA GetCurrentDirectory

inline DWORD GetProfileString(const char* lpAppName, const char* lpKeyName, const char* lpDefault, char* lpReturnedString, DWORD nSize) {
    if (lpDefault) {
        strncpy(lpReturnedString, lpDefault, nSize);
        lpReturnedString[nSize-1] = '\0';
        return strlen(lpReturnedString);
    }
    return 0;
}

inline HICON LoadIcon(HINSTANCE hInstance, const char* lpIconName) { return nullptr; }
#define IDI_APPLICATION     ((const char*)32512)

inline HBRUSH CreateSolidBrush(COLORREF color) { return nullptr; }
inline int FillRect(HDC hDC, const RECT* lprc, HBRUSH hbr) { return 0; }
inline BOOL DeleteObject(void* ho) { return TRUE; }

#define CS_DBLCLKS          0x0008
#define CS_HREDRAW          0x0002
#define CS_VREDRAW          0x0001
#define CS_OWNDC            0x0020

typedef struct tagCREATESTRUCTA {
  LPVOID    lpCreateParams;
  HINSTANCE hInstance;
  HMENU     hMenu;
  HWND      hwndParent;
  int       cy;
  int       cx;
  int       y;
  int       x;
  LONG      style;
  LPCSTR    lpszName;
  LPCSTR    lpszClass;
  DWORD     dwExStyle;
} CREATESTRUCTA, *LPCREATESTRUCTA, *LPCREATESTRUCT, CREATESTRUCT;

inline HCURSOR SetCursor(HCURSOR hCursor) { return nullptr; }
#define EXCEPTION_CONTINUE_SEARCH    0
#define EXCEPTION_EXECUTE_HANDLER       1

typedef struct tagTRACKMOUSEEVENT {
    DWORD cbSize;
    DWORD dwFlags;
    HWND  hwndTrack;
    DWORD dwHoverTime;
} TRACKMOUSEEVENT, *LPTRACKMOUSEEVENT;

#define TME_LEAVE           0x00000002
#define HOVER_DEFAULT       0xFFFFFFFF

typedef BOOL (WINAPI *PFNTrackMouseEvent)(TRACKMOUSEEVENT* tme);

#define _T(x)      x
#define TEXT(x)    x
#define _tcscpy    strcpy
#define _tcslen    strlen
#define _tcsicmp   strcasecmp
#define _tgetenv   getenv

#define LOWORD(l)           ((uint16_t)(((uintptr_t)(l)) & 0xffff))
#define HIWORD(l)           ((uint16_t)((((uintptr_t)(l)) >> 16) & 0xffff))
#define MAKELONG(a, b)      ((int32_t)(((uint16_t)(((uintptr_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uintptr_t)(b)) & 0xffff))) << 16))

#define VK_SHIFT            0x10
#define VK_CONTROL          0x11
#define VK_MENU             0x12
#define VK_PAUSE            0x13
#define VK_CAPITAL          0x14
#define VK_ESCAPE           0x1B
#define VK_SPACE            0x20
#define VK_PRIOR            0x21
#define VK_NEXT             0x22
#define VK_END              0x23
#define VK_HOME             0x24
#define VK_LEFT             0x25
#define VK_UP               0x26
#define VK_RIGHT            0x27
#define VK_DOWN             0x28
#define VK_INSERT           0x2D
#define VK_DELETE           0x2E
#define VK_RETURN           0x0D

inline int16_t GetKeyState(int nVirtKey) { return 0; }
inline int16_t GetAsyncKeyState(int nVirtKey) { return 0; }

#define EXCEPTION_ACCESS_VIOLATION          0xC0000005L
#define EXCEPTION_DATATYPE_MISALIGNMENT      0x80000002L
#define EXCEPTION_BREAKPOINT                0x80000003L
#define EXCEPTION_SINGLE_STEP               0x80000004L
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED     0xC000008CL
#define EXCEPTION_FLT_DENORMAL_OPERAND      0xC000008DL
#define EXCEPTION_FLT_DIVIDE_BY_ZERO        0xC000008EL
#define EXCEPTION_FLT_INEXACT_RESULT        0xC000008FL
#define EXCEPTION_FLT_INVALID_OPERATION     0xC0000090L
#define EXCEPTION_FLT_OVERFLOW              0xC0000091L
#define EXCEPTION_FLT_STACK_CHECK           0xC0000092L
#define EXCEPTION_FLT_UNDERFLOW             0xC0000093L
#define EXCEPTION_INT_DIVIDE_BY_ZERO        0xC0000094L
#define EXCEPTION_INT_OVERFLOW              0xC0000095L
#define EXCEPTION_PRIV_INSTRUCTION          0xC0000096L
#define EXCEPTION_IN_PAGE_ERROR             0xC0000006L
#define EXCEPTION_ILLEGAL_INSTRUCTION       0xC000001DL
#define EXCEPTION_NONCONTINUABLE_EXCEPTION  0xC0000025L
#define EXCEPTION_STACK_OVERFLOW            0xC00000FDL
#define EXCEPTION_INVALID_DISPOSITION       0xC0000026L
#define EXCEPTION_GUARD_PAGE                0x80000001L
#define EXCEPTION_INVALID_HANDLE            0xC0000008L
#define EXCEPTION_EXECUTE_HANDLER           1

#define IMAGE_FILE_MACHINE_I386             0x014c
#define IMAGE_FILE_MACHINE_AMD64            0x8664

// SEH stubs
typedef struct _EXCEPTION_RECORD {
    uint32_t ExceptionCode;
    uint32_t ExceptionFlags;
    struct _EXCEPTION_RECORD *ExceptionRecord;
    void* ExceptionAddress;
    uint32_t NumberParameters;
    uintptr_t ExceptionInformation[15];
} EXCEPTION_RECORD;

typedef struct _CONTEXT {
    // Empty for now
} CONTEXT;

typedef struct _EXCEPTION_POINTERS {
    EXCEPTION_RECORD *ExceptionRecord;
    CONTEXT          *ContextRecord;
} EXCEPTION_POINTERS, *LPEXCEPTION_POINTERS;


#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

#define GENERIC_READ        0x80000000L
#define GENERIC_WRITE       0x40000000L
#define FILE_SHARE_READ     0x00000001L
#define FILE_SHARE_WRITE    0x00000002L
#define CREATE_ALWAYS       2
#define OPEN_ALWAYS         4
#define FILE_ATTRIBUTE_NORMAL 0x00000080L
#define FILE_FLAG_WRITE_THROUGH 0x80000000L

#define CreateFileA         CreateFile

#define OF_READ             0x00000000
#define OF_WRITE            0x00000001
#define OF_CREATE           0x00001000
#define OF_SHARE_DENY_NONE  0x00000040
#define OF_SHARE_DENY_WRITE 0x00000020

inline void SetLastError(DWORD dwErrCode) {}
inline DWORD GetLastError() { return 0; }

#define _ASSERTE(x) ZAssert(x)

inline BOOL IsBadReadPtr(const void* lp, UINT_PTR ucb) { return FALSE; }

inline HANDLE CreateFile(const char* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, void* lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
    int flags = 0;
    if (dwDesiredAccess & GENERIC_WRITE) flags |= O_WRONLY | O_CREAT;
    if (dwCreationDisposition == CREATE_ALWAYS) flags |= O_TRUNC;
    
    int fd = open(lpFileName, flags, 0666);
    if (fd < 0) return nullptr;
    return (HANDLE)(intptr_t)fd;
}

inline BOOL CloseHandle(HANDLE hObject) {
    if (!hObject) return FALSE;
    return close((int)(intptr_t)hObject) == 0;
}

inline BOOL WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, void* lpOverlapped) {
    ssize_t written = write((int)(intptr_t)hFile, lpBuffer, nNumberOfBytesToWrite);
    if (written < 0) return FALSE;
    if (lpNumberOfBytesWritten) *lpNumberOfBytesWritten = (DWORD)written;
    return TRUE;
}

inline BOOL CreateDirectory(const char* lpPathName, void* lpSecurityAttributes) {
    return mkdir(lpPathName, 0777) == 0 || errno == EEXIST;
}

inline void OutputDebugStringA(const char* lpOutputString) {
    fprintf(stderr, "%s", lpOutputString);
}

#define localtime_s(tm, time) localtime_r(time, tm)

typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

inline void GetSystemTimeAsFileTime(LPFILETIME lpFileTime) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint64_t ticks = (uint64_t)ts.tv_sec * 10000000ULL + (uint64_t)ts.tv_nsec / 100ULL + 116444736000000000ULL;
    lpFileTime->dwLowDateTime = (DWORD)(ticks & 0xFFFFFFFF);
    lpFileTime->dwHighDateTime = (DWORD)(ticks >> 32);
}

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

inline void GetLocalTime(LPSYSTEMTIME lpSystemTime) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    lpSystemTime->wYear = tm->tm_year + 1900;
    lpSystemTime->wMonth = tm->tm_mon + 1;
    lpSystemTime->wDayOfWeek = tm->tm_wday;
    lpSystemTime->wDay = tm->tm_mday;
    lpSystemTime->wHour = tm->tm_hour;
    lpSystemTime->wMinute = tm->tm_min;
    lpSystemTime->wSecond = tm->tm_sec;
    lpSystemTime->wMilliseconds = 0;
}

inline DWORD GetCurrentThreadId() { return (DWORD)(uintptr_t)pthread_self(); }

// GUIDs
typedef struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t  Data4[8];
} GUID;

#include <string.h>

inline bool operator==(const GUID& guidOne, const GUID& guidOther) {
    return memcmp(&guidOne, &guidOther, sizeof(GUID)) == 0;
}

inline bool operator!=(const GUID& guidOne, const GUID& guidOther) {
    return !(guidOne == guidOther);
}

#ifndef NOMINMAX
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#endif

#else
#include <windows.h>
#endif

#endif // _zlib_platform_h_
