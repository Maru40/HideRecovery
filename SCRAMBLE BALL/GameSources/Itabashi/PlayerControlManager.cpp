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

namespace basecross
{
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
	}

	bool PlayerControlManager::IsUpdateCameraForward(Vec3* forward)
	{
		auto camera = m_forwardCamera.lock();

		if (!camera)
		{
			return false;
		}

		auto cameraForward = camera->GetAt() - camera->GetEye();
		cameraForward.normalize();

		if (cameraForward == m_beforeCameraForward)
		{
			return false;
		}

		m_beforeCameraForward = cameraForward;

		if (forward)
		{
			*forward = cameraForward;
		}

		return true;
	}

	void PlayerControlManager::ExecuteUpdateCameraForward(const Vec3& forward)
	{
		m_beforeCameraForward = forward;

		auto objectMover = m_objectMover.lock();
		auto useWeapon = m_useWeapon.lock();

		objectMover->SetDefaultForward(forward);
		useWeapon->SetDirection(forward);
	}

	bool PlayerControlManager::TryMove(const Vec2& inputVector, Vec3* outputMoveVector, Vec3* outputForward)
	{
		auto objectMover = m_objectMover.lock();

		if (!objectMover)
		{
			return false;
		}

		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);

		//特定のアニメーション中は移動を禁止する。
		if (animator->IsCurretAnimationState(PlayerAnimationState::State::Goal1) ||
			animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_Floor) ||
			animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_HideObject))
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

		auto direction = rotationController->GetDirect();

		if (rotationController && !useWeapon->IsAim())	//ローテーションがあり、Aim状態でないなら
		{
			direction = maru::Utility::CalcuCameraVec(Vec3(inputVector.x, 0, inputVector.y), GetStage()->GetView()->GetTargetCamera(), GetGameObject());

			rotationController->SetDirect(direction);
		}

		if (outputMoveVector)
		{
			*outputMoveVector = moveVector;
		}

		if (outputForward)
		{
			*outputForward = direction;
		}

		return true;
	}

	void PlayerControlManager::ExecuteMove(const Vec3& moveVector, const Vec3& forward)
	{
		auto velocityManager = m_velocityManager.lock();
		auto rotationController = m_rotationController.lock();

		velocityManager->SetVelocity(moveVector / App::GetApp()->GetElapsedTime());
		rotationController->SetDirect(forward);
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

		acquisitionManager->HideItemAcquisitionEvent(GetGameObject());
		acquisitionManager->ItemAcquisition(item);

		return true;
	}

	bool PlayerControlManager::TryShot(std::shared_ptr<BulletObjectBase>* outputBulletObject)
	{
		auto transform = m_transform.lock();
		auto useWeapon = m_useWeapon.lock();

		if (!useWeapon)
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

		// 条件に当てはまればエイム状態にする
		if (isAim && !useWeapon->IsAim())
		{
			useWeapon->SetIsAim(isAim);
			return true;
		}

		// 条件に当てはまればエイム状態を解除する
		if (!isAim && useWeapon->IsAim())
		{
			useWeapon->SetIsAim(isAim);
			return true;
		}

		return false;
	}

	void PlayerControlManager::ExecuteUpdateAim(bool isAim)
	{
		auto useWeapon = m_useWeapon.lock();

		useWeapon->SetIsAim(isAim);
	}

	std::shared_ptr<HidePlace> PlayerControlManager::GetCanOpenHidePlace() const
	{
		auto hidePlaceOpener = m_hidePlaceOpener.lock();
		return hidePlaceOpener ? hidePlaceOpener->GetCanOpenaHidePlace() : nullptr;
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

		return true;
	}
}