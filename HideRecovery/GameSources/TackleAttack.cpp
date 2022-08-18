
/*!
@file TackleAttack.cpp
@brief TackleAttack�N���X����
�S���F�ێR�T��
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

#include "TimeHelper.h"
#include "GameTimer.h"

#include "CollisionAction.h"
#include "Watanabe/Component/PlayerStatus.h"
#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	TackleAttack::TackleAttack(const std::shared_ptr<GameObject>& objPtr) :
		AttackBase(objPtr),
		m_taskList(new TaskList<TaskEnum>())
	{}

	void TackleAttack::OnCreate() {
		DefineTask();
	}

	void TackleAttack::OnLateStart() {
		m_status = GetGameObject()->GetComponent<PlayerStatus>(false);
	}

	void TackleAttack::OnUpdate() {
		auto status = m_status.lock();
		if (status && status->IsDead()) {
			m_taskList->ForceStop();
			return;
		}

		m_taskList->UpdateTask();
	}

	void TackleAttack::DefineTask() {
		auto object = GetGameObject();

		//�\������
		m_taskList->DefineTask(TaskEnum::Preliminary, std::make_shared<Task::Preriminary_Tackle>(object));

		//�U��
		m_taskList->DefineTask(TaskEnum::Tackle, std::make_shared<Task::Attack_Tackle>(object));

		//�U����d��
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
		if (IsTackle()) {	//�^�b�N�����Ȃ珈�����΂��B
			return;
		}

		SelectTask();
	}

	void TackleAttack::ForceStartAttack() {
		m_taskList->ForceStop();	//�^�X�N�����I��

		//������������
		if (auto velocityManager = GetGameObject()->GetComponent<VelocityManager>(false)) {
			velocityManager->SetIsDeseleration(false);
		}

		SelectTask();
	}

	bool TackleAttack::IsTackle() {
		return !m_taskList->IsEnd();
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
		{ }

		void Preriminary_Tackle::OnStart() {
			m_animator = GetOwner()->GetComponent<PlayerAnimator>(false);
			PlayAnimation();	//�A�j���[�V�����̍Đ�
		}

		bool Preriminary_Tackle::OnUpdate() {
			auto animator = m_animator.lock();
			if (!animator) {	//�A�j���[�^�[�����݂��Ȃ��Ȃ�A�������I��������B
				return true;
			}

			//�A�j���[�^��DAStart�łȂ��Ȃ�A�������I��������B
			if (!animator->IsCurretAnimationState(PlayerAnimationState::State::DAStart)) {
				return true;
			}

			return animator->IsTargetAnimationEnd();	//�A�j���[�V�������I�����Ă���Ȃ�true
		}

		void Preriminary_Tackle::OnExit() {

		}

		void Preriminary_Tackle::PlayAnimation() {
			if (auto animator = m_animator.lock()) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::DAStart);
			}
		}

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U��������̃p�����[�^
		//--------------------------------------------------------------------------------------

		Attack_Tackle_Parametor::Attack_Tackle_Parametor(const DamageData& damageData):
			damageData(damageData),
			tackleSpeed(500.0f)
		{}

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U��������
		//--------------------------------------------------------------------------------------

		Attack_Tackle::Attack_Tackle(const std::shared_ptr<GameObject>& objPtr) :
			Attack_Tackle(objPtr, Parametor(DamageData(1, objPtr)))
		{}

		Attack_Tackle::Attack_Tackle(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
			TaskNodeBase(objPtr),
			m_param(parametor),
			m_isCollision(false)
		{
			//�����蔻��ݒ�
			if (auto collisionAction = GetOwner()->GetComponent<maru::CollisionAction>(false)) {
				collisionAction->AddCollisionEnter([&](const CollisionPair& pair) { CollisionEnter(pair); });
			}
		}

		void Attack_Tackle::OnStart() {
			auto delta = App::GetApp()->GetElapsedTime();

			m_isCollision = true;
			m_animator = GetOwner()->GetComponent<PlayerAnimator>(false);

			//���x�����Z
			if (auto velocityManager = GetOwner()->GetComponent<VelocityManager>(false)) {
				auto transform = GetOwner()->GetComponent<Transform>();
				velocityManager->AddForce(transform->GetForward() * m_param.tackleSpeed);
				velocityManager->StartDeseleration();
				m_velocityManager = velocityManager;
			}

			PlayAnimation();
		}

		bool Attack_Tackle::OnUpdate() {
			auto animator = m_animator.lock();
			if (!animator) {	//�A�j���[�^�[�����݂��Ȃ��Ȃ�A�������I��������B
				return true;
			}

			//�A�j���[�^��DAStart�łȂ��Ȃ�A�������I��������B
			if (!animator->IsCurretAnimationState(PlayerAnimationState::State::DAttack)) {
				return true;
			}

			return animator->IsTargetAnimationEnd();	//�A�j���[�V�������I�����Ă���Ȃ�true
		}

		void Attack_Tackle::OnExit() {
			m_isCollision = false;
		}

		void Attack_Tackle::PlayAnimation() {
			if (auto animator = m_animator.lock()) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::DAttack);
			}
		}

		void Attack_Tackle::CollisionEnter(const CollisionPair& pair) {
			if (!m_isCollision) {
				return;
			}

			auto other = pair.m_Dest.lock()->GetGameObject();
			auto playerStatus = other->GetComponent<PlayerStatus>(false);
			if (playerStatus) {
				playerStatus->AddDamage(m_param.damageData);
				Debug::GetInstance()->Log(L"TackleDamage");
			}
		}

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U����d��
		//--------------------------------------------------------------------------------------

		EndWait_Tackle::EndWait_Tackle(const std::shared_ptr<GameObject>& objPtr, const float waitTime) :
			TaskNodeBase(objPtr),
			m_waitTime(waitTime),
			m_timer(new GameTimer(0))
		{}

		void EndWait_Tackle::OnStart() {
			m_velocityManager = GetOwner()->GetComponent<VelocityManager>(false);

			m_timer->ResetTimer(m_waitTime);
		}

		bool EndWait_Tackle::OnUpdate() {
			m_timer->UpdateTimer();

			return m_timer->IsTimeUp();
		}

		void EndWait_Tackle::OnExit() {
			//������������
			if (auto velocityManager = m_velocityManager.lock()) {
				velocityManager->SetIsDeseleration(false);
			}

			//�A�j���[�V�����̕ύX
			if (auto animator = GetOwner()->GetComponent<PlayerAnimator>(false)) {
				animator->ChangePlayerAnimation(PlayerAnimationState::State::Wait);
			}
		}
	}
}