
/*!
@file StatorBase.h
@brief StatorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/StateMachine/StateMachine.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class EnumType, class OwnerType, class TransitionStructMember>
		class StateMachine;
	}

	namespace Enemy {
		
		//--------------------------------------------------------------------------------------
		///	EnumType�����ɒ�`�������C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		template<class EnumType>
		class I_Stator_EnumType 
		{
		public:
			virtual ~I_Stator_EnumType() = default;

			/// <summary>
			/// �X�e�[�g�̐؂�ւ�
			/// </summary>
			/// <param name="type">�X�e�[�g��EnumType</param>
			/// <param name="priority">�D��x</param>
			virtual void ChangeState(const EnumType& state, const int& priority = 0) = 0;

			/// <summary>
			/// �X�e�[�g�̋����؂�ւ�
			/// </summary>
			/// <param name="state">�X�e�[�g</param>
			virtual void ForceChangeState(const EnumType state) = 0;

			/// <summary>
			/// ���ݎg�p���̃X�e�[�g�^�C�v���擾
			/// </summary>
			/// <returns>�X�e�[�g�^�C�v</returns>
			virtual EnumType GetCurrentState() const = 0;

			/// <summary>
			/// ���݂̃X�e�[�g�^�C�v���w�肵���^�C�v���ǂ�����Ԃ��B
			/// </summary>
			/// <param name="type">�m�F�������X�e�[�g�^�C�v</param>
			/// <returns>�w�肵���^�C�v�Ȃ�true</returns>
			virtual bool IsCurrentState(const EnumType& type) const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	TransitiomMember�����ɒ�`�������C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		template<class TransitionMember>
		class I_Stator_TransitionType
		{
		public:
			virtual ~I_Stator_TransitionType() = default;

			/// <summary>
			/// �J�ڏ����p�̃����o�[���擾
			/// </summary>
			/// <returns>�J�ڏ����p�̃����o�[</returns>
			virtual TransitionMember& GetTransitionMember() const = 0;
		};

		//--------------------------------------------------------------------------------------
		///	�X�e�[�^�[�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		template<class EnumType, class TransitionMember>
		class I_Stator : public I_Stator_EnumType<EnumType>, public I_Stator_TransitionType<TransitionMember>
		{
			//virtual ~I_Stator() = default;
		};

		//--------------------------------------------------------------------------------------
		///	�X�e�[�^�[�̊��N���X
		//--------------------------------------------------------------------------------------
		template<class NodeType, class EnumType, class TransitionMember>
		class StatorBase : public Component, public I_Stator<EnumType, TransitionMember>
		{
		public:
			using StateMachine = maru::StateMachine<NodeType, EnumType, TransitionMember>;

		protected:
			std::unique_ptr<StateMachine> m_stateMachine = nullptr;  //�X�e�[�g�}�V��

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			StatorBase(const std::shared_ptr<GameObject>& objPtr)
				: StatorBase(objPtr, TransitionMember())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="member">�J�ڃ����o�[</param>
			StatorBase(const std::shared_ptr<GameObject>& objPtr, const TransitionMember& member)
				: Component(objPtr), m_stateMachine(new StateMachine(member))
			{}

			virtual ~StatorBase() = default;

		protected:
			/// <summary>
			/// �m�[�h�̐���
			/// </summary>
			virtual void CreateNode() = 0;

			/// <summary>
			/// �G�b�W�̐���
			/// </summary>
			virtual void CreateEdge() = 0;

		public:

			void OnCreate() override final{
				CreateNode();
				CreateEdge();
			}

			void OnUpdate() override final{
				m_stateMachine->OnUpdate();
			}

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �X�e�[�g�̕ύX
			/// </summary>
			/// <param name="state">�ύX�������X�e�[�g</param>
			/// <param name="priority">�D��x</param>
			void ChangeState(const EnumType& state, const int& priority = 0) override final{
				m_stateMachine->ChangeState(state, priority);
			}

			/// <summary>
			/// �X�e�[�g�̋����ύX
			/// </summary>
			/// <param name="state">�ύX�������X�e�[�g</param>
			void ForceChangeState(const EnumType state) {
				m_stateMachine->ForceChangeState(state);
			}

			/// <summary>
			/// ���݂̃X�e�[�g�^�C�v���擾
			/// </summary>
			/// <returns>���݂̃X�e�[�g�^�C�v</returns>
			EnumType GetCurrentState() const override final{
				return m_stateMachine->GetCurrentNodeType();
			}

			/// <summary>
			/// �w�肵���X�e�[�g�̏�Ԃ��ǂ�����Ԃ��B
			/// </summary>
			/// <param name="type">�m�F�������X�e�[�g</param>
			/// <returns>�w�肵���X�e�[�g�Ȃ�true</returns>
			bool IsCurrentState(const EnumType& type) const override final {
				return m_stateMachine->IsCurrentNodeType(type);
			}

			/// <summary>
			/// �J�ڏ��������o�[���擾����B
			/// </summary>
			/// <returns>�J�ڏ��������o�[</returns>
			TransitionMember& GetTransitionMember() const override final{
				return m_stateMachine->GetTransitionStructMember();
			}
		};

	}
}