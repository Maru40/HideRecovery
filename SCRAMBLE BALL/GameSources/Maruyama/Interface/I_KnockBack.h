/*!
@file I_KnockBack.h
@brief I_KnockBackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ノックバックデータ
	//--------------------------------------------------------------------------------------
	struct KnockBackData {
		float power;   //ノックバック力
		Vec3 hitPoint; //衝突位置
		Vec3 direct;   //ノックバック方向

		/// <summary>
		/// コンストラクタ
		/// </summary>
		KnockBackData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="power">ノックバック力</param>
		/// <param name="hitPoint">衝突位置</param>
		/// <param name="direct">ノックバック方向</param>
		KnockBackData(const float& power, const Vec3& hitPoint, const Vec3& direct);
	};

	//--------------------------------------------------------------------------------------
	/// ノックバックインターフェース
	//--------------------------------------------------------------------------------------
	class I_KnockBack
	{
	public:
		/// <summary>
		/// ノックバックされる処理
		/// </summary>
		/// <param name="data">ノックバックデータ</param>
		virtual void KnockBack(const KnockBackData& data) = 0;

	};

}

//endbasecross