/*!
@file TargetManager.cpp
@brief TargetManager実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	見失ったターゲットデータ
	//--------------------------------------------------------------------------------------

	struct TargetManager::LostData
	{
		bool isLost = false;
		Vec3 position;
	};

	//--------------------------------------------------------------------------------------
	///	パラメータ
	//--------------------------------------------------------------------------------------

	struct TargetManager::Parametor
	{
		ex_weak_ptr<GameObject> target = nullptr;
		LostData lostData;

		//ターゲットをロストしているかどうか
		bool IsLost() const { return lostData.isLost; }
		bool SetIsLost(const bool isLost) {
			lostData.isLost = isLost;
			if (isLost && target) { //ターゲットが存在しつつ、Lostしたなら
				//ロストした場所をセット。
				lostData.position = target->GetComponent<Transform>()->GetPosition();
			}
		}
	};

	//--------------------------------------------------------------------------------------
	///	ターゲット管理本体
	//--------------------------------------------------------------------------------------

	TargetManager::TargetManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_paramPtr(new Parametor())
	{}

	void TargetManager::OnCreate() {

	}

	//--------------------------------------------------------------------------------------
	///	アクセッサ
	//--------------------------------------------------------------------------------------

	bool TargetManager::HasTarget() const {
		if (!m_paramPtr->target) {
			return false;
		}

		//return m_paramPtr->target->IsActive();

		return m_paramPtr->target ? true : false;
	}

	Vec3 TargetManager::GetTargetPosition() const {
		return GetTarget()->GetComponent<Transform>()->GetPosition();
	}

	Vec3 TargetManager::GetLostPosition() const {
		return m_paramPtr->lostData.position;
	}

	void TargetManager::SetTarget(const std::shared_ptr<GameObject>& target) {
		if (target == nullptr && GetTarget()) {
			m_paramPtr->lostData.position = GetTargetPosition();
		}

		m_paramPtr->target = target;
	}

	std::shared_ptr<GameObject> TargetManager::GetTarget() const {
		return m_paramPtr->target.GetShard();
	}

	Vec3 TargetManager::CalcuToTargetVec() {
		return GetTarget()->GetComponent<Transform>()->GetPosition() - transform->GetPosition();
	}
}

//endbasecross