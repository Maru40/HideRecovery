
/*!
@file HidePlace.h
@brief HidePlaceなど
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlace.h"

#include "Watanabe/Component/BoxAnimator.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Itabashi/Item.h"
#include "Maruyama/Item/HideItem.h"

#include "Maruyama/UI/3D/Component/OpenBoard.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	HidePlace_Parametor::HidePlace_Parametor():
		HidePlace_Parametor(Vec3(0.0f))
	{}

	HidePlace_Parametor::HidePlace_Parametor(const Vec3& hidePositionOffset):
		hidePositionOffset(hidePositionOffset),
		boardUIPositionOffset(Vec3(0.0f, 1.5f, 0.0f)),
		isOpen(false)
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

		CreateBoard();
	}

	void HidePlace::OnLateStart() {
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>();
	}

	void HidePlace::CreateBoard() {
		auto board = GetStage()->AddGameObject<GameObject>()->AddComponent<OpenBoard>();
		auto boardTrans = board->GetGameObject()->GetComponent<Transform>();
		boardTrans->SetPosition(transform->GetPosition() + m_param.boardUIPositionOffset);

		board->SetUpdateActive(false);
		m_boardUIObject = board;
	}

	void HidePlace::Open() {
		auto animator = GetGameObject()->GetComponent<BoxAnimator>(false);
		if (animator && animator->IsCurrentAnimator(BoxAnimationState::State::Close)) {
			animator->ChangeBoxAnimation(BoxAnimationState::State::Open);
		}

		if (auto soundEmitter = m_soundEmitter.lock()) {
			soundEmitter->PlaySoundClip(m_openSoundClip);
		}

		m_param.isOpen = true;
	}

	void HidePlace::Close() {
		if (auto animator = GetGameObject()->GetComponent<BoxAnimator>(false)) {
			animator->ChangeBoxAnimation(BoxAnimationState::State::Close);
		}

		m_param.isOpen = false;
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	void HidePlace::SetHideItem(const std::shared_ptr<HideItem>& item) {
		m_hideItem = item;
	}

	std::shared_ptr<HideItem> HidePlace::GetHideItem() const {
		return m_hideItem.lock();
	}

	void HidePlace::SetDrawUI(const bool isActive) {
		m_boardUIObject.lock()->SetUpdateActive(isActive);
	}

	bool HidePlace::IsDrawUI() const {
		return m_boardUIObject.lock()->GetUpdateActive();
	}
}