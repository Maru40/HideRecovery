#include "stdafx.h"
#include "PlayerOnlineController.h"
#include "ObjectMover.h"
#include "RotationController.h"
#include "PlayerInputer.h"
#include "MaruUtility.h"
#include "ItemAcquisitionManager.h"
#include "HideItem.h"
#include "Item.h"


namespace basecross
{
namespace Online
{
	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	void PlayerOnlineController::TryAquisition()
	{
		auto acquisitionManager = m_acquisitionManager.lock();

		if (!acquisitionManager)
		{
			return;
		}

		std::shared_ptr<Item> item;

		if (!PlayerInputer::IsItemAcquisition() || !acquisitionManager->IsAcquisition(item))
		{
			return;
		}

		int itemId = item->GetItemId();
		auto& localPlayer = OnlineManager::GetLocalPlayer();

		if (localPlayer.getIsMasterClient())
		{
			acquisitionManager->ItemAcquisition(item);
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&ItemOwnerShipData(itemId, m_playerNumber), sizeof(ItemOwnerShipData), EXECUTE_ACQUISITION_EVENT_CODE);
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&itemId, sizeof(int), TRY_ACQUISITION_EVENT_CODE);
	}

	void PlayerOnlineController::TryAcquisitionEvent(int itemId, int playerNumber)
	{
		auto& localPlayer = OnlineManager::GetLocalPlayer();

		if (!localPlayer.getIsMasterClient())
		{
			return;
		}

		auto acquisitionManager = m_acquisitionManager.lock();

		auto item = Item::StageFindToItemId(GetStage(), itemId);

		if (!item || item->GetItemOwner())
		{
			return;
		}

		for (auto& gameObject : GetStage()->GetGameObjectVec())
		{
			auto onlinePlayer = gameObject->GetComponent<PlayerOnlineController>(false);

			if (!onlinePlayer)
			{
				continue;
			}

			if (onlinePlayer->GetPlayerNumber() == playerNumber)
			{
				auto otherAcquisitionManager = onlinePlayer->m_acquisitionManager.lock();

				if (otherAcquisitionManager)
				{
					otherAcquisitionManager->ItemAcquisition(item);
				}

				break;
			}
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&ItemOwnerShipData(itemId, playerNumber), sizeof(ItemOwnerShipData), EXECUTE_ACQUISITION_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteAcquisitionEvent(const ItemOwnerShipData& ownerShipData)
	{
		if (m_playerNumber != ownerShipData.playerNumber)
		{
			return;
		}

		auto acquisitionManager = m_acquisitionManager.lock();

		if (!acquisitionManager)
		{
			return;
		}

		acquisitionManager->ItemAcquisition(Item::StageFindToItemId(GetStage(), ownerShipData.itemId));
	}

	void PlayerOnlineController::OnCreate()
	{
		auto& owner = GetGameObject();
		m_objectMover = owner->GetComponent<Operator::ObjectMover>();
		m_rotationController = owner->GetComponent<RotationController>();
		m_acquisitionManager = owner->GetComponent<ItemAcquisitionManager>();
	}

	void PlayerOnlineController::OnUpdate()
	{
		if (m_playerNumber == 0 || m_playerNumber != OnlineManager::GetLocalPlayer().getNumber())
		{
			return;
		}

		auto objectMover = m_objectMover.lock();

		if (!objectMover)
		{
			return;
		}

		objectMover->Move(PlayerInputer::GetMoveDirection());

		auto rotationController = m_rotationController.lock();

		if (rotationController)
		{
			auto input = PlayerInputer::GetMoveDirection();
			auto direct = maru::Utility::CalcuCameraVec(Vec3(input.x, 0, input.y), GetStage()->GetView()->GetTargetCamera(), GetGameObject());

			rotationController->SetDirect(direct);
		}

		TryAquisition();
	}

	void PlayerOnlineController::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (m_playerNumber == 0)
		{
			return;
		}

		if (eventCode == TRY_ACQUISITION_EVENT_CODE)
		{
			int itemId = *(int*)bytes;
			TryAcquisitionEvent(itemId, playerNumber);
			return;
		}

		if (eventCode == EXECUTE_ACQUISITION_EVENT_CODE)
		{
			auto ownerShipData = *(ItemOwnerShipData*)bytes;
			ExecuteAcquisitionEvent(ownerShipData);
			return;
		}
	}
}
}