
/*!
@file StateNode_Dyning.h
@brief StateNode_Dyning‚È‚Ç
’S“–FŠÛR—TŠì
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "Maruyama/StateMachine/Node_StateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	‘O•ûéŒ¾
	//--------------------------------------------------------------------------------------
	//class ObserveIsInEyeTarget;
	//class I_TeamMember;
	//class EyeSearchRange;
	class GoalAnimationController;

	namespace maru {
		namespace Behavior {
			namespace SubBehavior {

			}
		}
	}

	namespace Enemy {

		class EnemyBase;
		//class I_FactionMember;

		namespace Tuple {
			//class Damaged;
		}

		namespace StateNode {

			class Goal : public maru::NodeBase_StateMachine_Ex<EnemyBase>
			{
				std::weak_ptr<GoalAnimationController> m_goalAnimationController;

			public:
				Goal(const std::shared_ptr<EnemyBase>& owner);

				void OnStart() override;

				bool OnUpdate() override;

				void OnExit() override;
			};

		}
	}
}