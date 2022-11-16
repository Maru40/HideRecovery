/*!
@file StateNode_Dyning.cpp
@brief StateNode_Dyning‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Component/Stator/StateNode/StateNode_Goal.h"
#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Enemy/Component/SelfAstarNodeController.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Goal::Goal(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner)
			{
				auto object = owner->GetGameObject();

				m_goalAnimationController = object->GetComponent<GoalAnimationController>(false);
				AddChangeComponent(object->GetComponent<SelfAstarNodeController>(false), false, true);
			}

			void Goal::OnStart() {
				StartChangeComponents();
				Debug::GetInstance()->Log(L"GoalStart");
			}

			bool Goal::OnUpdate() {
				return !m_goalAnimationController.lock()->IsGoalAnimation();
			}

			void Goal::OnExit() {
				ExitChangeComponents();
				Debug::GetInstance()->Log(L"Goal_Exit");
			}

		}
	}
}