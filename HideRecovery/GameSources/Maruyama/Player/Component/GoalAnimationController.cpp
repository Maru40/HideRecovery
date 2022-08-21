
/*!
@file GoalAnimationController.cpp
@brief GoalAnimationControllerクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "GoalAnimationController.h"

#include "TaskList.h"

#include "TimeHelper.h"
#include "GameTimer.h"
#include "Task_Wait.h"

#include "Task_ToTargetMove.h"

#include "Watanabe/Component/PlayerAnimator.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ゴール中のアニメーションコントローラーのパラメータ
	//--------------------------------------------------------------------------------------

	GoalAnimationController_Parametor::GoalAnimationController_Parametor() :
		startPosition(Vec3(0)),
		dunkPosition(Vec3(0)),
		jumpPower(Vec3(0.0f, 1.0f, 0.0f)),
		preliminaryJumpParam(std::make_shared<Task::Wait::Parametor>(0.0f))
	{}

	//--------------------------------------------------------------------------------------
	/// ゴール中のアニメーションコントローラー本体
	//--------------------------------------------------------------------------------------

	GoalAnimationController::GoalAnimationController(const std::shared_ptr<GameObject>& objPtr) :
		Component(objPtr),
		m_taskList(new TaskList<TaskEnum>())
	{}

	void GoalAnimationController::OnLateStart() {
		auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false);
		if (!animator) {	//animatorがなかったら処理を終了
			return;
		}

		//ゴール時のアニメーションイベントを登録
		animator->AddAnimationEvent(
			PlayerAnimationState::State::Goal1,
			[&]() { StartAnimationEvent(); },
			[&]() { return UpdateAnimationEvent(); },
			[&]() { ExitAnimationEvent(); }
		);
	}

	void GoalAnimationController::StartAnimationEvent() {
		m_param.startPosition = transform->GetPosition();

		SelectTask();
	}

	bool GoalAnimationController::UpdateAnimationEvent() {
		m_taskList->UpdateTask();
		return m_taskList->IsEnd();
	}

	void GoalAnimationController::ExitAnimationEvent() {
		m_taskList->ForceStop();
	}

	void GoalAnimationController::DefineTask() {
		//ジャンプの溜め
		m_taskList->DefineTask(TaskEnum::PreliminaryJump, std::make_shared<Task::Wait>(m_param.preliminaryJumpParam));

		//ダンク位置まで移動
		//m_taskList->DefineTask(TaskEnum::DunkMove, std::make_shared<Task_ToTargetMove>());

		//ダンク後の待機

		//元の位置に戻る移動

		//終了時待機
	}

	void GoalAnimationController::SelectTask() {
		TaskEnum tasks[] = {
			TaskEnum::PreliminaryJump,
			TaskEnum::DunkMove,
			TaskEnum::DunkWait,
			TaskEnum::ReturnMove,
			TaskEnum::EndWait
		};

		for (auto& task : tasks) {
			m_taskList->AddTask(task);
		}
	}
}