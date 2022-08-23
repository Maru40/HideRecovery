
/*!
@file ChargeGun.cpp
@brief ChargeGunクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeGun.h"

#include "ChargeBulletObject.h"
#include "ChargeBullet.h"

#include "PlayerInputer.h"

#include "Itabashi/ObjectMover.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "SoundManager.h"

#include "Maruyama/Player/Component/UseWepon.h"

namespace basecross {

	ChargeGun::ChargeGun(const std::shared_ptr<GameObject>& objPtr) :
		WeponBase(objPtr)
	{}

	void ChargeGun::OnCreate() {
		SetBulletInstanceOffset(Vec3(0.0f, 0.5f, 0.0f));
	}

	void ChargeGun::OnLateStart() {
		m_useWepon = GetGameObject()->GetComponent<UseWepon>(false);
	}

	void ChargeGun::OnUpdate() {
		UpdateAnimation();
	}

	void ChargeGun::UpdateAnimation() {
		//エイム中なら処理をしない
		auto useWepon = m_useWepon.lock();
		if (useWepon && useWepon->IsAim()) {
			return;
		}

		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		if (!animator->IsCurretAnimationState(PlayerAnimationState::State::Shot)) {	//Shot中でないなら処理をしない
			return;
		}

		if (animator->IsTargetAnimationEnd()) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::GunEnd2);
		}
	}

	std::shared_ptr<BulletObjectBase> ChargeGun::Shot(const Vec3& direct) {
		//音の再生
		if (auto soundManager = SoundManager::GetInstance()) {
			constexpr float Volume = 0.1f;
			soundManager->PlayOneShot(SoundManager::ID::ShotSE, Volume);
		}

		//弾の生成
		Vec3 instancePosition = transform->GetPosition() + GetBulletInstanceOffset();
		auto bulletObject = InstantiateBullet<ChargeBulletObject>(instancePosition, transform->GetQuaternion());
		if (auto bullet = bulletObject->GetComponent<ChargeBullet>(false)) {
			bullet->Shot(GetGameObject(), direct);
		}

		//アニメーションの再生
		PlayAnimation();

		return bulletObject;
	}

	void ChargeGun::PlayAnimation() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		//animator->ChangePlayerAnimation(PlayerAnimationState::State::Shot);
	}

	void ChargeGun::RevisionShotDirection() {

	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	bool ChargeGun::IsShot() const {
		if (auto useWepon = GetGameObject()->GetComponent<UseWepon>(false)) {
			return useWepon->IsAim();
		}

		return false;
	}
}