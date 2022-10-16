/*!
@file Task_SearchBall.h
@brief Task_SearchBall�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Behavior/Interface/I_Task.h"

namespace basecross {
	
	template<class T>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
	}

	namespace maru {

		namespace Behavior {

			namespace Task {

				enum class TaskEnum {
					Move,
				};

				//--------------------------------------------------------------------------------------
				///	�{�[����T���^�X�N�p�����[�^
				//--------------------------------------------------------------------------------------
				struct SearchBall_Parametor {
					std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

					SearchBall_Parametor();
				};

				//--------------------------------------------------------------------------------------
				///	�{�[����T���^�X�N
				//--------------------------------------------------------------------------------------
				class SearchBall : public TaskBase<Enemy::EnemyBase>
				{
				public:
					using Parametor = SearchBall_Parametor;

				private:
					Parametor m_param;								//�p�����[�^
					
					std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

					std::weak_ptr<Transform> m_transform;

				public:
					SearchBall(const std::shared_ptr<Enemy::EnemyBase>& owner);

					void OnStart() override;

					bool OnUpdate() override;

					void OnExit() override;

				private:

					void DefineTask();

					void SelectTask();

					/// <summary>
					/// �p�j����ꏊ�̔z����擾
					/// </summary>
					/// <returns>�p�j����ꏊ�̔z��</returns>
					std::vector<Vec3> CalculateMovePositions();

					/// <summary>
					/// �p�j�ړ���̖ړI�n���擾
					/// </summary>
					/// <returns>�p�j�ړ�����擾</returns>
					Vec3 CalculateMoveTargetPosition();

					/// <summary>
					/// �p�����[�^�̏�����
					/// </summary>
					void InitializeParametor();
				};

			}

		}
	}
}