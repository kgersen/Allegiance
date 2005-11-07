/***********************************************************************

    SRVDBG.CPP


    Server Debug Support

    Copyright (C) 1995 Microsoft Corporation
    All rights reserved.

    Created on: 05/25/95

************************************************************************/
//#include "stdafx.h"
#include "pch.h"

static HANDLE g_hEventLog = NULL;
BOOL          g_fWantInt3 = TRUE;

char          g_szDbgModPath[MAX_PATH] = "<Path unknown>";



/***********************************************************************

    FUNCTION: SRVDBG_Init

    Initialize Server Debugging Support

    If NULL is passed for the path, this routine will attempt to
    determine the path through GetModuleFileName, which may not be
    accurate for DLLs, depending on the calling context of the call
    to this routine (won't work for DLLs unless called from ProcessAttach
    notification).

    Created on: 05/26/95

************************************************************************/

BOOL SRVDBG_Init(LPSTR lpszSource, LPSTR lpszPath, BOOL fWantInt3)
{
    g_fWantInt3 = fWantInt3;

    if ( !lpszSource )
        return (FALSE);

    if (!lpszPath)
        GetModuleFileName(NULL, g_szDbgModPath, sizeof(g_szDbgModPath));
    else
        lstrcpy(g_szDbgModPath, lpszPath);
    
    Assert(!g_hEventLog);
    g_hEventLog = RegisterEventSource(NULL, lpszSource );

    return (g_hEventLog != NULL);
}


/***********************************************************************

    FUNCTION: SRVDBG_Terminate

    Terminate Server Debugging Support

    Created on: 05/26/95

************************************************************************/

BOOL SRVDBG_Terminate( void )
{
    if ( !g_hEventLog )
    return ( FALSE );

    BOOL    fDeregisterEventSource =  DeregisterEventSource( g_hEventLog );
    Assert(fDeregisterEventSource);
    g_hEventLog = NULL;
    return fDeregisterEventSource;
}


/***********************************************************************

    FUNCTION: SRVDBG_ClearLog

    Clear debug event log.  SRVDBG_Init must be called first.

    Created on: 05/26/95

************************************************************************/

BOOL SRVDBG_ClearLog( void )
{
    if ( !g_hEventLog )
    return ( FALSE );

    return ( ClearEventLog( g_hEventLog, NULL ) );
}

/***********************************************************************

    FUNCTION: SRVDBG_Error

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before. 

************************************************************************/

void SRVDBG_Error( LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_ERROR_TYPE, SRVDBG_ERROR, lpsz, NULL, NULL );
}

/***********************************************************************

    FUNCTION: SRVDBG_Error2

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before. 

************************************************************************/

void SRVDBG_Error2( LPCSTR lpsz, LPCSTR lpsz2)
{
    SRVDBG_ReportEvent( SRVDBG_ERROR_TYPE, SRVDBG_ERROR, lpsz, lpsz2, NULL );
}

/***********************************************************************

    FUNCTION: SRVDBG_Error3

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Error3( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3)
{
    SRVDBG_ReportEvent( SRVDBG_ERROR_TYPE, SRVDBG_ERROR, lpsz, lpsz2, lpsz3 );
}

/***********************************************************************

    FUNCTION: SRVDBG_Info

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Info( LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_INFO_TYPE, SRVDBG_INFO, lpsz, NULL, NULL );
}

/***********************************************************************

    FUNCTION: SRVDBG_Info2

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Info2( LPCSTR lpsz, LPCSTR lpsz2 )
{
    SRVDBG_ReportEvent( SRVDBG_INFO_TYPE, SRVDBG_INFO, lpsz, lpsz2, NULL );
}

/***********************************************************************

    FUNCTION: SRVDBG_Info3

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Info3( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3 )
{
    SRVDBG_ReportEvent( SRVDBG_INFO_TYPE, SRVDBG_INFO, lpsz, lpsz2, lpsz3 );
}

/***********************************************************************

    FUNCTION: SRVDBG_Warning

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Warning( LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_WARNING_TYPE, SRVDBG_WARNING, lpsz, NULL, NULL );
}

/***********************************************************************

    FUNCTION: SRVDBG_Warning2

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Warning2( LPCSTR lpsz, LPCSTR lpsz2 )
{
    SRVDBG_ReportEvent( SRVDBG_WARNING_TYPE, SRVDBG_WARNING, lpsz, lpsz2, NULL );
}

/***********************************************************************

    FUNCTION: SRVDBG_Warning3

    Definition: Log error event  SRVDBG_Init must be called first.

    Created on: 12/09/97

    Comment: This was a macro before.

************************************************************************/

