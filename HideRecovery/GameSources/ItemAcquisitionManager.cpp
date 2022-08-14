
/*!
@file ItemAcquisitionManager.cpp
@brief ItemAcquisitionManagerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ItemAcquisitionManager.h"

#include "CollisionAction.h"
#include "Itabashi/Item.h"

#include "MaruUtility.h"
#include "MaruAction.h"

#include "PlayerInputer.h"
#include "ItemBag.h"

#include "PlayerAnimationCtrl.h"
#include "RotationController.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// パラメータ
	//--------------------------------------------------------------------------------------

	ItemAcquisitionManager_Parametor::ItemAcquisitionManager_Parametor() :
		ItemAcquisitionManager_Parametor(2.0f)
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

	ItemAcquisitionManager::ItemAcquisitionManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& param) :
		Component(objPtr), m_param(param)
	{}

	void ItemAcquisitionManager::OnLateStart() {
		//アイテムを取得する。
		m_allFieldItems = maru::Utility::ConvertArraySharedToWeak(maru::Utility::FindComponents<Item>());
	}

	void ItemAcquisitionManager::OnUpdate() {

		m_acquisitionItems.clear();

		//アイテムが範囲内にいるか索敵
		for (auto& itemWeak : m_allFieldItems) {

			auto item = itemWeak.lock();

			if (!item)
			{
				continue;
			}

			//アクティブ状態でないなら飛ばす。
			if (!item->GetGameObject()->IsActive()) {
				continue;
			}

			bool isAcquisitionRange = IsAcquisitionRange(item);

			if (IsAcquisitionRange(item))
			{
				m_acquisitionItems.push_back(item);
			}
		}
	}

	void ItemAcquisitionManager::Input_ItemAcquisition()
	{
		std::shared_ptr<Item> acquisitionedItem;
		if (IsAcquisition(acquisitionedItem))
		{
			ItemAcquisition(acquisitionedItem);
		}
	}

	void ItemAcquisitionManager::ItemAcquisition(const std::shared_ptr<Item>& item) {

		if (!item)
		{
			return;
		}

		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		auto animator = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false);
		if (!bag || !animator) {
			return;
		}

		//アイテムを入れる。
		bag->AddItem(item);

		//アイテムを獲得された状態にする。
		item->GetGameObject()->SetActive(false);

		//アニメーションを再生
		auto itemPosition = item->GetGameObject()->GetComponent<Transform>()->GetPosition();
		if (itemPosition.y < 0.0f) {
			//アイテムが床にあるなら
			animator->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_Floor);
		}
		else {
			//アイテムが床にないなら
			animator->ChangeAnimation(PlayerAnimationCtrl::State::PutItem_HideObject);
		}

		//向きたい方法を設定
		if (auto rotationController = GetGameObject()->GetComponent<RotationController>(false)) {
			auto direct = itemPosition - transform->GetPosition();
			rotationController->SetDirect(direct);
		}
	}

	bool ItemAcquisitionManager::IsAcquisitionRange(const std::shared_ptr<Item>& item) {
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

	bool ItemAcquisitionManager::IsAcquisition(std::shared_ptr<Item>& acquisitionedItem)
	{
		//バッグを所持していなかったら処理を飛ばす
		auto bag = GetGameObject()->GetComponent<ItemBag>(false);

		if (!bag)
		{
			return false;
		}

		auto animator = GetGameObject()->GetComponent<PlayerAnimationCtrl>(false);

		if (!animator)
		{
			return false;
		}

		//アニメーションが置く状態ならできない
		auto currentState = animator->GetCurrentAnimaiton();

		if (currentState == PlayerAnimationCtrl::State::PutItem_Floor || currentState == PlayerAnimationCtrl::State::PutItem_HideObject)
		{
			return false;
		}

		for (auto& itemWeak : m_acquisitionItems)
		{
			auto item = itemWeak.lock();

			//バッグに入れることが可能なら
			if (bag->IsAcquisition(item))
			{
				acquisitionedItem = item;
				return true;
			}
		}

		return false;
	}

	std::vector<std::shared_ptr<Item>> ItemAcquisitionManager::GetFieldAllItem() const
	{
		std::vector<std::shared_ptr<Item>> items;

		for (auto& itemWeak : m_allFieldItems)
		{
			auto item = itemWeak.lock();

			if (item)
			{
				items.push_back(item);
			}
		}

		return items;
	}

	std::vector<std::shared_ptr<Item>> ItemAcquisitionManager::GetCanAcquisitionItems() const
	{
		std::vector<std::shared_ptr<Item>> items;

		for (auto& itemWeak : m_acquisitionItems)
		{
			auto item = itemWeak.lock();

			if (item)
			{
				items.push_back(item);
			}
		}

		return items;
	}

}