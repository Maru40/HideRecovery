/*!
@file IsInEyeTargets.cpp
@brief IsInEyeTargetsなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "../Interface/I_Decorator.h"

#include "ObserveTargets.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/ObserveIsInEyeTarget.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Interface/I_TeamMember.h"
#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Decorator {

				ObserveTargets::ObserveTargets(
					const std::shared_ptr<Enemy::EnemyBase>& owner,
					const std::vector<std::shared_ptr<GameObject>>& targets
				):
					DecoratorBase(owner),
					m_observeEyeTargets(new ObserveIsInEyeTarget(owner->GetGameObject()->GetComponent<EyeSearchRange>(), targets))
				{
					auto object = owner->GetGameObject();

					m_transform = object->GetComponent<Transform>(false);
					m_teamMember = object->GetComponent<I_TeamMember>(false);
					m_targetManager = object->GetComponent<TargetManager>(false);
				}

				bool ObserveTargets::CanUpdate() {
					//将来的にはここで検索書けないようにする。
					m_observeEyeTargets->ClearObserveTargets();						//監視ターゲットのクリア
					m_observeEyeTargets->SetObserveTargets(SearchObserveTargets());	//ターゲットの指定

					auto targets = SearchOtherTarget(m_observeEyeTargets->SearchIsInEyeTargets());	//視界内のターゲットを取得

					NotifyTargets(targets);

					return true;
				}

				void ObserveTargets::NotifyTargets(const std::vector<std::shared_ptr<GameObject>>& targets) {
					using namespace basecross::Enemy;

					auto selfPosition = m_transform.lock()->GetPosition();
					auto assignedFaction = GetOwner()->GetAssignedFaction();
					auto tupleSpace = assignedFaction->GetTupleSpace();
					//ターゲットを見つけたことを通知する。
					for (const auto& target : targets) {
						auto targetPosition = target->GetComponent<Transform>()->GetPosition();
						auto toTargetVec = targetPosition - selfPosition;
						tupleSpace->Write<Tuple::FindTarget>(GetOwner(), target, toTargetVec.length());
					}
				}

				std::vector<std::shared_ptr<GameObject>> ObserveTargets::SearchOtherTarget(
					const std::vector<std::shared_ptr<GameObject>>& targets
				) const 
				{
					if (!HasTarget()) {	//ターゲットが存在しないなら、処理をしない。
						return targets;
					}

					std::vector<std::shared_ptr<GameObject>> result;

					//ターゲット管理で指定されたターゲットをのぞく。
					for (auto& target : targets) {
						if (target != m_targetManager.lock()->GetTarget()) {
							result.push_back(target);
						}
					}

					return result;
				}

				bool ObserveTargets::HasTarget() const {
					auto targetManager = m_targetManager.lock();
					return targetManager && targetManager->HasTarget();
				}

				std::vector<std::shared_ptr<GameObject>> ObserveTargets::SearchObserveTargets() const {
					std::vector<std::shared_ptr<GameObject>> result;

					auto selfTeamMember = m_teamMember.lock();
					if (!selfTeamMember) {
						return result;
					}

					//別チームをバトル監視対象にする。
					auto teamMembers = ShareClassesManager::GetInstance()->GetCastShareClasses<I_TeamMember>();
					for (auto& member : teamMembers) {
						//別チームなら、監視対象に追加
						if (member.lock()->GetTeam() != selfTeamMember->GetTeam()) {
							result.push_back(member.lock()->GetOwnerObject());
						}
					}

					return result;
				}

			}
		}
	}
}