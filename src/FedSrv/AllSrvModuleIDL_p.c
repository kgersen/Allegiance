

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Sun Apr 04 09:31:28 2004
 */
/* Compiler settings for .\AllSrvModuleIDL.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "AllSrvModuleIDL.h"

#define TYPE_FORMAT_STRING_SIZE   1503                              
#define PROC_FORMAT_STRING_SIZE   2185                              
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminSession_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminSession_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminSessionHost_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminSessionHost_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminSessionClass_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminSessionClass_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminSessionEvents_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminSessionEvents_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminShip_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminShip_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminGame_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminGame_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminGames_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminGames_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminServer_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminServer_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminUser_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminUser_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdminUsers_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IAdminUsers_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need a Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure put_SessionInfo */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* 8 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pSessionInfo */

/* 24 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	NdrFcShort( 0x2 ),	/* Type Offset=2 */

	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_SessionInfo */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppSessionInfo */

/* 60 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	NdrFcShort( 0x14 ),	/* Type Offset=20 */

	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_SessionInfos */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x8 ),	/* 8 */
/* 86 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppSessionInfos */

/* 96 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	NdrFcShort( 0x18 ),	/* Type Offset=24 */

	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Server */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x0 ),	/* 0 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppServer */

/* 132 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	NdrFcShort( 0x2e ),	/* Type Offset=46 */

	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ActivateEvents */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xb ),	/* 11 */
/* 152 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 154 */	NdrFcShort( 0x10 ),	/* 16 */
/* 156 */	NdrFcShort( 0x8 ),	/* 8 */
/* 158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter AGCEvent */

/* 168 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Parameter uniqueID */

/* 174 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 180 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 184 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeactivateEvents */

