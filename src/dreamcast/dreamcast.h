#include "windef.h"
#include "shintr.h"
#include "strig.h"

#define ZeroMemory(pmem, size) memset(pmem, 0, size)
/*
// redefine functions
#define acosf(x)    ((float)acos((double)(x)))
#define asinf(x)    ((float)asin((double)(x)))
#define atanf(x)    ((float)atan((double)(x)))
#define atan2f(x,y) ((float)atan2((double)(x), (double)(y)))
#define ceilf(x)    ((float)ceil((double)(x)))
#define cosf(x)     ((float)cos((double)(x)))
#define coshf(x)    ((float)cosh((double)(x)))
#define expf(x)     ((float)exp((double)(x)))
#define fabsf(x)    ((float)fabs((double)(x)))
#define floorf(x)   ((float)floor((double)(x)))
#define fmodf(x,y)  ((float)fmod((double)(x), (double)(y)))
#define frexpf(x,y) ((float)frexp((double)(x), (y)))
#define hypotf(x,y) ((float)hypot((double)(x), (double)(y)))
#define ldexpf(x,y) ((float)ldexp((double)(x), (y)))
#define logf(x)     ((float)log((double)(x)))
#define log10f(x)   ((float)log10((double)(x)))
#define modff(x,y)  ((float)modf((double)(x), (double *)(y)))
#define powf(x,y)   ((float)pow((double)(x), (double)(y)))
#define sinf(x)     ((float)sin((double)(x)))
#define sinhf(x)    ((float)sinh((double)(x)))
#define sqrtf(x)    ((float)sqrt((double)(x)))
#define tanf(x)     ((float)tan((double)(x)))
#define tanhf(x)    ((float)tanh((double)(x)))
*/
#define _stricmp    strcmp
#define _strnicmp   strncmp
#define isspace(_c) ( _c == ' ' )


HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, 
        int fnWeight, DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, 
        DWORD fdwCharSet, DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
        DWORD fdwPitchAndFamily, LPCSTR lpszFace);
BOOL TextOut(HDC hdc, int nXStart, int nYStart, LPCSTR lpString, int cbString);

#define Stub(name,type,value) inline type name(...) { return value; }

// todo: reimplement functions
Stub(wvsprintfA, int, 0)
Stub(wsprintf, int, 0)
Stub(sprintf, int, 0)
Stub(GetProfileString, DWORD, 0)
Stub(GetCurrentDirectory, DWORD, 0)
Stub(GetDIBits, BOOL, FALSE)
Stub(GetDIBColorTable, int, 16)
Stub(GetClientRect, BOOL, FALSE)
Stub(GetWindowRect, BOOL, FALSE)

// stub out these functions
Stub(InvalidateRgn, BOOL, TRUE)
Stub(RedrawWindow, BOOL, TRUE)
Stub(MessageBox, int, 0)
Stub(GetSystemMenu, HMENU, NULL)
Stub(DrawMenuBar, BOOL, TRUE)
Stub(SetWindowText, BOOL, TRUE)
Stub(ShowWindow, BOOL, TRUE)
Stub(SetScrollPos, int, 0)
Stub(SetScrollInfo, int, 0)
Stub(ClientToScreen, BOOL, TRUE)
Stub(ScreenToClient, BOOL, TRUE)
Stub(SendDlgItemMessage, LONG, 0)
Stub(TrackPopupMenu, BOOL, FALSE)
Stub(GetDC, HDC, NULL)
Stub(ReleaseDC, int, 0)
Stub(FillRect, BOOL, FALSE)
Stub(CreateSolidBrush, HBRUSH, NULL)
Stub(LoadCursor, HCURSOR, NULL)
Stub(AdjustWindowRect, BOOL, TRUE)
Stub(MoveWindow, BOOL, TRUE)
Stub(SetCursorPos, BOOL, TRUE)
Stub(SetCursor, BOOL, TRUE)
Stub(GetWindowTextLength, LONG, 0)
Stub(GetWindowText, BOOL, FALSE)
Stub(ShowCursor, BOOL, TRUE)
Stub(SetWindowPos, BOOL, TRUE)
Stub(GetCursorPos, BOOL, FALSE)
Stub(WindowFromPoint, HWND, NULL)
Stub(BeginPaint, BOOL, TRUE)
Stub(EndPaint, BOOL, TRUE)
Stub(MoveToEx, BOOL, FALSE)
Stub(LineTo, BOOL, FALSE)

typedef struct tagTRACKMOUSEEVENT {
    DWORD cbSize;
    DWORD dwFlags;
    HWND  hwndTrack;
    DWORD dwHoverTime;
} TRACKMOUSEEVENT, *LPTRACKMOUSEEVENT;

#define TME_LEAVE           0
#define HOVER_DEFAULT       0
#define WM_MOUSELEAVE       0x02A3
#define CS_HREDRAW          0
#define CS_VREDRAW          0


// bogus constants
#define SW_SHOW                 0
#define OF_READ					0
#define OF_SHARE_DENY_WRITE		0
#define OF_CREATE				0
#define OF_WRITE				0
#define OF_SHARE_EXCLUSIVE      0
#define RDW_FRAME               0
#define RDW_INVALIDATE          0
#define RDW_UPDATENOW           0
#define WS_OVERLAPPED           0
#define WS_POPUP                0
#define WS_CLIPSIBLINGS         0
#define WS_CLIPCHILDREN         0
#define WS_CAPTION              0
#define WS_BORDER               0
#define WS_SYSMENU              0
#define WS_MINIMIZEBOX          0
#define WS_MAXIMIZEBOX          0
#define WS_OVERLAPPEDWINDOW     0
#define WS_POPUPWINDOW          0
#define WS_CHILDWINDOW          0
#define WS_THICKFRAME           0
#define WS_EX_TOPMOST           0
#define WS_EX_TRANSPARENT       0
#define BS_PUSHBUTTON           0
#define BS_CHECKBOX             0
#define BS_RADIOBUTTON          0
#define BS_OWNERDRAW            0
#define ES_LEFT                 0
#define ES_AUTOHSCROLL          0
#define IDC_ARROW               0
#define ID_APP_ICON             0
#define SWP_NOSIZE              0
#define SWP_NOZORDER            0
#define SWP_NOMOVE              0
#define MB_ICONEXCLAMATION      0
#define MB_OK                   0
#define LR_LOADFROMFILE         0
#define LR_CREATEDIBSECTION     0
#define DIB_RGB_COLORS          0

// bogus windows messages
#define WM_SETFONT              0x0030
#define WM_PAINT                0x000F
#define WM_SETCURSOR            0x0020
#define WM_MOVE                 0x0003
#define WM_SIZE                 0x0005
#define WM_HSCROLL              0x0114
#define WM_VSCROLL              0x0115
#define WM_COMMAND              0x0111
#define WM_SYSCOMMAND           0x0112
#define WM_ACTIVATEAPP          0x001C
#define WM_DISPLAYCHANGE        0x007E
#define WM_WINDOWPOSCHANGED     0x0047
#define WM_WINDOWPOSCHANGING    0x0046
#define SC_KEYMENU              0xF100

//bogus structures

typedef void* LPSCROLLINFO;

typedef struct tagWINDOWPOS {
    HWND    hwnd;
    HWND    hwndInsertAfter;
    int     x;
    int     y;
    int     cx;
    int     cy;
    UINT    flags;
} WINDOWPOS, *LPWINDOWPOS, *PWINDOWPOS;

typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;


