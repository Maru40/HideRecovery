
/*!
@file StateNode_HidePlacePatrol.h
@brief StateNode_HidePlacePatrol�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
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
		}

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			///	�o�g���X�e�[�g
			//--------------------------------------------------------------------------------------
			class Buttle : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using ButtleTree = maru::Behavior::SubBehavior::ButtleTree;

			private:
				std::weak_ptr<TargetManager> m_targetManager;

				std::unique_ptr<ButtleTree> m_behaviorTree;

			public:
				Buttle(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;

			private:

				void ObserveOtherFindTarget(const std::shared_ptr<Tuple::FindTarget>& tuple);

				bool HasTarget() const;

			};

		}
	}
}