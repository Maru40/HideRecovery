/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTree�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

#include "HidePlacePatrolTree.h"

#include "Maruyama/Enemy/Behavior/BehaviorTree.h"

#include "Maruyama/Enemy/Component/EnemyBase.h"
#include "Maruyama/Enemy/Component/EyeSearchRange.h"

//�^�X�N
#include "Maruyama/Enemy/Behavior/Task/Task_SearchBall.h"
#include "Maruyama/Enemy/Behavior/Task/NearAstarMove.h"
#include "Maruyama/TaskList/CommonTasks/MoveAstar.h"
#include "Maruyama/TaskList/CommonTasks/Task_MovePositions.h"
#include "Maruyama/Enemy/Behavior/Task/NearSeekMove.h"
#include "Maruyama/TaskList/CommonTasks/TargetSeek.h"
#include "Maruyama/TaskList/CommonTasks/Task_ToTargetMove.h"
#include "Maruyama/Enemy/Behavior/Task/ToBallRunTask.h"

#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

//�f�R���[�^
#include "Maruyama/Enemy/Behavior/Decorator/OutSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/IsActiveSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/SettingStartTarget_Ball.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Utility/SingletonComponent/ShareClassesManager.h"

#include "Maruyama/Item/HideItemObject.h"

namespace basecross {

	namespace maru {
		namespace Behavior {

			namespace SubBehavior {

				//--------------------------------------------------------------------------------------
				/// �B��ꏊ��T���p�g���[���r�w�C�r�A�c���[�̃p�����[�^
				//--------------------------------------------------------------------------------------

				HidePlacePatrolTree_Parametor::HidePlacePatrolTree_Parametor():
					astarMoveParamPtr(new Task::NearAstarMove_Parametor(EyeSearchRangeParametor(20.0f, 3.0f, XMConvertToRadians(90.0f)))),
					seekMoveParamPtr(new Task::NearSeekMove_Parametor())
				{}

				HidePlacePatrolTree_Parametor::~HidePlacePatrolTree_Parametor(){
					delete(astarMoveParamPtr);
					delete(seekMoveParamPtr);
				}

				//--------------------------------------------------------------------------------------
				/// �B��ꏊ��T���p�g���[���r�w�C�r�A�c���[
				//--------------------------------------------------------------------------------------

				HidePlacePatrolTree::HidePlacePatrolTree(const std::shared_ptr<GameObject>& objPtr) :
					SubBehaviorTreeBase(objPtr),
					m_param(Parametor())
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

					auto& param = m_param.astarMoveParamPtr;
					auto& moveParam = param->moveParamPtr->movePositionsParam->moveParamPtr;
					moveParam->speed = 8.5f;
					moveParam->moveType = basecross::Task::ToTargetMove::MoveType::SeekVelocity;
					//Ball�̏ꏊ�܂ŋ삯����^�X�N
					m_behaviorTree->AddTask<Task::ToBallRunTask>(
						BehaviorType::ToBallRunTask,
						owner
					);

					//Goal�܂ōs���^�X�N

				}

				void HidePlacePatrolTree::CreateEdge() {
					using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

					//����Z���N�^�[
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
				}

				void HidePlacePatrolTree::CreateDecorator() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//�p�g���[���^�X�N--------------------------------------------------------------------------------------
					
					//HideItem���^�[�Q�b�g�̏ꍇ�A�J�ڂł��Ȃ��悤�ɂ���B
					//auto outSpecificTargets = ShareClassesManager::GetInstance()->GetCastShareClasses<HideItemObject>();
					//for (auto& target : outSpecificTargets) {
					//	m_behaviorTree->AddDecorator<Decorator::OutSpecificTarget>(
					//		BehaviorType::PatrolTask,
					//		enemy,
					//		target.lock()
					//	);
					//}

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

					//�{�[�����^�[�Q�b�g�ɂ���B
					//m_behaviorTree->AddDecorator<Decorator::SettingStartTarget_Ball>(
					//	BehaviorType::ToBallRunTask,
					//	GetOwner(),
					//	hideObjects[0].lock()
					//);

					//�S�[���܂ōs���^�X�N-----------------------------------------------------------------------------------

					//�{�[���������Ă���l������̂Ȃ�


					//-------------------------------------------------------------------------------------------------------
				}

			}
		}
	}
}