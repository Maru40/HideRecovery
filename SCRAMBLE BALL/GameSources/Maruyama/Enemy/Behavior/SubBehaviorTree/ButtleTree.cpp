/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "ButtleTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/Task/BehaviorTask_Shot.h"
#include "Maruyama/Enemy/Behavior/Task/NearSeekMove.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"

#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

#include "Maruyama/Enemy/Behavior/Decorator/IsInEyeTarget.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータのデコレータ群
				//--------------------------------------------------------------------------------------

				ButtleTree_DecoratorParametor::ButtleTree_DecoratorParametor():
					shot_isInEyeParamPtr(new Decorator::IsInEyeTarget_Parametor(EyeSearchRangeParametor(20.0f), 0.5f))
				{}

				ButtleTree_DecoratorParametor::~ButtleTree_DecoratorParametor() {
					delete(shot_isInEyeParamPtr);
				}

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータのタスク群
				//--------------------------------------------------------------------------------------

				ButtleTree_TaskParametor::ButtleTree_TaskParametor():
					nearSeekParamPtr(new Task::NearSeekMove::Parametor())
				{}

				ButtleTree_TaskParametor::~ButtleTree_TaskParametor() {
					delete(nearSeekParamPtr);
				}

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータ
				//--------------------------------------------------------------------------------------

				ButtleTree_Parametor::ButtleTree_Parametor() :
					taskParam(ButtleTree_TaskParametor()),
					decoratorParam(ButtleTree_DecoratorParametor())
				{}

				ButtleTree_Parametor::~ButtleTree_Parametor() {

				}

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリー
				//--------------------------------------------------------------------------------------

				ButtleTree::ButtleTree(
					const std::shared_ptr<GameObject>& owner
				) :
					SubBehaviorTreeBase(owner),
					m_param(Parametor())
				{
					InitializeParametor();
				}

				void ButtleTree::CreateNode() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//初期セレクター
					m_behaviorTree->AddSelecter(NodeType::FirstSelecter);

					//攻撃セレクター
					m_behaviorTree->AddSelecter(NodeType::AttackSelecter);

					//攻撃中の移動セレクター
					m_behaviorTree->AddSelecter(NodeType::AttackMoveSelecter);

					//近づくセレクター
					m_behaviorTree->AddSelecter(NodeType::NearMoveSelecter);

					//直線的に近づくセレクター
					m_behaviorTree->AddTask<Task::NearSeekMove>(NodeType::NearSeekMoveTask, enemy, m_param.taskParam.nearSeekParamPtr);

					//m_behaviorTree->AddTask(NodeType::NearAstarMoveTask);
					
					//回り込みセレクター
					m_behaviorTree->AddSelecter(NodeType::WanparoundSelecter);

					////右から回り込むタスク
					//m_behaviorTree->AddTask(NodeType::RightSideMoveTask);

					////左から回り込むタスク
					//m_behaviorTree->AddTask(NodeType::LeftSideMoveTask);

					////裏取をするタスク
					//m_behaviorTree->AddTask(NodeType::BackSideMoveTask);


					//射撃攻撃タスク
					m_behaviorTree->AddTask<Task::Shot>(NodeType::ShotTask, enemy);

					//退避セレクター
					m_behaviorTree->AddSelecter(NodeType::EvadeSelecter);

					//足止めセレクター
					m_behaviorTree->AddSelecter(NodeType::InducementStaySelecter);

					//足止め攻撃
					//m_behaviorTree->AddTask(NodeType::InducementAttack);

					//退避移動
					//m_behaviorTree->AddTask(NodeType::EvadeMoveTask);
				}

				void ButtleTree::CreateEdge() {
					//初期セレクター
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::AttackSelecter, (int)NodeType::AttackSelecter);
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::EvadeSelecter, (int)NodeType::EvadeSelecter);

					CreateAttackEdge();
					CreateEvadeEdge();
				}

				void ButtleTree::CreateAttackEdge() {
					//攻撃セレクター
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::AttackMoveSelecter, (int)NodeType::AttackMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::ShotTask, 0.0f);

					//攻撃中の移動セレクター
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::NearMoveSelecter, (int)NodeType::NearMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::WanparoundSelecter, (int)NodeType::WanparoundSelecter);

					//近づくときのセレクター
					m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearSeekMoveTask, (int)NodeType::NearSeekMoveTask);
					//m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearAstarMoveTask, (int)NodeType::NearAstarMoveTask);

					//回り込みセレクター
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::RightSideMoveTask, (int)NodeType::RightSideMoveTask);
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::LeftSideMoveTask, (int)NodeType::LeftSideMoveTask);
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::BackSideMoveTask, (int)NodeType::BackSideMoveTask);
				}

				void ButtleTree::CreateEvadeEdge() {
					//逃亡セレクター
					m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::InducementStaySelecter, (int)NodeType::InducementStaySelecter);
					//m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::EvadeMoveTask, (int)NodeType::EvadeMoveTask);

					////足止めセレクター
					//m_behaviorTree->AddEdge(NodeType::InducementStaySelecter, NodeType::InducementAttack, (int)NodeType::InducementAttack);
				}

				void ButtleTree::CreateDecorator() {
					auto& decoratorParam = m_param.decoratorParam;
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>(false);

					//NearSeekデコレータ


					//Shotにデコレータ追加
					//視界範囲内なら
					m_behaviorTree->AddDecorator<Decorator::IsInEyeTarget>(NodeType::ShotTask, enemy, decoratorParam.shot_isInEyeParamPtr);
				}

				void ButtleTree::InitializeParametor() {
					//通常シークパラメータ
					auto& nearSeekParamPtr = m_param.taskParam.nearSeekParamPtr;
					nearSeekParamPtr->moveParamPtr->toTargetMoveParam->speed = 6.0f;
					nearSeekParamPtr->moveParamPtr->toTargetMoveParam->targetNearRange = 15.0f;
					nearSeekParamPtr->moveParamPtr->toTargetMoveParam->moveType = basecross::Task::ToTargetMove::MoveType::ArriveVelocity;
				}

			}
		}
	}
}