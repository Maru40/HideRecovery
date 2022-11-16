/*!
@file SparseGraph.h
@brief SparseGraph
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/Utility.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	using maru::Utility;

	//--------------------------------------------------------------------------------------
	///	�ėp�^�O���t
	//--------------------------------------------------------------------------------------
	template<class node_type, class edge_type>
	class SparseGraph
	{
	public:
		//�N���C�A���g����O���t�Ŏg���Ă���G�b�W�ƃm�[�h�ւ̊ȈՂȃA�N�Z�X���\�ɂ���B
		using NodeType = node_type;
		using EdgeType = edge_type;

		using NodeVector = std::vector<std::shared_ptr<node_type>>;
		using EdgeVector = std::vector<std::shared_ptr<edge_type>>;
		using EdgeMapVector = std::map<int, EdgeVector>;

	private:
		//���̃N���X���\������m�[�h
		NodeVector m_nodes;

		//�אڃG�b�W���X�g�̃x�N�^�[
		//(�e�m�[�h�̃C���f�b�N�X�́A���̃m�[�h�Ɋ֘A�t����ꂽ�G�b�W�̃��X�g�̃L�[�ƂȂ�)
		EdgeMapVector m_edges;

		//���̃m�[�h�͗L���O���t���ǂ����H
		bool m_isDigraph;

		//�ǉ������ׂ����̃m�[�h�C���f�b�N�X
		int m_nextNodeIndex;

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="digraph">�L�����ǂ���</param>
		SparseGraph(bool digraph) :
			m_nextNodeIndex(0),
			m_isDigraph(digraph)
		{}

		virtual ~SparseGraph() = default;

		/// <summary>
		/// �m�[�h�̐����擾����B
		/// </summary>
		/// <returns>�m�[�h�̐�</returns>
		int GetNumNode() const noexcept {
			return m_nodes.size();
		}

		/// <summary>
		/// �w�肳�ꂽ�C���f�b�N�X�̃m�[�h��Ԃ�
		/// </summary>
		/// <param name="index">�~�����m�[�h�̃C���f�b�N�X</param>
		/// <returns>�w�肳�ꂽ�C���f�N�X�̃m�[�h</returns>
		const std::shared_ptr<NodeType> GetNode(const int& index) const {
			for (const auto& node : m_nodes) {
				if (node->GetIndex() == index) {
					return node;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// �n���ꂽ�����Ԗڂɕۑ�����Ă���m�[�h�̎擾
		/// </summary>
		/// <param name="index">�~�����z��̃C���f�b�N�X</param>
		/// <returns>�m�[�h</returns>
		const std::shared_ptr<NodeType> GetArrayNumberNode(const int& index) {
			//�C���f�b�N�X�I�[�o�[�Ȃ�nullptr��Ԃ�
			return index < m_nodes.size() ? m_nodes[index] : nullptr;
		}

		/// <summary>
		/// �S�Ẵm�[�h��n��
		/// </summary>
		/// <returns>�S�Ẵm�[�h</returns>
		const std::vector<std::shared_ptr<NodeType>>& GetNodes() const {
			return m_nodes;
		}

		/// <summary>
		/// �m�[�h�̒��g���R�s�[���ēn��
		/// </summary>
		std::vector<std::shared_ptr<NodeType>> GetNodesCopy() const {
			std::vector<std::shared_ptr<NodeType>> results;

			for (const auto& node : m_nodes) {
				results.push_back(std::make_shared<NodeType>(*(node.get())));
			}

			return results;
		}

		/// <summary>
		/// �m�[�h�������Ă���C���f�b�N�X�ԍ��̎擾
		/// </summary>
		/// <param name="index">�擾�������C���f�b�N�X</param>
		/// <returns>�m�[�h�������Ă���C���f�b�N�X�ԍ��̎擾</returns>
		int GetCurrentNodeArrayIndex(const int& index) {
			for (int i = 0; i < m_nodes.size(); i++) {
				if (m_nodes[i]->GetIndex() == index) {
					return i;
				}
			}

			return 0;
		}

		/// <summary>
		/// �G�b�W�̎Q�Ƃ𓾂�
		/// </summary>
		/// <param name="from"></param>
		/// <param name="back"></param>
		/// <returns>�G�b�W�̎Q��</returns>
		const std::shared_ptr<EdgeType>& GetEdge(const int& from, const int& to) const {
			for (auto edge : m_edges[from]) {
				if (edge->GetTo() == to) {
					return edge;
				}
			}

			return nullptr;
		}

		/// <summary>
		/// �^����ꂽ��������n�܂�G�b�W���X�g��S�Ď擾
		/// </summary>
		/// <param name="from">�~�����G�b�W���X�g�̃C���f�b�N�X</param>
		/// <returns>�G�b�W���X�g</returns>
		const std::vector<std::shared_ptr<EdgeType>> GetEdges(const int& from) const {
			if (m_edges.count(from) == 0) {
				return std::vector<std::shared_ptr<EdgeType>>();
			}
			return m_edges.at(from);
		}

		/// <summary>
		/// �G�b�W�̘A�z�z���S�Ď擾
		/// </summary>
		/// <returns>�G�b�W�̘A�z�z��S��</returns>
		const EdgeMapVector& GetEdgesMap() const {
			return m_edges;
		}

		/// <summary>
		/// �G�b�W�̘A�z�z��̃R�s�[��n���B
		/// </summary>
		/// <returns>�G�b�W�̘A�z�z��R�s�[</returns>
		EdgeMapVector GetEdgesMapCopy() const {
			EdgeMapVector results;

			for (const auto& pair : m_edges) {
				results[pair.first] = EdgeVector();
				for (const auto& edge : pair.second) {
					results[pair.first].push_back(std::make_shared<EdgeType>(*(edge.get())));
				}
			}

			return results;
		}

		/// <summary>
		/// �G�b�W�̘A�z�z����Z�b�g����B
		/// </summary>
		/// <param name="edgesMap">�G�b�W�̘A�z�z��S��</param>
		void SetEdgesMap(const EdgeMapVector& edgesMap) {
			m_edges = edgesMap;
		}

		/// <summary>
		/// ���̃t���[�m�[�h�̃C���f�b�N�X���Q�Ƃ���B
		/// </summary>
		/// <returns>���̃t���[�m�[�h�̃C���f�b�N�X</returns>
		int GetNextFreeNodeIndex() {
			//���������ꍇ�͕ʂ̃C���f�b�N�X�ɂ��Ȃ���΂Ȃ�Ȃ�
			if (m_edges.find(m_nextNodeIndex) != m_edges.end()) {
				m_nextNodeIndex = CalculateNextFreeNodeIndex();
			}

			return m_nextNodeIndex;
		}

		/// <summary>
		/// ���̃O���t�̃m�[�h��ǉ����āA���̃C���f�b�N�X��Ԃ��B
		/// </summary>
		/// <param name="node">�ǉ��������m�[�h</param>
		/// <returns>�ǉ������m�[�h�̃C���f�b�N�X</returns>
		virtual int AddNode(const std::shared_ptr<NodeType>& node) {
			m_nodes.push_back(node);
			return m_nextNodeIndex++;
			//return (int)m_nodes.size() - 1;
		}

		/// <summary>
		/// �m�[�h�̔z����Z�b�g
		/// </summary>
		/// <param name="nodes">�Z�b�g�������m�[�h�̔z��</param>
		void SetNodes(const std::vector<std::shared_ptr<NodeType>>& nodes) {
			m_nodes = nodes;
		}

		/// <summary>
		/// �C���f�b�N�X��invalid_node_index�ɐݒ肷�邱�ƂŃm�[�h���폜����B
		/// </summary>
		/// <param name="index">�폜�������m�[�h�̃C���f�b�N�X</param>
		void RemoveNode(const int& index) {
			if (m_edges.find(index) == m_edges.end()) { //������Ȃ������珈�������Ȃ��B
				return;
			}

			//�m�[�h�̍폜
			Utility::RemoveVec(m_nodes, m_nodes[index]);

			//���̃m�[�h��edges�̍폜
			m_edges.erase(index);

			//�폜�����C���f�N�X���܂܂��G�b�W���폜
			for (int i = 0; i < m_edges.size(); i++) {
				if (m_edges.count(i) == 0) {
					continue;
				}

				for (auto& edge : m_edges[i]) {
					if (edge->GetTo() == index) {  //�s�悪index�ƈꏏ�Ȃ�
						RemoveEdge(edge->GetFrom(), edge->GetTo());
					}
				}
			}

			m_nextNodeIndex = index;
		}

		void RemoveNode(const std::shared_ptr<NodeType>& node) {
			//�m�[�h�̍폜
			bool isRemove = Utility::RemoveVec(m_nodes, node);

			int index = node->GetIndex();

			//���̃m�[�h��edges�̍폜
			m_edges.erase(index);
			
			std::vector<std::function<void()>> removeFunctions;	//�폜�֐���p��

			for (auto& node : GetNodes()) {
				auto i = node->GetIndex();
				if (m_edges.count(i) == 0) {
					continue;
				}

				auto& edges = m_edges[i];
				for (auto& edge : edges) {
					if (edge->GetTo() == index) {  //�s�悪index�ƈꏏ�Ȃ�
						removeFunctions.push_back([&, edge]() { RemoveEdge(edge->GetFrom(), edge->GetTo()); });
					}
				}
			}

			//�폜����
			for (auto& removeFunc : removeFunctions) {
				removeFunc();
			}

			m_nextNodeIndex = index;
		}

		/// <summary>
		/// �S�Ẵm�[�h�ɐe�I�u�W�F�N�g��ݒ肷��B
		/// </summary>
		/// <param name="parent">�e�I�u�W�F�N�g�ɐݒ肷��I�u�W�F�N�g</param>
		void SetNodesParent(const std::shared_ptr<GameObject>& parent) {
			for (auto& node : m_nodes) {
				node->SetParent(parent);
			}
		}

		/// <summary>
		/// �G�b�W�̒ǉ�
		/// </summary>
		/// <param name="edge">�ǉ��������G�b�W</param>
		virtual void AddEdge(const std::shared_ptr<EdgeType>& edge) {
			m_edges[edge->GetFrom()].push_back(edge);
		}

		/// <summary>
		/// �G�b�W�̍폜
		/// </summary>
		/// <param name="from">��O���̃m�[�h�C���f�b�N�X</param>
		/// <param name="back">�摤�̃m�[�h�C���f�b�N�X</param>
		void RemoveEdge(const int& from, const int& to) {
			if (to == 315) {
				int i = 0;
			}

			auto& edges = m_edges[from];

			auto iter = edges.begin();
			while (iter != edges.end()) {
				if ((*iter)->GetTo() == to) {
					iter = edges.erase(iter);
					break;
				}

				iter++;
			}
		}

		/// <summary>
		/// �n���ꂽ�C���f�b�N�X���܂܂��G�b�W��S�č폜����B(���ݎg�p�s��)
		/// </summary>
		/// <param name="index">�폜�������C���f�b�N�X</param>
		void RemoveEdge(const int& index) {
			vector<std::function<void()>> functions;

			for (const auto& pair : m_edges) {
				for (const auto& edge : pair.second) {
					if (edge->GetTo() == index) {
						functions.push_back([&, pair, index]() { pair.second.erase(index); });
					}
				}
			}

			for (auto& function : functions) {
				function();
			}
		}

		/// <summary>
		/// �m�[�h�̃N���A
		/// </summary>
		void ClearNodes() {
			m_nodes.clear();
		}

		/// <summary>
		/// �G�b�W�̃N���A
		/// </summary>
		void ClearEdges() {
			m_edges.clear();
		}

		/// <summary>
		/// ���̃O���t���̃m�[�h�̌���Ԃ��B
		/// </summary>
		/// <returns>�m�[�h�̌�</returns>
		int GetNumNodes() const {
			return static_cast<int>(m_nodes.size());
		}

		/// <summary>
		/// ���̃O���t���ɑ��݂���A�N�e�B�u�ȃm�[�h�̌���Ԃ��B
		/// </summary>
		/// <returns>�A�N�e�B�u�ȃm�[�h�̌�</returns>
		int GetNumActiveNodes() const {
			int num = 0;
			for (auto node : m_nodes) {
				if (node.IsActive()) {
					num++;
				}
			}
			return num;
		}

		/// <summary>
		/// �G�b�W�̐����擾
		/// </summary>
		/// <returns>�G�b�W�̐�</returns>
		int GetNumEdges() const {
			int num = 0;
			for (auto edges : m_edges) {
				num += (int)edges.second.size();
			}

			return num;
		}

		/// <summary>
		/// ���̃O���t���L�����ǂ�����Ԃ��B
		/// </summary>
		/// <returns>�L���Ȃ�true</returns>
		bool IsDigraph() const {
			return m_isDigraph;
		}

		/// <summary>
		/// �m�[�h�����݂��邩�ǂ�����Ԃ��B
		/// </summary>
		/// <returns>�m�[�h�����݂���Ȃ�true</returns>
		bool IsEmpty() const {
			return m_nodes.size() == 0 ? true, false;
		}

		/// <summary>
		/// �w�肵���C���f�b�N�X�����m�[�h�����̃O���t���ɑ��݂���ꍇ�Atrue��Ԃ��B
		/// </summary>
		/// <param name="index">�w�肵���C���f�b�N�X</param>
		/// <returns>�w�肵���C���f�b�N�X�����m�[�h�����݂���ꍇ��true</returns>
		bool IsPresent(int index) const {
			for (auto node : m_nodes) {
				if (node->GetIndex() == index) {
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// �S�ẴG�b�W�ƃm�[�h�����Z�b�g
		/// </summary>
		void ResetAll() {
			m_nodes.clear();
			m_edges.clear();
		}

	private:

		/// <summary>
		/// ���݋󂫂̂���m�[�h�̃C���f�b�N�X��T���o���B
		/// </summary>
		/// <returns>�󂫂̃C���f�b�N�X</returns>
		int CalculateNextFreeNodeIndex() {
			int index = (int)m_edges.size();  //���[�v���Ō�܂ł�������Ō�̃m�[�h�̎����󂫃m�[�h�ɂȂ�B
			if (index == 0) {
				return ++index;
			}

			for (int i = 0; i < m_edges.size(); i++) {
				if (m_edges.find(i) == m_edges.end()) {  //end�������炻�̃m�[�h���Ȃ�����
					index = i;
					break;
				}
			}

			return index;
		}
	};

}