/***********************************************************************

	SRVDBG.H


	Server Debug Support

	Copyright (C) 1995 Microsoft Corporation
	All rights reserved.

	Created on: 05/25/95

************************************************************************/

#ifdef _SRVDBG_H
#error srvdbg.h included twice
#endif
#define _SRVDBG_H

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#define STRICT	1
#include <windows.h>
#else
#ifndef STRICT
#error STRICT must be defined (#define STRICT 1 recommended) before including windows.h
#endif
#endif

#ifndef BASED_CODE
#define BASED_CODE
#endif

//
// 4/15/97 A static LIB should not be exporting functions from a DLL that links in that LIB.
//
//#define _SRVDBG_EXPORT __declspec(dllexport) 
#define _SRVDBG_EXPORT 

//#include "utilmsg.h"
#include "sentmsg.h"
//#include "resource.h"  // all messages must be defined in string table resource

#if defined(DEBUG) && defined(INLINE)
#undef THIS_FILE
static char BASED_CODE SRVDBG_H[] = "srvdbg.h";
#define THIS_FILE SRVDBG_H
#endif

#define SRVDBG_ABORT_EXCEPTION  1
#define SRVDBG_SILENTABORT_EXCEPTION 2
#define SRVDBG_NOTIFYABORT_EXCEPTION 3
#define SRVDBG_HACKABORT_EXCEPTION 4

#define SRVDBG_ERROR_TYPE       EVENTLOG_ERROR_TYPE
#define SRVDBG_WARNING_TYPE     EVENTLOG_WARNING_TYPE
#define SRVDBG_INFO_TYPE        EVENTLOG_INFORMATION_TYPE

#ifdef __cplusplus
#define SRVDBG_DEFPATH = NULL
#define SRVDBG_DEFNULLSTR = ""
#else
#define SRVDBG_DEFPATH
#define SRVDBG_DEFNULLSTR
#endif

_SRVDBG_EXPORT BOOL SRVDBG_Init( LPSTR lpszSource, LPSTR lpszPath SRVDBG_DEFPATH);
_SRVDBG_EXPORT BOOL SRVDBG_Terminate( void );
_SRVDBG_EXPORT BOOL SRVDBG_ClearLog( void );

_SRVDBG_EXPORT void	SRVDBG_Error( LPCSTR lpsz SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_Error2( LPCSTR lpsz, LPCSTR lpsz2 SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_Error3( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3 SRVDBG_DEFNULLSTR);

_SRVDBG_EXPORT void	SRVDBG_Info( LPCSTR lpsz SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_Info2( LPCSTR lpsz, LPCSTR lpsz2 SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_Info3( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3 SRVDBG_DEFNULLSTR);

_SRVDBG_EXPORT void	SRVDBG_Warning( LPCSTR lpsz SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_Warning2( LPCSTR lpsz, LPCSTR lpsz2 SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_Warning3( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3 SRVDBG_DEFNULLSTR);

_SRVDBG_EXPORT void	SRVDBG_ErrorBinary( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3, DWORD dwDataSize, LPVOID lpRawData SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_WarningBinary( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3, DWORD dwDataSize, LPVOID lpRawData SRVDBG_DEFNULLSTR );
_SRVDBG_EXPORT void	SRVDBG_InfoBinary( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3, DWORD dwDataSize, LPVOID lpRawData SRVDBG_DEFNULLSTR );

_SRVDBG_EXPORT void	SRVDBG_ErrorEvt( DWORD IDEvent, LPCSTR lpsz SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_WarningEvt( DWORD IDEvent, LPCSTR lpsz SRVDBG_DEFNULLSTR);
_SRVDBG_EXPORT void	SRVDBG_InfoEvt( DWORD IDEvent, LPCSTR lpsz SRVDBG_DEFNULLSTR);

_SRVDBG_EXPORT void	SRVDBG_RebootMachine( );
_SRVDBG_EXPORT void	SRVDBG_RestartService( LPCSTR lpsz SRVDBG_DEFNULLSTR );

_SRVDBG_EXPORT void	SRVDBG_Assert( LPCSTR lpsz SRVDBG_DEFNULLSTR );

_SRVDBG_EXPORT BOOL SRVDBG_ReportEvent( WORD fwEventType, DWORD IDEvent,
                                        LPCSTR lpsz SRVDBG_DEFNULLSTR,
                                        LPCSTR lpsz2 SRVDBG_DEFNULLSTR,
                                        LPCSTR lpsz3 SRVDBG_DEFNULLSTR);

_SRVDBG_EXPORT BOOL SRVDBG_ReportEventBinary
(
    WORD                        fwEventType,
    DWORD                       IDEvent,
    LPCSTR                      lpsz,
    LPCSTR                      lpsz2,
    LPCSTR                      lpsz3,
    DWORD                       dwDataSize,
    LPVOID                      lpRawData
);

_SRVDBG_EXPORT BOOL SRVDBG_LogSystemID (WORD type,DWORD messageID, LPCSTR g_module, ...); 

_SRVDBG_EXPORT void	LogSystemSZ(WORD type, LPCSTR szMessage SRVDBG_DEFNULLSTR);

_SRVDBG_EXPORT BOOL SRVDBG_LogSystemSZ (WORD type,LPCSTR szMessage SRVDBG_DEFNULLSTR);

#define LoggingAssert( f, lpsz ) if (!(f)) SRVDBG_Assert( lpsz );

// LogSysytemXX support
//
typedef enum {
	LogNoOp         = 0,                                                    // Does nothing.
	LogInformation= SRVDBG_INFO_TYPE,                       // informational...might not be read right away
	LogError = SRVDBG_WARNING_TYPE,                         // error...noted & acted upon eventually
	LogAttentionRequired = SRVDBG_ERROR_TYPE        // bad error...requires attention (page operator)
	} LogType;

#define LogSystemID(type, messageID)                   SRVDBG_LogSystemID(-1,(WORD)type,(DWORD)messageID, g_module)
#define LogSystemID1(type, messageID,sz1)               SRVDBG_LogSystemID(-1,(WORD)type,(DWORD)messageID, g_module,sz1)
#define LogSystemID2(type, messageID,sz1,sz2)           SRVDBG_LogSystemID(-1,(WORD)type,(DWORD)messageID, g_module,sz1,sz2)
#define LogSystemID3(type, messageID,sz1,sz2,sz3)       SRVDBG_LogSystemID(-1,(WORD)type,(DWORD)messageID, g_module,sz1,sz2,sz3)
#define LogSystemID4(type, messageID,sz1,sz2,sz3,sz4)   SRVDBG_LogSystemID(-1,(WORD)type,(DWORD)messageID, g_module,sz1,sz2,sz3,sz4)
#define LogSystemID5(type, messageID,sz1,sz2,sz3,sz4,sz5) SRVDBG_LogSystemID(-1,(WORD)type,(DWORD)messageID, g_module,sz1,sz2,sz3,sz4,sz5)

extern char *g_module;

char *LoadMsg(UINT idMsg, HMODULE hmod);

void SRVDBG_DoAssert(
    LPCSTR                      szFile,
    int                         iLine,
    LPCSTR                      szAssert );
