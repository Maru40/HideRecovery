/*!
@file Task_MovePositions.cpp
@brief Task_MovePositions�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Task_ToTargetMove.h"

#include "Task_MovePositions.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�����[�^
	//--------------------------------------------------------------------------------------

	using MoveType = Task::ToTargetMove::MoveType;
	using DeltaType = Task::ToTargetMove::DeltaType;

	Task_MovePositions_Parametor::Task_MovePositions_Parametor() :
		Task_MovePositions_Parametor(std::make_shared<Task::ToTargetMove::Parametor>(
			Vec3(0.0f), Vec3(0.0f), 3.0f, 2.0f, MoveType::SeekVelocity, DeltaType::Normal))
	{}

	Task_MovePositions_Parametor::Task_MovePositions_Parametor(const Task::ToTargetMove::Parametor& moveParametor)
		: Task_MovePositions_Parametor(std::make_shared<Task::ToTargetMove::Parametor>(moveParametor))
	{}

	Task_MovePositions_Parametor::Task_MovePositions_Parametor(
		const std::shared_ptr<Task::ToTargetMove::Parametor>& moveParamPtr
	):
		moveParamPtr(moveParamPtr), 
		isSetInitializePosition(false)
	{}

	//--------------------------------------------------------------------------------------
	/// �p�j�ړ�����^�X�N�{��
	//--------------------------------------------------------------------------------------

	Task_MovePositions::Task_MovePositions(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr)
		:TaskNodeBase<GameObject>(owner), m_paramPtr(paramPtr), m_taskList(new TaskList<TaskEnum>())
	{
		DefineTask();
	}

	void Task_MovePositions::OnStart() {
		m_taskList->ForceStop(false);
		m_index = 0;
		if (IsEnd()) {
			return;
		}

		ChangeMoveTargetPosition(m_paramPtr->positions[m_index]);	//�ړ��ꏊ�̕ύX
		SelectTask();	//�^�X�N�̑I��

		if (m_paramPtr->isSetInitializePosition) {	//���񎞂ɏu�Ԉړ����邩�ǂ���
			GetOwner()->GetComponent<Transform>()->SetPosition(m_paramPtr->positions[m_index]);
		}
	}

	bool Task_MovePositions::OnUpdate() {
		m_taskList->UpdateTask();

		if (m_taskList->IsEnd()) {
			NextPosition();
			SelectTask();
		}

		return IsEnd();
	}

	void Task_MovePositions::OnExit() {
		m_taskList->ForceStop();
	}

	void Task_MovePositions::DefineTask() {
		//�^�[�Q�b�g�Ɍ���������
		m_taskList->DefineTask(TaskEnum::Move, std::make_shared<Task::ToTargetMove>(GetOwner(), m_paramPtr->moveParamPtr));
	}

	void Task_MovePositions::SelectTask() {
		m_taskList->AddTask(TaskEnum::Move);
	}

	void Task_MovePositions::NextPosition() {
		m_index++;
		if (IsEnd()) {
			return;
		}

		ChangeMoveTargetPosition(m_paramPtr->positions[m_index]);
	}

	void Task_MovePositions::ChangeMoveTargetPosition(const Vec3& position) {
		auto& moveParamPtr = m_paramPtr->moveParamPtr;
		moveParamPtr->startPosition = GetOwner()->GetComponent<Transform>()->GetPosition();
		moveParamPtr->endPosition = position;
	}

	//--------------------------------------------------------------------------------------
	/// �A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	bool Task_MovePositions::IsEnd() {
		return m_paramPtr->positions.size() <= m_index;
	}
}


//endbasecross