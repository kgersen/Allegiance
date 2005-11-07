#include "pch.h"



//////////////////////////////////////////////////////////////////////////////
//
// Dialog
//
//////////////////////////////////////////////////////////////////////////////

Dialog::Dialog(Window* pwndParent, int nResourceID):
    Window()
{
    m_pwindowParent = pwndParent,
    m_nResourceID = nResourceID;
}

Dialog::~Dialog()
{
	Destroy();
}


/////////////////////////////////////////////////////////////////////////////
// Window creation hooks


static Dialog*    s_pDlgToHook_BugMoveToTLS = NULL;
static HHOOK    s_hDlgHook_BugMoveToTLS = NULL;

LRESULT CALLBACK Dialog::DialogCreationHook(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HCBT_CREATEWND && s_pDlgToHook_BugMoveToTLS)
        {
        LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
        HWND hWnd = (HWND)wParam;
        s_pDlgToHook_BugMoveToTLS->m_hwnd = hWnd;

        // hook the window proc
        s_pDlgToHook_BugMoveToTLS->m_pfnWndProc = (WNDPROC)::GetWindowLong(hWnd, GWL_WNDPROC);
        ::SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)Win32WndProc);

        s_mapWindow.Set(hWnd, s_pDlgToHook_BugMoveToTLS);
        s_pDlgToHook_BugMoveToTLS = NULL;
        }

    return CallNextHookEx(s_hDlgHook_BugMoveToTLS, code, wParam, lParam);
}

void Dialog::HookDialogCreate()
{
    s_pDlgToHook_BugMoveToTLS = this;
    if (s_hDlgHook_BugMoveToTLS  == NULL)
        s_hDlgHook_BugMoveToTLS = ::SetWindowsHookEx(WH_CBT, Dialog::DialogCreationHook, NULL, ::GetCurrentThreadId());
}

BOOL Dialog::UnhookDialogCreate()
{
    ::UnhookWindowsHookEx(s_hDlgHook_BugMoveToTLS);
    s_hDlgHook_BugMoveToTLS = NULL;
    if (s_pDlgToHook_BugMoveToTLS != NULL)
        {
        s_pDlgToHook_BugMoveToTLS = NULL;
        return FALSE;   // was not successfully hooked
        }
    return TRUE;
}

BOOL Dialog::Create()
{
       
    HookDialogCreate();
    if (NULL == (m_hwnd = CreateDialog((HINSTANCE)GetModuleHandle(NULL),
                        MAKEINTRESOURCE(m_nResourceID),
                        m_pwindowParent->GetHWND(),
                        Win32DlgProc)))
        {
        UnhookDialogCreate();
        return FALSE;
        }

    //
    // Set the clipping bits correctly
    //

    ::SetWindowLong(
        m_hwnd,
        GWL_STYLE,
        ::GetWindowLong(m_hwnd, GWL_STYLE) | WS_CLIPCHILDREN | WS_CLIPSIBLINGS
    );

    if (!UnhookDialogCreate())
        return FALSE;

    //if (m_pwindowParent) {
    //    m_pwindowParent->AddChild(this);

    return TRUE;
}

void Dialog::Destroy()
{
	if (m_hwnd)
		{
	    DestroyWindow(m_hwnd);
	    s_mapWindow.Remove(m_hwnd);
	    m_hwnd = NULL;
	    }
}

BOOL CALLBACK Dialog::Win32DlgProc(
    HWND hwnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    Window* pwindow;

    if (!s_mapWindow.Find(hwnd, pwindow))
        pwindow = NULL;

    if (pwindow && message == WM_INITDIALOG)
        return ((Dialog*)pwindow)->OnInitDialog();

    return FALSE;
}

/*
BOOL Dialog::DlgProc(
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (message) 
        {
        case WM_INITDIALOG:
            return OnInitDialog();

        case WM_DESTROY:
            OnDestroy();
            return TRUE;

        case WM_COMMAND:
            return OnCommand(wParam, lParam);

        case WM_NOTIFY:
            {
            LRESULT lres;
            return OnNotify(((NMHDR*)lParam)->hwndFrom, ((NMHDR*)lParam)->idFrom, 
                        ((NMHDR*)lParam)->code, lParam, &lres);
            }
        case WM_DRAWITEM:
            OnDrawItem((UINT) wParam, (LPDRAWITEMSTRUCT) lParam);
            return 0;

        case WM_PAINT:
            PAINTSTRUCT paint;
            BeginPaint(GetHWND(), &paint);
            OnPaint(paint.hdc, paint.rcPaint);
            EndPaint(GetHWND(), &paint);
            return 0;
        }
    
    return FALSE;
}
*/

DWORD Dialog::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
        {
        case WM_DESTROY:
            OnDestroy();
            return TRUE;

        case WM_COMMAND:
            return OnCommand(wParam, lParam);

        case WM_NOTIFY:
            {
            LRESULT lres;
            if (OnNotify(((NMHDR*)lParam)->hwndFrom, ((NMHDR*)lParam)->idFrom, 
                        ((NMHDR*)lParam)->code, lParam, &lres))
                return lres;
            break;                
            }
        case WM_DRAWITEM:
            OnDrawItem((UINT) wParam, (LPDRAWITEMSTRUCT) lParam);
            return 0;

        case WM_PAINT:
            PAINTSTRUCT paint;
            BeginPaint(GetHWND(), &paint);
            OnPaint(paint.hdc, paint.rcPaint);
            EndPaint(GetHWND(), &paint);
            return 0;
        }

    return Window::WndProc(message, wParam, lParam);
}


BOOL Dialog::OnInitDialog()
{
    return FALSE;
}
    
BOOL Dialog::OnNotify(HWND hwndFrom, UINT idFrom, UINT code, LPARAM lParam, LRESULT* pResult)
{
    return FALSE;
}

void Dialog::OnDrawItem(UINT nCtrlID, LPDRAWITEMSTRUCT lpdis)
{
}
