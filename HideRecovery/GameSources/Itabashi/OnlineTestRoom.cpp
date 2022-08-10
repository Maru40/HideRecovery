#include "stdafx.h"
#include "OnlineTestRoom.h"
#include "OnlineManager.h"
#include "PlayerObject.h"

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