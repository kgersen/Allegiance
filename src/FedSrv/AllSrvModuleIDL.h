

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#ifndef __AllSrvModuleIDL_h__
#define __AllSrvModuleIDL_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAdminSession_FWD_DEFINED__
#define __IAdminSession_FWD_DEFINED__
typedef interface IAdminSession IAdminSession;
#endif 	/* __IAdminSession_FWD_DEFINED__ */


#ifndef __IAdminSessionHost_FWD_DEFINED__
#define __IAdminSessionHost_FWD_DEFINED__
typedef interface IAdminSessionHost IAdminSessionHost;
#endif 	/* __IAdminSessionHost_FWD_DEFINED__ */


#ifndef __IAdminSessionClass_FWD_DEFINED__
#define __IAdminSessionClass_FWD_DEFINED__
typedef interface IAdminSessionClass IAdminSessionClass;
#endif 	/* __IAdminSessionClass_FWD_DEFINED__ */


#ifndef __IAdminSessionEvents_FWD_DEFINED__
#define __IAdminSessionEvents_FWD_DEFINED__
typedef interface IAdminSessionEvents IAdminSessionEvents;
#endif 	/* __IAdminSessionEvents_FWD_DEFINED__ */


#ifndef ___IAdminSessionEvents_FWD_DEFINED__
#define ___IAdminSessionEvents_FWD_DEFINED__
typedef interface _IAdminSessionEvents _IAdminSessionEvents;
#endif 	/* ___IAdminSessionEvents_FWD_DEFINED__ */


#ifndef __IAdminShip_FWD_DEFINED__
#define __IAdminShip_FWD_DEFINED__
typedef interface IAdminShip IAdminShip;
#endif 	/* __IAdminShip_FWD_DEFINED__ */


#ifndef __IAdminGame_FWD_DEFINED__
#define __IAdminGame_FWD_DEFINED__
typedef interface IAdminGame IAdminGame;
#endif 	/* __IAdminGame_FWD_DEFINED__ */


#ifndef __IAdminGames_FWD_DEFINED__
#define __IAdminGames_FWD_DEFINED__
typedef interface IAdminGames IAdminGames;
#endif 	/* __IAdminGames_FWD_DEFINED__ */


#ifndef __IAdminServer_FWD_DEFINED__
#define __IAdminServer_FWD_DEFINED__
typedef interface IAdminServer IAdminServer;
#endif 	/* __IAdminServer_FWD_DEFINED__ */


#ifndef __IAdminUser_FWD_DEFINED__
#define __IAdminUser_FWD_DEFINED__
typedef interface IAdminUser IAdminUser;
#endif 	/* __IAdminUser_FWD_DEFINED__ */


#ifndef __IAdminUsers_FWD_DEFINED__
#define __IAdminUsers_FWD_DEFINED__
typedef interface IAdminUsers IAdminUsers;
#endif 	/* __IAdminUsers_FWD_DEFINED__ */


#ifndef __AdminInterfaces_FWD_DEFINED__
#define __AdminInterfaces_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminInterfaces AdminInterfaces;
#else
typedef struct AdminInterfaces AdminInterfaces;
#endif /* __cplusplus */

#endif 	/* __AdminInterfaces_FWD_DEFINED__ */


#ifndef __AdminSession_FWD_DEFINED__
#define __AdminSession_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminSession AdminSession;
#else
typedef struct AdminSession AdminSession;
#endif /* __cplusplus */

#endif 	/* __AdminSession_FWD_DEFINED__ */


#ifndef __AdminShip_FWD_DEFINED__
#define __AdminShip_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminShip AdminShip;
#else
typedef struct AdminShip AdminShip;
#endif /* __cplusplus */

#endif 	/* __AdminShip_FWD_DEFINED__ */


#ifndef __AdminGame_FWD_DEFINED__
#define __AdminGame_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminGame AdminGame;
#else
typedef struct AdminGame AdminGame;
#endif /* __cplusplus */

#endif 	/* __AdminGame_FWD_DEFINED__ */


#ifndef __AdminGames_FWD_DEFINED__
#define __AdminGames_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminGames AdminGames;
#else
typedef struct AdminGames AdminGames;
#endif /* __cplusplus */

#endif 	/* __AdminGames_FWD_DEFINED__ */


#ifndef __AdminServer_FWD_DEFINED__
#define __AdminServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminServer AdminServer;
#else
typedef struct AdminServer AdminServer;
#endif /* __cplusplus */

#endif 	/* __AdminServer_FWD_DEFINED__ */


#ifndef __AdminUser_FWD_DEFINED__
#define __AdminUser_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminUser AdminUser;
#else
typedef struct AdminUser AdminUser;
#endif /* __cplusplus */

#endif 	/* __AdminUser_FWD_DEFINED__ */


#ifndef __AdminUsers_FWD_DEFINED__
#define __AdminUsers_FWD_DEFINED__

#ifdef __cplusplus
typedef class AdminUsers AdminUsers;
#else
typedef struct AdminUsers AdminUsers;
#endif /* __cplusplus */

#endif 	/* __AdminUsers_FWD_DEFINED__ */


/* header files for imported files */
#include "AGCIDL.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_AllSrvModuleIDL_0000 */
/* [local] */ 


enum dispid_AdminIDL
    {	dispid_AdminIDL_Begin	= dispid_AGCIDL_End,
	dispid_GamesAdd	= dispid_AdminIDL_Begin + 1,
	dispid_SessionInfo	= dispid_GamesAdd + 1,
	dispid_Sessions	= dispid_SessionInfo + 1,
	dispid_SessionServer	= dispid_Sessions + 1,
	dispid_SessionGames	= dispid_SessionServer + 1,
	dispid_SessionRegisterForEvent	= dispid_SessionGames + 1,
	dispid_SessionUnregisterForEvent	= dispid_SessionRegisterForEvent + 1,
	dispid_EventActivated	= dispid_SessionUnregisterForEvent + 1,
	dispid_WhoStartedServer	= dispid_EventActivated + 1,
	dispid_ActivateAllEvents	= dispid_WhoStartedServer + 1,
	dispid_DeactivateAllEvents	= dispid_ActivateAllEvents + 1,
	dispid_PlayerCount	= dispid_DeactivateAllEvents + 1,
	dispid_MissionCount	= dispid_PlayerCount + 1,
	dispid_SendMsg	= dispid_MissionCount + 1,
	dispid_Users	= dispid_SendMsg + 1,
	dispid_Version	= dispid_Users + 1,
	dispid_Stop	= dispid_Version + 1,
	dispid_Pause	= dispid_Stop + 1,
	dispid_LookupUser	= dispid_Pause + 1,
	dispid_FindUser	= dispid_LookupUser + 1,
	dispid_ProcessID	= dispid_FindUser + 1,
	dispid_OnEvent	= dispid_ProcessID + 1,
	dispid_EventLog	= dispid_OnEvent + 1,
	dispid_MachineName	= dispid_EventLog + 1,
	dispid_PerfCounter1	= dispid_MachineName + 1,
	dispid_PerfCounter2	= dispid_PerfCounter1 + 1,
	dispid_PerfCounter3	= dispid_PerfCounter2 + 1,
	dispid_LobbyServer	= dispid_PerfCounter3 + 1,
	dispid_LobbyMode	= dispid_LobbyServer + 1,
	dispid_PerfCounters	= dispid_LobbyMode + 1,
	dispid_AdminIDL_End	= dispid_AdminIDL_Begin + 0x1000
    } ;












typedef /* [helpstring][uuid] */  DECLSPEC_UUID("4C3EB21E-2A97-11d3-8B66-00C04F681633") short AdminUserID;



extern RPC_IF_HANDLE __MIDL_itf_AllSrvModuleIDL_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AllSrvModuleIDL_0000_v0_0_s_ifspec;

