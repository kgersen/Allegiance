

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __FreeZoneAuth_h__
#define __FreeZoneAuth_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IZoneAuthSession_FWD_DEFINED__
#define __IZoneAuthSession_FWD_DEFINED__
typedef interface IZoneAuthSession IZoneAuthSession;
#endif 	/* __IZoneAuthSession_FWD_DEFINED__ */


#ifndef __IZoneAuth_FWD_DEFINED__
#define __IZoneAuth_FWD_DEFINED__
typedef interface IZoneAuth IZoneAuth;
#endif 	/* __IZoneAuth_FWD_DEFINED__ */


#ifndef __IZoneAuthDecrypt_FWD_DEFINED__
#define __IZoneAuthDecrypt_FWD_DEFINED__
typedef interface IZoneAuthDecrypt IZoneAuthDecrypt;
#endif 	/* __IZoneAuthDecrypt_FWD_DEFINED__ */


#ifndef __ZoneAuthSession_FWD_DEFINED__
#define __ZoneAuthSession_FWD_DEFINED__

#ifdef __cplusplus
typedef class ZoneAuthSession ZoneAuthSession;
#else
typedef struct ZoneAuthSession ZoneAuthSession;
#endif /* __cplusplus */

#endif 	/* __ZoneAuthSession_FWD_DEFINED__ */


#ifndef __ZoneAuth_FWD_DEFINED__
#define __ZoneAuth_FWD_DEFINED__

#ifdef __cplusplus
typedef class ZoneAuth ZoneAuth;
#else
typedef struct ZoneAuth ZoneAuth;
#endif /* __cplusplus */

#endif 	/* __ZoneAuth_FWD_DEFINED__ */


#ifndef __ZoneAuthDecrypt_FWD_DEFINED__
#define __ZoneAuthDecrypt_FWD_DEFINED__

#ifdef __cplusplus
typedef class ZoneAuthDecrypt ZoneAuthDecrypt;
#else
typedef struct ZoneAuthDecrypt ZoneAuthDecrypt;
#endif /* __cplusplus */

#endif 	/* __ZoneAuthDecrypt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_FreeZoneAuth_0000 */
/* [local] */ 

typedef /* [public] */ 
enum __MIDL___MIDL_itf_FreeZoneAuth_0000_0001
    {	ZT_NO_ERROR	= 0,
	ZT_E_BUFFER_TOO_SMALL	= -1,
	ZT_E_AUTH_INVALID_TICKET	= -2,
	ZT_E_AUTH_DENIED	= -3,
	ZT_E_AUTH_NO_LOGIN_INFO	= -4,
	ZT_E_AUTH_TIMEOUT	= -5
    } 	FAZSTATUS;

typedef struct _ZONETICKET_TOKEN
    {
    LPSTR szToken;
    double secTokenStart;
    double secTokenEnd;
    } 	ZONETICKET_TOKEN;

typedef struct _ZONETICKET
    {
    int nVersion;
    int cbSize;
    long tTicketIssued;
    CHAR szUserName[ 25 ];
    CHAR szZoneServer[ 200 ];
    CHAR gGUID[ 1 ];
    unsigned long nAccountID;
    unsigned long nTokens;
    ZONETICKET_TOKEN *token_list;
    } 	ZONETICKET;

typedef struct _ZONETICKET_KEY
    {
    int nVersion;
    int cbSize;
    } 	ZONETICKET_KEY;



extern RPC_IF_HANDLE __MIDL_itf_FreeZoneAuth_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_FreeZoneAuth_0000_v0_0_s_ifspec;

#ifndef __IZoneAuthSession_INTERFACE_DEFINED__
#define __IZoneAuthSession_INTERFACE_DEFINED__

/* interface IZoneAuthSession */
/* [unique][helpstring][uuid][object] */ 

typedef /* [public][public] */ 
enum __MIDL_IZoneAuthSession_0002
    {	STATUS_COMPLETED	= 0,
	STATUS_UNKNOWN	= STATUS_COMPLETED + 1
    } 	STATUS;


