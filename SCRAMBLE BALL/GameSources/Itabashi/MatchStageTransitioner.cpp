#include "stdafx.h"
#include "MatchStageTransitioner.h"
#include "OnlineMatching.h"
#include "Patch/InputHelper.h"
#include "Patch/PlayerInputer.h"
#include "Scene.h"
#include <random>

namespace basecross
{
	MatchStageTransitioner::MatchStageTransitioner(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner), m_holdTimer(2) // 長押し時間
	{
	}

	void MatchStageTransitioner::GoToMainStage(std::uint64_t seed)
	{
		SimpleSoundManager::StopBGM();
		SimpleSoundManager::OnePlaySE(L"DecisionSE", 0.5f);
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage", std::make_shared<std::uint64_t>(seed));
	}

	std::uint64_t MatchStageTransitioner::CreateStageInstanceIdSeed() const
	{
		return std::mt19937_64(std::random_device()())();
	}

	void MatchStageTransitioner::OnCreate()
	{
		m_onlineMatching = GetGameObject()->GetComponent<Online::OnlineMatching>(false);
	}

	void MatchStageTransitioner::OnUpdate()
	{
		auto onlineMatching = m_onlineMatching.lock();

		if (!onlineMatching)
		{
			return;
		}

		// キャンセルされたら、接続を切りタイトルに戻る
		if (PlayerInputer::IsCancel())
		{
			Online::OnlineManager::Disconnect();
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			return;
		}

		if (Online::OnlineManager::GetLocalPlayer().getIsMasterClient() && onlineMatching->GetPlayerCount() > 0)
		{
			const auto& gamePad = App::GetApp()->GetMyInputDevice()->GetXInputGamePad();
			// Aボタンが押されたら
			if (PlayerInputer::GetInstance()->IsDecision())
			{
				// 何秒か押し続けて遷移
				if (m_holdTimer.Count())
				{
					Online::OnlineManager::GetCurrentlyJoinedRoom().setIsOpen(false);
					onlineMatching->ShuffleTeam();

					auto seed = CreateStageInstanceIdSeed();
					GoToMainStage(seed);
					Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&seed, sizeof(std::uint64_t), TO_MAINSTAGE_EVENT_CODE);
					m_holdTimer.Reset();
					return;
				}
			}
			else
			{
				m_holdTimer.Reset();
			}
		}
	}

	void MatchStageTransitioner::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == TO_MAINSTAGE_EVENT_CODE)
		{
			GoToMainStage(*(std::uint64_t*)bytes);
			return;
		}
	}
}