#ifndef __IAdminSession_INTERFACE_DEFINED__
#define __IAdminSession_INTERFACE_DEFINED__

/* interface IAdminSession */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminSession;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8D9BE088-DDA4-11d2-8B46-00C04F681633")
    IAdminSession : public IDispatch
    {
    public:
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_SessionInfo( 
            /* [in] */ ITCSessionInfo *pSessionInfo) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SessionInfo( 
            /* [retval][out] */ ITCSessionInfo **ppSessionInfo) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SessionInfos( 
            /* [retval][out] */ ITCSessionInfos **ppSessionInfos) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Server( 
            /* [retval][out] */ IAdminServer **ppServer) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ActivateEvents( 
            AGCEventID AGCEvent,
            /* [defaultvalue][in] */ AGCUniqueID uniqueID = -1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeactivateEvents( 
            AGCEventID AGCEvent,
            /* [defaultvalue][in] */ AGCUniqueID uniqueObjectID = -1) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsEventActivated( 
            /* [in] */ AGCEventID AGCEvent,
            /* [defaultvalue][in] */ AGCUniqueID uniqueID,
            /* [retval][out] */ BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ActivateAllEvents( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DeactivateAllEvents( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ProcessID( 
            /* [retval][out] */ long *pdwProcessID) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ IAGCVersionInfo **ppVersion) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Pause( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_WhoStartedServer( 
            /* [retval][out] */ IAdminSession **pIAdminSession) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_EventLog( 
            /* [retval][out] */ IAGCEventLogger **ppEventLogger) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PerfCounters( 
            /* [retval][out] */ IAGCEvent **ppPerfCounters) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendAdminChat( 
            /* [in] */ BSTR bstrText,
            /* [in] */ long nUserID,
            /* [in] */ DATE dateOriginal) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Continue( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminSessionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminSession * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminSession * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminSession * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminSession * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminSession * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminSession * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminSession * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SessionInfo )( 
            IAdminSession * This,
            /* [in] */ ITCSessionInfo *pSessionInfo);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SessionInfo )( 
            IAdminSession * This,
            /* [retval][out] */ ITCSessionInfo **ppSessionInfo);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SessionInfos )( 
            IAdminSession * This,
            /* [retval][out] */ ITCSessionInfos **ppSessionInfos);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Server )( 
            IAdminSession * This,
            /* [retval][out] */ IAdminServer **ppServer);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ActivateEvents )( 
            IAdminSession * This,
            AGCEventID AGCEvent,
            /* [defaultvalue][in] */ AGCUniqueID uniqueID);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeactivateEvents )( 
            IAdminSession * This,
            AGCEventID AGCEvent,
            /* [defaultvalue][in] */ AGCUniqueID uniqueObjectID);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsEventActivated )( 
            IAdminSession * This,
            /* [in] */ AGCEventID AGCEvent,
            /* [defaultvalue][in] */ AGCUniqueID uniqueID,
            /* [retval][out] */ BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ActivateAllEvents )( 
            IAdminSession * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DeactivateAllEvents )( 
            IAdminSession * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ProcessID )( 
            IAdminSession * This,
            /* [retval][out] */ long *pdwProcessID);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IAdminSession * This,
            /* [retval][out] */ IAGCVersionInfo **ppVersion);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IAdminSession * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Pause )( 
            IAdminSession * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WhoStartedServer )( 
            IAdminSession * This,
            /* [retval][out] */ IAdminSession **pIAdminSession);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_EventLog )( 
            IAdminSession * This,
            /* [retval][out] */ IAGCEventLogger **ppEventLogger);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PerfCounters )( 
            IAdminSession * This,
            /* [retval][out] */ IAGCEvent **ppPerfCounters);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SendAdminChat )( 
            IAdminSession * This,
            /* [in] */ BSTR bstrText,
            /* [in] */ long nUserID,
            /* [in] */ DATE dateOriginal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Continue )( 
            IAdminSession * This);
        
        END_INTERFACE
    } IAdminSessionVtbl;

    interface IAdminSession
    {
        CONST_VTBL struct IAdminSessionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminSession_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminSession_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminSession_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminSession_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminSession_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminSession_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminSession_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminSession_put_SessionInfo(This,pSessionInfo)	\
    (This)->lpVtbl -> put_SessionInfo(This,pSessionInfo)

#define IAdminSession_get_SessionInfo(This,ppSessionInfo)	\
    (This)->lpVtbl -> get_SessionInfo(This,ppSessionInfo)

#define IAdminSession_get_SessionInfos(This,ppSessionInfos)	\
    (This)->lpVtbl -> get_SessionInfos(This,ppSessionInfos)

#define IAdminSession_get_Server(This,ppServer)	\
    (This)->lpVtbl -> get_Server(This,ppServer)

#define IAdminSession_ActivateEvents(This,AGCEvent,uniqueID)	\
    (This)->lpVtbl -> ActivateEvents(This,AGCEvent,uniqueID)

#define IAdminSession_DeactivateEvents(This,AGCEvent,uniqueObjectID)	\
    (This)->lpVtbl -> DeactivateEvents(This,AGCEvent,uniqueObjectID)

#define IAdminSession_get_IsEventActivated(This,AGCEvent,uniqueID,pVal)	\
    (This)->lpVtbl -> get_IsEventActivated(This,AGCEvent,uniqueID,pVal)

#define IAdminSession_ActivateAllEvents(This)	\
    (This)->lpVtbl -> ActivateAllEvents(This)

#define IAdminSession_DeactivateAllEvents(This)	\
    (This)->lpVtbl -> DeactivateAllEvents(This)

#define IAdminSession_get_ProcessID(This,pdwProcessID)	\
    (This)->lpVtbl -> get_ProcessID(This,pdwProcessID)

#define IAdminSession_get_Version(This,ppVersion)	\
    (This)->lpVtbl -> get_Version(This,ppVersion)

#define IAdminSession_Stop(This)	\
    (This)->lpVtbl -> Stop(This)

#define IAdminSession_Pause(This)	\
    (This)->lpVtbl -> Pause(This)

#define IAdminSession_get_WhoStartedServer(This,pIAdminSession)	\
    (This)->lpVtbl -> get_WhoStartedServer(This,pIAdminSession)

#define IAdminSession_get_EventLog(This,ppEventLogger)	\
    (This)->lpVtbl -> get_EventLog(This,ppEventLogger)

#define IAdminSession_get_PerfCounters(This,ppPerfCounters)	\
    (This)->lpVtbl -> get_PerfCounters(This,ppPerfCounters)

#define IAdminSession_SendAdminChat(This,bstrText,nUserID,dateOriginal)	\
    (This)->lpVtbl -> SendAdminChat(This,bstrText,nUserID,dateOriginal)

#define IAdminSession_Continue(This)	\
    (This)->lpVtbl -> Continue(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IAdminSession_put_SessionInfo_Proxy( 
    IAdminSession * This,
    /* [in] */ ITCSessionInfo *pSessionInfo);


void __RPC_STUB IAdminSession_put_SessionInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_SessionInfo_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ ITCSessionInfo **ppSessionInfo);


void __RPC_STUB IAdminSession_get_SessionInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_SessionInfos_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ ITCSessionInfos **ppSessionInfos);


void __RPC_STUB IAdminSession_get_SessionInfos_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_Server_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ IAdminServer **ppServer);


void __RPC_STUB IAdminSession_get_Server_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSession_ActivateEvents_Proxy( 
    IAdminSession * This,
    AGCEventID AGCEvent,
    /* [defaultvalue][in] */ AGCUniqueID uniqueID);


void __RPC_STUB IAdminSession_ActivateEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSession_DeactivateEvents_Proxy( 
    IAdminSession * This,
    AGCEventID AGCEvent,
    /* [defaultvalue][in] */ AGCUniqueID uniqueObjectID);


