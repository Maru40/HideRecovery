#include "stdafx.h"
#include "OnlineTestRoom.h"
#include "OnlineManager.h"
#include "Maruyama/Player/Object/PlayerObject.h"

namespace basecross
{
namespace Online
{
	OnlineTestRoom::OnlineTestRoom(const std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{

	}

	void OnlineTestRoom::OnCreate()
	{
		AddComponent<OnlineTester>();
	}
}
}