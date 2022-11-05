/*!
@file StateNode_HidePlacePatrol.cpp
@brief StateNode_HidePlacePatrolなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_HidePlacePatrol.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/SubBehaviorTree/HidePlacePatrolTree.h"

#include "Maruyama/Utility/ObserveIsInEyeTarget.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			HidePlacePatrol::HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner) :
				EnemyStateNodeBase(owner),
				m_behaviorTree(new HidePlacePatrolTree(owner->GetGameObject())),
				m_observeButtleTarget(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(false)))
			{
				m_behaviorTree->OnCreate();

				auto object = owner->GetGameObject();
				m_teamMember = object->GetComponent<I_TeamMember>(false);
				m_factionMember = object->GetComponent<I_FactionMember>(false);
			}

			void HidePlacePatrol::OnStart() {
				SettingObserveButtleTargets();
			}

			bool HidePlacePatrol::OnUpdate() {
				m_behaviorTree->OnUpdate();

				ObserveButtleTarget();	//バトルターゲットを監視する。

				return true;
			}

			void HidePlacePatrol::OnExit() {
				m_behaviorTree->ForceStop();
			}

			void HidePlacePatrol::ObserveButtleTarget() {
				//視界内にいるターゲットを全て取得
				auto targets = m_observeButtleTarget->SearchIsInEyeTargets();

				//視界内のターゲットが存在しないならfalse
				if (targets.empty()) {
					return;
				}

				//ターゲットを見つけたことをAIDirectorに伝える。
				for (auto& target : targets) {
					auto factionMember = m_factionMember.lock();
					auto tupleSpace = factionMember->GetAssignedFaction()->GetTupleSpace();
					tupleSpace->Write<Tuple::FindTarget>(GetOwner(), target);	//ターゲットを通知
				}
			}

			void HidePlacePatrol::SettingObserveButtleTargets() {
				auto selfTeamMember = m_teamMember.lock();
				if (!selfTeamMember) {
					return;
				}

				m_observeButtleTarget->ClearObserveTargets();	//監視ターゲットのクリア

				//別チームをバトル監視対象にする。
				auto teamMembers = ShareClassesManager::GetInstance()->GetCastShareClasses<I_TeamMember>();
				for (auto& member : teamMembers) {
					//別チームなら、監視対象に追加
					if (member.lock()->GetTeam() != selfTeamMember->GetTeam()) {
						m_observeButtleTarget->AddObserveTarget(member.lock()->GetOwnerObject());
					}
				}
			}

		}
	}
}