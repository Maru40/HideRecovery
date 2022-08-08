
/*!
@file EnemyStateNode_Chase.cpp
@brief EnemyStateNode_Chaseのクラス実体
担当：丸山裕喜
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
#include "EnemyStateNode_Chase.h"

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
		/// パラメータ
		//--------------------------------------------------------------------------------------

		Chase::Parametor::Parametor() :
			targetSeekParamPtr(new ChaseState::TargetSeekParametor(
				std::make_shared<SeekTarget::Parametor>(5.0f, 0.0f, Vec3(0.0f), SeekTarget::SeekType::VelocitySeek))),
			astarSeekParamPtr(new ChaseState::AstarSeekParametor(
				std::make_shared<Task_MovePositions_Parametor>(std::make_shared<Task_ToTargetMove::Parametor>(
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

		//遷移条件----------------------------------------------------------------------------------------------------

		bool Chase::ToChaseMove(const TransitionMember& member) {
			auto ownerObject = GetOwner()->GetGameObject();
			auto eye = ownerObject->GetComponent<EyeSearchRange>(false);
			auto targetManager = ownerObject->GetComponent<TargetManager>(false);
			if (!eye || !targetManager || !targetManager->HasTarget()) {
				return false;
			}

			//視界の中に入ったらChaseに変更
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
			//障害物が合ったらAstarChaseに変更
			return maru::UtilityObstacle::IsRayObstacle(startPosition, endPosition, objects);
		}

		//------------------------------------------------------------------------------------------------------------

		void Chase::OnStart() {
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

			//ターゲットシーク
			m_stateMachine->AddNode(StateType::TargetSeek, std::make_shared<ChaseState::ChaseState_TargetSeek>(enemy, m_paramPtr->targetSeekParamPtr));

			//AstarSeek
			m_stateMachine->AddNode(StateType::AstarSeek, std::make_shared<ChaseState::ChaseState_AstarSeek>(enemy, m_paramPtr->astarSeekParamPtr));
		}

		void Chase::CreateEdge() {
			//ターゲットに追従移動
			m_stateMachine->AddEdge(StateType::TargetSeek, StateType::AstarSeek, [&, this](const TransitionMember& member) { return ToAstarMove(member); });

			//Astar移動
			m_stateMachine->AddEdge(StateType::AstarSeek, StateType::TargetSeek, [&, this](const TransitionMember& member) { return ToChaseMove(member); });
		}
	}
}
}