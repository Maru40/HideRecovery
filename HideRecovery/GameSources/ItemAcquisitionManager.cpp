
/*!
@file ItemAcquisitionManager.cpp
@brief ItemAcquisitionManagerクラス実体
担当：丸山裕喜
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

#include "PlayerAnimationCtrl.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	ItemAcquisitionManager_Parametor::ItemAcquisitionManager_Parametor() :
		ItemAcquisitionManager_Parametor(10.0f)
	{}

	ItemAcquisitionManager_Parametor::ItemAcquisitionManager_Parametor(const float searchRange) :
		searchRange(searchRange)
	{}

	//--------------------------------------------------------------------------------------
	/// アイテム獲得管理本体
	//--------------------------------------------------------------------------------------

	ItemAcquisitionManager::ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr) :
		ItemAcquisitionManager(objPtr, Parametor())
	{}

	ItemAcquisitionManager::ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& param):
		Component(objPtr), m_param(param)
	{}

	void ItemAcquisitionManager::OnLateStart() {
		//アイテムを取得する。
		m_allFieldItems = maru::Utility::ConvertArraySharedToExweak(maru::Utility::FindComponents<ItemBase>());
	}

	void ItemAcquisitionManager::OnUpdate() {
		//アイテムが範囲内にいるか索敵
		for (auto& item : m_allFieldItems) {
			//アクティブ状態でないなら飛ばす。
			if (!item->GetGameObject()->IsActive()) {
				continue;
			}

			//索敵範囲内ならtrue
			item->SetIsAcquisition(IsAcquisitionRange(item.GetShard()));
		}

		//アイテム獲得ボタンを押したら
		if (PlayerInputer::IsItemAcquisition()) {
			Input_ItemAcquisition();
		}
	}

	void ItemAcquisitionManager::Input_ItemAcquisition() {
		//バッグを所持していなかったら処理を飛ばす
		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		if (!bag) {
			return;
		}

		for (auto& item : m_allFieldItems) {
			//獲得できるアイテム、かつ、バッグに入れることが可能なら
			if (item->IsAcquisition() && bag->IsAcquisition(item.GetShard())) {
				//アイテムを入れる。
				bag->AddItem(item.GetShard());

				//アイテムを獲得された状態にする。
				item->GetGameObject()->SetActive(false);

				//アニメーションを再生
				if (auto animation = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false)) {
					//アイテムが床にあるなら
					auto itemPosition = item->GetGameObject()->GetComponent<Transform>()->GetPosition();
					if (itemPosition.y < 0.0f) {
						animation->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_Floor);
					}
					else {
						animation->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_HideObject);
					}
				}
			}
		}
	}


	bool ItemAcquisitionManager::IsAcquisitionRange(const std::shared_ptr<ItemBase>& item) {
		auto toItemVec = maru::Utility::CalcuToTargetVec(GetGameObject(), item->GetGameObject());

		return toItemVec.length() < m_param.searchRange; //索敵範囲内ならtrue
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------



	//多分ボツ関数----------------------------------------------------

	void ItemAcquisitionManager::CreateSerchTriggerObject() {
		auto object = GetStage()->Instantiate<GameObject>(Vec3(0.0f), transform->GetQuaternion(), GetGameObject());
		auto objectTrans = object->GetComponent<Transform>();
		objectTrans->SetScale(Vec3(5.0f, 2.0f, 5.0f));

		//当たり判定設定
		auto collision = object->AddComponent<CollisionObb>();
		collision->SetAfterCollision(AfterCollision::None);
		collision->SetDrawActive(true);

		//衝突時イベント設定
		auto collisionAction = object->AddComponent<maru::CollisionAction>();
	}

}