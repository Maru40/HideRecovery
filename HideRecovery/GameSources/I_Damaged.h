/*!
@file I_Damaged.h
@brief I_Damagedなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ダメージデータ
	//--------------------------------------------------------------------------------------
	struct DamageData
	{
		float value;      //ダメージ値
		Vec3 knockVec;    //ノックバック方向
		float knockPower; //ノックバックパワー
		float slowTime;   //遅速時間
		Vec3 hitPoint;    //当たった場所

		/// <summary>
		/// コンストラクタ
		/// </summary>
		DamageData();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="value">ダメージ値</param>
		DamageData(const float& value);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="value">ダメージ値</param>
		/// <param name="knockVec">ノックバック方向</param>
		/// <param name="knockPower">ノックバックパワー</param>
		/// <param name="slowTime">遅速時間</param>
		DamageData(const float& value, const Vec3& knockVec, const float& knockPower, const float& slowTime);

		DamageData operator+(const DamageData& data);

		DamageData operator+=(const DamageData& data);
	};

	class I_Damaged
	{
	public:
		/// <summary>
		/// ダメージを受ける処理
		/// </summary>
		/// <param name="data">ダメージデータ</param>
		virtual void Damaged(const DamageData& data) = 0;
	};

}

//endbasecorss