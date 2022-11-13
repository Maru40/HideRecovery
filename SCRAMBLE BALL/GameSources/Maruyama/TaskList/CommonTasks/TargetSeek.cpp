
/*!
@file Task_PlowlingMove.cpp
@brief Task_PlowlingMove�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetSeek.h"

#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

namespace basecross {

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g��ǂ�������^�X�N�̃p�����[�^
		//--------------------------------------------------------------------------------------

		TargetSeek_Parametor::TargetSeek_Parametor():
			toTargetMoveParam(new Task::ToTargetMove::Parametor())
		{}

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g��ǂ�������^�X�N
		//--------------------------------------------------------------------------------------

		TargetSeek::TargetSeek(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr) :
			TaskNodeBase(owner),
			m_paramPtr(paramPtr),
			m_taskList(new TaskList<TaskEnum>())
		{
			DefineTask();

			m_transform = owner->GetComponent<Transform>();
			m_targetManager = owner->GetComponent<TargetManager>();
		}

		void TargetSeek::OnStart() {
			//�^�[�Q�b�g�����݂��Ȃ��Ȃ珈�����΂��B
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {	
				return;
			}

			SettingSeek();	//�Ǐ]�J�n�O�̃Z�b�e�B���O

			SelectTask();
		}

		bool TargetSeek::OnUpdate() {
			UpdateSeekPosition();
			m_taskList->UpdateTask();

			return IsEnd();
		}

		void TargetSeek::OnExit() {
			m_taskList->ForceStop();
		}

		void TargetSeek::SettingSeek() {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return;
			}

			auto& toTargetMoveParam = m_paramPtr->toTargetMoveParam;

			toTargetMoveParam->startPosition = GetOwner()->GetComponent<Transform>()->GetPosition();	//�J�n�ʒu�ݒ�
			toTargetMoveParam->endPosition = targetManager->GetTargetPosition();						//�I���ʒu�ݒ�
		}

		void TargetSeek::UpdateSeekPosition() {
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return;
			}

			auto& toTargetMoveParam = m_paramPtr->toTargetMoveParam;
			toTargetMoveParam->endPosition = targetManager->GetTargetPosition();	//�I���ʒu�ݒ�
		}

		void TargetSeek::DefineTask() {
			auto owner = GetOwner();

			m_taskList->DefineTask(
				TaskEnum::Move,
				std::make_shared<Task::ToTargetMove>(owner, m_paramPtr->toTargetMoveParam)
			);
		}

		void TargetSeek::SelectTask() {
			TaskEnum tasks[] = {
				TaskEnum::Move,
			};

			for (auto& task : tasks) {
				m_taskList->AddTask(task);
			}
		}

		bool TargetSeek::IsEnd() const {
			//�^�[�Q�b�g�����݂��Ȃ��Ȃ珈�����΂��B
			auto targetManager = m_targetManager.lock();
			if (!targetManager || !targetManager->HasTarget()) {
				return true;
			}

			//��Q���ɓ���������B
			auto objects = GetOwner()->GetStage()->GetGameObjectVec();
			if(maru::UtilityObstacle::IsRayObstacle(m_transform.lock()->GetPosition(), m_targetManager.lock()->GetTargetPosition(), objects)) {
				return true;
			}

			return m_taskList->IsEnd();
		}
	}
}

