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
		static constexpr std::uint8_t CONNECT_ALIVE_RESPONSE_EVENT_CODE = 172;

	private:
		float m_aliveCheckTime = 1.0f;
		float m_aliveResponseTime = 0.5f;

		float m_aliveCheckCountTime = 0.0f;
		float m_aliveResponseCountTime = 0.0f;

		bool m_isWaitingResponse = false;

		bool m_isAlive = true;

		static const ExitGames::LoadBalancing::RaiseEventOptions m_raiseEventOptions;

		void MasterProcess(float deltaTime);
		void ClientProcess(float deltaTime);

	public:
		OnlineAliveChecker(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		bool IsAlive() const { return m_isAlive; }

		bool PlayerAlive(int onlinePlayerNumber);
	};
}
}