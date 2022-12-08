
/*!
@file DebugCell.cpp
@brief DebugCellのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "DebugCell.h"

#include "Maruyama/DebugClass/Component/RectDraw.h"

namespace basecross {

	namespace maru {

		DebugCellObject::DebugCellObject(
			const std::shared_ptr<Stage>& stage,
			const std::shared_ptr<Cell>& cell
		) :
			GameObject(stage),
			m_cell(cell)
		{}

		void DebugCellObject::OnCreate() {
			
		}

		void DebugCellObject::OnUpdate() {

		}

	}
}