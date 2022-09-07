
/*!
@file Wait.cpp
@brief Task_Waitのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Task_AstarMove.h"

#include "Task_MovePositions.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"

#include "Maruyama/Utility/Component/TargetManager.h"
#include "VelocityManager.h"
#include "Maruyama/Utility/Component/RotationController.h"

#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// パラメータ
		//--------------------------------------------------------------------------------------

		AstarMove_Parametor::AstarMove_Parametor(const MoveType& moveType)
			:AstarMove_Parametor(
				moveType,
				std::make_shared<Task_MovePositions_Parametor>()
			)
		{}

		AstarMove_Parametor::AstarMove_Parametor(
			const MoveType& moveType,
			const std::shared_ptr<Task_MovePositions_Parametor>& movePositionsParamPtr
		) :
			moveType(moveType),
			movePositonsParamPtr(movePositionsParamPtr)
		{}

		//--------------------------------------------------------------------------------------
		/// Astarを利用した移動タスク本体
		//--------------------------------------------------------------------------------------

		AstarMove::AstarMove(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase(objPtr), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
		{
			DefineTask();
		}

		void AstarMove::OnStart() {
			m_taskList->ForceStop(false);
			SetRoutePositions();
			SelectTask();
		}

		bool AstarMove::OnUpdate() {
			m_taskList->UpdateTask();
			Rotation();

			return m_taskList->IsEnd();
		}

		void AstarMove::OnExit() {
			m_taskList->ForceStop(true);
		}

		void AstarMove::Rotation() {
			auto rotationController = GetOwner()->GetComponent<RotationController>(false);
			auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false);
			if (velocityManager && rotationController) {
				rotationController->SetDirect(velocityManager->GetVelocity());
			}
		}

		void AstarMove::DefineTask() {
			m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task_MovePositions>(GetOwner(), m_paramPtr->movePositonsParamPtr));
		}

		void AstarMove::SelectTask() {
			m_taskList->AddTask(TaskEnum::Move);
		}

		void AstarMove::SetRoutePositions() {
			//将来的にAIDirectorに依頼して逃げ方や追いかけ方を実装したい。
			const auto& impactMap = maru::FieldImpactMap::GetInstance();
			auto targetManager = GetOwner()->GetComponent<TargetManager>(false);
			if (!targetManager && !targetManager->HasTarget()) {
				return;
			}

			auto selfPosition = GetOwner()->GetComponent<Transform>()->GetPosition();
			auto targetPosition = targetManager->GetTargetPosition();
			auto toTargetDirect = targetManager->CalcuToTargetVec();
			switch (m_paramPtr->moveType)
			{
			case MoveType::Chase:
				m_paramPtr->movePositonsParamPtr->positions = impactMap->GetRoutePositons(selfPosition, targetPosition);
				break;

			case MoveType::Evade:
				m_paramPtr->movePositonsParamPtr->positions = impactMap->GetRoutePositons(selfPosition, selfPosition + -toTargetDirect);
				break;
			}
		}

	}
}