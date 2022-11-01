/*!
@file EnemyMainStateMachine.h
@brief EnemyMainStateMachine
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "StateNodeBase.h"
#include "StateEdgeBase.h"
#include "Maruyama/Enemy/Astar/GraphBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionStructMember>
	class EnemyMainStateMachine;

	/// <summary>
	/// EnemyStateMachine�p�̃e���v���[�g�N���X�B
	/// </summary>
	/// <typeparam name="NodeType">�g�p����m�[�h</typeparam>
	/// <typeparam name="EnumType">�g�p����񋓑�</typeparam>
	/// <typeparam name="TransitionStructMember">�J�ڏ����p�̍\���̃����o�[</typeparam>
	template<class node_type, class EnumType, class TransitionStructMember>
	class EnemyMainStateMachine : public GraphBase<StateNodeBase<node_type>, EnumType, TransitionStructMember>
	{
		//--------------------------------------------------------------------------------------
		///	�J�ڐ���̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct TransitionCandidateParametor
		{
			EnumType type;    //�؂�ւ���^�C�v
			int priority;     //�D��x

			/// <summary>
			/// �J�ڐ���p�����[�^
			/// </summary>
			/// <param name="type">�J�ڃ^�C�v</param>
			TransitionCandidateParametor(const EnumType type)
				:TransitionCandidateParametor(type)
			{}

			/// <summary>
			/// �J�ڐ���p�����[�^
			/// </summary>
			/// <param name="type">�J�ڃ^�C�v</param>
			/// <param name="priority">�D��x</param>
			TransitionCandidateParametor(const EnumType type, const int priority)
				:type(type), priority(priority)
			{}
		};

	public:
		//�O������Enum���g�����p��EnumType��public�ɂ���B
		using StateEnum = EnumType;

	private:
		EnumType m_currentNodeType = EnumType(0);  //���݂̃m�[�h�^�C�v

		TransitionStructMember m_transitionStruct; //�J�ڏ����ɗ��p����\����

		std::vector<TransitionCandidateParametor> m_transitionCandidates;  //�J�ڐ���p�����[�^�Q

		/// <summary>
		/// �X�e�[�g�̕ύX
		/// </summary>
		/// <param name="type">�ύX�������X�e�[�g�^�C�v</param>
		void ChangeState(const EnumType type) {
			std::shared_ptr<NodeType> nowNode = GetNowNode();
			if (nowNode) {
				nowNode->OnExit();
			}

			m_currentNodeType = type;
			std::shared_ptr<NodeType> nextNode = GetNode(m_currentNodeType);
			if (nextNode) {
				nextNode->OnStart();
			}
		}

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		EnemyMainStateMachine() 
			:EnemyMainStateMachine(TransitionStructMember())
		{}

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="member">�J�ڏ����p�����o</param>
		EnemyMainStateMachine(const TransitionStructMember& member)
			:GraphBase(), m_transitionStruct(member)
		{}

		virtual ~EnemyMainStateMachine() = default;

		/// <summary>
		/// �m�[�h�̒ǉ�
		/// </summary>
		/// <param name="type">�m�[�h��EnumType</param>
		/// <param name="node">�ǉ�����m�[�h</param>
		void AddNode(const EnumType type, const std::shared_ptr<NodeType>& node) override {
			if (IsEmpty()) {
				m_currentNodeType = type;
				if (node) {
					node->OnStart();
				}
			}

			GraphBase::AddNode(type, node);
		}

		/// <summary>
		/// ���ݎg�p���̃m�[�hEnumType���擾
		/// </summary>
		/// <returns>�m�[�h��EnumType</returns>
		EnumType GetCurrentNodeType() const noexcept {
			return m_currentNodeType;
		}

		/// <summary>
		/// ���ݎg�p���̃m�[�h�^�C�v�������̃^�C�v�ƍ����Ă��邩�ǂ���
		/// </summary>
		/// <param name="type">�m�F�������m�[�h�^�C�v</param>
		/// <returns>�����Ȃ�true</returns>
		bool IsCurrentNodeType(const EnumType& type) const {
			return GetCurrentNodeType() == type;
		}

		/// <summary>
		/// ���ݎg�p���̃m�[�h���擾
		/// </summary>
		/// <returns>�m�[�h�̎擾</returns>
		std::shared_ptr<NodeType> GetNowNode() const {
			return GetNode(m_currentNodeType);
		}

		/// <summary>
		/// ���ݎg�p���̃m�[�h�ɐݒ肵�Ă���G�b�W���擾
		/// </summary>
		/// <returns>�G�b�W�̎擾</returns>
		EdgeVector GetNowNodeEdges() const {
			return GetEdges(m_currentNodeType);
		}

		/// <summary>
		/// �J�ڂɗ��p����\���̂��擾����B
		/// </summary>
		/// <returns>�\���̂̎Q�Ƃ�n��</returns>
		TransitionStructMember& GetTransitionStructMember() {
			return m_transitionStruct;
		}

		/// <summary>
		/// �X�e�[�g�̐؂�ւ�
		/// </summary>
		/// <param name="type">�؂�ւ������X�e�[�g</param>
		/// <param name="priority">�D��x</param>
		void ChangeState(const EnumType& type, const int& priority) {
			m_transitionCandidates.push_back(TransitionCandidateParametor(type, priority));
		}

		/// <summary>
		/// �����I�ȃX�e�[�g�̐؂�ւ��B
		/// </summary>
		/// <param name="type">�����I�ȃX�e�[�g�̐؂�ւ�</param>
		void ForceChangeState(const EnumType& type) {
			ChangeState(type);
		}

		/// <summary>
		/// �X�V����
		/// </summary>
		void OnUpdate() {
			if (IsEmpty()) {
				return;
			}

			//�m�[�h��Update
			bool isEndNodeUpdate = NodeUpdate();

			//�G�b�W�̐ؑ֔��f
			TransitionCheck(isEndNodeUpdate);

			//�g���K�[�̃��Z�b�g
			TriggerReset();

			//�J��
			Transition();

			//�J�ڐ���̃N���A
			m_transitionCandidates.clear();
		}

	private:
		//�v���C�x�[�g�֐�----------------------------------------------------------------------------

		/// <summary>
		/// �m�[�h�̃A�b�v�f�[�g
		/// </summary>
		/// <returns>�m�[�h���I�����Ă��邩�ǂ���</returns>
		bool NodeUpdate() {
			auto node = GetNowNode();
			if (node) {
				return node->OnUpdate();
			}

			return false;
		}

		/// <summary>
		/// �J�ڃ`�F�b�N
		/// </summary>
		void TransitionCheck(const bool isEndNodeUpdate) {
			auto edges = GetNowNodeEdges();
			for (auto& edge : edges) {
				if (edge->IsTransition(m_transitionStruct, isEndNodeUpdate)) {
					const auto type = edge->GetToType();
					const auto priority = edge->GetPriority();
					m_transitionCandidates.push_back(TransitionCandidateParametor(type, priority));
				}
			}
		}

		/// <summary>
		/// �g���K�[�̃��Z�b�g
		/// </summary>
		void TriggerReset() {
			EdgeVectorMap edgesMap = GetEdgesMap();
			for (std::pair<EnumType, EdgeVector> edgePair : edgesMap) {
				for (auto& edge : edgePair.second) {
					edge->IsTransition(m_transitionStruct);
				}
			}
		}

		/// <summary>
		/// �J�ڏ���
		/// </summary>
		void Transition() {
			if (m_transitionCandidates.size() == 0) {  //�J�ڐ��₪0�Ȃ珈�����΂��B
				return;
			}

			//�J�ڐ���̃\�[�g
			std::sort(m_transitionCandidates.begin(), m_transitionCandidates.end(),
				[](const TransitionCandidateParametor& left, const TransitionCandidateParametor& right) 
				{ return left.priority > right.priority ? true : false; }); //�D��x������������J�ڂ���B
			
			ChangeState(m_transitionCandidates[0].type);
		}
	};

}