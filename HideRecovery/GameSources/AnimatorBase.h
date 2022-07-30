
/*!
@file AnimatorBase.h
@brief AnimatorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine;
	}

	//--------------------------------------------------------------------------------------
	///	�A�j���[�^�[�̊��N���X
	//--------------------------------------------------------------------------------------
	template<class NodeType, class StateEnum, class TransitionStructMember>
	class AnimatorBase : public Component
	{
	public:
		using StateMachine = maru::AnimationMainStateMachine<NodeType, StateEnum, TransitionStructMember>;

	protected:
		std::unique_ptr<StateMachine> m_stateMachine = nullptr; //�X�e�[�g�}�V��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		AnimatorBase(const std::shared_ptr<GameObject>& objPtr)
			: Component(objPtr), m_stateMachine(new StateMachine(TransitionStructMember()))
		{}

		void OnCreate() override final {
			CreateNode();
			CreateEdge();
		}

		void OnUpdate() override final {
			m_stateMachine->OnUpdate();
		}

	private:
		/// <summary>
		/// �m�[�h�̐���
		/// </summary>
		virtual void CreateNode() = 0;

		/// <summary>
		/// �G�b�W�̐���
		/// </summary>
		virtual void CreateEdge() = 0;

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �X�e�[�g�̕ύX
		/// </summary>
		/// <param name="state">�ύX�������X�e�[�g</param>
		/// <param name="priority">�D��x</param>
		void ChangeState(const StateEnum state, const int& priority = 0) {
			m_stateMachine->ChangeState(state, priority);
		}

		/// <summary>
		/// �����X�e�[�g�ύX
		/// </summary>
		/// <param name="state">�ύX�������X�e�[�g</param>
		/// <param name="priority">�D��x</param>
		void ForceChangeState(const StateEnum& state, const int& priority = 0) {
			m_stateMachine->ForceChangeState(state);
		}

		/// <summary>
		/// ���݂̃X�e�[�g�^�C�v���擾
		/// </summary>
		/// <returns>���݂̃X�e�[�g�^�C�v</returns>
	    StateEnum GetCurrentState() const {
			return m_stateMachine->GetCurrentState();
		}

		/// <summary>
		/// ���ݎg�p���̃m�[�h���擾
		/// </summary>
		/// <returns>���ݎg�p���̃m�[�h</returns>
		auto GetCurrentNode() const {
			return m_stateMachine->GetNowNode();
		}

	};

}