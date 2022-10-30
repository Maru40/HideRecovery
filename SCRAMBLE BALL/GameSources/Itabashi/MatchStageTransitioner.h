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

	/// <summary>
	/// MatchStageの遷移用コンポーネント
	/// </summary>
	class MatchStageTransitioner : public Online::OnlineComponent
	{
	public:
		static constexpr std::uint8_t TO_MAINSTAGE_EVENT_CODE = 40;
	private:
		std::weak_ptr<Online::OnlineMatching> m_onlineMatching;
		TimeCounter m_holdTimer;

		/// <summary>
		/// メインステージに遷移する
		/// </summary>
		void GoToMainStage(std::uint64_t seed);

		_NODISCARD std::uint64_t CreateStageInstanceIdSeed() const;

	public:
		MatchStageTransitioner(const std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate() override;

		void OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes) override;

		TimeCounter GetHoldTimer() { return m_holdTimer; }
	};
}