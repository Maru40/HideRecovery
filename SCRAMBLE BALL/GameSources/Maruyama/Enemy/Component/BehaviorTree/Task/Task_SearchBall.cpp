/*!
@file Task_SearchBall.cpp
@brief Task_SearchBall‚È‚ÇŽÀ‘Ì
*/

#include "stdafx.h"
#include "Project.h"

#include "Task_SearchBall.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

namespace basecross {
	namespace maru {

		namespace Behavior {

			namespace Task {

				SearchBall::SearchBall(const std::shared_ptr<Enemy::EnemyBase>& owner) :
					TaskBase(owner)
				{}

				void SearchBall::OnStart() {

				}

				bool SearchBall::OnUpdate() {
					return true;
				}

				void SearchBall::OnExit() {

				}

			}
		}
	}
}