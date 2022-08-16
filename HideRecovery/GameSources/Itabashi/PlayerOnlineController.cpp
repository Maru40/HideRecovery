#include "stdafx.h"
#include "PlayerOnlineController.h"
#include "ObjectMover.h"
#include "RotationController.h"
#include "PlayerInputer.h"
#include "MaruUtility.h"
#include "ItemAcquisitionManager.h"
#include "OwnHideItemManager.h"
#include "HideItem.h"
#include "Item.h"
#include "VelocityManager.h"

namespace basecross
{
namespace Online
{
	struct HideItemOnlineData
	{
		int playerNumber;
		Vec3 position;

		HideItemOnlineData(int playerNumber, const Vec3& position) :
			playerNumber(playerNumber),
			position(position)
		{

		}
	};

	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	std::vector<std::shared_ptr<PlayerOnlineController>> PlayerOnlineController::GetPlayerOnlineControllers() const
	{
		std::vector<std::shared_ptr<PlayerOnlineController>> onlineControllers;

		for (auto& gameObject : GetStage()->GetGameObjectVec())
		{
			auto controller = gameObject->GetComponent<PlayerOnlineController>(false);

			if (controller)
			{
				onlineControllers.push_back(controller);
			}
		}

		return onlineControllers;
	}

	void PlayerOnlineController::Move()
	{
		auto objectMover = m_objectMover.lock();

		if (!objectMover)
		{
			return;
		}

		auto moveVector = objectMover->Move(PlayerInputer::GetMoveDirection());

		if (moveVector != m_beforeMoveVector)
		{
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&(moveVector / App::GetApp()->GetElapsedTime()), sizeof(Vec3), EXECUTE_MOVE_EVENT_CODE);
		}

		auto rotationController = m_rotationController.lock();

		if (rotationController)
		{
			auto input = PlayerInputer::GetMoveDirection();
			auto direct = maru::Utility::CalcuCameraVec(Vec3(input.x, 0, input.y), GetStage()->GetView()->GetTargetCamera(), GetGameObject());

			rotationController->SetDirect(direct);
		}

		m_beforeMoveVector = moveVector;
	}

	void PlayerOnlineController::ExecuteMove(int playerNumber, const Vec3& moveVector)
	{
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		auto velocityManager = m_velocityManager.lock();

		if (velocityManager)
		{
			velocityManager->SetVelocity(moveVector);
		}
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

		if (localPlayer.getIsMasterClient() && !item->GetItemOwner())
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

		if (!localPlayer.getIsMasterClient() || m_playerNumber != localPlayer.getNumber())
		{
			return;
		}

		auto acquisitionManager = m_acquisitionManager.lock();

		auto item = Item::StageFindToItemId(GetStage(), itemId);

		if (!item || item->GetItemOwner())
		{
			return;
		}

		for (auto& onlineController : GetPlayerOnlineControllers())
		{
			if (onlineController->GetPlayerNumber() != playerNumber)
			{
				continue;
			}

			auto otherAcquisitionManager = onlineController->m_acquisitionManager.lock();

			if (otherAcquisitionManager)
			{
				otherAcquisitionManager->ItemAcquisition(item);
			}

			break;
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

	void PlayerOnlineController::TryItemHide()
	{
		if (!PlayerInputer::IsPutHideItem())
		{
			return;
		}

		auto hideItemManager = m_hideItemManager.lock();

		if (!hideItemManager || !hideItemManager->CanPut())
		{
			return;
		}

		auto& localPlayer = OnlineManager::GetLocalPlayer();
		int localNumber = localPlayer.getNumber();

		if (localPlayer.getIsMasterClient())
		{
			auto position = hideItemManager->PutHideItem();
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&HideItemOnlineData(localNumber, position), sizeof(HideItemOnlineData), EXECUTE_ITEM_HIDE_EVENT_CODE);
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&localNumber, sizeof(int), TRY_ITEM_HIDE_EVENT_CODE);
	}

	void PlayerOnlineController::TryItemHideEvent(int playerNumber)
	{
		auto& localPlayer = OnlineManager::GetLocalPlayer();

		if (!localPlayer.getIsMasterClient() || m_playerNumber != localPlayer.getNumber())
		{
			return;
		}

		for (auto& onlineController : GetPlayerOnlineControllers())
		{
			if (onlineController->GetPlayerNumber() != playerNumber)
			{
				continue;
			}

			auto hideItemManager = onlineController->m_hideItemManager.lock();

			if (hideItemManager && hideItemManager->CanPut())
			{
				auto position = hideItemManager->PutHideItem();
				OnlineManager::RaiseEvent(false, (std::uint8_t*)&HideItemOnlineData(playerNumber, position), sizeof(HideItemOnlineData), EXECUTE_ITEM_HIDE_EVENT_CODE);
			}

			break;
		}
	}

	void PlayerOnlineController::ExecuteItemHideEvent(int playerNumber, const Vec3& position)
	{
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		auto hideItemManager = m_hideItemManager.lock();

		if (hideItemManager)
		{
			hideItemManager->PutHideItem(position);
		}
	}

	void PlayerOnlineController::OnLateStart()
	{
		auto& owner = GetGameObject();
		m_objectMover = owner->GetComponent<Operator::ObjectMover>();
		m_rotationController = owner->GetComponent<RotationController>();
		m_acquisitionManager = owner->GetComponent<ItemAcquisitionManager>();
		m_hideItemManager = owner->GetComponent<OwnHideItemManager>(false);
		m_velocityManager = owner->GetComponent<VelocityManager>();
	}

	void PlayerOnlineController::OnUpdate()
	{
		if (m_playerNumber == 0 || m_playerNumber != OnlineManager::GetLocalPlayer().getNumber())
		{
			return;
		}

		Move();

		TryAquisition();

		TryItemHide();
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

		if (eventCode == TRY_ITEM_HIDE_EVENT_CODE)
		{
			TryItemHideEvent(playerNumber);
			return;
		}

		if (eventCode == EXECUTE_ITEM_HIDE_EVENT_CODE)
		{
			HideItemOnlineData data = *(HideItemOnlineData*)bytes;
			ExecuteItemHideEvent(data.playerNumber, data.position);
			return;
		}

		if (eventCode == EXECUTE_MOVE_EVENT_CODE)
		{
			Vec3 moveVector = *(Vec3*)bytes;
			ExecuteMove(playerNumber, moveVector);
			return;
		}
	}
}
}