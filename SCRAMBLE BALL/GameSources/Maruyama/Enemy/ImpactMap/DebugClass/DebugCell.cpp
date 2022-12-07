
/*!
@file DebugCell.cpp
@brief DebugCellのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "DebugCell.h"

namespace basecross {

	namespace maru {

		DebugCell::DebugCell(const std::shared_ptr<GameObject>& objPtr) :
			Component(objPtr)
		{}

		void DebugCell::OnCreate() {

		}

		void DebugCell::OnUpdate() {

		}

	}
}