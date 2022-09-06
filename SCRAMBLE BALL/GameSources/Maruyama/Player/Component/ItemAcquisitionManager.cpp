
/*!
@file ItemAcquisitionManager.cpp
@brief ItemAcquisitionManagerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ItemAcquisitionManager.h"

#include "Maruyama/Utility/Component/CollisionAction.h"
#include "Itabashi/Item.h"

#include "Maruyama/Utility/Utility.h"
#include "Maruyama/Utility/MaruAction.h"

#include "PlayerInputer.h"
#include "ItemBag.h"

#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Item/HideItem.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/UI/SplashMessageUI.h"
#include "Itabashi/PlayerOnlineController.h"

#include "VelocityManager.h"

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
		Component(objPtr), 
		m_param(param),
		m_getBallSoundClip(L"GetBallSE", false, 0.5f),
		m_stolenBallSoundClip(L"StolenBallSE", false, 0.5f)
	{}

	void ItemAcquisitionManager::OnLateStart() {
		//アイテムを取得する。
		m_allFieldItems = maru::Utility::ConvertArraySharedToWeak(maru::Utility::FindComponents<Item>(GetStage()));

		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>(false);
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

	void ItemAcquisitionManager::HideItemAcquisitionEvent(const std::shared_ptr<GameObject>& other) {
		auto selfTeamMember = Online::PlayerOnlineController::GetLocalOnlineController()->GetGameObject()->GetComponent<I_TeamMember>(false);
		auto otherTeamMember = other->GetComponent<I_TeamMember>(false);
		if (!selfTeamMember && !otherTeamMember) {
			return;
		}

		//スプラッシュメッセージがあるかどうか
		auto splashMessageUI = m_splashMessageUI.lock();
		if (!splashMessageUI) {
			splashMessageUI = maru::Utility::FindGameObject<SplashMessageUI>(GetStage());
			m_splashMessageUI = splashMessageUI;
		}

		if (selfTeamMember->GetTeam() == otherTeamMember->GetTeam()) {
			splashMessageUI->SetMessage(SplashMessageUI::MessageType::GetBall);
			m_soundEmitter.lock()->PlaySoundClip(m_getBallSoundClip);

		}
		else {
			splashMessageUI->SetMessage(SplashMessageUI::MessageType::StolenBall);
			m_soundEmitter.lock()->PlaySoundClip(m_stolenBallSoundClip);
		}

		splashMessageUI->SetColor(team::GetTeamColor(otherTeamMember->GetTeam()));
	}

	void ItemAcquisitionManager::ItemAcquisition(const std::shared_ptr<Item>& item) {

		if (!item) {
			return;
		}

		auto bag = GetGameObject()->GetComponent<ItemBag>(false);
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!bag || !animator) {
			return;
		}

		//アイテムを入れる。
		bag->AddItem(item);
		item->SetItemOwner(GetGameObject(), false);

		//アイテムを獲得された状態にする。
		item->GetGameObject()->SetActive(false);
		//HideItemAcquisitionEvent(GetGameObject());	//隠すアイテムの時に呼び出すイベント

		//アニメーションを再生
		auto itemPosition = item->GetGameObject()->GetComponent<Transform>()->GetPosition();
		if (itemPosition.y < 0.0f) {
			//アイテムが床にあるなら
			animator->ChangePlayerAnimation(PlayerAnimationState::State::PutItem_Floor);
		}
		else {
			//アイテムが床にないなら
			animator->ChangePlayerAnimation(PlayerAnimationState::State::PutItem_HideObject);
		}

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			velocityManager->ResetAll();
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

		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);

		if (!animator)
		{
			return false;
		}

		//アニメーションが置く状態ならできない
		if (animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_Floor) || animator->IsCurretAnimationState(PlayerAnimationState::State::PutItem_HideObject))
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