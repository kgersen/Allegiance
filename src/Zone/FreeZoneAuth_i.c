
/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Tue Apr 27 17:41:47 2004
 */
/* Compiler settings for .\FreeZoneAuth.idl:
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


#ifdef __cplusplus
extern "C"{
#endif


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IZoneAuthSession,0x405D10E7,0x5351,0x40FD,0xA7,0x9B,0x1B,0x63,0xE8,0xE9,0x0F,0xB5);


MIDL_DEFINE_GUID(IID, IID_IZoneAuth,0xBAE1110C,0xFE12,0x4F43,0x9C,0xE8,0xE4,0xA0,0x34,0x5D,0x4D,0x64);


MIDL_DEFINE_GUID(IID, IID_IZoneAuthDecrypt,0xA1F5E58D,0x44F5,0x4043,0xBC,0x10,0x00,0x71,0x8D,0xC4,0x12,0x3E);


MIDL_DEFINE_GUID(IID, LIBID_FreeZoneAuthLib,0x21AD7186,0xD88D,0x46BE,0xA2,0xFF,0x77,0x98,0x29,0xC5,0xC6,0xF4);


MIDL_DEFINE_GUID(CLSID, CLSID_ZoneAuthSession,0xFE483871,0x59D5,0x4A66,0xA1,0x47,0xC8,0x5B,0x28,0xF3,0x18,0xE7);


MIDL_DEFINE_GUID(CLSID, CLSID_ZoneAuth,0x5A507BB4,0xFE82,0x4CB9,0x8B,0x99,0xAC,0x2C,0xF0,0x78,0x44,0xDD);


MIDL_DEFINE_GUID(CLSID, CLSID_ZoneAuthDecrypt,0x18B10393,0x4A80,0x48E6,0x98,0x28,0x3B,0x05,0x80,0xA6,0x5C,0xD5);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif

#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/
