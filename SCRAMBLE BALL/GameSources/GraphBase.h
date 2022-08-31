
/*!
@file GraphBase.h
@brief GraphBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class node_type>
	class StateNodeBase;

	class I_StateNode;

	template<class EnumType, class TransitionStructMember>
	class StateEdgeBase;

	class GraphStateNode;

	#define DECLARE_NODE(NodeName, NodeType, Enter, Update, Exit) class NodeName : public StateNodeBase<NodeType>{ public: NodeName(); void OnStart() override{ if(Enter){ Enter(); } }; void OnUpdate() override { if(Update){ Update(); } }; void OnExit(){ if(Exit){ Exit(); } }; };

	/// <summary>
	/// Graph�̊��N���X
	/// </summary>
	/// <typeparam name="NodeType">�g�p����m�[�h</typeparam>
	/// <typeparam name="EnumType">�g�p����񋓑�</typeparam>
	/// <typeparam name="TransitionStructMember">�J�ڏ����p�̍\���̃����o�[</typeparam>
	template<class node_type, class EnumType, class TransitionStructMember>
	class GraphBase
	{
	public:
		//using�f�B���N�e�B�u
		using EdgeType = StateEdgeBase<EnumType, TransitionStructMember>;

		using NodeType = node_type;
		using NodeMap = std::map<EnumType, std::shared_ptr<NodeType>>;
		using EdgeVector = std::vector<std::shared_ptr<EdgeType>>;
		using EdgeVectorMap = std::map<EnumType, EdgeVector>;

	private:
		NodeMap m_nodes;          //�m�[�h���i�[����z��
		EdgeVectorMap m_edgesMap; //�G�b�W���i�[����z��

		bool m_isActive = true;   //�A�N�e�B�u���ǂ���

	public:
		GraphBase() {}
		~GraphBase() {}

		/// <summary>
		/// �w�肵���m�[�h�̎擾
		/// </summary>
		/// <param name="type">�m�[�h�^�C�v</param>
		/// <returns>�w�肵���m�[�h</returns>
		std::shared_ptr<NodeType> GetNode(const EnumType type) const {
			return m_nodes.at(type);
		}

		/// <summary>
		/// �m�[�h�̔z����擾
		/// </summary>
		/// <returns>�m�[�h�z��</returns>
		NodeMap GetNodes() const {
			return m_nodes;
		}

		/// <summary>
		/// �w�肵���m�[�h��A������G�b�W�̎擾
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <param name="to">��̃C���f�b�N�X</param>
		/// <returns>�w�肵���m�[�h��A������G�b�W�̎擾</returns>
		std::shared_ptr<EdgeType> GetEdge(const EnumType from, const EnumType to) const {
			//���݂��Ȃ�������nullptr��Ԃ��B
			if (m_edgesMap.count[from] == 0) {
				return nullptr;
			}

			auto edges = m_edgesMap[from];
			for (auto& edge : edges) {
				if (edge->GetToType() == to) {
					return edge;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// �w�肵���m�[�h����L�т�G�b�W�z��̎擾
		/// </summary>
		/// <param name="from">�擾�������m�[�h</param>
		/// <returns>�w�肵���m�[�h����L�т�G�b�W�z��</returns>
		EdgeVector GetEdges(const EnumType from) const {
			if (m_edgesMap.count(from) == 0) {
				return EdgeVector();
			}

			return m_edgesMap.at(from);
		}

		/// <summary>
		/// �G�b�W�A�z�z����擾
		/// </summary>
		/// <returns>�G�b�W�A�z�z��</returns>
		EdgeVectorMap GetEdgesMap() {
			return m_edgesMap;
		}

		/// <summary>
		/// �m�[�h�̒ǉ�
		/// </summary>
		/// <param name="type">�m�[�h�̃^�C�v</param>
		/// <param name="node">�m�[�h�̃|�C���^</param>
		virtual void AddNode(const EnumType type, const std::shared_ptr<NodeType>& node) {
			m_nodes[type] = node;
		}

		/// <summary>
		/// �m�[�h�̍폜
		/// </summary>
		/// <param name="node">�폜�������m�[�h</param>
		void RemoveNode(const std::shared_ptr<NodeType>& node) {
			//maru::MyUtility::RemoveVec(m_nodes, node);
		}

		/// <summary>
		/// �G�b�W�̒ǉ�
		/// </summary>
		/// <param name="edge">�ǉ��������G�b�W</param>
		void AddEdge(const std::shared_ptr<EdgeType>& edge) {
			m_edgesMap[edge->GetFromType()].push_back(edge);
		}

		/// <summary>
		/// �G�b�W�̒ǉ�
		/// </summary>
		/// <param name="from">���̃^�C�v</param>
		/// <param name="to">�J�ڐ�̃^�C�v</param>
		/// <param name="isTransitionFunc">�J�ڏ���</param>
		/// <param name="isEndTransition">�I�����ɑJ�ڂ��邩�ǂ���</param>
		void AddEdge(const EnumType from, const EnumType to,
			const std::function<bool(const TransitionStructMember& transition)>& isTransitionFunc,
			const bool isEndTransition = false)
		{
			auto newEdge = std::make_shared<EdgeType>(from, to, isTransitionFunc, static_cast<int>(to),isEndTransition);
			AddEdge(newEdge);
		}

		/// <summary>
		/// �m�[�h�̐��̎擾
		/// </summary>
		/// <returns>�m�[�h�̐�</returns>
		int GetNumNode() const {
			return static_cast<int>(m_nodes.size());
		}

		/// <summary>
		/// �G�b�W�̐��̎擾
		/// </summary>
		/// <param name="from">�ǂ̃m�[�h����L�т�G�b�W���w��</param>
		/// <returns>�G�b�W�̐�</returns>
		int GetNumEdge(const EnumType from) const {
			return static_cast<int>(m_edgesMap.count(from));
		}

		/// <summary>
		/// �G�b�W�A�z�z��̐����擾
		/// </summary>
		/// <returns>�G�b�W�̘A�z�z��</returns>
		int GetNumEdgeMap() const {
			return static_cast<int>(m_edgesMap.size());
		}

		/// <summary>
		/// �A�N�e�B�u�ȃO���t���ǂ���
		/// </summary>
		/// <returns>�A�N�e�B�u�ȃO���t�Ȃ�true</returns>
		bool IsActive() {
			return m_isActive;
		}

		/// <summary>
		/// �O���t�̃A�N�e�B�u��Ԃ�ݒ�
		/// </summary>
		/// <param name="isActive">�O���t�̃A�N�e�B�u���</param>
		void SetActive(const bool isActive) {
			m_isActive = isActive;
		}

		/// <summary>
		/// �m�[�h�̗L���𔻒f
		/// </summary>
		/// <returns>�m�[�h��������݂��Ȃ��̂Ȃ��true</returns>
		bool IsEmpty() const {
			return static_cast<int>(m_nodes.size()) == 0 ? true : false;
		}
	};

}

//endbasecross