void __RPC_STUB IAdminSession_DeactivateEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_IsEventActivated_Proxy( 
    IAdminSession * This,
    /* [in] */ AGCEventID AGCEvent,
    /* [defaultvalue][in] */ AGCUniqueID uniqueID,
    /* [retval][out] */ BOOL *pVal);


void __RPC_STUB IAdminSession_get_IsEventActivated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSession_ActivateAllEvents_Proxy( 
    IAdminSession * This);


void __RPC_STUB IAdminSession_ActivateAllEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSession_DeactivateAllEvents_Proxy( 
    IAdminSession * This);


void __RPC_STUB IAdminSession_DeactivateAllEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_ProcessID_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ long *pdwProcessID);


void __RPC_STUB IAdminSession_get_ProcessID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_Version_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ IAGCVersionInfo **ppVersion);


void __RPC_STUB IAdminSession_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSession_Stop_Proxy( 
    IAdminSession * This);


void __RPC_STUB IAdminSession_Stop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSession_Pause_Proxy( 
    IAdminSession * This);


void __RPC_STUB IAdminSession_Pause_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_WhoStartedServer_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ IAdminSession **pIAdminSession);


void __RPC_STUB IAdminSession_get_WhoStartedServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_EventLog_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ IAGCEventLogger **ppEventLogger);


void __RPC_STUB IAdminSession_get_EventLog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminSession_get_PerfCounters_Proxy( 
    IAdminSession * This,
    /* [retval][out] */ IAGCEvent **ppPerfCounters);


void __RPC_STUB IAdminSession_get_PerfCounters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdminSession_SendAdminChat_Proxy( 
    IAdminSession * This,
    /* [in] */ BSTR bstrText,
    /* [in] */ long nUserID,
    /* [in] */ DATE dateOriginal);


void __RPC_STUB IAdminSession_SendAdminChat_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdminSession_Continue_Proxy( 
    IAdminSession * This);


void __RPC_STUB IAdminSession_Continue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminSession_INTERFACE_DEFINED__ */


#ifndef __IAdminSessionHost_INTERFACE_DEFINED__
#define __IAdminSessionHost_INTERFACE_DEFINED__

/* interface IAdminSessionHost */
/* [helpstring][uuid][unique][oleautomation][object] */ 


