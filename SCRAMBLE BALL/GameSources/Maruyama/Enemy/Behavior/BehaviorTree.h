/*!
@file BehaviorTree.h
@brief BehaviorTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Enemy/Astar/SparseGraph.h"

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
				EnumType m_currentType = EnumType(0);	//���݂̃^�X�N

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
				int AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) override {
					int index = SparseGraph::AddNode(node);
					m_nodeMap[type] = node;

					return index;
				}

			public:
				virtual ~BehaviorTree() = default;

				/// <summary>
				/// �w�肵���^�C�v��Selecter�������Ă��邩�ǂ���
				/// </summary>
				/// <param name="type">�w��^�C�v</param>
				/// <returns>�����Ă���Ȃ�true</returns>
				bool HasSelecter(const EnumType type) {
					return static_cast<int>(m_selecterMap.count(type)) != 0;	//0�łȂ�������
				}

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
				void AddTask(const EnumType type, std::shared_ptr<I_Task>& task) {
					m_taskMap[type] = task;
					AddNode(type, task);
				}

				/// <summary>
				/// �^�X�N�̎擾
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				std::shared_ptr<I_Task> GetTask(const EnumType type) const {
					return m_taskMap.at(type);
				}

				/// <summary>
				/// ���݂̃m�[�h���擾
				/// </summary>
				std::shared_ptr<I_Task> GetCurrentTask() const {
					return m_taskMap.at(m_currentType);
				}

				/// <summary>
				/// �G�b�W�̒ǉ�
				/// </summary>
				/// <param name="fromType"></param>
				/// <param name="toType"></param>
				void AddEdge(const EnumType fromType, const EnumType toType) {
					AddEdge<EdgeBase>(fromType, toType);
				}

				/// <summary>
				/// �G�b�W�̒ǉ�
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<std::is_constructible_v<T, std::shared_ptr<I_Edge>&, Ts...>, std::nullptr_t> = nullptr>
				void AddEdge(Ts&&... params) {
					std::shared_ptr<I_Edge> newEdge = std::make_shared<T>(params...);

					std::shared_ptr<I_Node> fromNode = newEdge->GetFromNode();
					std::shared_ptr<I_Node> toNode = newEdge->GetToNode();
					
					auto type = static_cast<EnumType>(fromNode->GetIndex());

					//���̃Z���N�^�[������Ȃ�A�^�X�N�ɐ�̃m�[�h����ݒ�
					if (auto selecter = GetSelecter(type)) {
						selecter->SetFromNode(fromNode);
						selecter->AddTransitionNode(toNode);
					}

					auto fromType = static_cast<EnumType>(fromNode->GetIndex());
					m_edgesMap[fromType].push_back(newEdge);
				}

				/// <summary>
				/// ��̏�Ԃ��ǂ���
				/// </summary>
				/// <returns>��̏�ԂȂ�true</returns>
				bool IsEmpty() const { return static_cast<int>(m_taskMap.size()) == 0; }

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

			};

		}

	}
}