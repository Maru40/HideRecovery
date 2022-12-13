
/*!
@file GraphBase_Ex.h
@brief GraphBase_Ex
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {
		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;

		//template<class EnumType, class NodeType, class EdgeType>
		//class GraphBase;

		//--------------------------------------------------------------------------------------
		/// �O���t�̊��N���X�B�O��GraphBase�Ƃ͊֌W�Ȃ��A���S�V�K�A�O��͍̂폜�\��
		//--------------------------------------------------------------------------------------
		template<class EnumType, class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType> &&	//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		> = nullptr>
		class GraphBase //: public SparseGraphBase<NodeType, EdgeType>
		{
		public:
			//using�f�B���N�e�B�u

			//using NodeType = NodeBase;
			//using EdgeType = EdgeBase;

			using NodeMap = std::map<EnumType, std::shared_ptr<NodeType>>;
			using EdgeVector = std::vector<std::shared_ptr<EdgeType>>;
			using EdgeVectorMap = std::map<EnumType, EdgeVector>;

		private:
			NodeMap m_nodeMap;			//�m�[�h���i�[����z��
			EdgeVectorMap m_edgesMap;	//�G�b�W���i�[����z��

			bool m_isActive = true;		//�A�N�e�B�u���ǂ���

		public:
			GraphBase() = default;
			virtual ~GraphBase() = default;

			/// <summary>
			/// �w�肵���m�[�h�̎擾
			/// </summary>
			/// <param name="type">�m�[�h�^�C�v</param>
			/// <returns>�w�肵���m�[�h</returns>
			std::shared_ptr<NodeType> GetNode(const EnumType type) const {
				return m_nodeMap.at(type);
			}

			/// <summary>
			/// �m�[�h�̔z����擾
			/// </summary>
			/// <returns>�m�[�h�z��</returns>
			NodeMap GetNodes() const {
				return m_nodeMap;
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
				for (std::shared_ptr<EdgeBase>& edge : edges) {
					if (edge->GetToType<EnumType>() == to) {
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
			EdgeVectorMap GetEdgesMap() const {
				return m_edgesMap;
			}

			/// <summary>
			/// �m�[�h�̒ǉ�
			/// </summary>
			/// <param name="type">�m�[�h�̃^�C�v</param>
			/// <param name="node">�m�[�h�̃|�C���^</param>
			virtual void AddNode(const EnumType type, const std::shared_ptr<NodeType>& node) {
				m_nodeMap[type] = node;
			}

			/// <summary>
			/// �m�[�h�̍폜
			/// </summary>
			/// <param name="node">�폜�������m�[�h</param>
			void RemoveNode(const std::shared_ptr<NodeType>& node) {
				//maru::MyUtility::RemoveVec(m_nodeMap, node);
			}

			/// <summary>
			/// �G�b�W�̒ǉ�
			/// </summary>
			/// <param name="edge">�ǉ��������G�b�W</param>
			void AddEdge(const std::shared_ptr<EdgeType>& edge) {
				auto type = static_cast<EnumType>(edge->GetFromIndex());
				m_edgesMap[type].push_back(edge);
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
					std::is_constructible_v<EdgeType, std::shared_ptr<NodeBase>&, std::shared_ptr<NodeBase>&, Ts...>,	//�R���X�g���N�^�̈����̐�������ۏ؂���
					std::nullptr_t
				> = nullptr>
			void AddEdge(const EnumType fromType, const EnumType toType, Ts&&... params)
			{
				auto newEdge = std::make_shared<EdgeType>(GetNode(fromType), GetNode(toType), params...);
				AddEdge(newEdge);
			}

			/// <summary>
			/// �m�[�h�̐��̎擾
			/// </summary>
			/// <returns>�m�[�h�̐�</returns>
			int GetNumNode() const {
				return static_cast<int>(m_nodeMap.size());
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