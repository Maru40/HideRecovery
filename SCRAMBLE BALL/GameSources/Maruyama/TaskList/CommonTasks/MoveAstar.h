/*!
@file MoveAstar.h
@brief MoveAstar�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	class TargetManager;

	template<class T>
	class TaskList;

	struct Task_MovePositions_Parametor;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {

		enum class TaskEnum {
			Move,
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N�p�����[�^
		//--------------------------------------------------------------------------------------
		struct MoveNearPosition_Parametor {
			std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

			MoveNearPosition_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N
		//--------------------------------------------------------------------------------------
		class MoveAstar : public basecross::TaskNodeBase<Enemy::EnemyBase>
		{
			using Parametor = MoveNearPosition_Parametor;

		private:
			Parametor m_param;								//�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

			std::queue<int> m_areaRoute;					//�m�F����G���A���[�g

			std::weak_ptr<Transform> m_transform;
			std::weak_ptr<TargetManager> m_targetManager;

		public:
			MoveAstar(const std::shared_ptr<Enemy::EnemyBase>& owner);

			virtual ~MoveAstar() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void DefineTask();

			void SelectTask();

			void NextRoute();

			/// <summary>
			/// �G���A�Ԃ̃��[�g���v�Z����B
			/// </summary>
			/// <returns>�G���A�Ԃ̃��[�g</returns>
			std::queue<int> CalculateMoveAreaRouteQueue();

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

		public:

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			bool IsEnd() const;
		};


	}
}