/*!
@file StateNode_Dyning.cpp
@brief StateNode_Dyning‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Dyning.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Dyning::Dyning(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner)
			{
				
			}

			void Dyning::OnStart() {
				StartChangeComponents();


			}

			bool Dyning::OnUpdate() {
				return false;
			}

			void Dyning::OnExit() {
				ExitChangeComponents();
			}

		}
	}
}