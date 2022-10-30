
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
#include "Itabashi/ObjectHider.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

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

		if (auto shareManager = ShareClassesManager::GetInstance(GetStage())) {
			shareManager->AddShareClass(GetThis<HidePlace>());
		}
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
		if (IsOpen()) {
			return;
		}

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

	bool HidePlace::PutHideItem(const std::shared_ptr<HideItem>& hideItem)
	{
		auto oldHideItem = m_hideItem.lock();

		if (oldHideItem)
		{
			return false;
		}

		m_hideItem = hideItem;
		hideItem->GetItem()->SetItemOwner(GetGameObject(), false);
		hideItem->GetObjectHider()->Hide(GetHidePosition());

		return true;
	}

	std::shared_ptr<HideItem> HidePlace::TakeOutHideItem()
	{
		auto hideItem = m_hideItem.lock();

		if (!hideItem)
		{
			return nullptr;
		}

		m_hideItem.reset();

		hideItem->GetItem()->ReleaseItemOwner(false);
		hideItem->GetObjectHider()->Appear();

		return hideItem;
	}

	std::shared_ptr<HideItem> HidePlace::GetHideItem() const {
		return m_hideItem.lock();
	}

	bool HidePlace::CanPutable() const
	{
		auto hideItem = m_hideItem.lock();
		return hideItem == nullptr;
	}

	void HidePlace::SetDrawUI(const bool isActive) {
		m_boardUIObject.lock()->SetUpdateActive(isActive);
	}

	bool HidePlace::IsDrawUI() const {
		return m_boardUIObject.lock()->GetUpdateActive();
	}
}