//---------------------------------------------------------------------------------------------
//
//  This file provides DirectDraw rendering services through FlipHELP, a programming
//  technique that allows for the rendering of non-DirectDraw-aware windows to the
//  DirectDraw GDI surface.
//
//  Also of interest, and noted in this header, is FlipHelpES, which provides additional
//  services to FlipHELP. FlipHelpES allows for the rendering of out-of-process windows
//  (WOOPs) to the DirectDraw GDI surface. Prior to this, any activation of a window in
//  another process caused DirectDraw to minimize and mode-switch (its default behavior).
//
//  Search on the term "fswindow" in the DirectX 6.1 SDK documentation for additional 
//  details and alternative sample code.
//
//---------------------------------------------------------------------------------------------
//
//  (C) Copyright 1995-1999 Microsoft Corp.  All rights reserved.
//
//  You have a royalty-free right to use, modify, reproduce and
//  distribute the Sample Files (and/or any modified version) in
//  any way you find useful, provided that you agree that
//  Microsoft has no warranty obligations or liability for any
//  Sample Application Files which are modified.
//
//---------------------------------------------------------------------------------------------

#include "pch.h"
#include "FlipHelp.h"
//#include "FlipHelpES.h"             // support for out-of-process windows (WOOPs)

//
// declare/define global
//
SIZE    structSurfaceSize = {0, 0}; // DirectDraw front buffer dimensions

//
// declare/define module globals
//
static LPDIRECTDRAW2        ddObject = NULL;
/*
static LPDIRECTDRAWSURFACE3 ddFrontBuffer = NULL;
static LPDIRECTDRAWSURFACE3 ddBackBuffer = NULL;
*/
static LPDIRECTDRAWSURFACE2 ddFrontBuffer = NULL;
static LPDIRECTDRAWSURFACE2 ddBackBuffer = NULL;
static LPDIRECTDRAWCLIPPER  ddClipper = NULL;
static HWND                 hwndHelpWindow = NULL;
static HBITMAP              hwndHelpWindowBMP = NULL;
static HWND                 hwndAppWindow = NULL;
static HRESULT              hrResult = 0;

static BOOL                 f3Dfx = FALSE;
static BOOL                 fDrawMouse = FALSE;
static BOOL                 fBuffersSame = FALSE;
static BOOL                 fUsingBadDriver = FALSE;
static BOOL                 fStaticContent = FALSE;

//
// internal function prototypes
//
static HBITMAP              CreateHelpBMP(HWND hwnd);
static HBITMAP              CreateDibBMP(HDC hdc, int w, int h, int bpp);
static HRGN                 GetInvWindowRgn(HWND hwnd);
//static LPDIRECTDRAW2        GetDDObjectFromDDSurface(LPDIRECTDRAWSURFACE3 ddSurface);
static LPDIRECTDRAW2        GetDDObjectFromDDSurface(LPDIRECTDRAWSURFACE2 ddSurface);
/*
static BOOL                 CheckForBrokenDriver(LPDIRECTDRAWSURFACE3 pSurface);

*/


static BOOL                 CheckForBrokenDriver(LPDIRECTDRAWSURFACE2 pSurface);
static void                 GetOptimalDIBFormat(HDC hdc, BITMAPINFOHEADER *pbi);

//
// external function prototype
//
extern "C" int APIENTRY GetRandomRgn(HDC, HRGN, int);   // GDI function to get a region from a HDC

