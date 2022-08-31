
/*!
@file SubAnimatorBase.h
@brief SubAnimatorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	namespace maru {
		template<class node_type, class StateEnum, class TransitionMember>
		class AnimationMainStateMachine;
	}

	//--------------------------------------------------------------------------------------
	///	�T�u�A�j���[�^�[�̊��N���X
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionStructMember>
	class SubAnimatorBase : public maru::I_AnimationStateNode
	{
	public:
		using NodeType = node_type;
		using StateEnum = EnumType;
		using TransitionMember = TransitionStructMember;
		using StateMachine = maru::AnimationMainStateMachine<NodeType, EnumType, TransitionMember>;

	private:
		ex_weak_ptr<GameObject> m_owner;						//���L��

	protected:
		std::unique_ptr<StateMachine> m_stateMachine = nullptr;	//�X�e�[�g�}�V��

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		SubAnimatorBase(const std::shared_ptr<GameObject>& objPtr)
			:m_owner(objPtr), m_stateMachine(new StateMachine())
		{ }
		
		virtual void OnStart() override final {

		}

		virtual bool OnUpdate() override final {
			m_stateMachine->OnUpdate();
			return false;
		}

		virtual void OnExit() override final{
			//�X�e�[�g�}�V���̏��������K�v
			m_stateMachine->ResetState();
		}

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
		/// ���݂̃X�e�[�g���擾
		/// </summary>
		/// <returns>���݂̃X�e�[�g</returns>
		StateEnum GetCurrentState() const {
			return m_stateMachine->GetCurrentState();
		}

		/// <summary>
		/// ���ݎg���Ă���m�[�h�̎擾
		/// </summary>
		/// <returns>���ݎg���Ă���m�[�h</returns>
		auto GetCurrentNode() const {
			return m_stateMachine->GetNowNode();
		}

		std::shared_ptr<maru::AnimationClip> GetAnimationClip() const override {
			auto currentNode = m_stateMachine->GetNowNode();
			return currentNode ? currentNode->GetAnimationClip() : nullptr;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<GameObject> GetGameObject() const {
			return m_owner.GetShard();
		}

	};

}