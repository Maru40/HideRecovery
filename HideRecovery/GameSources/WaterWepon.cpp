/*!
@file WaterWepon.cpp
@brief WaterWeponのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "WeponBase.h"
#include "WaterWepon.h"

#include "BulletBase.h"

#include "LaserBase.h"

#include "PlayerInputer.h"
#include "WaterPressure.h"

#include "BulletBase.h"

#include "MaruUtility.h"

#include "LookTargetManager.h"

#include "GameTimer.h"
#include "TimeHelper.h"

namespace basecross {

	WaterWepon::WaterWepon(const std::shared_ptr<GameObject>& objPtr)
		:WeponBase(objPtr, Parametor(ShotType::Hold, Vec3(-1.25f, 0.8f, 1.75f)))
	{}

	void WaterWepon::HoldShotUpdate() {
		if (!m_holdBullet) {
			m_holdBullet = GetStage()->Instantiate<WaterPressure>(CalculateCreateBulletPosition(), transform->GetQuaternion());
		}

		m_holdBullet->GetComponent<Transform>()->SetPosition(CalculateCreateBulletPosition());
		auto bullet = m_holdBullet->GetComponent<BulletBase>(false);

		if (bullet) {
			bullet->Shot(GetDirect());
		}
	}

	void WaterWepon::HoldShotEnd() {
		if (!m_holdBullet) {
			return;
		}

		auto bullet = m_holdBullet->GetComponent<BulletBase>(false);
		if (bullet) {
			bullet->ShotEnd();
		}

		m_holdBullet = nullptr;
	}

	Vec3 WaterWepon::GetDirect() const {
		auto lookTarget = GetGameObject()->GetComponent<LookTargetManager>(false);
		if (lookTarget && lookTarget->HasTarget()) {
			return maru::Utility::CalcuToTargetVec(GetGameObject(), lookTarget->GetTarget()).GetNormalized();
		}

		return transform->GetForward();
	}

}