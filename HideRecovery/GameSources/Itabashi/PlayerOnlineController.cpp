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
#include "ChargeGun.h"
#include "ChargeBulletObject.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "TackleAttack.h"

template<class T>
T ConvertByteData(const std::uint8_t* bytes)
{
	return *(T*)bytes;
}

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

	struct ShotOnlineData
	{
		Vec3 position;
		Vec3 direction;

		ShotOnlineData(const Vec3& position, const Vec3& direction) :
			position(position),
			direction(direction)
		{

		}
	};

	struct OnlineDamageData
	{
		int attackerPlayerNumber;
		int damagedPlayerNumber;
		int damage;

		OnlineDamageData(int attackerPlayerNumber, int damagedPlayerNumber, int damage) :
			attackerPlayerNumber(attackerPlayerNumber),
			damagedPlayerNumber(damagedPlayerNumber),
			damage(damage)
		{

		}
	};

	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	std::shared_ptr<PlayerOnlineController> PlayerOnlineController::GetPlayerOnlineController(int playerNumber) const
	{
		for (auto& gameObject : GetStage()->GetGameObjectVec())
		{
			auto controller = gameObject->GetComponent<PlayerOnlineController>(false);

			if (controller && controller->GetPlayerNumber() == playerNumber)
			{
				return controller;
			}
		}

		return nullptr;
	}

	void PlayerOnlineController::Move()
	{
		auto objectMover = m_objectMover.lock();
		auto tackleAttack = m_tackleAttack.lock();

		if (!objectMover || !tackleAttack || tackleAttack->IsTackle())
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

		auto onlineController = GetPlayerOnlineController(playerNumber);

		if (onlineController)
		{
			auto otherAcquisitionManager = onlineController->m_acquisitionManager.lock();

			if (otherAcquisitionManager)
			{
				otherAcquisitionManager->ItemAcquisition(item);
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

		auto onlineController = GetPlayerOnlineController(playerNumber);

		if (!onlineController)
		{
			return;
		}

		auto hideItemManager = onlineController->m_hideItemManager.lock();

		if (hideItemManager && hideItemManager->CanPut())
		{
			auto position = hideItemManager->PutHideItem();
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&HideItemOnlineData(playerNumber, position), sizeof(HideItemOnlineData), EXECUTE_ITEM_HIDE_EVENT_CODE);
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

	void PlayerOnlineController::Shot()
	{
		if (!PlayerInputer::IsShot())
		{
			return;
		}

		auto transform = m_transform.lock();
		auto chargeGun = m_chargeGun.lock();

		if (!chargeGun)
		{
			return;
		}

		auto bulletObject = chargeGun->Shot(transform->GetForward());
		auto bulletTransform = bulletObject->GetComponent<Transform>();

		auto shotData = ShotOnlineData(bulletTransform->GetWorldPosition(), transform->GetForward());

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&shotData, sizeof(ShotOnlineData), EXECUTE_SHOT_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection)
	{
		if (playerNumber != m_playerNumber)
		{
			return;
		}

		auto chargeGun = m_chargeGun.lock();

		if (!chargeGun)
		{
			return;
		}

		auto bulletObject = chargeGun->Shot(bulletDirection);
		auto bulletTransform = bulletObject->GetComponent<Transform>();

		bulletTransform->SetWorldPosition(bulletPosition);
	}

	void PlayerOnlineController::Damaged(const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData)
	{
		if (!OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return;
		}

		auto attackerOnlineController = damageData.attacker->GetComponent<PlayerOnlineController>();
		auto damagedOnlineController = playerStatus->GetGameObject()->GetComponent<PlayerOnlineController>();
		OnlineDamageData data(attackerOnlineController->GetPlayerNumber(), damagedOnlineController->GetPlayerNumber(), damageData.value);

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineDamageData), EXECUTE_DAMAGE_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteDamagedEvent(int attackerPlayerNumber, int damagedPlayerNumber, int damage)
	{
		auto playerStatus = m_playerStatus.lock();

		if (!playerStatus || m_playerNumber != damagedPlayerNumber)
		{
			return;
		}

		auto attackerOnlineController = GetPlayerOnlineController(attackerPlayerNumber);

		playerStatus->AddDamage(DamageData(damage, attackerOnlineController->GetGameObject()));
	}

	void PlayerOnlineController::TryTackle()
	{
		if (!PlayerInputer::IsTackle())
		{
			return;
		}

		auto tackleAttack = m_tackleAttack.lock();

		if (!tackleAttack || tackleAttack->IsTackle())
		{
			return;
		}
		
		tackleAttack->ForceStartAttack();

		OnlineManager::RaiseEvent(false, (uint8_t*)&m_playerNumber, sizeof(int), EXECUTE_TACKLE_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteTackle(int playerNumber)
	{
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		auto tackleAttack = m_tackleAttack.lock();

		if (!tackleAttack)
		{
			return;
		}

		tackleAttack->StartAttack();
	}

	void PlayerOnlineController::OnLateStart()
	{
		auto& owner = GetGameObject();
		m_transform = owner->GetComponent<Transform>();
		m_objectMover = owner->GetComponent<Operator::ObjectMover>();
		m_rotationController = owner->GetComponent<RotationController>();
		m_acquisitionManager = owner->GetComponent<ItemAcquisitionManager>();
		m_hideItemManager = owner->GetComponent<OwnHideItemManager>(false);
		m_velocityManager = owner->GetComponent<VelocityManager>();
		m_chargeGun = owner->GetComponent<ChargeGun>();

		auto playerStatus = owner->GetComponent<PlayerStatus>();
		auto damageFunc = [&](const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData) {Damaged(playerStatus, damageData); };
		playerStatus->AddFuncAddDamage(damageFunc);
		m_playerStatus = owner->GetComponent<PlayerStatus>();

		m_tackleAttack = owner->GetComponent<TackleAttack>();
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

		Shot();

		TryTackle();
	}

	void PlayerOnlineController::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (m_playerNumber == 0)
		{
			return;
		}

		if (eventCode == TRY_ACQUISITION_EVENT_CODE)
		{
			int itemId = ConvertByteData<int>(bytes);
			TryAcquisitionEvent(itemId, playerNumber);
			return;
		}

		if (eventCode == EXECUTE_ACQUISITION_EVENT_CODE)
		{
			auto ownerShipData = ConvertByteData<ItemOwnerShipData>(bytes);
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
			auto data = ConvertByteData<HideItemOnlineData>(bytes);
			ExecuteItemHideEvent(data.playerNumber, data.position);
			return;
		}

		if (eventCode == EXECUTE_MOVE_EVENT_CODE)
		{
			auto moveVector = ConvertByteData<Vec3>(bytes);
			ExecuteMove(playerNumber, moveVector);
			return;
		}

		if (eventCode == EXECUTE_SHOT_EVENT_CODE)
		{
			auto data = ConvertByteData<ShotOnlineData>(bytes);
			ExecuteShot(playerNumber, data.position, data.direction);
			return;
		}

		if (eventCode == EXECUTE_DAMAGE_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineDamageData>(bytes);
			ExecuteDamagedEvent(data.attackerPlayerNumber, data.damagedPlayerNumber, data.damage);
			return;
		}

		if (eventCode == EXECUTE_TACKLE_EVENT_CODE)
		{
			ExecuteTackle(playerNumber);
			return;
		}
	}
}
}