//---------------------------------------------------------------------------------------------
// FlipHelp_Init() does preliminary setup of global values for FlipHelp. It should get
// called each time DirectDraw surfaces are altered (i.e. changes to the device that
// the client application is running under).
//
//  Be sure to call FlipHelp_Release() prior to calling any subsequent FlipHelp_Init() calls,
//  otherwise, the reference count on the DirectDraw objects will become out of sync.
//
//  Note that the buffers passed in are of type LPVOID. This allows DirectDraw buffers of
//  any surface interface type to be accepted. However, this function will fail if it
//  cannot get a base DirectDraw interface of type IID_IDirectDrawSurface.
//
//  purpose:        assigns local DD object and buffers, and sets globals
//  requires:       handle to app window, front/back buffer pointers, initialization flags
//  returns:        HRESULT
//  side effect:    sets globals and attaches clipper to DD object
//---------------------------------------------------------------------------------------------
HRESULT FlipHelp_Init(HWND hwndApp, LPVOID FrontBuffer, LPVOID BackBuffer, DWORD dwFlags)
    {
    hrResult = S_OK;

    //
    // check for valid buffer pointers
    //
    if (FrontBuffer == NULL || BackBuffer == NULL)
        return(HR_ERR);

    //
    //  make sure we're not trying to re-init (addref) our buffers
    //
    FlipHelp_Release();

    //
    // check for LPDIRECTDRAWSURFACE3 interface, return if none found
    //
   /* if (!((hrResult = ((IUnknown*)FrontBuffer)->QueryInterface(IID_IDirectDrawSurface3, (LPVOID *)&ddFrontBuffer)) == DD_OK) ||
        !((hrResult = ((IUnknown*)BackBuffer)->QueryInterface(IID_IDirectDrawSurface3, (LPVOID *)&ddBackBuffer)) == DD_OK))
		*/
	if (!((hrResult = ((IUnknown*)FrontBuffer)->QueryInterface(IID_IDirectDrawSurface2, (LPVOID *)&ddFrontBuffer)) == DD_OK) ||
        !((hrResult = ((IUnknown*)BackBuffer)->QueryInterface(IID_IDirectDrawSurface2, (LPVOID *)&ddBackBuffer)) == DD_OK))
        {
        FlipHelp_Release();
        return(hrResult);
        }

    //
    // get handle to application window
    //
    hwndAppWindow = hwndApp;

    //
    //  set flags
    //
    fDrawMouse = (BOOL)(dwFlags & FH_DRAW_MOUSE);       // do we draw hardware mouse?
    fBuffersSame = (ddFrontBuffer == ddBackBuffer);     // are DirectDraw buffers identical?
    f3Dfx = (BOOL)(dwFlags & FH_3DFX_HARDWARE);         // are we running on 3Dfx hardware?

    //
    // check for broken drivers that do not do 555<->565 blts correctly
    //
    fUsingBadDriver = ((f3Dfx) && (!(dwFlags & FH_BITMAP_UPDATE)) && CheckForBrokenDriver(ddBackBuffer));

    //
    // if not 3Dfx, then get DirectDraw object from surface passed in
    // needed for CreateClipper() and FlipToGDISurface() calls
    //
    if ((!f3Dfx) && (!(ddObject = GetDDObjectFromDDSurface(ddFrontBuffer))))
        return(hrResult);

    //
    // get DirectDraw surface dimensions
    //
    DDSURFACEDESC ddsd;
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;

    hrResult = ddBackBuffer->GetSurfaceDesc(&ddsd);
    structSurfaceSize.cx = ddsd.dwWidth;
    structSurfaceSize.cy = ddsd.dwHeight;

    return(hrResult);
    }

//---------------------------------------------------------------------------------------------
// FlipHelp_Begin() preps the DirectDraw surface depending on 3D hardware. It should get called
// whenever a window (represented by the hwnd parameter) needs to be displayed under DirectDraw.
// FlipHelp_Begin() should also get called if the window changes its content (if its static
// content becomes dynamic, and vice-versa).
//
//  purpose:        for 3DFX, creates content image, else just flip to GDI surface
//  requires:       handle to content window, 3DFX flag and if content is dynamic or static
//  returns:        handle to the hwndHelpWindow, else NULL if not successful
//  side effect:    creates content image or flips to GDI surface
//---------------------------------------------------------------------------------------------


