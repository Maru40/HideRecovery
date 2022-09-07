
/*!
@file AnimationMainStateMachine.h
@brief AnimationMainStateMachine�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "MaruAnimationHelper.h"
#include "Maruyama/Enemy/Astar/GraphBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class node_type, class EnumType, class TransitionStructMember>
	class GraphBase;

	template<class EnumType, class TransitionStructMember>
	class StateEdgeBase;

	template<class NodeType>
	class StateNodeBase;

	namespace maru {

		class AnimationClip;
		class I_Motion;

		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine;

		//--------------------------------------------------------------------------------------
		///	�A�j���[�V�����p�X�e�[�g�}�V��
		//--------------------------------------------------------------------------------------
		template<class node_type, class EnumType, class TransitionStructMember>
		class AnimationMainStateMachine : public GraphBase<I_AnimationStateNode, EnumType, TransitionStructMember>
		{
			/// <summary>
			/// �J�ڐ���̃p�����[�^
			/// </summary>
			struct TransitionCandidateParametor
			{
				EnumType type;  //�X�e�[�g�^�C�v
				int priority;   //�D��x

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="type">�X�e�[�g�^�C�v</param>
				TransitionCandidateParametor(const EnumType type)
					:TransitionCandidateParametor(type)
				{}

				/// <summary>
				/// �R���X�g���N�^
				/// </summary>
				/// <param name="type">�X�e�[�g�^�C�v</param>
				/// <param name="priority">�D��x</param>
				TransitionCandidateParametor(const EnumType type, const int priority)
					:type(type), priority(priority)
				{ }
			};

		public:
			//�O������Enum���g�����p��EnumType��public�ɂ���B
			using StateEnum = EnumType;

		private:
			EnumType m_currentNodeType;                                        //���݂̃m�[�h�^�C�v

			TransitionStructMember m_transitionStruct;                         //�J�ڏ����ɗ��p����\����

			std::vector<TransitionCandidateParametor> m_transitionCandidates;  //�J�ڐ���p�����[�^�Q

			/// <summary>
			/// �X�e�[�g�̕ύX
			/// </summary>
			/// <param name="type">�ύX����X�e�[�g�^�C�v</param>
			void ChangeState(const EnumType type) {
				auto nowNode = GetNowNode();
				if (nowNode) {
					nowNode->OnExit();
				}

				m_currentNodeType = type;
				auto nextNode = GetNode(m_currentNodeType);
				if (nextNode) {
					nextNode->OnStart();
				}
			}

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			AnimationMainStateMachine()
				:AnimationMainStateMachine(TransitionStructMember())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="member">�J�ڏ��������o�[</param>
			AnimationMainStateMachine(const TransitionStructMember& member)
				:GraphBase(), m_transitionStruct(member), m_currentNodeType(EnumType(0))
			{}

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			/// <param name="type">�ǉ��������m�[�h�^�C�v</param>
			/// <param name="node">�ǉ��������m�[�h</param>
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
			/// ���݂̃X�e�[�g���擾
			/// </summary>
			/// <returns>���݂̃X�e�[�g</returns>
			EnumType GetCurrentState() const {
				return m_currentNodeType;
			}

			/// <summary>
			/// �w�肵���X�e�[�g���ǂ����̔��f
			/// </summary>
			/// <param name="state">�m�F�������X�e�[�g</param>
			/// <returns>�w�肵���X�e�[�g�Ȃ�true</returns>
			bool IsCurrentState(const EnumType& state) const {
				return m_currentNodeType == state;
			}

			/// <summary>
			/// ���ݎg���Ă���m�[�h�̎擾
			/// </summary>
			/// <returns>���ݎg���Ă���m�[�h</returns>
			std::shared_ptr<NodeType> GetNowNode() const {
				return GetNode(m_currentNodeType);
			}

			/// <summary>
			/// ���ݎg���Ă���G�b�W�̎擾
			/// </summary>
			/// <returns>���ݎg���Ă���G�b�W</returns>
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
			void ChangeState(const EnumType& type, const int& priority)
			{
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

			/// <summary>
			/// �X�e�[�g�̃��Z�b�g
			/// </summary>
			void ResetState() {
				ForceChangeState(EnumType(0));
			}

		private:

			/// <summary>
			/// �m�[�h�̍X�V����
			/// </summary>
			/// <returns>�X�V���Ȃ�true</returns>
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
			/// <param name="isEndNodeUpdate">�X�V�����̏I�����m�F���邩�ǂ���</param>
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
			/// �J��
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
}

//endbasecross