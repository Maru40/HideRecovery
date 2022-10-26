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

namespace basecross
{
	void PlayerControlManager::OnLateStart()
	{
		m_teleport = GetGameObject()->GetComponent<Teleport>();
		m_velocityManager = GetGameObject()->GetComponent<VelocityManager>();
		m_acquisitionManager = GetGameObject()->GetComponent<ItemAcquisitionManager>();
	}

	bool PlayerControlManager::TryFindAquisitionableItem(std::shared_ptr<Item>* findItem)
	{
		auto acquisitionManager = m_acquisitionManager.lock();

		std::shared_ptr<Item> item;

		if (!acquisitionManager || !acquisitionManager->IsAcquisition(item))
		{
			return false;
		}

		if (findItem)
		{
			*findItem = item;
		}

		return true;
	}

	bool PlayerControlManager::TryAquisition(const std::shared_ptr<Item>& item)
	{
		if (!item || !item->GetItemOwner())
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

	bool PlayerControlManager::TryAquisition(int itemId)
	{
		return TryAquisition(Item::StageFindToItemId(GetStage(), itemId));
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
}