HWND FlipHelp_Begin(HWND hwnd, BOOL bStaticContent)
    {
    //
    // if no handle passed in, assume existing content window
    //
    if (hwnd == NULL)
        {
        if ((hwnd = hwndHelpWindow) == NULL)
            return(NULL);
        }

    fStaticContent = bStaticContent;

    if (f3Dfx)
        {
        //
        // constrain cursor to DirectDraw surface
        //
        RECT rc = {0, 0, structSurfaceSize.cx, structSurfaceSize.cy};
        ClipCursor(&rc);

        //
        // clear out lingering content
        //
        if (hwndHelpWindowBMP)
            {
            DeleteObject(hwndHelpWindowBMP);
            hwndHelpWindowBMP = NULL;
            }

        //
        // need to create an image of content window just once
        //
        if (fStaticContent)
            {
            if (!FlipHelp_IsActive())
                UpdateWindow(hwnd);

            //
            // assign content window image to global
            //
            hwndHelpWindowBMP = CreateHelpBMP(hwnd);
            }
        }
    else
        {
        //
        // create a clipper (used in LPDIRECTDRAWSURFACE::Blt)
        //
        if (ddObject->CreateClipper(0, &ddClipper, NULL) == DD_OK)
            {
            hrResult = ddClipper->SetHWnd(0, hwndAppWindow);
            assert(SUCCEEDED(hrResult));

            //
            // attach the clipper here if buffers are the same
            //
            if (fBuffersSame)
                {
                hrResult = ddFrontBuffer->SetClipper(ddClipper);
                assert(SUCCEEDED(hrResult));
                }
            }
        else
            return(NULL);

        //
        // not 3DFX, so just flip to GDI so content window can be seen
        //
        hrResult = ddObject->FlipToGDISurface();
        }

    hwndHelpWindow = hwnd;
    return(hwndHelpWindow);
    }

//---------------------------------------------------------------------------------------------
// FlipHelp_End() deletes objects associated with the content window. Note that these are
// objects created within this module, not objects created by the calling client (e.g. content
// window). Call this function whenever the content window is destroyed (e.g. WM_CLOSE).
//
//  purpose:        cleans up after content window
//  requires:       flag to determine if running under 3DFX
//  returns:        none
//  side effect:    cleans up anything associated with existing content window
//---------------------------------------------------------------------------------------------
void FlipHelp_End()
    {
    if (hwndHelpWindowBMP)
        {
        DeleteObject(hwndHelpWindowBMP);
        hwndHelpWindowBMP = NULL;
        }

    hwndHelpWindow = NULL;

    if (f3Dfx)
        ClipCursor(NULL);

	if (ddFrontBuffer)
		ddFrontBuffer->SetClipper(NULL);

    RELEASE(ddClipper);
    }

