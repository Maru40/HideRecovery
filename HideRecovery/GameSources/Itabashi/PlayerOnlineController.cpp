#include "stdafx.h"
#include "PlayerOnlineController.h"
#include "ObjectMover.h"
#include "RotationController.h"
#include "PlayerInputer.h"

namespace basecross
{
namespace Online
{
	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void PlayerOnlineController::OnCreate()
	{
		auto& owner = GetGameObject();
		m_objectMover = owner->GetComponent<Operator::ObjectMover>();
		m_rotationController = owner->GetComponent<RotationController>();
	}

	void PlayerOnlineController::OnUpdate()
	{
		if (m_playerNumber == 0 || m_playerNumber != OnlineManager::GetLocalPlayer().playerNumber)
		{
			return;
		}

		auto objectManager = m_objectMover.lock();

		if (!objectManager)
		{
			return;
		}

		objectManager->Move(PlayerInputer::GetMoveDirection());
	}

	void PlayerOnlineController::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (m_playerNumber == 0 || m_playerNumber != playerNumber || eventCode != EVENT_CODE)
		{
			return;
		}
	}
}
}