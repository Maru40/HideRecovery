/*!
@file MoveAstar.cpp
@brief MoveAstarなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "MoveAstar_Ex.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	MoveAstarのパラメータ
		//--------------------------------------------------------------------------------------

		MoveAstar_Ex_Parametor::MoveAstar_Ex_Parametor():
			movePositionsParam(new Task_MovePositions::Parametor())
		{}

		//--------------------------------------------------------------------------------------
		///	MoveAstar
		//--------------------------------------------------------------------------------------

		MoveAstar_Ex::MoveAstar_Ex(
			const std::shared_ptr<Enemy::EnemyBase>& owner,
			const Parametor* const parametor
		):
			TaskNodeBase(owner),
			m_param(parametor),
			m_taskList(new TaskList<TaskEnum>())
		{}

		void MoveAstar_Ex::OnCreate() {
			DefineTask();
		}

		void MoveAstar_Ex::OnStart() {
			m_taskList->ForceStop();

			SelectTask();
		}

		bool MoveAstar_Ex::OnUpdate() {
			return IsEnd();
		}

		void MoveAstar_Ex::OnExit() {
			m_taskList->ForceStop();
		}

		void MoveAstar_Ex::DefineTask() {
			auto owner = GetOwner();
			auto object = owner->GetGameObject();

			//移動タスク
			m_taskList->DefineTask<Task_MovePositions>(TaskEnum::Move, object, m_param->movePositionsParam);
		}

		void MoveAstar_Ex::SelectTask() {
			TaskEnum tasks[] = {
				TaskEnum::Move
			};

			for (auto& task : tasks) {
				m_taskList->AddTask(task);
			}
		}

		bool MoveAstar_Ex::IsEnd() const {
			return true;
		}
	}
}