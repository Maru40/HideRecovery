
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMove�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/TargetManager.h"

#include "Task_PlowlingMove.h"

#include "VelocityManager.h"
#include "Maruyama/Utility/Component/RotationController.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �^�X�N�^�C�v
		//--------------------------------------------------------------------------------------

		enum class Task_PlowlingMove::TaskEnum
		{
			ToTargetMove,
		};

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		Task_PlowlingMove::Parametor::Parametor()
			:Parametor(std::make_shared<Task::ToTargetMove::Parametor>())
		{}

		Task_PlowlingMove::Parametor::Parametor(const std::shared_ptr<Task::ToTargetMove::Parametor>& moveParamPtr)
			:moveParamPtr(moveParamPtr)
		{}

		//--------------------------------------------------------------------------------------
		/// �X���C���̜p�j�ړ��^�X�N�{��
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
				rotationController->SetDirection(velocityManager->GetVelocity());
			}
		}

		void Task_PlowlingMove::DefineTask() {
			//�ړI�n�Ɍ������Ĉړ����鏈��
			m_taskList->DefineTask(TaskEnum::ToTargetMove, std::make_shared<Task::ToTargetMove>(GetOwner()->GetGameObject(), m_paramPtr->moveParamPtr));
		}

		void Task_PlowlingMove::SelectTask() {
			m_taskList->AddTask(TaskEnum::ToTargetMove);
		}

	}
}