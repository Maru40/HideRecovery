
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
		if (PlayerInputer::IsPutHideItem()) {
			PutHideItem();
		}

		//タイムベント
		if (!m_timer->IsTimeUp()) {
			m_timer->UpdateTimer();

			if (m_timer->IsTimeUp()) {
				
			}
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
		if (!bag) {
			return;
		}

		auto hideItem = bag->GetHideItem();
		if (!hideItem) {
			return;
		}

		auto animator = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false);
		if (!animator) {
			return;
		}

		//置く場所の取得
		auto putEvent = [&, bag, hideItem]() {
			auto position = CalculateHidePosition();

			auto hideItemTrans = hideItem->GetGameObject()->GetComponent<Transform>();
			hideItemTrans->SetPosition(position);
			hideItem->GetGameObject()->SetActive(true);

			bag->RemoveItem(hideItem);
		};

		if (m_isFleePut) {
			animator->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_Floor);
			m_timer->ResetTimer(m_param.putFloorAnimationTime, putEvent);
		}
		else {
			animator->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_HideObject);
			m_timer->ResetTimer(m_param.putHideObjectAnimationTime, putEvent);
		}
	}

	bool OwnHideItemManager::IsPut() const {
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