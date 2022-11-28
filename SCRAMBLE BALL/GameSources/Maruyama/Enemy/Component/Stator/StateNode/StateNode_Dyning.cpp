/*!
@file StateNode_Dyning.cpp
@brief StateNode_Dyningなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Dyning.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

#include "Maruyama/Enemy/Component/Stator/AIPlayerStator.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Dyning::Dyning(const std::shared_ptr<EnemyBase>& owner):
				NodeBase_StateMachine(owner)
			{
				
			}

			void Dyning::OnStart() {
				//StartChangeComponents();

				//ファクションをどこにも属さないようにする。(応急処置、Statorがupdateでないならは...)
				auto stator = GetOwner()->GetGameObject()->GetComponent<Enemy::AIPlayerStator>(false);
				auto assignedFanction = GetOwner()->GetAssignedFaction();
				if (assignedFanction && stator && stator->GetUpdateActive()) {
					assignedFanction->RemoveMember(GetOwner());
					assignedFanction->GetTupleSpace()->RemoveAllNotifys(GetOwner());
				}
			}

			bool Dyning::OnUpdate() {
				return false;
			}

			void Dyning::OnExit() {
				//ExitChangeComponents();
			}

		}
	}
}