void SRVDBG_Warning3( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3 )
{
    SRVDBG_ReportEvent( SRVDBG_WARNING_TYPE, SRVDBG_WARNING, lpsz, lpsz2, lpsz3 );
}

void SRVDBG_ErrorEvt( DWORD IDEvent, LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_ERROR_TYPE, IDEvent, lpsz, NULL, NULL );
}

void SRVDBG_WarningEvt( DWORD IDEvent, LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_WARNING_TYPE, IDEvent, lpsz, NULL, NULL );
}

void SRVDBG_InfoEvt( DWORD IDEvent, LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_INFO_TYPE, IDEvent, lpsz, NULL, NULL );
}

void SRVDBG_Assert( LPCSTR lpsz )
{
    SRVDBG_ErrorEvt( SRVDBG_ASSERT, lpsz );
}

/***********************************************************************

    FUNCTION: SBRDBG_ReportEvent

    Report error, warning, or information. SRVDBG_Init must be called first.

    History
        Created on: 05/26/95
        06/05/96 cmason Added special support for getting Assert's logged 
                        into the QScript Thread Local Storage trave buffer
                        so that on Aborts and Exceptions, the Assert will
                        show up in the dump of the trace buffer.

************************************************************************/

BOOL SRVDBG_ReportEvent(
    WORD                        fwEventType,
    DWORD                       IDEvent,
    LPCSTR                      lpsz,
    LPCSTR                      lpsz2,
    LPCSTR                      lpsz3)
{
    LPCSTR                      plpszT[5];
    WORD                        cStrings;
    BOOL                        fSuccess;       

    cStrings = 5;
    plpszT[0] = g_szDbgModPath;
    plpszT[1] = ""; // was product id
    plpszT[2] = lpsz ? lpsz : "";
    plpszT[3] = lpsz2 ? lpsz2 : "";
    plpszT[4] = lpsz3 ? lpsz3 : "";


    // go ahead and send the event to anyone watching debug output
    for (int i=0; i<cStrings; i++)
    {
        if (*plpszT[i])
        {
            if( SRVDBG_ASSERT == IDEvent )
            {
                TraceN1( "%s ", plpszT[i] );
            }
            else
            {
                ODS(plpszT[i]);
                ODS(" ");
            }
        }
    }
    if( SRVDBG_ASSERT == IDEvent )
    {
        Trace0( "\r\n" );
    }
    else
    {
        ODS("\r\n");
    }

    switch ( fwEventType )
    {
    case SRVDBG_INFO_TYPE:
    case SRVDBG_WARNING_TYPE:
        break;

    case SRVDBG_ERROR_TYPE:
        if (g_fWantInt3)
        {
            //
            // 06/13/96 cmason Will only break into a debugger if one is 
            //  running.  If not running under a debugger, does nothing.
            //
            __try
            {
               DebugBreak();
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
            }
        }
        //ExitProcess(1);
        break;
    }

    //
    // 05/29/96 cmason Moved check to bottom of function so that Asserts still work even it
    //  SRVDBG could not init the event log.
    // 09/08/96 garrettm If no event source, we try to create one on the fly...
    //
    if ( !g_hEventLog )
    {
        HANDLE hLog = RegisterEventSource(NULL, "UNKNOWN");

        if (hLog)
        {
            ReportEvent( hLog, fwEventType, 0, IDEvent, NULL, cStrings, 0, plpszT, NULL );
            DeregisterEventSource( hLog );
        }

        return ( FALSE );
    }

    fSuccess = ReportEvent( g_hEventLog, fwEventType, 0, IDEvent, NULL,
                            cStrings, 0, plpszT, NULL );

    return ( fSuccess );
}

void SRVDBG_ErrorBinary(LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3, DWORD dwDataSize, LPVOID lpRawData )
{
    SRVDBG_ReportEventBinary(SRVDBG_ERROR_TYPE, SRVDBG_ERROR, lpsz, lpsz2, lpsz3, dwDataSize, (LPVOID)lpRawData);
}

