/*!
@file StateNode_HidePlacePatrol.cpp
@brief StateNode_HidePlacePatrol‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Buttle.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"


namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Buttle::Buttle(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner)
			{}

			void Buttle::OnStart() {

			}

			bool Buttle::OnUpdate() {
				return true;
			}

			void Buttle::OnExit() {

			}

		}
	}
}