#pragma once
#include "stdafx.h"
#include "OnlineManager.h"
#include "Watanabe/Utility/TimeCounter.h"

namespace basecross
{
	namespace Online
	{
		class OnlineMatching;
	}
	class MatchStageTransitioner : public Online::OnlineComponent
	{
	public:
		static constexpr std::uint8_t TO_MAINSTAGE_EVENT_CODE = 40;
	private:
		std::weak_ptr<Online::OnlineMatching> m_onlineMatching;
		TimeCounter m_holdTimer;
		void GoToMainStage();

	public:
		MatchStageTransitioner(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;
	};
}