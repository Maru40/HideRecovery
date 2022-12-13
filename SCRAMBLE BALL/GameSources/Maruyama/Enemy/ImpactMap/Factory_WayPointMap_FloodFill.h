
/*!
@file Factory_WayPointMap_FloodFill.h
@brief Factory_WayPointMap_FloodFill�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class AstarGraph;
	class GraphAstar;

	namespace maru {
		class NodeBase;
		class EdgeBase;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		> = nullptr>
		class SparceGraph;

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y��
		//--------------------------------------------------------------------------------------
		class Factory_WayPointMap_FloodFill
		{
		public:
			//using�f�B���N�e�B�u
			using GraphType = SparceGraph<NodeBase, EdgeBase>;

			struct Parametor {
				float intervalRange = 5.0f;		//�m�[�h�̊Ԋu����
			};

		private:

			std::weak_ptr<Stage> m_stage;		//�X�e�[�W

			mutable std::mutex m_mutex{};		//�~���[�e�b�N�X

		public:

			Factory_WayPointMap_FloodFill(const std::shared_ptr<Stage>& stage);

		private:
			/// <summary>
			/// ����WayPoint�������ł��邩�ǂ����𔻒f����B
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="targetPosition">�����������ʒu</param>
			/// <param name="parametor">�����p�����[�^</param>
			bool IsCreate(const Vec3& startPosition, const Vec3& targetPosition, const Parametor& parametor);

			/// <summary>
			/// �J�n�ʒu���甪�����̒��ŃE�F�C�|�C���g�𐶐��ł���ꏊ��Ԃ��B
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="parametor">�����p�����[�^</param>
			/// <returns>�J�n�ʒu���甪�����̒��ŃE�F�C�|�C���g�𐶐��ł���ꏊ�̔z��</returns>
			std::vector<Vec3> CalculationTargetPositions(const Vec3& startPosition, const Parametor& parametor);

			/// <summary>
			/// �E�F�C�|�C���g�̕�������
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="parametor">�����p�����[�^</param>
			void CreateWayPoints(
				const Vec3& startPosition,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

		public:
			/// <summary>
			/// �E�F�C�|�C���g�̐���
			/// </summary>
			/// <param name="startPosition">�J�n�ʒu</param>
			/// <param name="graph">�����������O���t</param>
			/// <param name="parametor">�����p�����[�^</param>
			void AddWayPointMap(
				const Vec3& baseStartPosition,
				const std::shared_ptr<GraphType>& graph,
				const Parametor& parametor
			);

			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }
		};

	}
}