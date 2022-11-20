#include "OnlineAliveChecker.h"

namespace basecross
{
namespace Online
{
	OnlineAliveChecker::OnlineAliveChecker(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
	}

	void OnlineAliveChecker::OnCreate()
	{
		const auto& players = OnlineManager::GetCurrentlyJoinedRoom().getPlayers();

		int localPlayerNumber = OnlineManager::GetLocalPlayer().getNumber();

		for (std::uint32_t i = 0; i < players.getSize(); ++i)
		{
			m_playerAliveCheckMap.insert(std::make_pair(players[i]->getNumber(), CheckState(true, 0.0f)));
		}
	}

	void OnlineAliveChecker::OnUpdate()
	{
		auto deltaTime = App::GetApp()->GetElapsedTime();

		m_aliveCheckCount += deltaTime;

		if (m_aliveCheckCount >= m_aliveCheckTime)
		{
			m_aliveCheckCount -= m_aliveCheckTime;
			OnlineManager::RaiseEvent(true, nullptr, 0, CONNECT_ALIVE_CHECK_EVENT_CODE);
			return;
		}

		for (auto& pair : m_playerAliveCheckMap)
		{
			if (pair.first == OnlineManager::GetLocalPlayer().getNumber())
			{
				continue;
			}

			auto& checkState = pair.second;
			checkState.responseCountTime += deltaTime;

			if (checkState.responseCountTime >= m_aliveCheckTime + m_aliveResponseTime && checkState.isAlive)
			{
				checkState.isAlive = false;
			}
		}
	}

	void OnlineAliveChecker::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode != CONNECT_ALIVE_CHECK_EVENT_CODE)
		{
			return;
		}

		auto& checkState = m_playerAliveCheckMap[playerNumber];
		checkState.isAlive = true;
		checkState.responseCountTime = 0.0f;
	}
}
}