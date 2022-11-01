
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
	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

				class HidePlacePatrolTree;

			}
		}
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			class HidePlacePatrol : public EnemyStateNodeBase<EnemyBase>
			{
				using HidePlacePatrolTree = maru::Behavior::SubBehavior::HidePlacePatrolTree;

				std::unique_ptr<HidePlacePatrolTree> m_behaviorTree;	//ビヘイビアツリー

			public:
				HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner);

				virtual ~HidePlacePatrol() = default;

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;
				
			};

		}
	}
}