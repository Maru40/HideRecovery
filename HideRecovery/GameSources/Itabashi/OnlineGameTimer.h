#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include <unordered_set>
#include <functional>

namespace basecross
{
	class OnlineGameTimer : public Online::OnlineComponent
	{
	public:
		static constexpr std::uint8_t GAMETIMER_START_CHECK_EVENT_CODE = 200;
		static constexpr std::uint8_t GAMETIMER_START_OK_EVENT_CODE = 201;
		static constexpr std::uint8_t GAMETIMER_START_EVENT_CODE = 202;

	private:

		bool m_isStartable = false;
		bool m_isGameStartable = false;
		bool m_isTimerUpdate = false;
		bool m_isBeforeTimeUp = false;

		float m_timeCount = 0.0f;

		float m_startCheckSpan;

		std::unordered_set<int> m_startOkPlayerNumbers;

		std::vector<std::function<void()>> m_gameStartCountFuncs;

		std::vector<std::function<void()>> m_gameFinishEventFuncs;

		bool IsStartable() const;

		void StartCheck();

		void GameCountStart();

	public:
		OnlineGameTimer(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnLateStart() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;

		void AddGameStartCountFunc(const std::function<void()>& gameStartCountFunc) { m_gameStartCountFuncs.push_back(gameStartCountFunc); }

		void AddGameFinishEventFunc(const std::function<void()>& gameFinishEventFunc) { m_gameFinishEventFuncs.push_back(gameFinishEventFunc); }

		void GameTimerStart() { m_isTimerUpdate = true; }

		void GameTimerStop() { m_isTimerUpdate = false; }
	};
}