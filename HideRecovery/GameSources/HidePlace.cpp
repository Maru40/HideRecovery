
/*!
@file HidePlace.h
@brief HidePlaceなど
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlace.h"

#include "Watanabe/Component/BoxAnimator.h"

#include "ItemBag.h"
#include "Itabashi/Item.h"
#include "HideItem.h"

namespace basecross {

	int HidePlace::m_objectCount = 1;

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	HidePlace_Parametor::HidePlace_Parametor():
		HidePlace_Parametor(Vec3(0.0f))
	{}

	HidePlace_Parametor::HidePlace_Parametor(const Vec3& hidePositionOffset):
		hidePositionOffset(hidePositionOffset)
	{}

	//--------------------------------------------------------------------------------------
	/// 隠す場所
	//--------------------------------------------------------------------------------------

	HidePlace::HidePlace(const std::shared_ptr<GameObject>& objPtr) :
		HidePlace(objPtr, Parametor())
	{}

	HidePlace::HidePlace(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		Component(objPtr),
		m_param(parametor),
		m_openSoundClip(L"OpenBoxSE", false, 0.5f)
	{}

	void HidePlace::OnCreate() {
		GetGameObject()->AddComponent<SoundEmitter>();

		m_objectId = m_objectCount;
		++m_objectCount;
	}

	void HidePlace::OnLateStart() {
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>();
	}

	void HidePlace::Open() {
		auto animator = GetGameObject()->GetComponent<BoxAnimator>(false);
		if (animator && animator->IsCurrentAnimator(BoxAnimationState::State::Close)) {
			animator->ChangeBoxAnimation(BoxAnimationState::State::Open);
		}

		if (auto soundEmitter = m_soundEmitter.lock()) {
			soundEmitter->PlaySoundClip(m_openSoundClip);
		}

		//auto item = GetHideItem();
		//if (!item) {
		//	return;
		//}

		//item->GetGameObject()->SetActive(true);
		//SetHideItem(nullptr);
	}

	void HidePlace::Close() {
		if (auto animator = GetGameObject()->GetComponent<BoxAnimator>(false)) {
			animator->ChangeBoxAnimation(BoxAnimationState::State::Close);
		}

		//auto item = GetHideItem();
		//if (!item) {
		//	return;
		//}

		//item->GetGameObject()->SetActive(false);
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void HidePlace::SetHideItem(const std::shared_ptr<HideItem>& item) {
		m_hideItem = item;

		//auto animator = GetGameObject()->GetComponent<BoxAnimator>(false);
		//if (item && animator && animator->IsCurrentAnimator(BoxAnimationState::State::Close)) {
		//	item->GetGameObject()->SetActive(false);
		//}
	}

	std::shared_ptr<HideItem> HidePlace::GetHideItem() const {
		return m_hideItem.lock();
	}

	std::shared_ptr<HidePlace> HidePlace::GetStageHidePlace(int objectId)
	{
		for (auto& gameObject : App::GetApp()->GetScene<Scene>()->GetActiveStage()->GetGameObjectVec())
		{
			auto hidePlace = gameObject->GetComponent<HidePlace>(false);

			if (!hidePlace || hidePlace->GetObjectId() != objectId)
			{
				continue;
			}

			return hidePlace;
		}

		return nullptr;
	}
}