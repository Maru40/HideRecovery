/*!
@file EnemyStatorBase.h
@brief EnemyStatorBase�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/StateMachine/Node_StateMachine.h"

namespace basecross {

	//�r�w�C�r�A���K�؂Ȃ��߁A���ݎg�p���Ă��Ȃ��B

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