//---------------------------------------------------------------------------------------------
// FlipHelp_Update() is responsible for the actual rendering of the help content window (held
// in global hwndHelpWindow). This function must be called each time a DirectDraw frame
// gets rendered and FlipHelp_IsActive() returns TRUE, so it should be placed in the main
// application's DirectDraw rendering routine. An example of this might look like the
// following:
//
// void RenderFrame()
//      {
//      if (FlipHelp_IsActive())
//          {
//          FlipHelp_Update();
//          }
//      else
//          {
//          FrontBuffer->Blt(...);
//          }
//      }
//
//  purpose:        main routine to handle DirectDraw output of content window
//  requires:       handle to application window and 3DFX flag
//  returns:        HRESULT
//  side effect:    blts/flips content window to DirectDraw surface
//---------------------------------------------------------------------------------------------
HRESULT FlipHelp_Update()
    {
    hrResult = 0;

    //
    // if front and back buffers are the same, then we're already drawing to display
    //
    if (fBuffersSame)
        return(hrResult);

    //
    // new multiwindow method
    //
    if (f3Dfx)
        {
        //
        // get a DC to the backbuffer on the 3Dfx (where we need to copy it)
        //
        HDC hdcBackBuffer;
        hrResult = ddBackBuffer->GetDC(&hdcBackBuffer);
        assert(SUCCEEDED(hrResult));

        if (FlipHelp_IsStatic())
            {
            //
		    // if window has a complex region associated with it, be sure to include it in the draw
            //
            RECT rc;
            GetWindowRect(hwndHelpWindow, &rc);
		    HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
		    if (GetWindowRgn(hwndHelpWindow, hrgn) == COMPLEXREGION)
		        {
			    OffsetRgn(hrgn, rc.left, rc.top);
			    SelectClipRgn(hdcBackBuffer, hrgn);
			    }

            //
            // since the window content is static, use the existing bitmap image
            //
			HDC hdcMemory = CreateCompatibleDC(NULL);
			SelectObject(hdcMemory, hwndHelpWindowBMP);
			BitBlt(hdcBackBuffer, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), hdcMemory, 0, 0, SRCCOPY);
			DeleteDC(hdcMemory);
            SelectClipRgn(hdcBackBuffer, NULL); 
            }
        else if (fUsingBadDriver)
            {
            //
            // to resolve the bad video driver situation, we must first create a bitmap, and then blt
            // to the screen, the same as is done in FlipHelp_IsStatic(), except that it must get
            // done for each successive frame
            //
			HDC hdcMemory = CreateCompatibleDC(NULL);
            HBITMAP hbm = CreateHelpBMP(hwndAppWindow);
			SelectObject(hdcMemory, hbm);

            //
            // since the window content is always changing, an image of it must
            // get created with each frame update
            //
            RECT rc;
            HRGN hrgn = GetInvWindowRgn(hwndAppWindow);
            GetRgnBox(hrgn, &rc);
            HDC hdcScreen = GetDC(NULL);

            //
            // blt from screen to memory
            //
            SelectClipRgn(hdcMemory, hrgn);
			BitBlt(hdcMemory, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), hdcScreen, rc.left, rc.top, SRCCOPY);
            SelectClipRgn(hdcMemory, NULL);

            //
            // blt from memory to backbuffer
            //
            SelectClipRgn(hdcBackBuffer, hrgn);
			BitBlt(hdcBackBuffer, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), hdcMemory, rc.left, rc.top, SRCCOPY);
            SelectClipRgn(hdcBackBuffer, NULL);

            DeleteObject(hrgn);
            DeleteObject(hdcMemory);
            DeleteObject(hbm);
            ReleaseDC(NULL, hdcScreen);
            }
        else
            {
            //
            // since the window content is always changing, an image of it must
            // get created with each frame update
            //
            RECT rc;
            HRGN hrgn = GetInvWindowRgn(hwndAppWindow);
            GetRgnBox(hrgn, &rc);
            HDC hdcScreen = GetDC(NULL);

            //
            // blt from screen directly to backbuffer
            //
            SelectClipRgn(hdcBackBuffer, hrgn);
			BitBlt(hdcBackBuffer, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), hdcScreen, rc.left, rc.top, SRCCOPY);
            SelectClipRgn(hdcBackBuffer, NULL);

            DeleteObject(hrgn);
            ReleaseDC(NULL, hdcScreen);
            }

        //
        // draw mouse cursor
        //
        if (fDrawMouse)
            {
            //
            // values for tracking changes in mouse cursor between blts
            //
            static HCURSOR      MouseCursor;
            static ICONINFO     IconInfo;
            HCURSOR             MouseCursorCur;
            HWND                hwndF = GetForegroundWindow();

            //
            // check for existence of out-of-process window (WOOP)
            // if it's the foreground window, then need to use AttachThreadInput() to
            // get back valid results of GetCursor() call
            //
            /*if (hwndF && GetWindowThreadProcessId(hwndF, NULL) == GetWindowThreadProcessId(hwndWOOP, NULL))
                {
                //
                // need to attach thread in order to allow correct return of GetCursor() call
                //
                DWORD dwWinHelpThreadId = GetWindowThreadProcessId(hwndWOOP, NULL);
                AttachThreadInput(GetCurrentThreadId(), dwWinHelpThreadId, TRUE);
                MouseCursorCur = GetCursor();
                AttachThreadInput(GetCurrentThreadId(), dwWinHelpThreadId, FALSE);
                }
            else*/
                {
                MouseCursorCur = GetCursor();
                };

            //
            // update the cursor only if it's changed
            //
            if (MouseCursorCur != MouseCursor)
                {
                MouseCursor = MouseCursorCur;
                GetIconInfo(MouseCursor, &IconInfo);

                if (IconInfo.hbmMask)
                    DeleteObject(IconInfo.hbmMask);

                if (IconInfo.hbmColor)
                    DeleteObject(IconInfo.hbmColor);
                };

            POINT pt;
            GetCursorPos(&pt);
            pt.x -= IconInfo.xHotspot;
            pt.y -= IconInfo.yHotspot;
            DrawIcon(hdcBackBuffer, pt.x, pt.y, MouseCursor);
            }

        hrResult = ddBackBuffer->ReleaseDC(hdcBackBuffer);
        assert(SUCCEEDED(hrResult));

        hrResult = ddFrontBuffer->Flip(NULL, DDFLIP_WAIT);
        assert(SUCCEEDED(hrResult));

        }
    else    // not 3DFX hardware
        {
        //
        // create region to update
        //
        POINT ptScreen = {0, 0};
        ClientToScreen(hwndAppWindow, &ptScreen);
        RECT rc = {ptScreen.x , ptScreen.y, ptScreen.x + structSurfaceSize.cx, ptScreen.y + structSurfaceSize.cy};

        hrResult = ddFrontBuffer->SetClipper(ddClipper);

        //
        // update the surface with a blt
        //
        hrResult = ddFrontBuffer->Blt(&rc, ddBackBuffer, NULL, DDBLT_WAIT, NULL);
        assert(SUCCEEDED(hrResult));
        };

    return(hrResult);
    }

