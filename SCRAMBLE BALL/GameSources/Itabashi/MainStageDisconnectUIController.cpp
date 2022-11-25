#include "MainStageDisconnectUIController.h"
#include "Itabashi/OnlineAliveChecker.h"

namespace basecross
{
	MainStageDisconnectUIController::MainStageDisconnectUIController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void MainStageDisconnectUIController::OnLateStart()
	{
	}

	void MainStageDisconnectUIController::OnUpdate()
	{
		auto onlineAliveChecker = m_onlineAliveChecker.lock();

		auto isAlive = onlineAliveChecker->IsMasterPlayerAlive();

		if (isAlive == m_isBeforeAlive)
		{
			return;
		}

		m_isBeforeAlive = isAlive;
		m_groupUIObject.lock()->SetActive(!isAlive);
		m_disconnectUIObject.lock()->SetActive(false);
	}

	void MainStageDisconnectUIController::OnDisconnected()
	{
		m_groupUIObject.lock()->SetActive(true);
		m_waitUIObject.lock()->SetActive(false);
		m_disconnectUIObject.lock()->SetActive(true);
		EventSystem::GetInstance(GetStage())->SetNowSelectableObject(m_selectObject.lock());
	}
}