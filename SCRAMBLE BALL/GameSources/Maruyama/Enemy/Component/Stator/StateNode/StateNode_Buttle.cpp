/*!
@file StateNode_HidePlacePatrol.cpp
@brief StateNode_HidePlacePatrolなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "StateNode_Buttle.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"
#include "Maruyama/Enemy/Behavior/SubBehaviorTree/ButtleTree.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "Watanabe/Component/PlayerStatus.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			Buttle::Buttle(const std::shared_ptr<EnemyBase>& owner):
				EnemyStateNodeBase(owner),
				m_behaviorTree(new ButtleTree(owner->GetGameObject()))
			{
				auto object = owner->GetGameObject();

				m_transform = object->GetComponent<Transform>();
				m_targetManager = object->GetComponent<TargetManager>(false);

				m_behaviorTree->OnCreate();
			}

			void Buttle::OnStart() {
				//通知の受け取り登録
				auto tupleSpace = GetOwner()->GetAssignedFaction()->GetTupleSpace();
				tupleSpace->Notify<Tuple::FindTarget>(
					GetOwner(), 
					[&](const std::shared_ptr<Tuple::FindTarget>& tuple) { ObserveOtherFindTarget(tuple); }
				);

				tupleSpace->Notify<Tuple::Damaged>(
					GetOwner(),
					[&](const std::shared_ptr<Tuple::Damaged>& tuple) { ObserveDamaged(tuple); }
				);
			}

			bool Buttle::OnUpdate() {
				m_behaviorTree->OnUpdate();

				return true;
			}

			void Buttle::OnExit() {
				m_behaviorTree->ForceStop();
			}

			void Buttle::ObserveOtherFindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple) {
				//条件次第でターゲット変更をする。
				if (!HasTarget()) {
					return;
				}
				
				auto targetManager = m_targetManager.lock();
				//評価値が今のターゲットより小さいなら
				if (tuple->GetValue() < targetManager->CalcuToTargetVec().length()) {
					//ファクションに、こいつを目標にしたいことを伝える。
					targetManager->SetTarget(tuple->GetTarget());
				}
			}

			void Buttle::ObserveDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				//自分自身がダメージを受けた時と、違う相手がダメージを受けた時で処理が違う。
				tuple->GetRequester() == GetOwner() ? SelfDamaged(tuple) : OtherDamaged(tuple);
			}

			void Buttle::SelfDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				auto otherTarget = tuple->GetDamageData().attacker;
				if (!HasTarget()) {
					//ターゲットを持ってないなら、ターゲットを設定。
					m_targetManager.lock()->SetTarget(otherTarget);
					return;
				}

				//同じターゲットなら切り替えの必要はない。
				if (m_targetManager.lock()->GetTarget() == otherTarget) {
					return;
				}

				float currentValue = CalculateEvalutionValue(m_targetManager.lock()->GetTarget());
				float otherValue = CalculateEvalutionValue(otherTarget);

				//ohterValueの方が優先なら、ターゲットを切り替える。
				if (otherValue < currentValue) {
					m_targetManager.lock()->SetTarget(otherTarget);
				}
			}

			void Buttle::OtherDamaged(const std::shared_ptr<Tuple::Damaged>& tuple) {
				auto otherTarget = tuple->GetDamageData().attacker;

				if (!HasTarget()) {
					m_targetManager.lock()->SetTarget(otherTarget);
					return;
				}

				//攻撃中の対象なら、特に何もしない。
				if (m_targetManager.lock()->GetTarget() == otherTarget) {
					return;
				}

				//攻撃対象以外なら、Help
				float currentValue = CalculateEvalutionValue(m_targetManager.lock()->GetTarget());
				float otherValue = CalculateEvalutionValue(otherTarget);

				//ohterValueの方が優先なら、ターゲットを切り替える。
				if (otherValue < currentValue) {
					auto tupleSpace = GetOwner()->GetAssignedFaction()->GetTupleSpace();
					//tupleSpace->Write<Tuple::>();	//HelpWriteする。
				}
				
			}

			float Buttle::CalculateEvalutionValue(const std::shared_ptr<GameObject>& target) {
				float result = 0.0f;

				auto targetTrans = target->GetComponent<Transform>();

				float toTargetRange = (targetTrans->GetPosition() - m_transform.lock()->GetPosition()).length();
				result += toTargetRange;	//距離を評価値とする。

				if (auto status = target->GetComponent<PlayerStatus>(false)) {
					float hpRate = (float)status->GetStatus().hp / (float)status->GetStatus().maxHp;
					result *= hpRate;	//hpが低いほど、距離を減衰(ターゲットにしやすくする。)
				}

				return result;
			}

			bool Buttle::HasTarget() const {
				auto targetManager = m_targetManager.lock();
				return targetManager->HasTarget();
			}
		}
	}
}