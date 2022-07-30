/*!
@file WaterAuraObject.h
@brief WaterAuraObjectなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "BulletBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 水圧ジェットのオーラオブジェクト
	//--------------------------------------------------------------------------------------
	class WaterAuraObject : public GameObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		WaterAuraObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};

}