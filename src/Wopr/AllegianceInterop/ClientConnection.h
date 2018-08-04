#pragma once

#include "NativeClient.h"
#include "ISupportManagedByteArrays.h"
#include "Enums.h"
//#include "igc.h"
#include "igcWrapper.h"
#include "clintlibWrapper.h"

using namespace System;
using namespace System::Threading;
using namespace System::Collections::Generic;

namespace AllegianceInterop
{
	// Sillyness to let OnAppMessageDelegate use ClientConnection as an argument.
	ref class ClientConnection; 

	[Serializable]
	delegate void OnAppMessageEventDelegate(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
	delegate void OnLogonLobbyAckEventDelegate(bool fValidated, bool bRetry, LPCSTR szFailureReason);
	delegate void OnLogonAckEventDelegate(bool fValidated, bool bRetry, LPCSTR szFailureReason);

	[Serializable]
	public ref class ClientConnection
	{
	private:
		//int m_counter = 0;

		[NonSerialized]
		System::Runtime::InteropServices::GCHandle m_gchOnAppMessageEventDelegate;
		[NonSerialized]
		System::Runtime::InteropServices::GCHandle m_gchOnLogonLobbyAckEventDelegate;
		[NonSerialized]
		System::Runtime::InteropServices::GCHandle m_gchOnLogonAckEventDelegate;

		// Becuase the client can be serialized between two app domains, only the original client actually needs to clean up the connection
		// when it's original creator hits finalize. All of the serialized copies do not need to do the delete. By marking this flag
		// as non-serialized we will ensure that any copies of this object will not attempt to delete the unmanaged pointer to the native client that
		// doesn't belong to them. 
		[NonSerialized]
		bool m_deleteNativeClientOnFinalize = false;
		
		NativeClient * m_nativeClient;

		void SendMessage(ConnectionType connectionType, ISupportManagedByteArrays ^ message);



	public:
		delegate void OnAppMessageDelegate(ClientConnection ^ clientConnection, array<Byte> ^ bytes);
		event OnAppMessageDelegate ^ OnAppMessage;

		ClientConnection();
		~ClientConnection();
		!ClientConnection();

		void OnAppMessageEvent(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm);
		void OnLogonLobbyAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason);
		void OnLogonAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason);
		 
		void ConnectToLobby(CancellationTokenSource ^ cancellationTokenSource, String ^ lobbyAddress, String ^ characterName, String ^ cdKey);
		void ConnectToServer(/*CancellationTokenSource ^ cancellationTokenSource, */String ^ serverAddress, int port, String ^ characterName, String ^ cdKey, int cookie);

		void DisconnectLobby()
		{
			m_nativeClient->DisconnectLobby();
		}

		void DisconnectServer()
		{
			m_nativeClient->Disconnect();
		}

		void SendAndReceiveUpdate();

		void SendMessageLobby(ISupportManagedByteArrays ^ message);
		void SendMessageServer(ISupportManagedByteArrays ^ message);

		//generic <class T> 
		//	T GetNullOrObject(void * unmanagedValue)
		//{
		//	if (unmanagedValue == nullptr)
		//		return nullptr;

		//	//return ICreate<T>::Create();
		//	return gcnew T(unmanagedValue);
		//}

		System::Collections::Generic::List<MissionInfoWrapper ^> ^ GetMissionList();
		void JoinMission(MissionInfoWrapper ^ missionInfo, String ^ password);
		IshipIGCWrapper ^ GetShip() { ConvertTo(IshipIGCWrapper, m_nativeClient->GetShip()) };
		IsideIGCWrapper ^ GetSide() { ConvertTo(IsideIGCWrapper, m_nativeClient->GetSide()) };
		void AddMoneyToBucket(IbucketIGCWrapper ^ bucket, Money money) { m_nativeClient->AddMoneyToBucket(bucket->m_instance, money); };
		Money GetMoney(void) { return m_nativeClient->GetMoney(); };
		ImissionIGCWrapper ^ GetCore() { ConvertTo(ImissionIGCWrapper, m_nativeClient->GetCore()) };
		void BuyLoadout(IshipIGCWrapper ^ ship, bool launch) { m_nativeClient->BuyLoadout(ship->m_instance, launch); };
		void BuyDefaultLoadout(IshipIGCWrapper ^ ship, IstationIGCWrapper ^ station, IhullTypeIGCWrapper ^ hullType, Money budget) { m_nativeClient->BuyDefaultLoadout(ship->m_instance, station->m_instance, hullType->m_instance, &budget); };
		IshipIGCWrapper ^ CreateEmptyShip() { ConvertTo(IshipIGCWrapper, m_nativeClient->CreateEmptyShip()) }
		void BoardShip(IshipIGCWrapper ^ ship) { m_nativeClient->BoardShip(ship->m_instance); };
		
		short BuyPartOnBudget(IshipIGCWrapper ^       pship, IpartTypeIGCWrapper ^   ppt, Mount           mount, Money          pbudget) {
			return m_nativeClient->BuyPartOnBudget(pship->m_instance, ppt->m_instance, mount, &pbudget);
		}

		/*void FireExpendable(IshipIGCWrapper ^ pShip,
			IdispenserIGCWrapper ^ pDispenser,
			uint32 timeFired) { m_nativeClient->FireExpendable(pShip->m_instance, pDispenser->m_instance, timeFired); };*/

		void FireDispenser(IshipIGCWrapper ^ ship)
		{
			IdispenserIGC * dispenser = (IdispenserIGC *) ship->m_instance->GetMountedPart(ET_Dispenser, 0);
			m_nativeClient->FireExpendable(ship->m_instance, dispenser, Time::Now());
		}

		void FireChaff(IshipIGCWrapper ^ ship)
		{
			IdispenserIGC * dispenser = (IdispenserIGC *)ship->m_instance->GetMountedPart(ET_ChaffLauncher, 0);
			m_nativeClient->FireExpendable(ship->m_instance, dispenser, Time::Now());
		}

		void FireMissile(IshipIGCWrapper ^ ship, ImodelIGCWrapper ^ target, float lock)
		{
			ImagazineIGC * magazine = (ImagazineIGC *)ship->m_instance->GetMountedPart(ET_Magazine, 0);
			m_nativeClient->FireMissile(ship->m_instance, magazine, Time::Now(), target->m_instance, lock);
		}

		IbuoyIGCWrapper ^ CreateBuoy(BuoyType buoyType, float x, float y, float z, int clusterID, bool visible)
		{
			DataBuoyIGC buoy;

			ImissionIGC * mission = m_nativeClient->GetSide()->GetMission();
			buoy.buoyID = mission->GenerateNewBuoyID();
			buoy.position = Vector(x, y, z);
			buoy.clusterID = clusterID;
			buoy.type = buoyType;
			buoy.visible = visible;

			IbuoyIGCWrapper ^ buoyWrapper = gcnew IbuoyIGCWrapper((IbuoyIGC *) mission->CreateObject(Time::Now(), OT_buoy, &buoy, sizeof(buoy)));
			return buoyWrapper;
		}

	};


}
