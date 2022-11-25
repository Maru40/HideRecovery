#include "MainStageCoreObject.h"
#include "OnlineGameTimer.h"
#include "OnlineGameItemManager.h"
#include "GamePlayerManager.h"
#include "MainStageTransitioner.h"
#include "OnlineAliveChecker.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Item/HideItem.h"

namespace basecross
{
namespace StageObject
{
	MainStageCoreObject::MainStageCoreObject(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{

	}

	void MainStageCoreObject::OnCreate()
	{
		auto onlineGameTimer = AddComponent<OnlineGameTimer>();
		std::weak_ptr<OnlineGameItemManager> weakGameItemManager = AddComponent<OnlineGameItemManager>();
		AddComponent<GamePlayerManager>();
		AddComponent<Online::MainStageTransitioner>();
		AddComponent<Online::OnlineAliveChecker>();

		onlineGameTimer->AddGameStartCountFunc([weakGameItemManager]()
			{
				if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
				{
					return;
				}

				auto gameItemManager = weakGameItemManager.lock();

				for (auto& hideItem : maru::Utility::FindComponents<HideItem>(gameItemManager->GetStage()))
				{
					gameItemManager->RandomHideItem(hideItem);
				}
			}
		);

		onlineGameTimer->AddGameFinishEventFunc([]() { SimpleSoundManager::OnePlaySE(L"GameSetSE", 0.25f); });
	}
}
}