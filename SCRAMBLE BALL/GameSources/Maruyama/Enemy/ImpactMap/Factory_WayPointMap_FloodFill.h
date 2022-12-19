
/*!
@file Factory_WayPointMap_FloodFill.h
@brief Factory_WayPointMap_FloodFill�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

#include "Maruyama/Enemy/Astar/AstarNode.h"
#include "Maruyama/Enemy/Astar/AstarEdge_Ex.h"
//#include "Maruyama/Patch/Graphs/SparseGraph_Ex.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	//class AstarGraph;
	//class GraphAstar;
	class ThreadPool;

	namespace maru {
		class NodeBase;
		class EdgeBase;
		class AstarNode;
		class AstarEdge;

		//template<class NodeType, class EdgeType>
		//class SparseGraph;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		> = nullptr>
		class SparseGraph;

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y��
		//--------------------------------------------------------------------------------------
		class Factory_WayPointMap_FloodFill
		{
		public:
			//using�f�B���N�e�B�u
			using GraphType = SparseGraph<AstarNode, AstarEdge>;

			/// <summary>
			/// �������̐i�s�^�C�v
			/// </summary>
			enum class DirectionType {
				Right,
				RightForward,
				RightBack,
				Left,
				LeftForward,
				LeftBack,
				Foward,
				Back,
			};

			/// <summary>
			/// �����^�C�v�ʃf�[�^
			/// </summary>
			struct DataByDirectionType {
				Vec3 direction;	//����
				int plusIndex;	//���Z����m�[�h�C���f�b�N�X
			};

			/// <summary>
			/// �����p�̃f�[�^
			/// </summary>
			struct OpenData
			{
				std::weak_ptr<AstarNode> parentNode;	//�����̑O�̃m�[�h
				std::shared_ptr<AstarNode> selfNode;	//�������g�̃m�[�h
				bool isActive;							//�m�[�h�������Ă��邩�ǂ���

				OpenData(
					const std::shared_ptr<AstarNode>& parentNode,
					const std::shared_ptr<AstarNode>& selfNode	
				);
			};

			/// <summary>
			/// �p�����[�^
			/// </summary>
			struct Parametor {
				float intervalRange = 5.0f;		//�m�[�h�̊Ԋu����
				maru::Rect rect;				//�l�p�f�[�^
				float createHeight = 0.5f;		//�����ݒ�
			};

		private:
			std::weak_ptr<Stage> m_stage;		//�X�e�[�W

			mutable std::mutex m_mutex{};		//�~���[�e�b�N�X

			std::queue<std::shared_ptr<OpenData>> m_openDataQueue;				//�I�[�v���f�[�^�L���[
			std::unordered_map<DirectionType, int> m_plusIndexMapByDirection;	//�����ʂ̉��Z����C���f�b�N�X��

		public:
			Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage);

		private:
			/// <summary>
			/// �����f�[�^�ɍ��킹���C���f�b�N�X�̏����ݒ肷��B
			/// </summary>
			/// <param name="parametor">�p�����[�^</param>
			/// <returns>�����f�[�^�ɍ��킹���C���f�b�N�X�̏����ݒ肷��</returns>
			std::unordered_map<DirectionType, int> SettingIndexByDirection(const Parametor& parametor);

			/// <summary>
			/// ����WayPoint�������ł��邩�ǂ����𔻒f����B
			/// </summary>
			/// <param name="newOpenData">�V�K�f�[�^</param>
			/// <param name="graph">�O���t</param>
			/// <param name="parametor">�����p�����[�^</param>
			/// <param name="isRayHit">��Q���ɓ����������ǂ������Q�Ƃ���bool�ɕۑ�����</param>
			/// <returns>�����ł���Ȃ�true</returns>
			bool IsNodeCreate(
				const std::shared_ptr<OpenData>& newOpenData,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor,
				bool& isRayHit
			);

			/// <summary>
			/// �G�b�W�������ł��邩�ǂ����𔻒f����B
			/// </summary>
			/// <param name="newOpenData">�V�K�f�[�^</param>
			/// <param name="graph">�O���t</param>
			/// <param name="parametor">�����p�����[�^</param>
			/// <param name="isRayHit">��Q���Ƀq�b�g�������ǂ���</param>
			/// <returns>�����ł���Ȃ�true</returns>
			bool IsEdgeCreate(
				const std::shared_ptr<OpenData>& newOpenData,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor,
				const bool isRayHit
			);

			/// <summary>
			/// �E�F�C�|�C���g�̕�������
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="parametor">�����p�����[�^</param>
			void CreateWayPoints(
				const std::shared_ptr<OpenData>& parentOpenData,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			/// <summary>
			/// �C���f�b�N�X���v�Z���ĕԂ�
			/// </summary>
			/// <param name="parentOpenData">�e�ƂȂ�I�[�v���f�[�^</param>
			/// <param name="directionType">������������f�[�^</param>
			/// <returns></returns>
			int CalculateIndex(const std::shared_ptr<OpenData>& parentOpenData, const DirectionType directionType) const;

			/// <summary>
			/// ��������OpenData�𐶐�����B
			/// </summary>
			/// <param name="parentOpenData">�e�ƂȂ�I�[�v���f�[�^</param>
			/// <param name="parametor">�p�����[�^</param>
			/// <returns>��������OpenData�𐶐�����</returns>
			std::vector<std::shared_ptr<OpenData>> CreateChildrenOpenDatas(
				const std::shared_ptr<OpenData>& parentOpenData,
				const Parametor& parametor
			);

			/// <summary>
			/// �}���`�X���b�h�Ő���(�t�ɒx���Ȃ�������g��Ȃ�)
			/// </summary>
			void MulchiThreadCretae();

		public:
			/// <summary>
			/// �E�F�C�|�C���g�̐���
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="graph">�����������O���t</param>
			/// <param name="parametor">�����p�����[�^</param>
			void AddWayPointMap(
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }
		};

	}
}