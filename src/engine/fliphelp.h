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

//#include "common.h"

#ifdef __cplusplus
	extern "C" {
#endif

// flags for FlipHelp initialization
#define FH_DRAW_MOUSE       0x0001  // FlipHelp draws mouse
#define FH_3DFX_HARDWARE    0x0002  // FlipHelp uses 3Dfx hardware
#define FH_BITMAP_UPDATE    0x0004  // FlipHelp always does a bitmap update

// error codes (for HRESULTs)
#define HR_ERR              0x0001  // general failure

// macros
#define RELEASE(x) if (x) { x->Release(); x = NULL; }

// declare globals
extern SIZE    structSurfaceSize; // DirectDraw front buffer dimensions

/*
#define DllImport   __declspec( dllimport )




DllImport HWND WINAPI CallmyHelp(HWND, HINSTANCE, BOOL, UINT);  //import call into Help DLL
*/


// exported function prototypes in FlipHelp.cpp
HRESULT     FlipHelp_Init(HWND hwndApp, LPVOID FrontBuffer, LPVOID BackBuffer, DWORD dwFlags);
void        FlipHelp_Release();
HWND		FlipHelp_Begin(HWND hwnd,BOOL StaticContent);
void		FlipHelp_End(void);
HRESULT     FlipHelp_Update(void);
BOOL		FlipHelp_IsActive(void);
BOOL		FlipHelp_IsStatic(void);

// exported function prototypes in FlipHelpES.cpp
BOOL        FlipHelp_WOOP(HWND hwndMain, LPSTR szCommandLine, LPSTR szClassName);
BOOL        FlipHelp_WinHelp(HWND hwndMain, LPCSTR szHelpFile, UINT uCmd, DWORD ulData);

#ifdef __cplusplus
	}
#endif
