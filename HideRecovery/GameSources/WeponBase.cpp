/*!
@file WeponBase.cpp
@brief WeponBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "WeponBase.h"

#include "BulletBase.h"

#include "LaserBase.h"

#include "PlayerInputer.h"
#include "WaterPressure.h"

#include "BulletBase.h"

#include "MaruUtility.h"

#include "I_Damaged.h"
#include "I_Pusher.h"
#include "PlayerStatusManager.h"

#include "GameTimer.h"
#include "TimeHelper.h"

#include "DebugObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	WeponBase_Parametor::WeponBase_Parametor() 
		: WeponBase_Parametor(ShotType::Hold, Vec3(0.0f))
	{}

	WeponBase_Parametor::WeponBase_Parametor(const ShotType& type, const Vec3& positionOffset) 
		: shotType(type), positionOffset(positionOffset), energyConsumed(0.5f), overheatCoolTime(1.5f), isOverheat(false)
	{}

	//--------------------------------------------------------------------------------------
	/// �������N���X
	//--------------------------------------------------------------------------------------

	WeponBase::WeponBase(const std::shared_ptr<GameObject>& objPtr)
		:WeponBase(objPtr, Parametor())
	{}

	WeponBase::WeponBase(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor)
		: Component(objPtr), m_timer(new GameTimer(0)), m_baseParam(parametor), m_holdBullet(nullptr)
	{}

	void WeponBase::OnUpdate() {
		if (m_timer->IsTimeUp()) {
			return;
		}

		m_timer->UpdateTimer();
		if (m_timer->IsTimeUp()) {
			m_baseParam.isOverheat = false;
		}
	}

	void WeponBase::Shot() {
		if (!IsShot()) {
			DebugObject::AddString(L"");
			return;
		}

		switch (GetShotType()) {
		case ShotType::Hold:
			HoldShotUpdate();
			break;
		}

		//�G�l���M�[����
		ReduceEnergy();
	}

	void WeponBase::ShotEnd() {
		switch (GetShotType()) {
		case ShotType::Hold:
			HoldShotEnd();
			break;
		}
	}

	void WeponBase::ReduceEnergy() {
		if (auto playerStatusManager = GetGameObject()->GetComponent<PlayerStatusManager>(false)) {
			playerStatusManager->ReduceEnergy(m_baseParam.energyConsumed);

			//�G�l���M�[���g�p�s�Ȃ�I�[�o�[�q�[�g
			if (!IsUseEnergy()) { 
				Overhear();
			}
		}
	}

	Vec3 WeponBase::CalculateCreateBulletPosition() {
		auto myPos = transform->GetPosition();
		auto forward = transform->GetForward();
		auto halfScale = transform->GetScale() * 0.5f;
		auto position = Vec3(myPos + forward * halfScale);
		auto offset = maru::Utility::ConvertForwardOffset(transform->GetForward(), m_baseParam.positionOffset);

		return position + offset;
	}

	void WeponBase::Overhear() {
		SetIsOverheat(true);

		m_timer->ResetTimer(m_baseParam.overheatCoolTime);
		ShotEnd();
	}

	bool WeponBase::IsUseEnergy() const {
		//����G�l���M�[��0�Ȃ�łĂȂ�
		if (auto playerStatusManager = GetGameObject()->GetComponent<PlayerStatusManager>(false)) {
			return playerStatusManager->IsUseEnergy();
		}

		return true;
	}

	bool WeponBase::IsShot() const {
		if (IsOverheat()) {
			return false;
		}

		//����G�l���M�[��0�Ȃ�łĂȂ�
		return IsUseEnergy();
	}
}