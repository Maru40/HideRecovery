
/*!
@file ItemAcquisitionManager.cpp
@brief ItemAcquisitionManager�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "OwnHideItemManager.h"

#include "PlayerInputer.h"

#include "ItemBag.h"
#include "HideItem.h"

#include "HidePlace.h"

#include "MaruUtility.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	OwnHideItemManager_Parametor::OwnHideItemManager_Parametor():
		OwnHideItemManager_Parametor(2.0f)
	{}

	OwnHideItemManager_Parametor::OwnHideItemManager_Parametor(const float searchHidePlaceRange):
		searchHidePlaceRange(searchHidePlaceRange)
	{}

	//--------------------------------------------------------------------------------------
	/// �����Ă���B���A�C�e���̊Ǘ��{��
	//--------------------------------------------------------------------------------------

	OwnHideItemManager::OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr):
		OwnHideItemManager(objPtr, Parametor())
	{}

	OwnHideItemManager::OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		Component(objPtr), m_param(parametor), m_isFleePut(true)
	{}

	void OwnHideItemManager::OnUpdate() {
		if (PlayerInputer::IsPutHideItem()) {
			PutHideItem();
		}

		//�f�o�b�O�R�}���h
		if (PlayerInputer::GetInstance()->IsDownDown()) {
			m_isFleePut = !m_isFleePut;
		}
	}

	void OwnHideItemManager::PutHideItem() {
		if (!IsPut()) { //�u�����ԂłȂ��Ȃ�
			return;
		}

		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		if (!bag) {
			return;
		}

		auto hideItem = bag->GetHideItem();
		if (!hideItem) {
			return;
		}

		//�u���ꏊ�̎擾
		auto position = CalculateHidePosition();

		auto hideItemTrans = hideItem->GetGameObject()->GetComponent<Transform>();
		hideItemTrans->SetPosition(position);
		hideItem->GetGameObject()->SetUpdateActive(true);
		hideItem->GetGameObject()->SetDrawActive(true);

		bag->RemoveItem(hideItem);
	}

	bool OwnHideItemManager::IsPut() const {
		if (m_isFleePut) { //�t���[�v�b�g��ԂȂ�
			return true;
		}

		//�����łȂ��Ȃ�߂��̃I�u�W�F�N�g��T��
		return SearchNearHidePlace() ? true : false;
	}

	Vec3 OwnHideItemManager::CalculateHidePosition() const {
		if (m_isFleePut) {
			auto position = transform->GetPosition();
			position += transform->GetForward();
			position.y = 0.0f;
			return position;
		}

		auto hidePlace = SearchNearHidePlace();
		return hidePlace->GetHidePosition();
	}

	std::shared_ptr<HidePlace> OwnHideItemManager::SearchNearHidePlace() const {
		auto hidePlaces = maru::Utility::FindComponents<HidePlace>();

		for (auto& hidePlace : hidePlaces) {
			auto toHidePraceVec = maru::Utility::CalcuToTargetVec(GetGameObject(), hidePlace->GetGameObject());
			if (toHidePraceVec.length() < m_param.searchHidePlaceRange) {
				return hidePlace;
			}
		}

		return nullptr;
	}

}