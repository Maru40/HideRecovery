
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

				std::unique_ptr<HidePlacePatrolTree> m_behaviorTree;	//�r�w�C�r�A�c���[

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