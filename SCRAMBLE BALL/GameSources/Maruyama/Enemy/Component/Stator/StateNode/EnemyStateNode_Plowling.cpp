
/*!
@file EnemyStateNode_Plowling.cpp
@brief EnemyStateNode_Plowling�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Timer/TimeHelper.h"
#include "Maruyama/Utility/Timer/GameTimer.h"
#include "Maruyama/Utility/Component/TargetManager.h"
#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/TaskList/CommonTasks/Task_Wait.h"
#include "Maruyama/Enemy/StateMachine/EnemyStateNodeBase.h"
#include "EnemyStateNode_Plowling.h"

#include "Maruyama/Utility/UtilityVelocity.h"
#include "VelocityManager.h"
#include "Maruyama/Enemy/Component/EnemyBase.h"

#include "Maruyama/TaskList/CommonTasks/Task_PlowlingMove.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "Maruyama/Enemy/ImpactMap/ImpactMap.h"
#include "Maruyama/Enemy/Astar/GraphAstar.h"
#include "Maruyama/Enemy/Astar/SparseGraph.h"
#include "Maruyama/Enemy/Astar/UtilityAstar.h"

#include "Maruyama/Enemy/ImpactMap/FieldImpactMap.h"
#include "Maruyama/Utility/Timer/GameTimer.h"

namespace basecross {
	namespace Enemy {

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// �p�����[�^
			//--------------------------------------------------------------------------------------

			Plowling::Parametor::Parametor() :
				Parametor(std::make_shared<Task_PlowlingMove::Parametor>(), std::make_shared<Task::Wait::Parametor>())
			{}

			Plowling::Parametor::Parametor(
				const std::shared_ptr<Task_PlowlingMove::Parametor>& moveParamPtr,
				const std::shared_ptr<Task::Wait::Parametor>& waitParamPtr
			) :
				moveParamPtr(moveParamPtr),
				waitParamPtr(waitParamPtr)
			{}


			//--------------------------------------------------------------------------------------
			/// �X���C���̜p�j�^�X�N�^�C�v
			//--------------------------------------------------------------------------------------

			enum class Plowling::TaskEnum
			{
				Move,  //�ړ�
				Wait,  //�ҋ@(�ړI�n�ɂ������Ɉ�莞�ԑҋ@)
			};

			//--------------------------------------------------------------------------------------
			/// �X���C���̜p�j�s���X�e�[�g�{��
			//--------------------------------------------------------------------------------------

			Plowling::Plowling(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr) :
				EnemyStateNodeBase<EnemyBase>(owner), 
				m_paramPtr(paramPtr), 
				m_taskList(new TaskList<TaskEnum>()), 
				m_arriveTimer(new GameTimer(0.0f))
			{
				DefineTask();
			}

			void Plowling::OnStart() {
				CalculateAstarRamdomPosition();

				//�|�W�V�������ݒ肳��Ă��Ȃ��Ȃ珈�������Ȃ�
				if (m_paramPtr->positions.size() == 0) { 
					DebugObject::AddString(L"Slime_Plowling::OnStart�ŁA�|�W�V�������ݒ肳��Ă��܂���B");
					return;
				}

				//�������|�W�V������ݒ�
				m_index = 0;
				m_addIndexDirect = 1;
				ChangeMoveTargetPosition(m_paramPtr->positions[m_index]);

				m_taskList->ForceStop(false);
				SelectTask();
			}

			bool Plowling::OnUpdate() {
				//�|�W�V�������ݒ肳��Ă��Ȃ��Ȃ珈�������Ȃ�
				if (m_paramPtr->positions.size() == 0) {  
					CalculateAstarRamdomPosition();
					return false; 
				}

				m_taskList->UpdateTask();

				//�^�X�N���I��������
				if (m_taskList->IsEnd()) { 
					NextPosition(); //���̃|�W�V�������Z�b�g
					SelectTask();   //�^�X�N�̍ăZ�b�g
					return false;
				}

				//�������Ԍv��
				m_arriveTimer->UpdateTimer();
				if (m_arriveTimer->IsTimeUp()) {
					//���������ł����ǂ蒅���Ȃ�������A�o�H�Č���B
					OnStart();
				}

				return false;
			}

			void Plowling::OnExit() {
				m_taskList->ForceStop(false);

				//�f�o�b�N�J���[������
				ChangeDebugNodeColor(Col4(1.0f));
			}

			void Plowling::DefineTask() {
				auto enemy = GetOwner();

				//�ړ�
				m_taskList->DefineTask(TaskEnum::Move, make_shared<Task_PlowlingMove>(enemy, m_paramPtr->moveParamPtr));

				//�ҋ@
				m_taskList->DefineTask(TaskEnum::Wait, make_shared<Task::Wait>(m_paramPtr->waitParamPtr));
			}

			void Plowling::SelectTask() {
				TaskEnum tasks[] = {
					TaskEnum::Move,
					TaskEnum::Wait
				};

				for (const auto& task : tasks) {
					m_taskList->AddTask(task);
				}
			}

			void Plowling::NextIndex() {
				m_index += m_addIndexDirect; //�J�E���g��i�߂�B

				if (m_index >= m_paramPtr->positions.size() || m_index < 0) {
					m_addIndexDirect *= -1;      //�J�E���g��i�߂�������t�ɂ���B
					m_index += m_addIndexDirect; //�J�E���g��i�߂�B
				}
			}

			void Plowling::NextPosition() {
				//DebugDraw�Ō��̐F�ɖ߂��B
				ChangeDebugNodeColor(Col4(1.0f));

				NextIndex();
				Vec3 targetPosition = m_paramPtr->positions[m_index];
				ChangeMoveTargetPosition(targetPosition);  //�ړ��ꏊ��ύX����B
			}

			void Plowling::ChangeMoveTargetPosition(const Vec3& position) {
				auto& moveParamPtr = m_paramPtr->moveParamPtr->moveParamPtr;
				moveParamPtr->startPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();
				moveParamPtr->endPosition = position;

				//�����\�����������肷��B
				m_arriveTimer->ResetTimer(CalculateArriveTime(position, moveParamPtr->speed));

				//DebugDraw�ŖړI�n�̐F��ύX����B
				ChangeDebugNodeColor(Col4(1.0f, 0.0f, 0.0f, 1.0f));
			}

			void Plowling::CalculateAstarRamdomPosition() {
				//Astar����|�W�V�����Q���擾
				auto impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap();
				if (!impactMap) {
					DebugObject::AddString(L"ImpactMap����`����Ă��܂���B");
					return;
				}

				auto astar = impactMap->GetGraphAstar();
				auto positions = astar->CalculateRandomRoute(GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition());

				for (auto& position : positions) {
					position.y = 0.0f;
				}

				m_paramPtr->positions = positions;
			}

			float Plowling::CalculateArriveTime(const Vec3& targetPosition, const float& speed) const {
				auto selfPosition = GetOwner()->GetGameObject()->GetComponent<Transform>()->GetPosition();
				auto toTargetVec = targetPosition - selfPosition;
				constexpr float GraceTime = 1.0f;   //�P�\����

				//���� = ���� / ���x
				float time = toTargetVec.length() / speed;

				return time + GraceTime;
			}

			//�f�o�b�O------------------------------------------------------------------------------------------------------------------

			void Plowling::ChangeDebugNodeColor(const Col4& color) {
				if (auto impactMap = maru::FieldImpactMap::GetInstance()->GetImpactMap()) {
					auto node = UtilityAstar::SearchNearNode(*impactMap->GetGraphAstar().get(), m_paramPtr->positions[m_index]);
					if (node) {
						//impactMap->ChangeTargetNodeColor(node->GetIndex(), color);
					}
				}
			}

		}
	}
}