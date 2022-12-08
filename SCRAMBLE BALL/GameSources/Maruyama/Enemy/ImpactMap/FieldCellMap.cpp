
/*!
@file FieldCellMap.cpp
@brief FieldCellMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FieldCellMap.h"

#include "Cell.h"
#include "CellMap.h"
#include "Factory_CellMap.h"

namespace basecross {

	namespace maru {

		FieldCellMap::FieldCellMap(const std::shared_ptr<GameObject>& objPtr) :
			Component(objPtr),
			m_isDebugDraw(true)
		{}

		void FieldCellMap::OnCreate() {
			m_cellMap = std::make_shared<CellMap>();

			auto param= Factory_CellMap::Parametor();
			auto cells = Factory_CellMap::CreateCells(param);

			m_cellMap->SetCells(cells);
		}

		void FieldCellMap::OnUpdate() {
			//デバッグ表示
			if (m_isDebugDraw) {
				DebugCellsDraw();
			}
		}

		void FieldCellMap::SetCellMap(const std::shared_ptr<CellMap>& cellMap) {
			m_cellMap = cellMap;
		}

		const std::shared_ptr<CellMap>& FieldCellMap::GetCellMap() const {
			return m_cellMap;
		}


		//--------------------------------------------------------------------------------------
		///	デバッグ
		//--------------------------------------------------------------------------------------

		void FieldCellMap::DebugCellsDraw() {
			auto cells = GetCellMap()->GetCells();

			for (auto& cell : cells) {
				cell->OnDebugDraw();
			}
		}
	}
}