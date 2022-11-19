#include "OnlineAliveChecker.h"

namespace basecross
{
namespace Online
{
	const ExitGames::LoadBalancing::RaiseEventOptions OnlineAliveChecker::m_raiseEventOptions =
		ExitGames::LoadBalancing::RaiseEventOptions(0, ExitGames::Lite::EventCache::DO_NOT_CACHE, NULL, 0, ExitGames::Lite::ReceiverGroup::MASTER_CLIENT);

	OnlineAliveChecker::OnlineAliveChecker(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void OnlineAliveChecker::MasterProcess(float deltaTime)
	{
		m_aliveCheckCountTime += deltaTime;

		if (m_aliveCheckCountTime >= m_aliveCheckTime)
		{
			m_aliveCheckCountTime -= m_aliveCheckTime;
			OnlineManager::RaiseEvent(true, nullptr, 0, CONNECT_ALIVE_CHECK_EVENT_CODE);
			m_isWaitingResponse = true;
			m_aliveResponseCountTime = 0.0f;
			return;
		}

		if (!m_isWaitingResponse)
		{
			return;
		}

		m_aliveResponseCountTime += deltaTime;
		
		if (m_aliveResponseCountTime < m_aliveResponseTime || !m_isAlive)
		{
			return;
		}

		m_isAlive = false;
	}

	void OnlineAliveChecker::ClientProcess(float deltaTime)
	{
		m_aliveResponseCountTime += deltaTime;

		if (m_aliveResponseCountTime < m_aliveCheckTime + m_aliveResponseTime)
		{
			return;
		}

		m_isAlive = false;
	}

	void OnlineAliveChecker::OnCreate()
	{
		const auto& inRoomPlayers = OnlineManager::GetCurrentlyJoinedRoom().getPlayers();


		OnlineManager::GetCurrentlyJoinedRoom().getPlayers();
	}

	void OnlineAliveChecker::OnUpdate()
	{
		auto deltaTime = App::GetApp()->GetElapsedTime();

		if (OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			MasterProcess(deltaTime);
		}
		else
		{
			ClientProcess(deltaTime);
		}		
	}

	void OnlineAliveChecker::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == CONNECT_ALIVE_CHECK_EVENT_CODE)
		{
			OnlineManager::RaiseEvent(true, nullptr, 0, CONNECT_ALIVE_RESPONSE_EVENT_CODE, m_raiseEventOptions);
			m_isAlive = true;
			m_aliveResponseCountTime = 0.0f;
			return;
		}
		if (eventCode == CONNECT_ALIVE_RESPONSE_EVENT_CODE)
		{
			m_isWaitingResponse = false;
			m_isAlive = true;
		}
	}
}
}