
/*!
@file StateNode_Dyning.h
@brief StateNode_Dyning�Ȃ�
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

			class Dyning : public basecross::maru::NodeBase_StateMachine<EnemyBase>
			{
			public:
				Dyning(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;
			};

		}
	}
}