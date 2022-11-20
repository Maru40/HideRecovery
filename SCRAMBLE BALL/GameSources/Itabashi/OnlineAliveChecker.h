#pragma once
#include "OnlineManager.h"

namespace basecross
{
namespace Online
{
	class OnlineAliveChecker : public OnlineComponent
	{
	public:
		static constexpr std::uint8_t CONNECT_ALIVE_CHECK_EVENT_CODE = 171;

	private:

		struct CheckState
		{
			bool isAlive = false;
			float responseCountTime = 0.0f;

			CheckState() noexcept {}

			CheckState(bool isAlive, float responseCountTime) :
				isAlive(isAlive), responseCountTime(responseCountTime) {}
		};

		float m_aliveCheckTime = 1.0f;
		float m_aliveResponseTime = 0.5f;

		float m_aliveCheckCountTime = 0.0f;

		std::unordered_map<int, CheckState> m_playerAliveCheckMap;

	public:
		OnlineAliveChecker(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		bool IsPlayerAlive(int onlinePlayerNumber) const { return m_playerAliveCheckMap.at(onlinePlayerNumber).isAlive; }

		bool IsLocalPlayerAlive() const { return IsPlayerAlive(OnlineManager::GetLocalPlayer().getNumber()); }
	};
}
}