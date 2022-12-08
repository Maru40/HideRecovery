/*!
@file Factory_CellMap.h
@brief Factory_CellMap�Ȃ�
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
		struct Factory_CellMap_Parametor
		{
			Rect oneCellRect;		//���Cell�̐������B
			int widthCount;			//���ɓW�J����Z���̐��B
			int depthCount;			//���s���ɓW�J����Z���̐��B
			Vec3 centerPosition;	//�}�b�v�̒��S�ʒu�B

			Factory_CellMap_Parametor();

			Factory_CellMap_Parametor(
				const Rect& oneCellRect,
				const int widthCount,
				const int depthCount,
				const Vec3& centerPosition
			);
		};

		//--------------------------------------------------------------------------------------
		///	�Z���}�b�v�t�@�N�g���[
		//--------------------------------------------------------------------------------------
		class Factory_CellMap
		{
		public:
			using Parametor = Factory_CellMap_Parametor;

		private:

		public:
			/// <summary>
			/// �Z���̐���
			/// </summary>
			/// <param name="param">�p�����[�^</param>
			/// <returns>�������ꂽ�Z���z��</returns>
			static std::vector<std::shared_ptr<Cell>> CreateCells(const Parametor& param);
		};

	}

}