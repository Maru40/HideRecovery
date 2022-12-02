/*!
@file StateMachine.h
@brief StateMachine
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "GraphBase_Ex.h"
#include "Node_StateMachine.h"
#include "Edge_StateMachine.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		template<class OwnerType>
		class NodeBase_StateMachine;
		class EdgeBase;
		template<class EnumType, class TransitionMember>
		class Edge_StateMachine;

		//�Ȃ����O���錾�ł��Ȃ��B
		//template<class EnumType, class NodeType, class EdgeType>
		//class GraphBase;

		//--------------------------------------------------------------------------------------
		/// �X�e�[�g�}�V���p�̐V�K�N���X(����������A�O��̍폜)
		//--------------------------------------------------------------------------------------
		template<class OwnerType, class EnumType, class TransitionStructMember>
		class StateMachine
		{
		public:
			using NodeType = NodeBase_StateMachine<OwnerType>;
			using EdgeType = Edge_StateMachine<EnumType, TransitionStructMember>;
			using GraphType = maru::GraphBase<EnumType, NodeType, EdgeType>;

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

		private:
			std::unique_ptr<GraphType> m_graph;				//�O���t�^�C�v

			EnumType m_currentNodeType = EnumType(0);		//���ݎg�p���̃m�[�h�^�C�v

			TransitionStructMember m_transitionStruct;		//�J�ڏ����ɗ��p����\����

			std::vector<TransitionCandidateParametor> m_transitionCandidates;	//�J�ڐ���p�����[�^�Q

		private:
			/// <summary>
			/// �X�e�[�g�̕ύX
			/// </summary>
			/// <param name="type">�ύX�������X�e�[�g�^�C�v</param>
			void ChangeState(const EnumType type) {
				std::shared_ptr<NodeType> nowNode = GetCurrentNode();
				if (nowNode) {
					nowNode->OnExit();
				}

				m_currentNodeType = type;

				std::shared_ptr<NodeType> nextNode = GetNode(m_currentNodeType);
				if (nextNode) {
					nextNode->OnStart();
				}
			}

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			/// <param name="type">�m�[�h��EnumType</param>
			/// <param name="node">�ǉ�����m�[�h</param>
			void AddNode(const EnumType type, const std::shared_ptr<NodeType>& node) {
				if (IsEmpty()) {
					m_currentNodeType = type;
					if (node) {
						node->OnStart();
					}
				}

				m_graph->AddNode(type, node);
			}

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			StateMachine():
				StateMachine(TransitionStructMember())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="member">�J�ڏ����p�����o</param>
			StateMachine(const TransitionStructMember& member) :
				m_graph(new GraphType()), 
				m_transitionStruct(member)
			{}

			virtual ~StateMachine() = default;

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			/// <param name="type">�m�[�h�̃^�C�v</param>
			template<class T, class... Ts,
				std::enable_if_t<
					std::is_base_of_v<const NodeType, T> &&			//���N���X�̐���
					std::is_constructible_v<T, const int, Ts...>,	//�R���X�g���N�^�̊Ď�
				std::nullptr_t> = nullptr
			>
			void AddNode(const EnumType type, Ts&&... params) {
				int typeInt = static_cast<int>(type);
				auto newNode = std::make_shared<T>(typeInt, params...);

				AddNode(type, newNode);
			}

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			/// <param name="type">�m�[�h�̃^�C�v</param>
			template<class T, class... Ts,
				std::enable_if_t<
					std::is_base_of_v<const NodeType, T>&&			//���N���X�̐���
					std::is_constructible_v<T, Ts...>,				//�R���X�g���N�^�̊Ď�
				std::nullptr_t> = nullptr
			>
			void AddNode(const EnumType type, Ts&&... params) {
				auto newNode = std::make_shared<T>(params...);
				newNode->SetIndex(static_cast<int>(type));

				AddNode(type, newNode);
			}

			/// <summary>
			/// �G�b�W�̒ǉ�
			/// </summary>
			/// <param name="edge">�ǉ��������G�b�W</param>
			void AddEdge(const std::shared_ptr<EdgeType>& edge) {
				m_graph->AddEdge(edge);
			}

			/// <summary>
			/// �G�b�W�̒ǉ�
			/// </summary>
			/// <param name="from">���̃^�C�v</param>
			/// <param name="to">�J�ڐ�̃^�C�v</param>
			/// <param name="isTransitionFunc">�J�ڏ���</param>
			/// <param name="isEndTransition">�I�����ɑJ�ڂ��邩�ǂ���</param>
			void AddEdge(
				const EnumType from, const EnumType to,
				const std::function<bool(const TransitionStructMember& transition)>& isTransitionFunc,
				const bool isEndTransition = false)
			{
				auto fromNode = GetNode(from);
				auto toNode = GetNode(to);

				auto newEdge = std::make_shared<EdgeType>(fromNode, toNode, isTransitionFunc, static_cast<int>(to), isEndTransition);
				AddEdge(newEdge);
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
			/// �m�[�h�̎擾
			/// </summary>
			/// <param name="type">�m�[�h�^�C�v</param>
			/// <returns>�m�[�h</returns>
			std::shared_ptr<NodeType> GetNode(const EnumType type) const {
				return m_graph->GetNode(type);
			}

			/// <summary>
			/// ���ݎg�p���̃m�[�h���擾
			/// </summary>
			/// <returns>�m�[�h�̎擾</returns>
			std::shared_ptr<NodeType> GetCurrentNode() const {
				return GetNode(m_currentNodeType);
			}

			/// <summary>
			/// ���ݎg�p���̃m�[�h�ɐݒ肵�Ă���G�b�W���擾
			/// </summary>
			/// <returns>�G�b�W�̎擾</returns>
			auto GetCurrentNodeEdges() const {
				return m_graph->GetEdges(m_currentNodeType);
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
			void ChangeState(const EnumType& type, const int priority) {
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
			/// �O���t���󂩂ǂ���
			/// </summary>
			/// <returns>��Ȃ�true</returns>
			bool IsEmpty() const { return m_graph->IsEmpty(); }

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
				auto node = GetCurrentNode();
				if (node) {
					return node->OnUpdate();
				}

				return false;
			}

			/// <summary>
			/// �J�ڃ`�F�b�N
			/// </summary>
			void TransitionCheck(const bool isEndNodeUpdate) {
				auto edges = GetCurrentNodeEdges();
				for (auto& edge : edges) {
					if (edge->IsTransition(m_transitionStruct, isEndNodeUpdate)) {
						const auto toType = edge->GetToType();
						const auto priority = edge->GetPriority();
						m_transitionCandidates.push_back(TransitionCandidateParametor(toType, priority));
					}
				}
			}

			/// <summary>
			/// �g���K�[�̃��Z�b�g
			/// </summary>
			void TriggerReset() {
				auto edgesMap = m_graph->GetEdgesMap();
				for (auto& edgePair : edgesMap) {
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
					{ return left.priority > right.priority ? true : false; }
				); //�D��x������������J�ڂ���B

				ChangeState(m_transitionCandidates[0].type);
			}
		};

	}
}