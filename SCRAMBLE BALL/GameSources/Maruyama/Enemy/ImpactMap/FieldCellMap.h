
/*!
@file FieldCellMap.h
@brief FieldCellMapなど
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
		///	フィールド用のセルマップ
		//--------------------------------------------------------------------------------------
		class FieldCellMap : public Component
		{
		public:
			FieldCellMap(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;

			
		};

	}
}