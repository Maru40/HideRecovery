
/*!
@file ImpactMap.h
@brief ImpactMapなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	四角形データ(生成する範囲に利用)
		//--------------------------------------------------------------------------------------
		struct Rect
		{
			Vec3 startPosition;   //開始場所
			Vec3 centerPosition;  //中心位置
			float width;          //横のサイズ
			float depth;          //奥行のサイズ

			Rect();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="centerPosition">中心位置</param>
			/// <param name="width">横の長さ</param>
			/// <param name="depth">奥行の長さ</param>
			Rect(const Vec3& centerPosition, const float& width, const float& depth);

			/// <summary>
			/// 開始位置を取得
			/// </summary>
			/// <returns>開始位置</returns>
			Vec3 CalculateStartPosition() const;

			/// <summary>
			/// 四角形内にいるかどうか
			/// </summary>
			/// <returns>四角形内ならtrue</returns>
			bool IsInRect(const Vec3& position);
		};

	}
}