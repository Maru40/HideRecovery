
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMoveのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "TargetManager.h"

#include "Task_PlowlingMove.h"

#include "VelocityManager.h"
#include "RotationController.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// タスクタイプ
		//--------------------------------------------------------------------------------------

		enum class Task_PlowlingMove::TaskEnum
		{
			ToTargetMove,
		};

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		Task_PlowlingMove::Parametor::Parametor()
			:Parametor(std::make_shared<Task_ToTargetMove::Parametor>())
		{}

		Task_PlowlingMove::Parametor::Parametor(const std::shared_ptr<Task_ToTargetMove::Parametor>& moveParamPtr)
			:moveParamPtr(moveParamPtr)
		{}

		//--------------------------------------------------------------------------------------
		/// スライムの徘徊移動タスク本体
		//--------------------------------------------------------------------------------------

		Task_PlowlingMove::Task_PlowlingMove(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
		{
			DefineTask();
		}

		void Task_PlowlingMove::OnStart() {
			m_taskList->ForceStop(false);
			SelectTask();
		}

		bool Task_PlowlingMove::OnUpdate() {
			m_taskList->UpdateTask();

			Rotation();
			return m_taskList->IsEnd();
		}

		void Task_PlowlingMove::OnExit() {

		}

		void Task_PlowlingMove::Rotation() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
			auto rotationController = ownerObject->GetComponent<RotationController>(false);
			if (velocityManager && rotationController) {
				rotationController->SetDirect(velocityManager->GetVelocity());
			}
		}

		void Task_PlowlingMove::DefineTask() {
			//目的地に向かって移動する処理
			m_taskList->DefineTask(TaskEnum::ToTargetMove, std::make_shared<Task_ToTargetMove>(GetOwner()->GetGameObject(), m_paramPtr->moveParamPtr));
		}

		void Task_PlowlingMove::SelectTask() {
			m_taskList->AddTask(TaskEnum::ToTargetMove);
		}

	}
}