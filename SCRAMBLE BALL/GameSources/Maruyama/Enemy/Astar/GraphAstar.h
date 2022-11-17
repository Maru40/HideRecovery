/*!
@file GraphAstar.h
@brief GraphAstar�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "AstarGraph.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarEdge;
	class Heuristic;

	template<class node_type, class edge_type>
	class SparseGraph;
	class AstarGraph;

	//--------------------------------------------------------------------------------------
	///	Astar�̊Ǘ�������O���t
	//--------------------------------------------------------------------------------------
	class GraphAstar
	{
	public:
		using GraphType = AstarGraph;
		using GraphMap = std::unordered_map<int, std::shared_ptr<GraphType>>;

	private:

		std::shared_ptr<GraphType> m_areaGraph = nullptr;	//�G���A�C���f�b�N�X�p�̃O���t����
		std::shared_ptr<GraphType> m_baseGraph = nullptr;	//�O���t�̃f�[�^(�G���A���̃O���t�f�[�^)
		GraphMap m_graphMap;								//�O���t�̃G���A�C���f�b�N�X���Ƃɕ������}�b�v

		std::stack<std::weak_ptr<NavGraphNode>> m_route;	//�����������[�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="graph">�O���t�f�[�^</param>
		GraphAstar(const std::shared_ptr<GraphType>& graph);

		virtual ~GraphAstar() = default;

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
		/// �����m�[�h�ƃG�b�W����A�ǂ̃��[�g����ԋ߂�������
		/// </summary>
		/// <param name="selfPos">�J�n�̃|�W�V����</param>
		/// <param name="targetPos">�^�[�Q�b�g�̃|�W�V����</param>
		/// <param name="areaIndex">�G���A�C���f�b�N�X</param>
		void SearchAstarStart(const Vec3& selfPosition, const Vec3& targetPosition, const int areaIndex, const int targetAreaIndex = -1);

		void SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const Vec3& targetPosition,
			const int areaIndex,
			const int targetAreaIndex = -1
		);

		void SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const int areaIndex,
			const int targetAreaIndex = -1
		);

		/// <summary>
		/// �����_���ȃ��[�g���v��
		/// </summary>
		/// <param name="selfPosition">�������g�̃m�[�h</param>
		/// <returns>�����������[�g��Ԃ�</returns>
		std::vector<Vec3> CalculateRandomRoute(const Vec3& selfPosition);

		/// <summary>
		/// �^�[�Q�b�g�ւ̃G���A�P�ʂ̑�܂��ȃ��[�g������
		/// </summary>
		std::vector<int> SearchAreaRouteIndices(const Vec3& selfPosition, const Vec3& targetPosition);

	private:
		/// <summary>
		/// Route�\�z�J�n
		/// </summary>
		/// <param name="selfNearNode">�������g�̃m�[�h</param>
		/// <param name="targetNearNode">���ǂ蒅�������m�[�h</param>
		void SearchAstarStart(
			const std::shared_ptr<NavGraphNode>& selfNearNode, 
			const std::shared_ptr<NavGraphNode>& targetNearNode,
			const std::shared_ptr<GraphType>& graph,
			const int targetAreaIndex = -1
		);

	public:

		/// <summary>
		/// ���ꂼ��̃O���t�̒��S�ʒu��ݒ肷��B
		/// (���݂�ImpactMap���琶�����Ă��邪�A���̕��@��������)
		/// </summary>
		void CreateAreaAstarGraph();

		/// <summary>
		/// ��ԋ߂��G���A�̃C���f�b�N�X���擾����B
		/// </summary>
		/// <param name="position">�����̃|�W�V����</param>
		/// <returns>��ԋ߂��G���A�̃C���f�b�N�X</returns>
		int SearchNearAreaIndex(const Vec3& position);

		/// <summary>
		/// ��ԋ߂��G���A�̃m�[�h���擾
		/// </summary>
		/// <param name="position">�����̃|�W�V����</param>
		/// <returns>��ԋ߂��G���A�̃|�W�V����</returns>
		std::shared_ptr<NavGraphNode> SearchNearAreaNode(const Vec3& position);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �O���t�̃R�s�[��Ԃ�
		/// </summary>
		/// <returns>�O���t�̃R�s�[</returns>
		std::shared_ptr<GraphType> CreateCopyGraph();

		/// <summary>
		/// �O���t�̃R�s�[��Ԃ�
		/// </summary>
		/// <param name="areaIndex">�R�s�[�������O���t</param>
		/// <returns>�O���t�̃R�s�[</returns>
		std::shared_ptr<GraphType> CreateCopyGraph(const std::shared_ptr<GraphType>& baseGraph);

		/// <summary>
		/// �O���t�̎擾
		/// </summary>
		/// <returns>�O���t</returns>
		const std::shared_ptr<const GraphType> GetGraph() const {
			return m_baseGraph;
		}

		const std::shared_ptr<GraphType> GetBaseGraph() const {
			return m_baseGraph;
		}

		/// <summary>
		/// �G���A���Ƃɕ������O���t�̎擾
		/// </summary>
		/// <param name="areaIndex">�G���A�C���f�b�N�X</param>
		/// <returns>�G���A���Ƃɕ������O���t</returns>
		const std::shared_ptr<const GraphType> GetGraph(const int areaIndex) const {
			if (m_graphMap.count(areaIndex) == 0) {
				return nullptr;
			}

			return m_graphMap.at(areaIndex);
		}

		const std::shared_ptr<GraphType> GetBaseGraph(const int areaIndex) const {
			if (m_graphMap.count(areaIndex) == 0) {
				return nullptr;
			}

			return m_graphMap.at(areaIndex);
		}

		const std::shared_ptr<GraphType> GetAreaGraph() const { 
			return m_areaGraph;
		}

		/// <summary>
		/// ���������\�ȏ�Ԃ̃G���A�O���t���擾
		/// </summary>
		/// <returns></returns>
		const std::shared_ptr<GraphType> GetReWiritingAreaGraph() const {
			return m_areaGraph;
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
		const std::stack<std::weak_ptr<NavGraphNode>> GetRoute() const {
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
		/// �G���A���Ƃ̃��[�g���G���A�C���f�b�N�X�z��Ŏ󂯎��B
		/// </summary>
		std::vector<int> GetRouteAreaIndex() const;

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

		void RemoveNode(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// �m�[�h�̐e�I�u�W�F�N�g��ݒ肷��B
		/// </summary>
		/// <param name="parent">�y�A�����g�ɂ���I�u�W�F�N�g</param>
		void SetNodesParent(const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// �G�b�W�̒ǉ�
		/// </summary>
		void AddEdges(const bool isRayHit = true);

		/// <summary>
		/// �G�b�W�̒ǉ�
		/// </summary>
		/// <param name="graph">�ǉ��������O���t</param>
		void AddEdges(const std::shared_ptr<GraphAstar::GraphType>& graph, const bool isRayHit = true);

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
		void ResetAllAstar();

	private:
		//�f�o�b�O����-------------------------------------------------------

		/// <summary>
		/// �f�o�b�O�Ńm�[�h�̃C���f�b�N�X��\������B
		/// </summary>
		/// <param name="node">�\���������m�[�h</param>
		/// <param name="description">������</param>
		void DebugNodeIndexDraw(const std::shared_ptr<NavGraphNode>& node, const std::wstring& description = L"");

		/// <summary>
		/// �f�o�b�O�ŁA�J�n�m�[�h�ƖڕW�m�[�h�̃C���f�b�N�X�\��������B
		/// </summary>
		void DebugStartAndEndIndexDraw(const std::shared_ptr<NavGraphNode>& startNode, const std::shared_ptr<NavGraphNode>& targetNode);

		/// <summary>
		/// �f�o�b�O�ŁA���[�g�C���f�b�N�X�̕\��
		/// </summary>
		void DebugRouteIndexDraw();

	};
}

//endbaecross