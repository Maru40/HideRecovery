
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

		//--------------------------------------------------------------------------------------
		///	Factory_�e���}�b�v_�t���b�h�t�B���A���S���Y��
		//--------------------------------------------------------------------------------------
		class Factory_WayPointMap_FloodFill
		{
			struct Parametor {
				float intervalRange = 5.0f;		//�m�[�h�̊Ԋu����
			};

			std::weak_ptr<Stage> m_stage;	//�X�e�[�W

			mutable std::mutex m_mutex{};	//�~���[�e�b�N�X

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
				const std::shared_ptr<GraphAstar>& graph,
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
				const std::shared_ptr<GraphAstar>& graph,
				const Parametor& parametor
			);

			std::shared_ptr<Stage> GetStage() const noexcept { return m_stage.lock(); }
		};

	}
}