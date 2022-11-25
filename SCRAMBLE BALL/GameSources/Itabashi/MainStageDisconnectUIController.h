#pragma once
#include "OnlineManager.h"

namespace basecross
{
	namespace Online
	{
		class OnlineAliveChecker;
	}

	class MainStageDisconnectUIController : public Online::OnlineComponent
	{
		std::weak_ptr<Online::OnlineAliveChecker> m_onlineAliveChecker;
		std::weak_ptr<GameObject> m_groupUIObject;
		std::weak_ptr<GameObject> m_waitUIObject;
		std::weak_ptr<GameObject> m_disconnectUIObject;
		std::weak_ptr<GameObject> m_selectObject;

		bool m_isBeforeAlive = true;

	public:
		MainStageDisconnectUIController(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnUpdate() override;

		void OnDisconnected() override;

		void SetOnlineAliveChecker(const std::shared_ptr<Online::OnlineAliveChecker>& onlineAliveChecker) { m_onlineAliveChecker = onlineAliveChecker; }

		void SetGroupUIObject(const std::shared_ptr<GameObject>& groupUIObject) { m_groupUIObject = groupUIObject; }
		void SetWaitUIObject(const std::shared_ptr<GameObject>& waitUIObject) { m_waitUIObject = waitUIObject; }

		void SetDisconnectUIObject(const std::shared_ptr<GameObject>& disconnectUIObject) { m_disconnectUIObject = disconnectUIObject; }
		void SetSelectObject(const std::shared_ptr<GameObject>& selectObject) { m_selectObject = selectObject; }
	};
}