#pragma once


#include "messagecore.h"
#include "clintlib.h"
#include "CriticalSectionManager.h"


// Enable IntPtr to cast to delegate signatures.
typedef HRESULT(__stdcall *OnAppMessageEventFunction)(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
typedef void(__stdcall * OnLogonLobbyAckEventFunction)(bool fValidated, bool bRetry, LPCSTR szFailureReason);
typedef void(__stdcall *OnLogonAckEventFunction)(bool fValidated, bool bRetry, LPCSTR szFailureReason);


namespace AllegianceInterop 
{


	
	[event_source(native)]
	public class NativeClient : public BaseClient
	{

	private:
		static CriticalSectionManager m_criticalSection;

		int m_counter = 0;

		Vector m_lastPosition = Vector(0, 0, 0);
		Time m_timeLastUpdate = Time::Now();
		//Time m_lastUpdate = Time::Now();

		OnAppMessageEventFunction m_OnAppMessageDelegate;
		OnLogonLobbyAckEventFunction m_OnLogonLobbyAckDelegate;
		OnLogonAckEventFunction m_OnLogonAckDelegate;

		//HRESULT(* m_OnAppMessageDelegate)(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
		//void(* m_OnLogonLobbyAckDelegate)(bool fValidated, bool bRetry, LPCSTR szFailureReason);
		//void(* m_OnLogonAckDelegate)(bool fValidated, bool bRetry, LPCSTR szFailureReason);

		int ExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep);

		// Inherited via IFedMessagingSite
		virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm) override;

		// IClientEventSink Overrides (BaseClient)
		virtual void OnLogonLobbyAck(bool fValidated, bool bRetry, LPCSTR szFailureReason) override;
		virtual void OnLogonAck(bool fValidated, bool bRetry, LPCSTR szFailureReason) override;

		__event void OnAppMessageEvent(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
		__event void OnLogonLobbyAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason);
		__event void OnLogonAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason);

		void OnAppMessageEventHandler(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
		void OnLogonLobbyAckEventHandler(bool fValidated, bool bRetry, LPCSTR szFailureReason);
		void OnLogonAckEventHandler(bool fValidated, bool bRetry, LPCSTR szFailureReason);

	public:

		

		NativeClient();

		
		void HookOnAppMessageEvent(OnAppMessageEventFunction function);
		void HookOnLogonLobbyAckEvent(OnLogonLobbyAckEventFunction function);
		void HookOnLogonAckEvent(OnLogonAckEventFunction function);

		//void HookOnAppMessageEvent(HRESULT(*function)(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm));
		//void HookOnLogonLobbyAckEvent(void(*function)(bool fValidated, bool bRetry, LPCSTR szFailureReason));
		//void HookOnLogonAckEvent(void(*function)(bool fValidated, bool bRetry, LPCSTR szFailureReason));

		void SendAndReceiveUpdate();

		// IGCCallBacks (Taken from pig.h)
		virtual TRef<ThingSite> CreateThingSite(ImodelIGC* pModel);
		virtual TRef<ClusterSite> CreateClusterSite(IclusterIGC* pCluster);
		virtual void ChangeStation(IshipIGC* pship, IstationIGC* pstationOld,
			IstationIGC* pstationNew);
		virtual void ChangeCluster(IshipIGC*  pship, IclusterIGC* pclusterOld,
			IclusterIGC* pclusterNew); // Without this, the current cluster doesn't become active, so our ship won't get any damn updates. 


	};
}


