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
#include "Maruyama/Enemy/Behavior/Task/ToGoalRunTask.h"
#include "Maruyama/Enemy/Behavior/Task/RelifMember.h"
#include "Maruyama/Enemy/Behavior/Task/ToHasBallOtherTeam.h"

#include "Maruyama/Enemy/Behavior/Interface/I_PriorityController.h"

//�f�R���[�^
#include "Maruyama/Enemy/Behavior/Decorator/OutSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/IsActiveSpecificTarget.h"
#include "Maruyama/Enemy/Behavior/Decorator/SettingStartTarget_Ball.h"
#include "Maruyama/Enemy/Behavior/Decorator/HasBallTransition.h"
#include "Maruyama/Enemy/Behavior/Decorator/HasBall_OtherMember.h"
#include "Maruyama/Enemy/Behavior/Decorator/HasBall_OtherTeam.h"
#include "Maruyama/Enemy/Behavior/Decorator/TimerWaitPosition.h"
#include "Maruyama/Enemy/Behavior/Decorator/SettingTarget_HasBallOtherMember.h"
#include "Maruyama/Enemy/Behavior/Decorator/SettingTarget_RandomTeamMember.h"

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

				HidePlacePatrolTree_Parametor::HidePlacePatrolTree_Parametor()

				{}

				HidePlacePatrolTree_Parametor::~HidePlacePatrolTree_Parametor(){

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

					//Ball�̏ꏊ�܂ŋ삯����^�X�N
					m_behaviorTree->AddTask<Task::ToBallRunTask>(
						BehaviorType::ToBallRunTask,
						owner
					);

					//Goal�܂ōs���^�X�N
					m_behaviorTree->AddTask<Task::ToGoalRunTask>(
						BehaviorType::ToGoalRunTask,
						owner
					);

					//���������^�X�N
					m_behaviorTree->AddTask<Task::RelifMember>(
						BehaviorType::RelifHasBallMemberTask,
						owner
					);

					//�{�[�������G��ǂ�������^�X�N
					m_behaviorTree->AddTask<Task::ToHasBallOtherTeam>(
						BehaviorType::ToMoveHasBallEnemyTask,
						owner
					);

					//�����_���ȃ����o�[�����B
					m_behaviorTree->AddTask<Task::RelifMember>(
						BehaviorType::RandomMemberRelif,
						owner
					);
				}

				void HidePlacePatrolTree::CreateEdge() {
					//using PriorityControllerBase = maru::Behavior::PriorityControllerBase;

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

					m_behaviorTree->AddEdge(
						BehaviorType::FirstSelecter,
						BehaviorType::RandomMemberRelif,
						(int)BehaviorType::RandomMemberRelif
					);
				}

				void HidePlacePatrolTree::CreateDecorator() {
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>();

					//�p�g���[���^�X�N--------------------------------------------------------------------------------------
					

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

					//-------------------------------------------------------------------------------------------------------

					//�S�[���܂ōs���^�X�N-----------------------------------------------------------------------------------

					//�{�[���������Ă���l������̂Ȃ�
					m_behaviorTree->AddDecorator<Decorator::HasBallTransition>(
						BehaviorType::ToGoalRunTask,
						GetOwner()
					);

					//-------------------------------------------------------------------------------------------------------

					//���������^�X�N---------------------------------------------------------------------------------------

					//�{�[���������Ă���l������Ȃ�
					m_behaviorTree->AddDecorator<Decorator::HasBall_OtherMember>(
						BehaviorType::RelifHasBallMemberTask,
						enemy
					);

					//�{�[���������Ă��閡�����^�[�Q�b�g�ɂ���B
					m_behaviorTree->AddDecorator<Decorator::SettingTarget_HasBallOtherMember>(
						BehaviorType::RelifHasBallMemberTask,
						enemy
					);

					//-------------------------------------------------------------------------------------------------------

					//�{�[�������G��ǂ�������^�X�N-----------------------------------------------------------------------

					//�{�[���������Ă���l������Ȃ�
					m_behaviorTree->AddDecorator<Decorator::HasBall_OtherTeam>(
						BehaviorType::ToMoveHasBallEnemyTask,
						enemy
					);

					//-------------------------------------------------------------------------------------------------------


					//�����_���ȃ`�[�������o�[�����-----------------------------------------------------------------------

					//�����_���ȃ`�[�������o�[�����B
					m_behaviorTree->AddDecorator<Decorator::SettingTarget_RandomTeamMember>(
						BehaviorType::RandomMemberRelif,
						enemy
					);

					//-------------------------------------------------------------------------------------------------------

					//�^�X�N�S�ĂɈ�莞�ԓ����ꏊ�ɂ�����A�o�H��T�����Ȃ���������ǉ��B
					m_behaviorTree->AddDecorator<Decorator::TimerWaitPosition>(
						BehaviorType::FirstSelecter,
						GetOwner()
					);
				}

			}
		}
	}
}