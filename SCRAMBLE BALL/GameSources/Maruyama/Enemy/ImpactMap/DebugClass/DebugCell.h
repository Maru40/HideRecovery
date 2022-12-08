
/*!
@file DebugCell.h
@brief DebugCell�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class Cell;

		//--------------------------------------------------------------------------------------
		///	�f�o�b�O�\��Cell(���ݎg�p�֎~)
		//--------------------------------------------------------------------------------------
		class DebugCellObject : public GameObject
		{
			std::weak_ptr<Cell> m_cell;	//�f�o�b�O�\������Ώۂ�cell

		public:
			DebugCellObject(
				const std::shared_ptr<Stage>& stage,
				const std::shared_ptr<Cell>& cell
			);

			void OnCreate() override;
			void OnUpdate() override;
		};

	}
}