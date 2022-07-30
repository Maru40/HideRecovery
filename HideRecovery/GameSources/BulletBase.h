/*!
@file BulletBase.h
@brief BulletBaseなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	弾丸の基底クラス
	//--------------------------------------------------------------------------------------
	class BulletBase : public Component
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		BulletBase(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// 撃つ処理
		/// </summary>
		/// <param name="direct">撃つ方向</param>
		virtual void Shot(const Vec3& direct) {}

		/// <summary>
		/// 撃つ処理が終了したタイミングで呼ぶ処理
		/// </summary>
		virtual void ShotEnd() {}
	};

}

//endbasecross