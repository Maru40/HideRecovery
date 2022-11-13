/*!
@file ToBallRunTask.h
@brief ToBallRunTask�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {

	template<class T>
	class TaskList;

	class TargetManager;
	class VelocityManager;
	class RotationController;
	class EyeSearchRange;
	class OnlinePlayerSynchronizer;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
		class I_FactionMember;
	}

	namespace Task {
		struct MoveAstar_Parametor;
		struct ToTargetMove_Parametor;
		struct Wait_Parametor;
	}

	namespace TaskListNode {
		struct TargetSeek_Parametor;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				//--------------------------------------------------------------------------------------
				///	�{�[���֌������^�X�N�̃p�����[�^
				//--------------------------------------------------------------------------------------
				struct ToBallRunTask_Parametor {
					basecross::Task::MoveAstar_Parametor* moveAstarParam;
					TaskListNode::TargetSeek_Parametor* targetSeekParam;
					std::shared_ptr<basecross::Task::Wait_Parametor> waitParam;

					ToBallRunTask_Parametor();

					virtual ~ToBallRunTask_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	�{�[���֌������^�X�N
				//--------------------------------------------------------------------------------------
				class ToBallRunTask : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = ToBallRunTask_Parametor;

					enum class TaskEnum {
						MoveAstar,
						MoveArrive,
						GetBall,
						//Wait,
					};

				private:
					Parametor m_param;								//�p�����[�^

					std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

					std::weak_ptr<Transform> m_transform;							//�g�����X�t�H�[��
					std::weak_ptr<EyeSearchRange> m_eyeRange;						//���E�Ǘ�
					std::weak_ptr<TargetManager> m_targetManager;					//�^�[�Q�b�g�Ǘ�
					std::weak_ptr<VelocityManager> m_velocityManager;				//���x�Ǘ�
					std::weak_ptr<RotationController> m_rotationController;			//�����Ǘ�
					std::weak_ptr<Enemy::I_FactionMember> m_factionMember;			//�t�@�N�V���������o�[
					std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSynchronizer;	//�I�����C���V���N��

				public:
					ToBallRunTask(const std::shared_ptr<Enemy::EnemyBase>& owner);

					virtual ~ToBallRunTask() = default;

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:

					/// <summary>
					/// �^�[�Q�b�g�̎擾
					/// </summary>
					/// <returns>�^�[�Q�b�g</returns>
					std::shared_ptr<GameObject> CalculateTarget();

					/// <summary>
					/// ����MoveArrive�^�X�N�ɕύX���邩���Ď�����B
					/// </summary>
					void CheckForceNextMoveArriveTask();

					/// <summary>
					/// �^�X�N�̒�`
					/// </summary>
					void DefineTask();

					/// <summary>
					/// �ҋ@���̃^�X�N�̒�`
					/// </summary>
					void DefineWaitTask();

					/// <summary>
					/// �^�X�N�̑I��
					/// </summary>
					void SelectTask();

					/// <summary>
					/// �p�����[�^�̏�����
					/// </summary>
					void InitializeParametor();

					/// <summary>
					/// ��������
					/// </summary>
					void Rotation();

				public:

					bool IsEnd() const;
				};

			}
		}
	}
}