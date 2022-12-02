
/*!
@file StateNode_Dead.h
@brief StateNode_Dead�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "Maruyama/StateMachine/Node_StateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class ObserveIsInEyeTarget;
	class I_TeamMember;
	class EyeSearchRange;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

			}
		}
	}

	namespace Enemy {

		class EnemyBase;
		class I_FactionMember;

		namespace Tuple {
			class Damaged;
		}

		namespace StateNode {

			class Dead : public maru::NodeBase_StateMachine<EnemyBase>
			{
			public:
				Dead(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;
			};

		}
	}
}