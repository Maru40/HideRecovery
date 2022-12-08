
/*!
@file DebugCell.h
@brief DebugCellなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class CellMap;

		//--------------------------------------------------------------------------------------
		///	デバッグ表のCell
		//--------------------------------------------------------------------------------------
		class DebugCellObject : public GameObject
		{

		public:
			DebugCellObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
			void OnUpdate() override;
		};

	}
}