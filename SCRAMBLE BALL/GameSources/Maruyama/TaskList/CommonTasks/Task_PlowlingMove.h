/*!
@file Task_PlowlingMove.h
@brief Task_PlowlingMove�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

#include "Task_ToTargetMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class TargetManager;

	template<class T>
	class TaskList;

	namespace Enemy {
		class EnemyBase;

		//--------------------------------------------------------------------------------------
		/// �X���C���̜p�j�ړ��^�X�N
		//--------------------------------------------------------------------------------------
		class Task_PlowlingMove : public TaskNodeBase<EnemyBase>
		{
		public:
			/// <summary>
			/// �^�X�N�^�C�v
			/// </summary>
			enum class TaskEnum;

			/// <summary>
			/// �p�����[�^
			/// </summary>
			struct Parametor
			{
				std::shared_ptr<Task::ToTargetMove::Parametor> moveParamPtr; //�ړ��p�����[�^

				Parametor();

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="moveParamPtr">�ړ��p�����[�^</param>
				Parametor(const std::shared_ptr<Task::ToTargetMove::Parametor>& moveParamPtr);
			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr;              //�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList; //�^�X�N�Ǘ��N���X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����N���X</param>
			/// <param name="paramPtr">�p�����[�^</param>
			Task_PlowlingMove(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �����̍X�V
			/// </summary>
			void Rotation();

			/// <summary>
			/// �^�X�N�̒�`
			/// </summary>
			void DefineTask();

			/// <summary>
			/// �^�X�N�̑I��
			/// </summary>
			void SelectTask();
		};

	}
}