/*!
@file SuccessMarkerObject.h
@brief SuccessMarkerObjectなど
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
	/// 成功時マーカーオブジェクト
	//--------------------------------------------------------------------------------------
	class SuccessMarkerObject : public BoardObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		SuccessMarkerObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;

	};

}