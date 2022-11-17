/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

//タスク
#include "Maruyama/Enemy/Behavior/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Task/NearAstarMove.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/Enemy/Behavior/Task/NearSeekMove.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"
#include "Maruyama/Enemy/Behavior/Task/ToBallRunTask.h"
#include "Maruyama/Enemy/Behavior/Task/ToGoalRunTask.h"
#include "Maruyama/Enemy/Behavior/Task/RelifMember.h"
#include "Maruyama/Enemy/Behavior/Task/ToHasBallOtherTeam.h"

#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

//デコレータ
#include "Maruyama/Enemy/Behavior/Decorator/OutSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/IsActiveSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/SettingStartTarget_Ball.h"
#include "Maruyama/Enemy/Behavior/Decorator/HasBallTransition.h"
#include "Maruyama/Enemy/Behavior/Decorator/HasBall_OtherMember.h"
#include "Maruyama/Enemy/Behavior/Decorator/HasBall_OtherTeam.h"
#include "Maruyama/Enemy/Behavior/Decorator/TimerWaitPosition.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Item/HideItemObject.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// 隠れ場所を探すパトロールビヘイビアツリーのパラメータ
				//--------------------------------------------------------------------------------------

				HidePlacePatrolTree_Parametor::HidePlacePatrolTree_Parametor()

				{}

				HidePlacePatrolTree_Parametor::~HidePlacePatrolTree_Parametor(){

				}

				//--------------------------------------------------------------------------------------
				/// 隠れ場所を探すパトロールビヘイビアツリー
				//--------------------------------------------------------------------------------------

				HidePlacePatrolTree::HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr) :
					SubBehaviorTreeBase(objPtr),
					m_param(Parametor())
				{}

				void HidePlacePatrolTree::CreateNode() {
					using namespace maru::Behavior;

					auto owner = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//初回セレクター
					m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

					//ボール探すタスク
					m_behaviorTree->AddTask(
						BehaviorType::PatrolTask,
						std::make_shared<maru::Behavior::Task::SearchBall>(owner)
					);

					//Ballの場所まで駆けつけるタスク
					m_behaviorTree->AddTask<Task::ToBallRunTask>(
						BehaviorType::ToBallRunTask,
						owner
					);

					//Goalまで行くタスク
					m_behaviorTree->AddTask<Task::ToGoalRunTask>(
						BehaviorType::ToGoalRunTask,
						owner
					);

					//見方を守るタスク
					m_behaviorTree->AddTask<Task::RelifMember>(
						BehaviorType::RelifHasBallMemberTask,
						owner
					);

					//ボールを持つ敵を追いかけるタスク
					m_behaviorTree->AddTask<Task::ToHasBallOtherTeam>(
						BehaviorType::ToMoveHasBallEnemyTask,
						owner
					);
				}

				void HidePlacePatrolTree::CreateEdge() {
					using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

					//初回セレクター
					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter, 
						BehaviorType::PatrolTask, 
						(int)BehaviorType::PatrolTask
					);

					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter,
						BehaviorType::ToBallRunTask,
						(int)BehaviorType::ToBallRunTask
					);

					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter,
						BehaviorType::ToGoalRunTask,
						(int)BehaviorType::ToGoalRunTask
					);

					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter,
						BehaviorType::RelifHasBallMemberTask,
						(int)BehaviorType::RelifHasBallMemberTask
					);

					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter,
						BehaviorType::ToMoveHasBallEnemyTask,
						(int)BehaviorType::ToMoveHasBallEnemyTask
					);
				}

				void HidePlacePatrolTree::CreateDecorator() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//パトロールタスク--------------------------------------------------------------------------------------
					

					//-------------------------------------------------------------------------------------------------------

					//ボールまで行くタスク-----------------------------------------------------------------------------------
					
					//HideItemがActive状態なら
					auto hideObjects = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();
					for (auto& hideObject : hideObjects) {
						m_behaviorTree->AddDecorator<Decorator::IsAcitiveSpecificTarget>(
							BehaviorType::ToBallRunTask,
							GetOwner(),
							hideObject.lock()
						);
					}

					//-------------------------------------------------------------------------------------------------------

					//ゴールまで行くタスク-----------------------------------------------------------------------------------

					//ボールを持っている人がいるのなら
					m_behaviorTree->AddDecorator<Decorator::HasBallTransition>(
						BehaviorType::ToGoalRunTask,
						GetOwner()
					);

					//-------------------------------------------------------------------------------------------------------

					//見方を守るタスク---------------------------------------------------------------------------------------

					//ボールを持っている人がいるなら
					m_behaviorTree->AddDecorator<Decorator::HasBall_OtherMember>(
						BehaviorType::RelifHasBallMemberTask,
						enemy
					);

					//-------------------------------------------------------------------------------------------------------

					//ボールを持つ敵を追いかけるタスク---------------------------------------------------------------------------------------

					//ボールを持っている人がいるなら
					m_behaviorTree->AddDecorator<Decorator::HasBall_OtherTeam>(
						BehaviorType::ToMoveHasBallEnemyTask,
						enemy
					);

					//-------------------------------------------------------------------------------------------------------

					m_behaviorTree->AddDecorator<Decorator::TimerWaitPosition>(
						BehaviorType::FirstSelecter,
						GetOwner()
					);
				}

			}
		}
	}
}