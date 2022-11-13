/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTree�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/Enemy/Behavior/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

//�f�R���[�^
#include "Maruyama/Enemy/Behavior/Decorator/OutSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/IsActiveSpecificTarget.h"

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

					//����Z���N�^�[
					m_behaviorTree->AddSelecter(BehaviorType::FirstSelecter);

					//�{�[���T���^�X�N
					m_behaviorTree->AddTask(
						BehaviorType::PatrolTask,
						std::make_shared<maru::Behavior::Task::SearchBall>(owner)
					);

					//Ball�̏ꏊ�܂ŋ삯����^�X�N
					

					//Goal�܂ōs���^�X�N

				}

				void HidePlacePatrolTree::CreateEdge() {
					using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

					//����Z���N�^�[
					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter, 
						BehaviorType::PatrolTask, 
						std::make_shared<PriorityControllerBase>((int)BehaviorType::PatrolTask)
					);

					//m_behaviorTree->AddEdge(
					//	BehaviorType::FirstSelecter,
					//	BehaviorType::ToBallRunTask,
					//	std::make_shared<PriorityControllerBase>((int)BehaviorType::ToBallRunTask)
					//);
				}

				void HidePlacePatrolTree::CreateDecorator() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//�p�g���[���^�X�N--------------------------------------------------------------------------------------
					
					//HideItem���^�[�Q�b�g�̏ꍇ�A�J�ڂł��Ȃ��悤�ɂ���B
					auto outSpecificTargets = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();
					for (auto& target : outSpecificTargets) {
						m_behaviorTree->AddDecorator<Decorator::OutSpecificTarget>(
							BehaviorType::PatrolTask,
							enemy,
							target.lock()
						);
					}

					//-------------------------------------------------------------------------------------------------------

					//�{�[���܂ōs���^�X�N-----------------------------------------------------------------------------------
					
					//HideItem��Active��ԂȂ�
					auto hideObjects = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();
					for (auto& hideObject : hideObjects) {
						m_behaviorTree->AddDecorator<Decorator::IsAcitiveSpecificTarget>(
							BehaviorType::ToBallRunTask,
							GetOwner(),
							hideObject.lock()
						);
					}

					//�S�[���܂ōs���^�X�N


				}

			}
		}
	}
}