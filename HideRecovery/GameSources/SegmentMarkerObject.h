/*!
@file SegmentMarkerObject.h
@brief SegmentMarkerObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BoardObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Builder {
		struct VertexPCTParametor;
	}

	//--------------------------------------------------------------------------------------
	/// セグメントマーカーオブジェクト
	//--------------------------------------------------------------------------------------
	class SegmentMarkerObject : public BoardObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		SegmentMarkerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}