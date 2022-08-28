
/*!
@file HidePlace.h
@brief HidePlace�Ȃ�
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlace.h"

#include "Watanabe/Component/BoxAnimator.h"

#include "ItemBag.h"
#include "Itabashi/Item.h"
#include "HideItem.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	HidePlace_Parametor::HidePlace_Parametor():
		HidePlace_Parametor(Vec3(0.0f))
	{}

	HidePlace_Parametor::HidePlace_Parametor(const Vec3& hidePositionOffset):
		hidePositionOffset(hidePositionOffset)
	{}

	//--------------------------------------------------------------------------------------
	/// �B���ꏊ
	//--------------------------------------------------------------------------------------

	HidePlace::HidePlace(const std::shared_ptr<GameObject>& objPtr) :
		HidePlace(objPtr, Parametor())
	{}

	HidePlace::HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr), m_param(parametor)
	{}

	void HidePlace::Open() {
		auto animator = GetGameObject()->GetComponent<BoxAnimator>(false);
		if (animator && animator->IsCurrentAnimator(BoxAnimationState::State::Close)) {
			animator->ChangeBoxAnimation(BoxAnimationState::State::Open);
		}

		auto item = GetHideItem();
		if (!item) {
			return;
		}

		item->GetGameObject()->SetActive(true);
		SetHideItem(nullptr);
	}

	void HidePlace::Close() {
		if (auto animator = GetGameObject()->GetComponent<BoxAnimator>(false)) {
			animator->ChangeBoxAnimation(BoxAnimationState::State::Close);
		}
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	void HidePlace::SetHideItem(const std::shared_ptr<HideItem>& item) {
		m_hideItem = item;

		auto animator = GetGameObject()->GetComponent<BoxAnimator>(false);
		if (item && animator && animator->IsCurrentAnimator(BoxAnimationState::State::Close)) {
			item->GetGameObject()->SetActive(false);
		}
	}

	std::shared_ptr<HideItem> HidePlace::GetHideItem() const {
		return m_hideItem.lock();
	}

}