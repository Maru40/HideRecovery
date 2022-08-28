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
#include "Watanabe/Effekseer/EfkEffect.h"
#include "SoundManager.h"

#include "Maruyama/Player/Component/UseWepon.h"

#include "TimeHelper.h"
#include "GameTimer.h"

namespace basecross {
	ChargeGun::ChargeGun(const std::shared_ptr<GameObject>& objPtr) :
		WeponBase(objPtr),
		m_timer(new GameTimer(0.0f)),
		m_shotSoundClip(L"Shot_SE", false, 0.5f)
	{}

	void ChargeGun::OnCreate() {
		SetBulletInstanceOffset(Vec3(0.0f, 0.5f, 0.0f));
	}

	void ChargeGun::OnLateStart() {
		m_useWepon = GetGameObject()->GetComponent<UseWepon>(false);
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>(false);
	}

	void ChargeGun::OnUpdate() {
		//UpdateAnimation();
		m_timer->UpdateTimer();
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
		if (!m_timer->IsTimeUp()) {
			return nullptr;
		}

		auto useWeapon = m_useWepon.lock();
		if (useWeapon && !useWeapon->IsAim()) {
			return nullptr;
		}

		m_timer->ResetTimer(GetShotIntervalTime());

		if (auto soundEmitter = m_soundEmitter.lock())
		{
			soundEmitter->PlaySoundClip(m_shotSoundClip);
		}

		//弾の生成
		Vec3 instancePosition = transform->GetPosition() + GetBulletInstanceOffset();
		auto bulletObject = InstantiateBullet<ChargeBulletObject>(instancePosition, transform->GetQuaternion());
		if (auto bullet = bulletObject->GetComponent<ChargeBullet>(false)) {
			bullet->Shot(GetGameObject(), direct);
		}

		//アニメーションの再生
		PlayAnimation();

		// マズルフラッシュエフェクトの再生
		if (auto efkComp = GetGameObject()->GetComponent<EfkComponent>(false)) {
			efkComp->Play(L"Hit", true);
			// 再生先が先じゃないと反映されない
			//efkComp->SetPosition(L"Hit", instancePosition);
		}

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