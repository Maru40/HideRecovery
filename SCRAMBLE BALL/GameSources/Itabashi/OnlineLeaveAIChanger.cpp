#include "OnlineLeaveAIChanger.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"
#include "Itabashi/PlayerControlManager.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Enemy/Component/AIVirtualController.h"
#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"
#include "Itabashi/OnlineTransformSynchronization.h"
#include "Itabashi/AIActiveChanger.h"
#include "Itabashi/OnlineAliveChecker.h"

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

		m_aiActiveChanger.lock()->AIActiveChange(true);
		gameObject->GetComponent<Online::OnlineTransformSynchronization>()->SetIsMaster(true);
	}

	int OnlineLeaveAIChanger::GetCheckPlayerId() const
	{
		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		int onlinePlayerNumber = onlinePlayerSynchronizer->GetOnlinePlayerNumber();
		return (onlinePlayerNumber >= 0) ? onlinePlayerNumber : Online::OnlineManager::GetCurrentlyJoinedRoom().getMasterClientID();
	}

	void OnlineLeaveAIChanger::StateChange(bool isAlive)
	{
		if (isAlive)
		{
			return;
		}

		m_playerControlManager.lock()->StateReset();
	}

	void OnlineLeaveAIChanger::OnLateStart()
	{
		const auto& ownerObject = GetGameObject();
		m_onlinePlayerSynchronizer = ownerObject->GetComponent<OnlinePlayerSynchronizer>();
		m_aiActiveChanger = ownerObject->GetComponent<AIActiveChanger>();
		m_playerControlManager = ownerObject->GetComponent<PlayerControlManager>();

		auto onlineAliveChanger = m_onlineAliveChecker.lock();

		if (onlineAliveChanger)
		{
			onlineAliveChanger->AddPlayerStateChangeEvent( GetCheckPlayerId() ,[&](bool isAlive) { StateChange(isAlive); });
		}
	}

	void OnlineLeaveAIChanger::OnUpdate()
	{
	}

	void OnlineLeaveAIChanger::OnDisconnected()
	{
		m_playerControlManager.lock()->StateReset();
		m_aiActiveChanger.lock()->AIActiveChange(false);
	}

	void OnlineLeaveAIChanger::OnLeaveRoomEventAction(int playerNumber, bool isInactive)
	{
		auto onlinePlayerSynchronizer = m_onlinePlayerSynchronizer.lock();

		if (!OnlineManager::GetLocalPlayer().getIsMasterClient() || onlinePlayerSynchronizer->GetOnlinePlayerNumber() != playerNumber)
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