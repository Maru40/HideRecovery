
/*!
@file EnemyStatusBase.cpp
@brief EnemyStatusBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyStatusBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		///	EnemyStatus
		//--------------------------------------------------------------------------------------

		EnemyStatus::EnemyStatus()
			:EnemyStatus(10.0f, 0.0f)
		{}

		EnemyStatus::EnemyStatus(const float& maxHp, const float& toughness)
			:hp(maxHp), maxHp(maxHp), toughness(toughness), baseToughness(toughness)
		{}

		//--------------------------------------------------------------------------------------
		///	EnemyStatusManagerBase
		//--------------------------------------------------------------------------------------

		EnemyStatusManagerBase::EnemyStatusManagerBase(const std::shared_ptr<GameObject>& objPtr)
			:EnemyStatusManagerBase(objPtr, EnemyStatus())
		{}

		EnemyStatusManagerBase::EnemyStatusManagerBase(const std::shared_ptr<GameObject>& objPtr, const EnemyStatus& status)
			: Component(objPtr), m_status(status)
		{}

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void EnemyStatusManagerBase::SetStatus(const EnemyStatus& status) noexcept {
			m_status = status;
		}

		EnemyStatus EnemyStatusManagerBase::GetStatus() const noexcept {
			return m_status;
		}

		EnemyStatus& EnemyStatusManagerBase::GetRefStatus() {
			return m_status;
		}

		bool EnemyStatusManagerBase::IsToughnessBreak(const float& power) {
			return m_status.toughness < power;
		}

	}
}