
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

namespace basecross {

	ChargeGun::ChargeGun(const std::shared_ptr<GameObject>& objPtr) :
		WeponBase(objPtr)
	{}

	void ChargeGun::OnUpdate() {
		//�f�o�b�O
		if (PlayerInputer::GetInstance()->IsYDown()) {
			Shot(transform->GetForward());
		}
	}

	void ChargeGun::Shot(const Vec3& direct) {
		Vec3 instancePosition = transform->GetPosition() + GetBulletInstanceOffset();
		auto bulletObject = InstantiateBullet(instancePosition, transform->GetQuaternion());
		if (auto bullet = bulletObject->GetComponent<ChargeBullet>(false)) {
			bullet->Shot(direct);
		}
	}

}