/* 186 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 188 */	NdrFcLong( 0x0 ),	/* 0 */
/* 192 */	NdrFcShort( 0xc ),	/* 12 */
/* 194 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 196 */	NdrFcShort( 0x10 ),	/* 16 */
/* 198 */	NdrFcShort( 0x8 ),	/* 8 */
/* 200 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 202 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 204 */	NdrFcShort( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter AGCEvent */

/* 210 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 212 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 214 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Parameter uniqueObjectID */

/* 216 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 218 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 220 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 222 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IsEventActivated */

/* 228 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 230 */	NdrFcLong( 0x0 ),	/* 0 */
/* 234 */	NdrFcShort( 0xd ),	/* 13 */
/* 236 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 238 */	NdrFcShort( 0x10 ),	/* 16 */
/* 240 */	NdrFcShort( 0x24 ),	/* 36 */
/* 242 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 244 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 246 */	NdrFcShort( 0x0 ),	/* 0 */
/* 248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 250 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter AGCEvent */

/* 252 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 254 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 256 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Parameter uniqueID */

/* 258 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 260 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 264 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 266 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 270 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 272 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 274 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ActivateAllEvents */

/* 276 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 278 */	NdrFcLong( 0x0 ),	/* 0 */
/* 282 */	NdrFcShort( 0xe ),	/* 14 */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	NdrFcShort( 0x0 ),	/* 0 */
/* 288 */	NdrFcShort( 0x8 ),	/* 8 */
/* 290 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 292 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 302 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure DeactivateAllEvents */

/* 306 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 308 */	NdrFcLong( 0x0 ),	/* 0 */
/* 312 */	NdrFcShort( 0xf ),	/* 15 */
/* 314 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	NdrFcShort( 0x8 ),	/* 8 */
/* 320 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 322 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 330 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 332 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 334 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PlayersOnline */


	/* Procedure get_ProcessID */

/* 336 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 338 */	NdrFcLong( 0x0 ),	/* 0 */
/* 342 */	NdrFcShort( 0x10 ),	/* 16 */
/* 344 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */
/* 348 */	NdrFcShort( 0x24 ),	/* 36 */
/* 350 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 352 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 354 */	NdrFcShort( 0x0 ),	/* 0 */
/* 356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 358 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pdwProcessID */

/* 360 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 362 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 364 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 366 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 368 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 370 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Version */

/* 372 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 374 */	NdrFcLong( 0x0 ),	/* 0 */
/* 378 */	NdrFcShort( 0x11 ),	/* 17 */
/* 380 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 384 */	NdrFcShort( 0x8 ),	/* 8 */
/* 386 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 388 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 390 */	NdrFcShort( 0x0 ),	/* 0 */
/* 392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 394 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppVersion */

/* 396 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 398 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 400 */	NdrFcShort( 0x48 ),	/* Type Offset=72 */

	/* Return value */

/* 402 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 404 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 406 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Stop */

/* 408 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 410 */	NdrFcLong( 0x0 ),	/* 0 */
/* 414 */	NdrFcShort( 0x12 ),	/* 18 */
/* 416 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */
/* 420 */	NdrFcShort( 0x8 ),	/* 8 */
/* 422 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 424 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 426 */	NdrFcShort( 0x0 ),	/* 0 */
/* 428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 430 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 432 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 434 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Pause */

/* 438 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 440 */	NdrFcLong( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x13 ),	/* 19 */
/* 446 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 448 */	NdrFcShort( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x8 ),	/* 8 */
/* 452 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 454 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 456 */	NdrFcShort( 0x0 ),	/* 0 */
/* 458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 460 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 462 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 464 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_WhoStartedServer */

/* 468 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 470 */	NdrFcLong( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x14 ),	/* 20 */
/* 476 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 480 */	NdrFcShort( 0x8 ),	/* 8 */
/* 482 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 484 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pIAdminSession */

/* 492 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 494 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 496 */	NdrFcShort( 0x5e ),	/* Type Offset=94 */

	/* Return value */

/* 498 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 500 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_EventLog */

/* 504 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0x15 ),	/* 21 */
/* 512 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x8 ),	/* 8 */
/* 518 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 520 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppEventLogger */

/* 528 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 530 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 532 */	NdrFcShort( 0x74 ),	/* Type Offset=116 */

	/* Return value */

/* 534 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 536 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PerfCounters */

/* 540 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 542 */	NdrFcLong( 0x0 ),	/* 0 */
/* 546 */	NdrFcShort( 0x16 ),	/* 22 */
/* 548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 554 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 556 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppPerfCounters */

/* 564 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 566 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 568 */	NdrFcShort( 0x8a ),	/* Type Offset=138 */

	/* Return value */

/* 570 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 572 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendAdminChat */

/* 576 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 578 */	NdrFcLong( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x17 ),	/* 23 */
/* 584 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 586 */	NdrFcShort( 0x18 ),	/* 24 */
/* 588 */	NdrFcShort( 0x8 ),	/* 8 */
/* 590 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 592 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 596 */	NdrFcShort( 0x1 ),	/* 1 */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrText */

/* 600 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 602 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 604 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Parameter nUserID */

/* 606 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 608 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 610 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter dateOriginal */

/* 612 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 614 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 616 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 618 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 620 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 622 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Continue */

/* 624 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 626 */	NdrFcLong( 0x0 ),	/* 0 */
/* 630 */	NdrFcShort( 0x18 ),	/* 24 */
/* 632 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 636 */	NdrFcShort( 0x8 ),	/* 8 */
/* 638 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 640 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 644 */	NdrFcShort( 0x0 ),	/* 0 */
/* 646 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 648 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 650 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 652 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetIdentity */

/* 654 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 656 */	NdrFcLong( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x3 ),	/* 3 */
/* 662 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 664 */	NdrFcShort( 0x8 ),	/* 8 */
/* 666 */	NdrFcShort( 0x8 ),	/* 8 */
/* 668 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 670 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 672 */	NdrFcShort( 0x0 ),	/* 0 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter nCookie */

/* 678 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 680 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppIdentity */

/* 684 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 686 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 688 */	NdrFcShort( 0xc4 ),	/* Type Offset=196 */

	/* Return value */

/* 690 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 692 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 694 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateSession */

/* 696 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 698 */	NdrFcLong( 0x0 ),	/* 0 */
/* 702 */	NdrFcShort( 0x3 ),	/* 3 */
/* 704 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 708 */	NdrFcShort( 0x8 ),	/* 8 */
/* 710 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 712 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 714 */	NdrFcShort( 0x0 ),	/* 0 */
/* 716 */	NdrFcShort( 0x0 ),	/* 0 */
/* 718 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pHost */

/* 720 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 722 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 724 */	NdrFcShort( 0xda ),	/* Type Offset=218 */

	/* Parameter ppSession */

/* 726 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 728 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 730 */	NdrFcShort( 0xec ),	/* Type Offset=236 */

	/* Return value */

/* 732 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 734 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 736 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OnEvent */

/* 738 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 740 */	NdrFcLong( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x3 ),	/* 3 */
/* 746 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x8 ),	/* 8 */
/* 752 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 754 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 756 */	NdrFcShort( 0x0 ),	/* 0 */
/* 758 */	NdrFcShort( 0x0 ),	/* 0 */
/* 760 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pEvent */

/* 762 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 764 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 766 */	NdrFcShort( 0x8e ),	/* Type Offset=142 */

	/* Return value */

/* 768 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 770 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 772 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_User */

/* 774 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 776 */	NdrFcLong( 0x0 ),	/* 0 */
/* 780 */	NdrFcShort( 0x2a ),	/* 42 */
/* 782 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x8 ),	/* 8 */
/* 788 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 790 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */
/* 796 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppUser */

/* 798 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 800 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 802 */	NdrFcShort( 0x102 ),	/* Type Offset=258 */

	/* Return value */

/* 804 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 806 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendMsg */

/* 810 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 812 */	NdrFcLong( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x1b ),	/* 27 */
/* 818 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 820 */	NdrFcShort( 0x0 ),	/* 0 */
/* 822 */	NdrFcShort( 0x8 ),	/* 8 */
/* 824 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 826 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 830 */	NdrFcShort( 0x1 ),	/* 1 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrMessage */

/* 834 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 836 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 838 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Return value */

/* 840 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 842 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 844 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Kill */

/* 846 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 848 */	NdrFcLong( 0x0 ),	/* 0 */
/* 852 */	NdrFcShort( 0x1c ),	/* 28 */
/* 854 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x8 ),	/* 8 */
/* 860 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 862 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 864 */	NdrFcShort( 0x0 ),	/* 0 */
/* 866 */	NdrFcShort( 0x0 ),	/* 0 */
/* 868 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 870 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 872 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 874 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Users */

/* 876 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 878 */	NdrFcLong( 0x0 ),	/* 0 */
/* 882 */	NdrFcShort( 0x1d ),	/* 29 */
/* 884 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 888 */	NdrFcShort( 0x8 ),	/* 8 */
/* 890 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 892 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 894 */	NdrFcShort( 0x0 ),	/* 0 */
/* 896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 898 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppUsers */

/* 900 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 902 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 904 */	NdrFcShort( 0x118 ),	/* Type Offset=280 */

	/* Return value */

/* 906 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 908 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 910 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GameOwnerUser */

/* 912 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 914 */	NdrFcLong( 0x0 ),	/* 0 */
/* 918 */	NdrFcShort( 0x1e ),	/* 30 */
/* 920 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 922 */	NdrFcShort( 0x0 ),	/* 0 */
/* 924 */	NdrFcShort( 0x8 ),	/* 8 */
/* 926 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 928 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 930 */	NdrFcShort( 0x0 ),	/* 0 */
/* 932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 934 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppUser */

/* 936 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 938 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 940 */	NdrFcShort( 0x102 ),	/* Type Offset=258 */

	/* Return value */

/* 942 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 944 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 946 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartGame */

/* 948 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 950 */	NdrFcLong( 0x0 ),	/* 0 */
/* 954 */	NdrFcShort( 0x1f ),	/* 31 */
/* 956 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 958 */	NdrFcShort( 0x0 ),	/* 0 */
/* 960 */	NdrFcShort( 0x8 ),	/* 8 */
/* 962 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 964 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 966 */	NdrFcShort( 0x0 ),	/* 0 */
/* 968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 970 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 972 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 974 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 976 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Description */

/* 978 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 980 */	NdrFcLong( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0x20 ),	/* 32 */
/* 986 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 988 */	NdrFcShort( 0x0 ),	/* 0 */
/* 990 */	NdrFcShort( 0x8 ),	/* 8 */
/* 992 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 994 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 996 */	NdrFcShort( 0x1 ),	/* 1 */
/* 998 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1000 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrDescription */

/* 1002 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1004 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1006 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 1008 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1010 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1012 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure StartCountdown */

/* 1014 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1016 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x21 ),	/* 33 */
/* 1022 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1026 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1028 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1030 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1032 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1034 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1036 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetTeamName */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1052 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1054 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1056 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1058 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1060 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1064 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter iSideID */

/* 1068 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bstrName */

/* 1074 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Return value */

/* 1080 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1082 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1084 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure OverrideTechBit */

/* 1086 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1088 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1092 */	NdrFcShort( 0x23 ),	/* 35 */
/* 1094 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 1096 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1098 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1100 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 1102 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1104 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1108 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter iSideID */

/* 1110 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1112 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter nTechBitID */

/* 1116 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1118 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1120 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter bNewSetting */

/* 1122 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1128 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1130 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1132 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetDetailsFiles */

/* 1134 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1136 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1140 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1142 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1144 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1146 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1148 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1150 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1154 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrName */

/* 1158 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1160 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1162 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Return value */

/* 1164 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1166 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure RandomizeTeams */

/* 1170 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1172 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1176 */	NdrFcShort( 0x25 ),	/* 37 */
/* 1178 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1180 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1182 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1184 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1186 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1190 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1192 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1194 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1196 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1198 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Item */

/* 1200 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1206 */	NdrFcShort( 0xa ),	/* 10 */
/* 1208 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1214 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1216 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1218 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1220 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1222 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pvIndex */

/* 1224 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 1226 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1228 */	NdrFcShort( 0x53a ),	/* Type Offset=1338 */

	/* Parameter ppGame */

/* 1230 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1232 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1234 */	NdrFcShort( 0x544 ),	/* Type Offset=1348 */

	/* Return value */

/* 1236 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1238 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1240 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Add */

/* 1242 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1244 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1248 */	NdrFcShort( 0xb ),	/* 11 */
/* 1250 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1252 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1254 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1256 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1258 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1260 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1262 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1264 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pGameParameters */

/* 1266 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 1268 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1270 */	NdrFcShort( 0x55a ),	/* Type Offset=1370 */

	/* Return value */

/* 1272 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1274 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1276 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Games */

/* 1278 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1280 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1284 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1286 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1290 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1292 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1294 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1296 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1300 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppGames */

/* 1302 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1304 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1306 */	NdrFcShort( 0x56c ),	/* Type Offset=1388 */

	/* Return value */

/* 1308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1310 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PlayerCount */

/* 1314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1320 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1322 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1326 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1328 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1330 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1334 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1336 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1338 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1340 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1342 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1344 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1346 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1348 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_MissionCount */

/* 1350 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1352 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1356 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1358 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1360 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1362 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1364 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1366 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1368 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1372 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1374 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1376 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1378 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1380 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1382 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1384 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendMsg */

/* 1386 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1388 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1392 */	NdrFcShort( 0xa ),	/* 10 */
/* 1394 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1396 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1398 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1400 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1402 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1406 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1408 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrMessage */

/* 1410 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1412 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1414 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Return value */

/* 1416 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1418 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1420 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Users */

/* 1422 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1424 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1428 */	NdrFcShort( 0xb ),	/* 11 */
/* 1430 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1432 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1434 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1436 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1438 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1440 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1444 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppUsers */

/* 1446 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1448 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1450 */	NdrFcShort( 0x582 ),	/* Type Offset=1410 */

	/* Return value */

/* 1452 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1454 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1456 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LookupUser */

/* 1458 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1460 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1464 */	NdrFcShort( 0xc ),	/* 12 */
/* 1466 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1468 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1470 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1472 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1474 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1476 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1478 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1480 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter id */

/* 1482 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1484 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter ppUser */

/* 1488 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1490 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1492 */	NdrFcShort( 0x598 ),	/* Type Offset=1432 */

	/* Return value */

/* 1494 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1496 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1498 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_FindUser */

/* 1500 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1502 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1506 */	NdrFcShort( 0xd ),	/* 13 */
/* 1508 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1512 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1514 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 1516 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1518 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1520 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1522 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrName */

/* 1524 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1526 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1528 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Parameter ppUser */

/* 1530 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1532 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1534 */	NdrFcShort( 0x598 ),	/* Type Offset=1432 */

	/* Return value */

/* 1536 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1538 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1540 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_MachineName */

/* 1542 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1544 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1548 */	NdrFcShort( 0xe ),	/* 14 */
/* 1550 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1554 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1556 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1558 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1560 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1562 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1564 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrMachineName */

/* 1566 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1568 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1570 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 1572 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1574 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1576 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PacketsIn */

/* 1578 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1580 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1584 */	NdrFcShort( 0xf ),	/* 15 */
/* 1586 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1590 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1592 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1594 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1598 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1600 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1602 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1604 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1606 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1608 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1610 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1612 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_TimeInnerLoop */

/* 1614 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1616 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1620 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1622 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1624 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1626 */	NdrFcShort( 0x24 ),	/* 36 */
/* 1628 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1630 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1632 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1634 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1636 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1638 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1640 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1642 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1644 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1646 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1648 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LobbyServer */

/* 1650 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1652 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1656 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1658 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1662 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1664 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1666 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1670 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1672 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrLobbyServer */

/* 1674 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1676 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1678 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Return value */

/* 1680 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1682 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1684 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LobbyServer */

/* 1686 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1688 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1692 */	NdrFcShort( 0x13 ),	/* 19 */
/* 1694 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1696 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1698 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1700 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1702 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1704 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1706 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1708 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstrLobbyServer */

/* 1710 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1712 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1714 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 1716 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1718 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1720 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_PublicLobby */

/* 1722 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1724 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1728 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1730 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1732 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1734 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1736 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1738 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1744 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bPublic */

/* 1746 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1748 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1750 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1752 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1754 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1756 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PublicLobby */

/* 1758 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1760 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1764 */	NdrFcShort( 0x15 ),	/* 21 */
/* 1766 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1768 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1770 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1772 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1774 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1776 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1778 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1780 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbPublic */

/* 1782 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1784 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1786 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1788 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1790 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1792 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CreateDefaultGames */

/* 1794 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1796 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1800 */	NdrFcShort( 0x16 ),	/* 22 */
/* 1802 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1804 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1806 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1808 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 1810 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1812 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1814 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1816 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 1818 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1820 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1822 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Name */

/* 1824 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1826 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1830 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1832 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1836 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1838 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1840 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1842 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1844 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1846 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbstr */

/* 1848 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1850 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1852 */	NdrFcShort( 0x136 ),	/* Type Offset=310 */

	/* Return value */

/* 1854 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1856 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1858 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SendMsg */

/* 1860 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1862 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1866 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1868 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1872 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1874 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1876 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1880 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1882 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter bstrMessage */

/* 1884 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1886 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1888 */	NdrFcShort( 0xba ),	/* Type Offset=186 */

	/* Return value */

/* 1890 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1892 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_isReady */

/* 1896 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1898 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1902 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1904 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1908 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1910 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1912 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1916 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1918 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1920 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1922 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1924 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1926 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1928 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1930 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_isReady */

/* 1932 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1934 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1938 */	NdrFcShort( 0xa ),	/* 10 */
/* 1940 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1942 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1944 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1946 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1948 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1950 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1952 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1954 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Val */

/* 1956 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1958 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1960 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1962 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1964 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1966 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Ship */

/* 1968 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1970 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1974 */	NdrFcShort( 0xb ),	/* 11 */
/* 1976 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1980 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1982 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1984 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1986 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1988 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1990 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppAdminShip */

/* 1992 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1994 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1996 */	NdrFcShort( 0x5b2 ),	/* Type Offset=1458 */

	/* Return value */

/* 1998 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2000 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2002 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Boot */

/* 2004 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2006 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2010 */	NdrFcShort( 0xc ),	/* 12 */
/* 2012 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2016 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2018 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 2020 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2022 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2026 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 2028 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2030 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2032 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UserID */

/* 2034 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2036 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2040 */	NdrFcShort( 0xd ),	/* 13 */
/* 2042 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2044 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2046 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2048 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2050 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2052 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2054 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2056 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter userid */

/* 2058 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2060 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2062 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2064 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2066 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2068 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_UniqueID */

/* 2070 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2072 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2076 */	NdrFcShort( 0xe ),	/* 14 */
/* 2078 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2080 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2082 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2084 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2086 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2088 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2090 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2092 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter uniqueID */

/* 2094 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2096 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2098 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2100 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2102 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2104 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PlayerStats */

/* 2106 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2108 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2112 */	NdrFcShort( 0xf ),	/* 15 */
/* 2114 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2116 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2120 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2122 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2124 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2128 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ppStats */

/* 2130 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2132 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2134 */	NdrFcShort( 0x5c8 ),	/* Type Offset=1480 */

	/* Return value */

/* 2136 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2138 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2140 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Item */

/* 2142 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2144 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2148 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2150 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 2152 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2154 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2156 */	0x47,		/* Oi2 Flags:  srv must size, clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 2158 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2160 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2162 */	NdrFcShort( 0x20 ),	/* 32 */
/* 2164 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 2166 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2168 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2170 */	NdrFcShort( 0x53a ),	/* Type Offset=1338 */

	/* Parameter ppUser */

/* 2172 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2174 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 2176 */	NdrFcShort( 0x598 ),	/* Type Offset=1432 */

	/* Return value */

/* 2178 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2180 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 2182 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/*  4 */	NdrFcLong( 0x34e485e2 ),	/* 887391714 */
/*  8 */	NdrFcShort( 0xc7b7 ),	/* -14409 */
/* 10 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 12 */	0x85,		/* 133 */
			0xc9,		/* 201 */
/* 14 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 16 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 18 */	0xde,		/* 222 */
			0xb0,		/* 176 */
/* 20 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 22 */	NdrFcShort( 0xffec ),	/* Offset= -20 (2) */
/* 24 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 26 */	NdrFcShort( 0x2 ),	/* Offset= 2 (28) */
/* 28 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 30 */	NdrFcLong( 0x45038c6e ),	/* 1157860462 */
/* 34 */	NdrFcShort( 0xc7b7 ),	/* -14409 */
/* 36 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 38 */	0x85,		/* 133 */
			0xc9,		/* 201 */
/* 40 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 42 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 44 */	0xde,		/* 222 */
			0xb0,		/* 176 */
/* 46 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 48 */	NdrFcShort( 0x2 ),	/* Offset= 2 (50) */
/* 50 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 52 */	NdrFcLong( 0x20721450 ),	/* 544347216 */
/* 56 */	NdrFcShort( 0xe7c0 ),	/* -6208 */
/* 58 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 60 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 62 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 64 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 66 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 68 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 70 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 72 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 74 */	NdrFcShort( 0x2 ),	/* Offset= 2 (76) */
/* 76 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 78 */	NdrFcLong( 0x160da17d ),	/* 369992061 */
/* 82 */	NdrFcShort( 0xc8d5 ),	/* -14123 */
/* 84 */	NdrFcShort( 0x4352 ),	/* 17234 */
/* 86 */	0x96,		/* 150 */
			0x54,		/* 84 */
/* 88 */	0x6e,		/* 110 */
			0x65,		/* 101 */
/* 90 */	0x8a,		/* 138 */
			0x5a,		/* 90 */
/* 92 */	0x8,		/* 8 */
			0x87,		/* 135 */
/* 94 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 96 */	NdrFcShort( 0x2 ),	/* Offset= 2 (98) */
/* 98 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 100 */	NdrFcLong( 0x8d9be088 ),	/* -1919164280 */
/* 104 */	NdrFcShort( 0xdda4 ),	/* -8796 */
/* 106 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 108 */	0x8b,		/* 139 */
			0x46,		/* 70 */
/* 110 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 112 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 114 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 116 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 118 */	NdrFcShort( 0x2 ),	/* Offset= 2 (120) */
/* 120 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 122 */	NdrFcLong( 0x28755aa1 ),	/* 678779553 */
/* 126 */	NdrFcShort( 0x6391 ),	/* 25489 */
/* 128 */	NdrFcShort( 0x4d37 ),	/* 19767 */
/* 130 */	0xa9,		/* 169 */
			0x95,		/* 149 */
/* 132 */	0x5a,		/* 90 */
			0xe3,		/* 227 */
/* 134 */	0x87,		/* 135 */
			0xd0,		/* 208 */
/* 136 */	0xb6,		/* 182 */
			0x10,		/* 16 */
/* 138 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 140 */	NdrFcShort( 0x2 ),	/* Offset= 2 (142) */
/* 142 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 144 */	NdrFcLong( 0xd2efe888 ),	/* -756029304 */
/* 148 */	NdrFcShort( 0x1480 ),	/* 5248 */
/* 150 */	NdrFcShort( 0x11d3 ),	/* 4563 */
/* 152 */	0x8b,		/* 139 */
			0x5f,		/* 95 */
/* 154 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 156 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 158 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 160 */	
			0x12, 0x0,	/* FC_UP */
/* 162 */	NdrFcShort( 0xe ),	/* Offset= 14 (176) */
/* 164 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 166 */	NdrFcShort( 0x2 ),	/* 2 */
/* 168 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 170 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 172 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 174 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 176 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 178 */	NdrFcShort( 0x8 ),	/* 8 */
/* 180 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (164) */
/* 182 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 184 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 186 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 188 */	NdrFcShort( 0x0 ),	/* 0 */
/* 190 */	NdrFcShort( 0x4 ),	/* 4 */
/* 192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 194 */	NdrFcShort( 0xffde ),	/* Offset= -34 (160) */
/* 196 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 198 */	NdrFcShort( 0x2 ),	/* Offset= 2 (200) */
/* 200 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 202 */	NdrFcLong( 0x0 ),	/* 0 */
/* 206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 210 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 212 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 214 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 216 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 218 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 220 */	NdrFcLong( 0xcd7d4875 ),	/* -847427467 */
/* 224 */	NdrFcShort( 0x3d61 ),	/* 15713 */
/* 226 */	NdrFcShort( 0x4cc4 ),	/* 19652 */
/* 228 */	0x83,		/* 131 */
			0xc1,		/* 193 */
/* 230 */	0x48,		/* 72 */
			0xf6,		/* 246 */
/* 232 */	0x98,		/* 152 */
			0xed,		/* 237 */
/* 234 */	0x45,		/* 69 */
			0xf5,		/* 245 */
/* 236 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 238 */	NdrFcShort( 0x2 ),	/* Offset= 2 (240) */
/* 240 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 242 */	NdrFcLong( 0x8d9be088 ),	/* -1919164280 */
/* 246 */	NdrFcShort( 0xdda4 ),	/* -8796 */
/* 248 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 250 */	0x8b,		/* 139 */
			0x46,		/* 70 */
/* 252 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 254 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 256 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 258 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 260 */	NdrFcShort( 0x2 ),	/* Offset= 2 (262) */
/* 262 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 264 */	NdrFcLong( 0xd5c9ad80 ),	/* -708203136 */
/* 268 */	NdrFcShort( 0xeba4 ),	/* -5212 */
/* 270 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 272 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 274 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 276 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 278 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 280 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 282 */	NdrFcShort( 0x2 ),	/* Offset= 2 (284) */
/* 284 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 286 */	NdrFcLong( 0xf9e52a79 ),	/* -102421895 */
/* 290 */	NdrFcShort( 0xeba4 ),	/* -5212 */
/* 292 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 294 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 296 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 298 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 300 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 302 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 304 */	NdrFcShort( 0x6 ),	/* Offset= 6 (310) */
/* 306 */	
			0x13, 0x0,	/* FC_OP */
/* 308 */	NdrFcShort( 0xff7c ),	/* Offset= -132 (176) */
/* 310 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 312 */	NdrFcShort( 0x0 ),	/* 0 */
/* 314 */	NdrFcShort( 0x4 ),	/* 4 */
/* 316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 318 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (306) */
/* 320 */	
			0x11, 0x0,	/* FC_RP */
/* 322 */	NdrFcShort( 0x3f8 ),	/* Offset= 1016 (1338) */
/* 324 */	
			0x12, 0x0,	/* FC_UP */
/* 326 */	NdrFcShort( 0x3e0 ),	/* Offset= 992 (1318) */
/* 328 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 330 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 332 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 334 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 336 */	NdrFcShort( 0x2 ),	/* Offset= 2 (338) */
/* 338 */	NdrFcShort( 0x10 ),	/* 16 */
/* 340 */	NdrFcShort( 0x2f ),	/* 47 */
/* 342 */	NdrFcLong( 0x14 ),	/* 20 */
/* 346 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 348 */	NdrFcLong( 0x3 ),	/* 3 */
/* 352 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 354 */	NdrFcLong( 0x11 ),	/* 17 */
/* 358 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 360 */	NdrFcLong( 0x2 ),	/* 2 */
/* 364 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 366 */	NdrFcLong( 0x4 ),	/* 4 */
/* 370 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 372 */	NdrFcLong( 0x5 ),	/* 5 */
/* 376 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 378 */	NdrFcLong( 0xb ),	/* 11 */
/* 382 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 384 */	NdrFcLong( 0xa ),	/* 10 */
/* 388 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 390 */	NdrFcLong( 0x6 ),	/* 6 */
/* 394 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (626) */
/* 396 */	NdrFcLong( 0x7 ),	/* 7 */
/* 400 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 402 */	NdrFcLong( 0x8 ),	/* 8 */
/* 406 */	NdrFcShort( 0xff0a ),	/* Offset= -246 (160) */
/* 408 */	NdrFcLong( 0xd ),	/* 13 */
/* 412 */	NdrFcShort( 0xff2c ),	/* Offset= -212 (200) */
/* 414 */	NdrFcLong( 0x9 ),	/* 9 */
/* 418 */	NdrFcShort( 0xd6 ),	/* Offset= 214 (632) */
/* 420 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 424 */	NdrFcShort( 0xe2 ),	/* Offset= 226 (650) */
/* 426 */	NdrFcLong( 0x24 ),	/* 36 */
/* 430 */	NdrFcShort( 0x32e ),	/* Offset= 814 (1244) */
/* 432 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 436 */	NdrFcShort( 0x328 ),	/* Offset= 808 (1244) */
/* 438 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 442 */	NdrFcShort( 0x326 ),	/* Offset= 806 (1248) */
/* 444 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 448 */	NdrFcShort( 0x324 ),	/* Offset= 804 (1252) */
/* 450 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 454 */	NdrFcShort( 0x322 ),	/* Offset= 802 (1256) */
/* 456 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 460 */	NdrFcShort( 0x320 ),	/* Offset= 800 (1260) */
/* 462 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 466 */	NdrFcShort( 0x31e ),	/* Offset= 798 (1264) */
/* 468 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 472 */	NdrFcShort( 0x31c ),	/* Offset= 796 (1268) */
/* 474 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 478 */	NdrFcShort( 0x306 ),	/* Offset= 774 (1252) */
/* 480 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 484 */	NdrFcShort( 0x304 ),	/* Offset= 772 (1256) */
/* 486 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 490 */	NdrFcShort( 0x30e ),	/* Offset= 782 (1272) */
/* 492 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 496 */	NdrFcShort( 0x304 ),	/* Offset= 772 (1268) */
/* 498 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 502 */	NdrFcShort( 0x306 ),	/* Offset= 774 (1276) */
/* 504 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 508 */	NdrFcShort( 0x304 ),	/* Offset= 772 (1280) */
/* 510 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 514 */	NdrFcShort( 0x302 ),	/* Offset= 770 (1284) */
/* 516 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 520 */	NdrFcShort( 0x300 ),	/* Offset= 768 (1288) */
/* 522 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 526 */	NdrFcShort( 0x2fe ),	/* Offset= 766 (1292) */
/* 528 */	NdrFcLong( 0x10 ),	/* 16 */
/* 532 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 534 */	NdrFcLong( 0x12 ),	/* 18 */
/* 538 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 540 */	NdrFcLong( 0x13 ),	/* 19 */
/* 544 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 546 */	NdrFcLong( 0x15 ),	/* 21 */
/* 550 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 552 */	NdrFcLong( 0x16 ),	/* 22 */
/* 556 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 558 */	NdrFcLong( 0x17 ),	/* 23 */
/* 562 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 564 */	NdrFcLong( 0xe ),	/* 14 */
/* 568 */	NdrFcShort( 0x2dc ),	/* Offset= 732 (1300) */
/* 570 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 574 */	NdrFcShort( 0x2e0 ),	/* Offset= 736 (1310) */
/* 576 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 580 */	NdrFcShort( 0x2de ),	/* Offset= 734 (1314) */
/* 582 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 586 */	NdrFcShort( 0x29a ),	/* Offset= 666 (1252) */
/* 588 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 592 */	NdrFcShort( 0x298 ),	/* Offset= 664 (1256) */
/* 594 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 598 */	NdrFcShort( 0x296 ),	/* Offset= 662 (1260) */
/* 600 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 604 */	NdrFcShort( 0x28c ),	/* Offset= 652 (1256) */
/* 606 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 610 */	NdrFcShort( 0x286 ),	/* Offset= 646 (1256) */
/* 612 */	NdrFcLong( 0x0 ),	/* 0 */
/* 616 */	NdrFcShort( 0x0 ),	/* Offset= 0 (616) */
/* 618 */	NdrFcLong( 0x1 ),	/* 1 */
/* 622 */	NdrFcShort( 0x0 ),	/* Offset= 0 (622) */
/* 624 */	NdrFcShort( 0xffff ),	/* Offset= -1 (623) */
/* 626 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 628 */	NdrFcShort( 0x8 ),	/* 8 */
/* 630 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 632 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 634 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 644 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 646 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 648 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 650 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 652 */	NdrFcShort( 0x2 ),	/* Offset= 2 (654) */
/* 654 */	
			0x12, 0x0,	/* FC_UP */
/* 656 */	NdrFcShort( 0x23a ),	/* Offset= 570 (1226) */
/* 658 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 660 */	NdrFcShort( 0x18 ),	/* 24 */
/* 662 */	NdrFcShort( 0xa ),	/* 10 */
/* 664 */	NdrFcLong( 0x8 ),	/* 8 */
/* 668 */	NdrFcShort( 0x5a ),	/* Offset= 90 (758) */
/* 670 */	NdrFcLong( 0xd ),	/* 13 */
/* 674 */	NdrFcShort( 0x90 ),	/* Offset= 144 (818) */
/* 676 */	NdrFcLong( 0x9 ),	/* 9 */
/* 680 */	NdrFcShort( 0xc2 ),	/* Offset= 194 (874) */
/* 682 */	NdrFcLong( 0xc ),	/* 12 */
/* 686 */	NdrFcShort( 0xec ),	/* Offset= 236 (922) */
/* 688 */	NdrFcLong( 0x24 ),	/* 36 */
/* 692 */	NdrFcShort( 0x148 ),	/* Offset= 328 (1020) */
/* 694 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 698 */	NdrFcShort( 0x164 ),	/* Offset= 356 (1054) */
/* 700 */	NdrFcLong( 0x10 ),	/* 16 */
/* 704 */	NdrFcShort( 0x17e ),	/* Offset= 382 (1086) */
/* 706 */	NdrFcLong( 0x2 ),	/* 2 */
/* 710 */	NdrFcShort( 0x198 ),	/* Offset= 408 (1118) */
/* 712 */	NdrFcLong( 0x3 ),	/* 3 */
/* 716 */	NdrFcShort( 0x1b2 ),	/* Offset= 434 (1150) */
/* 718 */	NdrFcLong( 0x14 ),	/* 20 */
/* 722 */	NdrFcShort( 0x1cc ),	/* Offset= 460 (1182) */
/* 724 */	NdrFcShort( 0xffff ),	/* Offset= -1 (723) */
/* 726 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 728 */	NdrFcShort( 0x4 ),	/* 4 */
/* 730 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 732 */	NdrFcShort( 0x0 ),	/* 0 */
/* 734 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 736 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 738 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 740 */	NdrFcShort( 0x4 ),	/* 4 */
/* 742 */	NdrFcShort( 0x0 ),	/* 0 */
/* 744 */	NdrFcShort( 0x1 ),	/* 1 */
/* 746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	0x12, 0x0,	/* FC_UP */
/* 752 */	NdrFcShort( 0xfdc0 ),	/* Offset= -576 (176) */
/* 754 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 756 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 758 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 760 */	NdrFcShort( 0x8 ),	/* 8 */
/* 762 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 764 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 766 */	NdrFcShort( 0x4 ),	/* 4 */
/* 768 */	NdrFcShort( 0x4 ),	/* 4 */
/* 770 */	0x11, 0x0,	/* FC_RP */
/* 772 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (726) */
/* 774 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 776 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 778 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 780 */	NdrFcLong( 0x0 ),	/* 0 */
/* 784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 790 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 792 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 794 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 796 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 800 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 802 */	NdrFcShort( 0x0 ),	/* 0 */
/* 804 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 806 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 810 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 812 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 814 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (778) */
/* 816 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 818 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 820 */	NdrFcShort( 0x8 ),	/* 8 */
/* 822 */	NdrFcShort( 0x0 ),	/* 0 */
/* 824 */	NdrFcShort( 0x6 ),	/* Offset= 6 (830) */
/* 826 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 828 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 830 */	
			0x11, 0x0,	/* FC_RP */
/* 832 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (796) */
/* 834 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 836 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 840 */	NdrFcShort( 0x0 ),	/* 0 */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 846 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 848 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 850 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 852 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 856 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 858 */	NdrFcShort( 0x0 ),	/* 0 */
/* 860 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 862 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 866 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 868 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 870 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (834) */
/* 872 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 874 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 876 */	NdrFcShort( 0x8 ),	/* 8 */
/* 878 */	NdrFcShort( 0x0 ),	/* 0 */
/* 880 */	NdrFcShort( 0x6 ),	/* Offset= 6 (886) */
/* 882 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 884 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 886 */	
			0x11, 0x0,	/* FC_RP */
/* 888 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (852) */
/* 890 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 892 */	NdrFcShort( 0x4 ),	/* 4 */
/* 894 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 898 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 900 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 902 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 904 */	NdrFcShort( 0x4 ),	/* 4 */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	NdrFcShort( 0x1 ),	/* 1 */
/* 910 */	NdrFcShort( 0x0 ),	/* 0 */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	0x12, 0x0,	/* FC_UP */
/* 916 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1318) */
/* 918 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 920 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 922 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 924 */	NdrFcShort( 0x8 ),	/* 8 */
/* 926 */	NdrFcShort( 0x0 ),	/* 0 */
/* 928 */	NdrFcShort( 0x6 ),	/* Offset= 6 (934) */
/* 930 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 932 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 934 */	
			0x11, 0x0,	/* FC_RP */
/* 936 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (890) */
/* 938 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 940 */	NdrFcLong( 0x2f ),	/* 47 */
/* 944 */	NdrFcShort( 0x0 ),	/* 0 */
/* 946 */	NdrFcShort( 0x0 ),	/* 0 */
/* 948 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 950 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 952 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 954 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 956 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 958 */	NdrFcShort( 0x1 ),	/* 1 */
/* 960 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 962 */	NdrFcShort( 0x4 ),	/* 4 */
/* 964 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 966 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 968 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 970 */	NdrFcShort( 0x10 ),	/* 16 */
/* 972 */	NdrFcShort( 0x0 ),	/* 0 */
/* 974 */	NdrFcShort( 0xa ),	/* Offset= 10 (984) */
/* 976 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 978 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 980 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (938) */
/* 982 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 984 */	
			0x12, 0x0,	/* FC_UP */
/* 986 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (956) */
/* 988 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 990 */	NdrFcShort( 0x4 ),	/* 4 */
/* 992 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */
/* 996 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 998 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1000 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 1002 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1006 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1008 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1010 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1012 */	0x12, 0x0,	/* FC_UP */
/* 1014 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (968) */
/* 1016 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1018 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1020 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1022 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1024 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1026 */	NdrFcShort( 0x6 ),	/* Offset= 6 (1032) */
/* 1028 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 1030 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1032 */	
			0x11, 0x0,	/* FC_RP */
/* 1034 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (988) */
/* 1036 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 1038 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1040 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1042 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1044 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1046 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 1048 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 1050 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (1036) */
			0x5b,		/* FC_END */
/* 1054 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1056 */	NdrFcShort( 0x18 ),	/* 24 */
/* 1058 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1060 */	NdrFcShort( 0xa ),	/* Offset= 10 (1070) */
/* 1062 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 1064 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1066 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (1042) */
/* 1068 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1070 */	
			0x11, 0x0,	/* FC_RP */
/* 1072 */	NdrFcShort( 0xfeec ),	/* Offset= -276 (796) */
/* 1074 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 1076 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1078 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1080 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1082 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1084 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 1086 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1088 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1090 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1092 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1094 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1096 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1098 */	0x12, 0x0,	/* FC_UP */
/* 1100 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1074) */
/* 1102 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1104 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1106 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 1108 */	NdrFcShort( 0x2 ),	/* 2 */
/* 1110 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1114 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1116 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 1118 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1122 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1124 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1126 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1128 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1130 */	0x12, 0x0,	/* FC_UP */
/* 1132 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1106) */
/* 1134 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1136 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1138 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1140 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1142 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1144 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1146 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1148 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1150 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1152 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1154 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1156 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1158 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1160 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1162 */	0x12, 0x0,	/* FC_UP */
/* 1164 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1138) */
/* 1166 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1168 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1170 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 1172 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1174 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 1176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1178 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1180 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1182 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 1184 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1186 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 1188 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 1190 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1192 */	NdrFcShort( 0x4 ),	/* 4 */
/* 1194 */	0x12, 0x0,	/* FC_UP */
/* 1196 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (1170) */
/* 1198 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 1200 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 1202 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 1204 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1206 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1208 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1210 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 1212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1214 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 1216 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 1218 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 1220 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1222 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1202) */
/* 1224 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1226 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 1228 */	NdrFcShort( 0x28 ),	/* 40 */
/* 1230 */	NdrFcShort( 0xffec ),	/* Offset= -20 (1210) */
/* 1232 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1232) */
/* 1234 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1236 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1238 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1240 */	NdrFcShort( 0xfdba ),	/* Offset= -582 (658) */
/* 1242 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1244 */	
			0x12, 0x0,	/* FC_UP */
/* 1246 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (968) */
/* 1248 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1250 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 1252 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1254 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1256 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1258 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 1260 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1262 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 1264 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1266 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 1268 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1270 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1272 */	
			0x12, 0x0,	/* FC_UP */
/* 1274 */	NdrFcShort( 0xfd78 ),	/* Offset= -648 (626) */
/* 1276 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1278 */	NdrFcShort( 0xfba2 ),	/* Offset= -1118 (160) */
/* 1280 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1282 */	NdrFcShort( 0xfe08 ),	/* Offset= -504 (778) */
/* 1284 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1286 */	NdrFcShort( 0xfe3c ),	/* Offset= -452 (834) */
/* 1288 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1290 */	NdrFcShort( 0xfd80 ),	/* Offset= -640 (650) */
/* 1292 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1294 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1296) */
/* 1296 */	
			0x12, 0x0,	/* FC_UP */
/* 1298 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1318) */
/* 1300 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1302 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1304 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1306 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1308 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1310 */	
			0x12, 0x0,	/* FC_UP */
/* 1312 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1300) */
/* 1314 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1316 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1318 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1320 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1322 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1324 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1324) */
/* 1326 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1328 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1330 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1332 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1334 */	NdrFcShort( 0xfc12 ),	/* Offset= -1006 (328) */
/* 1336 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1338 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1340 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1342 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1346 */	NdrFcShort( 0xfc02 ),	/* Offset= -1022 (324) */
/* 1348 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1350 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1352) */
/* 1352 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1354 */	NdrFcLong( 0x8fc514f8 ),	/* -1882909448 */
/* 1358 */	NdrFcShort( 0xe6ce ),	/* -6450 */
/* 1360 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 1362 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 1364 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1366 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1368 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 1370 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1372 */	NdrFcLong( 0xe71ea5b9 ),	/* -417421895 */
/* 1376 */	NdrFcShort( 0xeba4 ),	/* -5212 */
/* 1378 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 1380 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 1382 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1384 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1386 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 1388 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1390 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1392) */
/* 1392 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1394 */	NdrFcLong( 0x3ace4410 ),	/* 986596368 */
/* 1398 */	NdrFcShort( 0xe6d3 ),	/* -6445 */
/* 1400 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 1402 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 1404 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1406 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1408 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 1410 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1412 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1414) */
/* 1414 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1416 */	NdrFcLong( 0xf9e52a79 ),	/* -102421895 */
/* 1420 */	NdrFcShort( 0xeba4 ),	/* -5212 */
/* 1422 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 1424 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 1426 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1428 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1430 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 1432 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1434 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1436) */
/* 1436 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1438 */	NdrFcLong( 0xd5c9ad80 ),	/* -708203136 */
/* 1442 */	NdrFcShort( 0xeba4 ),	/* -5212 */
/* 1444 */	NdrFcShort( 0x11d2 ),	/* 4562 */
/* 1446 */	0x8b,		/* 139 */
			0x4b,		/* 75 */
/* 1448 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1450 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1452 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 1454 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 1456 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 1458 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1460 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1462) */
/* 1462 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1464 */	NdrFcLong( 0x986e689a ),	/* -1737594726 */
/* 1468 */	NdrFcShort( 0x2db4 ),	/* 11700 */
/* 1470 */	NdrFcShort( 0x11d3 ),	/* 4563 */
/* 1472 */	0x8b,		/* 139 */
			0x66,		/* 102 */
/* 1474 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1476 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1478 */	0x16,		/* 22 */
			0x33,		/* 51 */
/* 1480 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1482 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1484) */
/* 1484 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1486 */	NdrFcLong( 0xd2efe888 ),	/* -756029304 */
/* 1490 */	NdrFcShort( 0x1480 ),	/* 5248 */
/* 1492 */	NdrFcShort( 0x11d3 ),	/* 4563 */
/* 1494 */	0x8b,		/* 139 */
			0x5f,		/* 95 */
/* 1496 */	0x0,		/* 0 */
			0xc0,		/* 192 */
/* 1498 */	0x4f,		/* 79 */
			0x68,		/* 104 */
/* 1500 */	0x16,		/* 22 */
			0x33,		/* 51 */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_AllSrvModuleIDL_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IAdminSession, ver. 0.0,
   GUID={0x8D9BE088,0xDDA4,0x11d2,{0x8B,0x46,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminSession_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    144,
    186,
    228,
    276,
    306,
    336,
    372,
    408,
    438,
    468,
    504,
    540,
    576,
    624
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminSession_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminSession_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminSession_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminSession_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(25) _IAdminSessionProxyVtbl = 
{
    &IAdminSession_ProxyInfo,
    &IID_IAdminSession,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::put_SessionInfo */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_SessionInfo */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_SessionInfos */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_Server */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::ActivateEvents */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::DeactivateEvents */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_IsEventActivated */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::ActivateAllEvents */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::DeactivateAllEvents */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_ProcessID */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_Version */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::Stop */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::Pause */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_WhoStartedServer */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_EventLog */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::get_PerfCounters */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::SendAdminChat */ ,
    (void *) (INT_PTR) -1 /* IAdminSession::Continue */
};


static const PRPC_STUB_FUNCTION IAdminSession_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminSessionStubVtbl =
{
    &IID_IAdminSession,
    &IAdminSession_ServerInfo,
    25,
    &IAdminSession_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAdminSessionHost, ver. 0.0,
   GUID={0xCD7D4875,0x3D61,0x4cc4,{0x83,0xC1,0x48,0xF6,0x98,0xED,0x45,0xF5}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminSessionHost_FormatStringOffsetTable[] =
    {
    654
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminSessionHost_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminSessionHost_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminSessionHost_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminSessionHost_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _IAdminSessionHostProxyVtbl = 
{
    &IAdminSessionHost_ProxyInfo,
    &IID_IAdminSessionHost,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IAdminSessionHost::GetIdentity */
};

const CInterfaceStubVtbl _IAdminSessionHostStubVtbl =
{
    &IID_IAdminSessionHost,
    &IAdminSessionHost_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IAdminSessionClass, ver. 0.0,
   GUID={0xB3339C1B,0xD267,0x420b,{0xB3,0xD7,0x59,0xF1,0x27,0xCD,0xD9,0xCF}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminSessionClass_FormatStringOffsetTable[] =
    {
    696
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminSessionClass_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminSessionClass_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminSessionClass_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminSessionClass_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _IAdminSessionClassProxyVtbl = 
{
    &IAdminSessionClass_ProxyInfo,
    &IID_IAdminSessionClass,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IAdminSessionClass::CreateSession */
};

const CInterfaceStubVtbl _IAdminSessionClassStubVtbl =
{
    &IID_IAdminSessionClass,
    &IAdminSessionClass_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IAdminSessionEvents, ver. 0.0,
   GUID={0xDE3ED156,0x76A0,0x4a8e,{0x8C,0xFE,0x9E,0xD2,0x6C,0x3B,0x0A,0x5E}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminSessionEvents_FormatStringOffsetTable[] =
    {
    738
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminSessionEvents_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminSessionEvents_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminSessionEvents_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminSessionEvents_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(4) _IAdminSessionEventsProxyVtbl = 
{
    &IAdminSessionEvents_ProxyInfo,
    &IID_IAdminSessionEvents,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) (INT_PTR) -1 /* IAdminSessionEvents::OnEvent */
};

const CInterfaceStubVtbl _IAdminSessionEventsStubVtbl =
{
    &IID_IAdminSessionEvents,
    &IAdminSessionEvents_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IAGCCommon, ver. 0.0,
   GUID={0x6EB476CE,0x17BC,0x11d3,{0x8B,0x61,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAGCBase, ver. 0.0,
   GUID={0x6967EA73,0x0A42,0x11D3,{0x8B,0x58,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAGCModel, ver. 0.0,
   GUID={0xD64CB547,0x0A42,0x11D3,{0x8B,0x58,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAGCDamage, ver. 0.0,
   GUID={0xDF2F4B4E,0x0A42,0x11D3,{0x8B,0x58,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAGCScanner, ver. 0.0,
   GUID={0xE9004E70,0x0A42,0x11D3,{0x8B,0x58,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAGCShip, ver. 0.0,
   GUID={0x4F860D30,0x0A42,0x11D3,{0x8B,0x58,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAdminShip, ver. 0.0,
   GUID={0x986E689A,0x2DB4,0x11d3,{0x8B,0x66,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminShip_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    774
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminShip_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminShip_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminShip_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminShip_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(43) _IAdminShipProxyVtbl = 
{
    &IAdminShip_ProxyInfo,
    &IID_IAdminShip,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCCommon::get_Type */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCBase::get_ObjectType */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCBase::get_ObjectID */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCBase::get_Game */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCBase::get_UniqueID */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_IsVisible */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_IsSeenBySide */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Position */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Velocity */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Orientation */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Radius */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Team */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Mass */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Sector */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Signature */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCModel::get_Name */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCDamage::get_Fraction */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCScanner::get_InScannerRange */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCScanner::get_CanSee */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::put_Ammo */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_Ammo */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::put_Fuel */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_Fuel */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::put_Energy */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_Energy */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::put_WingID */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_WingID */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::SendChat */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::SendCommand */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::put_AutoDonate */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_AutoDonate */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::put_ShieldFraction */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_ShieldFraction */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_HullType */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCShip::get_BaseHullType */ ,
    (void *) (INT_PTR) -1 /* IAdminShip::get_User */
};


static const PRPC_STUB_FUNCTION IAdminShip_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminShipStubVtbl =
{
    &IID_IAdminShip,
    &IAdminShip_ServerInfo,
    43,
    &IAdminShip_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAGCGame, ver. 0.0,
   GUID={0xE6D48FB7,0x0A41,0x11D3,{0x8B,0x58,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAdminGame, ver. 0.0,
   GUID={0x8FC514F8,0xE6CE,0x11D2,{0x8B,0x4B,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminGame_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    810,
    846,
    876,
    912,
    948,
    978,
    1014,
    1044,
    1086,
    1134,
    1170
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminGame_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminGame_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminGame_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminGame_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(38) _IAdminGameProxyVtbl = 
{
    &IAdminGame_ProxyInfo,
    &IID_IAdminGame,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCCommon::get_Type */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Name */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Sectors */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Teams */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_LookupShip */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Ships */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Alephs */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Asteroids */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_GameParameters */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_GameID */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_LookupTeam */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::SendChat */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::SendCommand */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_GameStage */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Probes */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Buoys */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Treasures */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_Mines */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_ReplayCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCGame::get_ContextName */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::SendMsg */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::Kill */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::get_Users */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::get_GameOwnerUser */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::StartGame */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::get_Description */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::StartCountdown */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::SetTeamName */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::OverrideTechBit */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::SetDetailsFiles */ ,
    (void *) (INT_PTR) -1 /* IAdminGame::RandomizeTeams */
};


static const PRPC_STUB_FUNCTION IAdminGame_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminGameStubVtbl =
{
    &IID_IAdminGame,
    &IAdminGame_ServerInfo,
    38,
    &IAdminGame_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAGCCollection, ver. 0.0,
   GUID={0xB8DB7D42,0x1E9C,0x11d3,{0x8B,0x63,0x00,0xC0,0x4F,0x68,0x16,0x33}} */


/* Object interface: IAdminGames, ver. 0.0,
   GUID={0x3ACE4410,0xE6D3,0x11D2,{0x8B,0x4B,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminGames_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1200,
    1242
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminGames_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminGames_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminGames_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminGames_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(12) _IAdminGamesProxyVtbl = 
{
    &IAdminGames_ProxyInfo,
    &IID_IAdminGames,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCCommon::get_Type */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCCollection::get_Count */ ,
    0 /* (void *) (INT_PTR) -1 /* IAGCCollection::get__NewEnum */ ,
    (void *) (INT_PTR) -1 /* IAdminGames::get_Item */ ,
    (void *) (INT_PTR) -1 /* IAdminGames::Add */
};


static const PRPC_STUB_FUNCTION IAdminGames_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminGamesStubVtbl =
{
    &IID_IAdminGames,
    &IAdminGames_ServerInfo,
    12,
    &IAdminGames_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAdminServer, ver. 0.0,
   GUID={0x20721450,0xE7C0,0x11D2,{0x8B,0x4B,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminServer_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1278,
    1314,
    1350,
    1386,
    1422,
    1458,
    1500,
    1542,
    1578,
    336,
    1614,
    1650,
    1686,
    1722,
    1758,
    1794
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminServer_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminServer_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminServer_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminServer_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(23) _IAdminServerProxyVtbl = 
{
    &IAdminServer_ProxyInfo,
    &IID_IAdminServer,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_Games */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_PlayerCount */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_MissionCount */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::SendMsg */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_Users */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_LookupUser */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_FindUser */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_MachineName */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_PacketsIn */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_PlayersOnline */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_TimeInnerLoop */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::put_LobbyServer */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_LobbyServer */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::put_PublicLobby */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::get_PublicLobby */ ,
    (void *) (INT_PTR) -1 /* IAdminServer::CreateDefaultGames */
};


static const PRPC_STUB_FUNCTION IAdminServer_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminServerStubVtbl =
{
    &IID_IAdminServer,
    &IAdminServer_ServerInfo,
    23,
    &IAdminServer_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IAdminUser, ver. 0.0,
   GUID={0xD5C9AD80,0xEBA4,0x11D2,{0x8B,0x4B,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminUser_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1824,
    1860,
    1896,
    1932,
    1968,
    2004,
    2034,
    2070,
    2106
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminUser_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminUser_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminUser_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminUser_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(16) _IAdminUserProxyVtbl = 
{
    &IAdminUser_ProxyInfo,
    &IID_IAdminUser,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::get_Name */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::SendMsg */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::get_isReady */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::put_isReady */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::get_Ship */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::Boot */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::get_UserID */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::get_UniqueID */ ,
    (void *) (INT_PTR) -1 /* IAdminUser::get_PlayerStats */
};


static const PRPC_STUB_FUNCTION IAdminUser_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminUserStubVtbl =
{
    &IID_IAdminUser,
    &IAdminUser_ServerInfo,
    16,
    &IAdminUser_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ITCCollection, ver. 0.0,
   GUID={0xB1935981,0xD27B,0x11d2,{0x96,0x46,0x00,0xC0,0x4F,0x68,0xDE,0xB0}} */


/* Object interface: IAdminUsers, ver. 0.0,
   GUID={0xF9E52A79,0xEBA4,0x11D2,{0x8B,0x4B,0x00,0xC0,0x4F,0x68,0x16,0x33}} */

#pragma code_seg(".orpc")
static const unsigned short IAdminUsers_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    2142
    };

static const MIDL_STUBLESS_PROXY_INFO IAdminUsers_ProxyInfo =
    {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdminUsers_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IAdminUsers_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdminUsers_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(10) _IAdminUsersProxyVtbl = 
{
    &IAdminUsers_ProxyInfo,
    &IID_IAdminUsers,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    0 /* (void *) (INT_PTR) -1 /* ITCCollection::get_Count */ ,
    0 /* (void *) (INT_PTR) -1 /* ITCCollection::get__NewEnum */ ,
    (void *) (INT_PTR) -1 /* IAdminUsers::get_Item */
};


static const PRPC_STUB_FUNCTION IAdminUsers_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IAdminUsersStubVtbl =
{
    &IID_IAdminUsers,
    &IAdminUsers_ServerInfo,
    10,
    &IAdminUsers_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Standard interface: __MIDL_itf_AllSrvModuleIDL_0326, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
    };

const CInterfaceProxyVtbl * _AllSrvModuleIDL_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IAdminGamesProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminSessionClassProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminServerProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminSessionEventsProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminSessionHostProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminUsersProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminUserProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminSessionProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminShipProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IAdminGameProxyVtbl,
    0
};

const CInterfaceStubVtbl * _AllSrvModuleIDL_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IAdminGamesStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminSessionClassStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminServerStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminSessionEventsStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminSessionHostStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminUsersStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminUserStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminSessionStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminShipStubVtbl,
    ( CInterfaceStubVtbl *) &_IAdminGameStubVtbl,
    0
};

PCInterfaceName const _AllSrvModuleIDL_InterfaceNamesList[] = 
{
    "IAdminGames",
    "IAdminSessionClass",
    "IAdminServer",
    "IAdminSessionEvents",
    "IAdminSessionHost",
    "IAdminUsers",
    "IAdminUser",
    "IAdminSession",
    "IAdminShip",
    "IAdminGame",
    0
};

const IID *  _AllSrvModuleIDL_BaseIIDList[] = 
{
    &IID_IAGCCollection,
    0,
    &IID_IDispatch,
    0,
    0,
    &IID_ITCCollection,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IAGCShip,
    &IID_IAGCGame,
    0
};


#define _AllSrvModuleIDL_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _AllSrvModuleIDL, pIID, n)

int __stdcall _AllSrvModuleIDL_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _AllSrvModuleIDL, 10, 8 )
    IID_BS_LOOKUP_NEXT_TEST( _AllSrvModuleIDL, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _AllSrvModuleIDL, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _AllSrvModuleIDL, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _AllSrvModuleIDL, 10, *pIndex )
    
}

const ExtendedProxyFileInfo AllSrvModuleIDL_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _AllSrvModuleIDL_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _AllSrvModuleIDL_StubVtblList,
    (const PCInterfaceName * ) & _AllSrvModuleIDL_InterfaceNamesList,
    (const IID ** ) & _AllSrvModuleIDL_BaseIIDList,
    & _AllSrvModuleIDL_IID_Lookup, 
    10,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

