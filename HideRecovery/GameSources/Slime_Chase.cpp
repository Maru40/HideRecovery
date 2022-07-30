
/*!
@file Slime_Chase.cpp
@brief Slime_Chase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TaskList.h"
#include "EnemyBase.h"
#include "TargetManager.h"
#include "SeekTarget.h"
#include "EnemyStateNodeBase.h"
#include "TaskList.h"
#include "ChaseState_TargetSeek.h"

#include "Task_MovePositions.h"
#include "Task_AroundEyeCheck.h"
#include "ChaseState_AstarSeek.h"

#include "EnemyMainStateMachine.h"
#include "Slime_Chase.h"

#include "GameStageBase.h"
#include "RotationController.h"
#include "UtilityObstacle.h"
#include "EyeSearchRange.h"

#include "DebugObject.h"

namespace basecross {
	using MoveType = Task_ToTargetMove::MoveType;
	using DeltaType = Task_ToTargetMove::DeltaType;

namespace Enemy {

	namespace StateNode {

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		Slime_Chase::Parametor::Parametor() :
			targetSeekParamPtr(new ChaseState::TargetSeekParametor(
				std::make_shared<SeekTarget::Parametor>(5.0f, 0.0f, Vec3(0.0f), SeekTarget::SeekType::VelocitySeek))),
			astarSeekParamPtr(new ChaseState::AstarSeekParametor(
				std::make_shared<Task_MovePositions_Parametor>(std::make_shared<Task_ToTargetMove::Parametor>(
					5.0f, 1.0f, MoveType::SeekVelocity, DeltaType::Normal)),
				std::make_shared<Task::AroundEyeCheck_Parametor>())
			)
		{}

		Slime_Chase::Slime_Chase(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
			EnemyStateNodeBase<EnemyBase>(owner), 
			m_paramPtr(paramPtr), 
			m_stateMachine(new StateMachine())
		{
			CreateNode();
			CreateEdge();
		}

		//�J�ڏ���----------------------------------------------------------------------------------------------------

		bool Slime_Chase::ToChaseMove(const TransitionMember& member) {
			auto ownerObject = GetOwner()->GetGameObject();
			auto eye = ownerObject->GetComponent<EyeSearchRange>(false);
			auto targetManager = ownerObject->GetComponent<TargetManager>(false);
			if (!eye || !targetManager || !targetManager->HasTarget()) {
				return false;
			}

			//���E�̒��ɓ�������Chase�ɕύX
			return eye->IsInEyeRange(targetManager->GetTarget());
		}

		bool Slime_Chase::ToAstarMove(const TransitionMember& member) {
			auto ownerObject = GetOwner()->GetGameObject();
			auto targetManager = ownerObject->GetComponent<TargetManager>(false);
			if (!targetManager || !targetManager->HasTarget()) {
				return false;
			}

			auto startPosition = ownerObject->GetComponent<Transform>()->GetPosition();
			auto endPosition = targetManager->GetTargetPosition();
			auto objects = GetOwner()->GetStage()->GetGameObjectVec();
			//��Q������������AstarChase�ɕύX
			return maru::UtilityObstacle::IsRayObstacle(startPosition, endPosition, objects);
		}

		//------------------------------------------------------------------------------------------------------------

		void Slime_Chase::OnStart() {
			m_stateMachine->ChangeState(StateType::TargetSeek, (int)StateType::TargetSeek);
		}

		bool Slime_Chase::OnUpdate() {
			m_stateMachine->OnUpdate();

			return false;
		}

		void Slime_Chase::OnExit() {
			m_stateMachine->ForceChangeState(StateType::Null);
		}

		void Slime_Chase::CreateNode() {
			auto enemy = GetOwner();

			//Null
			m_stateMachine->AddNode(StateType::Null, nullptr);

			//�^�[�Q�b�g�V�[�N
			m_stateMachine->AddNode(StateType::TargetSeek, std::make_shared<ChaseState::ChaseState_TargetSeek>(enemy, m_paramPtr->targetSeekParamPtr));

			//AstarSeek
			m_stateMachine->AddNode(StateType::AstarSeek, std::make_shared<ChaseState::ChaseState_AstarSeek>(enemy, m_paramPtr->astarSeekParamPtr));
		}

		void Slime_Chase::CreateEdge() {
			//�^�[�Q�b�g�ɒǏ]�ړ�
			m_stateMachine->AddEdge(StateType::TargetSeek, StateType::AstarSeek, [&, this](const TransitionMember& member) { return ToAstarMove(member); });

			//Astar�ړ�
			m_stateMachine->AddEdge(StateType::AstarSeek, StateType::TargetSeek, [&, this](const TransitionMember& member) { return ToChaseMove(member); });
		}
	}
}
}