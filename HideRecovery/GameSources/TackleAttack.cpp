
/*!
@file TackleAttack.cpp
@brief TackleAttackクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "TackleAttack.h"

#include "TaskList.h"

#include "Watanabe/Component/PlayerAnimator.h"

#include "PlayerInputer.h"
#include "VelocityManager.h"

#include "Itabashi/ObjectMover.h"
#include "Itabashi/PlayerOnlineController.h"

namespace basecross {

	TackleAttack::TackleAttack(const std::shared_ptr<GameObject>& objPtr) :
		AttackBase(objPtr),
		m_taskList(new TaskList<TaskEnum>())
	{}

	void TackleAttack::OnCreate() {
		DefineTask();
	}

	void TackleAttack::OnUpdate() {
		if (PlayerInputer::GetInstance()->IsYDown()) {
			SelectTask();
		}

		m_taskList->UpdateTask();
	}

	void TackleAttack::DefineTask() {
		auto object = GetGameObject();

		//予備動作
		m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<Task::Preriminary_Tackle>(object));

		//攻撃
		m_taskList->DefineTask(TaskEnum::Tackle, std::make_shared<Task::Attack_Tackle>(object));

		//攻撃後硬直
		m_taskList->DefineTask(TaskEnum::EndWait, std::make_shared<Task::EndWait_Tackle>(object));
	}

	void TackleAttack::SelectTask() {
		TaskEnum tasks[] = {
			TaskEnum::Preliminary,
			TaskEnum::Tackle,
			TaskEnum::EndWait,
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
			m_animator = GetOwner()->GetComponent<PlayerAnimator>(false);
			PlayAnimation();	//アニメーションの再生
		}

		bool Preriminary_Tackle::OnUpdate() {
			auto animator = m_animator.lock();
			if (!animator) {	//アニメーターが存在しないなら、処理を終了させる。
				return true;
			}

			//アニメータがDAStartでないなら、処理を終了させる。
			if (!animator->IsCurretAnimationState(PlayerAnimationState::State::DAStart)) {
				return true;
			}

			return animator->IsTargetAnimationEnd();	//アニメーションが終了しているならtrue
		}

		void Preriminary_Tackle::OnExit() {

		}

		void Preriminary_Tackle::PlayAnimation() {
			if (auto animator = m_animator.lock()) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::DAStart);
			}
		}

		//--------------------------------------------------------------------------------------
		/// タックル攻撃の攻撃中動作
		//--------------------------------------------------------------------------------------

		Attack_Tackle::Attack_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			TaskNodeBase(objPtr)
		{
			
		}

		void Attack_Tackle::OnStart() {
			auto delta = App::GetApp()->GetElapsedTime();

			m_animator = GetOwner()->GetComponent<PlayerAnimator>(false);

			if (auto playerController = GetOwner()->GetComponent<Online::PlayerOnlineController>(false)) {
				playerController->SetUpdateActive(false);
			}

			//速度を加算
			if (auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false)) {
				auto transform = GetOwner()->GetComponent<Transform>();
				velocityManager->AddForce(transform->GetForward() * 100.0f);
				velocityManager->StartDeseleration();
				m_velocityManager = velocityManager;
			}

			PlayAnimation();
		}

		bool Attack_Tackle::OnUpdate() {
			//auto velocityManager = m_velocityManager.lock();
			//if (!velocityManager) {
			//	return true;
			//}

			//return !velocityManager->IsDeseleration();	//減速処理が完了したら切り替える。

			auto animator = m_animator.lock();
			if (!animator) {	//アニメーターが存在しないなら、処理を終了させる。
				return true;
			}

			//アニメータがDAStartでないなら、処理を終了させる。
			if (!animator->IsCurretAnimationState(PlayerAnimationState::State::DAttack)) {
				return true;
			}

			return animator->IsTargetAnimationEnd();	//アニメーションが終了しているならtrue
		}

		void Attack_Tackle::OnExit() {
			if (auto velocityManager = m_velocityManager.lock()) {
				velocityManager->SetIsDeseleration(false);
			}

			if (auto playerController = GetOwner()->GetComponent<Online::PlayerOnlineController>(false)) {
				playerController->SetUpdateActive(true);
			}
		}

		void Attack_Tackle::PlayAnimation() {
			if (auto animator = m_animator.lock()) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::DAttack);
			}
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