/*!
@file StateNode_Dyning.cpp
@brief StateNode_Dyning�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Dyning.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Dyning::Dyning(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner)
			{
				
			}

			void Dyning::OnStart() {
				StartChangeComponents();

				//�t�@�N�V�������ǂ��ɂ������Ȃ��悤�ɂ���B
				if (auto assignedFanction = GetOwner()->GetAssignedFaction()) {
					assignedFanction->RemoveMember(GetOwner());
					assignedFanction->GetTupleSpace()->RemoveAllNotifys(GetOwner());
				}
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