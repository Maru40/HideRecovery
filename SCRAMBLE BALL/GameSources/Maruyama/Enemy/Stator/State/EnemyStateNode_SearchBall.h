/*!
@file EnemyStatorBase.h
@brief EnemyStatorBaseなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/StateMachine/Node_StateMachine.h"

namespace basecross {

	//ビヘイビアが適切なため、現在使用していない。

	namespace maru {
		template<class EnumType, class OwnerType, class TransitionStructMember>
		class StateMachine;
	}

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//enum class StateType_SearchBall {
			//	
			//};

			//struct TransitionMember_SearchBall {

			//};

			//class SearchBall : public maru::NodeBase_StateMachine<EnemyBase>
			//{	
			//public:
			//	using StateType = StateType_SearchBall;
			//	using TransitionMember = TransitionMember_SearchBall;
			//	using StateMachine = maru::StateMachine<StateType, EnemyBase, TransitionMember>;

			//private:
			//	std::unique_ptr<StateMachine> m_stateMachine;

			//public:
			//	SearchBall(const std::shared_ptr<EnemyBase>& owner);

			//	void OnStart() override;

			//	bool OnUpdate() override;

			//	void OnExit() override;
			//};

		}

	}
}