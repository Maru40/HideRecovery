
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMap�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "ImpactCellMap.h"

#include "Cell.h"

namespace basecross {

	namespace maru {

		ImpactCellMap::ImpactCellMap()

		{}

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		void ImpactCellMap::SetCells(const std::vector<std::shared_ptr<Cell>>& cells) {
			m_cells = cells;
		}

		std::vector<std::shared_ptr<Cell>> ImpactCellMap::GetCells() {
			return m_cells;
		}

	}
}