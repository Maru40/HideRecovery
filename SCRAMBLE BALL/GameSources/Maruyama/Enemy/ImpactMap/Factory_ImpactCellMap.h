/*!
@file Factory_ImpactCellMap.h
@brief Factory_ImpactCellMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Patch/CommonStructs/RectData.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	前方宣言
		//--------------------------------------------------------------------------------------
		class Cell;

		//--------------------------------------------------------------------------------------
		///	セルマップファクトリーのパラメータ
		//--------------------------------------------------------------------------------------
		struct Factory_ImpactCellMap_Parametor
		{
			Rect oneCellRect;		//一つのCellの生成情報。
			int widthCount;			//横に展開するセルの数。
			int depthCount;			//奥行きに展開するセルの数。
			Vec3 centerPosition;	//マップの中心位置。

			Factory_ImpactCellMap_Parametor();

			Factory_ImpactCellMap_Parametor(
				const Rect& oneCellRect,
				const int widthCount,
				const int depthCount,
				const Vec3& centerPosition
			);
		};

		//--------------------------------------------------------------------------------------
		///	セルマップファクトリー
		//--------------------------------------------------------------------------------------
		class Factory_ImpactCellMap
		{
		public:
			using Parametor = Factory_ImpactCellMap_Parametor;

		private:

		public:
			static std::vector<std::shared_ptr<Cell>> CreateCells(const Parametor& param);
		};

	}

}