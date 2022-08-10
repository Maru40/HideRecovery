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
	/// <summary>
	/// オンラインのコールバック用インターフェース
	/// </summary>
	class I_OnlineCallBacks
	{
	public:
		/// <summary>
		/// 接続に成功した場合
		/// </summary>
		virtual void OnConnected() = 0;
		/// <summary>
		/// 接続に失敗したとき
		/// </summary>
		/// <param name="errorCode">エラーコード</param>
		virtual void OnConnectFailed(int errorCode) = 0;
		/// <summary>
		/// 接続を切った時
		/// </summary>
		virtual void OnDisconnected() = 0;
		/// <summary>
		/// 部屋に誰か参加したとき(自分含む)
		/// </summary>
		/// <param name="playerNr">参加したプレイヤー番号</param>
		/// <param name="playernrs">部屋にいる全員のプレイヤー番号配列</param>
		/// <param name="player">プレイヤー情報</param>
		virtual void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) = 0;
		/// <summary>
		/// 部屋から誰かが抜けたとき(自分含む)
		/// </summary>
		/// <param name="playerNr">抜けたプレイヤー番号</param>
		/// <param name="isInactive">非アクティブか（詳しい意味は調査中）</param>
		virtual void OnLeaveRoomEventAction(int playerNumber, bool isInactive) = 0;
		/// <summary>
		/// カスタムイベントが発行されたとき
		/// </summary>
		/// <param name="playerNumber">発行したプレイヤー番号</param>
		/// <param name="eventCode">イベントコード</param>
		/// <param name="bytes">イベントデータ(バイト配列)</param>
		virtual void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) = 0;
		/// <summary>
		/// 部屋を自分が作成したとき
		/// </summary>
		virtual void OnCreateRoom() = 0;
		/// <summary>
		/// 自分が部屋の作成に失敗したとき
		/// </summary>
		/// <param name="errorCode">エラーコード</param>
		virtual void OnCreateRoomFailed(int errorCode) = 0;
		/// <summary>
		/// 部屋の参加に成功したとき
		/// </summary>
		virtual void OnJoinRoom() = 0;
		/// <summary>
		/// 部屋の参加に失敗したとき
		/// </summary>
		/// <param name="errorCode">エラーコード</param>
		virtual void OnJoinRoomFailed(int errorCode) = 0;
	};

	/// <summary>
	/// オンラインのコールバックを持ったコンポーネント
	/// </summary>
	class OnlineComponent : public Component, public I_OnlineCallBacks
	{
	public:
		OnlineComponent(const std::shared_ptr<GameObject>& owner);
		~OnlineComponent();

		virtual void OnConnected() override {}
		virtual void OnConnectFailed(int errorCode) override {}

		virtual void OnDisconnected() override {}

		virtual void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) {}
		virtual void OnLeaveRoomEventAction(int playerNumber, bool isInactive) {}

		virtual void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override {}

		virtual void OnCreateRoom() override {}
		virtual void OnCreateRoomFailed(int errorCode) override {}

		virtual void OnJoinRoom() override {}
		virtual void OnJoinRoomFailed(int errorCode) override {}
	};

	/// <summary>
	/// オンライン制御用のシングルトンクラス
	/// </summary>
	class OnlineManager : public ExitGames::LoadBalancing::Listener
	{
		/// <summary>
		/// 自分自身のインスタンス
		/// </summary>
		static std::unique_ptr<OnlineManager> m_instance;

		/// <summary>
		/// オンラインのコールバック用の配列
		/// </summary>
		std::vector<I_OnlineCallBacks*> m_callBacksVector;
		/// <summary>
		/// Photonに接続する用のアプリケーションId
		/// </summary>
		std::wstring m_applicationId;
		/// <summary>
		/// 接続するためのクライアント
		/// </summary>
		std::unique_ptr<ExitGames::LoadBalancing::Client> m_client;

		OnlineManager() noexcept = default;

	public:
		~OnlineManager() noexcept = default;

		OnlineManager(const OnlineManager&) = delete;
		OnlineManager& operator=(const OnlineManager&) = delete;
		OnlineManager(OnlineManager&&) = delete;
		OnlineManager& operator=(OnlineManager&&) = delete;

		/// <summary>
		/// 更新関数
		/// </summary>
		static void Update();

		/// <summary>
		/// コールバックを呼ぶ対象を追加する
		/// </summary>
		/// <param name="callbacks">コールバック</param>
		static void AddCallBacks(I_OnlineCallBacks* callbacks);
		/// <summary>
		/// コールバックを呼ぶ対象を削除する
		/// </summary>
		/// <param name="callbacks"></param>
		static void RemoveCallBacks(I_OnlineCallBacks* callbacks);
		/// <summary>
		/// インスタンスの取得
		/// </summary>
		/// <returns>インスタンス</returns>
		static const std::unique_ptr<OnlineManager>& GetInstance();
		/// <summary>
		/// ローカルプレイヤー情報の取得
		/// </summary>
		/// <returns>ローカルプレイヤー情報</returns>
		static const ExitGames::LoadBalancing::MutablePlayer& GetLocalPlayer() { return GetInstance()->m_client->getLocalPlayer(); }
		/// <summary>
		/// 部屋情報の取得
		/// </summary>
		/// <returns>部屋情報</returns>
		static const ExitGames::LoadBalancing::MutableRoom& GetCurrentlyJoinedRoom() { return GetInstance()->m_client->getCurrentlyJoinedRoom(); }
		/// <summary>
		/// Photon用のアプリケーションIdの設定
		/// </summary>
		/// <param name="applicationId">アプリケーションId</param>
		static void SetApplicationId(const std::wstring& applicationId) { GetInstance()->m_applicationId = applicationId; }
		/// <summary>
		/// Photon用のアプリケーションIdの取得
		/// </summary>
		/// <returns>アプリケーションId</returns>
		static const std::wstring& GetApplicationId() { return GetInstance()->m_applicationId; }
		/// <summary>
		/// 接続処理
		/// </summary>
		/// <param name="version">接続バージョン</param>
		/// <returns>接続を試すことができたか</returns>
		static bool Connect(const std::wstring& version = std::wstring());
		/// <summary>
		/// 接続の終了
		/// </summary>
		static void Disconnect();
		/// <summary>
		/// オンラインの部屋作成
		/// </summary>
		/// <param name="roomName">部屋名</param>
		/// <param name="roomOptions">部屋設定</param>
		static void CreateRoom(const std::wstring& roomName = std::wstring(),
			const ExitGames::LoadBalancing::RoomOptions& roomOptions = ExitGames::LoadBalancing::RoomOptions());
		/// <summary>
		/// オンラインの部屋への参加
		/// </summary>
		/// <param name="roomName">部屋名</param>
		static void JoinRoom(const std::wstring& roomName);
		/// <summary>
		/// ランダムな部屋参加をするか、部屋を作成する
		/// </summary>
		/// <param name="roomOptions">部屋設定</param>
		static void JoinRandomOrCreateRoom(const ExitGames::LoadBalancing::RoomOptions& roomOptions = ExitGames::LoadBalancing::RoomOptions());
		/// <summary>
		/// オンラインでイベントを発行する
		/// </summary>
		/// <param name="reliable"></param>
		/// <param name="bytes">送るデータ(バイト配列)</param>
		/// <param name="size">データサイズ</param>
		/// <param name="eventCode">イベントコード</param>
		/// <param name="options">部屋設定</param>
		static void RaiseEvent(bool reliable, const std::uint8_t* bytes, int size, std::uint8_t eventCode, const ExitGames::LoadBalancing::RaiseEventOptions& options = ExitGames::LoadBalancing::RaiseEventOptions());

		// 自分自身のコールバック -------------------------------------------------------------------------------------------------

		void debugReturn(int debugLevel, const ExitGames::Common::JString& string) override {}

		void connectionErrorReturn(int errorCode) override {}
		void clientErrorReturn(int errorCode) override {}
		void warningReturn(int warningCode) override {}
		void serverErrorReturn(int errorCode) override {}

		void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player) override;
		void leaveRoomEventAction(int playerNr, bool isInactive) override;

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

		// -------------------------------------------------------------------------------------------------------------------------
	};
}
}