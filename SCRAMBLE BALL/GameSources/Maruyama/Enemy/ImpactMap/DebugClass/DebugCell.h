
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
		class Cell;

		//--------------------------------------------------------------------------------------
		///	デバッグ表のCell
		//--------------------------------------------------------------------------------------
		class DebugCellObject : public GameObject
		{
			std::weak_ptr<Cell> m_cell;	//デバッグ表示する対象のcell

		public:
			DebugCellObject(
				const std::shared_ptr<Stage>& stage,
				const std::shared_ptr<Cell>& cell
			);

			void OnCreate() override;
			void OnUpdate() override;
		};

	}
}