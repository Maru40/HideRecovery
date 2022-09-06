
/*!
@file EnemyStateNode_Chase.cpp
@brief EnemyStateNode_Chase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/Utility/Component/SeekTarget.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "ChaseState_TargetSeek.h"

#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/TaskList/CommonTasks/Task_AroundEyeCheck.h"
#include "ChaseState_AstarSeek.h"

#include "Maruyama/Enemy/StateMachine/EnemyMainStateMachine.h"
#include "EnemyStateNode_Chase.h"

#include "GameStageBase.h"
#include "Maruyama/Utility/Component/RotationController.h"
#include "Maruyama/Utility/UtilityObstacle.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

#include "Maruyama/Interface/I_FactionMember.h"
#include "Maruyama/Enemy/AIDirector/FactionCoordinator.h"

namespace basecross {
	using MoveType = Task::ToTargetMove::MoveType;
	using DeltaType = Task::ToTargetMove::DeltaType;

namespace Enemy {

	namespace StateNode {

		//--------------------------------------------------------------------------------------
		/// �p�����[�^
		//--------------------------------------------------------------------------------------

		Chase::Parametor::Parametor() :
			targetSeekParamPtr(new ChaseState::TargetSeekParametor(
				std::make_shared<SeekTarget::Parametor>(5.0f, 0.0f, Vec3(0.0f), SeekTarget::SeekType::VelocitySeek))),
			astarSeekParamPtr(new ChaseState::AstarSeekParametor(
				std::make_shared<Task_MovePositions_Parametor>(std::make_shared<Task::ToTargetMove::Parametor>(
					5.0f, 1.0f, MoveType::SeekVelocity, DeltaType::Normal)),
				std::make_shared<Task::AroundEyeCheck_Parametor>())
			)
		{}

		Chase::Chase(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
			EnemyStateNodeBase<EnemyBase>(owner), 
			m_paramPtr(paramPtr), 
			m_stateMachine(new StateMachine())
		{
			CreateNode();
			CreateEdge();

			AddChangeComponent(owner->GetGameObject()->GetComponent<SeekTarget>(false), true, false);
		}

		//--------------------------------------------------------------------------------------
		/// �J�ڏ���
		//--------------------------------------------------------------------------------------

		bool Chase::ToChaseMove(const TransitionMember& member) {
			auto ownerObject = GetOwner()->GetGameObject();
			auto eye = ownerObject->GetComponent<EyeSearchRange>(false);
			auto targetManager = ownerObject->GetComponent<TargetManager>(false);
			if (!eye || !targetManager || !targetManager->HasTarget()) {
				return false;
			}

			//���E�̒��ɓ�������Chase�ɕύX
			return eye->IsInEyeRange(targetManager->GetTarget());
		}

		bool Chase::ToAstarMove(const TransitionMember& member) {
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

		//--------------------------------------------------------------------------------------
		/// �Ǐ]�X�e�[�g�{��
		//--------------------------------------------------------------------------------------

		void Chase::OnStart() {
			//�f�o�b�O�ŁA���������Ƃ�m�点��B
			auto object = GetOwner()->GetGameObject();
			auto factionMember = object->GetComponent<I_FactionMember>(false);
			auto targetManager = object->GetComponent<TargetManager>(false);
			if (factionMember && targetManager) {
				std::shared_ptr<FactionCoordinator> coordinator = factionMember->GetFactionCoordinator();
				//�R�[�f�B�l�[�^�[�ɓG�����������Ƃ�`����B
				coordinator->DebugWriteTarget(factionMember, targetManager->GetTarget());
			}

			StartChangeComponents();

			m_stateMachine->ChangeState(StateType::TargetSeek, (int)StateType::TargetSeek);
		}

		bool Chase::OnUpdate() {
			m_stateMachine->OnUpdate();

			return false;
		}

		void Chase::OnExit() {
			m_stateMachine->ForceChangeState(StateType::Null);

			ExitChangeComponents();
		}

		void Chase::CreateNode() {
			auto enemy = GetOwner();

			//Null
			m_stateMachine->AddNode(StateType::Null, nullptr);

			//�^�[�Q�b�g�V�[�N
			m_stateMachine->AddNode(StateType::TargetSeek, std::make_shared<ChaseState::ChaseState_TargetSeek>(enemy, m_paramPtr->targetSeekParamPtr));

			//AstarSeek
			m_stateMachine->AddNode(StateType::AstarSeek, std::make_shared<ChaseState::ChaseState_AstarSeek>(enemy, m_paramPtr->astarSeekParamPtr));
		}

		void Chase::CreateEdge() {
			//�^�[�Q�b�g�ɒǏ]�ړ�
			m_stateMachine->AddEdge(StateType::TargetSeek, StateType::AstarSeek, [&, this](const TransitionMember& member) { return ToAstarMove(member); });

			//Astar�ړ�
			m_stateMachine->AddEdge(StateType::AstarSeek, StateType::TargetSeek, [&, this](const TransitionMember& member) { return ToChaseMove(member); });
		}
	}
}
}