#include "stdafx.h"
#include "PlayerOnlineController.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Item/HideItem.h"
#include "Item.h"
#include "Maruyama/Bullet/Object/BulletObjectBase.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Player/Component/WeponBase.h"
#include "Maruyama/Bullet/Component/ChargeBullet.h"
#include "PlayerControlManager.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "OnlineStatus.h"
#include "Maruyama/Utility/Component/RangeDestroyManager.h"

template<class T>
T ConvertByteData(const std::uint8_t* bytes)
{
	return *(T*)bytes;
}

namespace basecross
{
namespace Online
{
	struct OnlineMoveData
	{
		Vec3 moveVector;
		Vec3 forward;

		OnlineMoveData(const Vec3& moveVector, const Vec3& forward) :
			moveVector(moveVector),
			forward(forward)
		{

		}
	};

	struct ShotOnlineData
	{
		Vec3 position;
		Vec3 direction;
		std::uint32_t instanceId;

		ShotOnlineData(const Vec3& position, const Vec3& direction, const std::uint32_t instanceId) :
			position(position),
			direction(direction),
			instanceId(instanceId)
		{

		}
	};

	struct BulletDestroyData
	{
		std::uint32_t instanceId;
		Vec3 position;

		BulletDestroyData(std::uint32_t instanceId, const Vec3& position) :
			instanceId(instanceId),
			position(position)
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

	struct OnlineTeleportData
	{
		Vec3 teleportPosition;
		Vec3 cameraPosition;

		OnlineTeleportData(const Vec3& teleportPosition, const Vec3& cameraPosition) :
			teleportPosition(teleportPosition),
			cameraPosition(cameraPosition)
		{

		}
	};

	struct OnlineFindOjbectData
	{
		std::uint32_t objectId;
		int playerNumber;

		OnlineFindOjbectData(std::uint32_t objectId, int playerNumber) :
			objectId(objectId),
			playerNumber(playerNumber)
		{

		}
	};

	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	std::shared_ptr<PlayerOnlineController> PlayerOnlineController::GetPlayerOnlineController(int playerNumber)
	{
		for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
		{
			auto controller = gameObject->GetComponent<PlayerOnlineController>(false);

			if (controller && controller->GetOnlinePlayerNumber() == playerNumber)
			{
				return controller;
			}
		}

		return nullptr;
	}

	void PlayerOnlineController::UpdateCameraForward()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		Vec3 forward;

		if (!controlManager->IsUpdateCameraForward(&forward))
		{
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&forward, sizeof(Vec3), EXECUTE_CAMERA_FORWARD_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteCameraForward(int playerNumber, const Vec3& cameraForward)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		controlManager->ExecuteUpdateCameraForward(cameraForward);
	}

	void PlayerOnlineController::Move()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		Vec3 moveVector;
		Vec3 forward;

		if (!controlManager->TryMove(PlayerInputer::GetMoveDirection(), &moveVector, &forward))
		{
			return;
		}

		if (moveVector != m_beforeMoveVector)
		{
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineMoveData(moveVector, forward), sizeof(OnlineMoveData), EXECUTE_MOVE_EVENT_CODE);
		}

		m_beforeMoveVector = moveVector;
	}

