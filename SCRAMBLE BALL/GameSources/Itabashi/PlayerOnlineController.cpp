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

	PlayerOnlineController::PlayerOnlineController(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{

	}

	std::shared_ptr<PlayerOnlineController> PlayerOnlineController::GetPlayerOnlineController(int playerNumber)
	{
		for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
		{
			auto controller = gameObject->GetComponent<PlayerOnlineController>(false);

			if (controller && controller->GetPlayerNumber() == playerNumber)
			{
				return controller;
			}
		}

		return nullptr;
	}

	void PlayerOnlineController::UpdateCameraForward()
	{
		auto camera = m_camera.lock();

		if (!camera)
		{
			return;
		}

		auto forward = camera->GetAt() - camera->GetEye();
		forward.normalize();

		if (m_cameraForward == forward)
		{
			return;
		}

		m_cameraForward = forward;

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&m_cameraForward, sizeof(Vec3), EXECUTE_CAMERA_FORWARD_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteCameraForward(int playerNumber, const Vec3& cameraForward)
	{
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		m_cameraForward = cameraForward;

		auto objectMover = m_objectMover.lock();
		auto useWeapon = m_useWepon.lock();

		objectMover->SetDefaultForward(cameraForward);
		useWeapon->SetDirection(cameraForward);
	}

	void PlayerOnlineController::Move()
	{
		auto objectMover = m_objectMover.lock();
		auto tackleAttack = m_tackleAttack.lock();
		auto useWepon = m_useWepon.lock();
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);

		if (!objectMover || !tackleAttack || tackleAttack->IsTackle() || animator->IsCurretAnimationState(PlayerAnimationState::State::Goal1))
		{
			return;
		}

		//特定のアニメーション中は移動を禁止する。
		if (animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_Floor) ||
			animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_HideObject)
		) {
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
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		auto velocityManager = m_velocityManager.lock();
		auto rotationController = m_rotationController.lock();

		velocityManager->SetVelocity(moveVector / App::GetApp()->GetElapsedTime());
		rotationController->SetDirect(forward);
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

		// ホストであり、アイテムに所持者がなければ取得する
		if (localPlayer.getIsMasterClient() && !item->GetItemOwner())
		{
			acquisitionManager->HideItemAcquisitionEvent(GetGameObject());
			acquisitionManager->ItemAcquisition(item);
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&ItemOwnerShipData(itemId, m_playerNumber), sizeof(ItemOwnerShipData), EXECUTE_ACQUISITION_EVENT_CODE);
			return;
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&itemId, sizeof(int), TRY_ACQUISITION_EVENT_CODE);
	}

	void PlayerOnlineController::TryAcquisitionEvent(int itemId, int playerNumber)
	{
		auto& localPlayer = OnlineManager::GetLocalPlayer();

		// 自分がマスターで無いか、対応したプレイヤーではないなら
		if (!localPlayer.getIsMasterClient() || m_playerNumber != localPlayer.getNumber())
		{
			return;
		}

		auto acquisitionManager = m_acquisitionManager.lock();

		auto item = Item::StageFindToItemId(GetStage(), itemId);

		// アイテムが見つからなかったかアイテムが既に誰かに所有されているか
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
				otherAcquisitionManager->HideItemAcquisitionEvent(onlineController->GetGameObject());
				otherAcquisitionManager->ItemAcquisition(item);
			}
		}

		OnlineManager::RaiseEvent(false, (std::uint8_t*)&ItemOwnerShipData(itemId, playerNumber), sizeof(ItemOwnerShipData), EXECUTE_ACQUISITION_EVENT_CODE);
	}

	void PlayerOnlineController::ExecuteAcquisitionEvent(const ItemOwnerShipData& ownerShipData)
	{
		// 取得したのが自分ではないのなら
		if (m_playerNumber != ownerShipData.playerNumber)
		{
			return;
		}

		auto acquisitionManager = m_acquisitionManager.lock();

		if (!acquisitionManager)
		{
			return;
		}

		acquisitionManager->HideItemAcquisitionEvent(GetGameObject());
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

	void PlayerOnlineController::TryAim()
	{
		auto useWeapon = m_useWepon.lock();

		auto teleport = GetGameObject()->GetComponent<Teleport>(false);

		// テレポート中ならば
		if (teleport && teleport->IsTeleporting()) {
			return;
		}

		auto goal = GetGameObject()->GetComponent<GoalAnimationController>(false);

		// ゴールアニメーション中ならば
		if (goal && goal->IsGoalAnimation()) {
			return;
		}

		if (!useWeapon)
		{
			return;
		}

		bool isAim = false;

		// 条件に当てはまればエイム状態にする
		if (PlayerInputer::GetInstance()->IsAim() && !useWeapon->IsAim())
		{
			isAim = true;
			useWeapon->SetIsAim(isAim);
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&isAim, sizeof(bool), EXECUTE_AIM_STATE_CHANGE_EVENT_CODE);
		}

		// 条件に当てはまればエイム状態を解除する
		if (PlayerInputer::GetInstance()->IsAimRelease() && useWeapon->IsAim())
		{
			isAim = false;
			useWeapon->SetIsAim(isAim);
			OnlineManager::RaiseEvent(false, (std::uint8_t*)&isAim, sizeof(bool), EXECUTE_AIM_STATE_CHANGE_EVENT_CODE);
		}

	}

	void PlayerOnlineController::ExecuteAimEvent(int playerNumber, bool isAim)
	{
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		auto useWeapon = m_useWepon.lock();

		useWeapon->SetIsAim(isAim);
	}

	int PlayerOnlineController::CreateInstanceId() const
	{
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_int_distribution<int> distr(0, INT32_MAX);

		return distr(eng);
	}

	void PlayerOnlineController::TeleportInputer() {

		auto teleport = m_teleport.lock();

		if (!teleport)
		{
			return;
		}

		if (teleport->IsTeleporting())
		{
			auto velocityManager = m_velocityManager.lock();
			velocityManager->ResetAll();
			return;
		}

		if (PlayerInputer::GetInstance()->IsOpenMap()) {

			teleport->OpenMap();
			return;
		}

		if (PlayerInputer::GetInstance()->IsDesitionDown()) {

			if (teleport->CanTeleport())
			{
				auto position = teleport->GetFieldMap()->GetMapCursor()->GetCursorFiledPosition();
				auto playerObject = dynamic_pointer_cast<PlayerObject>(GetGameObject());
				auto springArm = playerObject->GetArm()->GetComponent<SpringArmComponent>();
				auto childObject = springArm->GetChildObject();
				Vec3 offset(0.0f);
				if (auto lookAt = childObject->GetComponent<LookAtCameraManager>(false))
				{
					offset = lookAt->GetParametor().centerOffset;
				}

				auto& tpsCamera = GetStage()->GetView()->GetTargetCamera();
				auto tpsAt = tpsCamera->GetAt();
				auto tpsForward = tpsAt - tpsCamera->GetEye();

				position += offset + -tpsForward;

				teleport->SetCameraPosition(position);

				teleport->StartTeleport();
				OnlineManager::RaiseEvent(false, (std::uint8_t*)&OnlineTeleportData(teleport->GetTeleportPosition(), position),
					sizeof(OnlineTeleportData), EXECUTE_TELEPORT_EVENT_CODE);
			}
		}
	}

	void PlayerOnlineController::ExecuteTeleportEvent(int playerNumber, const Vec3& teleportPosition, const Vec3& cameraPosition)
	{
		if (m_playerNumber != playerNumber)
		{
			return;
		}

		auto teleport = m_teleport.lock();

		teleport->SetCameraPosition(cameraPosition);
		teleport->StartTeleport(teleportPosition);
	}

	void PlayerOnlineController::AccessHideInputer() {
		if (PlayerInputer::GetInstance()->IsDecision()) {
			if (auto accessHide = GetGameObject()->GetComponent<AccessHidePlace>(false)) {
				accessHide->Access();
			}
		}
	}

	void PlayerOnlineController::MapCursorMove() {
		
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
		m_useWepon = owner->GetComponent<UseWeapon>();

		m_teleport = owner->GetComponent<Teleport>(false);
	}

	void PlayerOnlineController::OnUpdate()
	{
		if (m_playerNumber == 0 || m_playerNumber != OnlineManager::GetLocalPlayer().getNumber())
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

		auto teleporter = m_teleport.lock();

		if (teleporter->IsTeleporting())
		{
			auto useWeapon = m_useWepon.lock();
			useWeapon->SetIsAim(false);
			return;
		}

		Move();

		TryAquisition();

		//TryItemHide();

		Shot();

		//TryTackle();

		TryAim();

		AccessHideInputer();

		MapCursorMove();
	}

	void PlayerOnlineController::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (m_playerNumber == 0)
		{
			return;
		}

		if (eventCode == EXECUTE_CAMERA_FORWARD_EVENT_CODE)
		{
			auto cameraForward = ConvertByteData<Vec3>(bytes);
			ExecuteCameraForward(playerNumber, cameraForward);
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

		if (eventCode == EXECUTE_TACKLE_EVENT_CODE)
		{
			ExecuteTackle(playerNumber);
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
	}
}
}