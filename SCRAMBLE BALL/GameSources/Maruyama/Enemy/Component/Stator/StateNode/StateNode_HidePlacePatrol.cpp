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
#include "Maruyama/Enemy/AIDirector/PatrolCoordinator/HidePlacePatrol.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Watanabe/DebugClass/Debug.h"

//#include "Maruyama/Enemy/Component/AIVirtualController.h"

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
				m_targetManager = object->GetComponent<TargetManager>(false);
			}

			void HidePlacePatrol::OnStart() {
				SettingObserveButtleTargets();

				//パトルールファクションに移動
				auto factionCoordinator = GetOwner()->GetFactionCoordinator();
				factionCoordinator->TransitionFaction<AICoordinator::Patrol::HidePlacePatrol>(GetOwner());

				//ダメージ通知を受け取れるようにする。
				if (auto factionMember = m_factionMember.lock()) {
					factionMember->GetAssignedFaction()->GetTupleSpace()->Notify<Tuple::Damaged>(
						GetOwner(),
						[&](const std::shared_ptr<Tuple::Damaged>& tuple) { Damaged(tuple); },
						[&](const std::shared_ptr<Tuple::Damaged>& tuple) { return tuple->GetRequester() == GetOwner(); }
					);
				}
			}

			bool HidePlacePatrol::OnUpdate() {
				m_behaviorTree->OnUpdate();

				ObserveButtleTarget();	//バトルターゲットを監視する。

				return true;
			}

			void HidePlacePatrol::OnExit() {
				m_behaviorTree->ForceStop();

				auto  member = GetOwner();
				auto assignedFaction = member->GetAssignedFaction();
				assignedFaction->RemoveMember(member);	//ファクションから離脱。
				assignedFaction->GetTupleSpace()->RemoveAllNotifys(member); //タプルスペースに登録された者を変更
				assignedFaction->GetTupleSpace()->RemoveAllTuples(member);
			}

			void HidePlacePatrol::Damaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				//死亡状態なら遷移しない
				//auto status = tuple->GetDamageData().attacker->GetComponent<PlayerStatus>(false);
				//if (status && status->IsDead()) {
				//	return;
				//}

				//ダメージを与えてきた相手を伝える。
				auto tupleSpace = m_factionMember.lock()->GetAssignedFaction()->GetTupleSpace();
				tupleSpace->Write<Tuple::FindTarget>(GetOwner(), tuple->GetDamageData().attacker, tuple->GetValue());
				tupleSpace->Take(tuple);	//タプルを自分自身の者とする。
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
					auto status = target->GetComponent<PlayerStatus>(false);
					if (status && status->IsDead()) {
						continue;
					}

					auto targetManager = m_targetManager.lock();
					if (targetManager && targetManager->HasTarget() && targetManager->GetTarget() == target) {
						continue;
					}

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