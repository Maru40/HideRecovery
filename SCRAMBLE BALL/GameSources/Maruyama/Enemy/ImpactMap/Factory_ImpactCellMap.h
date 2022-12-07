/*!
@file Factory_ImpactCellMap.h
@brief Factory_ImpactCellMap�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class Cell;

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�t�@�N�g���[�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct Factory_ImpactCellMap_Parametor
		{
			Rect oneCellRect;		//���Cell�̐������B
			int widthCount;			//���ɓW�J����Z���̐��B
			int depthCount;			//���s���ɓW�J����Z���̐��B
			Vec3 centerPosition;	//�}�b�v�̒��S�ʒu�B

			Factory_ImpactCellMap_Parametor();

			Factory_ImpactCellMap_Parametor(
				const Rect& oneCellRect,
				const int widthCount,
				const int depthCount,
				const Vec3& centerPosition
			);
		};

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�t�@�N�g���[
		//--------------------------------------------------------------------------------------
		class Factory_ImpactCellMap
		{
		public:
			using Parametor = Factory_ImpactCellMap_Parametor;

		private:

		public:
			static std::vector<std::shared_ptr<Cell>> CreateCells(const Parametor& param);
		};

	}

}