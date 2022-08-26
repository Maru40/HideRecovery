#include "stdafx.h"
#include "OnlineGameTimer.h"
#include "Watanabe/Manager/TimeManager.h"

namespace basecross
{
	OnlineGameTimer::OnlineGameTimer(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner),
		m_startCheckSpan(1.0f)
	{

	}

	bool OnlineGameTimer::IsStartable() const
	{
		return m_isStartable;
	}

	void OnlineGameTimer::StartCheck()
	{
		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return;
		}

		m_timeCount += App::GetApp()->GetElapsedTime();

		if (m_timeCount < m_startCheckSpan)
		{
			return;
		}

		m_timeCount -= m_startCheckSpan;

		if (m_startOkPlayerNumbers.size() >= Online::OnlineManager::GetCurrentlyJoinedRoom().getPlayerCount() - 1)
		{
			m_isGameStartable = true;

			for (auto& m_gameStartCountFuncs : m_gameStartCountFuncs)
			{
				m_gameStartCountFuncs();
			}

			Online::OnlineManager::RaiseEvent(false, nullptr, 0, GAMETIMER_START_EVENT_CODE);

			return;
		}

		Online::OnlineManager::RaiseEvent(false, nullptr, 0, GAMETIMER_START_CHECK_EVENT_CODE);
	}

	void OnlineGameTimer::OnCreate()
	{
		m_isStartable = true;
	}

	void OnlineGameTimer::OnUpdate()
	{
		if (!m_isGameStartable)
		{
			StartCheck();
			return;
		}

		if (!m_isTimerUpdate)
		{
			return;
		}

		TimeManager::GetInstance()->UpdateTime();
	}

	void OnlineGameTimer::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == GAMETIMER_START_CHECK_EVENT_CODE)
		{
			if (!IsStartable())
			{
				return;
			}

			auto raiseOption = ExitGames::LoadBalancing::RaiseEventOptions();
			raiseOption.setReceiverGroup(ExitGames::Lite::ReceiverGroup::MASTER_CLIENT);
			Online::OnlineManager::RaiseEvent(false, nullptr, 0, GAMETIMER_START_OK_EVENT_CODE, raiseOption);
			return;
		}

		if (eventCode == GAMETIMER_START_OK_EVENT_CODE)
		{
			if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
			{
				return;
			}

			m_startOkPlayerNumbers.insert(playerNumber);
		}

		if (eventCode == GAMETIMER_START_EVENT_CODE)
		{
			m_isGameStartable = true;

			for (auto& gameStartCountFunc : m_gameStartCountFuncs)
			{
				gameStartCountFunc();
			}

			return;
		}
	}

	void OnlineGameTimer::OnLeaveRoomEventAction(int playerNumber, bool isInactive)
	{
		m_startOkPlayerNumbers.erase(playerNumber);
	}
}