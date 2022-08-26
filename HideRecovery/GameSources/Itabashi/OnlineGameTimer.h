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

		float m_timeCount = 0.0f;

		float m_startCheckSpan;

		std::unordered_set<int> m_startOkPlayerNumbers;

		std::vector<std::function<void()>> m_gameStartCountFuncs;

		bool IsStartable() const;

		void StartCheck();

	public:
		OnlineGameTimer(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;

		void AddGameStartCountFuncs(const std::function<void()>& gameStartCountFunc) { m_gameStartCountFuncs.push_back(gameStartCountFunc); }
	};
}