EXTERN_C const IID IID_IAdminSessionHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CD7D4875-3D61-4cc4-83C1-48F698ED45F5")
    IAdminSessionHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetIdentity( 
            /* [in] */ long nCookie,
            /* [retval][out] */ IUnknown **ppIdentity) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminSessionHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminSessionHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminSessionHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminSessionHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetIdentity )( 
            IAdminSessionHost * This,
            /* [in] */ long nCookie,
            /* [retval][out] */ IUnknown **ppIdentity);
        
        END_INTERFACE
    } IAdminSessionHostVtbl;

    interface IAdminSessionHost
    {
        CONST_VTBL struct IAdminSessionHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminSessionHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminSessionHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminSessionHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminSessionHost_GetIdentity(This,nCookie,ppIdentity)	\
    (This)->lpVtbl -> GetIdentity(This,nCookie,ppIdentity)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAdminSessionHost_GetIdentity_Proxy( 
    IAdminSessionHost * This,
    /* [in] */ long nCookie,
    /* [retval][out] */ IUnknown **ppIdentity);


void __RPC_STUB IAdminSessionHost_GetIdentity_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminSessionHost_INTERFACE_DEFINED__ */


#ifndef __IAdminSessionClass_INTERFACE_DEFINED__
#define __IAdminSessionClass_INTERFACE_DEFINED__

/* interface IAdminSessionClass */
/* [helpstring][uuid][unique][oleautomation][object] */ 


EXTERN_C const IID IID_IAdminSessionClass;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B3339C1B-D267-420b-B3D7-59F127CDD9CF")
    IAdminSessionClass : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateSession( 
            /* [in] */ IAdminSessionHost *pHost,
            /* [retval][out] */ IAdminSession **ppSession) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminSessionClassVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminSessionClass * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminSessionClass * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminSessionClass * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateSession )( 
            IAdminSessionClass * This,
            /* [in] */ IAdminSessionHost *pHost,
            /* [retval][out] */ IAdminSession **ppSession);
        
        END_INTERFACE
    } IAdminSessionClassVtbl;

    interface IAdminSessionClass
    {
        CONST_VTBL struct IAdminSessionClassVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminSessionClass_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminSessionClass_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminSessionClass_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminSessionClass_CreateSession(This,pHost,ppSession)	\
    (This)->lpVtbl -> CreateSession(This,pHost,ppSession)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdminSessionClass_CreateSession_Proxy( 
    IAdminSessionClass * This,
    /* [in] */ IAdminSessionHost *pHost,
    /* [retval][out] */ IAdminSession **ppSession);


void __RPC_STUB IAdminSessionClass_CreateSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminSessionClass_INTERFACE_DEFINED__ */


#ifndef __IAdminSessionEvents_INTERFACE_DEFINED__
#define __IAdminSessionEvents_INTERFACE_DEFINED__

/* interface IAdminSessionEvents */
/* [helpstring][uuid][nonextensible][unique][oleautomation][object] */ 


EXTERN_C const IID IID_IAdminSessionEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("DE3ED156-76A0-4a8e-8CFE-9ED26C3B0A5E")
    IAdminSessionEvents : public IUnknown
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OnEvent( 
            /* [in] */ IAGCEvent *pEvent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminSessionEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminSessionEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminSessionEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminSessionEvents * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OnEvent )( 
            IAdminSessionEvents * This,
            /* [in] */ IAGCEvent *pEvent);
        
        END_INTERFACE
    } IAdminSessionEventsVtbl;

    interface IAdminSessionEvents
    {
        CONST_VTBL struct IAdminSessionEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminSessionEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminSessionEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminSessionEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminSessionEvents_OnEvent(This,pEvent)	\
    (This)->lpVtbl -> OnEvent(This,pEvent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminSessionEvents_OnEvent_Proxy( 
    IAdminSessionEvents * This,
    /* [in] */ IAGCEvent *pEvent);


void __RPC_STUB IAdminSessionEvents_OnEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminSessionEvents_INTERFACE_DEFINED__ */


#ifndef __IAdminShip_INTERFACE_DEFINED__
#define __IAdminShip_INTERFACE_DEFINED__

/* interface IAdminShip */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminShip;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("986E689A-2DB4-11d3-8B66-00C04F681633")
    IAdminShip : public IAGCShip
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_User( 
            /* [retval][out] */ IAdminUser **ppUser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminShipVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminShip * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminShip * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminShip * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminShip * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminShip * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminShip * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminShip * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IAdminShip * This,
            /* [retval][out] */ BSTR *pbstrType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectType )( 
            IAdminShip * This,
            /* [retval][out] */ AGCObjectType *pObjectType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectID )( 
            IAdminShip * This,
            /* [retval][out] */ AGCObjectID *pObjectID);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Game )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCGame **ppGame);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UniqueID )( 
            IAdminShip * This,
            /* [retval][out] */ AGCUniqueID *pUniqueID);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsVisible )( 
            IAdminShip * This,
            /* [retval][out] */ VARIANT_BOOL *pbVisible);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsSeenBySide )( 
            IAdminShip * This,
            /* [in] */ IAGCTeam *pTeam,
            /* [retval][out] */ VARIANT_BOOL *pbSeen);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Position )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCVector **ppVector);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Velocity )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCVector **ppVector);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Orientation )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCOrientation **ppOrientation);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Radius )( 
            IAdminShip * This,
            /* [retval][out] */ float *pfRadius);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Team )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCTeam **ppTeam);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mass )( 
            IAdminShip * This,
            /* [retval][out] */ float *pfMass);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Sector )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCSector **ppSector);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Signature )( 
            IAdminShip * This,
            /* [retval][out] */ float *pfSignature);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IAdminShip * This,
            /* [retval][out] */ BSTR *pbstr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Fraction )( 
            IAdminShip * This,
            /* [retval][out] */ float *pfFraction);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_InScannerRange )( 
            IAdminShip * This,
            /* [in] */ IAGCModel *pModel,
            /* [retval][out] */ VARIANT_BOOL *pbInScannerRange);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CanSee )( 
            IAdminShip * This,
            /* [in] */ IAGCModel *pModel,
            /* [retval][out] */ VARIANT_BOOL *pbCanSee);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Ammo )( 
            IAdminShip * This,
            /* [in] */ short Val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Ammo )( 
            IAdminShip * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Fuel )( 
            IAdminShip * This,
            /* [in] */ float Val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Fuel )( 
            IAdminShip * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Energy )( 
            IAdminShip * This,
            /* [in] */ float Val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Energy )( 
            IAdminShip * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_WingID )( 
            IAdminShip * This,
            /* [in] */ short Val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_WingID )( 
            IAdminShip * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SendChat )( 
            IAdminShip * This,
            /* [in] */ BSTR bstrText,
            /* [defaultvalue][in] */ AGCSoundID idSound);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SendCommand )( 
            IAdminShip * This,
            /* [in] */ BSTR bstrCommand,
            /* [in] */ IAGCModel *pModelTarget,
            /* [defaultvalue][in] */ AGCSoundID idSound);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AutoDonate )( 
            IAdminShip * This,
            /* [in] */ IAGCShip *pShip);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AutoDonate )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCShip **ppShip);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShieldFraction )( 
            IAdminShip * This,
            /* [in] */ float pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShieldFraction )( 
            IAdminShip * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HullType )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCHullType **ppHullType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_BaseHullType )( 
            IAdminShip * This,
            /* [retval][out] */ IAGCHullType **ppHullType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_User )( 
            IAdminShip * This,
            /* [retval][out] */ IAdminUser **ppUser);
        
        END_INTERFACE
    } IAdminShipVtbl;

    interface IAdminShip
    {
        CONST_VTBL struct IAdminShipVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminShip_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminShip_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminShip_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminShip_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminShip_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminShip_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminShip_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminShip_get_Type(This,pbstrType)	\
    (This)->lpVtbl -> get_Type(This,pbstrType)


#define IAdminShip_get_ObjectType(This,pObjectType)	\
    (This)->lpVtbl -> get_ObjectType(This,pObjectType)

#define IAdminShip_get_ObjectID(This,pObjectID)	\
    (This)->lpVtbl -> get_ObjectID(This,pObjectID)

#define IAdminShip_get_Game(This,ppGame)	\
    (This)->lpVtbl -> get_Game(This,ppGame)

#define IAdminShip_get_UniqueID(This,pUniqueID)	\
    (This)->lpVtbl -> get_UniqueID(This,pUniqueID)


#define IAdminShip_get_IsVisible(This,pbVisible)	\
    (This)->lpVtbl -> get_IsVisible(This,pbVisible)

#define IAdminShip_get_IsSeenBySide(This,pTeam,pbSeen)	\
    (This)->lpVtbl -> get_IsSeenBySide(This,pTeam,pbSeen)

#define IAdminShip_get_Position(This,ppVector)	\
    (This)->lpVtbl -> get_Position(This,ppVector)

#define IAdminShip_get_Velocity(This,ppVector)	\
    (This)->lpVtbl -> get_Velocity(This,ppVector)

#define IAdminShip_get_Orientation(This,ppOrientation)	\
    (This)->lpVtbl -> get_Orientation(This,ppOrientation)

#define IAdminShip_get_Radius(This,pfRadius)	\
    (This)->lpVtbl -> get_Radius(This,pfRadius)

#define IAdminShip_get_Team(This,ppTeam)	\
    (This)->lpVtbl -> get_Team(This,ppTeam)

#define IAdminShip_get_Mass(This,pfMass)	\
    (This)->lpVtbl -> get_Mass(This,pfMass)

#define IAdminShip_get_Sector(This,ppSector)	\
    (This)->lpVtbl -> get_Sector(This,ppSector)

#define IAdminShip_get_Signature(This,pfSignature)	\
    (This)->lpVtbl -> get_Signature(This,pfSignature)

#define IAdminShip_get_Name(This,pbstr)	\
    (This)->lpVtbl -> get_Name(This,pbstr)


#define IAdminShip_get_Fraction(This,pfFraction)	\
    (This)->lpVtbl -> get_Fraction(This,pfFraction)


#define IAdminShip_get_InScannerRange(This,pModel,pbInScannerRange)	\
    (This)->lpVtbl -> get_InScannerRange(This,pModel,pbInScannerRange)

#define IAdminShip_get_CanSee(This,pModel,pbCanSee)	\
    (This)->lpVtbl -> get_CanSee(This,pModel,pbCanSee)


#define IAdminShip_put_Ammo(This,Val)	\
    (This)->lpVtbl -> put_Ammo(This,Val)

#define IAdminShip_get_Ammo(This,pVal)	\
    (This)->lpVtbl -> get_Ammo(This,pVal)

#define IAdminShip_put_Fuel(This,Val)	\
    (This)->lpVtbl -> put_Fuel(This,Val)

#define IAdminShip_get_Fuel(This,pVal)	\
    (This)->lpVtbl -> get_Fuel(This,pVal)

#define IAdminShip_put_Energy(This,Val)	\
    (This)->lpVtbl -> put_Energy(This,Val)

#define IAdminShip_get_Energy(This,pVal)	\
    (This)->lpVtbl -> get_Energy(This,pVal)

#define IAdminShip_put_WingID(This,Val)	\
    (This)->lpVtbl -> put_WingID(This,Val)

#define IAdminShip_get_WingID(This,pVal)	\
    (This)->lpVtbl -> get_WingID(This,pVal)

#define IAdminShip_SendChat(This,bstrText,idSound)	\
    (This)->lpVtbl -> SendChat(This,bstrText,idSound)

#define IAdminShip_SendCommand(This,bstrCommand,pModelTarget,idSound)	\
    (This)->lpVtbl -> SendCommand(This,bstrCommand,pModelTarget,idSound)

#define IAdminShip_put_AutoDonate(This,pShip)	\
    (This)->lpVtbl -> put_AutoDonate(This,pShip)

#define IAdminShip_get_AutoDonate(This,ppShip)	\
    (This)->lpVtbl -> get_AutoDonate(This,ppShip)

#define IAdminShip_put_ShieldFraction(This,pVal)	\
    (This)->lpVtbl -> put_ShieldFraction(This,pVal)

#define IAdminShip_get_ShieldFraction(This,pVal)	\
    (This)->lpVtbl -> get_ShieldFraction(This,pVal)

#define IAdminShip_get_HullType(This,ppHullType)	\
    (This)->lpVtbl -> get_HullType(This,ppHullType)

#define IAdminShip_get_BaseHullType(This,ppHullType)	\
    (This)->lpVtbl -> get_BaseHullType(This,ppHullType)


#define IAdminShip_get_User(This,ppUser)	\
    (This)->lpVtbl -> get_User(This,ppUser)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminShip_get_User_Proxy( 
    IAdminShip * This,
    /* [retval][out] */ IAdminUser **ppUser);


void __RPC_STUB IAdminShip_get_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminShip_INTERFACE_DEFINED__ */


#ifndef __IAdminGame_INTERFACE_DEFINED__
#define __IAdminGame_INTERFACE_DEFINED__

/* interface IAdminGame */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminGame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8FC514F8-E6CE-11D2-8B4B-00C04F681633")
    IAdminGame : public IAGCGame
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendMsg( 
            BSTR bstrMessage) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Kill( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Users( 
            /* [retval][out] */ IAdminUsers **ppUsers) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GameOwnerUser( 
            /* [retval][out] */ IAdminUser **ppUser) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartGame( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pbstrDescription) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartCountdown( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetTeamName( 
            /* [in] */ int iSideID,
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OverrideTechBit( 
            /* [in] */ int iSideID,
            /* [in] */ int nTechBitID,
            /* [in] */ BOOL bNewSetting) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDetailsFiles( 
            /* [in] */ BSTR bstrName) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RandomizeTeams( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminGameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminGame * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminGame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminGame * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminGame * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminGame * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminGame * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminGame * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IAdminGame * This,
            /* [retval][out] */ BSTR *pbstrType);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IAdminGame * This,
            /* [retval][out] */ BSTR *pbstr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Sectors )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCSectors **ppAGCSectors);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Teams )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCTeams **ppAGCTeams);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LookupShip )( 
            IAdminGame * This,
            /* [in] */ AGCUniqueID idAGC,
            /* [retval][out] */ IAGCShip **ppAGCObject);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Ships )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCShips **ppShips);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Alephs )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCAlephs **ppAlephs);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Asteroids )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCAsteroids **ppAsteroids);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GameParameters )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCGameParameters **ppParams);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GameID )( 
            IAdminGame * This,
            /* [retval][out] */ AGCGameID *pMissionId);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LookupTeam )( 
            IAdminGame * This,
            /* [in] */ AGCObjectID idAGC,
            /* [retval][out] */ IAGCTeam **ppAGCObject);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SendChat )( 
            IAdminGame * This,
            /* [in] */ BSTR bstrText,
            /* [defaultvalue][in] */ AGCSoundID idSound);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SendCommand )( 
            IAdminGame * This,
            /* [in] */ BSTR bstrCommand,
            /* [in] */ IAGCModel *pModelTarget,
            /* [defaultvalue][in] */ AGCSoundID idSound);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GameStage )( 
            IAdminGame * This,
            /* [retval][out] */ AGCGameStage *pStage);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Probes )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCProbes **ppProbes);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Buoys )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCModels **ppBuoys);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Treasures )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCModels **ppTreasures);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mines )( 
            IAdminGame * This,
            /* [retval][out] */ IAGCModels **ppMines);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ReplayCount )( 
            IAdminGame * This,
            /* [retval][out] */ short *pnReplays);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ContextName )( 
            IAdminGame * This,
            /* [retval][out] */ BSTR *pbstrContextName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendMsg )( 
            IAdminGame * This,
            BSTR bstrMessage);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Kill )( 
            IAdminGame * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Users )( 
            IAdminGame * This,
            /* [retval][out] */ IAdminUsers **ppUsers);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GameOwnerUser )( 
            IAdminGame * This,
            /* [retval][out] */ IAdminUser **ppUser);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartGame )( 
            IAdminGame * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IAdminGame * This,
            /* [retval][out] */ BSTR *pbstrDescription);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StartCountdown )( 
            IAdminGame * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetTeamName )( 
            IAdminGame * This,
            /* [in] */ int iSideID,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OverrideTechBit )( 
            IAdminGame * This,
            /* [in] */ int iSideID,
            /* [in] */ int nTechBitID,
            /* [in] */ BOOL bNewSetting);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDetailsFiles )( 
            IAdminGame * This,
            /* [in] */ BSTR bstrName);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RandomizeTeams )( 
            IAdminGame * This);
        
        END_INTERFACE
    } IAdminGameVtbl;

    interface IAdminGame
    {
        CONST_VTBL struct IAdminGameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminGame_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminGame_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminGame_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminGame_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminGame_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminGame_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminGame_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminGame_get_Type(This,pbstrType)	\
    (This)->lpVtbl -> get_Type(This,pbstrType)


#define IAdminGame_get_Name(This,pbstr)	\
    (This)->lpVtbl -> get_Name(This,pbstr)

#define IAdminGame_get_Sectors(This,ppAGCSectors)	\
    (This)->lpVtbl -> get_Sectors(This,ppAGCSectors)

#define IAdminGame_get_Teams(This,ppAGCTeams)	\
    (This)->lpVtbl -> get_Teams(This,ppAGCTeams)

#define IAdminGame_get_LookupShip(This,idAGC,ppAGCObject)	\
    (This)->lpVtbl -> get_LookupShip(This,idAGC,ppAGCObject)

#define IAdminGame_get_Ships(This,ppShips)	\
    (This)->lpVtbl -> get_Ships(This,ppShips)

#define IAdminGame_get_Alephs(This,ppAlephs)	\
    (This)->lpVtbl -> get_Alephs(This,ppAlephs)

#define IAdminGame_get_Asteroids(This,ppAsteroids)	\
    (This)->lpVtbl -> get_Asteroids(This,ppAsteroids)

#define IAdminGame_get_GameParameters(This,ppParams)	\
    (This)->lpVtbl -> get_GameParameters(This,ppParams)

#define IAdminGame_get_GameID(This,pMissionId)	\
    (This)->lpVtbl -> get_GameID(This,pMissionId)

#define IAdminGame_get_LookupTeam(This,idAGC,ppAGCObject)	\
    (This)->lpVtbl -> get_LookupTeam(This,idAGC,ppAGCObject)

#define IAdminGame_SendChat(This,bstrText,idSound)	\
    (This)->lpVtbl -> SendChat(This,bstrText,idSound)

#define IAdminGame_SendCommand(This,bstrCommand,pModelTarget,idSound)	\
    (This)->lpVtbl -> SendCommand(This,bstrCommand,pModelTarget,idSound)

#define IAdminGame_get_GameStage(This,pStage)	\
    (This)->lpVtbl -> get_GameStage(This,pStage)

#define IAdminGame_get_Probes(This,ppProbes)	\
    (This)->lpVtbl -> get_Probes(This,ppProbes)

#define IAdminGame_get_Buoys(This,ppBuoys)	\
    (This)->lpVtbl -> get_Buoys(This,ppBuoys)

#define IAdminGame_get_Treasures(This,ppTreasures)	\
    (This)->lpVtbl -> get_Treasures(This,ppTreasures)

#define IAdminGame_get_Mines(This,ppMines)	\
    (This)->lpVtbl -> get_Mines(This,ppMines)

#define IAdminGame_get_ReplayCount(This,pnReplays)	\
    (This)->lpVtbl -> get_ReplayCount(This,pnReplays)

#define IAdminGame_get_ContextName(This,pbstrContextName)	\
    (This)->lpVtbl -> get_ContextName(This,pbstrContextName)


#define IAdminGame_SendMsg(This,bstrMessage)	\
    (This)->lpVtbl -> SendMsg(This,bstrMessage)

#define IAdminGame_Kill(This)	\
    (This)->lpVtbl -> Kill(This)

#define IAdminGame_get_Users(This,ppUsers)	\
    (This)->lpVtbl -> get_Users(This,ppUsers)

#define IAdminGame_get_GameOwnerUser(This,ppUser)	\
    (This)->lpVtbl -> get_GameOwnerUser(This,ppUser)

#define IAdminGame_StartGame(This)	\
    (This)->lpVtbl -> StartGame(This)

#define IAdminGame_get_Description(This,pbstrDescription)	\
    (This)->lpVtbl -> get_Description(This,pbstrDescription)

#define IAdminGame_StartCountdown(This)	\
    (This)->lpVtbl -> StartCountdown(This)

#define IAdminGame_SetTeamName(This,iSideID,bstrName)	\
    (This)->lpVtbl -> SetTeamName(This,iSideID,bstrName)

#define IAdminGame_OverrideTechBit(This,iSideID,nTechBitID,bNewSetting)	\
    (This)->lpVtbl -> OverrideTechBit(This,iSideID,nTechBitID,bNewSetting)

#define IAdminGame_SetDetailsFiles(This,bstrName)	\
    (This)->lpVtbl -> SetDetailsFiles(This,bstrName)

#define IAdminGame_RandomizeTeams(This)	\
    (This)->lpVtbl -> RandomizeTeams(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_SendMsg_Proxy( 
    IAdminGame * This,
    BSTR bstrMessage);


void __RPC_STUB IAdminGame_SendMsg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_Kill_Proxy( 
    IAdminGame * This);


void __RPC_STUB IAdminGame_Kill_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminGame_get_Users_Proxy( 
    IAdminGame * This,
    /* [retval][out] */ IAdminUsers **ppUsers);


void __RPC_STUB IAdminGame_get_Users_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminGame_get_GameOwnerUser_Proxy( 
    IAdminGame * This,
    /* [retval][out] */ IAdminUser **ppUser);


void __RPC_STUB IAdminGame_get_GameOwnerUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_StartGame_Proxy( 
    IAdminGame * This);


void __RPC_STUB IAdminGame_StartGame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminGame_get_Description_Proxy( 
    IAdminGame * This,
    /* [retval][out] */ BSTR *pbstrDescription);


void __RPC_STUB IAdminGame_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_StartCountdown_Proxy( 
    IAdminGame * This);


void __RPC_STUB IAdminGame_StartCountdown_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_SetTeamName_Proxy( 
    IAdminGame * This,
    /* [in] */ int iSideID,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IAdminGame_SetTeamName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_OverrideTechBit_Proxy( 
    IAdminGame * This,
    /* [in] */ int iSideID,
    /* [in] */ int nTechBitID,
    /* [in] */ BOOL bNewSetting);


void __RPC_STUB IAdminGame_OverrideTechBit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_SetDetailsFiles_Proxy( 
    IAdminGame * This,
    /* [in] */ BSTR bstrName);


void __RPC_STUB IAdminGame_SetDetailsFiles_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGame_RandomizeTeams_Proxy( 
    IAdminGame * This);


void __RPC_STUB IAdminGame_RandomizeTeams_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminGame_INTERFACE_DEFINED__ */


#ifndef __IAdminGames_INTERFACE_DEFINED__
#define __IAdminGames_INTERFACE_DEFINED__

/* interface IAdminGames */
/* [helpstring][uuid][unique][dual][object] */ 


EXTERN_C const IID IID_IAdminGames;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3ACE4410-E6D3-11D2-8B4B-00C04F681633")
    IAdminGames : public IAGCCollection
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT *pvIndex,
            /* [retval][out] */ IAdminGame **ppGame) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            IAGCGameParameters *pGameParameters) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminGamesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminGames * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminGames * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminGames * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminGames * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminGames * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminGames * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminGames * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IAdminGames * This,
            /* [retval][out] */ BSTR *pbstrType);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IAdminGames * This,
            /* [retval][out] */ long *pnCount);
        
        /* [helpstring][id][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IAdminGames * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IAdminGames * This,
            /* [in] */ VARIANT *pvIndex,
            /* [retval][out] */ IAdminGame **ppGame);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IAdminGames * This,
            IAGCGameParameters *pGameParameters);
        
        END_INTERFACE
    } IAdminGamesVtbl;

    interface IAdminGames
    {
        CONST_VTBL struct IAdminGamesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminGames_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminGames_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminGames_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminGames_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminGames_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminGames_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminGames_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminGames_get_Type(This,pbstrType)	\
    (This)->lpVtbl -> get_Type(This,pbstrType)


#define IAdminGames_get_Count(This,pnCount)	\
    (This)->lpVtbl -> get_Count(This,pnCount)

#define IAdminGames_get__NewEnum(This,ppunkEnum)	\
    (This)->lpVtbl -> get__NewEnum(This,ppunkEnum)


#define IAdminGames_get_Item(This,pvIndex,ppGame)	\
    (This)->lpVtbl -> get_Item(This,pvIndex,ppGame)

#define IAdminGames_Add(This,pGameParameters)	\
    (This)->lpVtbl -> Add(This,pGameParameters)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminGames_get_Item_Proxy( 
    IAdminGames * This,
    /* [in] */ VARIANT *pvIndex,
    /* [retval][out] */ IAdminGame **ppGame);


void __RPC_STUB IAdminGames_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminGames_Add_Proxy( 
    IAdminGames * This,
    IAGCGameParameters *pGameParameters);


void __RPC_STUB IAdminGames_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminGames_INTERFACE_DEFINED__ */


#ifndef __IAdminServer_INTERFACE_DEFINED__
#define __IAdminServer_INTERFACE_DEFINED__

/* interface IAdminServer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("20721450-E7C0-11D2-8B4B-00C04F681633")
    IAdminServer : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Games( 
            /* [retval][out] */ IAdminGames **ppGames) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlayerCount( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MissionCount( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendMsg( 
            BSTR bstrMessage) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Users( 
            /* [retval][out] */ IAdminUsers **ppUsers) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LookupUser( 
            /* [in] */ AGCUniqueID id,
            /* [retval][out] */ IAdminUser **ppUser) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FindUser( 
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IAdminUser **ppUser) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MachineName( 
            /* [retval][out] */ BSTR *pbstrMachineName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PacketsIn( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlayersOnline( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TimeInnerLoop( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_LobbyServer( 
            /* [in] */ BSTR bstrLobbyServer) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LobbyServer( 
            /* [retval][out] */ BSTR *pbstrLobbyServer) = 0;
        
        virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_PublicLobby( 
            /* [in] */ VARIANT_BOOL bPublic) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublicLobby( 
            /* [retval][out] */ VARIANT_BOOL *pbPublic) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateDefaultGames( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminServer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Games )( 
            IAdminServer * This,
            /* [retval][out] */ IAdminGames **ppGames);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlayerCount )( 
            IAdminServer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MissionCount )( 
            IAdminServer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendMsg )( 
            IAdminServer * This,
            BSTR bstrMessage);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Users )( 
            IAdminServer * This,
            /* [retval][out] */ IAdminUsers **ppUsers);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LookupUser )( 
            IAdminServer * This,
            /* [in] */ AGCUniqueID id,
            /* [retval][out] */ IAdminUser **ppUser);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FindUser )( 
            IAdminServer * This,
            /* [in] */ BSTR bstrName,
            /* [retval][out] */ IAdminUser **ppUser);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MachineName )( 
            IAdminServer * This,
            /* [retval][out] */ BSTR *pbstrMachineName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PacketsIn )( 
            IAdminServer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlayersOnline )( 
            IAdminServer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TimeInnerLoop )( 
            IAdminServer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LobbyServer )( 
            IAdminServer * This,
            /* [in] */ BSTR bstrLobbyServer);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LobbyServer )( 
            IAdminServer * This,
            /* [retval][out] */ BSTR *pbstrLobbyServer);
        
        /* [helpstring][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PublicLobby )( 
            IAdminServer * This,
            /* [in] */ VARIANT_BOOL bPublic);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PublicLobby )( 
            IAdminServer * This,
            /* [retval][out] */ VARIANT_BOOL *pbPublic);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *CreateDefaultGames )( 
            IAdminServer * This);
        
        END_INTERFACE
    } IAdminServerVtbl;

    interface IAdminServer
    {
        CONST_VTBL struct IAdminServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminServer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminServer_get_Games(This,ppGames)	\
    (This)->lpVtbl -> get_Games(This,ppGames)

#define IAdminServer_get_PlayerCount(This,pVal)	\
    (This)->lpVtbl -> get_PlayerCount(This,pVal)

#define IAdminServer_get_MissionCount(This,pVal)	\
    (This)->lpVtbl -> get_MissionCount(This,pVal)

#define IAdminServer_SendMsg(This,bstrMessage)	\
    (This)->lpVtbl -> SendMsg(This,bstrMessage)

#define IAdminServer_get_Users(This,ppUsers)	\
    (This)->lpVtbl -> get_Users(This,ppUsers)

#define IAdminServer_get_LookupUser(This,id,ppUser)	\
    (This)->lpVtbl -> get_LookupUser(This,id,ppUser)

#define IAdminServer_get_FindUser(This,bstrName,ppUser)	\
    (This)->lpVtbl -> get_FindUser(This,bstrName,ppUser)

#define IAdminServer_get_MachineName(This,pbstrMachineName)	\
    (This)->lpVtbl -> get_MachineName(This,pbstrMachineName)

#define IAdminServer_get_PacketsIn(This,pVal)	\
    (This)->lpVtbl -> get_PacketsIn(This,pVal)

#define IAdminServer_get_PlayersOnline(This,pVal)	\
    (This)->lpVtbl -> get_PlayersOnline(This,pVal)

#define IAdminServer_get_TimeInnerLoop(This,pVal)	\
    (This)->lpVtbl -> get_TimeInnerLoop(This,pVal)

#define IAdminServer_put_LobbyServer(This,bstrLobbyServer)	\
    (This)->lpVtbl -> put_LobbyServer(This,bstrLobbyServer)

#define IAdminServer_get_LobbyServer(This,pbstrLobbyServer)	\
    (This)->lpVtbl -> get_LobbyServer(This,pbstrLobbyServer)

#define IAdminServer_put_PublicLobby(This,bPublic)	\
    (This)->lpVtbl -> put_PublicLobby(This,bPublic)

#define IAdminServer_get_PublicLobby(This,pbPublic)	\
    (This)->lpVtbl -> get_PublicLobby(This,pbPublic)

#define IAdminServer_CreateDefaultGames(This)	\
    (This)->lpVtbl -> CreateDefaultGames(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_Games_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ IAdminGames **ppGames);


void __RPC_STUB IAdminServer_get_Games_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_PlayerCount_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IAdminServer_get_PlayerCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_MissionCount_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IAdminServer_get_MissionCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdminServer_SendMsg_Proxy( 
    IAdminServer * This,
    BSTR bstrMessage);


void __RPC_STUB IAdminServer_SendMsg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_Users_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ IAdminUsers **ppUsers);