EXTERN_C const IID IID_IZoneAuthSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("405D10E7-5351-40FD-A79B-1B63E8E90FB5")
    IZoneAuthSession : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Cancel( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetError( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetStatus( 
            /* [out] */ STATUS *status) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetTicket( 
            /* [out] */ BYTE *ppBuffer,
            /* [out] */ DWORD *pcbBuffer,
            /* [out] */ CHAR *szLogin,
            /* [out] */ DWORD *pcbLogin,
            LONG *uk1,
            LONG *uk2) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IZoneAuthSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZoneAuthSession * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZoneAuthSession * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZoneAuthSession * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Cancel )( 
            IZoneAuthSession * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetError )( 
            IZoneAuthSession * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetStatus )( 
            IZoneAuthSession * This,
            /* [out] */ STATUS *status);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetTicket )( 
            IZoneAuthSession * This,
            /* [out] */ BYTE *ppBuffer,
            /* [out] */ DWORD *pcbBuffer,
            /* [out] */ CHAR *szLogin,
            /* [out] */ DWORD *pcbLogin,
            LONG *uk1,
            LONG *uk2);
        
        END_INTERFACE
    } IZoneAuthSessionVtbl;

    interface IZoneAuthSession
    {
        CONST_VTBL struct IZoneAuthSessionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZoneAuthSession_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IZoneAuthSession_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IZoneAuthSession_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IZoneAuthSession_Cancel(This)	\
    (This)->lpVtbl -> Cancel(This)

#define IZoneAuthSession_GetError(This)	\
    (This)->lpVtbl -> GetError(This)

#define IZoneAuthSession_GetStatus(This,status)	\
    (This)->lpVtbl -> GetStatus(This,status)

#define IZoneAuthSession_GetTicket(This,ppBuffer,pcbBuffer,szLogin,pcbLogin,uk1,uk2)	\
    (This)->lpVtbl -> GetTicket(This,ppBuffer,pcbBuffer,szLogin,pcbLogin,uk1,uk2)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuthSession_Cancel_Proxy( 
    IZoneAuthSession * This);


void __RPC_STUB IZoneAuthSession_Cancel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuthSession_GetError_Proxy( 
    IZoneAuthSession * This);


void __RPC_STUB IZoneAuthSession_GetError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuthSession_GetStatus_Proxy( 
    IZoneAuthSession * This,
    /* [out] */ STATUS *status);


void __RPC_STUB IZoneAuthSession_GetStatus_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuthSession_GetTicket_Proxy( 
    IZoneAuthSession * This,
    /* [out] */ BYTE *ppBuffer,
    /* [out] */ DWORD *pcbBuffer,
    /* [out] */ CHAR *szLogin,
    /* [out] */ DWORD *pcbLogin,
    LONG *uk1,
    LONG *uk2);


void __RPC_STUB IZoneAuthSession_GetTicket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IZoneAuthSession_INTERFACE_DEFINED__ */


#ifndef __IZoneAuth_INTERFACE_DEFINED__
#define __IZoneAuth_INTERFACE_DEFINED__

/* interface IZoneAuth */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IZoneAuth;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("BAE1110C-FE12-4F43-9CE8-E4A0345D4D64")
    IZoneAuth : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Authenticate( 
            const CHAR *server,
            const CHAR *token_group,
            const GUID appguid,
            CHAR *szName,
            CHAR *szPW,
            BOOL fPWChanged,
            BOOL fRememberPW,
            ULONG *hFinished,
            /* [out] */ IZoneAuthSession **m_pzas) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE ReAuthenticate( 
            const CHAR *server,
            const CHAR *token_group,
            const GUID appguid,
            ULONG *hFinished,
            /* [out] */ IZoneAuthSession **m_pzas) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetLoginInfo( 
            /* [out] */ CHAR *szName,
            DWORD cbName,
            /* [out] */ CHAR *szPW,
            DWORD cbPW,
            /* [out] */ BOOL *pfRememberPW) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IZoneAuthVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZoneAuth * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZoneAuth * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZoneAuth * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Authenticate )( 
            IZoneAuth * This,
            const CHAR *server,
            const CHAR *token_group,
            const GUID appguid,
            CHAR *szName,
            CHAR *szPW,
            BOOL fPWChanged,
            BOOL fRememberPW,
            ULONG *hFinished,
            /* [out] */ IZoneAuthSession **m_pzas);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *ReAuthenticate )( 
            IZoneAuth * This,
            const CHAR *server,
            const CHAR *token_group,
            const GUID appguid,
            ULONG *hFinished,
            /* [out] */ IZoneAuthSession **m_pzas);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GetLoginInfo )( 
            IZoneAuth * This,
            /* [out] */ CHAR *szName,
            DWORD cbName,
            /* [out] */ CHAR *szPW,
            DWORD cbPW,
            /* [out] */ BOOL *pfRememberPW);
        
        END_INTERFACE
    } IZoneAuthVtbl;

    interface IZoneAuth
    {
        CONST_VTBL struct IZoneAuthVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZoneAuth_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IZoneAuth_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IZoneAuth_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IZoneAuth_Authenticate(This,server,token_group,appguid,szName,szPW,fPWChanged,fRememberPW,hFinished,m_pzas)	\
    (This)->lpVtbl -> Authenticate(This,server,token_group,appguid,szName,szPW,fPWChanged,fRememberPW,hFinished,m_pzas)

#define IZoneAuth_ReAuthenticate(This,server,token_group,appguid,hFinished,m_pzas)	\
    (This)->lpVtbl -> ReAuthenticate(This,server,token_group,appguid,hFinished,m_pzas)

#define IZoneAuth_GetLoginInfo(This,szName,cbName,szPW,cbPW,pfRememberPW)	\
    (This)->lpVtbl -> GetLoginInfo(This,szName,cbName,szPW,cbPW,pfRememberPW)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuth_Authenticate_Proxy( 
    IZoneAuth * This,
    const CHAR *server,
    const CHAR *token_group,
    const GUID appguid,
    CHAR *szName,
    CHAR *szPW,
    BOOL fPWChanged,
    BOOL fRememberPW,
    ULONG *hFinished,
    /* [out] */ IZoneAuthSession **m_pzas);


void __RPC_STUB IZoneAuth_Authenticate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuth_ReAuthenticate_Proxy( 
    IZoneAuth * This,
    const CHAR *server,
    const CHAR *token_group,
    const GUID appguid,
    ULONG *hFinished,
    /* [out] */ IZoneAuthSession **m_pzas);


void __RPC_STUB IZoneAuth_ReAuthenticate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuth_GetLoginInfo_Proxy( 
    IZoneAuth * This,
    /* [out] */ CHAR *szName,
    DWORD cbName,
    /* [out] */ CHAR *szPW,
    DWORD cbPW,
    /* [out] */ BOOL *pfRememberPW);


void __RPC_STUB IZoneAuth_GetLoginInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IZoneAuth_INTERFACE_DEFINED__ */


#ifndef __IZoneAuthDecrypt_INTERFACE_DEFINED__
#define __IZoneAuthDecrypt_INTERFACE_DEFINED__

/* interface IZoneAuthDecrypt */
/* [unique][nonextensible][oleautomation][dual][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IZoneAuthDecrypt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A1F5E58D-44F5-4043-BC10-00718DC4123E")
    IZoneAuthDecrypt : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GenerateKey( 
            CHAR *KeyName,
            /* [out] */ ZONETICKET_KEY *rKey) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE DecryptTicket( 
            ZONETICKET_KEY *key,
            BYTE *pbTicket,
            DWORD cbTicket,
            /* [out] */ ZONETICKET *rTicket,
            /* [out] */ DWORD *cbZT) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IZoneAuthDecryptVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IZoneAuthDecrypt * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IZoneAuthDecrypt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IZoneAuthDecrypt * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *GenerateKey )( 
            IZoneAuthDecrypt * This,
            CHAR *KeyName,
            /* [out] */ ZONETICKET_KEY *rKey);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *DecryptTicket )( 
            IZoneAuthDecrypt * This,
            ZONETICKET_KEY *key,
            BYTE *pbTicket,
            DWORD cbTicket,
            /* [out] */ ZONETICKET *rTicket,
            /* [out] */ DWORD *cbZT);
        
        END_INTERFACE
    } IZoneAuthDecryptVtbl;

    interface IZoneAuthDecrypt
    {
        CONST_VTBL struct IZoneAuthDecryptVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IZoneAuthDecrypt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IZoneAuthDecrypt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IZoneAuthDecrypt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IZoneAuthDecrypt_GenerateKey(This,KeyName,rKey)	\
    (This)->lpVtbl -> GenerateKey(This,KeyName,rKey)

#define IZoneAuthDecrypt_DecryptTicket(This,key,pbTicket,cbTicket,rTicket,cbZT)	\
    (This)->lpVtbl -> DecryptTicket(This,key,pbTicket,cbTicket,rTicket,cbZT)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuthDecrypt_GenerateKey_Proxy( 
    IZoneAuthDecrypt * This,
    CHAR *KeyName,
    /* [out] */ ZONETICKET_KEY *rKey);


void __RPC_STUB IZoneAuthDecrypt_GenerateKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IZoneAuthDecrypt_DecryptTicket_Proxy( 
    IZoneAuthDecrypt * This,
    ZONETICKET_KEY *key,
    BYTE *pbTicket,
    DWORD cbTicket,
    /* [out] */ ZONETICKET *rTicket,
    /* [out] */ DWORD *cbZT);


void __RPC_STUB IZoneAuthDecrypt_DecryptTicket_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IZoneAuthDecrypt_INTERFACE_DEFINED__ */



#ifndef __FreeZoneAuthLib_LIBRARY_DEFINED__
#define __FreeZoneAuthLib_LIBRARY_DEFINED__

/* library FreeZoneAuthLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_FreeZoneAuthLib;

EXTERN_C const CLSID CLSID_ZoneAuthSession;

#ifdef __cplusplus

class DECLSPEC_UUID("FE483871-59D5-4A66-A147-C85B28F318E7")
ZoneAuthSession;
#endif

EXTERN_C const CLSID CLSID_ZoneAuth;

#ifdef __cplusplus

class DECLSPEC_UUID("5A507BB4-FE82-4CB9-8B99-AC2CF07844DD")
ZoneAuth;
#endif

EXTERN_C const CLSID CLSID_ZoneAuthDecrypt;

#ifdef __cplusplus

class DECLSPEC_UUID("18B10393-4A80-48E6-9828-3B0580A65CD5")
ZoneAuthDecrypt;
#endif
#endif /* __FreeZoneAuthLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


