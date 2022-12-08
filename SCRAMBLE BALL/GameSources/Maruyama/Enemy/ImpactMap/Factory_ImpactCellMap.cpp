

/*!
@file Factory_ImpactCellMap.cpp
@brief Factory_ImpactCellMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Factory_ImpactCellMap.h"

#include "Cell.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�t�@�N�g���[�̃p�����[�^
		//--------------------------------------------------------------------------------------
		
		Factory_ImpactCellMap_Parametor::Factory_ImpactCellMap_Parametor():
			Factory_ImpactCellMap_Parametor(
				Rect(Vec3(0.0f), 2.0f, 2.0f),	//��̃Z�����\������RectData
				5,								//widthCount
				5,								//depthCount
				Vec3(0.0f, 0.0f, 0.0f)			//centerPosition
			)
		{}

		Factory_ImpactCellMap_Parametor::Factory_ImpactCellMap_Parametor(
			const Rect& oneCellRect,
			const int widthCount,
			const int depthCount,
			const Vec3& centerPosition
		):
			oneCellRect(oneCellRect),
			widthCount(widthCount),
			depthCount(depthCount),
			centerPosition(centerPosition)
		{}

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�t�@�N�g���[
		//--------------------------------------------------------------------------------------

		std::vector<std::shared_ptr<Cell>> Factory_ImpactCellMap::CreateCells(const Parametor& param) {
			std::vector<std::shared_ptr<Cell>> result;

			const float halfWidthCount = param.widthCount * 0.5f;
			const float halfDepthCount = param.depthCount * 0.5f;

			auto fieldRect = Rect(param.centerPosition, (float)param.widthCount, (float)param.depthCount);
			auto startPosition = fieldRect.CalculateStartPosition();				//�}�b�v�̍���̌��_���擾

			//���[�v���Đ���
			for (int i = 0 ; i < param.depthCount ; i++) {
				float depth = startPosition.z + (param.oneCellRect.depth * i);		//�c�ʒu�̎�������
				for (int j = 0; j < param.widthCount; j++) {
					float width = startPosition.x + (param.oneCellRect.width * j);	//���ʒu�̎�������
					auto position = Vec3(width, param.centerPosition.y, depth);

					//Cell�̃p�����[�^��ݒ�
					auto cellParam = Cell::Parametor(param.oneCellRect);	
					auto cell = std::make_shared<Cell>(cellParam);			//Cell����
					cell->SetPosition(position);							//Cell�̈ʒu�ύX

					result.push_back(cell);	//result�ɒǉ�
				}
			}

			return result;
		}

	}
}