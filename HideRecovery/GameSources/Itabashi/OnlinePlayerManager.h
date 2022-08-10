#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_map>

namespace basecross
{
	class PlayerObject;

namespace Online
{
	class OnlinePlayerManager : public OnlineComponent
	{
	public:
		/// <summary>
		/// プレイヤーの最大数
		/// </summary>
		static constexpr int MAX_PLAYER_NUM = 6;

	private:
		/// <summary>
		/// プレイヤー番号の使用状況
		/// </summary>
		bool m_numberUses[MAX_PLAYER_NUM];

		/// <summary>
		/// プレイヤー配列
		/// </summary>
		std::vector<std::weak_ptr<PlayerObject>> m_players;
		/// <summary>
		/// shard_ptrに変換した配列を返す
		/// </summary>
		/// <returns></returns>
		std::vector<std::shared_ptr<PlayerObject>> GetPlayers();
		/// <summary>
		/// プレイヤーオブジェクトの生成
		/// </summary>
		/// <param name="playerNumber">プレイヤー番号</param>
		/// <param name="position">生成する座標</param>
		/// <returns></returns>
		std::shared_ptr<PlayerObject> CreatePlayerObject(int playerNumber, const Vec3& position);
	public:

		OnlinePlayerManager(const std::shared_ptr<GameObject>& owner);
		
		void OnUpdate() override {}

		void OnDraw() override{}

		void OnJoinRoomEventAction(int playerNumber, const std::vector<int>& playerNumbers, const ExitGames::LoadBalancing::Player& player) override;
		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;
	};
}
}