void __RPC_STUB IAdminServer_get_Users_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_LookupUser_Proxy( 
    IAdminServer * This,
    /* [in] */ AGCUniqueID id,
    /* [retval][out] */ IAdminUser **ppUser);


void __RPC_STUB IAdminServer_get_LookupUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_FindUser_Proxy( 
    IAdminServer * This,
    /* [in] */ BSTR bstrName,
    /* [retval][out] */ IAdminUser **ppUser);


void __RPC_STUB IAdminServer_get_FindUser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_MachineName_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ BSTR *pbstrMachineName);


void __RPC_STUB IAdminServer_get_MachineName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_PacketsIn_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IAdminServer_get_PacketsIn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_PlayersOnline_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IAdminServer_get_PlayersOnline_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_TimeInnerLoop_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IAdminServer_get_TimeInnerLoop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IAdminServer_put_LobbyServer_Proxy( 
    IAdminServer * This,
    /* [in] */ BSTR bstrLobbyServer);


void __RPC_STUB IAdminServer_put_LobbyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_LobbyServer_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ BSTR *pbstrLobbyServer);


void __RPC_STUB IAdminServer_get_LobbyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IAdminServer_put_PublicLobby_Proxy( 
    IAdminServer * This,
    /* [in] */ VARIANT_BOOL bPublic);


