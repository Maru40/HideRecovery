/*!
@file BoostCenterObject.h
@brief BoostCenterObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ブーストの中心オブジェクト
	//--------------------------------------------------------------------------------------
	class BoostCenterObject : public GameObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		BoostCenterObject(const std::shared_ptr<Stage>& stage);

		void OnCreate();
	};

}