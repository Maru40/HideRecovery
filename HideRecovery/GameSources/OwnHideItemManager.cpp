
/*!
@file ItemAcquisitionManager.cpp
@brief ItemAcquisitionManagerクラス実体
担当：丸山裕喜
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
	/// パラメータ
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
	/// 持っている隠すアイテムの管理本体
	//--------------------------------------------------------------------------------------

	OwnHideItemManager::OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr):
		OwnHideItemManager(objPtr, Parametor())
	{}

	OwnHideItemManager::OwnHideItemManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		Component(objPtr), m_param(parametor), m_timer(new GameTimer(0)), m_isFleePut(true)
	{}

	void OwnHideItemManager::OnUpdate() {
		//隠すボタンを押したら
		if (PlayerInputer::IsPutHideItem()) {
			PutHideItem();
		}

		//ブラフアイテムボタン
		if (PlayerInputer::IsBluffPutItem()) {
			BluffPutHideItem();
		}

		//タイムベント
		if (!m_timer->IsTimeUp()) {
			m_timer->UpdateTimer();
		}

		//デバッグコマンド
		if (PlayerInputer::GetInstance()->IsDownDown()) {
			m_isFleePut = !m_isFleePut;
		}
	}

	void OwnHideItemManager::PutHideItem() {
		if (!IsPut()) { //置ける状態でないなら
			return;
		}

		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		auto hideItem = bag->GetHideItem();

		//置く場所の取得
		auto hidePosition = CalculateHidePosition();	//隠す場所
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

		//アニメーションが置く状態ならできない
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
		auto hidePosition = CalculateHidePosition();	//隠す場所

		//向きたい方法を設定
		if (auto rotationController = GetGameObject()->GetComponent<RotationController>(false)) {
			auto direct = hidePosition - transform->GetPosition();
			rotationController->SetDirect(direct);
		}
	}

	bool OwnHideItemManager::IsPut() const {
		//必要コンポーネント確認
		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		auto animator = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false);
		if (!bag || !animator) { return false; }	//必要コンポーネントがあるかどうか

		auto hideItem = bag->GetHideItem();	
		if (!hideItem) { return false; }	//hideItemがあるかどうか

		if (m_isFleePut) { //フリープット状態なら
			return true;
		}

		//そうでないなら近くのオブジェクトを探す
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