#include "stdafx.h"
#include "MatchStageTransitioner.h"
#include "OnlineMatching.h"
#include "PlayerInputer.h"
#include "Scene.h"

namespace basecross
{
	MatchStageTransitioner::MatchStageTransitioner(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

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

		if (PlayerInputer::IsDecision() && Online::OnlineManager::GetLocalPlayer().getIsMasterClient() && onlineMatching->GetPlayerCount() > 0)
		{
			Online::OnlineManager::GetCurrentlyJoinedRoom().setIsOpen(false);
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage");
			Online::OnlineManager::RaiseEvent(false, nullptr, 0, TO_MAINSTAGE_EVENT_CODE);
			return;
		}
	}

	void MatchStageTransitioner::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == TO_MAINSTAGE_EVENT_CODE)
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToMainStage");
			return;
		}
	}
}