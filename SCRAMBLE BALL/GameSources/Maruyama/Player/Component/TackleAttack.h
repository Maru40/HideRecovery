
/*!
@file TackleAttack.h
@brief TackleAttack�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "AttackBase.h"
#include "Maruyama/TaskList/TaskList.h"
#include "Maruyama/Interface/I_Damaged.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class PlayerStatus;
	class PlayerAnimator;
	class VelocityManager;
	class GameTimer;
	struct CollisionPair;

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
		std::weak_ptr<PlayerStatus> m_status;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

		virtual ~TackleAttack() = default;

		void OnCreate() override;
		void OnLateStart() override;
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

		/// <summary>
		/// �����U���J�n
		/// </summary>
		void ForceStartAttack();

		/// <summary>
		/// �^�b�N�������ǂ���
		/// </summary>
		bool IsTackle();

		/// <summary>
		/// �^�X�N�̋����I��
		/// </summary>
		void ForceTaskReset();
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
			std::weak_ptr<PlayerAnimator> m_animator;

		public:
			Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~Preriminary_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			void PlayAnimation();
		};

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U��������̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct Attack_Tackle_Parametor {
			DamageData damageData;	//�_���[�W�f�[�^
			float tackleSpeed;		//�^�b�N���X�s�[�h

			Attack_Tackle_Parametor(const DamageData& damageData);
		};

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U��������
		//--------------------------------------------------------------------------------------
		class Attack_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = Attack_Tackle_Parametor;

		private:
			Parametor m_param;	//�p�����[�^

			std::weak_ptr<PlayerAnimator> m_animator;			//�A�j���[�^
			std::weak_ptr<VelocityManager> m_velocityManager;	//���x�Ǘ�

			bool m_isCollision;	//�����蔻����Ƃ邩�ǂ���

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			Attack_Tackle(const std::shared_ptr<GameObject>& objPtr);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="parametor">�p�����[�^</param>
			Attack_Tackle(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

			virtual ~Attack_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			void PlayAnimation();

			void CollisionEnter(const CollisionPair& pair);
		};

		//--------------------------------------------------------------------------------------
		/// �^�b�N���U���̍U����d��
		//--------------------------------------------------------------------------------------
		class EndWait_Tackle : public TaskNodeBase<GameObject>
		{
			std::unique_ptr<GameTimer> m_timer;
			std::weak_ptr<VelocityManager> m_velocityManager;

			float m_waitTime = 0.0f;

		public:
			EndWait_Tackle(const std::shared_ptr<GameObject>& objPtr, const float waitTime = 0.5f);

			virtual ~EndWait_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};
	}

}