
/*!
@file PlayerDeader.cpp
@brief PlayerDeaderクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PlayerDeader.h"

#include "Maruyama/Player/Component/Respawner.h"

#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Component/PlayerStatus.h"

#include "VelocityManager.h"
#include "UseWepon.h"
#include "ItemBag.h"
#include "HideItem.h"
#include "Itabashi/OnlineTransformSynchronization.h"

namespace basecross {

	PlayerDeader::PlayerDeader(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_updateFunction(nullptr)
	{}

	void PlayerDeader::OnLateStart() {
		m_animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		m_onlineTransformSynchonization = GetGameObject()->GetComponent<Online::OnlineTransformSynchronization>();

		auto animator = m_animator.lock();
		if (animator) {
			PlayerAnimationState::State states[] = {
				PlayerAnimationState::State::Dead,
				PlayerAnimationState::State::GSDead,
			};

			auto update = [&, animator]() {
				if (m_updateFunction) {
					m_updateFunction();
				}
				return false;
			};

			for (auto& state : states) {
				animator->AddAnimationEvent(state, nullptr, update, nullptr);
			}
		}
	}

	void PlayerDeader::OnUpdate() {

	}

	void PlayerDeader::ObserveAnimation() {
		auto animator = m_animator.lock();
		if (!animator) {
			return;
		}

		//アニメーションが死亡状態でないなら監視しない
		if (!animator->IsCurretAnimationState(PlayerAnimationState::State::Dead) && 
			!animator->IsCurretAnimationState(PlayerAnimationState::State::GSDead))
		{
			return;
		}

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false))
		{
			velocityManager->ResetAll();
		}

		if (animator->IsTargetAnimationEnd()) {	//アニメーションが終了したら
			//リスポーン処理がアタッチされているなら
			if (auto respawner = GetGameObject()->GetComponent<Respawner>(false)) {
				respawner->StartRespawn();
			}

			m_updateFunction = nullptr;	//更新をやめる。
		}
	}

	void PlayerDeader::StartDead() {
		auto useWeapon = GetGameObject()->GetComponent<UseWepon>(false);
		auto animator = m_animator.lock();

		auto onlineTransform = m_onlineTransformSynchonization.lock();
		onlineTransform->SetUpdateActive(false);

		//アニメーション遷移
		if (animator && useWeapon) {
			//IsAimなら
			if (useWeapon->IsAim()) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::GSDead);
			}
			else {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::Dead);
			}

			useWeapon->SetIsAim(false);
		}

		//速度リセット
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			velocityManager->ResetAll();	
		}

		//アイテムを落とす。
		if (auto itemBag = GetGameObject()->GetComponent<ItemBag>(false)) 
		{
			auto hideItem = itemBag->GetHideItem();
			if (hideItem)
			{
				auto item = hideItem->GetItem();
				itemBag->RemoveItem(item);
				hideItem->GetGameObject()->GetComponent<Transform>()->SetPosition(transform->GetPosition());
				hideItem->GetGameObject()->SetActive(true);
			}
		}

		//重力を消す
		if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {
			gravity->SetUpdateActive(false);
		}

		//当たり判定を消す
		if (auto collision = GetGameObject()->GetComponent<CollisionObb>(false)) {
			collision-> SetUpdateActive(false);
		}

		m_updateFunction = [&]() { ObserveAnimation(); };	//更新処理設定
	}

	bool PlayerDeader::IsDead() {
		auto status = GetGameObject()->GetComponent<PlayerStatus>(false);
		if (status) {
			return status->IsDead();
		}

		return false;
	}
}