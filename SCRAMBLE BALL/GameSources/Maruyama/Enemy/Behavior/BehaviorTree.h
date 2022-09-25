/*!
@file BehaviorTree.h
@brief BehaviorTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/SparseGraph.h"

#include "Interface/I_Node.h"
#include "Interface/I_Edge.h"
#include "Interface/I_Task.h"
#include "Interface/I_Selecter.h"
#include "Interface/I_Decorator.h"
#include "Interface/I_PriorityController.h"

enum class TestEnumTask : std::uint8_t
{
	Task
};

namespace basecross {
	namespace maru {

		namespace Behavior {
			//--------------------------------------------------------------------------------------
			/// �O���錾
			//--------------------------------------------------------------------------------------
			class I_Node;
			class NodeBase;

			class I_Edge;
			class EdgeBase;

			class I_Decorator;
			class I_PriorityController;
			class I_Task;

			class I_Selecter;
			class I_SelecterBase;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree
			{
			public:
				using EdgesMap = std::unordered_map<EnumType, std::vector<std::shared_ptr<I_Edge>>>;

			private:
				//EnumType m_currentType = EnumType(0);	//���݂̃^�X�N
				EnumType m_firstNodeType = EnumType(0);	//����m�[�h�^�C�v
				std::weak_ptr<I_Node> m_currentNode;	//���ݐς܂�Ă���^�X�N

				std::unordered_map<EnumType, std::shared_ptr<I_Node>> m_nodeMap;			//��`�����m�[�h
				std::unordered_map<EnumType, std::shared_ptr<I_Selecter>> m_selecterMap;	//��`�����Z���N�^�[
				std::unordered_map<EnumType, std::shared_ptr<I_Task>> m_taskMap;			//��`�����^�X�N

				EdgesMap m_edgesMap;	//�J�ڃG�b�W

			private:
				/// <summary>
				/// �m�[�h�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				/// <param name="node">�m�[�h</param>
				std::shared_ptr<I_Node> AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) {
					m_nodeMap[type] = node;

					return node;
				}

				void Union(const std::shared_ptr<I_Edge>& edge) {
					std::shared_ptr<I_Node> fromNode = edge->GetFromNode();
					std::shared_ptr<I_Node> toNode = edge->GetToNode();

					//���̃Z���N�^�[������Ȃ�A�^�X�N�ɐ�̃m�[�h����ݒ�
					if (auto selecter = GetSelecter(fromNode->GetType<EnumType>())) {
						selecter->SetFromNode(fromNode);
						selecter->AddTransitionNode(edge->GetPriorityContorller(), toNode);
					}
				}


			public:
				virtual ~BehaviorTree() = default;

				/// <summary>
				/// ���݂̃^�X�N�̃^�C�v
				/// </summary>
				/// <returns>���݂̃^�X�N�^�C�v</returns>
				EnumType GetCurrentType() const { return m_currentNode.lock() ? m_currentNode.lock()->GetType<EnumType>() : EnumType(0); }

				std::shared_ptr<I_Node> GetCurrentNode() const { return m_currentNode.lock(); }

				bool HasNode(const EnumType type) const { return static_cast<int>(m_nodeMap.count(type)) != 0; }

				std::shared_ptr<I_Node> GetNode(const EnumType type) const {
					return m_nodeMap.count(type) != 0 ? m_nodeMap.at(type) : nullptr;
				}

				/// <summary>
				/// �ŗD��̑J�ڐ�m�[�h�̎擾
				/// </summary>
				/// <param name="type">�J�n�m�[�h</param>
				/// <returns>�ŗD��̑J�ڐ�m�[�h</returns>
				std::shared_ptr<I_Node> CalculateFirstPriorityNode(const EnumType type) {
					//�����m�[�h������L�т�G�b�W���擾
					std::vector<std::shared_ptr<I_Edge>> edges = GetEdges(type);

					////�D��x���ɕ��ѕς���
					auto sortEvent = [](const std::shared_ptr<I_Edge>& right, const std::shared_ptr<I_Edge>& left) {
						return right->GetPriority() < left->GetPriority();	//�D�揇�ʂ��Ⴂ���Ƀ\�[�g
					};
					std::sort(edges.begin(), edges.end(), sortEvent);

					return edges[0]->GetToNode();	//�D�揇�ʂ̍����m�[�h���擾
				}

				/// <summary>
				/// �w�肵���^�C�v��Selecter�������Ă��邩�ǂ���
				/// </summary>
				/// <param name="type">�w��^�C�v</param>
				/// <returns>�����Ă���Ȃ�true</returns>
				bool HasSelecter(const EnumType type) const { return static_cast<int>(m_selecterMap.count(type)) != 0; }

				/// <summary>
				/// �Z���N�^�[�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				/// <param name="selecter">�Z���N�^�[</param>
				void AddSelecter(const EnumType type, const std::shared_ptr<I_Selecter>& selecter) {
					m_selecterMap[type] = selecter;
					AddNode(type, selecter);
				}

				/// <summary>
				/// �Z���N�^�[�̎擾
				/// </summary>
				/// <returns>�Z���N�^�[</returns>
				std::shared_ptr<I_Selecter> GetSelecter(const EnumType type) const {
					return HasSelecter(type) ? m_selecterMap.at(type) : nullptr;	//�����Ă��Ȃ��Ȃ�nullptr��Ԃ��B
				}

				/// <summary>
				/// �^�X�N����`����Ă��邩�ǂ���
				/// </summary>
				/// <param name="type">�^�X�N�^�C�v</param>
				/// <returns>�^�X�N����`����Ă�����true</returns>
				bool HasTask(const EnumType type) const {
					return static_cast<int>(m_taskMap.count(type)) != 0;
				}

				/// <summary>
				/// �^�X�N�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				/// <param name="node">�^�X�N</param>
				void AddTask(const EnumType type, const std::shared_ptr<I_Task>& task) {
					m_taskMap[type] = task;
					AddNode(type, task);
				}

				/// <summary>
				/// �^�X�N�̎擾
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				std::shared_ptr<I_Task> GetTask(const EnumType type) const {
					return HasTask(type) ? m_taskMap.at(type) : nullptr;
				}

				/// <summary>
				/// ���݂̃m�[�h���擾
				/// </summary>
				std::shared_ptr<I_Task> GetCurrentTask() const {
					return dynamic_pointer_cast<I_Task>(m_currentNode.lock());
				}

				/// <summary>
				/// �G�b�W�̒ǉ�
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<std::is_constructible_v<T, Ts...>, std::nullptr_t> = nullptr>
				void AddEdge(Ts&&... params) {
					std::shared_ptr<I_Edge> newEdge = std::make_shared<T>(params...);
					Union(newEdge);

					auto fromType = newEdge->GetFromNode()->GetType<EnumType>();
					m_edgesMap[fromType].push_back(newEdge);
				}

				/// <summary>
				/// �G�b�W�̒ǉ�
				/// </summary>
				/// <param name="fromType"></param>
				/// <param name="toType"></param>
				void AddEdge(const EnumType fromType, const EnumType toType, const std::shared_ptr<I_PriorityController>& priorityController) {
					AddEdge<EdgeBase>(GetNode(fromType), GetNode(toType), priorityController);
				}

				/// <summary>
				/// �����̃^�C�v����L�т�G�b�W�z��̎擾
				/// </summary>
				/// <param name="type">�^�C�v</param>
				/// <returns>�����̃^�C�v����L�т�G�b�W�z��</returns>
				std::vector<std::shared_ptr<I_Edge>> GetEdges(const EnumType type) const {
					if (!HasEdges(type)) {	//���̃^�C�v�̃G�b�W�����݂��Ȃ��Ȃ��̔z���Ԃ��B
						return std::vector<std::shared_ptr<I_Edge>>();
					}

					return m_edgesMap.at(type);
				}

				/// <summary>
				/// �G�b�W�������Ă��邩�ǂ���
				/// </summary>
				/// <param name="type">�G�b�W��From�^�C�v</param>
				/// <returns>�G�b�W������Ȃ�true</returns>
				bool HasEdges(const EnumType type) const { return static_cast<int>(m_edgesMap.count(type)) != 0; }

				/// <summary>
				/// ��̏�Ԃ��ǂ���
				/// </summary>
				/// <returns>��̏�ԂȂ�true</returns>
				bool IsEmpty() const { return static_cast<int>(m_taskMap.size()) == 0; }

				/// <summary>
				/// �J�n�m�[�h�^�C�v�̐ݒ�
				/// </summary>
				/// <param name="type">�J�n�m�[�h�^�C�v</param>
				void SetFirstNodeType(const EnumType type) { m_firstNodeType = type; }

				/// <summary>
				/// �J�n�m�[�h�̎擾
				/// </summary>
				/// <returns>�J�n�m�[�h</returns>
				EnumType GetFirstNodeType() const { return m_firstNodeType; }

			public:
				/// <summary>
				/// �X�V����
				/// </summary>
				void OnUpdate() {
					if (IsEmpty()) {
						return;
					}

					bool isEndTaskUpdate = TaskUpdate();

					//�m�[�h���I��������A�ŏ��ɖ߂�
					if (isEndTaskUpdate) {
						Transition();	//�J��
					}
				}

			private:
				/// <summary>
				/// �m�[�h�̍X�V
				/// </summary>
				/// <returns>�m�[�h�̍X�V���I�������Ȃ�true</returns>
				bool TaskUpdate() {
					auto currentTask = GetCurrentTask();
					if (currentTask) {
						return currentTask->OnUpdate();
					}

					return true;
				}

				//�ċN���������āA�J�ڐ�̃m�[�h���擾����B
				std::shared_ptr<I_Node> Recursive_TransitionNode(const std::shared_ptr<I_Node>& node) {
					//�G�b�W�����݂��Ȃ��Ȃ�node�𐶐�����B
					if (!HasEdges(node->GetType<EnumType>())) {	
						return node;
					}

					//��ԗD�揇�ʂ̍����m�[�h���擾����B
					return Recursive_TransitionNode(CalculateFirstPriorityNode(node->GetType<EnumType>()));
				}

				/// <summary>
				/// �J�ڏ���
				/// </summary>
				void Transition() {
					//�D��x�̈�ԍ����m�[�h�ɑJ��
					auto node = Recursive_TransitionNode(GetNode(m_firstNodeType));

					m_currentNode = node;
				}

			};

		}

	}
}