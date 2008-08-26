;//////////////////////////////////////////////////////////////////////////////
;//
;// SENTMSG.MSG - error constants and error strings for SENTINAL
;//
;// Copyright (C) 1998 Microsoft Corporation
;// All Rights Reserved.
;//

LanguageNames=(English=0x409:SENTINAL)

LanguageNames=(English=0x409:FEDMSG)
MessageId=0
SymbolicName=SRVDBG_ASSERT
Language=English
Sentinal Assert from %1:%n%n%2%n%3%n%n%4%5
.
MessageId=1
SymbolicName=SRVDBG_EXCEPTION
Language=English
Sentinal Exception from %1:%n%n%2%n%n%3%4
.
MessageId=2
SymbolicName=SRVDBG_ABORT
Language=English
Sentinal Abort from %1:%n%n%2%n%n%3%4
.
MessageId=3
SymbolicName=SRVDBG_NOTIFYABORT
Language=English
Sentinal NotifyAbort from %1:%n%n%2%n%n%3%4
.
MessageId=1000
SymbolicName=SRVDBG_ERROR
Language=English
Sentinal Error from %1:%n%n%2%n%3%n%n%4%5
.
MessageId=2000
SymbolicName=SRVDBG_WARNING
Language=English
Sentinal Warning from %1:%n%n%2%n%3%n%n%4%5
.
MessageId=3000
SymbolicName=SRVDBG_INFO
Language=English
Sentinal Information from %1:%n%n%2%n%3%n%n%4%5
.
MessageId=4000
Severity=Error
SymbolicName=SERVICE_ERROR
Language=English
%1%n%n%2%n%3
.

MessageId=4001
Severity=Error
SymbolicName=SERVICE_DOWN
Language=English
%1%n%n%2%n%3
.

MessageId=4002
Severity=Error
SymbolicName=SERVICE_RESTART
Language=English
%1%n%n%2%n%3
.

MessageId=4003
Severity=Warning
SymbolicName=SERVICE_WARNING
Language=English
%1%n%n%2%n%3
.

MessageId=4004
Severity=Informational
SymbolicName=SERVICE_INFO
Language=English
%1%n%n%2%n%3
.

MessageId=9001
Severity=Error
SymbolicName=SENTINAL_INTERNETOPENURL
Language=English
Sentinal Information from %1:%n%2%nInternetOpenUrl failed with %3
.

MessageId=9002
Severity=Error
SymbolicName=SENTINAL_INTERNETGETLASTRESPONSEINFO
Language=English
Sentinal Information from %1:%n%2%nInternetGetLastResponseInfo failed with %3
.

MessageId=9003
Severity=Error
SymbolicName=SENTINAL_GENERAL_FAILURE
Language=English
Sentinal Information from %1:%n%nError: %2
.

MessageId=9004
Severity=Error
SymbolicName=SENTINAL_MONITORED_SERVER_DOWN
Language=English
Sentinal Information from %1:%n%nMonitored Server Failure%nServer: %3%nError: %4
.

MessageId=10035
Language=English
WSAEWOULDBLOCK
.

