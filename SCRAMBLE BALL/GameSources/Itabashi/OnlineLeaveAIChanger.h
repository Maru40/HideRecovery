#pragma once
#include "Itabashi/OnlineManager.h"

namespace basecross
{
	class OnlinePlayerSynchronizer;

namespace Online
{
	class OnlineLeaveAIChanger : public OnlineComponent
	{
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlinePlayerSynchronizer;
	public:
		OnlineLeaveAIChanger(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;
	};
}
}