
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


	namespace Enemy {

		class EnemyBase;
		class HidePlacePatrolTree;

		namespace StateNode {

			class HidePlacePatrol : public EnemyStateNodeBase<EnemyBase>
			{
				std::unique_ptr<HidePlacePatrolTree> m_behaviorTree;

			public:
				HidePlacePatrol(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;
				
			};

		}
	}
}