void SRVDBG_WarningBinary( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3, DWORD dwDataSize, LPVOID lpRawData )
{
    SRVDBG_ReportEventBinary( SRVDBG_WARNING_TYPE, SRVDBG_WARNING, lpsz, lpsz2, lpsz3, dwDataSize, (LPVOID)lpRawData );
}

void SRVDBG_InfoBinary( LPCSTR lpsz, LPCSTR lpsz2, LPCSTR lpsz3, DWORD dwDataSize, LPVOID lpRawData )
{
    SRVDBG_ReportEventBinary( SRVDBG_INFO_TYPE, SRVDBG_INFO, lpsz, lpsz2, lpsz3, dwDataSize, (LPVOID)lpRawData );
}

void SRVDBG_RebootMachine()
{
    SRVDBG_ReportEvent( SRVDBG_ERROR_TYPE, SRVDBG_ERROR, "Reboot Machine", NULL, NULL );
}

void SRVDBG_RestartService( LPCSTR lpsz )
{
    SRVDBG_ReportEvent( SRVDBG_ERROR_TYPE, SRVDBG_ERROR, "Restart Service", lpsz, NULL );
}

/***********************************************************************

    FUNCTION: SBRDBG_ReportEventBinary

    Report error, warning, or information in binary form. 
    
    SRVDBG_Init must be called first.

    History
        04/14/97 cmason Created to allow us to log larger pieces of information
                        than allowed in the Event Log with strings.

************************************************************************/
BOOL SRVDBG_ReportEventBinary
(
    WORD                        fwEventType,
    DWORD                       IDEvent,
    LPCSTR                      lpsz,
    LPCSTR                      lpsz2,
    LPCSTR                      lpsz3,
    DWORD                       dwDataSize,
    LPVOID                      lpRawData
)
{
    const WORD                  cStrings = 5;
    LPCSTR                      plpszT[cStrings];
    BOOL                        fSuccess;       

    plpszT[0] = g_szDbgModPath;
    plpszT[1] = "";  // was productid
    plpszT[2] = lpsz ? lpsz : "";
    plpszT[3] = lpsz2 ? lpsz2 : "";
    plpszT[4] = lpsz3 ? lpsz3 : "";


    // go ahead and send the event to anyone watching debug output
    for (int i=0; i < cStrings; i++)
    {
        if (*plpszT[i])
        {
            if( SRVDBG_ASSERT == IDEvent )
            {
                TraceN1( "%s ", plpszT[i] );
            }
            else
            {
                ODS(plpszT[i]);
                ODS(" ");
            }
        }
    }
    if( SRVDBG_ASSERT == IDEvent )
    {
        Trace0( "\r\n" );
    }
    else
    {
        ODS("\r\n");
    }

    switch ( fwEventType )
    {
    case SRVDBG_INFO_TYPE:
    case SRVDBG_WARNING_TYPE:
        break;

    case SRVDBG_ERROR_TYPE:
        if (g_fWantInt3)
        {
            //
            // 06/13/96 cmason Will only break into a debugger if one is 
            //  running.  If not running under a debugger, does nothing.
            //
            __try
            {
                DebugBreak();
            }
            __except (EXCEPTION_EXECUTE_HANDLER)
            {
            }
        }
        break;
    }

    //
    // 05/29/96 cmason Moved check to bottom of function so that Asserts still work even it
    //  SRVDBG could not init the event log.
    // 09/08/96 garrettm If no event source, we try to create one on the fly...
    //
    if ( !g_hEventLog )
    {
        HANDLE hLog = RegisterEventSource(NULL, "UNKNOWN");

        if (hLog)
        {
            ReportEvent( hLog, fwEventType, 0, IDEvent, NULL, cStrings, dwDataSize, plpszT, lpRawData );
            DeregisterEventSource( hLog );
        }

        return ( FALSE );
    }

    fSuccess = ReportEvent( g_hEventLog, fwEventType, 0, IDEvent, NULL,
                            cStrings, dwDataSize, plpszT, lpRawData );

    return ( fSuccess );
}


/***********************************************************************

***********************************************************************/
void SRVDBG_DoAssert(
    LPCSTR                      szFile,
    int                         iLine,
    LPCSTR                      szAssert )
{
    char                        szBuf[ 2048 ];

    wsprintf( szBuf, "File: %s at line %d\r\n%s", szFile, iLine, szAssert );
    SRVDBG_Assert( szBuf );
}
    
