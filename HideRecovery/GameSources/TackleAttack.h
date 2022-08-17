
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
			EndWait,		//�U����̍d��
		};

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

		virtual ~TackleAttack() = default;

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
		/// �U���J�n
		/// </summary>
		void StartAttack();
	};

	//--------------------------------------------------------------------------------------
	/// �^�b�N���U���̃^�X�N
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̗\������
		//--------------------------------------------------------------------------------------
		class Preriminary_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~Preriminary_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U��������
		//--------------------------------------------------------------------------------------
		class Attack_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			Attack_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~Attack_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U����d��
		//--------------------------------------------------------------------------------------
		class EndWait_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			EndWait_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~EndWait_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};
	}

}