/*!
@file Task_AstarMove
@brief Task_AstarMove�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	struct Task_MovePositions_Parametor;

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// Astar�ړ��^�C�v
		//--------------------------------------------------------------------------------------
		enum class AstarMove_Type
		{
			Chase,  //�u��������B
			Evade,  //������B
		};

		//--------------------------------------------------------------------------------------
		/// Astar�ړ��p�����[�^
		//--------------------------------------------------------------------------------------
		struct AstarMove_Parametor 
		{
			using MoveType = AstarMove_Type;

			MoveType moveType;													//�ړ��^�C�v
			std::shared_ptr<Task_MovePositions_Parametor> movePositonsParamPtr; //�ړ��p�����[�^

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="moveType">�ړ��^�C�v</param>
			AstarMove_Parametor(const MoveType& moveType);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="moveType">�ړ��^�C�v</param>
			/// <param name="movePositionsParamPtr">�ړ��p�����[�^</param>
			AstarMove_Parametor(
				const MoveType& moveType, 
				const std::shared_ptr<Task_MovePositions_Parametor>& movePositionsParamPtr);
		};

		//--------------------------------------------------------------------------------------
		/// Astar�𗘗p�����ړ��^�X�N
		//--------------------------------------------------------------------------------------
		class AstarMove : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = AstarMove_Parametor;
			using MoveType = AstarMove_Type;

			/// <summary>
			/// �^�X�N�^�C�v
			/// </summary>
			enum class TaskEnum {
				Move,   //�ړ�
			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr;               //�p�����[�^

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //�^�X�N�Ǘ��N���X

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="paramPtr">�p�����[�^</param>
			AstarMove(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �����X�V
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

			/// <summary>
			/// �ړ����[�g���Z�b�g����B
			/// </summary>
			void SetRoutePositions();

		public:

		};

	}

}

//endbasecross