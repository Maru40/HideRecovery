/*!
@file BehaviorTree.h
@brief BehaviorTree�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

enum class TestEnumTask : std::uint8_t
{
	Task
};

namespace basecross {
	namespace maru {

		namespace Behavior {

			//--------------------------------------------------------------------------------------
			/// �f�R���[�^�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Decorator {
			public:
				/// <summary>
				/// �J�ڂł��邩�ǂ���
				/// </summary>
				/// <returns>�J�ڂł���Ȃ�true</returns>
				virtual bool CanTransition() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �D��x�Ǘ��̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_PriorityController {
			public:
				/// <summary>
				/// �D��x�̎擾
				/// </summary>
				/// <returns>�D��x</returns>
				virtual float GetPriority() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �D��x�Ǘ��̊��N���X
			//--------------------------------------------------------------------------------------
			class PriorityControllerBase : public I_PriorityController {
				float m_priority = 0;

			public:
				float GetPriority() const override { return m_priority; }

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�m�[�h�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Node {
			public:
				/// <summary>
				/// �C���f�b�N�X�̃Z�b�g
				/// </summary>
				virtual void SetIndex(const int index) noexcept = 0;

				/// <summary>
				/// �C���f�b�N�X�̎擾
				/// </summary>
				/// <returns></returns>
				virtual int GetIndex() const noexcept = 0;

				/// <summary>
				/// �J�ڂł��邩�ǂ���
				/// </summary>
				/// <returns>�J�ڂł���Ȃ�true</returns>
				virtual bool CanTransition() const = 0;

				/// <summary>
				/// �f�R���[�^�̒ǉ�
				/// </summary>
				virtual void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) = 0;

				/// <summary>
				/// �f�R���[�^�����邩�ǂ���
				/// </summary>
				/// <returns>�f�R���[�^��Empty�Ȃ�true</returns>
				virtual bool IsDecoratorEmpty() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�m�[�h�̊��N���X
			//--------------------------------------------------------------------------------------
			class NodeBase : public I_Node
			{
				int m_index = 0;										//�m�[�h�C���f�b�N�X
				std::vector<std::shared_ptr<I_Decorator>> m_decorators;	//�f�R���[�^�z��

			public:
				void SetIndex(const int index) noexcept override { m_index = index; }

				int GetIndex() const noexcept override { return m_index; }

				bool CanTransition() const override;

				void AddDecorator(const std::shared_ptr<I_Decorator>& decorator) override;

				bool IsDecoratorEmpty() const override;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�^�X�N�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Task : public NodeBase {
			public:
				virtual void OnStart() = 0;
				virtual bool OnUpdate() = 0;
				virtual void OnExit() = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̑J�ڐ�m�[�h�f�[�^
			//--------------------------------------------------------------------------------------
			struct Selecter_TransitionNodeData {
				std::shared_ptr<I_PriorityController> priorityController;	//�D��x�R���g���[���[
				std::weak_ptr<I_Node> node;									//�m�[�h

				Selecter_TransitionNodeData(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) :
					priorityController(priorityController),
					node(node)
				{}
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Selecter : public NodeBase {
			public:
				using TransitionNodeData = Selecter_TransitionNodeData;

			public:
				/// <summary>
				/// ��O�̃m�[�h�̐ݒ�
				/// </summary>
				/// <param name="node">��O�̃m�[�h</param>
				virtual void SetFromNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// ��O�̃m�[�h�̎擾
				/// </summary>
				/// <returns>��O�̃m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetFromNode() const = 0;

				/// <summary>
				/// �J�ڐ�m�[�h�̒ǉ�
				/// </summary>
				/// <param name="priorityController">�D��x�Ǘ�</param>
				/// <param name="node">�J�ڐ�m�[�h</param>
				virtual void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) = 0;

				/// <summary>
				/// ��ԗD��x�����m�[�h�̎擾
				/// </summary>
				/// <returns></returns>
				virtual std::shared_ptr<I_Node> GetFirstPriorityNode() const = 0;

				/// <summary>
				/// �J�ڐ�m�[�h���󂩂ǂ���
				/// </summary>
				/// <returns>�J�ڐ�m�[�h����Ȃ�true</returns>
				virtual bool IsEmptyTransitionNodes() const = 0;

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�Z���N�^�[�̊��N���X
			//--------------------------------------------------------------------------------------

			class SelecterBase : public I_Selecter
			{
				std::weak_ptr<I_Node> m_fromNode;									//�����̎�O�ɑ��݂���m�[�h
				std::vector<std::shared_ptr<TransitionNodeData>> m_transitionNodes;	//�����̑J�ڐ�m�[�h�Q(�D��x)

			public:
				SelecterBase(const std::shared_ptr<I_Node>& fromNode);

				void SetFromNode(const std::shared_ptr<I_Node>& node) { m_fromNode = node; }
				
				std::shared_ptr<I_Node> GetFromNode() const { return m_fromNode.lock(); }

				void AddTransitionNode(
					const std::shared_ptr<I_PriorityController>& priorityController,
					const std::shared_ptr<I_Node>& node
				) {
					m_transitionNodes.push_back(std::make_shared<TransitionNodeData>(priorityController, node));
				}

				std::shared_ptr<I_Node> GetFirstPriorityNode() const;

				bool IsEmptyTransitionNodes() const;

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�̃G�b�W�C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Edge {
			public:
				/// <summary>
				/// ��Ԃ̃m�[�h��ݒ�
				/// </summary>
				/// <param name="node">��O�̃m�[�h</param>
				virtual void SetFromNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// ��O�̃m�[�h���擾
				/// </summary>
				/// <returns>��O�̃m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetFromNode() const = 0;

				/// <summary>
				/// ��̃m�[�h�̐ݒ�
				/// </summary>
				/// <param name="node">��̃m�[�h</param>
				virtual void SetToNode(const std::shared_ptr<I_Node>& node) = 0;

				/// <summary>
				/// ��̃m�[�h���擾
				/// </summary>
				/// <returns>��̃m�[�h</returns>
				virtual std::shared_ptr<I_Node> GetToNode() const = 0;
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�̃G�b�W�̊��N���X
			//--------------------------------------------------------------------------------------
			class EdgeBase : public I_Edge
			{
				std::weak_ptr<I_Node> m_fromNode;	//�����̎�O�̃m�[�h
				std::weak_ptr<I_Node> m_toNode;		//�����̐�̃m�[�h

			public:
				EdgeBase(const std::shared_ptr<I_Node>& fromNode, const std::shared_ptr<I_Node>& toNode) :
					m_fromNode(fromNode),
					m_toNode(toNode)
				{ }

				void SetFromNode(const std::shared_ptr<I_Node>& node) override { m_fromNode = node; }

				std::shared_ptr<I_Node> GetFromNode() const override { return m_fromNode.lock(); }

				void SetToNode(const std::shared_ptr<I_Node>& node) override { m_toNode = node; }

				std::shared_ptr<I_Node> GetToNode() const override { return m_toNode.lock(); }
			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree
			{
				EnumType m_currentType = EnumType(0);	//���݂̃^�X�N

				std::unordered_map<EnumType, std::shared_ptr<I_Selecter>> m_selecterMap;		//��`�����Z���N�^�[
				std::unordered_map<EnumType, std::shared_ptr<I_Task>> m_taskMap;				//��`�����^�X�N
				std::unordered_map<EnumType, std::shared_ptr<I_Node>> m_nodeMap;				//��`�����m�[�h
				std::unordered_map<EnumType, std::vector<std::shared_ptr<I_Edge>>> m_edgesMap;	//��`�����G�b�W

			private:
				/// <summary>
				/// �m�[�h�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				/// <param name="node">�m�[�h</param>
				void AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) {
					m_nodeMap[type] = node;
				}

			public:

				/// <summary>
				/// �Z���N�^�[�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				/// <param name="selecter">�Z���N�^�[</param>
				void AddSelecter(const EnumType type, const std::shared_ptr<I_Selecter>& selecter) {
					m_selecter[type] = selecter;
					AddNode(type, selecter);
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
					m_edgesMap[fromType].push_back(std::make_shared<I_Edge>());
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