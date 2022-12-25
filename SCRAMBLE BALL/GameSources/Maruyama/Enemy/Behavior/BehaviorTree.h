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

			class Selecter;

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A�̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------
			class I_Behavior {

			};

			//--------------------------------------------------------------------------------------
			/// �r�w�C�r�A
			//--------------------------------------------------------------------------------------
			template<class EnumType>
			class BehaviorTree : public I_Behavior
			{
			public:
				using NodeMap = std::unordered_map<EnumType, std::shared_ptr<I_Node>>;
				using SelecterMap = std::unordered_map<EnumType, std::shared_ptr<Selecter>>;
				using TaskMap = std::unordered_map<EnumType, std::shared_ptr<I_Task>>;

				using EdgesMap = std::unordered_map<EnumType, std::vector<std::shared_ptr<I_Edge>>>;

			private:
				EnumType m_firstNodeType = EnumType(0);	//����m�[�h�^�C�v
				std::weak_ptr<I_Node> m_currentNode;	//���ݐς܂�Ă���^�X�N

				NodeMap m_nodeMap;						//��`�����m�[�h
				SelecterMap m_selecterMap;				//��`�����Z���N�^�[
				TaskMap m_taskMap;						//��`�����^�X�N

				std::stack<std::weak_ptr<I_Node>> m_currentStack;	//���ݐς܂�Ă���m�[�h�X�^�b�N

				EdgesMap m_edgesMap;	//�J�ڃG�b�W
	
			private:
				/// <summary>
				/// �m�[�h�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				/// <param name="node">�m�[�h</param>
				std::shared_ptr<I_Node> AddNode(const EnumType type, const std::shared_ptr<I_Node>& node) {
					node->SetIndex(static_cast<int>(type));
					m_nodeMap[type] = node;
					return node;
				}

				/// <summary>
				/// �Z���N�^�[�ƃG�b�W�̌���
				/// </summary>
				/// <param name="edge"></param>
				void Union(const std::shared_ptr<I_Edge>& edge) {
					std::shared_ptr<I_Node> fromNode = edge->GetFromNode();
					std::shared_ptr<I_Node> toNode = edge->GetToNode();

					//�G�b�W�̎�O�m�[�h���Z���N�^�[�ł���Ȃ�A�J�ڐ�m�[�h����ݒ�
					if (auto fromSelecter = GetSelecter(fromNode->GetType<EnumType>())) {
						fromSelecter->AddTransitionEdge(edge);
					}

					//�J�ڐ�̃m�[�h���Z���N�^�[�Ȃ�A��O�m�[�h��o�^����B
					if (auto toSelecter = GetSelecter(toNode->GetType<EnumType>())) {
						toSelecter->SetFromEdge(edge);
					}
				}

				/// <summary>
				/// �J�����g�X�^�b�N�ɒǉ�����B
				/// </summary>
				/// <param name="node"></param>
				void AddCurrentStack(const std::shared_ptr<I_Node>& node) {
					//�m�[�h�����݂���Ȃ�ǉ�����������B
					if (node) {
						node->OnDecoratorStart();
						node->OnStart();
						node->SetState(BehaviorState::Running);
						m_currentStack.push(node);
					}
				}

				/// <summary>
				/// �J�����g�X�^�b�N����|�b�v����B
				/// </summary>
				void PopCurrentStack() {
					if (m_currentStack.empty()) {	//�X�^�b�N����Ȃ珈�������Ȃ��B
						return;
					}

					auto node = m_currentStack.top().lock();
					if (node) {
						node->OnDecoratorExit();
						node->OnExit();		//�m�[�h�̏I�����菈��
						node->SetState(BehaviorState::Completed);
					}

					m_currentStack.pop();	//�X�^�b�N����|�b�v
				}

				/// <summary>
				///	�����m�[�h�̃��Z�b�g
				/// </summary>
				void ResetFirstNode() {
					for (auto& edge : GetEdges(m_firstNodeType)) {
						edge->GetToNode()->SetState(BehaviorState::Inactive);
					}
				}

				/// <summary>
				/// �S�ẴX�^�b�N���ꂽ�m�[�h�̏I������������B
				/// </summary>
				void ResetAllStack() {
					while (!m_currentStack.empty()) {	//�X�^�b�N����ɂȂ�܂Ń��[�v
						PopCurrentStack();
					}
				}

			public:
				virtual ~BehaviorTree() = default;

				/// <summary>
				/// ���݂̃^�X�N�̃^�C�v
				/// </summary>
				/// <returns>���݂̃^�X�N�^�C�v</returns>
				EnumType GetCurrentType() const { return m_currentNode.lock() ? m_currentNode.lock()->GetType<EnumType>() : EnumType(0); }

				/// <summary>
				/// ���݂̃m�[�h��ݒ�
				/// </summary>
				/// <param name="node">���݂̃m�[�h</param>
				void SetCurrentNode(const std::shared_ptr<I_Node>& node) { m_currentNode = node; }

				/// <summary>
				/// ���݂̃m�[�h���擾
				/// </summary>
				/// <returns>���݂̃m�[�h</returns>
				std::shared_ptr<I_Node> GetCurrentNode() const { return m_currentNode.lock(); }

				/// <summary>
				/// ���̃m�[�h�����݂��邩�ǂ���
				/// </summary>
				/// <param name="type">�m�F�������^�C�v</param>
				/// <returns>�m�[�h�����݂���Ȃ�true</returns>
				bool HasNode(const EnumType type) const { return static_cast<int>(m_nodeMap.count(type)) != 0; }

				/// <summary>
				/// �m�[�h�̎擾
				/// </summary>
				/// <param name="type">�m�[�h�̃^�C�v</param>
				/// <returns>�擾�����m�[�h</returns>
				std::shared_ptr<I_Node> GetNode(const EnumType type) const {
					return m_nodeMap.count(type) != 0 ? m_nodeMap.at(type) : nullptr;
				}

				/// <summary>
				/// �n�����m�[�h�̑J�ڐ�m�[�h���擾
				/// </summary>
				/// <param name="node">�J�ڐ���擾�������m�[�h</param>
				/// <returns>�ŗD��̑J�ڐ�m�[�h</returns>
				std::shared_ptr<I_Node> GetTransitionNode(const std::shared_ptr<I_Node>& node) {
					//Selecter�ł��邱�Ƃ�ۏ؂���B
					auto selecter = GetSelecter(node->GetType<EnumType>());
					if (!selecter) {
						return nullptr;
					}

					//�Z���N�^�[�̃J�����g�m�[�h���������Ď擾
					return selecter->SearchCurrentNode();
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
				void AddSelecter(const EnumType type, const std::shared_ptr<Selecter>& selecter = std::make_shared<Selecter>()) {
					m_selecterMap[type] = selecter;
					AddNode(type, selecter);
				}

				/// <summary>
				/// �Z���N�^�[�̎擾
				/// </summary>
				/// <returns>�Z���N�^�[</returns>
				std::shared_ptr<Selecter> GetSelecter(const EnumType type) const {
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
					task->SetIndex((int)type);
					AddNode(type, task);
				}

				/// <summary>
				/// �^�X�N�̒ǉ�
				/// </summary>
				/// <param name="type">�m�[�h�^�C�v</param>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_constructible_v<T, Ts...> &&	//�R���X�g���N�^�̈������Ď�
						std::is_base_of_v<I_Task, T>,			//���N���X����
					std::nullptr_t> = nullptr
				>
				void AddTask(const EnumType type, Ts&&... params) {
					auto newTask = std::make_shared<T>(params...);
					AddTask(type, newTask);
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
					std::enable_if_t<
						std::is_constructible_v<T, Ts...> &&
						std::is_base_of_v<I_Edge, T>, 
					std::nullptr_t> = nullptr
				>
				std::shared_ptr<T> AddEdge(Ts&&... params) {
					auto newEdge = std::make_shared<T>(params...);
					Union(newEdge);

					auto fromType = newEdge->GetFromNode()->GetType<EnumType>();
					m_edgesMap[fromType].push_back(newEdge);

					return newEdge;
				}

				/// <summary>
				/// �G�b�W�̒ǉ�
				/// </summary>
				/// <param name="fromType">��O�̃m�[�h�^�C�v</param>
				/// <param name="toType">�J�ڐ�̃m�[�h�^�C�v</param>
				/// <param name="priority">�D��x</param>
				std::shared_ptr<EdgeBase> AddEdge(const EnumType fromType, const EnumType toType, const float priority) {
					return AddEdge<EdgeBase>(GetNode(fromType), GetNode(toType), priority);
				}

				/// <summary>
				/// �G�b�W�̎擾
				/// </summary>
				/// <param name="fromType">�G�b�W��From�^�C�v</param>
				/// <param name="toType">�G�b�W��To�^�C�v</param>
				/// <returns>�G�b�W</returns>
				std::shared_ptr<I_Edge> GetEdge(const EnumType fromType, const EnumType toType) const {
					auto edges = GetEdges(fromType);
					for (auto& edge : edges) {
						if (toType == edge->GetToNode()->GetType<EnumType>()) {
							return edge;
						}
					}

					return nullptr;
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
				/// <param name="fromType">�G�b�W��From�^�C�v</param>
				/// <param name="toType">�G�b�W��To�^�C�v</param>
				/// <returns>�G�b�W������Ȃ�true</returns>
				bool HasEdge(const EnumType fromType, const EnumType toType) const {
					auto edges = GetEdges(fromType);
					for (auto& edge : edges) {
						if (toType == edge->GetToNode()->GetType<EnumType>()) {
							return true;
						}
					}

					return false;
				}

				/// <summary>
				/// �G�b�W�������Ă��邩�ǂ���
				/// </summary>
				/// <param name="type">�G�b�W��From�^�C�v</param>
				/// <returns>�G�b�W������Ȃ�true</returns>
				bool HasEdges(const EnumType type) const { return static_cast<int>(m_edgesMap.count(type)) != 0; }

				/// <summary>
				/// �G�b�W�̗D��x�Ǘ���ǉ�
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_base_of_v<I_PriorityController, T>&&	//���N���X
						std::is_constructible_v<T, Ts...>,				//�R���X�g���N�^
					std::nullptr_t> = nullptr
				>
				std::shared_ptr<T> AddEdgePriorityController(const EnumType fromType, const EnumType toType, Ts&&... params) {
					//�G�b�W�̎擾
					auto edge = GetEdge(fromType, toType);
					if (!edge) {
						return nullptr;
					}

					auto newController = std::make_shared<T>(params...);	//�D��x�Ǘ��̐V�K�����B
					edge->AddPriorityController(newController);				//�G�b�W�ɗD��x�Ǘ���o�^����B
					
					return newController;
				}

				/// <summary>
				/// �f�R���[�^�̐ݒ�
				/// </summary>
				/// <param name="type">�ݒ肵�����m�\�h�^�C�v</param>
				/// <param name="decorator">�ݒ肵�����f�R���[�^</param>
				bool AddDecorator(const EnumType type, const std::shared_ptr<I_Decorator>& decorator) {
					if (!HasNode(type)) {
						return false;
					}

					auto node = GetNode(type);
					decorator->OnCreate();			//�������Ɉ�x�����Ăяo�����������B
					node->AddDecorator(decorator);

					return true;
				}

				/// <summary>
				/// �f�R���[�^�̐ݒ�
				/// </summary>
				template<class T, class... Ts,
					std::enable_if_t<
						std::is_constructible_v<T, Ts...> &&	//�R���X�g���N�^�̈���
						std::is_base_of_v<I_Decorator, T>,		//���N���X�̐���
					std::nullptr_t> = nullptr
				>
				bool AddDecorator(const EnumType type, Ts&&... params) {
					return AddDecorator(type, std::make_shared<T>(params...));
				}

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

				/// <summary>
				/// �����I��
				/// </summary>
				void ForceStop() {
					ResetAllStack();
					ResetFirstNode();
					m_currentNode.reset();
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

				/// <summary>
				/// �X�V�s�ɂȂ��Ă���m�[�h�����݂��邩�ǂ���
				/// </summary>
				/// <returns>�X�V�s�̃m�[�h��Ԃ�</returns>
				std::shared_ptr<I_Node> SearchStopNode() {
					auto copyStack = m_currentStack;
					while (!copyStack.empty()) {	//�X�^�b�N����ɂȂ�܂�
						auto node = copyStack.top().lock();
						if (!node->CanUpdate()) {	//�m�[�h���X�V�\�łȂ�������
							return node;
						}

						copyStack.pop();
					}

					return nullptr;
				}

				/// <summary>
				/// �ċN���Ċ����߂��Ă��鎞�́APop�����Ǝ��̃m�[�h��Ԃ�����
				/// </summary>
				/// <returns>���̃m�[�h</returns>
				std::shared_ptr<I_Node> ReverseProcess() {
					PopCurrentStack();

					if (m_currentStack.empty()) {	//�X�^�b�N��0�ɂȂ����珉���m�[�h��Ԃ��B
						return GetNode(m_firstNodeType);
					}

					return m_currentStack.top().lock();
				}

				/// <summary>
				/// �J�ڐ�̃m�[�h��������܂ŁA�X�^�b�N�������߂��B
				/// </summary>
				/// <param name="node">�m�F�������m�[�h</param>
				/// <returns></returns>
				std::shared_ptr<I_Node> ReverseStack(const std::shared_ptr<I_Node>& node) {
					if (node == GetNode(m_firstNodeType)) {
						ResetFirstNode();
					}

					auto selecter = GetSelecter(node->GetType<EnumType>());
					if (!selecter) {	//�Z���N�^�[�łȂ��Ȃ�O�̃m�[�h�ɖ߂�B
						return ReverseProcess();
					}

					auto nextNode = selecter->SearchCurrentNode();
					if (!nextNode) {	//�m�[�h�����݂��Ȃ��Ȃ�A��O�̃m�[�h�ɖ߂�B
						return ReverseProcess();
					}

					return nextNode;
				}

				/// <summary>
				/// �ċN���������āA�J�ڐ�̃m�[�h���擾����B
				/// </summary>
				/// <returns>��ԗD��x�̍����m�[�h</returns>
				std::shared_ptr<I_Node> Recursive_TransitionNode(const std::shared_ptr<I_Node>& node) {
					if (!node) {
						return nullptr;
					}

					//Task�m�[�h�Ȃ�A���[�m�[�h�ƂȂ�B
					if (HasTask(node->GetType<EnumType>())) {
						AddCurrentStack(node);	//�ŏI�m�[�h���X�^�b�N�ɓ����B
						return node;			//���[�m�[�h���m��
					}

					//�J�ڐ悪���݂���Ȃ�A�X�^�b�N�ɒǉ����čċN����
					if (auto transitionNode = GetTransitionNode(node)) {
						AddCurrentStack(node);	//�X�^�b�N�ɐςށB
						//��ԗD�揇�ʂ̍����m�[�h���擾����B
						return Recursive_TransitionNode(transitionNode);
					}

					//�J�ڐ悪�Ȃ����߁APop���Ė߂�B
					return Recursive_TransitionNode(ReverseStack(node));
				}

				/// <summary>
				/// �����œn�����m�[�h�̎�O�m�[�h�܂ŃX�^�b�N��߂�
				/// </summary>
				/// <param name="targetNode">�߂肽���m�[�h</param>
				void ReverseTargetBeforeStack(const std::shared_ptr<I_Node>& targetNode) {
					if (m_currentStack.empty()) {
						return;
					}

					auto currentNode = m_currentStack.top().lock();
					if (currentNode == targetNode) {
						PopCurrentStack();	//�X�g�b�v������O�̃m�[�h�܂Ŗ߂��ďI��
						return;
					}

					PopCurrentStack();		//�X�^�b�N���|�b�v
					ReverseTargetBeforeStack(targetNode);	//�ċN����
				}

				/// <summary>
				/// �J�ڂ���Ƃ��̔��f�J�n�ʒu�̃m�[�h���擾����B
				/// </summary>
				std::shared_ptr<I_Node> GetTransitionStartNode() {
					if (m_currentStack.size() == 0) {	
						return GetNode(m_firstNodeType);
					}
					
					return ReverseStack(m_currentStack.top().lock());
				}

				/// <summary>
				/// �J�ڏ���
				/// </summary>
				void Transition() {
					//�D��x�̈�ԍ����m�[�h�ɑJ��
					auto node = Recursive_TransitionNode(GetTransitionStartNode());
					SetCurrentNode(node);	//�J�����g�m�[�h�̐ݒ�
				}

			public:
				/// <summary>
				/// �X�V����
				/// </summary>
				void OnUpdate() {
					if (IsEmpty()) {
						return;
					}

					//�Ď����K�v�ȃf�R���[�^�̍X�V
					auto stopNode = SearchStopNode();
					if (stopNode) {	//�߂�l�����݂���Ȃ�A�X�V�s�\�ȃm�[�h�����݂����B
						//�X�g�b�v�����m�[�h�̎�O�m�[�h�܂Ŗ߂�B
						ReverseTargetBeforeStack(stopNode);
						Transition();	//�J��
					}

					bool isEndTaskUpdate = TaskUpdate();	//�m�[�h�̍X�V

					//�m�[�h���I��������A�J��
					if (isEndTaskUpdate) {
						Transition();	//�J��
					}
				}

			};

		}

	}
}