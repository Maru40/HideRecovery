
/*!
@file ItemAcquisitionManager.cpp
@brief ItemAcquisitionManager�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ItemAcquisitionManager.h"

#include "CollisionAction.h"
#include "ItemBase.h"

#include "MaruUtility.h"
#include "MaruAction.h"

#include "PlayerInputer.h"
#include "ItemBag.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	ItemAcquisitionManager_Parametor::ItemAcquisitionManager_Parametor() :
		ItemAcquisitionManager_Parametor(10.0f)
	{}

	ItemAcquisitionManager_Parametor::ItemAcquisitionManager_Parametor(const float searchRange) :
		searchRange(searchRange)
	{}

	//--------------------------------------------------------------------------------------
	/// �A�C�e���l���Ǘ��{��
	//--------------------------------------------------------------------------------------

	ItemAcquisitionManager::ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr), m_param(Parametor())
	{}

	void ItemAcquisitionManager::OnCreate() {

	}

	void ItemAcquisitionManager::OnLateStart() {
		//�A�C�e�����擾����B
		m_allFieldItems = maru::Utility::ConvertArraySharedToExweak(maru::Utility::FindComponents<ItemBase>());
	}

	void ItemAcquisitionManager::OnUpdate() {
		//�A�C�e�����͈͓��ɂ��邩���G
		for (auto& item : m_allFieldItems) {
			//�A�N�e�B�u��ԂłȂ��Ȃ��΂��B
			if (!item->GetGameObject()->IsUpdateActive()) {
				continue;
			}

			//���G�͈͓��Ȃ�true
			item->SetIsAcquisition(IsAcquisitionRange(item.GetShard()));
		}

		//�A�C�e���l���{�^������������
		if (PlayerInputer::IsItemAcquisition()) {
			Input_ItemAcquisition();
		}
	}

	void ItemAcquisitionManager::Input_ItemAcquisition() {
		//�o�b�O���������Ă��Ȃ������珈�����΂�
		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		if (!bag) {
			return;
		}

		for (auto& item : m_allFieldItems) {
			//�l���ł���A�C�e���A���A�o�b�O�ɓ���邱�Ƃ��\�Ȃ�
			if (item->IsAcquisition() && bag->IsAcquisition(item.GetShard())) {
				//�A�C�e��������B
				bag->AddItem(item.GetShard());

				//�A�C�e�����l�����ꂽ��Ԃɂ���B
				item->GetGameObject()->SetActive(false);
			}
		}
	}


	bool ItemAcquisitionManager::IsAcquisitionRange(const std::shared_ptr<ItemBase>& item) {
		auto toItemVec = maru::Utility::CalcuToTargetVec(GetGameObject(), item->GetGameObject());

		return toItemVec.length() < m_param.searchRange; //���G�͈͓��Ȃ�true
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------



	//�����{�c�֐�----------------------------------------------------

	void ItemAcquisitionManager::CreateSerchTriggerObject() {
		auto object = GetStage()->Instantiate<GameObject>(Vec3(0.0f), transform->GetQuaternion(), GetGameObject());
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(5.0f, 2.0f, 5.0f));

		//�����蔻��ݒ�
		auto collision = object->AddComponent<CollisionObb>();
		collision->SetAfterCollision(AfterCollision::None);
		collision->SetDrawActive(true);

		//�Փˎ��C�x���g�ݒ�
		auto collisionAction = object->AddComponent<maru::CollisionAction>();
	}

}