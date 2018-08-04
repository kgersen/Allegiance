#include "stdafx.h"
#include "ClientConnection.h"
#include "clintlib.h"
#include "igcWrapper.h"

using namespace System::Runtime::InteropServices;

namespace AllegianceInterop
{
	// Uses a method I found here: https://stackoverflow.com/questions/13770832/fire-an-event-in-c-cli-from-unmanaged-c-c-function
	// Have to jump through some hoops to get an event raised in unmanaged C++ to get to C++/CLI
	// Cleaned it up with this: https://msdn.microsoft.com/en-us/library/367eeye0.aspx
	ClientConnection::ClientConnection()
	{
		m_nativeClient = new NativeClient();
		m_deleteNativeClientOnFinalize = true;

		// Create delegate wrappers. 
		OnAppMessageEventDelegate ^ onAppMessageEventDelegate = gcnew OnAppMessageEventDelegate(this, &ClientConnection::OnAppMessageEvent);
		OnLogonLobbyAckEventDelegate ^ onLogonLobbyAckEventDelegate = gcnew OnLogonLobbyAckEventDelegate(this, &ClientConnection::OnLogonLobbyAckEvent);
		OnLogonAckEventDelegate ^ onLogonAckEventDelegate = gcnew OnLogonAckEventDelegate(this, &ClientConnection::OnLogonAckEvent);

		// These aren't used, but apparently this keeps the memory from moving around in the gc so these can be called?
		// Not sure why these don't get released when they fall out of scope after the method completes. This is some voodoo crap. 
		// Maybe they aren't used after all... I commented them out, and it's still working. 
		/*pin_ptr<OnAppMessageEventDelegate ^> pinnedOnAppMessageEventDelegate = &onAppMessageEventDelegate;
		pin_ptr<OnLogonLobbyAckEventDelegate ^> pinnedOnLogonLobbyAckEventDelegate = &onLogonLobbyAckEventDelegate;
		pin_ptr<OnLogonAckEventDelegate ^> pinnedOnLogonAckEventDelegate = &onLogonAckEventDelegate;*/

		// This keeps the function pointers for the delegates from being moved around by garbage collection. 
		m_gchOnAppMessageEventDelegate = GCHandle::Alloc(onAppMessageEventDelegate);
		m_gchOnLogonLobbyAckEventDelegate = GCHandle::Alloc(onLogonLobbyAckEventDelegate);
		m_gchOnLogonAckEventDelegate = GCHandle::Alloc(onLogonAckEventDelegate);

		IntPtr ipOnAppMessageEventDelegate = Marshal::GetFunctionPointerForDelegate(onAppMessageEventDelegate);
		IntPtr ipOnLogonLobbyAckEventDelegate = Marshal::GetFunctionPointerForDelegate(onLogonLobbyAckEventDelegate);
		IntPtr ipOnLogonAckEventDelegate = Marshal::GetFunctionPointerForDelegate(onLogonAckEventDelegate);


		OnAppMessageEventFunction OnAppMessageEventFunctionPointer = static_cast<OnAppMessageEventFunction>(ipOnAppMessageEventDelegate.ToPointer());
		OnLogonLobbyAckEventFunction OnLogonLobbyAckEventFunctionPointer = static_cast<OnLogonLobbyAckEventFunction>(ipOnLogonLobbyAckEventDelegate.ToPointer());
		OnLogonAckEventFunction OnLogonAckEventFunctionPointer = static_cast<OnLogonAckEventFunction>(ipOnLogonAckEventDelegate.ToPointer());

		
		// Just using the pinned pointer didn't work, it needed the Marshal pointer. But apparently, that pointer can move around..
		// So, using the above pin to keep it in one spot?
		/*m_simpleClient->HookOnAppMessageEvent((HRESULT(*)(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)) pinnedOnAppMessageEventDelegate);
		m_simpleClient->HookOnLogonLobbyAckEvent((void(*)(bool fValidated, bool bRetry, LPCSTR szFailureReason)) pinnedOnLogonLobbyAckEventDelegate);
		m_simpleClient->HookOnLogonAckEvent((void(*)(bool fValidated, bool bRetry, LPCSTR szFailureReason)) pinnedOnLogonAckEventDelegate);*/

		//__hook(&SimpleClient::OnAppMessageEvent, m_simpleClient, &ClientConnection::OnAppMessageEvent);

		/*m_simpleClient->HookOnAppMessageEvent((HRESULT(*)(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)) Marshal::GetFunctionPointerForDelegate(onAppMessageEventDelegate).ToPointer());
		m_simpleClient->HookOnLogonLobbyAckEvent((void(*)(bool fValidated, bool bRetry, LPCSTR szFailureReason)) Marshal::GetFunctionPointerForDelegate(onLogonLobbyAckEventDelegate).ToPointer());
		m_simpleClient->HookOnLogonAckEvent((void(*)(bool fValidated, bool bRetry, LPCSTR szFailureReason)) Marshal::GetFunctionPointerForDelegate(onLogonAckEventDelegate).ToPointer());
*/
		//__hook(&SimpleClient::OnAppMessageEvent, m_simpleClient, &OnAppMessageEventFunctionPointer);

		/*m_simpleClient->OnAppMessageEvent*/

		m_nativeClient->HookOnAppMessageEvent(OnAppMessageEventFunctionPointer);
		m_nativeClient->HookOnLogonLobbyAckEvent(OnLogonLobbyAckEventFunctionPointer);
		m_nativeClient->HookOnLogonAckEvent(OnLogonAckEventFunctionPointer);

		//m_simpleClient->HookOnLogonLobbyAckEvent((void(*)(bool fValidated, bool bRetry, LPCSTR szFailureReason)) Marshal::GetFunctionPointerForDelegate(onLogonLobbyAckEventDelegate).ToPointer());
		//m_simpleClient->HookOnLogonAckEvent((void(*)(bool fValidated, bool bRetry, LPCSTR szFailureReason)) Marshal::GetFunctionPointerForDelegate(onLogonAckEventDelegate).ToPointer());

	}

