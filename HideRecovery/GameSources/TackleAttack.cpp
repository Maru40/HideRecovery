
/*!
@file TackleAttack.cpp
@brief TackleAttack�N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TackleAttack.h"

#include "TaskList.h"

namespace basecross {

	TackleAttack::TackleAttack(const std::shared_ptr<GameObject>& objPtr) :
		AttackBase(objPtr),
		m_taskList(new TaskList<TaskEnum>())
	{}

	void TackleAttack::OnCreate() {
		DefineTask();
	}

	void TackleAttack::OnUpdate() {
		m_taskList->UpdateTask();
	}

	void TackleAttack::DefineTask() {
		//�\������
		m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<Task::Preriminary_Tackle>());

		//�U��
		m_taskList->DefineTask(TaskEnum::Tackle, std::make_shared<Task::Attack_Tackle>());

		//�U����d��
		m_taskList->DefineTask(TaskEnum::EndWait, std::make_shared<Task::EndWait_Tackle>());
	}

	void TackleAttack::SelectTask() {
		TaskEnum tasks[] = {
			TaskEnum::Preliminary,
			TaskEnum::Tackle,
			TaskEnum::EndWait
		};

		for (const auto& task : tasks) {
			m_taskList->AddTask(task);
		}
	}

	void TackleAttack::StartAttack() {
		if (!m_taskList->IsEnd()) {	//�^�X�N���I�����Ă��Ȃ��Ȃ珈�����΂��B
			return;
		}

		SelectTask();
	}

	//--------------------------------------------------------------------------------------
	/// �^�b�N���U���̃^�X�N
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̗\������
		//--------------------------------------------------------------------------------------

		Preriminary_Tackle::Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			TaskNodeBase(objPtr)
		{}

		void Preriminary_Tackle::OnStart() {
			//�A�j���[�V�����̍Đ�

		}

		bool Preriminary_Tackle::OnUpdate() {
			return true;
		}

		void Preriminary_Tackle::OnExit() {

		}

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U��������
		//--------------------------------------------------------------------------------------

		Attack_Tackle::Attack_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			TaskNodeBase(objPtr)
		{}

		void Attack_Tackle::OnStart() {

		}

		bool Attack_Tackle::OnUpdate() {
			return true;
		}

		void Attack_Tackle::OnExit() {

		}

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U����d��
		//--------------------------------------------------------------------------------------

		EndWait_Tackle::EndWait_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			TaskNodeBase(objPtr)
		{}

		void EndWait_Tackle::OnStart() {

		}

		bool EndWait_Tackle::OnUpdate() {
			return true;
		}

		void EndWait_Tackle::OnExit() {

		}
	}
}