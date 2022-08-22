/*!
@file Task_MovePositions.h
@brief Task_MovePositions
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Task_ToTargetMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �p�j�ړ�����^�X�N�̃p�����[�^
	//--------------------------------------------------------------------------------------
	struct Task_MovePositions_Parametor{
		std::vector<Vec3> positions;  //�p�j����ړI�n�Q
		std::shared_ptr<Task::ToTargetMove::Parametor> moveParamPtr;
		bool isSetInitializePosition; //�����ݒ�̃|�W�V�����ɏu�Ԉړ������邩�ǂ���

		Task_MovePositions_Parametor();
		Task_MovePositions_Parametor(const Task::ToTargetMove::Parametor& moveParametor);
		Task_MovePositions_Parametor(const std::shared_ptr<Task::ToTargetMove::Parametor>& moveParamPtr);
	};

	//--------------------------------------------------------------------------------------
	/// �p�j�ړ�����^�X�N
	//--------------------------------------------------------------------------------------
	class Task_MovePositions : public TaskNodeBase<GameObject>
	{
	public:
		using Parametor = Task_MovePositions_Parametor;

		/// <summary>
		/// �^�X�N�^�C�v
		/// </summary>
		enum class TaskEnum {
			Move,  //�ړ�
		};

		ex_weak_ptr<Parametor> m_paramPtr;               //�p�����[�^
		std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //�^�X�N�Ǘ��N���X

		int m_index = 0;                                 //���݂̖ړI�n�̃C���f�b�N�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="owner">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="paramPtr">�p�����[�^</param>
		Task_MovePositions(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

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

		/// <summary>
		/// ���̖ړI�n�ɕύX
		/// </summary>
		void NextPosition();

		/// <summary>
		/// �ړI�n��ύX
		/// </summary>
		/// <param name="position">�ύX�������ړI�n</param>
		void ChangeMoveTargetPosition(const Vec3& position);

	public:
		//--------------------------------------------------------------------------------------
		/// �A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �I�����f
		/// </summary>
		/// <returns>�ړ��悪�Ȃ��Ȃ�true</returns>
		bool IsEnd();
 	};

}

//endbasecross