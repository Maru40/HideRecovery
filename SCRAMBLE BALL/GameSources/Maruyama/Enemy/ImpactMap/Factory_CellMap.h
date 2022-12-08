/*!
@file Factory_CellMap.h
@brief Factory_CellMapなど
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
		struct Factory_CellMap_Parametor
		{
			Rect oneCellRect;		//一つのCellの生成情報。
			int widthCount;			//横に展開するセルの数。
			int depthCount;			//奥行きに展開するセルの数。
			Vec3 centerPosition;	//マップの中心位置。

			Factory_CellMap_Parametor();

			Factory_CellMap_Parametor(
				const Rect& oneCellRect,
				const int widthCount,
				const int depthCount,
				const Vec3& centerPosition
			);
		};

		//--------------------------------------------------------------------------------------
		///	セルマップファクトリー
		//--------------------------------------------------------------------------------------
		class Factory_CellMap
		{
		public:
			using Parametor = Factory_CellMap_Parametor;

		private:

		public:
			/// <summary>
			/// セルの生成
			/// </summary>
			/// <param name="param">パラメータ</param>
			/// <returns>生成されたセル配列</returns>
			static std::vector<std::shared_ptr<Cell>> CreateCells(const Parametor& param);
		};

	}

}