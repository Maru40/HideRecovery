/*!
@file StateNode_Dyning.cpp
@brief StateNode_Dyning‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Goal.h"
#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Player/Component/GoalAnimationController.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Goal::Goal(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner)
			{
				m_goalAnimationController = owner->GetGameObject()->GetComponent<GoalAnimationController>(false);
			}

			void Goal::OnStart() {

			}

			bool Goal::OnUpdate() {
				return !m_goalAnimationController.lock()->IsGoalAnimation();
			}

			void Goal::OnExit() {
				Debug::GetInstance()->Log(L"Goal_Exit");
			}

		}
	}
}