
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

#include "PlayerAnimationCtrl.h"

#include "TimeHelper.h"
#include "GameTimer.h"

#include "RotationController.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	OwnHideItemManager_Parametor::OwnHideItemManager_Parametor():
		OwnHideItemManager_Parametor(2.0f)
	{}

	OwnHideItemManager_Parametor::OwnHideItemManager_Parametor(const float searchHidePlaceRange) :
		searchHidePlaceRange(searchHidePlaceRange),
		putFloorAnimationTime(0.2f),
		putHideObjectAnimationTime(0.2f)
	{}

	//--------------------------------------------------------------------------------------
	/// �����Ă���B���A�C�e���̊Ǘ��{��
	//--------------------------------------------------------------------------------------

	OwnHideItemManager::OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr):
		OwnHideItemManager(objPtr, Parametor())
	{}

	OwnHideItemManager::OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		Component(objPtr), m_param(parametor), m_timer(new GameTimer(0)), m_isFleePut(true)
	{}

	void OwnHideItemManager::OnUpdate() {
		//�B���{�^������������
		if (PlayerInputer::IsPutHideItem()) {
			PutHideItem();
		}

		//�u���t�A�C�e���{�^��
		if (PlayerInputer::IsBluffPutItem()) {
			BluffPutHideItem();
		}

		//�^�C���x���g
		if (!m_timer->IsTimeUp()) {
			m_timer->UpdateTimer();
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
		auto hideItem = bag->GetHideItem();

		//�u���ꏊ�̎擾
		auto hidePosition = CalculateHidePosition();	//�B���ꏊ
		auto putEvent = [&, bag, hideItem, hidePosition]() {
			auto hideItemTrans = hideItem->GetGameObject()->GetComponent<Transform>();
			hideItemTrans->SetPosition(hidePosition);
			hideItem->GetGameObject()->SetActive(true);

			bag->RemoveItem(hideItem);
		};

		PlayAnimation(putEvent);
		Rotation();
	}

	void OwnHideItemManager::BluffPutHideItem() {
		if (!IsPut()) {
			return;
		}

		PlayAnimation(nullptr);
		Rotation();
	}

	void OwnHideItemManager::PlayAnimation(const std::function<void()>& putEvent) {
		auto animator = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false);

		//�A�j���[�V�������u����ԂȂ�ł��Ȃ�
		auto currentState = animator->GetCurrentAnimaiton();
		if (currentState == PlayerAnimationCtrl::State::PutItem_Floor || currentState == PlayerAnimationCtrl::State::PutItem_HideObject) {
			return;
		}

		if (m_isFleePut) {
			animator->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_Floor);
			m_timer->ResetTimer(m_param.putFloorAnimationTime, putEvent);
		}
		else {
			animator->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_HideObject);
			m_timer->ResetTimer(m_param.putHideObjectAnimationTime, putEvent);
		}
	}

	void OwnHideItemManager::Rotation() {
		auto hidePosition = CalculateHidePosition();	//�B���ꏊ

		//�����������@��ݒ�
		if (auto rotationController = GetGameObject()->GetComponent<RotationController>(false)) {
			auto direct = hidePosition - transform->GetPosition();
			rotationController->SetDirect(direct);
		}
	}

	bool OwnHideItemManager::IsPut() const {
		//�K�v�R���|�[�l���g�m�F
		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		auto animator = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false);
		if (!bag || !animator) { return false; }	//�K�v�R���|�[�l���g�����邩�ǂ���

		auto hideItem = bag->GetHideItem();	
		if (!hideItem) { return false; }	//hideItem�����邩�ǂ���

		if (m_isFleePut) { //�t���[�v�b�g��ԂȂ�
			return true;
		}

		//�����łȂ��Ȃ�߂��̃I�u�W�F�N�g��T��
		return SearchNearHidePlace() ? true : false;
	}

	Vec3 OwnHideItemManager::CalculateHidePosition() const {
		if (m_isFleePut) {
			auto position = transform->GetPosition();
			const float PutOffsetSize = 0.5f;
			position += transform->GetForward() * PutOffsetSize;
			position.y = -0.25f;
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