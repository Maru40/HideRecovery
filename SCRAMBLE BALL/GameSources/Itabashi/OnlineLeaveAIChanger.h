﻿#pragma once
#include "Itabashi/OnlineManager.h"

namespace basecross
{
	class OnlinePlayerSynchronizer;
	class AIActiveChanger;
	class PlayerControlManager;

namespace Online
{
	class OnlineAliveChecker;

	class OnlineLeaveAIChanger : public OnlineComponent
	{
		std::weak_ptr<AIActiveChanger> m_aiActiveChanger;
		std::weak_ptr<OnlinePlayerSynchronizer> m_onlinePlayerSynchronizer;
		std::weak_ptr<OnlineAliveChecker> m_onlineAliveChecker;
		std::weak_ptr<PlayerControlManager> m_playerControlManager;

		bool m_beforeOnlineAlive = true;

		void ChangeAI();

	public:
		OnlineLeaveAIChanger(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDisconnected() override;

		void OnLeaveRoomEventAction(int playerNumber, bool isInactive) override;

		void OnMasterClientChanged(int id, int oldId) override;

		void SetOnlineAliveChecker(const std::shared_ptr<OnlineAliveChecker>& onlineAliveChecker) { m_onlineAliveChecker = onlineAliveChecker; }
	};
}
}