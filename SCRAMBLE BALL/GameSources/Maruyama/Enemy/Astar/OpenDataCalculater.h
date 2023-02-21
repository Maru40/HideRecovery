/*!
@file OpenDataCalculater.h
@brief OpenDataCalculater�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

//#include "Maruyama/StateMachine/NodeBase.h"
//#include "Maruyama/StateMachine/EdgeBase.h"

namespace basecross {

	namespace maru
	{

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class NodeBase;
		class EdgeBase;
		class AstarNode;
		class AstarEdge;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		>>
		class SparseGraph;

		//--------------------------------------------------------------------------------------
		///	�I�[�v���f�[�^(�V�K��)
		//--------------------------------------------------------------------------------------
		struct OpenData
		{
			std::weak_ptr<AstarNode> parentNode;	//�e�m�[�h
			std::weak_ptr<AstarNode> selfNode;		//�������g�̃m�[�h
			float range;		//���R�X�g
			float heuristic;	//�q���[�X���b�N�R�X�g
			bool isActive;		//�L�����

			OpenData(
				const std::shared_ptr<AstarNode>& parentNode,
				const std::shared_ptr<AstarNode>& selfNode,
				const float range,
				const float heuristic
			);

			/// <summary>
			/// ���R�X�g�ƃq���[�X���b�N�R�X�g�̍��v�l��Ԃ�
			/// </summary>
			/// <returns>���R�X�g�ƃq���[�X���b�N�R�X�g�̍��v�l</returns>
			float GetSumRange() const;

			bool operator ==(const OpenData& data) const;
		};

		//--------------------------------------------------------------------------------------
		///	�I�[�v���f�[�^�v�Z�N���X(�V�K��)
		//--------------------------------------------------------------------------------------
		class OpenDataCalculater
		{
		public:
			using GraphType = SparseGraph<AstarNode, AstarEdge, nullptr>;		//�O���t�^�C�v
			using DataPtrList = std::list<std::shared_ptr<OpenData>>;	//�f�[�^���X�g

		private:
			std::stack<std::weak_ptr<AstarNode>> m_route;

			std::weak_ptr<AstarNode> m_otherAreaNode;		//�ʂ̃G���A�m�[�h(�����I�ɍ폜)

		public:
			OpenDataCalculater();

			virtual ~OpenDataCalculater() = default;

		private:
			/// <summary>
			/// �q���[�X���b�N�R�X�g���v�Z���ĕԂ�
			/// </summary>
			/// <param name="currentNode">���݂̃m�[�h</param>
			/// <param name="targetNode">�ړI�n�̃m�[�h</param>
			/// <returns>�q���[�X���b�N�R�X�g</returns>
			float CalculateHeuristicRange(
				const std::shared_ptr<AstarNode> currentNode,
				const std::shared_ptr<AstarNode> targetNode
			) const;

			/// <summary>
			/// �I�[�v���f�[�^�𐶐����邽�߂̊�ƂȂ�I�[�v���f�[�^������(��Ԋ��Ғl�̍����m�[�h���擾����B)
			/// </summary>
			/// <returns>�I�[�v���f�[�^�𐶐����邽�߂̊J�n�m�[�h</returns>
			std::shared_ptr<OpenData> FindSearchBaseOpenData(DataPtrList& openDataList);

			/// <summary>
			/// ���̃G���A���ڕW�ɂȂ��Ă��邩�ǂ���(�܂�������)
			/// </summary>
			/// <param name="startNode">�J�n�m�[�h</param>
			/// <param name="targetAreaIndex">�ڕW�G���A</param>
			/// <returns>���̃G���A���ڕW�Ȃ�true</returns>
			bool IsOhterAreaTarget(const std::shared_ptr<AstarNode>& startNode, const int targetAreaIndex);

			/// <summary>
			/// �I�[�v���f�[�^�𐶐�����B
			/// </summary>
			/// <param name="baseNode">�x�[�X�ƂȂ�m�[�h</param>
			/// <param name="graph">�O���t�̎Q��</param>
			/// <returns>�I�[�v���f�[�^</returns>
			bool CreateOpenDatas(
				DataPtrList& openDataList,
				DataPtrList& closeDataList,
				const std::shared_ptr<OpenData>& openData,
				const std::shared_ptr<GraphType>& graph,
				const std::shared_ptr<AstarNode>& startNode,
				const std::shared_ptr<AstarNode>& targetNode,
				const int targetAreaIndex = -1
			);

			/// <summary>
			/// ���[�g�̐���
			/// </summary>
			/// <param name="openDataList">�I�[�v���f�[�^���X�g</param>
			/// <param name="targetNode">�ڕW�m�[�h</param>
			/// <returns>����������������true</returns>
			std::stack<std::weak_ptr<AstarNode>> CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<AstarNode>& targetNode);

			/// <summary>
			/// �I�[�v���f�[�^�̎擾
			/// </summary>
			/// <param name="openDataList">�I�[�v���f�[�^���X�g</param>
			/// <param name="node">�m�F�������m�[�h</param>
			/// <returns>�����f�[�^������Ȃ炻�̃f�[�^��Ԃ�</returns>
			std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<AstarNode>& node);

			/// <summary>
			/// �I�[�v���f�[�^�̎擾
			/// </summary>
			/// <param name="openDataList">�I�[�v���f�[�^���X�g</param>
			/// <param name="openData">�����f�[�^�����݂��邩���f</param>
			/// <returns>�����f�[�^������Ȃ炻�̃f�[�^��Ԃ�</returns>
			std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData);

			/// <summary>
			/// �o�^���ꂽ�I�[�v���f�[�^�����f
			/// </summary>
			/// <param name="openDataList">�f�[�^���X�g</param>
			/// <param name="openData">�m�F�������f�[�^</param>
			/// <returns>�o�^����Ă�����true</returns>
			bool IsRegisterData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData);

			/// <summary>
			/// �I�[�v���f�[�^�̒ǉ�
			/// </summary>
			/// <param name="openDatas">�ǉ��������I�[�v���f�[�^�̔z��</param>
			/// <param name="openData">�ǉ��������I�[�v���f�[�^</param>
			/// <returns></returns>
			bool AddOpenData(DataPtrList& openDataList, DataPtrList& closeDataList, const std::shared_ptr<OpenData>& openData);

		public:

			/// <summary>
			/// Astar�𗘗p�����o�H�T���J�n
			/// </summary>
			/// <param name="startNode">�J�n�m�[�h</param>
			/// <param name="targetNode">�ڕW�m�[�h</param>
			/// <param name="graph">�T���ɗ��p����O���t</param>
			/// <returns>�o�H�T��������������true</returns>
			bool StartSearchAstar(
				const std::shared_ptr<AstarNode>& startNode,
				const std::shared_ptr<AstarNode>& targetNode,
				const std::shared_ptr<GraphType>& graph,
				const int targetAreaIndex = -1
			);

			/// <summary>
			/// ���[�g�̎擾
			/// </summary>
			std::stack<std::weak_ptr<AstarNode>> GetRoute();

		};
	}

}