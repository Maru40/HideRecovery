
/*!
@file FieldCellMap.cpp
@brief FieldCellMap�̃N���X����
�S���F�ێR�T��
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
			//�Z���}�b�v�̐���
			m_cellMap = std::make_shared<CellMap>();

			//�Z���z��̐���
			auto param= Factory_CellMap::Parametor();
			auto cells = Factory_CellMap::CreateCells(param);

			//�Z���z��̐ݒ�
			m_cellMap->SetCells(cells);
		}

		void FieldCellMap::OnUpdate() {
			//�f�o�b�O�\��
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
		///	�f�o�b�O
		//--------------------------------------------------------------------------------------

		void FieldCellMap::DebugCellsDraw() {
			auto cells = GetCellMap()->GetCells();

			for (auto& cell : cells) {
				cell->OnDebugDraw();
			}
		}
	}
}