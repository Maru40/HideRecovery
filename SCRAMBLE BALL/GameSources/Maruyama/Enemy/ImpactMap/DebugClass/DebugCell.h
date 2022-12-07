
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
		class ImpactCellMap;

		//--------------------------------------------------------------------------------------
		///	デバッグ表のCell
		//--------------------------------------------------------------------------------------
		class DebugCell : public Component
		{
			std::weak_ptr<ImpactCellMap> m_impactCellMap;

		public:
			DebugCell(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;
		};

	}
}