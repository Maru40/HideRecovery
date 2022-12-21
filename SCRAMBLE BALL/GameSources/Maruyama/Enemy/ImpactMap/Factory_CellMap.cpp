

/*!
@file Factory_CellMap.cpp
@brief Factory_CellMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Factory_CellMap.h"

#include "Cell.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�t�@�N�g���[�̃p�����[�^
		//--------------------------------------------------------------------------------------
		
		Factory_CellMap_Parametor::Factory_CellMap_Parametor():
			Factory_CellMap_Parametor(
				Rect(Vec3(0.0f), 2.0f, 2.0f),	//��̃Z�����\������RectData
				6,								//widthCount
				4,								//depthCount
				Vec3(0.0f, 0.0f, 0.0f)			//centerPosition
			)
		{}

		Factory_CellMap_Parametor::Factory_CellMap_Parametor(
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

		std::vector<std::shared_ptr<Cell>> Factory_CellMap::CreateCells(const Parametor& param) {
			std::vector<std::shared_ptr<Cell>> result;

			const float halfWidthCount = param.widthCount * 0.5f;
			const float halfDepthCount = param.depthCount * 0.5f;

			const float halfOneRectWidth = param.oneCellRect.width * 0.5f;
			const float halfOneRectDepth = param.oneCellRect.depth * 0.5f;

			const float fieldRectWidth = (float)param.widthCount * param.oneCellRect.width;
			const float fieldRectDepth = (float)param.depthCount * param.oneCellRect.depth;
			auto fieldRect = Rect(param.centerPosition, fieldRectWidth, fieldRectDepth);
			auto startPosition = fieldRect.CalculateStartPosition();				//�}�b�v�̍���̌��_���擾

			//���[�v���Đ���
			for (int i = 0 ; i < param.depthCount ; i++) {
				//�c�ʒu�̎�������
				float depth = startPosition.z + (param.oneCellRect.depth * i);
				depth += halfOneRectDepth;

				Debug::GetInstance()->Log(L"��" + std::to_wstring(i) + L"��");
				for (int j = 0; j < param.widthCount; j++) {
					//���ʒu�̎�������
					float width = startPosition.x + (param.oneCellRect.width * j);	
					width += halfOneRectWidth;

					auto position = Vec3(width, param.centerPosition.y, depth);	//�ʒu���̌���

					//Cell�̃p�����[�^��ݒ�
					auto cellParam = Cell::Parametor(param.oneCellRect);	
					auto cell = std::make_shared<Cell>(cellParam);			//Cell����
					cell->SetPosition(position);							//Cell�̈ʒu�ύX

					Debug::GetInstance()->Log(position);

					result.push_back(cell);	//result�ɒǉ�
				}
			}

			return result;
		}

	}
}