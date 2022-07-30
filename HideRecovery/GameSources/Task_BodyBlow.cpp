
/*!
@file Task_BodyBlow.cpp
@brief Task_BodyBlow�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EnemyBase.h"
#include "TargetManager.h"
#include "I_Damaged.h"

#include "Task_BodyBlow.h"

#include "VelocityManager.h"
#include "RotationController.h"
#include "TargetManager.h"

#include "UtilityVelocity.h"

#include "CollisionAction.h"

#include "MaruAnimationHelper.h"
#include "StateEdgeBase.h"
#include "StateNodeBase.h"
#include "AnimationMainStateMachine.h"
#include "AnimatorBase.h"
#include "Animator_NormalSlime.h"

#include "EnemyStatusBase.h"

namespace basecross {
	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		Task_BodyBlow_Parametor::Task_BodyBlow_Parametor()
			:Task_BodyBlow_Parametor(15.0f, 2.0f)
		{}

		Task_BodyBlow_Parametor::Task_BodyBlow_Parametor(const float& value) :
			Task_BodyBlow_Parametor(value, value, value, DamageData(value), value)
		{}

		Task_BodyBlow_Parametor::Task_BodyBlow_Parametor(const float& speed, const float& range)
			: Task_BodyBlow_Parametor(speed, range, DamageData())
		{}

		Task_BodyBlow_Parametor::Task_BodyBlow_Parametor(const float& speed, const float& range, const DamageData& damageData)
			: Task_BodyBlow_Parametor(speed, range, 0.5f, damageData, 20.0f)
		{}

		Task_BodyBlow_Parametor::Task_BodyBlow_Parametor(const float& speed, const float& range, const float& endVelocitySpeed, const DamageData& damageData, const float& toughness):
			speed(speed), range(range), endVelocitySpeed(endVelocitySpeed), 
			damageData(std::make_unique<DamageData>(damageData)), toughness(toughness),
			elapsedRange(0.0f)
		{}

		bool Task_BodyBlow_Parametor::IsMoveEnd() { //�ړ��������ړ������������ǂ���
			return range <= elapsedRange;
		}

		//--------------------------------------------------------------------------------------
		/// �{�f�B�\�u���[�^�X�N�{��
		//--------------------------------------------------------------------------------------

		Task_BodyBlow::Task_BodyBlow(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr)
			:TaskNodeBase<EnemyBase>(owner), m_paramPtr(paramPtr), m_isActive(false)
		{
			auto collisionAction = owner->GetGameObject()->GetComponent<maru::CollisionAction>(false);
			if (collisionAction) {
				collisionAction->AddCollisionEnter([&, this](const CollisionPair& pair) { CollisionEnter(pair); });
			}
		}

		void Task_BodyBlow::OnStart() {
			m_paramPtr->elapsedRange = 0.0f;
			m_isActive = true;

			auto ownerObject = GetOwner()->GetGameObject();
			auto animator = ownerObject->GetComponent<NormalSlime::Animator>(false);
			if (animator) {
				animator->ChangeState(NormalSlime::Animator::StateType::BodyBlow);
			}

			auto statusManager = ownerObject->GetComponent<EnemyStatusManagerBase>(false);
			if (statusManager) {
				auto& status = statusManager->GetRefStatus();
				status.toughness = m_paramPtr->toughness;
			}
		}

		bool Task_BodyBlow::OnUpdate() {
			if (!m_paramPtr->IsMoveEnd()) { //Move���I�����Ă��Ȃ��Ȃ�
				MoveUpdate();
				RotationUpdate();
				ElapsedMoveRangeUpdate();
			}

			return IsEnd();
		}

		void Task_BodyBlow::OnExit() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto rotationController = ownerObject->GetComponent<RotationController>(false);
			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);

			//��]����
			if (rotationController) {
				rotationController->SetDirect(ownerObject->GetComponent<Transform>()->GetForward());
			}

			//�A�j���[�^
			auto animator = GetOwner()->GetGameObject()->GetComponent<NormalSlime::Animator>(false);
			if (animator) {
				animator->ChangeState(NormalSlime::Animator::StateType::Idle);
			}

			//�X�e�[�^�X�̃^�t�l�X�����ɖ߂��B
			auto statusManager = ownerObject->GetComponent<EnemyStatusManagerBase>(false);
			if (statusManager) {
				auto& status = statusManager->GetRefStatus();
				status.toughness = status.baseToughness;
			}

			m_paramPtr->elapsedRange = m_paramPtr->range; //�I����Ԃɂ���B
			m_isActive = false;
		}

		void Task_BodyBlow::MoveUpdate() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto targetManager = ownerObject->GetComponent<TargetManager>(false);
			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
			//�^�[�Q�b�g�}�l�[�W���\�����݂��Ȃ��A���́A�^�[�Q�b�g�����݂��Ȃ��A���́AvelocityManager�����݂��Ȃ��B
			if (targetManager == nullptr || !targetManager->HasTarget() || velocityManager == nullptr) {
				return;
			}

			auto direct = targetManager->CalcuToTargetVec();
			auto force = UtilityVelocity::CalucSeekVec(velocityManager->GetVelocity(), direct, m_paramPtr->speed);
			
			velocityManager->AddForce(force);
		}

		void Task_BodyBlow::RotationUpdate() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto rotationController = ownerObject->GetComponent<RotationController>(false);
			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);

			if (rotationController && velocityManager) {
				if (velocityManager->GetVelocity() == Vec3(0.0f)) {
					return;
				}

				rotationController->SetDirect(velocityManager->GetVelocity());
			}
		}

		void Task_BodyBlow::ElapsedMoveRangeUpdate() {
			auto ownerObject = GetOwner()->GetGameObject();
			auto ownerTrans = ownerObject->GetComponent<Transform>();
			auto moveRange = (ownerTrans->GetPosition() - ownerTrans->GetBeforePosition()).length();
			m_paramPtr->elapsedRange += moveRange;

			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
			if (m_paramPtr->IsMoveEnd()) {
				velocityManager->StartDeseleration();
			}
		}

		void Task_BodyBlow::CollisionEnter(const CollisionPair& pair) {
			if (m_isActive) { //�_���[�W��^�����ԂȂ�B
				SendDamage(pair);
			}
		}

		void Task_BodyBlow::SendDamage(const CollisionPair& pair) {
			auto other = pair.m_Dest.lock();

			auto damaged = other->GetGameObject()->GetComponent<I_Damaged>(false);
			if (damaged) {
				auto damageData = *m_paramPtr->damageData.get();
				damageData.hitPoint = pair.m_CalcHitPoint;
				damaged->Damaged(damageData);
			}
		}

		bool Task_BodyBlow::IsEnd() {
			//���������S�Ɋ���������B
			auto ownerObject = GetOwner()->GetGameObject();
			auto velocityManager = ownerObject->GetComponent<VelocityManager>(false);
			if (!velocityManager) {
				DebugObject::AddString(L"Task_BodyBlow::IsEnd()�AVelocityManager�����݂��܂���B");
				return true;
			}

			//�������āA�w��̋����𑖂�I������I��
			return (velocityManager->GetVelocity().length() < m_paramPtr->endVelocitySpeed) && (m_paramPtr->IsMoveEnd());
		}
	}
}