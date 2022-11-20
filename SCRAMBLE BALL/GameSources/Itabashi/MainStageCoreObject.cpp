#include "MainStageCoreObject.h"
#include "OnlineGameTimer.h"
#include "OnlineGameItemManager.h"
#include "GamePlayerManager.h"
#include "MainStageTransitioner.h"
#include "OnlineAliveChecker.h"

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
		AddComponent<OnlineGameTimer>();
		AddComponent<OnlineGameItemManager>();
		AddComponent<GamePlayerManager>();
		AddComponent<Online::MainStageTransitioner>();
		AddComponent<Online::OnlineAliveChecker>();
	}
}
}