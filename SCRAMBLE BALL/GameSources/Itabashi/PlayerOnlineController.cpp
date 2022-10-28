#include "stdafx.h"
#include "PlayerOnlineController.h"
#include "ObjectMover.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Patch/PlayerInputer.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Player/Component/ItemAcquisitionManager.h"
#include "Maruyama/Player/Component/OwnHideItemManager.h"
#include "Maruyama/Item/HideItem.h"
#include "Item.h"
#include "VelocityManager.h"
#include "Maruyama/Player/Component/ChargeGun.h"
#include "Maruyama/Bullet/Object/ChargeBulletObject.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Player/Component/TackleAttack.h"
#include "Maruyama/Bullet/Component/ChargeBullet.h"
#include <random>

#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Player/Component/AccessHidePlace.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"

#include "Watanabe/Component/PlayerAnimator.h"
#include "Patch/SpringArmComponent.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "Maruyama/UI/2D/Component/FieldMap.h"
#include "Maruyama/UI/2D/Component/MapCursor.h"

#include "PlayerControlManager.h"
#include "Maruyama/StageObject/HidePlace.h"

#include "OnlineStatus.h"

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
		int instanceId;

		ShotOnlineData(const Vec3& position, const Vec3& direction, const int instanceId) :
			position(position),
			direction(direction),
			instanceId(instanceId)
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
		auto objectMover = m_objectMover.lock();
		auto useWepon = m_useWepon.lock();
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);

		if (!objectMover || animator->IsCurretAnimationState(PlayerAnimationState::State::Goal1))
		{
			return;
		}

		//特定のアニメーション中は移動を禁止する。
		if (animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_Floor) ||
			animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_HideObject))
		{
			return;
		}

		auto teleport = GetGameObject()->GetComponent<Teleport>(false);
		if (teleport && teleport->IsTeleporting()) {
			return;
		}

		auto moveVector = objectMover->Move(PlayerInputer::GetMoveDirection());

		auto rotationController = m_rotationController.lock();

		auto direction = rotationController->GetDirect();

		if (rotationController && !useWepon->IsAim())	//ローテーションがあり、Aim状態でないなら
		{
			auto input = PlayerInputer::GetMoveDirection();
			direction = maru::Utility::CalcuCameraVec(Vec3(input.x, 0, input.y), GetStage()->GetView()->GetTargetCamera(), GetGameObject());

			rotationController->SetDirect(direction);
		}

		if (moveVector != m_beforeMoveVector)
		{
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineMoveData(moveVector, direction), sizeof(OnlineMoveData), EXECUTE_MOVE_EVENT_CODE);
		}

		m_beforeMoveVector = moveVector;
	}

	void PlayerOnlineController::ExecuteMove(int playerNumber, const Vec3& moveVector, const Vec3& forward)
	{
		if (m_onlinePlayerNumber != playerNumber)
		{
			return;
		}

		auto velocityManager = m_velocityManager.lock();
		auto rotationController = m_rotationController.lock();

		velocityManager->SetVelocity(moveVector / App::GetApp()->GetElapsedTime());
		rotationController->SetDirect(forward);
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
		if (!bulletObject) {
			return;
		}

		auto bulletTransform = bulletObject->GetComponent<Transform>();
		
		auto chargeBullet = bulletObject->GetComponent<ChargeBullet>();

		// ホストが弾の削除の管理を行う
		if (OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			chargeBullet->AddDestroyEventFunc([&](const std::shared_ptr<GameObject>& gameObject) { BulletDestroyed(gameObject); });
		}

		int instanceId = CreateInstanceId();

		m_chargeBulletMap.insert(std::make_pair(instanceId, chargeBullet));
		chargeBullet->SetInstanceId(instanceId);

		auto shotData = ShotOnlineData(bulletTransform->GetWorldPosition(), transform->GetForward(), instanceId);

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&shotData, sizeof(ShotOnlineData), EXECUTE_SHOT_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteShot(int playerNumber, const Vec3& bulletPosition, const Vec3& bulletDirection, int instanceId)
	{
		if (playerNumber != m_onlinePlayerNumber)
		{
			return;
		}

		auto chargeGun = m_chargeGun.lock();

		if (!chargeGun)
		{
			return;
		}

		auto bulletObject = chargeGun->Shot(bulletDirection);
		if (!bulletObject) {
			return;
		}

		auto bulletTransform = bulletObject->GetComponent<Transform>();

		bulletTransform->SetWorldPosition(bulletPosition);

		auto chargeBullet = bulletObject->GetComponent<ChargeBullet>();

		// ホストが弾の削除の管理を行う
		if (OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			chargeBullet->AddDestroyEventFunc([&](const std::shared_ptr<GameObject>& gameObject) {BulletDestroyed(gameObject); });
		}

		m_chargeBulletMap.insert(std::make_pair(instanceId, chargeBullet));
		chargeBullet->SetInstanceId(instanceId);
	}

	void PlayerOnlineController::BulletDestroyed(const std::shared_ptr<GameObject>& gameObject)
	{
		auto chargeBullet = gameObject->GetComponent<ChargeBullet>();

		int instanceId = chargeBullet->GetInstanceId();

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&instanceId, sizeof(int), EXECUTE_BULLET_DESTROY_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteBulletDestroyEvent(int bulletInstanceId)
	{
		auto find = m_chargeBulletMap.find(bulletInstanceId);

		if (find == m_chargeBulletMap.end())
		{
			return;
		}

		GetStage()->RemoveGameObject<GameObject>(find->second->GetGameObject());
		find->second->GetGameObject();
		m_chargeBulletMap.erase(find);
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

	int PlayerOnlineController::CreateInstanceId() const
	{
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_int_distribution<int> distr(0, INT32_MAX);

		return distr(eng);
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

		if (!PlayerInputer::GetInstance()->IsDecision() || !controlManager)
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
		m_transform = owner->GetComponent<Transform>();
		m_objectMover = owner->GetComponent<Operator::ObjectMover>();
		m_rotationController = owner->GetComponent<RotationController>();
		m_velocityManager = owner->GetComponent<VelocityManager>();
		m_chargeGun = owner->GetComponent<ChargeGun>();

		auto playerStatus = owner->GetComponent<PlayerStatus>();
		auto damageFunc = [&](const std::shared_ptr<PlayerStatus>& playerStatus, const DamageData& damageData) {Damaged(playerStatus, damageData); };
		playerStatus->AddFuncAddDamage(damageFunc);
		m_playerStatus = owner->GetComponent<PlayerStatus>();

		m_useWepon = owner->GetComponent<UseWeapon>();

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

		if (eventCode == EXECUTE_BULLET_DESTROY_EVENT_CODE)
		{
			int instanceId = ConvertByteData<int>(bytes);
			ExecuteBulletDestroyEvent(instanceId);
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