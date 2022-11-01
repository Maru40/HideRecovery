/*!
@file StateNode_HidePlacePatrol.cpp
@brief StateNode_HidePlacePatrol‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_HidePlacePatrol.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/SubBehaviorTree/HidePlacePatrolTree.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner) :
				EnemyStateNodeBase(owner),
				m_behaviorTree(new HidePlacePatrolTree(owner->GetGameObject()))
			{
				m_behaviorTree->OnCreate();
			}

			void HidePlacePatrol::OnStart() {
				
			}

			bool HidePlacePatrol::OnUpdate() {
				m_behaviorTree->OnUpdate();

				return true;
			}

			void HidePlacePatrol::OnExit() {
				m_behaviorTree->ForceStop();
			}

		}
	}
}