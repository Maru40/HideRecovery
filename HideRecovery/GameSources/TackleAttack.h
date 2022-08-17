
/*!
@file TackleAttack.h
@brief TackleAttack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "AttackBase.h"
#include "TaskList.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	//--------------------------------------------------------------------------------------
	/// �^�b�N���U��
	//--------------------------------------------------------------------------------------
	class TackleAttack : public AttackBase
	{
		enum class TaskEnum {
			Preliminary,	//�\������
			Tackle,			//�^�b�N���U��
			Wait,			//�U����̍d��
		};

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

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
	};


	namespace Task {

		class Preriminary_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};

	}

}