/***********************************************************************

    FUNCTION: SRVDBG_LogSystemID

    Place an event of some sort (by message ID) into the NT event log.

    Created 29-Mar-96 LeoN

************************************************************************/
BOOL SRVDBG_LogSystemID (
WORD type,                                              // error, warning, info
DWORD messageID,                                // id of message in module
LPCSTR g_module,                                // module containing the message
...
) {
    va_list args;
    HMODULE hMod;
    char *  psz;
   
    hMod = GetModuleHandle (g_module);
    if (hMod)
    {
        DWORD id;

        switch (type)
            {
            case 0:
                return TRUE;
            case SRVDBG_INFO_TYPE:
                id = SRVDBG_INFO;
                break;
            case SRVDBG_WARNING_TYPE:
                id = SRVDBG_WARNING;
                break;
            default:
               case SRVDBG_ERROR_TYPE:
                id = SRVDBG_ERROR;
                break;
            }

        va_start(args, g_module);
        if (0 == FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                                  | FORMAT_MESSAGE_FROM_HMODULE
                                  | FORMAT_MESSAGE_FROM_SYSTEM,
                                  hMod, messageID, 0,
                                  (char *)(&psz), 16, &args))
        {
            char sz[200];
            wsprintf(sz, "Couldn't find error for id %d", messageID);

            SRVDBG_ReportEvent (SRVDBG_ERROR_TYPE, id, sz, g_module, NULL);
        }
        else
        {
               SRVDBG_ReportEvent (type, id, psz, NULL, NULL);
            LocalFree(psz);
        }
        va_end(args);
      }
    else
    {
          SRVDBG_ReportEvent (SRVDBG_ERROR_TYPE, SRVDBG_ERROR, "Failure Logging Message", g_module, NULL);
    }

    return TRUE;
    }


void LogSystemSZ(WORD type, LPCSTR szMessage )
{
    SRVDBG_LogSystemSZ((WORD)type, szMessage);
}


/***********************************************************************

    FUNCTION: SRVDBG_LogSystemSZ

    Place an event of some sort (by string) into the NT event log.

    Created 29-Mar-96 LeoN

************************************************************************/
BOOL SRVDBG_LogSystemSZ (
WORD type,                                              // error, warning, info
LPCSTR szMessage
) {
    DWORD id;

    switch (type)
    {
    case 0:
        return TRUE;

    case SRVDBG_INFO_TYPE:
        id = SRVDBG_INFO;
        break;
    case SRVDBG_WARNING_TYPE:
        id = SRVDBG_WARNING;
        break;
    default:
    case SRVDBG_ERROR_TYPE:
        id = SRVDBG_ERROR;
        break;
    }
    return SRVDBG_ReportEvent (type, id, szMessage, NULL, NULL);
  }

extern "C" void DoAssert(
    LPCSTR                      szFile,
    int                         iLine,
    LPCSTR                      szAssert )
{
    char                        szBuf[ 2048 ];

    wsprintf( szBuf, "File: %s at line %d\r\n%s", szFile, iLine, szAssert );
    SRVDBG_Assert( szBuf );
}

#ifdef DEBUG

// Force Debug initialization early
// #pragma init_seg(compiler)


DebugInfo g_dbi = {
    true,        // Show trace
    -1,            // No TLS trace buffer
    NULL,        // No special trace function
    true,        // Show asserts
    0,            // Assert count
    false,        // Assert on alloc failure
    false,        // Stop on Assert
};


/*---------------------------------------------------------------------------
    PrivateDbgTraceV
        Trace string with va_list arguments.  Handle indenting.
    Author: kennt
 ---------------------------------------------------------------------------*/
static void PrivateDbgTraceV(LPCSTR szFormat, va_list pvargs)
{
    char szBuffer[2048+16];
    char *psz = szBuffer;
#ifdef CORP_BUILD
    psz += wsprintf( psz, "tid(%x): ", GetCurrentThreadId());
#endif

    wvsprintf(psz, szFormat, pvargs);
    OutputTrace(szBuffer);
}

/*!--------------------------------------------------------------------------
    DbgTraceV
        Trace string with va_list arguments.  Handle indenting.
    Author: GaryBu
 ---------------------------------------------------------------------------*/
