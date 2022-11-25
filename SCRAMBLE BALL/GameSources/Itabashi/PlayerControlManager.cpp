#include "stdafx.h"
#include "PlayerControlManager.h"
#include "Maruyama/Player/Component/Teleport.h"
#include "Maruyama/UI/2D/Component/FieldMap.h"
#include "Maruyama/UI/2D/Component/MapCursor.h"
#include "Patch/SpringArmComponent.h"
#include "Maruyama/Camera/Component/LookAtCameraManager.h"
#include "VelocityManager.h"
#include "Maruyama/Player/Component/ItemAcquisitionManager.h"
#include "Item.h"
#include "ObjectMover.h"
#include "Maruyama/Player/Component/UseWeapon.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Player/Component/HidePlaceOpener.h"
#include "Maruyama/StageObject/HidePlace.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Player/Component/WeponBase.h"
#include "Maruyama/Bullet/Object/BulletObjectBase.h"
#include "Maruyama/Utility/Component/RotationController.h"

namespace basecross
{
	bool PlayerControlManager::IsControlableAnimation() const
	{
		auto animator = m_playerAnimator.lock();

		//特定のアニメーション中は操作を禁止する。
		return 
			!animator->IsCurretAnimationState(PlayerAnimationState::State::Goal1) &&
			!animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_Floor)&&
			!animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_HideObject);
	}

	void PlayerControlManager::OnLateStart()
	{
		auto ownerObject = GetGameObject();

		m_transform = ownerObject->GetComponent<Transform>();
		m_teleport = ownerObject->GetComponent<Teleport>();
		m_velocityManager = ownerObject->GetComponent<VelocityManager>();
		m_acquisitionManager = ownerObject->GetComponent<ItemAcquisitionManager>();
		m_objectMover = ownerObject->GetComponent<Operator::ObjectMover>();
		m_useWeapon = ownerObject->GetComponent<UseWeapon>();
		m_goalAnimationController = ownerObject->GetComponent<GoalAnimationController>();
		m_hidePlaceOpener = ownerObject->GetComponent<HidePlaceOpener>();
		m_playerAnimator = ownerObject->GetComponent<PlayerAnimator>();
		m_rotationController = ownerObject->GetComponent<RotationController>();

		m_defaultSpeed = m_objectMover.lock()->GetMoveSpeed();
	}

	void PlayerControlManager::OnUpdate()
	{
		if (m_playerAnimator.lock()->IsCurretAnimationState(PlayerAnimationState::State::PutItem_Floor))
		{
			m_objectMover.lock()->Move(Vec2());
		}
	}

	bool PlayerControlManager::TryUpdateDefaultForward(const Vec3& forward)
	{
		if (forward == m_beforeDefaultForward)
		{
			return false;
		}

		m_beforeDefaultForward = forward;

		ExecuteUpdateDefaultForward(forward);

		return true;
	}

	void PlayerControlManager::ExecuteUpdateDefaultForward(const Vec3& forward)
	{
		m_beforeDefaultForward = forward;

		//auto objectMover = m_objectMover.lock();
		auto useWeapon = m_useWeapon.lock();

		//objectMover->SetDefaultForward(forward);
		useWeapon->SetDirection(forward);
	}

	bool PlayerControlManager::TryMove(const Vec2& inputVector, Vec3* outputMoveVector)
	{
		auto objectMover = m_objectMover.lock();

		if (!objectMover)
		{
			return false;
		}

		if (!IsControlableAnimation())
		{
			return false;
		}

		auto teleport = GetGameObject()->GetComponent<Teleport>(false);
		if (teleport && teleport->IsTeleporting()) {
			return false;
		}
		auto useWeapon = m_useWeapon.lock();

		Vec3 moveVector = objectMover->Move(inputVector);

		auto rotationController = m_rotationController.lock();

		if (rotationController && !useWeapon->IsAim())	//ローテーションがあり、Aim状態でないなら
		{
			Vec3 direction = moveVector;
			direction.y = 0;
			direction = direction.GetNormalized();

			rotationController->SetDirection(direction);
		}

		if (outputMoveVector)
		{
			*outputMoveVector = moveVector;
		}

		return true;
	}

	void PlayerControlManager::ExecuteMove(const Vec3& moveVector)
	{
		auto velocityManager = m_velocityManager.lock();
		auto useWeapon = m_useWeapon.lock();

		velocityManager->SetVelocity(moveVector);

		if (!useWeapon->IsAim())
		{
			auto rotationController = m_rotationController.lock();

			Vec3 direction = moveVector;
			direction.y = 0;
			direction = direction.GetNormalized();

			rotationController->SetDirection(direction);
		}
	}

	bool PlayerControlManager::TryAquisition(const std::shared_ptr<Item>& item)
	{
		if (!item || item->GetItemOwner())
		{
			return false;
		}

		auto acquisitionManager = m_acquisitionManager.lock();

		if (!acquisitionManager)
		{
			return false;
		}

		auto animator = m_playerAnimator.lock();

		if (animator->IsCurretAnimationState(PlayerAnimationState::State::Goal1))
		{
			return false;
		}

		acquisitionManager->HideItemAcquisitionEvent(GetGameObject());
		acquisitionManager->ItemAcquisition(item);

		return true;
	}

	bool PlayerControlManager::TryShot(std::shared_ptr<BulletObjectBase>* outputBulletObject)
	{
		auto transform = m_transform.lock();
		auto useWeapon = m_useWeapon.lock();

		if (!useWeapon || !useWeapon->IsAim())
		{
			return false;
		}

		auto weapon = useWeapon->GetWepon();

		if (!weapon)
		{
			return false;
		}

		auto bulletObject = weapon->Shot(transform->GetForward());

		if (!bulletObject)
		{
			return false;
		}

		if (outputBulletObject)
		{
			*outputBulletObject = bulletObject;
		}

		return true;
	}

	std::shared_ptr<BulletObjectBase> PlayerControlManager::ExecuteShot(const Vec3& direction)
	{
		auto useWeapon = m_useWeapon.lock();

		if (!useWeapon)
		{
			return nullptr;
		}

		auto weapon = useWeapon->GetWepon();

		if (!weapon)
		{
			return nullptr;
		}

		return weapon->Shot(direction);
	}

	bool PlayerControlManager::TryOpenMap()
	{
		auto teleport = m_teleport.lock();

		if (!teleport)
		{
			return false;
		}

		teleport->OpenMap();
		return true;
	}

	bool PlayerControlManager::TryTeleport(Vec3* teleportPosition, Vec3* cameraPosition)
	{
		auto teleport = m_teleport.lock();

		if (!teleport || teleport->IsTeleporting() || !teleport->CanTeleport())
		{
			return false;
		}

		auto velocityManager = m_velocityManager.lock();
		velocityManager->ResetAll();


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
		auto& tpsAt = tpsCamera->GetAt();
		auto tpsForward = tpsAt - tpsCamera->GetEye();

		position += offset + -tpsForward;

		teleport->SetCameraPosition(position);

		teleport->StartTeleport();

		*teleportPosition = teleport->GetTeleportPosition();
		*cameraPosition = position;

		return true;
	}

	void PlayerControlManager::ExecuteTeleport(const Vec3& teleportPosition, const Vec3& cameraPosition)
	{
		auto teleport = m_teleport.lock();

		if (!teleport)
		{
			return;
		}

		auto velocityManager = m_velocityManager.lock();
		velocityManager->ResetAll();

		teleport->SetCameraPosition(cameraPosition);
		teleport->StartTeleport(teleportPosition);
	}

	bool PlayerControlManager::TryUpdateAim(bool isAim)
	{
		auto useWeapon = m_useWeapon.lock();

		if (!useWeapon)
		{
			return false;
		}

		auto teleport = GetGameObject()->GetComponent<Teleport>(false);

		// テレポート中ならば
		if (teleport && teleport->IsTeleporting())
		{
			return false;
		}

		auto goalAnimationController = m_goalAnimationController.lock();

		// ゴールアニメーション中ならば
		if (goalAnimationController && goalAnimationController->IsGoalAnimation()) {
			return false;
		}

		auto objectMover = m_objectMover.lock();

		// 条件に当てはまればエイム状態にする
		if (isAim && !useWeapon->IsAim())
		{
			useWeapon->SetIsAim(isAim);
			objectMover->SetMoveSpeed(m_defaultSpeed - useWeapon->GetWeaponWeight());
			return true;
		}

		// 条件に当てはまればエイム状態を解除する
		if (!isAim && useWeapon->IsAim())
		{
			useWeapon->SetIsAim(isAim);
			objectMover->SetMoveSpeed(m_defaultSpeed);

			return true;
		}

		return false;
	}

	void PlayerControlManager::ExecuteUpdateAim(bool isAim)
	{
		auto useWeapon = m_useWeapon.lock();
		auto objectMover = m_objectMover.lock();

		useWeapon->SetIsAim(isAim);

		float speed = isAim ? m_defaultSpeed - useWeapon->GetWeaponWeight() : m_defaultSpeed;

		objectMover->SetMoveSpeed(speed);
	}

	std::shared_ptr<HidePlace> PlayerControlManager::GetCanOpenHidePlace() const
	{
		auto hidePlaceOpener = m_hidePlaceOpener.lock();
		return hidePlaceOpener ? hidePlaceOpener->GetCanOpenaHidePlace() : nullptr;
	}

	std::shared_ptr<Item> PlayerControlManager::GetCanAquisitionItem() const
	{
		auto acquisitionManager = m_acquisitionManager.lock();
		auto canAquisitionItems = acquisitionManager->GetCanAcquisitionItems();

		return canAquisitionItems.empty() ? nullptr : canAquisitionItems[0];
	}

	bool PlayerControlManager::TryAccessHidePlace(const std::shared_ptr<HidePlace>& hidePlace)
	{
		auto hidePlaceOpener = m_hidePlaceOpener.lock();

		if (!hidePlaceOpener)
		{
			return false;
		}

		if (!hidePlace || hidePlace->IsOpen())
		{
			return false;
		}

		hidePlace->Open();

		//アニメーションの再生
		if (auto animator = m_playerAnimator.lock()) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::PutItem_Floor);
		}

		//向きたい方向の指定
		if (auto rotationController = m_rotationController.lock()) {
			auto hidePlacePosition = hidePlace->GetGameObject()->GetComponent<Transform>()->GetPosition();
			auto toVec = hidePlacePosition - transform->GetPosition();
			rotationController->SetDirection(toVec);
		}

		StateReset();


		return true;
	}

	void PlayerControlManager::ResetMoveSpeed() {
		auto objectMover = m_objectMover.lock();

		if (objectMover) {
			objectMover->SetMoveSpeed(m_defaultSpeed);
		}
	}

	void PlayerControlManager::StateReset()
	{
		auto objectMover = m_objectMover.lock();

		if (objectMover)
		{
			objectMover->Move(Vec2());
		}

		auto useWeapon = m_useWeapon.lock();

		if (useWeapon)
		{
			useWeapon->SetIsAim(false);
		}
	}
}