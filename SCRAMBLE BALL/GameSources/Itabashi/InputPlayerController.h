#pragma once
#include <unordered_map>

namespace basecross
{
	class OnlinePlayerSynchronizer;
	class PlayerStatus;
	class Teleport;

	/// <summary>
	/// オンラインでのプレイヤーのイベント制御
	/// </summary>
	class InputPlayerController : public Component
	{
		std::weak_ptr<PlayerStatus> m_playerStatus;
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlinePlayerSynchronizer;
		std::weak_ptr<Teleport> m_teleport;
	public:

		InputPlayerController(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDraw() override {}
	};
}