DBG_API(void) DbgTraceV(LPCSTR szFormat, va_list pvargs)
{
    PrivateDbgTraceV(szFormat, pvargs);
}

/*!--------------------------------------------------------------------------
    DbgTrace
        Trace string with args.
    Author: suryanr
 ---------------------------------------------------------------------------*/
DBG_APIV(void) DbgTrace(LPSTR szFormat, ...)
{
    va_list args;

    if (g_dbi.fDoTrace)
        {
        va_start(args, szFormat);
        DbgTraceV(szFormat, args);
        va_end(args);
        }
}


/*---------------------------------------------------------------------------
    OutputTrace
        OutputDebugString + any local handling
    Author: MarkGo
 ---------------------------------------------------------------------------*/
DBG_API(void) OutputTrace(LPCTSTR lpBuffer)
{
    DbgTraceBuf    *ptb;
    
    if (g_dbi.pfnTrace)
        g_dbi.pfnTrace(lpBuffer);
    else
        ODS(lpBuffer);

    if (g_dbi.itlsTraceRGB != (DWORD) -1
        && (ptb = (DbgTraceBuf *)TlsGetValue(g_dbi.itlsTraceRGB))!=NULL )
        {
        DWORD    cbOut = lstrlen(lpBuffer)+1;
        // Buffer the trace output

        // Is output larger than buf?
        if (cbOut >= ptb->cchBuf)
            {
            // Copy *last* cchBuf chars
            memcpy(ptb->rgch, lpBuffer + (cbOut - ptb->cchBuf), ptb->cchBuf);
            ptb->ichStart = 0;
            ptb->ichEnd = ptb->cchBuf-1;
            }
        else
            {
            // Is output larger than remaining contig space?
            if (cbOut > ptb->cchBuf - ptb->ichEnd)
                {
                memcpy(&(ptb->rgch[ptb->ichEnd]), lpBuffer,
                       ptb->cchBuf - ptb->ichEnd);
                memcpy(ptb->rgch, lpBuffer+(ptb->cchBuf-ptb->ichEnd),
                       cbOut-(ptb->cchBuf-ptb->ichEnd));
                ptb->ichStart = cbOut-(ptb->cchBuf-ptb->ichEnd);
                ptb->ichEnd = ptb->ichStart-1;
                }
            else
                {
                memcpy(&ptb->rgch[ptb->ichEnd], lpBuffer, cbOut);
                if (ptb->ichStart > ptb->ichEnd)
                    {
                    ptb->ichEnd += cbOut-1;
                    ptb->ichStart = ptb->ichEnd+1;
                    }
                else
                    ptb->ichEnd += cbOut-1;
                }
            
            }
        }
}

#ifndef FDbgFalse
/*!--------------------------------------------------------------------------
    FDbgFalse
        Returns false, but insures that debug code page is in memory.
        Also makes a handy breakpoint.

    Author: MarkGo
 ---------------------------------------------------------------------------*/
DBG_API(BOOL)    FDbgFalse()
{
    return false;
}
#endif

#ifdef __AFX_H__
/*!--------------------------------------------------------------------------
    AfxAssertFailedLine
        Overrides Afx's assertion handling
    Author: GaryBu, kennt, MarkGo
 ---------------------------------------------------------------------------*/
BOOL AFXAPI AfxAssertFailedLine(LPCSTR lpszFileName, int nLine)
{
    DbgAssert(lpszFileName, nLine, "AFX Assert");
    return false;
}

#endif

/*!--------------------------------------------------------------------------
    DbgAssert
        Display assert dialog.
    Author: GaryBu, kennt, MarkGo
 ---------------------------------------------------------------------------*/
