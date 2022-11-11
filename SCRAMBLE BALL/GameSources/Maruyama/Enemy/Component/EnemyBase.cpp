/*!
@file AIPlayerBehaviorTree.cpp
@brief AIPlayerBehaviorTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"

#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"
#include "VelocityManager.h"

namespace basecross {
	namespace Enemy {

		EnemyBase::EnemyBase(const std::shared_ptr<GameObject>& objPtr) :
			Component(objPtr),
			m_tupleSpace(new Tuple::TupleSpace())
		{}

		void EnemyBase::OnCreate() {

			if (auto shareManager = ShareClassesManager::GetInstance(GetStage())) {
				shareManager->AddShareClass<EnemyBase>(GetThis<EnemyBase>());
			}
		}

		void EnemyBase::OnStart() {
			m_targetManager = GetGameObject()->GetComponent<TargetManager>(false);
			m_velocityManager = GetGameObject()->GetComponent<VelocityManager>(false);
			m_onlineSynchronizer = GetGameObject()->GetComponent<OnlinePlayerSynchronizer>(false);
		}

		void EnemyBase::OnUpdate() {
			auto velocity = m_velocityManager.lock()->GetVelocity();
			velocity /= m_velocityManager.lock()->GetMaxSpeed();
			Debug::GetInstance()->Log(velocity);
			auto moveVec = Vec2(velocity.x, velocity.z);
			m_onlineSynchronizer.lock()->Move(moveVec);
			//m_onlineSynchronizer.lock()->Move(moveVec);
		}

		void EnemyBase::SetTarget(const std::shared_ptr<GameObject>& target) {
			if (!m_targetManager.lock()) {
				return;
			}

			m_targetManager.lock()->SetTarget(target);
		}

		std::shared_ptr<GameObject> EnemyBase::GetTarget() const {
			return m_targetManager.lock() ? m_targetManager.lock()->GetTarget() : nullptr;
		}

		std::shared_ptr<GameObject> EnemyBase::GetSelfObject() const {
			return GetGameObject();
		}

		std::shared_ptr<Tuple::TupleSpace> EnemyBase::GetTupleSpace() const noexcept {
			return m_tupleSpace;
		}

	}
}