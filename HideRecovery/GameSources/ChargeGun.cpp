
/*!
@file ChargeGun.cpp
@brief ChargeGun�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ChargeGun.h"

#include "ChargeBulletObject.h"
#include "ChargeBullet.h"

#include "PlayerInputer.h"

#include "Watanabe/Component/PlayerAnimator.h"

namespace basecross {

	ChargeGun::ChargeGun(const std::shared_ptr<GameObject>& objPtr) :
		WeponBase(objPtr)
	{}

	void ChargeGun::OnCreate() {
		SetBulletInstanceOffset(Vec3(0.0f, 0.9f, 0.0f));
	}

	void ChargeGun::OnUpdate() {
		UpdateAnimation();
	}

	void ChargeGun::UpdateAnimation() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		//�V���b�g�X�e�[�g�łȂ��Ȃ�
		if (!animator->IsCurretAnimationState(PlayerAnimationState::State::Shot)) {
			return;
		}

		if (animator->IsTargetAnimationEnd()) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::GunEnd2);
		}
	}

	void ChargeGun::Shot(const Vec3& direct) {
		Vec3 instancePosition = transform->GetPosition() + GetBulletInstanceOffset();
		auto bulletObject = InstantiateBullet(instancePosition, transform->GetQuaternion());
		if (auto bullet = bulletObject->GetComponent<ChargeBullet>(false)) {
			bullet->Shot(GetGameObject(), direct);
		}

		//�A�j���[�V�����̍Đ�
		PlayAnimation();
	}

	void ChargeGun::PlayAnimation() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {
			return;
		}

		animator->ChangePlayerAnimation(PlayerAnimationState::State::Shot);
	}

}