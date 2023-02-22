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

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	struct Task_MovePositions_Parametor;
	template<class T>
	class TaskList;

	namespace Enemy {
		class EnemyBase;
	}

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	MoveAstar�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct MoveAstar_Ex_Parametor
		{
			std::shared_ptr<Task_MovePositions_Parametor> movePositionsParam;

			MoveAstar_Ex_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N
		//--------------------------------------------------------------------------------------
		class MoveAstar_Ex : public TaskNodeBase<Enemy::EnemyBase>
		{
		public:
			using Parametor = MoveAstar_Ex_Parametor;

		private:
			enum class TaskEnum {
				Move,	//�ړ�
			};

			const Parametor* const m_param;					//�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;	//�^�X�N���X�g

		public:
			MoveAstar_Ex(
				const std::shared_ptr<Enemy::EnemyBase>& owner,
				const Parametor* const parametor
			);

			virtual ~MoveAstar_Ex() = default;

			void OnCreate() override;

			void OnStart() override;

			bool OnUpdate() override;
			
			void OnExit() override;

		private:
			/// <summary>
			/// �^�X�N�̒�`
			/// </summary>
			void DefineTask();

			/// <summary>
			/// �^�X�N�̑I��
			/// </summary>
			void SelectTask();
			
		public:
			bool IsEnd() const;

		};

	}

}