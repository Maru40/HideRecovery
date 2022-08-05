
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

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	OwnHideItemManager_Parametor::OwnHideItemManager_Parametor():
		OwnHideItemManager_Parametor(2.0f)
	{}

	OwnHideItemManager_Parametor::OwnHideItemManager_Parametor(const float searchHidePlaceRange):
		searchHidePlaceRange(searchHidePlaceRange)
	{}

	//--------------------------------------------------------------------------------------
	/// 持っている隠すアイテムの管理本体
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

		//置く場所の取得
		auto position = CalculateHidePosition();

		auto hideItemTrans = hideItem->GetGameObject()->GetComponent<Transform>();
		hideItemTrans->SetPosition(position);
		hideItem->GetGameObject()->SetUpdateActive(true);
		hideItem->GetGameObject()->SetDrawActive(true);

		bag->RemoveItem(hideItem);
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