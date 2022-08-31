#include "stdafx.h"
#include "MatchStageTransitioner.h"
#include "OnlineMatching.h"
#include "InputHelper.h"
#include "PlayerInputer.h"
#include "Scene.h"

namespace basecross
{
	MatchStageTransitioner::MatchStageTransitioner(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner), m_holdTimer(2) // 長押し時間
	{
	}

	void MatchStageTransitioner::GoToMainStage()
	{
		SimpleSoundManager::StopBGM();
		SimpleSoundManager::OnePlaySE(L"DecisionSE", 0.5f);
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage");
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
			if (gamePad.IsInputPush(XInputCode::A))
			{
				// 何秒か押し続けて遷移
				if (m_holdTimer.Count())
				{
					Online::OnlineManager::GetCurrentlyJoinedRoom().setIsOpen(false);
					onlineMatching->ShuffleTeam();
					GoToMainStage();
					Online::OnlineManager::RaiseEvent(false, nullptr, 0, TO_MAINSTAGE_EVENT_CODE);
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
			GoToMainStage();
			return;
		}
	}
}