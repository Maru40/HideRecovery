
/*!
@file ImpactCellMap.cpp
@brief ImpactCellMapのクラス実体
担当：丸山裕喜
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
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		void ImpactCellMap::SetCells(const std::vector<std::shared_ptr<Cell>>& cells) {
			m_cells = cells;
		}

		std::vector<std::shared_ptr<Cell>> ImpactCellMap::GetCells() {
			return m_cells;
		}

	}
}