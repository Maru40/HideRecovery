/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTreeなど実体
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

//デコレータ
#include "Maruyama/Enemy/Behavior/Decorator/OutSpecificTarget.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Item/HideItemObject.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				HidePlacePatrolTree::HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr) :
					SubBehaviorTreeBase(objPtr)
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

				}

				void HidePlacePatrolTree::CreateEdge() {
					using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

					//初回セレクター
					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter, 
						BehaviorType::PatrolTask, 
						std::make_shared<PriorityControllerBase>(0.0f)
					);

					//m_behaviorTree->AddEdge(
					//	BehaviorType::FirstSelecter,
					//	BehaviorType::ToBallRunTask,
					//	std::make_shared<PriorityControllerBase>((int)BehaviorType::ToBallRunTask)
					//);
				}

				void HidePlacePatrolTree::CreateDecorator() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//パトロールタスク--------------------------------------------------------------------------------------
					
					//HideItemがターゲットの場合、遷移できないようにする。
					auto outSpecificTargets = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();
					for (auto& target : outSpecificTargets) {
						m_behaviorTree->AddDecorator<Decorator::OutSpecificTarget>(
							BehaviorType::PatrolTask,
							enemy,
							target.lock()
						);
					}

					//HideItemが存在するなら


					//-------------------------------------------------------------------------------------------------------
				}

			}
		}
	}
}