	ClientConnection::~ClientConnection()
	{
		if(m_deleteNativeClientOnFinalize == true)
			delete m_nativeClient;
	}

	ClientConnection::!ClientConnection()
	{
		if (m_deleteNativeClientOnFinalize == true)
			delete m_nativeClient;
	}

	void ClientConnection::ConnectToLobby(CancellationTokenSource ^ cancellationTokenSource, String ^ lobbyAddress, String ^ characterName, String ^ cdKey)
	{
		BaseClient::ConnectInfo connectInfo;
		
		ZeroMemory(&connectInfo, sizeof(BaseClient::ConnectInfo));

		IntPtr cstrLobbyAddress = Marshal::StringToHGlobalAnsi(lobbyAddress);
		IntPtr cstrCharacterName = Marshal::StringToHGlobalAnsi(characterName);
		IntPtr cstrCdKey = Marshal::StringToHGlobalAnsi(cdKey);

		strcpy(connectInfo.szName, (const char*)cstrCharacterName.ToPointer());
		connectInfo.strServer = (const char*)cstrLobbyAddress.ToPointer();
		strcpy((char *) connectInfo.steamAuthTicket, "NO STEAM AUTH");
		connectInfo.steamAuthTicketLength = strlen((char *)connectInfo.steamAuthTicket);

		m_nativeClient->SetCDKey((const char*)cstrCdKey.ToPointer());

		Marshal::FreeHGlobal(cstrLobbyAddress);
		Marshal::FreeHGlobal(cstrCharacterName);
		Marshal::FreeHGlobal(cstrCdKey);
		
		// This hack comes from Pig.cpp... That's how they did it. :/
		m_nativeClient->GetCfgInfo().strClubLobby = connectInfo.strServer;
		m_nativeClient->GetCfgInfo().strPublicLobby = connectInfo.strServer;

		SYSTEMTIME st;
		GetSystemTime(&st);
		SystemTimeToFileTime(&st, &connectInfo.ftLastArtUpdate);

		m_nativeClient->ConnectToLobby(&connectInfo);

		/*while (cancellationTokenSource->IsCancellationRequested == false)
		{
			m_nativeClient->SendAndReceiveUpdate();
			Thread::Sleep(10);
		}

		m_nativeClient->DisconnectLobby();*/
		
	}

	System::Collections::Generic::List<MissionInfoWrapper ^> ^ ClientConnection::GetMissionList()
	{
		System::Collections::Generic::List<MissionInfoWrapper ^> ^ returnValue = gcnew System::Collections::Generic::List<MissionInfoWrapper ^>();

		for (TMapListWrapper<DWORD, MissionInfo*>::Iterator it(m_nativeClient->m_mapMissions); !it.End(); it.Next())
		{
			//if (it.Value()->GetAnySlotsAreAvailable())
			//{
				returnValue->Add(gcnew MissionInfoWrapper(it.Value()));
			//}
		}	

		return returnValue;
	}

