/*!
@file StateNode_Dead.cpp
@brief StateNode_Dead‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Dead.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Dead::Dead(const std::shared_ptr<EnemyBase>& owner) :
				NodeBase_StateMachine(owner)
			{

			}

			void Dead::OnStart() {
				//StartChangeComponents();


			}

			bool Dead::OnUpdate() {
				return false;
			}

			void Dead::OnExit() {
				//ExitChangeComponents();
			}

		}
	}
}