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
			GameCountStart();

			Online::OnlineManager::RaiseEvent(false, nullptr, 0, GAMETIMER_START_EVENT_CODE);

			return;
		}

		Online::OnlineManager::RaiseEvent(false, nullptr, 0, GAMETIMER_START_CHECK_EVENT_CODE);
	}

	void OnlineGameTimer::GameCountStart()
	{
		m_isGameStartable = true;
		m_timeCount = 0.0f;
		m_count = 1;
		SimpleSoundManager::OnePlaySE(L"StartCountSE", 0.5f);

		for (auto& m_gameStartCountFuncs : m_gameStartCountFuncs)
		{
			m_gameStartCountFuncs();
		}
	}

	void OnlineGameTimer::TimeSkipToFinish()
	{
		auto& timeManager = TimeManager::GetInstance();

		while (timeManager->GetTimer().GetElaspedTime() < FINISH_COUNT_ELAPSED_TIME)
		{
			timeManager->UpdateTime();
		}
	}

	void OnlineGameTimer::OnCreate()
	{
		m_isStartable = true;
	}

	void OnlineGameTimer::OnLateStart()
	{
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
			m_timeCount += App::GetApp()->GetElapsedTime();

			if (m_timeCount >= 1.0f && m_count < 3)
			{
				m_timeCount -= 1.0f;
				SimpleSoundManager::OnePlaySE(L"StartCountSE", 0.5f);
				++m_count;
			}

			return;
		}

		TimeManager::GetInstance()->UpdateTime();

		// デバッグ用
		if (App::GetApp()->GetMyInputDevice()->GetKeyBoard().IsInputDown(itbs::Input::KeyCode::X))
		{
			TimeSkipToFinish();
		}

		auto timeCounter = TimeManager::GetInstance()->GetTimer();

		bool isFinishCount = timeCounter.GetElaspedTime() >= FINISH_COUNT_ELAPSED_TIME;
		bool isTimeUp = timeCounter.IsTimeUp();

		if (!m_isBeforeFinishCount && isFinishCount)
		{
			m_timeCount = 0.0f;
			m_count = 1;
			SimpleSoundManager::OnePlaySE(L"StartCountSE", 0.5f);

			for (auto& finishCountEventFunc : m_gameFinishCountEventFuncs)
			{
				finishCountEventFunc();
			}
		}

		if (isFinishCount)
		{
			m_timeCount += App::GetApp()->GetElapsedTime();
			
			if (m_timeCount >= 1.0f && m_count < 5)
			{
				m_timeCount -= 1.0f;
				SimpleSoundManager::OnePlaySE(L"StartCountSE", 0.5f);
				++m_count;
			}
		}

		if (!m_isBeforeTimeUp && isTimeUp)
		{
			for (auto& finishEventFunc : m_gameFinishEventFuncs)
			{
				finishEventFunc();
			}
		}

		m_isBeforeFinishCount = isFinishCount;
		m_isBeforeTimeUp = isTimeUp;
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
			GameCountStart();

			return;
		}
	}

	void OnlineGameTimer::OnLeaveRoomEventAction(int playerNumber, bool isInactive)
	{
		m_startOkPlayerNumbers.erase(playerNumber);
	}
}