/*!
@file HidePlacePatrolTree.cpp
@brief HidePlacePatrolTree�Ȃǎ���
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
#include "Maruyama/Enemy/Behavior/Decorator/TimerWaitPosition.h"

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
				/// �o�g���p�̃r�w�C�r�A�c���[�̃p�����[�^�̃f�R���[�^�Q
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
				/// �o�g���p�̃r�w�C�r�A�c���[�̃p�����[�^�̃^�X�N�Q
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
				/// �o�g���p�̃r�w�C�r�A�c���[�̃p�����[�^
				//--------------------------------------------------------------------------------------

				ButtleTree_Parametor::ButtleTree_Parametor() :
					taskParam(ButtleTree_TaskParametor()),
					decoratorParam(ButtleTree_DecoratorParametor())
				{}

				ButtleTree_Parametor::~ButtleTree_Parametor() {

				}

				//--------------------------------------------------------------------------------------
				/// �o�g���p�̃r�w�C�r�A�c���[
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

					//�����Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::FirstSelecter);

					//�U���Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::AttackSelecter);

					//�U�����̈ړ��Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::AttackMoveSelecter);

					//�߂Â��Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::NearMoveSelecter);

					//�����I�ɋ߂Â��Z���N�^�[
					m_behaviorTree->AddTask<Task::NearSeekMove>(NodeType::NearSeekMoveTask, enemy, m_param.taskParam.nearSeekParamPtr);

					//Astar���g���ċ߂Â��^�X�N
					m_behaviorTree->AddTask<Task::NearAstarMove>(NodeType::NearAstarMoveTask, enemy, m_param.taskParam.nearAstarParamPtr);
					
					//��荞�݃Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::WanparoundSelecter);

					////�E�����荞�ރ^�X�N
					//m_behaviorTree->AddTask(NodeType::RightSideMoveTask);

					////�������荞�ރ^�X�N
					//m_behaviorTree->AddTask(NodeType::LeftSideMoveTask);

					////���������^�X�N
					//m_behaviorTree->AddTask(NodeType::BackSideMoveTask);


					//�ˌ��U���^�X�N
					m_behaviorTree->AddTask<Task::Shot>(NodeType::ShotTask, enemy);

					//�ޔ��Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::EvadeSelecter);

					//���~�߃Z���N�^�[
					m_behaviorTree->AddSelecter(NodeType::InducementStaySelecter);

					//���~�ߍU��
					//m_behaviorTree->AddTask(NodeType::InducementAttack);

					//�ޔ��ړ�
					//m_behaviorTree->AddTask(NodeType::EvadeMoveTask);
				}

				void ButtleTree::CreateEdge() {
					//�����Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::AttackSelecter, (int)NodeType::AttackSelecter);
					m_behaviorTree->AddEdge(NodeType::FirstSelecter, NodeType::EvadeSelecter, (int)NodeType::EvadeSelecter);

					CreateAttackEdge();
					CreateEvadeEdge();
				}

				void ButtleTree::CreateAttackEdge() {
					//�U���Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::AttackMoveSelecter, (int)NodeType::AttackMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackSelecter, NodeType::ShotTask, 0.0f);

					//�U�����̈ړ��Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::NearMoveSelecter, (int)NodeType::NearMoveSelecter);
					m_behaviorTree->AddEdge(NodeType::AttackMoveSelecter, NodeType::WanparoundSelecter, (int)NodeType::WanparoundSelecter);

					//�߂Â��Ƃ��̃Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearSeekMoveTask, (int)NodeType::NearSeekMoveTask);
					m_behaviorTree->AddEdge(NodeType::NearMoveSelecter, NodeType::NearAstarMoveTask, (int)NodeType::NearAstarMoveTask);

					//��荞�݃Z���N�^�[
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::RightSideMoveTask, (int)NodeType::RightSideMoveTask);
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::LeftSideMoveTask, (int)NodeType::LeftSideMoveTask);
					//m_behaviorTree->AddEdge(NodeType::WanparoundSelecter, NodeType::BackSideMoveTask, (int)NodeType::BackSideMoveTask);
				}

				void ButtleTree::CreateEvadeEdge() {
					//���S�Z���N�^�[
					m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::InducementStaySelecter, (int)NodeType::InducementStaySelecter);
					//m_behaviorTree->AddEdge(NodeType::EvadeSelecter, NodeType::EvadeMoveTask, (int)NodeType::EvadeMoveTask);

					////���~�߃Z���N�^�[
					//m_behaviorTree->AddEdge(NodeType::InducementStaySelecter, NodeType::InducementAttack, (int)NodeType::InducementAttack);
				}

				void ButtleTree::CreateDecorator() {
					auto& decoratorParam = m_param.decoratorParam;
					auto enemy = GetOwner()->GetComponent<Enemy::EnemyBase>(false);

					//FirstSelecter-----------------------------------------------
					//���E�͈͂ő��̓G���Ď�
					m_behaviorTree->AddDecorator<Decorator::ObserveTargets>(
						NodeType::FirstSelecter, enemy, std::vector<std::shared_ptr<GameObject>>()
					);

					//NearSeek�f�R���[�^------------------------------------------
					//���E�͈͓��Ȃ�
					m_behaviorTree->AddDecorator<Decorator::IsInEyeTarget>(
						NodeType::NearSeekMoveTask, enemy, decoratorParam.nearSeek_isInEyeParamPtr
					);

					//��莞�ԓ����ꏊ�ɂ���Ȃ�B
					m_behaviorTree->AddDecorator<Decorator::TimerWaitPosition>(
						NodeType::NearSeekMoveTask, GetOwner()
					);

					//AstarSeek----------------------------------------------------
					//�^�[�Q�b�g�����E�O�Ɉ�莞�Ԃ�����A�^�[�Q�b�g��������߂�B
					//m_behaviorTree->AddDecorator<Decorator::OutTargetTimer>(
					//	NodeType::NearAstarMoveTask, enemy, decoratorParam.nearAstar_outTargetTimerParamPtr
					//);

					//��莞�ԓ����ꏊ�ɂ����猟�����Ȃ����B
					m_behaviorTree->AddDecorator<Decorator::TimerWaitPosition>(
						NodeType::NearAstarMoveTask, GetOwner()
					);

					//Shot�Ƀf�R���[�^�ǉ�-----------------------------------------
					//���E�͈͓��Ȃ�
					m_behaviorTree->AddDecorator<Decorator::IsInEyeTarget>(
						NodeType::ShotTask, enemy, decoratorParam.shot_isInEyeParamPtr
					);

					//�^�[�Q�b�g������̃^�[�Q�b�g�Ȃ�J�ڂł��Ȃ��悤�ɂ���B
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
					//�����I�ɂ͊O���t�@�C���ɂ���B
					constexpr float MoveSpeed = 8.5f;

					//�ʏ�V�[�N�p�����[�^ 
					{
						auto& param = m_param.taskParam.nearSeekParamPtr;
						auto& moveParam = param->moveParamPtr->toTargetMoveParam;
						moveParam->speed = MoveSpeed;
						moveParam->targetNearRange = 20.0f;
						moveParam->moveType = basecross::Task::ToTargetMove::MoveType::ArriveVelocity;
					}

					//AstarMove�p�����[�^
					{
						auto& param = m_param.taskParam.nearAstarParamPtr;
						auto& moveParam = param->moveParamPtr->movePositionsParam->moveParamPtr;
						moveParam->speed = MoveSpeed;
						moveParam->moveType = basecross::Task::ToTargetMove::MoveType::SeekVelocity;
					}

					//Shot�p�����[�^
					{

					}
				}

				std::shared_ptr<GameObject> ButtleTree::GetShotOutTarget() {
					//�`�[�������o�[���A�^�b�`����Ă��邩�ǂ������f
					auto teamMember = GetOwner()->GetComponent<I_TeamMember>(false);
					if (!teamMember) {
						Debug::GetInstance()->Log(L"ButtleTree::GetShotOutTarget() : I_TeamMember���A�^�b�`����Ă��܂���B");
						return nullptr;
					}

					auto goals = ShareClassesManager::GetInstance()->GetCastShareClasses<Goal>();

					for (auto& goal : goals) {
						//�����̃`�[���ƈႤ�S�[���Ȃ�B
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