//---------------------------------------------------------------------------------------------
// FlipHelp_IsActive() simply checks to see if there's a content window displayed. This check
// should be made prior to calling FlipHelp_Update().
//
//  purpose:        checks status of help window
//  requires:       none
//  returns:        true/false
//  side effect:    none
//---------------------------------------------------------------------------------------------
BOOL FlipHelp_IsActive(void)
    {
    return(hwndHelpWindow != NULL);
    }

//---------------------------------------------------------------------------------------------
// FlipHelp_IsStatic() checks to see whether or not the content window needs to be regularly
// updated (its content is dynamic, such as an animation or text entry field). A static window
// is created (an image of the window created with a call to CreateHelpBMP()) once and used
// over and over.
//
//  purpose:        checks if help window content is static or dynamic
//  requires:       none
//  returns:        true/false
//  side effect:    none
//---------------------------------------------------------------------------------------------
BOOL FlipHelp_IsStatic(void)
    {
    return(fStaticContent);
    }

//---------------------------------------------------------------------------------------------
// FlipHelp_Release() releases interface pointers as necessary. It should get
// called each time DirectDraw surfaces are altered (e.g. changes to the device that
// the client application is running under).
//
//  purpose:        decrement refcount for interfaces created in FlipHelp
//  requires:       none
//  returns:        none
//  side effect:    releases interface pointers created in FlipHelp
//---------------------------------------------------------------------------------------------
void FlipHelp_Release()
    {
    RELEASE(ddObject);
    RELEASE(ddBackBuffer);
    RELEASE(ddFrontBuffer);
    }

//---------------------------------------------------------------------------------------------
// CreateHelpBMP() takes the hwnd of the content window, and returns a bitmap handle. Note that
// this is an internal (not exported) function.
//
//  purpose:        creates a bitmap from hwnd (content window)
//  requires:       handle to content window
//  returns:        handle to updated bitmap
//  side effect:    none
//---------------------------------------------------------------------------------------------
static HBITMAP CreateHelpBMP(HWND hwnd)
    {
    //
    // create a bitmap of the window passed in
    //
    RECT rc;
    GetWindowRect(hwnd, &rc);

    int x = rc.left;
    int y = rc.top;
    int cx = rc.right - rc.left;
    int cy = rc.bottom - rc.top;

    HDC hdcScreen = GetDC(NULL);
    HDC hdcMemory = CreateCompatibleDC(NULL);

    HBITMAP hbmBitmap = NULL;
    hbmBitmap = CreateDibBMP(hdcScreen, cx, cy, 0);

    //
    // blt the image from screen to bitmap
    //
    SelectObject(hdcMemory, hbmBitmap);
    BitBlt(hdcMemory, 0, 0, cx, cy, hdcScreen, x, y, SRCCOPY);
    DeleteDC(hdcMemory);
    ReleaseDC(NULL, hdcScreen);

    return(hbmBitmap);
    }

