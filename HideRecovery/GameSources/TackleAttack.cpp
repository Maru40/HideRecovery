
/*!
@file TackleAttack.cpp
@brief TackleAttackクラス実体
担当：丸山裕喜
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
		//m_taskList->DefineTask(TaskEnum::Preliminary, );
	}

	void TackleAttack::SelectTask() {

	}


	namespace Task {

		Preriminary_Tackle::Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			TaskNodeBase(objPtr)
		{}

		void Preriminary_Tackle::OnStart() {

		}

		bool Preriminary_Tackle::OnUpdate() {
			return true;
		}

		void Preriminary_Tackle::OnExit() {

		}
	}
}