void __RPC_STUB IAdminServer_put_PublicLobby_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IAdminServer_get_PublicLobby_Proxy( 
    IAdminServer * This,
    /* [retval][out] */ VARIANT_BOOL *pbPublic);


void __RPC_STUB IAdminServer_get_PublicLobby_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdminServer_CreateDefaultGames_Proxy( 
    IAdminServer * This);


void __RPC_STUB IAdminServer_CreateDefaultGames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminServer_INTERFACE_DEFINED__ */


#ifndef __IAdminUser_INTERFACE_DEFINED__
#define __IAdminUser_INTERFACE_DEFINED__

/* interface IAdminUser */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminUser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D5C9AD80-EBA4-11D2-8B4B-00C04F681633")
    IAdminUser : public IDispatch
    {
    public:
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pbstr) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SendMsg( 
            BSTR bstrMessage) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_isReady( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_isReady( 
            /* [in] */ VARIANT_BOOL Val) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Ship( 
            /* [retval][out] */ IAdminShip **ppAdminShip) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Boot( void) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_UserID( 
            /* [retval][out] */ AdminUserID *userid) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_UniqueID( 
            /* [retval][out] */ AGCUniqueID *uniqueID) = 0;
        
        virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_PlayerStats( 
            /* [retval][out] */ IAGCEvent **ppStats) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminUserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminUser * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminUser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminUser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminUser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminUser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminUser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminUser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IAdminUser * This,
            /* [retval][out] */ BSTR *pbstr);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *SendMsg )( 
            IAdminUser * This,
            BSTR bstrMessage);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_isReady )( 
            IAdminUser * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_isReady )( 
            IAdminUser * This,
            /* [in] */ VARIANT_BOOL Val);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Ship )( 
            IAdminUser * This,
            /* [retval][out] */ IAdminShip **ppAdminShip);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE *Boot )( 
            IAdminUser * This);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UserID )( 
            IAdminUser * This,
            /* [retval][out] */ AdminUserID *userid);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UniqueID )( 
            IAdminUser * This,
            /* [retval][out] */ AGCUniqueID *uniqueID);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlayerStats )( 
            IAdminUser * This,
            /* [retval][out] */ IAGCEvent **ppStats);
        
        END_INTERFACE
    } IAdminUserVtbl;

    interface IAdminUser
    {
        CONST_VTBL struct IAdminUserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminUser_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminUser_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminUser_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminUser_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminUser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminUser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminUser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminUser_get_Name(This,pbstr)	\
    (This)->lpVtbl -> get_Name(This,pbstr)

#define IAdminUser_SendMsg(This,bstrMessage)	\
    (This)->lpVtbl -> SendMsg(This,bstrMessage)

#define IAdminUser_get_isReady(This,pVal)	\
    (This)->lpVtbl -> get_isReady(This,pVal)

#define IAdminUser_put_isReady(This,Val)	\
    (This)->lpVtbl -> put_isReady(This,Val)

#define IAdminUser_get_Ship(This,ppAdminShip)	\
    (This)->lpVtbl -> get_Ship(This,ppAdminShip)

#define IAdminUser_Boot(This)	\
    (This)->lpVtbl -> Boot(This)

#define IAdminUser_get_UserID(This,userid)	\
    (This)->lpVtbl -> get_UserID(This,userid)

#define IAdminUser_get_UniqueID(This,uniqueID)	\
    (This)->lpVtbl -> get_UniqueID(This,uniqueID)

#define IAdminUser_get_PlayerStats(This,ppStats)	\
    (This)->lpVtbl -> get_PlayerStats(This,ppStats)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAdminUser_get_Name_Proxy( 
    IAdminUser * This,
    /* [retval][out] */ BSTR *pbstr);


void __RPC_STUB IAdminUser_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdminUser_SendMsg_Proxy( 
    IAdminUser * This,
    BSTR bstrMessage);


void __RPC_STUB IAdminUser_SendMsg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAdminUser_get_isReady_Proxy( 
    IAdminUser * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IAdminUser_get_isReady_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IAdminUser_put_isReady_Proxy( 
    IAdminUser * This,
    /* [in] */ VARIANT_BOOL Val);


void __RPC_STUB IAdminUser_put_isReady_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAdminUser_get_Ship_Proxy( 
    IAdminUser * This,
    /* [retval][out] */ IAdminShip **ppAdminShip);


void __RPC_STUB IAdminUser_get_Ship_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdminUser_Boot_Proxy( 
    IAdminUser * This);


void __RPC_STUB IAdminUser_Boot_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAdminUser_get_UserID_Proxy( 
    IAdminUser * This,
    /* [retval][out] */ AdminUserID *userid);


void __RPC_STUB IAdminUser_get_UserID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAdminUser_get_UniqueID_Proxy( 
    IAdminUser * This,
    /* [retval][out] */ AGCUniqueID *uniqueID);


void __RPC_STUB IAdminUser_get_UniqueID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IAdminUser_get_PlayerStats_Proxy( 
    IAdminUser * This,
    /* [retval][out] */ IAGCEvent **ppStats);


void __RPC_STUB IAdminUser_get_PlayerStats_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminUser_INTERFACE_DEFINED__ */


#ifndef __IAdminUsers_INTERFACE_DEFINED__
#define __IAdminUsers_INTERFACE_DEFINED__

/* interface IAdminUsers */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAdminUsers;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F9E52A79-EBA4-11D2-8B4B-00C04F681633")
    IAdminUsers : public ITCCollection
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT index,
            /* [retval][out] */ IAdminUser **ppUser) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAdminUsersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAdminUsers * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAdminUsers * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAdminUsers * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAdminUsers * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAdminUsers * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAdminUsers * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAdminUsers * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IAdminUsers * This,
            /* [retval][out] */ long *pnCount);
        
        /* [helpstring][id][restricted][hidden][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IAdminUsers * This,
            /* [retval][out] */ IUnknown **ppunkEnum);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IAdminUsers * This,
            /* [in] */ VARIANT index,
            /* [retval][out] */ IAdminUser **ppUser);
        
        END_INTERFACE
    } IAdminUsersVtbl;

    interface IAdminUsers
    {
        CONST_VTBL struct IAdminUsersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAdminUsers_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdminUsers_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAdminUsers_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAdminUsers_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdminUsers_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdminUsers_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdminUsers_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdminUsers_get_Count(This,pnCount)	\
    (This)->lpVtbl -> get_Count(This,pnCount)

#define IAdminUsers_get__NewEnum(This,ppunkEnum)	\
    (This)->lpVtbl -> get__NewEnum(This,ppunkEnum)


#define IAdminUsers_get_Item(This,index,ppUser)	\
    (This)->lpVtbl -> get_Item(This,index,ppUser)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAdminUsers_get_Item_Proxy( 
    IAdminUsers * This,
    /* [in] */ VARIANT index,
    /* [retval][out] */ IAdminUser **ppUser);


void __RPC_STUB IAdminUsers_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAdminUsers_INTERFACE_DEFINED__ */



#ifndef __ALLEGIANCESERVERLib_LIBRARY_DEFINED__
#define __ALLEGIANCESERVERLib_LIBRARY_DEFINED__

/* library ALLEGIANCESERVERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_ALLEGIANCESERVERLib;

EXTERN_C const CLSID CLSID_AdminInterfaces;

#ifdef __cplusplus

class DECLSPEC_UUID("8A1E3F7F-7A21-4fbf-9B39-D02CDB0D5402")
AdminInterfaces;
#endif

EXTERN_C const CLSID CLSID_AdminSession;

#ifdef __cplusplus

class DECLSPEC_UUID("AAA8270F-DDA4-11d2-8B46-00C04F681633")
AdminSession;
#endif

EXTERN_C const CLSID CLSID_AdminShip;

#ifdef __cplusplus

class DECLSPEC_UUID("3E389806-2DB4-11d3-8B66-00C04F681633")
AdminShip;
#endif

EXTERN_C const CLSID CLSID_AdminGame;

#ifdef __cplusplus

class DECLSPEC_UUID("9F17114A-E6CE-11D2-8B4B-00C04F681633")
AdminGame;
#endif

EXTERN_C const CLSID CLSID_AdminGames;

#ifdef __cplusplus

class DECLSPEC_UUID("67757064-E6FC-11D2-8B4B-00C04F681633")
AdminGames;
#endif

EXTERN_C const CLSID CLSID_AdminServer;

#ifdef __cplusplus

class DECLSPEC_UUID("5B5ED2A6-E7C0-11D2-8B4B-00C04F681633")
AdminServer;
#endif

EXTERN_C const CLSID CLSID_AdminUser;

#ifdef __cplusplus

class DECLSPEC_UUID("E71EA5B8-EBA4-11D2-8B4B-00C04F681633")
AdminUser;
#endif

EXTERN_C const CLSID CLSID_AdminUsers;

#ifdef __cplusplus

class DECLSPEC_UUID("0BD51B94-EBA5-11D2-8B4B-00C04F681633")
AdminUsers;
#endif
#endif /* __ALLEGIANCESERVERLib_LIBRARY_DEFINED__ */

/* interface __MIDL_itf_AllSrvModuleIDL_0326 */
/* [local] */ 

#if defined(__cplusplus) && _MSC_VER >= 1100
  extern "C++"
  {
    /////////////////////////////////////////////////////////////////////////
    // Smart Pointer Declarations

    #include <comdef.h>

    #ifndef TC_COM_SMARTPTR_TYPEDEF
      #define TC_COM_SMARTPTR_TYPEDEF(Interface) \
        _COM_SMARTPTR_TYPEDEF(Interface, __uuidof(Interface))
    #endif // TC_COM_SMARTPTR_TYPEDEF

    TC_COM_SMARTPTR_TYPEDEF(IAdminSession);
    TC_COM_SMARTPTR_TYPEDEF(IAdminSessionEvents);
    TC_COM_SMARTPTR_TYPEDEF(IAdminSessionHost);
    TC_COM_SMARTPTR_TYPEDEF(IAdminSessionClass);
    TC_COM_SMARTPTR_TYPEDEF(IAdminServer);
    TC_COM_SMARTPTR_TYPEDEF(IAdminShip);
    TC_COM_SMARTPTR_TYPEDEF(IAdminGame);
    TC_COM_SMARTPTR_TYPEDEF(IAdminGames);
    TC_COM_SMARTPTR_TYPEDEF(IAdminServer);
    TC_COM_SMARTPTR_TYPEDEF(IAdminUser);
    TC_COM_SMARTPTR_TYPEDEF(IAdminUsers);

  } // extern "C++"
#endif // defined(__cplusplus) && _MSC_VER >= 1100

/////////////////////////////////////////////////////////////////////////////
// AllSrv Existence
#define szAllSrvRunning TEXT("{5B5ED2A6-E7C0-11D2-8B4B-00C04F681633}_Running")
#define szAllSrvRunningGlobal (TEXT("Global\\") szAllSrvRunning)

#if _MSC_VER >= 1310
BOOL inline IsAllSrvRunning()
#else
BOOL IsAllSrvRunning()
#endif
{
  // Open the mutex using the global name first
  HANDLE hMutex = ::OpenMutex(SYNCHRONIZE, false, szAllSrvRunningGlobal);
  if (!hMutex)
    hMutex = ::OpenMutex(SYNCHRONIZE, false, szAllSrvRunning);
  BOOL bRunning = NULL != hMutex;
  if (bRunning)
    ::CloseHandle(hMutex);
  return bRunning;
}



extern RPC_IF_HANDLE __MIDL_itf_AllSrvModuleIDL_0326_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_AllSrvModuleIDL_0326_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


