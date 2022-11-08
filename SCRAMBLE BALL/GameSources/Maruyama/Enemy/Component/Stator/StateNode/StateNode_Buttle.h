
/*!
@file StateNode_HidePlacePatrol.h
@brief StateNode_HidePlacePatrolなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

				//class HidePlacePatrolTree;
				class ButtleTree;

			}
		}
	}

	namespace Enemy {

		class EnemyBase;

		namespace Tuple {
			class FindTarget;
			class Damaged;
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	バトルステート
			//--------------------------------------------------------------------------------------
			class Buttle : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using ButtleTree = maru::Behavior::SubBehavior::ButtleTree;

			private:
				std::weak_ptr<Transform> m_transform;
				std::weak_ptr<TargetManager> m_targetManager;

				std::unique_ptr<ButtleTree> m_behaviorTree;

			public:
				Buttle(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;

			private:

				void ObserveOtherFindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple);

				void ObserveDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				void SelfDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				void OtherDamaged(const std::shared_ptr<Tuple::Damaged>& tuple);

				//そのターゲットを狙う評価値を計算して返す。
				float CalculateEvalutionValue(const std::shared_ptr<GameObject>& target);

				bool HasTarget() const;

			};

		}
	}
}