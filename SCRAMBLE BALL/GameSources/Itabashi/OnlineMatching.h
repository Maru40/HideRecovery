#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_map>

namespace basecross
{
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

		/// <summary>
		/// オンライン上のプレイヤー番号とゲーム上のプレイヤー番号の対応配列
		/// </summary>
		static int m_playerNumbers[MAX_PLAYER_NUM];

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

	public:
		OnlineMatching(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnConnected() override;

		void OnCreateRoom() override;

		void OnJoinRoom() override;

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
		/// ゲーム上のプレイヤー番号から、オンライン上のプレイヤー番号に変換する
		/// </summary>
		/// <param name="gameNumber">ゲーム上のプレイヤー番号</param>
		/// <returns>オンライン上のプレイヤー番号</returns>
		static int GetPlayerNumberToGameNumber(int gameNumber) { return m_playerNumbers[gameNumber]; }
	};
}
}