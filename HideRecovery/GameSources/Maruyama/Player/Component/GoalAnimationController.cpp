
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

#include "Task_GoalAnimation.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "VelocityManager.h"
#include "RotationController.h"
#include "Watanabe/Component/BallAnimator.h"

#include "HideItemObject.h"
#include "PlayerInputer.h"
#include "Maruyama/StageObject/Goal.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ゴール中のアニメーションコントローラーのパラメータ
	//--------------------------------------------------------------------------------------

	GoalAnimationController_Parametor::GoalAnimationController_Parametor() :
		startPosition(Vec3(0)),
		dunkPosition(Vec3(0)),
		dunkBallPosition(Vec3(0)),
		jumpDegree(30.0f),
		dunkMoveSpeed(3.0f),

		preliminaryJumpParam(std::make_shared<Task::Wait::Parametor>(0.0f)),
		dunkMoveParam(std::make_shared<Task::ToTargetMove::Parametor>(3.0f, 0.1f, Task::ToTargetMove_MoveType::Lerp)),
		dunkAfterWaitParam(std::make_shared<Task::Wait::Parametor>(0.65f)),
		returnJumpParam(std::make_shared<Task::ReturnJump_Parametor>()),
		endWaitParam(std::make_shared<Task::Wait::Parametor>(0.5f))
	{}

	float GoalAnimationController_Parametor::GetJumpRad() const { return XMConvertToRadians(jumpDegree); }

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

		DefineTask();

		//ゴール時のアニメーションイベントを登録
		animator->AddAnimationEvent(
			PlayerAnimationState::State::Goal1,
			[&]() { StartAnimationEvent(); },
			[&]() { return UpdateAnimationEvent(); },
			[&]() { ExitAnimationEvent(); }
		);

		auto timeEvent = [&]() {
			if (!m_ball.lock()) {
				return;
			}

			m_ball.lock()->SetParent(nullptr);
			m_ball.lock()->GetComponent<Transform>()->SetPosition(GetDunkBallPosition());

			if (auto goal = m_goal.lock()) {
				goal->PlayFireEffects();
			}
		};
		animator->AddTimeEvent(
			PlayerAnimationState::State::Goal1,
			0.9f,
			timeEvent
		);
	}

	void GoalAnimationController::OnUpdate() {
		//if (PlayerInputer::GetInstance()->IsRightDown()) {
		//	GetGameObject()->GetComponent<PlayerAnimator>()->ChangePlayerAnimation(PlayerAnimationState::State::Goal1);
		//}
	}

	void GoalAnimationController::StartAnimationEvent() {
		m_param.startPosition = transform->GetPosition();	//開始位置の設定

		m_param.dunkMoveParam->startPosition = m_param.startPosition;
		m_param.dunkMoveParam->endPosition = m_param.dunkPosition;

		m_param.returnJumpParam->returnDirect = transform->GetPosition() - m_param.dunkPosition;

		if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {							//重力の解除
			gravity->SetUpdateActive(false);
		}

		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {			//速度停止
			velocityManager->ResetAll();
		}

		if (auto rotationController = GetGameObject()->GetComponent<RotationController>(false)) {	//回転コントローラー
			rotationController->SetDirect(m_param.dunkPosition - transform->GetPosition());
		}

		m_taskList->ForceStop();
		SelectTask();
	}

	bool GoalAnimationController::UpdateAnimationEvent() {
		m_taskList->UpdateTask();
		return m_taskList->IsEnd();
	}

	void GoalAnimationController::ExitAnimationEvent() {
		if (auto gravity = GetGameObject()->GetComponent<Gravity>(false)) {	//重力の解除
			gravity->SetUpdateActive(true);
		}
	}

	void GoalAnimationController::DefineTask() {
		auto object = GetGameObject();

		//ジャンプの溜め
		m_param.preliminaryJumpParam->start = [&]() { //開始時イベント
			const auto Offset = Vec3(0.0f, 0.25f, 0.0f);
			auto hideItem = GetStage()->Instantiate<HideItemObject>(Offset, Quat::Identity(), GetGameObject());
			m_ball = hideItem;

			auto animator = hideItem->GetComponent<BallAnimator>(false);
			if (animator) {
				animator->ChangeBallAnimation(BallAnimationState::State::Goal);
			}
		};	
		m_taskList->DefineTask(TaskEnum::PreliminaryJump, std::make_shared<Task::Wait>(m_param.preliminaryJumpParam));

		//ダンク位置まで移動
		auto& dunkMoveParam = m_param.dunkMoveParam;
		m_taskList->DefineTask(TaskEnum::DunkMove, std::make_shared<Task::ToTargetMove>(object, m_param.dunkMoveParam));

		//ダンク後の待機
		m_param.dunkAfterWaitParam->start = [&]() {

		};

		m_param.dunkAfterWaitParam->exit = [&]() {
			
		};
		m_taskList->DefineTask(TaskEnum::DunkWait, std::make_shared<Task::Wait>(m_param.dunkAfterWaitParam));

		//元の位置に戻る移動
		m_taskList->DefineTask(TaskEnum::ReturnMove, std::make_shared<Task::ReturnJump>(GetGameObject(), m_param.returnJumpParam));

		//終了時待機
		m_param.endWaitParam->exit = [&]() {
			if (auto animator = GetGameObject()->GetComponent<PlayerAnimator>(false)) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
			}

			auto ball = m_ball.lock();
			if (ball) {
				GetStage()->RemoveGameObject<GameObject>(ball);
			}
		};
		m_taskList->DefineTask(TaskEnum::EndWait, std::make_shared<Task::Wait>(m_param.endWaitParam));
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