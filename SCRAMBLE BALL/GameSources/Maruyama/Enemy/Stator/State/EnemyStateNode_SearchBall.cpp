/*!
@file EnemyStatorBase.cpp
@brief EnemyStatorBase‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyStateNode_SearchBall.h"

#include "Maruyama/Utility/StateMachine/StateMachine.h"
#include "Maruyama/Enemy/Component/EnemyBase.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			SearchBall::SearchBall(const std::shared_ptr<EnemyBase>& owner) :
				NodeBase_StateMachine<EnemyBase>(owner),
				m_stateMachine(new StateMachine())
			{}

			void SearchBall::OnStart() {
				
			}

			bool SearchBall::OnUpdate() {
				m_stateMachine->OnUpdate();

				return false;
			}

			void SearchBall::OnExit() {

			}

		}

	}
}