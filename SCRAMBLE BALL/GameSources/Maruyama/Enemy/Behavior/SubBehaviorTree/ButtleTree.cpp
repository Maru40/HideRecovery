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
#include "Maruyama/Enemy/Behavior/Task/NearAstarMove.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"

#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

#include "Maruyama/Enemy/Behavior/Decorator/IsInEyeTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/ObserveTargets.h"
#include "Maruyama/Enemy/Behavior/Decorator/OutSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/CanCurrentTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/OutTargetTimer.h"

#include "Maruyama/Enemy/AIDirector/CoordinatorBase.h"
#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

#include "Maruyama/Utility/Component/TargetManager.h"

#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"
#include "Maruyama/StageObject/Goal.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				constexpr float EYE_DEGREE = 90.0f;

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータのデコレータ群
				//--------------------------------------------------------------------------------------

				using EyeParametor = EyeSearchRangeParametor;

				ButtleTree_DecoratorParametor::ButtleTree_DecoratorParametor():
					nearSeek_isInEyeParamPtr(new Decorator::IsInEyeTarget_Parametor(EyeParametor(20.0f, 3.0f, XMConvertToRadians(EYE_DEGREE)), 0.5f, 0.5f)),
					nearAstar_isInEyeParamPtr(new Decorator::IsInEyeTarget_Parametor(EyeParametor(20.0f, 3.0f, XMConvertToRadians(EYE_DEGREE)), 5.0f, 5.0f)),
					nearAstar_outTargetTimerParamPtr(new Decorator::OutTargetTimer_Parametor(EyeParametor(20.0f, 3.0f, XMConvertToRadians(EYE_DEGREE)), 5.0f, 5.0f)),
					shot_isInEyeParamPtr(new Decorator::IsInEyeTarget_Parametor(EyeParametor(25.0f, 3.0f, XMConvertToRadians(EYE_DEGREE)), 0.0f, 1.5f))
				{}

				ButtleTree_DecoratorParametor::~ButtleTree_DecoratorParametor() {
					delete(nearSeek_isInEyeParamPtr);
					delete(nearAstar_isInEyeParamPtr);
					delete(nearAstar_outTargetTimerParamPtr);
					delete(shot_isInEyeParamPtr);
				}

				//--------------------------------------------------------------------------------------
				/// バトル用のビヘイビアツリーのパラメータのタスク群
				//--------------------------------------------------------------------------------------

				ButtleTree_TaskParametor::ButtleTree_TaskParametor():
					nearSeekParamPtr(new Task::NearSeekMove::Parametor()),
					nearAstarParamPtr(new Task::NearAstarMove_Parametor(EyeParametor(20.0f, 3.0f, XMConvertToRadians(EYE_DEGREE))))
				{}

				ButtleTree_TaskParametor::~ButtleTree_TaskParametor() {
					delete(nearSeekParamPtr);
					delete(nearAstarParamPtr);
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

					//Astarを使って近づくタスク
					m_behaviorTree->AddTask<Task::NearAstarMove>(NodeType::NearAstarMoveTask, enemy, m_param.taskParam.nearAstarParamPtr);
					
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
					m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearAstarMoveTask, (int)NodeType::NearAstarMoveTask);

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

					//FirstSelecter-----------------------------------------------
					//視界範囲で他の敵を監視
					m_behaviorTree->AddDecorator<Decorator::ObserveTargets>(
						NodeType::FirstSelecter, enemy, std::vector<std::shared_ptr<GameObject>>()
					);

					//NearSeekデコレータ------------------------------------------
					//視界範囲内なら
					m_behaviorTree->AddDecorator<Decorator::IsInEyeTarget>(
						NodeType::NearSeekMoveTask, enemy, decoratorParam.nearSeek_isInEyeParamPtr
					);

					//AstarSeek----------------------------------------------------
					//ターゲットが視界外に一定時間いたら、ターゲットをあきらめる。
					//m_behaviorTree->AddDecorator<Decorator::OutTargetTimer>(
					//	NodeType::NearAstarMoveTask, enemy, decoratorParam.nearAstar_outTargetTimerParamPtr
					//);

					//Shotにデコレータ追加-----------------------------------------
					//視界範囲内なら
					m_behaviorTree->AddDecorator<Decorator::IsInEyeTarget>(
						NodeType::ShotTask, enemy, decoratorParam.shot_isInEyeParamPtr
					);

					//ターゲットが特定のターゲットなら遷移できないようにする。
					m_behaviorTree->AddDecorator<Decorator::OutSpecificTarget>(
						NodeType::ShotTask, enemy, GetShotOutTarget()
					);

					m_behaviorTree->AddDecorator<Decorator::CanCurrentTarget>(
						NodeType::AttackMoveSelecter, enemy->GetGameObject()
					);

					m_behaviorTree->AddDecorator<Decorator::CanCurrentTarget>(
						NodeType::ShotTask, enemy->GetGameObject()
					);
				}

				void ButtleTree::InitializeParametor() {
					//将来的には外部ファイルにする。
					constexpr float MoveSpeed = 8.5f;

					//通常シークパラメータ 
					{
						auto& param = m_param.taskParam.nearSeekParamPtr;
						auto& moveParam = param->moveParamPtr->toTargetMoveParam;
						moveParam->speed = MoveSpeed;
						moveParam->targetNearRange = 20.0f;
						moveParam->moveType = basecross::Task::ToTargetMove::MoveType::ArriveVelocity;
					}

					//AstarMoveパラメータ
					{
						auto& param = m_param.taskParam.nearAstarParamPtr;
						auto& moveParam = param->moveParamPtr->movePositionsParam->moveParamPtr;
						moveParam->speed = MoveSpeed;
						moveParam->moveType = basecross::Task::ToTargetMove::MoveType::SeekVelocity;
					}

					//Shotパラメータ
					{

					}
				}

				std::shared_ptr<GameObject> ButtleTree::GetShotOutTarget() {
					//チームメンバーがアタッチされているかどうか判断
					auto teamMember = GetOwner()->GetComponent<I_TeamMember>(false);
					if (!teamMember) {
						Debug::GetInstance()->Log(L"ButtleTree::GetShotOutTarget() : I_TeamMemberがアタッチされていません。");
						return nullptr;
					}

					auto goals = ShareClassesManager::GetInstance()->GetCastShareClasses<Goal>();

					for (auto& goal : goals) {
						//自分のチームと違うゴールなら。
						if (goal.lock()->GetTeam() != teamMember->GetTeam()) {
							return goal.lock()->GetGameObject();
						}
					}

					return nullptr;
				}

			}
		}
	}
}