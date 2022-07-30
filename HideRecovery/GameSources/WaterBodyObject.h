/*!
@file WaterBodyObject.h
@brief WaterBodyObject
担当：丸山 裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 水圧ジェットオブジェクト
	//--------------------------------------------------------------------------------------
	class WaterBodyObject : public GameObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		WaterBodyObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}