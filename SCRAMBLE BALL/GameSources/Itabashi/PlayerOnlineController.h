#pragma once
#include <unordered_map>

namespace basecross
{
	class OnlinePlayerSynchronizer;
	class PlayerStatus;

	/// <summary>
	/// オンラインでのプレイヤーのイベント制御
	/// </summary>
	class PlayerOnlineController : public Component
	{
		std::weak_ptr<PlayerStatus> m_playerStatus;
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlinePlayerSynchronizer;

	public:

		PlayerOnlineController(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDraw() override {}
	};
}