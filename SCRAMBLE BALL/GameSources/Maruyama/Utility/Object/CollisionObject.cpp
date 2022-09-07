/*!
@file CollisionObject.cpp
@brief CollisionObject実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "CollisionObject.h"

#include "Maruyama/Load/StageMapCSV.h"
#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/MaruAction.h"

namespace basecross {

	CollisionObjectManager::CollisionObjectManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr), m_param(parametor)
	{}

	void CollisionObjectManager::OnCreate() {
		const wstring& objName = m_param.objName;
		const wstring& folderName = m_param.folderName;
		const wstring& fileName = m_param.fileName;
		const Vec3& baseScale = m_param.baseScale;

		auto offsetDatas = StageMapCSV::GetPositionScaleRotations(objName, folderName, fileName);

		for (const auto& offsetData : offsetDatas) {
			m_objects.push_back(CreateCollisionObject(baseScale, offsetData));
		}
	}

	void CollisionObjectManager::OnUpdate() {
		for (auto& object : m_objects) {
			if (!object->GetDrawActive()) {
				GetGameObject()->SetDrawActive(object->GetDrawActive());
				return;
			}
		}

		GetGameObject()->SetDrawActive(true);
	}

	std::shared_ptr<GameObject> CollisionObjectManager::CreateCollisionObject(const Vec3& baseScale, const PositionScaleRotation& offsetData) {
		auto colObject = GetStage()->AddGameObject<GameObject>();
		auto offsetPosition = maru::Utility::ConvertForwardOffset(transform->GetForward(), offsetData.position);
		colObject->GetComponent<Transform>()->SetPosition(transform->GetPosition() + offsetPosition);
		colObject->GetComponent<Transform>()->SetScale(baseScale + offsetData.scale);
		colObject->GetComponent<Transform>()->Rotate(offsetData.rotation);
		auto quat = colObject->GetComponent<Transform>()->GetQuaternion();
		quat *= transform->GetQuaternion();
		colObject->GetComponent<Transform>()->SetQuaternion(quat);
		auto col = colObject->AddComponent<CollisionObb>();
		col->SetFixed(true);

		auto tagSet = GetGameObject()->GetTagSet();
		for (auto tag : tagSet) {
			colObject->AddTag(tag);
		}

		return colObject;
	}

	void CollisionObjectManager::RemoveCollisionObject() {
		maru::Action<void()> action;

		for (auto& object : m_objects) {
			action.AddFunction([&, object]() { GetStage()->RemoveGameObject<GameObject>(object.GetShard()); });
		}

		action.Invoke();
		m_objects.clear();
	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	void CollisionObjectManager::SetCollisionDrawActive(const bool isDrawActive) {
		for (auto& object : m_objects) {
			object->GetComponent<CollisionObb>()->SetDrawActive(isDrawActive);
		}
	}

	void CollisionObjectManager::SetCollisionUpdateActive(const bool isUpdateActive) {
		for (auto& object : m_objects) {
			object->GetComponent<CollisionObb>()->SetUpdateActive(isUpdateActive);
		}
	}

	std::vector<ex_weak_ptr<GameObject>> CollisionObjectManager::GetCollisionObjects() const {
		return m_objects;
	}

	void CollisionObjectManager::SettingCollisionObject(const Parametor& parametor) {
		RemoveCollisionObject();

		const Vec3& baseScale = m_param.baseScale;
		auto offsetDatas = StageMapCSV::GetPositionScaleRotations(parametor.objName, parametor.folderName, parametor.fileName);
		m_param = parametor;

		for (const auto& offsetData : offsetDatas) {
			m_objects.push_back(CreateCollisionObject(baseScale, offsetData));
		}
	}
}