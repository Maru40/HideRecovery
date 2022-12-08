
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "CellMap.h"

#include "Cell.h"

namespace basecross {

	namespace maru {

		CellMap::CellMap()

		{}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void CellMap::SetCells(const std::vector<std::shared_ptr<Cell>>& cells) {
			m_cells = cells;
		}

		std::vector<std::shared_ptr<Cell>> CellMap::GetCells() {
			return m_cells;
		}

	}
}