#include "OnlineLeaveAIChanger.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/AIVirtualController.h"
#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"
#include "Itabashi/OnlineTransformSynchronization.h"

namespace basecross
{
namespace Online
{
	OnlineLeaveAIChanger::OnlineLeaveAIChanger(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void OnlineLeaveAIChanger::ChangeAI()
	{
		auto gameObject = GetGameObject();

		gameObject->GetComponent<Enemy::EnemyBase>()->SetUpdateActive(true);
		gameObject->GetComponent<SeekTarget>()->SetUpdateActive(true);
		gameObject->GetComponent<SelfAstarNodeController>()->SetUpdateActive(true);
		gameObject->GetComponent<TargetManager>()->SetUpdateActive(true);
		gameObject->GetComponent<AIVirtualController>()->SetUpdateActive(true);
		gameObject->GetComponent<Enemy::AIPlayerStator>()->SetUpdateActive(true);
		gameObject->GetComponent<Online::OnlineTransformSynchronization>()->SetIsMaster(true);
	}

	void OnlineLeaveAIChanger::OnLateStart()
	{
		m_onlinePlayerSynchronizer = GetGameObject()->GetComponent<OnlinePlayerSynchronizer>();
	}

	void OnlineLeaveAIChanger::OnDisconnected()
	{
		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		if (onlinePlayerSynchronizer->GetOnlinePlayerNumber() == OnlineManager::GetLocalPlayer().getNumber())
		{
			return;
		}

		ChangeAI();
	}

	void OnlineLeaveAIChanger::OnLeaveRoomEventAction(int playerNumber, bool isInactive)
	{
		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		if (onlinePlayerSynchronizer->GetOnlinePlayerNumber() != playerNumber)
		{
			return;
		}

		ChangeAI();
	}

	void OnlineLeaveAIChanger::OnMasterClientChanged(int id, int oldId)
	{
		if (OnlineManager::GetLocalPlayer().getNumber() != id)
		{
			return;
		}

		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		if (onlinePlayerSynchronizer->GetOnlinePlayerNumber() >= 0)
		{
			return;
		}

		ChangeAI();
	}
}
}