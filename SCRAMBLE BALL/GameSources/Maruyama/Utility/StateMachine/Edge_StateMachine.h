
/*!
@file Edge_StateMachine.h
@brief Edge_StateMachine
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "EdgeBase.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;

		//--------------------------------------------------------------------------------------
		/// �X�e�[�g�}�V���p�̃G�b�W�N���X
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �X�e�[�g�}�V���p�̃G�b�W�N���X
		/// </summary>
		/// <typeparam name="EnumType">�g�p����񋓑�</typeparam>
		/// <typeparam name="TransitionStructMember">�J�ڏ����p�̍\���̃����o�[</typeparam>
		template<class EnumType, class TransitionStructMember>
		class Edge_StateMachine : public EdgeBase
		{
		public:
			using IsTransitionFunction = std::function<bool(const TransitionStructMember& member)>;

		private:
			IsTransitionFunction m_isTransitionFunc = nullptr; //�J�ڂ������

			int m_priority = 0;             //�D��x
			bool m_isEndTransition = false; //�I�����ɑJ�ڂ��邩�ǂ���

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="from">��O�̃C���f�b�N�X</param>
			/// <param name="to">��̃C���f�b�N�X</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode
			) :
				StateEdgeBase(fromNode, toNode, nullptr)
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="from">��O�̃C���f�b�N�X</param>
			/// <param name="to">��̃C���f�b�N�X</param>
			/// <param name="isTransitionFunc">�J�ڏ����֐�</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode,
				const IsTransitionFunction& isTransitionFunc
			) :
				StateEdgeBase(fromNode, toNode, isTransitionFunc, static_cast<int>(to))
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="from">��O�̃C���f�b�N�X</param>
			/// <param name="to">��̃C���f�b�N�X</param>
			/// <param name="isTransitionFunc">�J�ڏ����֐�</param>
			/// <param name="priority">�D��x</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode,
				const IsTransitionFunction& isTransitionFunc,
				const int priority
			) :
				Edge_StateMachine(fromNode, toNode, isTransitionFunc, toNode->GetIndex(), false)
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="from">��O�̃C���f�b�N�X</param>
			/// <param name="to">��̃C���f�b�N�X</param>
			/// <param name="isTransitionFunc">�J�ڏ����֐�</param>
			/// <param name="priority">�D��x</param>
			/// <param name="isEndTransition">�X�V�����I�����ɔ��f���邩�ǂ���</param>
			Edge_StateMachine(
				const std::shared_ptr<NodeBase>& fromNode,
				const std::shared_ptr<NodeBase>& toNode,
				const IsTransitionFunction& isTransitionFunc,
				const int priority,
				const bool isEndTransition
			) :
				EdgeBase(fromNode, toNode),
				m_isTransitionFunc(isTransitionFunc),
				m_priority(priority),
				m_isEndTransition(isEndTransition)
			{}

			virtual ~Edge_StateMachine() = default;

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// To�ɑJ�ڂ��������ݒ肷��B
			/// </summary>
			/// <param name="func">�ݒ肷�����</param>
			void SetIsToTransition(const std::function<bool(const TransitionStructMember& member)>& func) {
				m_isTransitionFunc = func;
			}

			/// <summary>
			/// �J�ڂł��邩�ǂ���
			/// </summary>
			/// <param name="member">�J�ڏ����p�����o�[</param>
			/// <param name="isEndNodeUpdate">�X�V�I�����ɔ��f���邩�ǂ���</param>
			/// <returns></returns>
			bool IsTransition(const TransitionStructMember& member, const bool isEndNodeUpdate = false) {
				//�I�����J�ڂȂ�
				if (m_isEndTransition) {
					//�m�[�h���I�����Ă���Ȃ�Ď��A�����łȂ��Ȃ�false
					return isEndNodeUpdate ? m_isTransitionFunc(member) : false;
				}

				//�I�����J�ڂłȂ��Ȃ��ɊĎ�
				return m_isTransitionFunc(member);
			}

			/// <summary>
			/// �D��x�̐ݒ�
			/// </summary>
			/// <param name="priority">�D��x</param>
			void SetPriority(const int priority) noexcept {
				m_priority = priority;
			}

			/// <summary>
			/// �D��x�̎擾
			/// </summary>
			/// <returns>�D��x</returns>
			int GetPriority() const noexcept {
				return m_priority;
			}

			_NODISCARD EnumType GetFromType() const noexcept {
				return static_cast<EnumType>(EdgeBase::GetFromIndex());
			}

			_NODISCARD EnumType GetToType() const noexcept {
				return static_cast<EnumType>(EdgeBase::GetToIndex());
			}

			/// <summary>
			/// �X�V�I�����ɑJ�ڔ��f���邩�ǂ�����ݒ�
			/// </summary>
			/// <param name="isEndTransition">�X�V�I�����ɑJ�ڔ��f���邩�ǂ���</param>
			void SetIsEndTransition(const bool isEndTransition) noexcept {
				m_isEndTransition = true;
			}

			/// <summary>
			/// �X�V�I�����ɑJ�ڔ��f���邩�ǂ������擾
			/// </summary>
			/// <returns>�X�V�I�����ɑJ�ڔ��f����Ȃ�true</returns>
			bool IsEndTransition() const noexcept {
				return m_isEndTransition;
			}
		};

	}
}