	void ClientConnection::JoinMission(MissionInfoWrapper ^ missionInfo, String ^ password)
	{ 
		char strPassword[256];

		IntPtr cstrPassword = Marshal::StringToHGlobalAnsi(password);
		strcpy(strPassword, (const char*)cstrPassword.ToPointer());

		m_nativeClient->JoinMission(missionInfo->m_instance, strPassword);

		Marshal::FreeHGlobal(cstrPassword);
	}

	void ClientConnection::ConnectToServer(/*CancellationTokenSource ^ cancellationTokenSource, */ String ^ serverAddress, int port, String ^ characterName, String ^ cdKey, int cookie)
	{
		BaseClient::ConnectInfo connectInfo;

		ZeroMemory(&connectInfo, sizeof(BaseClient::ConnectInfo));

		IntPtr cstrServerAddress = Marshal::StringToHGlobalAnsi(serverAddress);
		IntPtr cstrCharacterName = Marshal::StringToHGlobalAnsi(characterName);
		IntPtr cstrCdKey = Marshal::StringToHGlobalAnsi(cdKey);

		strcpy(connectInfo.szName, (const char*)cstrCharacterName.ToPointer());
		connectInfo.strServer = (const char*)cstrServerAddress.ToPointer();
		connectInfo.dwPort = port;
		connectInfo.guidSession = GUID_NULL;

		m_nativeClient->SetCDKey((char *) cstrCdKey.ToPointer());

		m_nativeClient->m_lastUpdate = Time::Now();
		m_nativeClient->m_now = Time::Now();

		m_nativeClient->ConnectToServer(connectInfo, cookie, m_nativeClient->m_now, "", false);

		Marshal::FreeHGlobal(cstrCharacterName);
		Marshal::FreeHGlobal(cstrServerAddress);
		Marshal::FreeHGlobal(cstrCdKey);

		/*while (cancellationTokenSource->IsCancellationRequested == false)
		{
			m_nativeClient->SendAndReceiveUpdate();
			Thread::Sleep(100);
		}

		m_nativeClient->Disconnect();*/
	}

	/*void ClientConnection::LogoffLobby()
	{
		m_simpleClient->DisconnectLobby();
	}*/

	// From Pig.cpp
	void ClientConnection::SendAndReceiveUpdate()
	{
		m_nativeClient->SendAndReceiveUpdate();

	}

	void ClientConnection::SendMessageLobby(ISupportManagedByteArrays ^ message)
	{
		SendMessage(ConnectionType::Lobby, message);
	}

	void ClientConnection::SendMessageServer(ISupportManagedByteArrays ^ message)
	{
		SendMessage(ConnectionType::Server, message);
	}

	void ClientConnection::SendMessage(ConnectionType connectionType, ISupportManagedByteArrays ^ message)
	{
		FedMessaging *fm;

		switch (connectionType)
		{
		case ConnectionType::Lobby:
			fm = m_nativeClient->GetFMLobby();
			break;

		case ConnectionType::Server:
			fm = &m_nativeClient->m_fm;
			break;

		default:
			throw gcnew NotSupportedException(Convert::ToString(connectionType));
		}

		array<unsigned char> ^ bytes = message->ToManagedByteArray();

		pin_ptr<byte> pinnedBytes = &bytes[0];

		if(fm->IsConnected() == true)
			fm->GenericSend(fm->GetServerConnection(), pinnedBytes, bytes->Length, FM_GUARANTEED);

		// Release it.
		pinnedBytes = nullptr;
	}


	/////////////////////////////////////////////////////////////////////
	// SimpleClient Event Handlers

	void ClientConnection::OnAppMessageEvent(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
	{
		array<Byte> ^ bytes = gcnew array<Byte>(pfm->cbmsg);

		Marshal::Copy((IntPtr)pfm, bytes, 0, pfm->cbmsg);

		OnAppMessage(this, bytes);
	}

	void ClientConnection::OnLogonLobbyAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason)
	{
		printf("ClientConnection::OnLogonLobbyAckEvent\n");
	}

	void ClientConnection::OnLogonAckEvent(bool fValidated, bool bRetry, LPCSTR szFailureReason)
	{
		printf("ClientConnection::OnLogonAckEvent\n");
	}
}
