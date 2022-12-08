
/*!
@file FieldCellMap.cpp
@brief FieldCellMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "FieldCellMap.h"

#include "Cell.h"


namespace basecross {

	namespace maru {

		FieldCellMap::FieldCellMap(const std::shared_ptr<GameObject>& objPtr) :
			Component(objPtr)
		{}

		void FieldCellMap::OnCreate() {
			
		}

		void FieldCellMap::OnUpdate() {

		}

		void FieldCellMap::SetCellMap(const std::shared_ptr<CellMap>& cellMap) {
			m_cellMap = cellMap;
		}

		const std::shared_ptr<CellMap>& FieldCellMap::GetCellMap() const {
			return m_cellMap;
		}

	}
}