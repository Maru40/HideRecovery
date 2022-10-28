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
#include "Maruyama/Player/Component/AccessHidePlace.h"
#include "Maruyama/StageObject/HidePlace.h"

namespace basecross
{
	void PlayerControlManager::OnLateStart()
	{
		m_teleport = GetGameObject()->GetComponent<Teleport>();
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>();
		m_acquisitionManager = GetGameObject()->GetComponent<ItemAcquisitionManager>();
		m_objectMover = GetGameObject()->GetComponent<Operator::ObjectMover>();
		m_useWeapon = GetGameObject()->GetComponent<UseWeapon>();
		m_goalAnimationController = GetGameObject()->GetComponent<GoalAnimationController>();
		m_hidePlaceOpener = GetGameObject()->GetComponent<HidePlaceOpener>();
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