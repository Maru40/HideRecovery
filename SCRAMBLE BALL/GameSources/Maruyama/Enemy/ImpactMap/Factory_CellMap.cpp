

/*!
@file Factory_CellMap.cpp
@brief Factory_CellMapのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Factory_CellMap.h"

#include "Cell.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	セルマップファクトリーのパラメータ
		//--------------------------------------------------------------------------------------
		
		Factory_CellMap_Parametor::Factory_CellMap_Parametor():
			Factory_CellMap_Parametor(
				Rect(Vec3(0.0f), 2.0f, 2.0f),	//一つのセルを構成するRectData
				6,								//widthCount
				4,								//depthCount
				Vec3(0.0f, 0.0f, 0.0f)			//centerPosition
			)
		{}

		Factory_CellMap_Parametor::Factory_CellMap_Parametor(
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

		std::vector<std::shared_ptr<Cell>> Factory_CellMap::CreateCells(const Parametor& param) {
			std::vector<std::shared_ptr<Cell>> result;

			const float halfWidthCount = param.widthCount * 0.5f;
			const float halfDepthCount = param.depthCount * 0.5f;

			const float halfOneRectWidth = param.oneCellRect.width * 0.5f;
			const float halfOneRectDepth = param.oneCellRect.depth * 0.5f;

			auto fieldRect = Rect(param.centerPosition, (float)param.widthCount * param.oneCellRect.width, (float)param.depthCount * param.oneCellRect.depth);
			auto startPosition = fieldRect.CalculateStartPosition();				//マップの左上の原点を取得

			//ループして生成
			for (int i = 0 ; i < param.depthCount ; i++) {
				//縦位置の軸を決定
				float depth = startPosition.z + (param.oneCellRect.depth * i);
				depth += halfOneRectDepth;

				Debug::GetInstance()->Log(L"★" + std::to_wstring(i) + L"★");
				for (int j = 0; j < param.widthCount; j++) {
					//横位置の軸を決定
					float width = startPosition.x + (param.oneCellRect.width * j);	
					width += halfOneRectWidth;

					auto position = Vec3(width, param.centerPosition.y, depth);	//位置情報の決定

					//Cellのパラメータを設定
					auto cellParam = Cell::Parametor(param.oneCellRect);	
					auto cell = std::make_shared<Cell>(cellParam);			//Cell生成
					cell->SetPosition(position);							//Cellの位置変更

					Debug::GetInstance()->Log(position);

					result.push_back(cell);	//resultに追加
				}
			}

			return result;
		}

	}
}