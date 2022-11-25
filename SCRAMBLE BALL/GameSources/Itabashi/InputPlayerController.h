#pragma once
#include <unordered_map>

namespace basecross
{
	class OnlinePlayerSynchronizer;
	class PlayerControlManager;
	class PlayerStatus;
	class Teleport;

	namespace Online
	{
		class OnlineAliveChecker;
	}

	/// <summary>
	/// オンラインでのプレイヤーのイベント制御
	/// </summary>
	class InputPlayerController : public Component
	{
		std::weak_ptr<PlayerStatus> m_playerStatus;
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlinePlayerSynchronizer;
		std::weak_ptr<Teleport> m_teleport;
		std::weak_ptr<Camera> m_camera;
		std::weak_ptr<PlayerControlManager> m_playerControlManager;
		std::weak_ptr<Online::OnlineAliveChecker> m_onlineAliveChecker;

	public:

		InputPlayerController(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDraw() override {}

		void OnCollisionEnter(std::shared_ptr<GameObject>& other) override;

		void SetOnlineAliveChecker(const std::shared_ptr<Online::OnlineAliveChecker>& onlineAliveChecker) { m_onlineAliveChecker = onlineAliveChecker; }
	};
}