#pragma once
#include <vector>
#include <memory>

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
	class I_OnlineCallBacks
	{
	public:
		virtual void OnConnected() = 0;
		virtual void OnConnectFailed(int errorCode) = 0;

		virtual void OnDisconnected() = 0;

		virtual void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) = 0;

		virtual void OnCreateRoom() = 0;
		virtual void OnCreateRoomFailed(int errorCode) = 0;

		virtual void OnJoinRoom() = 0;
		virtual void OnJoinRoomFailed(int errorCode) = 0;
	};

	struct OnlinePlayer
	{
		int playerNumber = 0;
	};

	class OnlineRoom
	{
		ExitGames::Common::Hashtable m_customProperties;

	public:
		OnlineRoom(const ExitGames::Common::Hashtable& customProperties) :
			m_customProperties(customProperties)
		{

		}

		const ExitGames::Common::Hashtable& GetCustomProperties() const { return m_customProperties; }
	};

	class OnlineComponent : public Component, public I_OnlineCallBacks
	{
	public:
		OnlineComponent(const std::shared_ptr<GameObject>& owner);
		~OnlineComponent();

		virtual void OnConnected() override {}
		virtual void OnConnectFailed(int errorCode) override {}

		virtual void OnDisconnected() override {}

		virtual void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override {}

		virtual void OnCreateRoom() override {}
		virtual void OnCreateRoomFailed(int errorCode) override {}

		virtual void OnJoinRoom() override {}
		virtual void OnJoinRoomFailed(int errorCode) override {}
	};

	class OnlineManager : public ExitGames::LoadBalancing::Listener
	{
		static std::unique_ptr<OnlineManager> m_instance;

		OnlinePlayer m_localPlayer;
		std::unique_ptr<OnlineRoom> m_room;
		std::vector<I_OnlineCallBacks*> m_callBacksVector;
		std::wstring m_applicationId;
		std::unique_ptr<ExitGames::LoadBalancing::Client> m_client;

		OnlineManager() noexcept = default;

	public:
		~OnlineManager() noexcept = default;

		OnlineManager(const OnlineManager&) = delete;
		OnlineManager& operator=(const OnlineManager&) = delete;
		OnlineManager(OnlineManager&&) = delete;
		OnlineManager& operator=(OnlineManager&&) = delete;

		static void Update();

		static void AddCallBacks(I_OnlineCallBacks* callbacks);
		static void RemoveCallBacks(I_OnlineCallBacks* callbacks);

		static const std::unique_ptr<OnlineManager>& GetInstance();

		static const OnlinePlayer& GetLocalPlayer() { return GetInstance()->m_localPlayer; }
		static const std::unique_ptr<OnlineRoom>& GetRoom() { return GetInstance()->m_room; }

		static void SetApplicationId(const std::wstring& applicationId) { GetInstance()->m_applicationId = applicationId; }
		static const std::wstring& GetApplicationId() { return GetInstance()->m_applicationId; }

		static bool Connect(const std::wstring& version = std::wstring());

		static void Disconnect();

		static void CreateRoom(const std::wstring& roomName = std::wstring(),
			const ExitGames::LoadBalancing::RoomOptions& roomOptions = ExitGames::LoadBalancing::RoomOptions());

		static void JoinRoom(const std::wstring& roomName);

		static void JoinRandomOrCreateRoom(const ExitGames::LoadBalancing::RoomOptions& roomOptions = ExitGames::LoadBalancing::RoomOptions());

		static void RaiseEvent(bool reliable, const std::uint8_t* bytes, int size, std::uint8_t eventCode, const ExitGames::LoadBalancing::RaiseEventOptions& options = ExitGames::LoadBalancing::RaiseEventOptions());

		void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {}

		void connectionErrorReturn(int errorCode) override {}
		void clientErrorReturn(int errorCode) override {}
		void warningReturn(int warningCode) override {}
		void serverErrorReturn(int errorCode) override {}

		void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override {}
		void leaveRoomEventAction(int playerNr, bool isInactive) override {}

		void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent) override;

		void connectReturn(int errorCode, const ExitGames::Common::JString& errorString,
			const ExitGames::Common::JString& region, const ExitGames::Common::JString& cluster) override;

		void disconnectReturn() override;

		void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
			const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override;

		void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
			const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override;

		void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& roomProperties,
			const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString) override;

		void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString) override {}

	};
}
}