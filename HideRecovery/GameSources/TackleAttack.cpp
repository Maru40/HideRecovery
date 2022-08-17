
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
		//予備動作
		m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<Task::Preriminary_Tackle>());

		//攻撃
		m_taskList->DefineTask(TaskEnum::Tackle, std::make_shared<Task::Attack_Tackle>());

		//攻撃後硬直
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
		if (!m_taskList->IsEnd()) {	//タスクが終了していないなら処理を飛ばす。
			return;
		}

		SelectTask();
	}

	//--------------------------------------------------------------------------------------
	/// タックル攻撃のタスク
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// タックル攻撃の予備動作
		//--------------------------------------------------------------------------------------

		Preriminary_Tackle::Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			TaskNodeBase(objPtr)
		{}

		void Preriminary_Tackle::OnStart() {
			//アニメーションの再生

		}

		bool Preriminary_Tackle::OnUpdate() {
			return true;
		}

		void Preriminary_Tackle::OnExit() {

		}

		//--------------------------------------------------------------------------------------
		/// タックル攻撃の攻撃中動作
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
		/// タックル攻撃の攻撃後硬直
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