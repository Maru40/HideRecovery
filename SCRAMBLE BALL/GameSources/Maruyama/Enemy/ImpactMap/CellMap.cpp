
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMapのクラス実体
担当：丸山裕喜
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
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void CellMap::SetCells(const std::vector<std::shared_ptr<Cell>>& cells) {
			m_cells = cells;
		}

		std::vector<std::shared_ptr<Cell>> CellMap::GetCells() {
			return m_cells;
		}

	}
}