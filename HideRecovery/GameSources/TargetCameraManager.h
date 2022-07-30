/*!
@file TargetCameraManager.h
@brief TargetCameraManager�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Task_ToTargetMove.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class MulchTaskList;

	//--------------------------------------------------------------------------------------
	///	�^�[�Q�b�g�J�����Ǘ��N���X
	//--------------------------------------------------------------------------------------
	class TargetCameraManager : public Component
	{
		/// <summary>
		/// �^�X�N�^�C�v
		/// </summary>
		enum class TaskEnum
		{
			AtMove,   //���_�ړ�
			EyeMove,  //�ʒu�ړ�
		};

		std::unique_ptr<MulchTaskList<TaskEnum>> m_mulchTaskList = nullptr; //�}���`�^�X�N�N���X

		std::shared_ptr<Task_ToTargetMove::Parametor> m_eyeParamPtr;        //�^�X�N�̈ړ��p�����[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		TargetCameraManager(const std::shared_ptr<GameObject>& objPtr);

		void OnCreate() override;
		void OnUpdate() override;

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
		/// <summary>
		/// �^�X�N�̊J�n
		/// </summary>
		void StartTask();

	};

}

//endbasecross