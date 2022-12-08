

/*!
@file Factory_ImpactCellMap.cpp
@brief Factory_ImpactCellMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Factory_ImpactCellMap.h"

#include "Cell.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	セルマップファクトリーのパラメータ
		//--------------------------------------------------------------------------------------
		
		Factory_ImpactCellMap_Parametor::Factory_ImpactCellMap_Parametor():
			Factory_ImpactCellMap_Parametor(
				Rect(Vec3(0.0f), 2.0f, 2.0f),	//一つのセルを構成するRectData
				5,								//widthCount
				5,								//depthCount
				Vec3(0.0f, 0.0f, 0.0f)			//centerPosition
			)
		{}

		Factory_ImpactCellMap_Parametor::Factory_ImpactCellMap_Parametor(
			const Rect& oneCellRect,
			const int widthCount,
			const int depthCount,
			const Vec3& centerPosition
		):
			oneCellRect(oneCellRect),
			widthCount(widthCount),
			depthCount(depthCount),
			centerPosition(centerPosition)
		{}

		//--------------------------------------------------------------------------------------
		///	セルマップファクトリー
		//--------------------------------------------------------------------------------------

		std::vector<std::shared_ptr<Cell>> Factory_ImpactCellMap::CreateCells(const Parametor& param) {
			std::vector<std::shared_ptr<Cell>> result;

			const float halfWidthCount = param.widthCount * 0.5f;
			const float halfDepthCount = param.depthCount * 0.5f;

			auto fieldRect = Rect(param.centerPosition, (float)param.widthCount, (float)param.depthCount);
			auto startPosition = fieldRect.CalculateStartPosition();				//マップの左上の原点を取得

			//ループして生成
			for (int i = 0 ; i < param.depthCount ; i++) {
				float depth = startPosition.z + (param.oneCellRect.depth * i);		//縦位置の軸を決定
				for (int j = 0; j < param.widthCount; j++) {
					float width = startPosition.x + (param.oneCellRect.width * j);	//横位置の軸を決定
					auto position = Vec3(width, param.centerPosition.y, depth);

					//Cellのパラメータを設定
					auto cellParam = Cell::Parametor(param.oneCellRect);	
					auto cell = std::make_shared<Cell>(cellParam);			//Cell生成
					cell->SetPosition(position);							//Cellの位置変更

					result.push_back(cell);	//resultに追加
				}
			}

			return result;
		}

	}
}