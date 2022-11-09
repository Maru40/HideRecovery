#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_map>
#include <random>

namespace basecross
{
	class Numbers;
	class MatchingUIController;

namespace Online
{
	/// <summary>
	/// オンラインマッチングを担当するコンポーネント
	/// </summary>
	class OnlineMatching : public OnlineComponent
	{
	public:
		/// <summary>
		/// ゲームの最大人数
		/// </summary>
		static constexpr int MAX_PLAYER_NUM = 6;
		/// <summary>
		/// プレイヤー番号が更新された際のオンラインイベントコード
		/// </summary>
		static constexpr std::uint8_t EXECUTE_UPDATE_PLAYER_NUMBERS = 101;
		
	private:

		static constexpr int PASSWORD_DISIT_NUM = 4; // パスワードの桁数
		static constexpr std::uint32_t PASSWORD_UINT_MAX = 9999; // uintパスワードの最大値
		std::uniform_int_distribution<std::uint32_t> m_passwordDistribution; // 数字パスワードの有効範囲

		ExitGames::LoadBalancing::RoomOptions m_roomOptions; // ルームの設定

		/// <summary>
		/// オンライン上のプレイヤー番号とゲーム上のプレイヤー番号の対応配列
		/// </summary>
		static int m_playerNumbers[MAX_PLAYER_NUM];

		bool m_isConnected = false; // 現在接続されているか
		bool m_isMatching = false; // マッチング中か
		bool m_usePassword = false; // パスワードを使用しているか

		std::wstring m_password;

		std::weak_ptr<MatchingUIController> m_matchingUIController; // MatchingUIコントローラー

		/// <summary>
		/// プレイヤー番号が更新された際に呼ぶイベント
		/// </summary>
		/// <param name="playerNumber">更新されたプレイヤー番号</param>
		/// <param name="isAdd">追加ならtrue、削除ならfalse</param>
		void PlayerStateUpdateEvent(int playerNumber, bool isAdd);
		/// <summary>
		/// プレイヤー番号対応配列が更新された際に呼ぶイベント
		/// </summary>
		/// <param name="playerNumbers">新しいプレイヤー番号対応配列</param>
		void ExecuteGetPlayerStateEvent(int playerNumbers[]);

		/// <summary>
		/// パスワードの作成
		/// </summary>
		/// <returns>パスワード</returns>
		std::wstring CreatePassword();

	public:
		OnlineMatching(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnLateStart() override;

		void OnConnected() override;

		void OnCreateRoom() override;

		virtual void OnCreateRoomFailed(int errorCode) override;

		void OnJoinRoom() override;

		void OnJoinRoomFailed(int errorCode) override;

		void OnLeaveRoom() override;

		void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		/// <summary>
		/// 所属している部屋が満室か
		/// </summary>
		/// <returns>満室ならtrue</returns>
		bool IsFullyOccupied() const;
		/// <summary>
		/// 所属している部屋の人数を取得
		/// </summary>
		/// <returns>人数</returns>
		int GetPlayerCount() const;

		/// <summary>
		/// プレイヤー番号の初期化
		/// </summary>
		void Reset();

		/// <summary>
		/// チームをシャッフルする
		/// </summary>
		void ShuffleTeam();

		/// <summary>
		/// フリーマッチングを開始する
		/// </summary>
		void StartFreeMatching();

		/// <summary>
		/// パスワード付きの部屋を作成する
		/// </summary>
		void StartCreatePasswordMatching();

		/// <summary>
		/// パスワード付きの部屋に参加する
		/// </summary>
		/// <param name="password">パスワード</param>
		void StartJoinPasswordMatching(const std::wstring& password);

		/// <summary>
		/// 部屋から抜ける
		/// </summary>
		void StartLeaveRoom();

		/// <summary>
		/// 現在接続されているか
		/// </summary>
		/// <returns>接続されているならtrue</returns>
		bool IsConnected() const { return m_isConnected; }
		/// <summary>
		/// マッチング中か
		/// </summary>
		/// <returns>マッチング中ならtrue</returns>
		bool IsMatching() const { return m_isMatching; }
		/// <summary>
		/// パスワードを使用しているか
		/// </summary>
		/// <returns>しているならtrue</returns>
		bool UsePassword() const { return m_usePassword; }

		/// <summary>
		/// MatchStageのUI操作コンポーネントの取得
		/// </summary>
		/// <param name="matchingUIController">MatchStageのUI操作コンポーネント</param>
		void SetMatchingUIController(const std::shared_ptr<MatchingUIController>& matchingUIController) { m_matchingUIController = matchingUIController; }

		/// <summary>
		/// ゲーム上のプレイヤー番号から、オンライン上のプレイヤー番号に変換する
		/// </summary>
		/// <param name="gameNumber">ゲーム上のプレイヤー番号</param>
		/// <returns>オンライン上のプレイヤー番号</returns>
		static int GetPlayerNumberToGameNumber(int gameNumber) { return m_playerNumbers[gameNumber]; }
	};
}
}