DBG_APIV(void) DbgAssert(LPCSTR szFile, int iLine, LPCSTR szFmt, ...)
{
    va_list    arg;
    char sz[MAX_PATH*2];
    char *pch = sz;

    va_start(arg, szFmt);
    pch += wvsprintf(sz, szFmt, arg);
    va_end(arg);
    // Remove trailing newlines...
    while (*(pch-1) == '\n' || *(pch-1) == '\r')
        --pch;

    *pch = 0;
    if (g_dbi.fShowAsserts)
    {
        //
        // 03/27/96 cmason Added doing trace output as well as the Event Viewer 
        //    entry for asserts.
        // 04/04/96 cmason Removed redundancy of tracing out the assert twice, once
        //  here and agian inside the SRVDBG code called by DoAssert.  The code here 
        //  was first, but the the SRVDBG code traces out more events than just asserts.
        //
        DbgTrace( "Assert failed: ");

        //
        // This loggs to the Event Viewer
        //
        DoAssert(szFile, iLine, sz);

        //
        // 03/27/96 cmason Added breaking into the debugger if requested.
        //
        if( g_dbi.fStopOnAssert )
        {
            //
            // This does a DebugBreak() and is protected by byt __try/__except so
            //    that nothing happens if a debugger isn't running.
            //
            DbgStop();
        }
    }
    else
        DbgTrace(
    "Skipped Assert (ShowAsserts is FALSE):\r\n%s (file %s, line %d)\r\n",
                 sz, szFile, iLine);
}

/*!--------------------------------------------------------------------------
    DbgStop -- causes a break into the debugger if the debugger is running.
    -
    Author: afx
 ---------------------------------------------------------------------------*/
#pragma optimize("qgel", off) // assembler cannot be globally optimized
DBG_API(void) DbgStop()
{
    __try
    {
        DebugBreak();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
}

DBG_API(DWORD)    DbgSetThreadTraceBuffer(DWORD itlsTrace)
{
    DWORD itlsPrev = g_dbi.itlsTraceRGB;

    g_dbi.itlsTraceRGB = itlsTrace;

    return itlsPrev;
}


DBG_API(const char *)    DbgGetTraceBuffer(BOOL fSecondChunk)
{
    DbgTraceBuf    *ptb;
    
    if (g_dbi.itlsTraceRGB == (DWORD)-1)
        return fSecondChunk ? "" : "<No trace information available>\r\n";
            
    if (NULL==(ptb = (DbgTraceBuf *)TlsGetValue(g_dbi.itlsTraceRGB)))
        return fSecondChunk ? "" : "<No trace buffer set>\r\n";

    if (!fSecondChunk)
        {
        // Return initial chunk, regardless of wraparound.
        return &(ptb->rgch[ptb->ichStart]);
        }
    
    // We only have a second chunk in wraparound
    if (ptb->ichStart > ptb->ichEnd)
        return ptb->rgch;

    return "";
}


DBG_API(BOOL)    DbgSetTraceBuffer(void *rgbTrace, int cbTrace)
{
    DbgTraceBuf    *ptb;
    
    if (g_dbi.itlsTraceRGB == (DWORD) -1
        || (rgbTrace && cbTrace <= sizeof(DbgTraceBuf)))
        return false;

    ptb = (DbgTraceBuf *)rgbTrace;
    if (ptb)
        {
        ptb->cchBuf = cbTrace - sizeof(DbgTraceBuf);
        ptb->ichStart = ptb->ichEnd = 0;
        ptb->rgch[ptb->ichEnd] = '\0';
        ptb->rgch[ptb->cchBuf] = '\0';
        }
    
    TlsSetValue(g_dbi.itlsTraceRGB, ptb);
    return true;
}


#pragma optimize("", on)

#endif //DEBUG

/***********************************************************************

    FUNCTION: LoadMsg

    Loads a string from the message table and returns it in an allocated
    string buffer.    

    History
      Created on: 06/01/95
      04/29/96 cmason   Trav#336: Added FORMAT_MESSAGE_MAX_WIDTH_MASK
                        so that CRLF's must be explicitly specified.

************************************************************************/

char *LoadMsg(UINT idMsg,                   
        HMODULE hmod)    // May be null, in which case app's msg table is used
{
    void *pv;
    char *pszRet;
    DWORD cch;

    cch = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER
                  | FORMAT_MESSAGE_IGNORE_INSERTS
                  | FORMAT_MESSAGE_FROM_HMODULE
                  | FORMAT_MESSAGE_FROM_SYSTEM
                  | FORMAT_MESSAGE_MAX_WIDTH_MASK,
                  hmod, idMsg, 0 /*Default language*/,
                  (char *)(&pv), 16, NULL);
    if ( !cch )
        return NULL;

    //
    // 04/30/96 cmason #336: Removing the trailing space.
    //
    Assert( ' ' == ((char *)pv)[cch - 1]);
    ((char *)pv)[cch - 1] = '\0';

    pszRet = new char[cch];
    memcpy(pszRet, pv, cch);

    LocalFree(pv);
    return pszRet;
}