//---------------------------------------------------------------------------------------------
// CreateDibBMP() created an empty bitmap, used exclusively in CreateHelpBMP(). Note that
// this is an internal (not exported) function.
//
//  purpose:        creates an "empty" bitmap
//  requires:       dc, dimensions of image, and color depth (bits per pixel)
//                  note that bpp == 0 will call into GetOptimalDIBFormat()
//  returns:        handle to new bitmap
//  side effect:    none
//---------------------------------------------------------------------------------------------
static HBITMAP CreateDibBMP(HDC hdc, int w, int h, int bpp)
    {
    LPVOID lpBits;

    struct
        {
        BITMAPINFOHEADER bi;
        DWORD ct[256];
        } dib;

    dib.bi.biSize = sizeof(BITMAPINFOHEADER);
    dib.bi.biWidth = w;
    dib.bi.biHeight = h;
    dib.bi.biBitCount = (WORD)bpp;
    dib.bi.biPlanes = 1;
    dib.bi.biCompression = 0;
    dib.bi.biSizeImage = 0;
    dib.bi.biClrUsed = 0;

    if (bpp == 0)
        {
        GetOptimalDIBFormat(hdc, &dib.bi);
        dib.bi.biBitCount = (dib.bi.biBitCount < 16 ? 16 : dib.bi.biBitCount);
        dib.bi.biWidth = w;
        dib.bi.biHeight = h;
        }
    else if (bpp == 15)
        {
        dib.bi.biBitCount = 16;
        }
    else if (bpp == 16)
        {
        dib.bi.biCompression = BI_BITFIELDS;
        dib.ct[0] = 0xf800;
        dib.ct[1] = 0x07E0;
        dib.ct[2] = 0x001F;
        };

    return(CreateDIBSection(hdc, (LPBITMAPINFO)&dib, DIB_RGB_COLORS, &lpBits, NULL, 0));
    }

//---------------------------------------------------------------------------------------------
// GetInvWindowRgn() get the inverse of the visible region of a window
//
//  purpose:        used to Blt the contents of a window to the 3DFX
//  requires:       handle to the app window
//  returns:        HRGN of non-visible region
//  side effect:    none
//---------------------------------------------------------------------------------------------
static HRGN GetInvWindowRgn(HWND hwnd)
    {
    HRGN    WinRgn;
    HRGN    VisRgn;
    HRGN    InvRgn;
    RECT    rc;
    HDC     hdc;

    //
    // get the whole window rect/rgn
    //
    GetWindowRect(hwnd, &rc);
    WinRgn = CreateRectRgnIndirect(&rc);

    //
    // get the visible region
    //
    hdc = GetDCEx(hwnd, NULL, DCX_CACHE | DCX_CLIPCHILDREN | DCX_CLIPSIBLINGS);
    VisRgn = CreateRectRgn(0, 0, 0, 0);
    GetRandomRgn(hdc, VisRgn, 4);
    ReleaseDC(hwnd, hdc);

    //
    // subtract the vis rgn from the whole window rgn, where InvRgn = WinRgn - VisRgn;
    //
    InvRgn = CreateRectRgn(0, 0, 0, 0);
    CombineRgn(InvRgn, WinRgn, VisRgn, RGN_DIFF);
    DeleteObject(WinRgn);
    DeleteObject(VisRgn);

    //
    // convert the region from screen cordinates to window cordinates
    //
    OffsetRgn(InvRgn, -rc.left, -rc.top);
    return(InvRgn);
    }

//---------------------------------------------------------------------------------------------
// GetDDObjectFromDDSurface() gets the DirectDraw object from a surface passed in
//
//  purpose:        gets DirectDraw object from DirectDraw surface
//  requires:       pointer to DirectDraw surface
//  returns:        pointer to DirectDraw object
//  side effect:    none
//---------------------------------------------------------------------------------------------
//static LPDIRECTDRAW2 GetDDObjectFromDDSurface(LPDIRECTDRAWSURFACE3 ddSurface)
static LPDIRECTDRAW2 GetDDObjectFromDDSurface(LPDIRECTDRAWSURFACE2 ddSurface)
    {
    LPDIRECTDRAW2           ddObject = NULL;
    LPDIRECTDRAWSURFACE2    ddSurface2 = NULL;

    hrResult = 0;

    //
    // check to make sure surface is valid
    //
    if (ddSurface == NULL)
        return(NULL);

    //
    // get the DirectDraw object, but first check for LPDIRECTDRAWSURFACE2 interface
    // needed for GetDDInterface() call
    //
    if (ddSurface->QueryInterface(IID_IDirectDrawSurface2, (LPVOID *)&ddSurface2) != DD_OK)
        return(NULL);

    hrResult = ddSurface2->GetDDInterface((LPVOID *)&ddObject);
    assert(SUCCEEDED(hrResult));

    hrResult = ddSurface2->Release();
    assert(SUCCEEDED(hrResult));

    return(ddObject);
    }

