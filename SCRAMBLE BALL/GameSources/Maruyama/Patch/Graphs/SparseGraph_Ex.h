
/*!
@file SparseGraphBase.h
@brief SparseGraphBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;

		//template<class NodeType, class EdgeType,
		//	std::enable_if_t<
		//		std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
		//		std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
		//	std::nullptr_t
		//> = nullptr>
		//class SparseGraph;

		//--------------------------------------------------------------------------------------
		///	�ėp�^�O���t(�O��쐬����SparseGraph�̉��ǌ^�A�����I��SparseGraph�͍폜�\��)
		//--------------------------------------------------------------------------------------
		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		>>
		class SparseGraph
		{
		public:
			using NodeMap = std::unordered_map<int, std::shared_ptr<NodeType>>;
			using EdgeVector = std::vector<std::shared_ptr<EdgeType>>;
			using EdgeVectorMap = std::unordered_map<int, EdgeVector>;

		private:
			bool m_isActive = true;		//�A�N�e�B�u���ǂ���

			//int m_nextNodeIndex = 0;	//���̃m�[�h�̃C���f�b�N�X

		protected:
			NodeMap m_nodeMap;			//�m�[�h���i�[����z��
			EdgeVectorMap m_edgesMap;	//�G�b�W���i�[����z��

		public:
			SparseGraph() = default;
			virtual ~SparseGraph() = default;

			/// <summary>
			/// �w�肵���m�[�h�̎擾
			/// </summary>
			/// <param name="index">�m�[�h�C���f�b�N�X</param>
			/// <returns>�w�肵���m�[�h</returns>
			virtual std::shared_ptr<NodeType> GetNode(const int index) const {
				if (m_nodeMap.count(index) == 0) {
					return nullptr;
				}

				return m_nodeMap.at(index);
			}

			/// <summary>
			/// �m�[�h�̔z����擾
			/// </summary>
			/// <returns>�m�[�h�z��</returns>
			virtual NodeMap GetNodes() const {
				return m_nodeMap;
			}

			/// <summary>
			/// �w�肵���m�[�h��A������G�b�W�̎擾
			/// </summary>
			/// <param name="from">��O�̃C���f�b�N�X</param>
			/// <param name="to">��̃C���f�b�N�X</param>
			/// <returns>�w�肵���m�[�h��A������G�b�W�̎擾</returns>
			virtual std::shared_ptr<EdgeType> GetEdge(const int from, const int to) const {
				//���݂��Ȃ�������nullptr��Ԃ��B
				if (m_edgesMap.count(0) == 0) {
					return nullptr;
				}

				auto edges = m_edgesMap.at(from);
				for (const auto& edge : edges) {
					if (edge->GetToIndex() == to) {
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
			virtual EdgeVector GetEdges(const int from) const {
				if (m_edgesMap.count(from) == 0) {
					return EdgeVector();
				}

				return m_edgesMap.at(from);
			}

			/// <summary>
			/// �G�b�W�A�z�z����擾
			/// </summary>
			/// <returns>�G�b�W�A�z�z��</returns>
			virtual EdgeVectorMap GetEdgesMap() const {
				return m_edgesMap;
			}

			/// <summary>
			/// 
			/// </summary>
			/// <param name="node"></param>
			/// <returns></returns>
			virtual std::shared_ptr<NodeType> AddNode(const std::shared_ptr<NodeType>& node) {
				return AddNode(node->GetIndex(), node);
			}

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			/// <param name="index">�m�[�h�̃^�C�v</param>
			/// <param name="node">�m�[�h�̃|�C���^</param>
			virtual std::shared_ptr<NodeType> AddNode(const int index, const std::shared_ptr<NodeType>& node) {
				m_nodeMap[index] = node;
				return node;
			}

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			template<class... Ts,
				std::enable_if_t<
					std::is_constructible_v<NodeType, const int, const Ts...>,	//�R���X�g���N�^�̈�������������B
				std::nullptr_t
			> = nullptr>
			std::shared_ptr<NodeType> AddNode(const int index, Ts&&... params) {
				//int index = GetNextNodeIndex();
				auto node = std::make_shared<NodeType>(index, params...);
				return AddNode(index, node);
			}

			/// <summary>
			/// �m�[�h�̍폜
			/// </summary>
			/// <param name="node">�폜�������m�[�h</param>
			virtual void RemoveNode(const std::shared_ptr<NodeType>& node) {
				//maru::MyUtility::RemoveVec(m_nodeMap, node);
			}

			/// <summary>
			/// �G�b�W�̒ǉ�
			/// </summary>
			/// <param name="edge">�ǉ��������G�b�W</param>
			virtual std::shared_ptr<EdgeType> AddEdge(const std::shared_ptr<EdgeType>& edge) {
				int index = edge->GetFromIndex();
				if (m_edgesMap.count(index) == 0) {
					m_edgesMap[index] = EdgeVector();
				}

				m_edgesMap[index].push_back(edge);

				return edge;
			}

			/// <summary>
			/// �G�b�W�̒ǉ�
			/// </summary>
			/// <param name="from">���̃^�C�v</param>
			/// <param name="to">�J�ڐ�̃^�C�v</param>
			/// <param name="isTransitionFunc">�J�ڏ���</param>
			/// <param name="isEndTransition">�I�����ɑJ�ڂ��邩�ǂ���</param>
			template<class... Ts,
				std::enable_if_t<
					std::is_constructible_v<EdgeType, std::shared_ptr<NodeType>&, std::shared_ptr<NodeType>&, Ts...>,	//�R���X�g���N�^�̈����̐�������ۏ؂���
				std::nullptr_t
			> = nullptr>
			std::shared_ptr<EdgeType> AddEdge(const int fromIndex, const int toIndex, Ts&&... params)
			{
				//�ǂ��炩�̃m�[�h�����݂��Ȃ��Ȃ琶���ł��Ȃ��B
				auto fromNode = GetNode(fromIndex);
				auto toNode = GetNode(toIndex);
				if (!fromNode || !toNode) {
					return nullptr;
				}

				auto newEdge = std::make_shared<EdgeType>(fromNode, toNode, params...);
				return AddEdge(newEdge);
			}

			/// <summary>
			/// �m�[�h�̐��̎擾
			/// </summary>
			/// <returns>�m�[�h�̐�</returns>
			int GetNumNode() const noexcept {
				return static_cast<int>(m_nodeMap.size());
			}

			/// <summary>
			/// �G�b�W�̐��̎擾
			/// </summary>
			/// <param name="from">�ǂ̃m�[�h����L�т�G�b�W���w��</param>
			/// <returns>�G�b�W�̐�</returns>
			virtual int GetNumEdge(const int from) const noexcept {
				return static_cast<int>(m_edgesMap.count(from));
			}

			/// <summary>
			/// �G�b�W�A�z�z��̐����擾
			/// </summary>
			/// <returns>�G�b�W�̘A�z�z��</returns>
			int GetNumEdgeMap() const noexcept {
				return static_cast<int>(m_edgesMap.size());
			}

			/// <summary>
			/// �S�ẴG�b�W�̐�
			/// </summary>
			/// <returns>�S�ẴG�b�W�̐�</returns>
			int GetNumAllEdges() const {
				int count = 0;
				for (auto& pair : m_edgesMap) {
					count +=  static_cast<int>(pair.second.size());
				}

				return count;
			}

			/// <summary>
			/// ���̃m�[�h�̃C���f�b�N�X���擾
			/// </summary>
			/// <returns>���̃m�[�h�C���f�b�N�X</returns>
			//_NODISCARD int GetNextNodeIndex() const noexcept { return m_nextNodeIndex; }

			/// <summary>
			/// �����C���f�b�N�X�m�[�h�����݂��邩�ǂ���
			/// </summary>
			/// <param name="index">�m�F�������C���f�b�N�X</param>
			/// <returns>�����C���f�b�N�X�m�[�h�����݂���Ȃ�true</returns>
			bool IsSomeIndexNode(const int index) const {
				return m_nodeMap.count(index) != 0;	//�w�肵���C���f�b�N�X�m�[�h���������݂���Ȃ�A�����C���f�b�N�X�m�[�h�����݂���B
			}

			/// <summary>
			/// �����m�[�h���Ȃ��G�b�W�����݂��邩�ǂ���
			/// </summary>
			/// <param name="fromIndex">��O�̃m�[�h�C���f�b�N�X</param>
			/// <param name="toIndex">�J�ڐ�̃m�[�h�C���f�b�N�X</param>
			/// <returns>���݂���Ȃ�true</returns>
			bool IsSomeIndexEdge(const int fromIndex, const int toIndex) const {
				//formIndex�����݂��Ȃ��Ȃ�A�V�K�G�b�W
				if (m_edgesMap.count(fromIndex) == 0) {
					return false;
				}

				//�����C���f�b�N�X�����݂���Ȃ�Atrue
				for (auto& edge : m_edgesMap.at(fromIndex)) {
					if (edge->GetToIndex() == toIndex) {
						return true;
					}
				}

				return false;	//���݂��Ȃ����߁A�����łȂ�
			}

			/// <summary>
			/// �A�N�e�B�u�ȃO���t���ǂ���
			/// </summary>
			/// <returns>�A�N�e�B�u�ȃO���t�Ȃ�true</returns>
			bool IsActive() const noexcept {
				return m_isActive;
			}


			/// <summary>
			/// �O���t�̃A�N�e�B�u��Ԃ�ݒ�
			/// </summary>
			/// <param name="isActive">�O���t�̃A�N�e�B�u���</param>
			void SetActive(const bool isActive) noexcept {
				m_isActive = isActive;
			}

			/// <summary>
			/// �m�[�h�̗L���𔻒f
			/// </summary>
			/// <returns>�m�[�h��������݂��Ȃ��̂Ȃ��true</returns>
			bool IsEmpty() const noexcept {
				return static_cast<int>(m_nodeMap.size()) == 0 ? true : false;
			}
		};

	}
}