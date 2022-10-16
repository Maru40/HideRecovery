/*!
@file GraphAstar.h
@brief GraphAstar�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarEdge;
	class Heuristic;

	template<class node_type, class edge_type>
	class SparseGraph;

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^
	//--------------------------------------------------------------------------------------
	struct OpenData {
		ex_weak_ptr<NavGraphNode> node;  //�m�[�h�̃|�C���^
		float range;            //������
		float heuristic;        //�q���[�X���b�N����
		bool isActive = true;   //Active��Ԃ��ǂ���

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		OpenData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="node">�m�[�h</param>
		/// <param name="range">������</param>
		/// <param name="heuristic">�q���[�X���b�N����</param>
		OpenData(const std::shared_ptr<NavGraphNode>& node, const float& range, const float& heuristic);

		/// <summary>
		/// �������ƃq���[�X���b�N�����̍��v��Ԃ�
		/// </summary>
		/// <returns>���v�l</returns>
		float GetSumRange() const;
	};


	//--------------------------------------------------------------------------------------
	///	Astar�̊Ǘ�������O���t
	//--------------------------------------------------------------------------------------
	class GraphAstar
	{
	public:
		using GraphType = SparseGraph<NavGraphNode, AstarEdge>;
		using GraphMap = std::unordered_map<int, std::shared_ptr<GraphType>>;

	private:

		std::shared_ptr<GraphType> m_baseGraph = nullptr;	//�O���t�̃f�[�^
		GraphMap m_graphMap;								//�O���t�̃G���A�C���f�b�N�X���Ƃɕ������}�b�v
		std::shared_ptr<Heuristic> m_heuristic = nullptr;	//�q���[�X���b�N�̐��l���v�Z����N���X�B

		std::map<int, OpenData> m_openDataMap;             //�I�[�v���f�[�^��Map
		std::stack<std::shared_ptr<NavGraphNode>> m_route; //�����������[�g

		bool m_isRouteEnd = true;        //���[�g�̍Ō�ɗ������ǂ����H

		std::vector<int> m_debugIndices; //�f�o�b�O�p�̃C���f�b�N�X

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="graph">�O���t�f�[�^</param>
		GraphAstar(const std::shared_ptr<GraphType>& graph);

		/// <summary>
		/// �ŒZ�m�[�h�̒��ō��~�����m�[�h�̏ꏊ���擾����B
		/// </summary>
		/// <param name="objPtr">���݂��̏������s���Ă���I�u�W�F�N�g</param>
		/// <returns>���݌������ׂ��m�[�h�̃|�W�V����</returns>
		Vec3 CalucTargetNode(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// �����m�[�h�ƃG�b�W����A�ǂ̃��[�g����ԋ߂�������
		/// </summary>
		/// <param name="self">�J�n�̃I�u�W�F�N�g</param>
		/// <param name="target">�^�[�Q�b�g�ƂȂ�I�u�W�F�N�g</param>
		void SearchAstarStart(const std::shared_ptr<GameObject>& self, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// �����m�[�h�ƃG�b�W����A�ǂ̃��[�g����ԋ߂�������
		/// </summary>
		/// <param name="self">�J�n�̃I�u�W�F�N�g</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		void SearchAstarStart(const std::shared_ptr<GameObject>& self, const Vec3& targetPos);

		/// <summary>
		/// �����m�[�h�ƃG�b�W����A�ǂ̃��[�g����ԋ߂�������
		/// </summary>
		/// <param name="selfPos">�J�n�̃|�W�V����</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		void SearchAstarStart(const Vec3& selfPos, const Vec3& targetPos);

		/// <summary>
		/// �����_���ȃ��[�g���v��
		/// </summary>
		/// <param name="selfPosition">�������g�̃m�[�h</param>
		/// <returns>�����������[�g��Ԃ�</returns>
		std::vector<Vec3> CalculateRandomRoute(const Vec3& selfPosition);

	private:
		/// <summary>
		/// Route�\�z�J�n
		/// </summary>
		/// <param name="selfNearNode">�������g�̃m�[�h</param>
		/// <param name="targetNearNode">���ǂ蒅�������m�[�h</param>
		void SearchAstarStart(const std::shared_ptr<NavGraphNode>& selfNearNode, const std::shared_ptr<NavGraphNode>& targetNearNode);

		/// <summary>
		/// �I�[�v���f�[�^�����̊�ƂȂ�m�[�h�𐶐��B
		/// </summary>
		/// <param name="initialNode">�����m�[�h</param>
		/// <returns>��̃m�[�h</returns>
		std::shared_ptr<NavGraphNode> CalculateCreateOpenDataBaseNode(const std::shared_ptr<NavGraphNode>& initialNode);

		/// <summary>
		/// �I�[�v���f�[�^�̐���
		/// </summary>
		/// <param name="baseNode">��ƂȂ�m�[�h</param>
		/// <param name="graph">�O���t�f�[�^</param>
		/// <returns></returns>
		bool CreateOpenData(const ex_weak_ptr<NavGraphNode>& baseNode, const std::shared_ptr<GraphType>& graph);

		/// <summary>
		/// �ŒZ���[�g�̐���
		/// </summary>
		/// <param name="initialNode">�����m�[�h</param>
		/// <param name="targetNode">���ǂ蒅�������m�[�h</param>
		/// <param name="graph">�O���t�f�[�^</param>
		void CreateRoute(
			const std::shared_ptr<NavGraphNode>& initialNode, 
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		);

		/// <summary>
		/// ���[�v���ĒT���o�H�𑪂�B
		/// </summary>
		/// <param name="initialNode">�����X�^�[�g�m�[�h</param>
		void LoopSearchAstar(const std::shared_ptr<NavGraphNode>& initialNode, const std::shared_ptr<NavGraphNode>& targetNode);

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �O���t�̃R�s�[��Ԃ�
		/// </summary>
		/// <returns>�O���t�̃R�s�[</returns>
		std::shared_ptr<GraphType> CreateCopyGraph();

		/// <summary>
		/// �����������[�g�̍Ō�܂ŗ�����true
		/// </summary>
		/// <returns>�Ō�܂ŗ�����true</returns>
		bool IsRouteEnd() {
			if (m_route.size() == 0) {
				m_isRouteEnd = true;
			}

			return m_isRouteEnd;
		}

		/// <summary>
		/// �O���t�̎擾
		/// </summary>
		/// <returns>�O���t</returns>
		const std::shared_ptr<const GraphType> GetGraph() const {
			return m_baseGraph;
		}

		/// <summary>
		/// �G���A���Ƃɕ������O���t�̎擾
		/// </summary>
		/// <param name="areaIndex">�G���A�C���f�b�N�X</param>
		/// <returns>�G���A���Ƃɕ������O���t</returns>
		const std::shared_ptr<const GraphType> GetGraph(const int areaIndex) const {
			return m_graphMap.at(areaIndex);
		}

		/// <summary>
		/// �O���t���o�^����Ă��邩�ǂ���
		/// </summary>
		/// <param name="areaIndex">�G���A�C���f�b�N�X</param>
		/// <returns>�O���t</returns>
		bool HasGraph(const int areaIndex) const {
			return m_graphMap.count(areaIndex) != 0;	//0�łȂ��Ȃ�true
		}

		/// <summary>
		/// ���[�g�̎擾
		/// </summary>
		/// <returns>���[�g</returns>
		const std::stack<std::shared_ptr<NavGraphNode>> GetRoute() const {
			return m_route;
		}

		/// <summary>
		/// ���[�g���x�N�^�[�z��ɂ��ĕԂ��B
		/// </summary>
		/// <returns></returns>
		std::vector<std::shared_ptr<NavGraphNode>> GetRouteVec() const;

		/// <summary>
		/// �m�ۂ������[�g�̃|�W�V�����Q
		/// </summary>
		/// <returns>���[�g�̃|�W�V�����Q��Ԃ�</returns>
		std::vector<Vec3> GetRoutePositions() const;

		/// <summary>
		/// �m�[�h�̒ǉ�
		/// </summary>
		/// <param name="position">�ǉ��������m�[�h�̃|�W�V����</param>
		/// <returns>�ǉ������m�[�h�̃C���f�b�N�X</returns>
		//int AddNode(const Vec3& position);

		template<class... Ts,
			std::enable_if_t<std::is_constructible_v<NavGraphNode, int, Ts...>, std::nullptr_t> = nullptr>
		int AddNode(Ts&&... params) {
			//�m�[�h�̐���
			auto index = m_baseGraph->GetNextFreeNodeIndex();
			auto newNode = std::make_shared<NavGraphNode>(index, params...);

			m_baseGraph->AddNode(newNode);	//�x�[�X�O���t�ɓo�^
			AddAreaGraphNode(newNode);		//�G���A�C���f�b�N�X���Ƃ̃}�b�v�ɓo�^����B
			
			return index++;
		}

		/// <summary>
		/// �G���A�C���f�b�N�X���Ƃɕ������G���A�}�b�v�ɓo�^����B
		/// </summary>
		/// <param name="node">�o�^����m�[�h</param>
		void AddAreaGraphNode(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// �m�[�h�̒ǉ�
		/// </summary>
		/// <param name="position">�ǉ��������m�[�h�̃|�W�V����</param>
		/// <param name="obstacleObjs">��Q���Ώۂ̃I�u�W�F�N�g</param>
		/// <param name="excluteObjs">��Q�����珜���Ώۂ̃I�u�W�F�N�g</param>
		/// <returns>�ǉ������m�[�h�̃C���f�b�N�X</returns>
		int AddNode(const Vec3& position,
			const vector<shared_ptr<GameObject>>& obstacleObjs, const vector<shared_ptr<GameObject>>& excluteObjs);

		/// <summary>
		/// �m�[�h�̍폜
		/// </summary>
		/// <param name="index">�폜�������m�[�h�̃C���f�b�N�X</param>
		void RemoveNode(const int& index);

		/// <summary>
		/// �m�[�h�̐e�I�u�W�F�N�g��ݒ肷��B
		/// </summary>
		/// <param name="parent">�y�A�����g�ɂ���I�u�W�F�N�g</param>
		void SetNodesParent(const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// �G�b�W�̒ǉ�
		/// </summary>
		void AddEdges();

		/// <summary>
		/// �m�[�h�̑S�ăN���A
		/// </summary>
		void ClearNodes();

		/// <summary>
		/// �G�b�W�̑S�ăN���A
		/// </summary>
		void ClearEdges();

		/// <summary>
		/// Astar�̃��Z�b�g
		/// </summary>
		void ResetAstar(); 

		/// <summary>
		/// �S�ă��Z�b�g
		/// </summary>
		void ResetAll();


	private:
		//�f�o�b�O����-------------------------------------------------------

		/// <summary>
		/// �f�o�b�O�\��
		/// </summary>
		void DebugDraw();

	};
}

//endbaecross