//---------------------------------------------------------------------------------------------
// CheckForBrokenDriver() checks for the case when the 2D video driver does not correctly
// handle 565<->555 bitmap conversion
//
//  purpose:        side-step really bad video drivers in this world
//  requires:       DirectDraw surface
//  returns:        whether or not the video driver has passed the test
//  side effect:    none
//---------------------------------------------------------------------------------------------
//static BOOL CheckForBrokenDriver(LPDIRECTDRAWSURFACE3 pSurface)
static BOOL CheckForBrokenDriver(LPDIRECTDRAWSURFACE2 pSurface)
    {
    HDC         hdcSurface = NULL, hdcScreen = NULL;
    COLORREF    rgbTest = 0, rgbScreen = 0, rgbSurface = 0;

    //
    //  to test for bad driver case, put a pure green pixel in the upper left corner
    //  of the surface, then get it again (getting rid of the pixel so it doesn't display)
    //  
    //  if the pixel comes back with red or blue in it, then the test failed
    //  and we have a bad driver to content with (so return TRUE)
    //
    if (pSurface->GetDC(&hdcSurface) == DD_OK)
        {
        hdcScreen = GetDC(NULL);

        //
        //  get the pixel
        //
        rgbScreen = GetPixel(hdcScreen, 0, 0);
        rgbSurface= GetPixel(hdcSurface, 0, 0);

        //
        //  set the pixel to green and blt it to the screen
        //
        SetPixel(hdcScreen, 0, 0, RGB(0, 255, 0));
        BitBlt(hdcSurface, 0, 0, 1, 1, hdcScreen, 0, 0, SRCCOPY);

        //
        //  get the pixel from the screen
        //
        rgbTest = GetPixel(hdcSurface, 0, 0);

        //
        //  put the original pixel back
        //
        SetPixel(hdcScreen, 0, 0, rgbScreen);
        SetPixel(hdcSurface, 0, 0, rgbSurface);
        ReleaseDC(NULL, hdcScreen);
        pSurface->ReleaseDC(hdcSurface);
        }

    //
    //  test here to see if it there's any red or blue in the pixel
    //
    return (GetRValue(rgbTest) != 0 || GetBValue(rgbTest) != 0);
    }

//---------------------------------------------------------------------------------------------
//  GetOptimalDIBFormat() gets the optimal DIB format for a display device. The optimal DIB 
//  format is the format that exactly matches the format of the device, this is very important 
//  when dealing with 16bpp modes, you need to know what bitfields to use (555 or 565 for 
//  example).
//
//  You normally use this function to get the best format to pass to CreateDIBSection().
//
//  If you are going to use this function on a 8bpp device, you should make sure the 
//  colortable contains an identity palette for optimal blt'ing
//
//  requires:       device to get format for, pointer to bitmapinfo + color table
//  returns:        none
//  side effect:    optimal DIB format in pbi
//                  if <= 8bpp, color table will contain system palette
//                  if >= 16bpp, the "table" will contain corrected BI_BITFIELDS
//---------------------------------------------------------------------------------------------
static void GetOptimalDIBFormat(HDC hdc, BITMAPINFOHEADER *pbi)
    {
    HBITMAP hbm;

    hbm = CreateCompatibleBitmap(hdc, 1, 1);
    ZeroMemory(pbi, sizeof(BITMAPINFOHEADER));
    pbi->biSize = sizeof(BITMAPINFOHEADER);
    pbi->biBitCount = 0;

    //
    // first call will fill in the optimal biBitCount
    //
    GetDIBits(hdc, hbm, 0, 1, NULL, (BITMAPINFO*)pbi, DIB_RGB_COLORS);

    //
    // second call will get the optimal color table, or the optimal bitfields
    //
    GetDIBits(hdc, hbm, 0, 1, NULL, (BITMAPINFO*)pbi, DIB_RGB_COLORS);
    DeleteObject(hbm);
    }
