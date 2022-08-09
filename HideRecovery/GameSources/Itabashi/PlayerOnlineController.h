#pragma once
#include "stdafx.h"

#include <LoadBalancing-cpp/inc/Listener.h>
#include <LoadBalancing-cpp/inc/Client.h>


#if defined(_DEBUG)
#pragma comment(lib,"Common-cpp/lib/Debug/Common-cpp.lib")
#pragma comment(lib,"Photon-cpp/lib/Debug/Photon-cpp.lib")
#pragma comment(lib,"LoadBalancing-cpp/lib/Debug/LoadBalancing-cpp.lib")
#else
#pragma comment(lib,"Common-cpp/lib/Release/Common-cpp.lib")
#pragma comment(lib,"Photon-cpp/lib/Release/Photon-cpp.lib")
#pragma comment(lib,"LoadBalancing-cpp/lib/Release/LoadBalancing-cpp.lib")
#endif

namespace basecross
{
namespace Online
{
	class PlayerOnlineController : public Component, public ExitGames::LoadBalancing::Listener
	{
		static std::wstring m_applicationID;

		ExitGames::LoadBalancing::Client m_client;
		int m_playerNumber = 0;

	public:
		PlayerOnlineController(const std::shared_ptr<GameObject>& owner);

		void OnUpdate() override;

		void OnDraw() override {}

		void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {}

		void connectionErrorReturn(int errorCode) override {}
		void clientErrorReturn(int errorCode) override {}
		void warningReturn(int warningCode) override {}
		void serverErrorReturn(int errorCode) override {}

		void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override {}
		void leaveRoomEventAction(int playerNr, bool isInactive) override {}

		void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override {}

		void connectReturn(int errorCode, const ExitGames::Common::JString& errorString,
			const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override {}

		void disconnectReturn(void) override {}

		void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
			const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override {}

		void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
			const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override {}

		void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
			const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override {}

		void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override {}

		void Connect() {}


		static void SetApplicationID(const std::wstring& applicationID) { m_applicationID = applicationID; }
		static const std::wstring& GetApplicationID() { return m_applicationID; }
	};
}
}