/*!
@file StateEdgeBase.h
@brief StateEdgeBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/GraphEdgeBase.h"

namespace basecross {

	/// <summary>
	/// �G�b�W�̊��N���X
	/// </summary>
	/// <typeparam name="EnumType">�g�p����񋓑�</typeparam>
	/// <typeparam name="TransitionStructMember">�J�ڏ����p�̍\���̃����o�[</typeparam>
	template<class EnumType, class TransitionStructMember>
	class StateEdgeBase : public GraphEdgeBase
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
		StateEdgeBase(const EnumType from, const EnumType to)
			:StateEdgeBase(from, to, nullptr)
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <param name="to">��̃C���f�b�N�X</param>
		/// <param name="isTransitionFunc">�J�ڏ����֐�</param>
		StateEdgeBase(
			const EnumType from, 
			const EnumType to,
			const IsTransitionFunction& isTransitionFunc
		) :
			StateEdgeBase(from, to, isTransitionFunc, static_cast<int>(to))
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <param name="to">��̃C���f�b�N�X</param>
		/// <param name="isTransitionFunc">�J�ڏ����֐�</param>
		/// <param name="priority">�D��x</param>
		StateEdgeBase(
			const EnumType from, 
			const EnumType to,
			const IsTransitionFunction& isTransitionFunc,
			const int priority
		) :
			StateEdgeBase(from, to, isTransitionFunc, static_cast<int>(to), false)
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <param name="to">��̃C���f�b�N�X</param>
		/// <param name="isTransitionFunc">�J�ڏ����֐�</param>
		/// <param name="priority">�D��x</param>
		/// <param name="isEndTransition">�X�V�����I�����ɔ��f���邩�ǂ���</param>
		StateEdgeBase(
			const EnumType from, 
			const EnumType to,
			const IsTransitionFunction& isTransitionFunc,
			const int priority,
			const bool isEndTransition
		) :
			GraphEdgeBase((int)from, (int)to), 
			m_isTransitionFunc(isTransitionFunc), 
			m_priority(priority), 
			m_isEndTransition(isEndTransition)
		{}

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
		/// ��O�̃m�[�h�^�C�v���擾
		/// </summary>
		/// <returns>��O�̃m�[�h�^�C�v</returns>
		EnumType GetFromType() const {
			return static_cast<EnumType>(GetFrom());
		}

		/// <summary>
		/// ��̃m�[�h�^�C�v���擾
		/// </summary>
		/// <returns>��̃m�[�h�^�C�v</returns>
		EnumType GetToType() const {
			return static_cast<EnumType>(GetTo());
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

//endbasecross