	void PlayerOnlineController::ExecuteMove(int playerNumber, const Vec3& moveVector, const Vec3& forward)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();
		controlManager->ExecuteMove(moveVector, forward);
	}

	void PlayerOnlineController::Shot()
	{
		if (!PlayerInputer::IsShot())
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		std::shared_ptr<BulletObjectBase> bulletObject;

		if (!controlManager->TryShot(&bulletObject))
		{
			return;
		}
		
		auto bulletOnlineStatus = bulletObject->GetComponent<Online::OnlineStatus>();

		// ホストが弾の削除の管理を行う
		if (OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			auto chargeBullet = bulletObject->GetComponent<ChargeBullet>();
			chargeBullet->AddDestroyEventFunc([&](const std::shared_ptr<GameObject>& gameObject) { BulletDestroyed(gameObject); });
		}
		else
		{
			auto collision = bulletObject->GetComponent<Collision>();
			collision->SetUpdateActive(false);
		}

		auto bulletTransform = bulletObject->GetComponent<Transform>();

		bulletOnlineStatus->SetCreateInstanceId();

		m_bulletObjectMap.insert(std::make_pair(bulletOnlineStatus->GetInstanceId(), bulletObject));

		auto shotData = ShotOnlineData(bulletTransform->GetWorldPosition(), transform->GetForward(), bulletOnlineStatus->GetInstanceId());

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&shotData, sizeof(ShotOnlineData), EXECUTE_SHOT_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection, std::uint32_t instanceId)
	{
		if (playerNumber != m_onlinePlayerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		auto bulletObject = controlManager->ExecuteShot(bulletDirection);

		if (!bulletObject) {
			return;
		}

		auto bulletTransform = bulletObject->GetComponent<Transform>();
		auto bulletOnlineStatus = bulletObject->GetComponent<Online::OnlineStatus>();

		bulletTransform->SetWorldPosition(bulletPosition);

		// ホストが弾の削除の管理を行う
		if (OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			auto chargeBullet = bulletObject->GetComponent<ChargeBullet>();
			chargeBullet->AddDestroyEventFunc([&](const std::shared_ptr<GameObject>& gameObject) { BulletDestroyed(gameObject); });
		}
		else
		{
			auto collision = bulletObject->GetComponent<Collision>();
			collision->SetUpdateActive(false);
		}

		bulletOnlineStatus->ChangeInstanceId(instanceId);

		m_bulletObjectMap.insert(std::make_pair(bulletOnlineStatus->GetInstanceId(), bulletObject));
	}

	void PlayerOnlineController::BulletDestroyed(const std::shared_ptr<GameObject>& gameObject)
	{
		auto onlineStatus = gameObject->GetComponent<OnlineStatus>();

		auto destroyData = BulletDestroyData(onlineStatus->GetInstanceId(), gameObject->GetComponent<Transform>()->GetPosition());

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&destroyData, sizeof(BulletDestroyData), EXECUTE_BULLET_DESTROY_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteBulletDestroyEvent(std::uint32_t bulletInstanceId, const Vec3& position)
	{
		auto find = m_bulletObjectMap.find(bulletInstanceId);

		if (find == m_bulletObjectMap.end())
		{
			return;
		}

		auto bulletObject = find->second.lock();
		m_bulletObjectMap.erase(bulletInstanceId);

		if (!bulletObject)
		{
			return;
		}

		auto transform = bulletObject->GetComponent<Transform>();
		transform->SetWorldPosition(position);

		bulletObject->Destroy();
	}


	void PlayerOnlineController::Damaged(const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData)
	{
		if (!OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return;
		}

		auto attackerOnlineController = damageData.attacker->GetComponent<PlayerOnlineController>();
		auto damagedOnlineController = playerStatus->GetGameObject()->GetComponent<PlayerOnlineController>();
		OnlineDamageData data(attackerOnlineController->GetOnlinePlayerNumber(), damagedOnlineController->GetOnlinePlayerNumber(), damageData.value);

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineDamageData), EXECUTE_DAMAGE_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteDamagedEvent(int attackerPlayerNumber, int damagedPlayerNumber, int damage)
	{
		auto playerStatus = m_playerStatus.lock();

		if (!playerStatus || m_onlinePlayerNumber != damagedPlayerNumber)
		{
			return;
		}

		auto attackerOnlineController = GetPlayerOnlineController(attackerPlayerNumber);

		playerStatus->AddDamage(DamageData(damage, attackerOnlineController->GetGameObject()));
	}

	void PlayerOnlineController::TryAim()
	{
		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		bool isUpdate = false;
		bool isAim = true;

		if (PlayerInputer::GetInstance()->IsAim() && controlManager->TryUpdateAim(true))
		{
			isAim = true;
			isUpdate = true;
		}

		if (PlayerInputer::GetInstance()->IsAimRelease() && controlManager->TryUpdateAim(false))
		{
			isAim = false;
			isUpdate = true;
		}

		if (!isUpdate)
		{
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&isAim, sizeof(bool), EXECUTE_AIM_STATE_CHANGE_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteAimEvent(int playerNumber, bool isAim)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();
		controlManager->ExecuteUpdateAim(isAim);
	}

	void PlayerOnlineController::TeleportInputer() {

		auto controlManager = m_controlManager.lock();

		if (!controlManager)
		{
			return;
		}

		if (PlayerInputer::GetInstance()->IsOpenMap())
		{
			controlManager->TryOpenMap();
			return;
		}

		Vec3 teleportPosition;
		Vec3 cameraPosition;

		if (PlayerInputer::GetInstance()->IsDesitionDown() && !controlManager->TryTeleport(&teleportPosition, &cameraPosition))
		{
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineTeleportData(teleportPosition, cameraPosition),
				sizeof(OnlineTeleportData), EXECUTE_TELEPORT_EVENT_CODE);

			return;
		}
	}

	void PlayerOnlineController::ExecuteTeleportEvent(int playerNumber, const Vec3& teleportPosition, const Vec3& cameraPosition)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		controlManager->ExecuteTeleport(teleportPosition, cameraPosition);
	}

	void PlayerOnlineController::OpenHidePlaceInputer()
	{
		auto controlManager = m_controlManager.lock();

		if (!PlayerInputer::GetInstance()->IsDesitionDown() || !controlManager)
		{
			return;
		}

		std::shared_ptr<HidePlace> hidePlace = controlManager->GetCanOpenHidePlace();

		if (!hidePlace)
		{
			return;
		}

		auto onlineStatus = hidePlace->GetGameObject()->GetComponent<OnlineStatus>();
		std::uint32_t instanceId = onlineStatus->GetInstanceId();

		// ホストではないのなら
		if (!OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&instanceId, sizeof(std::uint32_t), TRY_OPEN_HIDEPLACE_EVENT_CODE);
			return;
		}

		if (!controlManager->TryAccessHidePlace(hidePlace))
		{
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineFindOjbectData(instanceId, m_onlinePlayerNumber), sizeof(OnlineFindOjbectData), EXECUTE_OPEN_HIDEPLACE_EVENT_CODE);

		auto hideItem = hidePlace->TakeOutHideItem();

		if (!hideItem)
		{
			return;
		}

		controlManager->TryAquisition(hideItem->GetItem());
	}

	void PlayerOnlineController::TryOpenHidePlaceEvent(int playerNumber, std::uint32_t instanceId)
	{
		auto& localPlayer = OnlineManager::GetLocalPlayer();

		// 自分がホストで無いか、対応したプレイヤーではないなら
		if (!localPlayer.getIsMasterClient() || m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();
		auto hidePlace = OnlineStatus::FindOnlineGameObject(instanceId)->GetComponent<HidePlace>();

		if (!controlManager->TryAccessHidePlace(hidePlace))
		{
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineFindOjbectData(instanceId, playerNumber), sizeof(OnlineFindOjbectData), EXECUTE_OPEN_HIDEPLACE_EVENT_CODE);

		auto hideItem = hidePlace->TakeOutHideItem();

		if (!hideItem)
		{
			return;
		}

		controlManager->TryAquisition(hideItem->GetItem());
	}

	void PlayerOnlineController::ExecuteOpenHidePlace(int playerNumber, std::uint32_t instanceId)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto controlManager = m_controlManager.lock();

		auto onlineGameObject = OnlineStatus::FindOnlineGameObject(instanceId);
		auto hidePlace = onlineGameObject->GetComponent<HidePlace>();
		controlManager->TryAccessHidePlace(hidePlace);

		auto hideItem = hidePlace->TakeOutHideItem();

		if (!hideItem)
		{
			return;
		}

		controlManager->TryAquisition(hideItem->GetItem());
	}

	void PlayerOnlineController::OnLateStart()
	{
		auto& owner = GetGameObject();

		auto playerStatus = owner->GetComponent<PlayerStatus>();
		auto damageFunc = [&](const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData) {Damaged(playerStatus, damageData); };
		playerStatus->AddFuncAddDamage(damageFunc);
		m_playerStatus = owner->GetComponent<PlayerStatus>();

		m_controlManager = owner->GetComponent<PlayerControlManager>();
	}

	void PlayerOnlineController::OnUpdate()
	{
		if (m_onlinePlayerNumber == OnlineManager::INVALID_ONLINE_PLAYER_NUMBER || m_onlinePlayerNumber != OnlineManager::GetLocalPlayer().getNumber())
		{
			return;
		}

		auto playerStatus = m_playerStatus.lock();

		if (playerStatus->IsDead())
		{
			return;
		}

		UpdateCameraForward();

		TeleportInputer();

		Move();

		OpenHidePlaceInputer();

		Shot();

		TryAim();

	}

	void PlayerOnlineController::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		// オンラインプレイヤー番号が無効なら
		if (m_onlinePlayerNumber == OnlineManager::INVALID_ONLINE_PLAYER_NUMBER)
		{
			return;
		}

		if (eventCode == EXECUTE_CAMERA_FORWARD_EVENT_CODE)
		{
			auto cameraForward = ConvertByteData<Vec3>(bytes);
			ExecuteCameraForward(playerNumber, cameraForward);
			return;
		}

		if (eventCode == EXECUTE_MOVE_EVENT_CODE)
		{
			auto moveData = ConvertByteData<OnlineMoveData>(bytes);
			ExecuteMove(playerNumber, moveData.moveVector, moveData.forward);
			return;
		}

		if (eventCode == EXECUTE_SHOT_EVENT_CODE)
		{
			auto data = ConvertByteData<ShotOnlineData>(bytes);
			ExecuteShot(playerNumber, data.position, data.direction, data.instanceId);
			return;
		}

		if (eventCode == EXECUTE_BULLET_DESTROY_EVENT_CODE)
		{
			auto data = ConvertByteData<BulletDestroyData>(bytes);
			ExecuteBulletDestroyEvent(data.instanceId, data.position);
		}

		if (eventCode == EXECUTE_DAMAGE_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineDamageData>(bytes);
			ExecuteDamagedEvent(data.attackerPlayerNumber, data.damagedPlayerNumber, data.damage);
			return;
		}

		if (eventCode == EXECUTE_TELEPORT_EVENT_CODE)
		{
			auto data = ConvertByteData<OnlineTeleportData>(bytes);
			ExecuteTeleportEvent(playerNumber, data.teleportPosition, data.cameraPosition);
			return;
		}

		if (eventCode == EXECUTE_AIM_STATE_CHANGE_EVENT_CODE)
		{
			bool isAim = ConvertByteData<bool>(bytes);
			ExecuteAimEvent(playerNumber, isAim);
			return;
		}

		if (eventCode == TRY_OPEN_HIDEPLACE_EVENT_CODE)
		{
			std::uint32_t instanceId = ConvertByteData<std::uint32_t>(bytes);
			TryOpenHidePlaceEvent(playerNumber, instanceId);
			return;
		}

		if (eventCode == EXECUTE_OPEN_HIDEPLACE_EVENT_CODE)
		{
			auto findGameObjectData = ConvertByteData<OnlineFindOjbectData>(bytes);
			ExecuteOpenHidePlace(findGameObjectData.playerNumber, findGameObjectData.objectId);
			return;
		}
	}
}
}